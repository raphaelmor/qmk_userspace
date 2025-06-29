#include QMK_KEYBOARD_H

#include "raphaelmor.h"
#include "elpekenin/ledmap.h"

// Include the ledmap definitions
#include "ramo_ledmap.c"

// Info shared by subroutines (tap_dance, combos, etc..)
uint8_t  saved_modifiers;
uint32_t linger_timer = 0; // time elapsed since combo was pressed
// Adaptive
uint16_t prior_keycode = KC_NO;
uint16_t prior_keydown_timer = 0; // timer of keydown for adaptive threshhold.

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable=true;
    debug_keyboard=true;
    layer_state_set(default_layer_state);
};


// Include tap_dance processing code
#include "ramo_tap_dance.c"

// Include combo processing code
#include "ramo_combos.c"

// Include matrix processing code
#include "ramo_matrix.c"

// Include adaptive processing code
#include "ramo_adaptive.c"

// Include process_record code
#include "ramo_process_record.c"


