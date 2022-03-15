//
// Created by alen_ on 2021-05-17.
//

#ifndef _PART_2_BASE_SOUND_H
#define _PART_2_BASE_SOUND_H

#include "TinyTimber.h"
#include <stdbool.h>

// A pointer to the DAC register
#define DAC_REGISTER ((unsigned char *) 0x4000741C)
// Initializes a sound object with default values
// Such as the frequency of 1kHz and the volume of 5
#define initSoundChannel(intervalMS) {initObject(), DAC_REGISTER, USEC(intervalMS), 0, 0, false}

// Struct for playing a certain frequency in a channel
typedef struct {
    Object  super;
    unsigned char *channel;
    Time interval;
    unsigned int volume;
    unsigned int output;
    bool isMuted;
} Sound;

// Returns the current volume of a sound channel
unsigned int getVolume(Sound *self, int unused);

// Sets the volume in a sound channel
void setVolume(Sound *self, int value);

// Alter the volume by adding the modifier to it
// as long as the result isn't below 0 or greater than 6
void changeVolume(Sound *self, int modifier);

// Toggles a channel between mute and it's current volume
void toggleVolume(Sound *self, int unused);

// Set the interval that the object runs on
void setInterval(Sound *self, int microseconds);

// This function toggles a value in the DAC recursively
void playSound(Sound *self, int unused);

#endif //_PART_2_BASE_SOUND_H
