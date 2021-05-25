#ifndef I2S_CONFIG_H
#define I2S_CONFIG_H

#include <Wire.h>
#include <driver/i2s.h>

#include "config.hpp"

static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number
static const i2s_config_t i2s_config = {
    #ifdef INTERNAL_DAC
    (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    #endif
    #ifdef WM8960_DAC
    (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    #endif
    (int)(SAMPLE_RATE),
    I2S_BITS_PER_SAMPLE_16BIT, /* the DAC module will only take the 8bits from MSB */
    I2S_CHANNEL_FMT_RIGHT_LEFT,
    I2S_COMM_FORMAT_I2S_MSB,
    ESP_INTR_FLAG_LEVEL1, // default interrupt priority
    8,
    BUFFER_SIZE*4,
    false
};

#ifdef WM8960_DAC

#include "wm8960.hpp"

i2s_pin_config_t pin_config = {
    13, //this is BCK pin
    12, // this is LRCK pin
    14, // this is DATA output pin
    -1   //Not used
};
#endif

void init_I2S() {
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
    #ifdef INTERNAL_DAC
    i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
    i2s_set_pin(i2s_num, NULL);
    #endif
    #ifdef WM8960_DAC
    Wire.begin();
    WM8960_Init();
    i2s_set_pin(i2s_num, &pin_config);
    #endif
}

#endif