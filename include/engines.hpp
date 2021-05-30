#ifndef ENGINES_H
#define ENGINES_H

#include <Arduino.h>
#include "config.hpp"
#include "voices.hpp"
#include "filters.hpp"
#include "mixers.hpp"
#include "functions.hpp"

class Basic_engine {
    public:
        Basic_voice voices[VOICE_CNT];
        State_variable_filter filter;
        Mixer mixer;

        Basic_engine() {
            filter.compute();
        }

        void noteOn(char ch, char nt, char vl) {
            if(vl!=0) {
                for(int vc = 0; vc < VOICE_CNT; vc ++) {
                    if(voices[vc].noteOn(ch, nt, vl)) {
                        //Serial.println("Note on");
                        break;
                    }
                }
            } else noteOff(ch, nt, vl);
        }

        void noteOff(char ch, char nt, char vl) {
            for(int vc = 0; vc < VOICE_CNT; vc ++) {
                if(voices[vc].noteOff(ch, nt, vl)) break;
            }
        }

        void ctrlChange(byte ch, byte ctrl, byte val) {

            // midi cc message list att : https://anotherproducer.com/online-tools-for-musicians/midi-cc-list/
            // Serial.println(ctrl);
            switch (ctrl) {
                case 7 : // resonance
                filter.set_resonance(dmap(val, 0, 127, 0.1, 10));
                break;
                case 72 : // releas
                for(int voice = 0; voice < VOICE_CNT; voice ++) {
                    voices[voice].env0.releas = dmap(val, 0, 127, 0.1f, 0.001f);
                }
                break;
                case 73 : // attack
                for(int voice = 0; voice < VOICE_CNT; voice ++) {
                    voices[voice].env0.attack = dmap(val, 0, 127, 0.1f, 0.001f);
                }
                break;
                case 74 : // cutoff
                filter.set_cutoff(dmap(val, 0, 127, 10, 10000));
                break;
                case 75 : // decay
                for(int voice = 0; voice < VOICE_CNT; voice ++) {
                    voices[voice].env0.decay = dmap(val, 0, 127, 0.1f, 0.001f);
                }
                break;
                case 76 : // sustain
                for(int voice = 0; voice < VOICE_CNT; voice ++) {
                    voices[voice].env0.sustain = val/127.0;
                }
                break;
                case 91 : // reverb
                break;
                case 93 : // chorus
                break;
            }
        }

        inline float process() {
            for(int vc = 0; vc < VOICE_CNT; vc ++) {
                mixer.mix(voices[vc].process());
            }
            float sample = mixer.process();
            // Serial.println(sample);
            //sample = filter.process(sample);
            return sample;
        }

        inline void compute() {
            filter.compute();
            for(int vc = 0; vc < VOICE_CNT; vc ++) {
                voices[vc].compute();
            }
        }

};

#endif