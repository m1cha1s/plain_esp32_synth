#ifndef ENVELOPES_H
#define ENVELOPES_H

typedef struct {
    float envelope = 0;

    float attack = 0.1;
    float decay = 0.001;
    float sustain = 0.7;
    float releas = 0.001;

    char state = 4;
}ADSR;

void computeADSR(ADSR *env) {
    switch(env->state) {
        case(0) :
            env->envelope += env->attack;
            if(env->envelope >= 1.0) env->state = 1;
            break;
        case(1) :
            env->envelope -= env->decay;
            if(env->envelope <= env->sustain) {
                env->state = 2;
                env->envelope = env->sustain;
            }
            break;
        case(3) :
            env->envelope -= env->releas;
            if(env->envelope <= 0.0) {
                env->state = 4;
                env->envelope = 0;
            }
            break;

    }
}

#endif