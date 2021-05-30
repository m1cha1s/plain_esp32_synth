#ifndef NOTES_H
#define NOTES_H

#include "lookuptables.hpp"
#include "oscillators.hpp"

class Basic_note : public Oscillator {
  public :

    float velocity = 0;
    float pitch = 0;

    inline void set_note(char note, float vel) {
      velocity = vel;
      pitch = noteToFreq[note];
      set_frequency(pitch);
    }

    inline float get_sample() {
        return Oscillator::get_sample() * velocity;
    }
};

#endif