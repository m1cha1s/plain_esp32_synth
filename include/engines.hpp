#ifndef ENGINES_H
#define ENGINES_H

#include <Arduino.h>
#include "config.hpp"
#include "voices.hpp"
#include "filters.hpp"
#include "mixers.hpp"
#include "functions.hpp"
#include "delays.hpp"
#include "reverb.hpp"
#include "oscillators.hpp"

class Basic_engine {
    public:
        Basic_voice voices[VOICE_CNT];
        Wavetable_oscillator lfo0;
        Biquad_filter filter;
        Mixer mixer;
        Basic_delay delay0;
        Reverb reverb0;

        float lfo0_amount = 1;

        Basic_engine() {
            lfo0.set_frequency(200);
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

        void pitchBendChange(char ch, int val) {
            // for(int voice = 0; voice < VOICE_CNT; voice ++) {
            //     voices[voice].modulate(dmap(val, -9000, 9000, -100, 100));
            // }
        }

        void ctrlChange(byte ch, byte ctrl, byte val) {

            // midi cc message list att : https://anotherproducer.com/online-tools-for-musicians/midi-cc-list/
            Serial.println(ctrl);
            switch (ctrl) {
                case 1 : // mod
                lfo0_amount = dmap(val, 0, 127, 1, 1000);

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
                filter.set_cutoff(dmap(val, 0, 127, 30, 10000));
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
                reverb0.wet = dmap(val, 0, 127, 0, 1);
                break;
                case 93 : // chorus
                break;
                case 11 : // delay
                delay0.set_wet(dmap(val, 0, 127, 0, 1));
                break;
            }
        }

        inline float process() {
            float sample = 0;
            for(int vc = 0; vc < VOICE_CNT; vc ++) {
                sample += voices[vc].process();
            }
            sample = filter.process(sample);
            sample = delay0.process(sample);
            sample = reverb0.process(sample);
            return sample;
        }

        inline void compute() {
            filter.modulate(lfo0.process()*lfo0_amount);
            filter.compute();
            for(int vc = 0; vc < VOICE_CNT; vc ++) {
                voices[vc].compute();
            }
        }
};

#endif