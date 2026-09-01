/*
 * ramo_process_record.c
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

// This is called everytime a key is pressed or released
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Do we need to filter multi-function keys?


    if (record->event.pressed) {
        uprintf("RAMO: USER: Pressed kc: %s\n", get_keycode_string(keycode));
    } else {
        uprintf("RAMO: USER: Released kc: %s\n", get_keycode_string(keycode));
    }

    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            if (!record->tap.count) // if not tapped yet…
                return true; // let QMK do that first
            keycode &= QK_BASIC_MAX; // mods & taps have been handled.
            uprintf("RAMO: USER: Key is complex -> Keycode &= QK_BASIC_MAX: %s\n", get_keycode_string(keycode));
    }

    // Linger brackets: track ( and { presses for hold-to-autopair (see ramo_linger.c)
    if (linger_track(keycode, record)) {
        return false; // consumed: open bracket already tapped
    }

    // Caps Word spaces: space -> _, double space -> real space + exit.
    // (see caps_word_space_track in ramo_linger.c)
    if (caps_word_space_track(keycode, record)) {
        return false;
    }

    // Shift + Backspace = Delete. Keeps Del available on the Hands Down base,
    // where it's no longer on a thumb (Option A). Only fires with shift alone.
    if (record->event.pressed && keycode == KC_BSPC
        && (get_mods() & MOD_MASK_SHIFT) && !(get_mods() & ~MOD_MASK_SHIFT)) {
        uint8_t saved = get_mods();
        clear_mods();
        RAMO_TAP(KC_DEL);
        set_mods(saved);
        return false;
    }

    // ADAPT_SHIFT (Hands Down base only): comma-leader one-shot Shift.
    // Type `,` then a letter within RAMO_ADAPT_SHIFT_TERM → delete the comma and
    // capitalize the letter (", the" -> "The"). Skipped while Caps Word is active.
    // Gated to HDP because `,` without a trailing space is common in code (arg lists).
    if (record->event.pressed
        && get_highest_layer(default_layer_state) == L_HANDSDOWN
        && prior_keycode == KC_COMM
        && !is_caps_word_on()
        && TIMER_DIFF_16(record->event.time, prior_keydown_timer) <= RAMO_ADAPT_SHIFT_TERM
        && keycode >= KC_A && keycode <= KC_Z) {
        RAMO_TAP(KC_BSPC);                 // remove the leader comma
        RAMO_TAP16(S(keycode));            // send the capitalized letter
        preprior_keycode = prior_keycode;  // keep history consistent for 3-key rules
        prior_keycode = keycode;           // record for the next adaptive check
        prior_keydown_timer = record->event.time;
        return false;                      // handled
    }

    if (record->event.pressed) {
        if (!process_adaptive_key(keycode, record)) { // false means we handled the key press
            preprior_keycode = prior_keycode;
            prior_keycode = keycode; // this keycode is stripped of mods+taps
            // Store the PRESS time, not timer_read(): mod-taps arrive late (see
            // the note in ramo_adaptive.c), and using read-time here would make
            // the *next* key's window start from this key's release instead.
            prior_keydown_timer = record->event.time;

            uprintf("RAMO: USER: Adaptive succeed. Setting prior to %s and reset timer\n", get_keycode_string(prior_keycode));
            return false; // took care of that key
        }

        preprior_keycode = prior_keycode;
        prior_keycode = keycode; // this keycode is stripped of mods+taps
        prior_keydown_timer = record->event.time; // press time (see above)
        uprintf("RAMO: USER: Adaptive failed. setting prior to %s and reset timer\n", get_keycode_string(prior_keycode));
    }
    return true;
}
