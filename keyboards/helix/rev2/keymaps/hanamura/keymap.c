#include "helix.h"
#include "bootloader.h"
#include "action_layer.h"
#include "eeconfig.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#include "LUFA/Drivers/Peripheral/TWI.h"
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;
#endif

void dance_l_finished(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code(KC_LSFT);
    register_code(KC_9);
  } else if (state->count == 2) {
    register_code(KC_LBRC);
  } else {
    register_code(KC_LSFT);
    register_code(KC_LBRC);
  }
}

void dance_l_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    unregister_code(KC_LSFT);
    unregister_code(KC_9);
  } else if (state->count == 2) {
    unregister_code(KC_LBRC);
  } else {
    unregister_code(KC_LSFT);
    unregister_code(KC_LBRC);
  }
}

void dance_r_finished(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code(KC_LSFT);
    register_code(KC_0);
  } else if (state->count == 2) {
    register_code(KC_RBRC);
  } else {
    register_code(KC_LSFT);
    register_code(KC_RBRC);
  }
}

void dance_r_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    unregister_code(KC_LSFT);
    unregister_code(KC_0);
  } else if (state->count == 2) {
    unregister_code(KC_RBRC);
  } else {
    unregister_code(KC_LSFT);
    unregister_code(KC_RBRC);
  }
}

enum {
  TD_L = 0,
  TD_R
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_L] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_l_finished, dance_l_reset),
  [TD_R] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_r_finished, dance_r_reset),
};

extern uint8_t is_master;

#define _QWERTY 0
#define _LOWER 3
#define _RAISE 4
#define _NUMPAD 5
#define _ADJUST 16

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  NUMPAD,
  ADJUST,
  BACKLIT,
  RGBRST
};

// mod taps
#define CMD_EN MT(MOD_LGUI,KC_LANG2)
#define CMD_JA MT(MOD_RGUI,KC_LANG1)
#define CTL_ESC MT(MOD_LCTL,KC_ESC)
#define SFT_TAB MT(MOD_LSFT,KC_TAB)

// mod
#define M_CAG LCTL(LALT(KC_LGUI))

// characters
#define C_ELPS LALT(KC_SCLN)
#define C_TRUDQ LALT(KC_LBRC)
#define C_TRUSQ LALT(KC_RBRC)

// macos focuses
#define FO_MENU LCTL(KC_F2)
#define FO_TOOL LCTL(KC_F5)

// prev / next actions
#define TAB_PREV LGUI(LSFT(KC_LBRC))
#define TAB_NEXT LGUI(LSFT(KC_RBRC))
#define PANE_PREV LGUI(KC_LBRC)
#define PANE_NEXT LGUI(KC_RBRC)

// tap dances
#define TDL TD(TD_L)
#define TDR TD(TD_R)

// layers
#define NUMPAD_Z LT(_NUMPAD,KC_Z)

// utils
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = KEYMAP( \
      KC_TAB,  KC_Q,     KC_W,    KC_E,    KC_R,   KC_T,                  KC_Y,  KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC, \
      CTL_ESC, KC_A,     KC_S,    KC_D,    KC_F,   KC_G,                  KC_H,  KC_J,   KC_K,    KC_L,    KC_SCLN, KC_ENT,  \
      SFT_TAB, NUMPAD_Z, KC_X,    KC_C,    KC_V,   KC_B,                  KC_N,  KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
      M_CAG,   _______,  KC_LALT, KC_LALT, CMD_EN, LOWER, NUMPAD, KC_SPC, RAISE, CMD_JA, _______, _______, _______, _______  \
      ),

  [_LOWER] = KEYMAP( \
      KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSLS, \
      _______, KC_TILD, KC_GRV,  KC_DQT,  KC_QUOT, KC_UNDS,                   KC_MINS, TDL,     TDR,     KC_LBRC, KC_RBRC, _______, \
      _______, _______, _______, C_TRUDQ, C_TRUSQ, KC_PLUS,                   KC_EQL,  C_ELPS,  KC_PIPE, KC_LCBR, KC_RCBR, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
      ),

  [_RAISE] = KEYMAP( \
      KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSLS, \
      _______, KC_TILD, KC_GRV,  KC_DQT,  KC_QUOT, KC_UNDS,                   KC_MINS, TDL,     TDR,     KC_LBRC, KC_RBRC, _______, \
      _______, _______, _______, C_TRUDQ, C_TRUSQ, KC_PLUS,                   KC_EQL,  C_ELPS,  KC_PIPE, KC_LCBR, KC_RCBR, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
      ),

  [_NUMPAD] = KEYMAP( \
      _______, _______, _______, KC_UP,   _______, _______,                   KC_PPLS, KC_P7,   KC_P8, KC_P9,   KC_P0,   _______, \
      _______, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______,                   KC_PMNS, KC_P4,   KC_P5, KC_P6,   KC_PAST, _______, \
      _______, _______, _______, _______, _______, _______,                   KC_PEQL, KC_P1,   KC_P2, KC_P3,   KC_PSLS, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_P0, KC_PDOT, KC_PCMM, _______  \
      ),

  [_ADJUST] = KEYMAP( \
      _______, _______, _______, KC_PGUP, KC_VOLU, FO_MENU,                     FO_TOOL,   _______, _______, _______, _______, _______, \
      _______, _______, _______, KC_PGDN, KC_VOLD, TAB_PREV,                    TAB_NEXT,  _______, _______, _______, _______, _______, \
      _______, RESET,   _______, _______, _______, PANE_PREV,                   PANE_NEXT, _______, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______,   _______, _______, _______,   _______, _______, _______, _______, _______  \
      )
};

bool TOG_STATUS = false;
int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    #ifdef RGBLIGHT_ENABLE
      //rgblight_mode(RGB_current_mode);
    #endif
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
          //not sure how to have keyboard check mode and set it to a variable, so my work around
          //uses another variable that would be set to true after the first time a reactive key is pressed.
        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
        } else {
          TOG_STATUS = !TOG_STATUS;
          #ifdef RGBLIGHT_ENABLE
            //rgblight_mode(16);
          #endif
        }
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        #ifdef RGBLIGHT_ENABLE
          //rgblight_mode(RGB_current_mode);   // revert RGB to initial mode prior to RGB mode change
        #endif
        TOG_STATUS = false;
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        //not sure how to have keyboard check mode and set it to a variable, so my work around
        //uses another variable that would be set to true after the first time a reactive key is pressed.
        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
        } else {
          TOG_STATUS = !TOG_STATUS;
          #ifdef RGBLIGHT_ENABLE
            //rgblight_mode(15);
          #endif
        }
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        #ifdef RGBLIGHT_ENABLE
          //rgblight_mode(RGB_current_mode);  // revert RGB to initial mode prior to RGB mode change
        #endif
        layer_off(_RAISE);
        TOG_STATUS = false;
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case NUMPAD:
      if (record->event.pressed) {
        layer_on(_NUMPAD);
      } else {
        layer_off(_NUMPAD);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
      //led operations - RGB mode change now updates the RGB_current_mode to allow the right RGB mode to be set after reactive keys are released
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
     iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

//assign the right code to your layers for OLED display
#define L_BASE 0
#define L_LOWER 8
#define L_RAISE 16
#define L_FNLAYER 64
#define L_NUMLAY 128
#define L_NLOWER 136
#define L_NFNLAYER 192
#define L_MOUSECURSOR 256
#define L_ADJUST 65536
#define L_ADJUST_TRI 65560

static void render_logo(struct CharacterMatrix *matrix) {

  static char logo[]={
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
    0};
  matrix_write(matrix, logo);
  //matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}



void render_status(struct CharacterMatrix *matrix) {

  // Render to mode icon
  static char logo[][2][3]={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
  if(keymap_config.swap_lalt_lgui==false){
    matrix_write(matrix, logo[0][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[0][1]);
  }else{
    matrix_write(matrix, logo[1][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[1][1]);
  }

  // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
  char buf[40];
  snprintf(buf,sizeof(buf), "Undef-%ld", layer_state);
  matrix_write_P(matrix, PSTR("\nLayer: "));
    switch (layer_state) {
        case L_BASE:
           matrix_write_P(matrix, PSTR("Default"));
           break;
        case L_RAISE:
           matrix_write_P(matrix, PSTR("Raise"));
           break;
        case L_LOWER:
           matrix_write_P(matrix, PSTR("Lower"));
           break;
        case L_ADJUST:
        case L_ADJUST_TRI:
           matrix_write_P(matrix, PSTR("Adjust"));
           break;
        default:
           matrix_write(matrix, buf);
    }

  // Host Keyboard LED Status
  char led[40];
    snprintf(led, sizeof(led), "\n%s  %s  %s",
            (host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
            (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
            (host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
  matrix_write(matrix, led);
}


void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable) {
    return;
  }
#endif

  matrix_clear(&matrix);
  if(is_master){
    render_status(&matrix);
  }else{
    render_logo(&matrix);
  }
  matrix_update(&display, &matrix);
}

#endif
