#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

#define USE_I2C
#define USE_SERIAL

#define MASTER_LEFT

#define SSD1306OLED

#define HELIX_ROWS 4
#define MATRIX_ROWS 8
#define MATRIX_COLS 7
#define MATRIX_ROW_PINS { D4, C6, D7, E6 }

#define USE_SERIAL_PD2

#define TAPPING_TERM 150

#undef RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 25
#define RGBLIGHT_LIMIT_VAL 130
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17
#endif
