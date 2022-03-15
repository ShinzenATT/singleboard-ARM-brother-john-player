//
// Created by alen_ on 2021-05-17.
//

#include "sound.h"

unsigned int getVolume(Sound *self, int unused){
    return self->volume;
}

void setVolume(Sound *self, int value){
    if(value < 7 && value >= 0){
        int temp = self->volume;
        self->volume = value;

        if(temp == 0 && value != 0){
            BEFORE(USEC(100), self, playSound, 0);
        }
    }
}

void toggleVolume(Sound *self, int unused){
    if(self->isMuted){
        self->isMuted = false;
    } else{
        self->isMuted = true;
    }
}

void changeVolume(Sound *self, int modifier){
    // Change the volume if it is in bounds
    if(self->volume + modifier > 0 && self->volume + modifier < 7){
        // If the sound has stopped, attempt to change the volume later instead
        if(self->volume == 0){
            SEND(MSEC(100), USEC(100), self, changeVolume, modifier);
            return;;
        }
        self->volume += modifier;
    }
}

void setInterval(Sound *self, int microseconds){
    self->interval = USEC(microseconds);
}

void playSound(Sound *self, int unused){
    if(self->output > 0 || self->isMuted){
        self->output = 0;
    } else{
        self->output = self->volume;
    }

    *(self->channel) = self->output;

    // Continue playing the sound unless the volume is 0
    if(self->volume != 0) {
        SEND(self->interval, USEC(100), self, playSound, unused);
    }
}