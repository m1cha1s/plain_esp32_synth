#ifndef ENVELOPES_H
#define ENVELOPES_H

#include "notes.hpp"

class ADSR {
    public:
        float envelope = 0;

        float attack = 0.1;
        float decay = 0.001;
        float sustain = 0.7;
        float releas = 0.001;

        char state = 4;

        inline void compute() {
            switch(state) {
                case(0) :
                    envelope += attack;
                    if(envelope >= 1.0) state = 1;
                    break;
                case(1) :
                    envelope -= decay;
                    if(envelope <= sustain) {
                        state = 2;
                        envelope = sustain;
                    }
                    break;
                case(3) :
                    envelope -= releas;
                    if(envelope <= 0.0) {
                        state = 4;
                        envelope = 0;
                    }
                    break;
            }
        }
};
#endif