#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "config.hpp"
#include "voices.hpp"
#include "filters.hpp"
#include "engines.hpp"

//Voice voices[VOICE_CNT];
//Biquad_filter main_filter;



Basic_engine eng;

size_t bytesWritten;
uint32_t sample_buffer_i[BUFFER_SIZE] = {0};

float sample_buffer_f[BUFFER_SIZE] = {0};

float soundNoiseLevel = 0.001;

#endif