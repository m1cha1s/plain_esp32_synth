#ifndef VOICES_H
#define VOICES_H

#include "envelopes.hpp"

class Basic_voice : public Basic_ADSR {
  public:
    
};

typedef struct {

  

  char channel = 0;
  float *waveform = wavetables[0];
  Note note;
  ADSR env;
}Voice;

inline float do_voice(Voice *voice) {
  int integral = voice->note.phase;
  float fractional = voice->note.phase-integral;
  float y0 = voice->waveform[integral], y1 = voice->waveform[integral+1], y2 = voice->waveform[integral+2], y3 = voice->waveform[integral+3];
  float sample = (CubicInterpolate(y0, y1, y2, y3,fractional))*voice->note.velocity*voice->env.envelope;
  voice->note.phase += voice->note.step;
  if(voice->note.phase > WAVETABLE_LEN-1.0) voice->note.phase -= WAVETABLE_LEN-1.0;
  return sample;
}

#endif