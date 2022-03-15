//
// Created by alen_ on 2021-05-17.
//

#include "rhythm.h"
/*
// Debug dependencies
#include "sciTinyTimber.h"
#include <stdio.h>

extern Serial sci0;
 */


void setTempo(Rhythm *self, int bpm){
	if(bpm >= 60 && bpm <= 240){
		self->tempo = bpm;
		// Converts value from BPM to milliseconds
		self->beatLength = 1000/(bpm/60);
	}
}

int getTempo(Rhythm *self, int unused){
    return self->tempo;
}

void moveKeyOrigin(Rhythm *self, int modifier){
    if(self->keyModifier + modifier < 6 && self->keyModifier + modifier > -6){
        self->keyModifier += modifier;
    }
}

void setKeyMod(Rhythm *self, int value){
	if(value >= -5 && value <= 5){
		self->keyModifier = value;
	}
}

void changeFrequency(Rhythm *self, int noteIndex){
    SYNC(self->channel, setInterval, self->periods[noteIndex + 10]);
}

void nextBeat(Rhythm *self, int volume){
    SYNC(self->channel, setVolume, volume);
    int beat = self->beatLength;

    switch (self->beats[self->index]) {
        case 'b':
            beat *= 2;
            break;
        case 'c':
            beat /= 2;
            break;
    }

    bool shortGap = false;
    if(beat - 100 < 50) {
        beat -= 50;
        shortGap = true;
    }
    else{
        beat -= 100;
    }
	
	/*
	// Debug code logging
	char buffer[50];
	snprintf(buffer, 50, "\n[%d %c %d]", self->index, self->beats[self->index], self->keys[self->index] + self->keyModifier);
	SCI_WRITE(&sci0, buffer);
	 */
	
	
    self->index = (self->index + 1) % self->length;
    SEND(MSEC(beat), USEC(200), self, silenceBeat, shortGap);
}

void silenceBeat(Rhythm *self, int shortGap){
    int volume = SYNC(self->channel, getVolume, 0);
    BEFORE(USEC(100), self->channel, setVolume, 0);
    SEND(USEC(100), USEC(100), self, changeFrequency, self->keys[self->index] + self->keyModifier);
    int gap = 100;
    if(shortGap){
        gap = 50;
    }
    SEND(MSEC(gap), USEC(100), self, nextBeat,volume);
}