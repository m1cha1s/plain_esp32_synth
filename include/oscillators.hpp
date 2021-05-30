#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#include "config.hpp"
#include "globals.hpp"
#include "wavetables.hpp"

class Oscillator : {
  public:

    float step = 0;
    float phase = 0;

    float velocity = 0;

    float *waveform = wavetables[0];

    Oscillator(float *wf = wavetables[0]) {
      waveform = wf;
    }

    inline void set_waveform(float *wf) {
      waveform = wf;
    }

    inline void set_frequency(float frequency) {
      step = (WAVETABLE_LEN/SAMPLE_RATE) * frequency;
    }

    inline float get_sample() {
      int integral = phase;
      float fractional = phase-integral;
      float y0 = waveform[integral], y1 = waveform[integral+1], y2 = waveform[integral+2], y3 = waveform[integral+3];
      float sample = CubicInterpolate(y0, y1, y2, y3,fractional);
      phase += step;
      if(phase > WAVETABLE_LEN-1.0) phase -= WAVETABLE_LEN-1.0;
      return sample;
    }
};

typedef struct {
  
}Note;

#endif