#ifndef NOTES_H
#define NOTES_H

#include "lookuptables.hpp"
#include "oscillators.hpp"

class Note {
  public :

    char channel = 0;
    char note = 0;
    float velocity = 0;
    float pitch = 0;

    inline bool check(char ch, char nt) {
      return (channel == ch && pitch == noteToFreq[nt]);
    }

    inline void set_note(char ch, char nt, float vel) {
      channel = ch;
      velocity = vel;
      pitch = noteToFreq[nt];
      note = nt;
    }

    inline float process(float sample) {
        return sample * velocity;
    }
};

#endif