// Copyright 2022 Mark Stosberg (@markstos)
// Copyright 2024 Mike Szczys <@szczys>
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

// RGB Light off timer
#define RGB_LIGHT_TIMEOUT_MS 300000
static bool _leds_on = true;
static uint32_t _leds_timer = 0;
static bool _layer_has_changed = true;
static layer_state_t _last_layer = 1;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum custom_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _NUMPAD,
  _FUNC,
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  NUMPAD,
  FUNC,
  BACKLIT,
  ALT_TAB,
};

// Short names for complex keys
#define OSM_AGR  OSM(MOD_RALT)
#define GUI_ENT  GUI_T(KC_ENT)
#define LOW_TAB  LT(_LOWER, KC_TAB)
#define RSE_BSP  LT(_RAISE, KC_BSPC)
#define NUM_KCF  LT(_NUMPAD, KC_F)

enum combos {
  ER_PLUS,
  DF_MINUS,
  CV_UNDER,
  FG_EQL,
  QP_PRTSCR_SFT,
  ZP_PRTSCR,
  ESC_ALT_NUMLOCK,
};

const uint16_t PROGMEM er_combo[] = {KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM df_combo[] = {KC_D, NUM_KCF, COMBO_END};
const uint16_t PROGMEM cv_combo[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM fg_combo[] = {NUM_KCF, KC_G, COMBO_END};
const uint16_t PROGMEM qp_combo[] = {KC_Q, KC_P, COMBO_END};
const uint16_t PROGMEM zp_combo[] = {KC_Z, KC_P, COMBO_END};
const uint16_t PROGMEM esc_alt_combo[] = {KC_ESC, KC_LALT, COMBO_END};

combo_t key_combos[] = {
  [ER_PLUS]    = COMBO(er_combo, KC_PLUS),
  [DF_MINUS]    = COMBO(df_combo, KC_MINS),
  [CV_UNDER]    = COMBO(cv_combo, KC_UNDS),
  [FG_EQL]    = COMBO(fg_combo, KC_EQL),
  [QP_PRTSCR_SFT]    = COMBO(qp_combo, LSFT(KC_PSCR)),
  [ZP_PRTSCR]    = COMBO(zp_combo, KC_PSCR),
  [ESC_ALT_NUMLOCK]    = COMBO(esc_alt_combo, TG(_NUMPAD)),
};

// Tap Dance declarations
enum {
    TD_OP_BRC,
    TD_CL_BRC,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_OP_BRC] = ACTION_TAP_DANCE_DOUBLE(LSFT(KC_LBRC), KC_LBRC),
    [TD_CL_BRC] = ACTION_TAP_DANCE_DOUBLE(LSFT(KC_RBRC), KC_RBRC),
};

// Super alt-tab
bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        _leds_timer = timer_read32();
        if (!_leds_on) {
            rgblight_enable_noeeprom();
            _leds_on = true;
        }
    }

  switch (keycode) { // This will do most of the grunt work with the keycodes.
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
  }
  return true;
}

void housekeeping_task_user(void) {
    if ((_leds_on) && (timer_elapsed32(_leds_timer) > RGB_LIGHT_TIMEOUT_MS)) {
        rgblight_disable_noeeprom();
        _leds_on = false;
    }
}

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (!layer_state_is(_LOWER)) {
      // Release LALT when layer key is released
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    _layer_has_changed = true;
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y    ,KC_U    ,KC_I    ,KC_O    ,KC_P    ,KC_DEL  ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LALT,    KC_A,    KC_S,    KC_D, NUM_KCF,    KC_G,                     KC_H    ,KC_J    ,KC_K    ,KC_L    ,KC_SCLN ,KC_QUOT ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                     KC_N    ,KC_M    ,KC_COMM ,KC_DOT  ,KC_SLSH ,MO(_FUNC),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL, GUI_ENT, LOW_TAB,   RSE_BSP ,KC_SPC  ,KC_LSFT
                                      //`--------------------------'  `--------------------------'
  ),

  [_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, KC_EXLM, KC_AT,  KC_HASH, KC_DLR,  KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_1,    KC_2,   KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_VOLD, KC_VOLU, C(S(KC_C)),C(S(KC_V)), ALT_TAB,                KC_MPLY,LALT(KC_J),LALT(KC_K), _______, _______, _______ ,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS,  KC_TRNS, LOWER,    KC_F12,  KC_TRNS, KC_COLON
                                      //`--------------------------'  `--------------------------'
    ),

  [_RAISE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, KC_DEL , XXXXXXX, KC_UNDS, KC_PLUS, KC_PGUP,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_HOME, KC_END , KC_MINS, KC_EQL , KC_PGDN,                      KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_APP ,_______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_LT  , KC_GT  ,  KC_GRV, KC_TILD, XXXXXXX,                     C(KC_SPC) ,TD(TD_OP_BRC), TD(TD_CL_BRC), KC_PIPE, KC_BSLS,_______ ,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                     LCTL(KC_ENT), KC_TRNS, XXXXXXX,    RAISE  , KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  ),

  [_NUMPAD] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_PPLS,   KC_P7,   KC_P8,   KC_P9, KC_PAST,_______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, NUMPAD, XXXXXXX,                       KC_PMNS,   KC_P4,   KC_P5,   KC_P6, KC_PSLS,_______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_PEQL,   KC_P1,   KC_P2,   KC_P3, KC_PENT,_______ ,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX,  KC_ENT,  KC_TAB,     KC_SPC,   KC_P0, KC_PDOT
                                      //`--------------------------'  `--------------------------'
  ),

  [_FUNC] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, KC_F1  , KC_F2  , KC_F3   , KC_F4 ,  KC_F5 ,                     KC_F6   , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,_______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_F11 , KC_F12 , XXXXXXX, RGB_M_P, RGB_M_R,                     RGB_TOG , RGB_VAI, RGB_HUI, RGB_SAI, XXXXXXX,_______ ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_CAPS, XXXXXXX, XXXXXXX, RGB_M_SW, RGB_M_B,                    RGB_MOD , RGB_VAD, RGB_HUD, RGB_SAD, XXXXXXX,   FUNC ,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  )
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case LT(_RAISE, KC_BSPC):
            return TAPPING_TERM_THUMB;
      case LT(_LOWER, KC_TAB):
            return TAPPING_TERM_THUMB;
      default:
            return TAPPING_TERM;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_RAISE, KC_BSPC):
        case LT(_LOWER, KC_TAB):
            // Immediately select the hold action when another key is pressed.
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}

#ifdef OLED_ENABLE

void oled_render_logo(void) {
    // clang-format off
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    // clang-format on
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (_layer_has_changed) {
        _layer_has_changed = false;
        if (get_highest_layer(layer_state) != _last_layer) {
            _last_layer = get_highest_layer(layer_state);

            switch (get_highest_layer(layer_state)) {
                case _QWERTY:
                    oled_write_P(PSTR("Layer: "), false);
                    oled_write_P(PSTR("Qwerty\n"), false);
                    break;
                case _LOWER:
                    oled_write_P(PSTR("Layer: "), false);
                    oled_write_P(PSTR("Lower\n"), false);
                    break;
                case _RAISE:
                    oled_write_P(PSTR("Layer: "), false);
                    oled_write_P(PSTR("Raise\n"), false);
                    break;
                case _NUMPAD:
                    oled_write_P(PSTR("Layer: "), false);
                    oled_write_P(PSTR("Numpad\n"), false);
                    break;
                case _FUNC:
                    oled_write_P(PSTR("Layer: "), false);
                    oled_write_P(PSTR("Function\n"), false);
                    break;
                default:
                    // Or use the write_ln shortcut over adding '\n' to the end of your string
                    oled_write_P(PSTR("Layer: "), false);
                    oled_write_ln_P(PSTR("Undefined"), false);
            }
        }
    }
    return false;
}

#endif
