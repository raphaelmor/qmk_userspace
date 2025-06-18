#include QMK_KEYBOARD_H
#include "raphaelmor.h"

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable=true;
    // rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    // rgb_matrix_sethsv_noeeprom(HSV_OFF);
};

// Info shared by subroutines (tap_dance, combos, etc..)
uint8_t  saved_modifiers;
uint32_t linger_timer = 0; // time elapsed since combo was pressed

// process
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    saved_modifiers = get_mods(); // share modifiers
    return true;
}

// Include tap_dance processing code
#include "tap_dance.c"

// Include combo processing code
#include "combos.c"


