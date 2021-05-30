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
  // Serial.println(ctrl);
  eng.ctrlChange(ch, ctrl, val);  
}

void handleNoteOff(byte ch, byte nt, byte vl) {
  eng.noteOff(ch, nt, vl);
}

void handleNoteOn(byte ch, byte nt, byte vl) {
  eng.noteOn(ch, nt, vl);
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