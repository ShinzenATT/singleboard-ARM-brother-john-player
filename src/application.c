#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "sound.h"
#include "rhythm.h"

typedef struct {
    Object super;
    int count;
    char c;
	char buffer[20];
	int index;
} App;

App app = { initObject(), 0, 'X', {'\0'}, 0};

// Initialise needed objects for playing sound
Sound sound = initSoundChannel(500);
Rhythm rhythm = initRhythmObj(&sound);

void reader(App*, int);
void receiver(App*, int);


Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
}

void reader(App *self, int c) {
	char outBuffer[50];
	int parameter = 0;
	
	switch (c) {
            // Mute or unmute
        case 'm':
            SYNC(&sound, toggleVolume, 0);
            SCI_WRITE(&sci0,"\n\nMute toggled");
			
			// Restore context
			snprintf(outBuffer, 50, "\n\nBJ>%s", self->buffer);
			SCI_WRITE(&sci0, outBuffer);
            break;
            // Volume up
        case 'n':
            SYNC(&sound, changeVolume, 1);
			
            snprintf(
                    outBuffer, 50, "\n\nThe volume is %d", 
                    SYNC(&sound, getVolume, 0)
                    );
            SCI_WRITE(&sci0, outBuffer);
			 
			 // Restore context
			snprintf(outBuffer, 50, "\n\nBJ>%s", self->buffer);
			SCI_WRITE(&sci0, outBuffer);
            break;
            // Volume down
        case 'b':
            SYNC(&sound, changeVolume, -1);
			
            snprintf(
                    outBuffer, 50, "\n\nThe volume is %d",
                    SYNC(&sound, getVolume, 0)
            );
            SCI_WRITE(&sci0, outBuffer);
			 
			 // Restore context
			snprintf(outBuffer, 50, "\n\nBJ>%s", self->buffer);
			SCI_WRITE(&sci0, outBuffer);
            break;
            // Change key
        case 'k':
			self->buffer[self->index] = '\0';
            parameter = atoi(self->buffer);
            self->buffer[0] = '\0';
            self->index = 0;
			SCI_WRITE(&sci0, " [Key changed] \n\n");
            SCI_WRITE(&sci0, "BJ>");
			
            SYNC(&rhythm, setKeyMod, parameter);
            break;
            // Change tempo
        case 't':
            self->buffer[self->index] = '\0';
            parameter = atoi(self->buffer);
            self->buffer[0] = '\0';
            self->index = 0;
			SCI_WRITE(&sci0, " [Tempo changed] \n\n");
            SCI_WRITE(&sci0, "BJ>");
			
            SYNC(&rhythm, setTempo, parameter);
            break;
		case '-':
            if (self->index != 0)
                break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            SCI_WRITECHAR(&sci0, c);
            self->buffer[self->index++] = (char) c;
            break;
    }
}

void startApp(App *self, int arg) {
    CANMsg msg;

    CAN_INIT(&can0);
    SCI_INIT(&sci0);
    SCI_WRITE(&sci0, "Hello, hello...\n\n");
	SCI_WRITE(&sci0, "BJ>");
	

    SYNC(&rhythm, changeFrequency, 0);
	BEFORE(USEC(100), &rhythm, nextBeat, 5);

    msg.msgId = 1;
    msg.nodeId = 1;
    msg.length = 6;
    msg.buff[0] = 'H';
    msg.buff[1] = 'e';
    msg.buff[2] = 'l';
    msg.buff[3] = 'l';
    msg.buff[4] = 'o';
    msg.buff[5] = 0;
    CAN_SEND(&can0, &msg);
}

int main() {
    INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
	INSTALL(&can0, can_interrupt, CAN_IRQ0);
    TINYTIMBER(&app, startApp, 0);
    return 0;
}
