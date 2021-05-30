#include <Arduino.h>
#include <MIDI.h>
#include <driver/i2s.h>
#include <math.h> 
#include <WiFi.h>

#include "globals.hpp"
#include "interpolators.hpp"
#include "lookuptables.hpp"
#include "wavetables.hpp"
#include "filters.hpp"
#include "envelopes.hpp"
#include "notes.hpp"
#include "voices.hpp"
#include "i2s_config.hpp"
#include "config.hpp"
#include "functions.hpp"
#include "midi.hpp"
#include "engines.hpp"

void core_0_task(void * param);
void setup_0();
void loop_0();

void setup() {
  Serial.begin(9600);
  
  init_midi();
  init_I2S();

  randomSeed(34547379);

  xTaskCreatePinnedToCore(core_0_task, "Core 0 task", 10000, NULL, 0, NULL, 0);
  xTaskCreatePinnedToCore(midi_task, "midi task", 10000, NULL, 0, NULL, 0);
}

void loop() {
  for(int sample = 0; sample < BUFFER_SIZE; sample ++) {
    sample_buffer_f[sample] = 0;
    if(sample % 31 == 0) {
      eng.compute();
    }
    sample_buffer_f[sample] += eng.process();
    // Serial.println(sample_buffer_f[sample]);
  }

  for(int sample = 0; sample < BUFFER_SIZE; sample ++) {
    uint32_t curr_sample_i = uint16_t(((sample_buffer_f[sample]/(float)VOICE_CNT)+1.0)*32767.5);
    // Serial.println(curr_sample_i);
    sample_buffer_i[sample] = curr_sample_i;
  }
  i2s_write(i2s_num, &sample_buffer_i, sizeof(sample_buffer_i), &bytesWritten, portMAX_DELAY);
}

void core_0_task(void * param) {
  setup_0();  
  while(true) {
    loop_0();
    vTaskDelay(1);
  }
}

void setup_0() {
    pinMode(2, OUTPUT);
}

void loop_0() {
    digitalWrite(2, 1);
    delay(1000);
    digitalWrite(2, 0);
    delay(1000);
}