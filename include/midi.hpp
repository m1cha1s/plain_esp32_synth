#ifndef MIDI_HPP
#define MIDI_HPP

#include <Arduino.h>
#include <MIDI.h>
#include "globals.hpp"
#include "config.hpp"
#include "functions.hpp"
#include "interpolators.hpp"
#include "lookuptables.hpp"
#include "wavetables.hpp"

using Transport = MIDI_NAMESPACE::SerialMIDI<HardwareSerial>;
Transport serialMIDI(Serial2);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport&)serialMIDI);

void handleCtrlChange(byte ch, byte ctrl, byte val) {

  // midi cc message list att : https://anotherproducer.com/online-tools-for-musicians/midi-cc-list/
  Serial.println(ctrl);
  switch (ctrl) {
    case 7 : // resonance
      main_filter.set_resonance(dmap(val, 0, 127, 0.1, 10));
      break;
    case 72 : // releas
      for(int voice = 0; voice < VOICE_CNT; voice ++) {
        voices[voice].env.releas = dmap(val, 0, 127, 0.1f, 0.001f);
      }
      break;
    case 73 : // attack
      for(int voice = 0; voice < VOICE_CNT; voice ++) {
        voices[voice].env.attack = dmap(val, 0, 127, 0.1f, 0.001f);
      }
      break;
    case 74 : // cutoff
      main_filter.set_cutoff(dmap(val, 0, 127, 10, 10000));
      break;
    case 75 : // decay
      for(int voice = 0; voice < VOICE_CNT; voice ++) {
        voices[voice].env.decay = dmap(val, 0, 127, 0.1f, 0.001f);
      }
      break;
    case 76 : // sustain
      for(int voice = 0; voice < VOICE_CNT; voice ++) {
        voices[voice].env.sustain = val/127.0;
      }
      break;
    case 91 : // reverb
      break;
    case 93 : // chorus
      break;
  }
}

void handleNoteOff(byte ch, byte nt, byte vl) {
  for(int voice = 0; voice < VOICE_CNT; voice++) {
    if (voices[voice].note.pitch == noteToFreq[nt] && voices[voice].channel == ch) {
      voices[voice].env.state = 3;
      break;
    }
  }
}

void handleNoteOn(byte ch, byte nt, byte vl) {
  if(vl) {
    for(int voice = 0; voice < VOICE_CNT; voice++) {
      if (voices[voice].env.state == 4 || (voices[voice].env.state == 3 && voices[voice].note.pitch == noteToFreq[nt] && voices[voice].channel == ch)) {
        voices[voice].note.channel = ch;
        voices[voice].note.velocity = vl/127.0;
        voices[voice].note.pitch = noteToFreq[nt];
        voices[voice].note.step = (WAVETABLE_LEN/SAMPLE_RATE) * voices[voice].note.pitch;
        voices[voice].env.state = 0;
        voices[voice].waveform = wavetables[ch-1];
        voices[voice].channel = ch;
        break;
      }
    }
  } else handleNoteOff(ch, nt, vl);
}


void init_midi() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleControlChange(handleCtrlChange);
}

void midi_task(void * param) {
    while(true) {
        MIDI.read();
        vTaskDelay(1);
    }
}

#endif