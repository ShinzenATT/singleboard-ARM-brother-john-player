//
// Created by alen_ on 2021-05-17.
//

#ifndef _PART_2_BASE_RHYTHM_H
#define _PART_2_BASE_RHYTHM_H

#include <stdbool.h>
#include "TinyTimber.h"
#include "sound.h"

#define songLength 32

// Initializes a rhythm object with default values for brother Jakob
// such as the beats, tones and 120 bpm tempo
#define initRhythmObj(soundPointer) \
{ initObject(), \
{'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'b', 'a', 'a', 'b', 'c', 'c', 'c', 'c', 'a', 'a', 'c', 'c', 'c', 'c', 'a', 'a', 'a', 'a', 'b', 'a', 'a', 'b'}, \
{0, 2, 4, 0, 0, 2, 4, 0, 4, 5, 7, 4, 5, 7, 7, 9, 7, 5, 4, 0, 7, 9, 7, 5, 4, 0, 0, -5, 0, 0, -5, 0}, \
{2025, 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, 956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506},\
0, songLength, 120, 500, 0, (soundPointer)}

// An object that keeps track of the beats for a sound object
typedef struct {
    Object super;
    char beats[songLength];
    int keys[songLength];
    int periods[25];
    int index;
    int length;
    int tempo;
    int beatLength;
    int keyModifier;
    Sound *channel;
} Rhythm;

// Sets the tempo which determines the beat length
void setTempo(Rhythm *self, int bpm);

// Get the current tempo
int getTempo(Rhythm *self, int unused);

// Alters the pitch by transposing the keys
// The modifiers adds to the value as long as the result is within -5 to 5
void moveKeyOrigin(Rhythm *self, int modifier);

// Sets the pitch and transposes the keys
// Will set along as arguments is within -5 to 5
void setKeyMod(Rhythm *self, int value);

// Picks the next tone and changes the the frequency in the sound channel
void changeFrequency(Rhythm *self, int noteIndex);

// Starts the next the beat and later on summons the silent gap
void nextBeat(Rhythm *self, int volume);

// Starts the silent gap and prepares the next note.
// When reaching the end of the gap, the next beat is called.
void silenceBeat(Rhythm *self, int shortGap);

#endif //_PART_2_BASE_RHYTHM_H
