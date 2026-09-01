/*
 * raphaelmor.c
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

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
uint16_t preprior_keycode = KC_NO; // key before prior (for 3-key adaptives)
uint16_t prior_keydown_timer = 0; // timer of keydown for adaptive threshhold.
// Linger brackets
uint16_t linger_key = 0;    // currently-lingering open bracket (0 = none)
uint16_t linger_close = 0;  // its matching close bracket
uint16_t linger_ktimer = 0; // keydown timer for the linger hold
bool     linger_fired = false; // has the pair already been inserted?

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

// Include linger-bracket + casemods code (must precede matrix + process_record)
#include "ramo_linger.c"

// Include matrix processing code
#include "ramo_matrix.c"

// Include adaptive processing code
#include "ramo_adaptive.c"

// Include process_record code
#include "ramo_process_record.c"


