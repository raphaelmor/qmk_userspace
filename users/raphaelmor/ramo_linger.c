/*
 * ramo_linger.c
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

// Linger brackets + smart Caps Word (casemods)
//
// Linger: tap ( or { for a normal single bracket; HOLD it past RAMO_LINGER_TERM
// and it auto-closes with the caret between the pair:  (|)   {|} .
// We consume the press and TAP the open bracket immediately (so the key is never
// held in the HID report → no OS auto-repeat), then add the closing bracket + Left
// once the hold threshold is reached. No event deferral, no roll-over ordering bugs.

// returns true if it consumed the key press (caller should stop processing).
bool linger_track(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uint16_t close = 0;
        switch (keycode) {
            case KC_LPRN: close = KC_RPRN; break;
            case KC_LCBR: close = KC_RCBR; break;
        }
        if (close) {
            RAMO_TAP16(keycode);       // type the open bracket now (tap → no auto-repeat)
            linger_key    = keycode;
            linger_close  = close;
            linger_ktimer = timer_read();
            linger_fired  = false;
            return true;               // consumed
        }
        linger_key = 0;                // any other key press cancels a pending linger
    } else if (keycode == linger_key) {
        linger_key = 0;                // released before/after threshold
    }
    return false;
}

void linger_scan(void) {
    if (linger_key && !linger_fired && timer_elapsed(linger_ktimer) > RAMO_LINGER_TERM) {
        RAMO_TAP16(linger_close); // add the closing bracket
        RAMO_TAP16(KC_LEFT);      // move caret between the pair
        linger_fired = true;
    }
}

// Smart Caps Word: letters get shifted; digits / _ / - / bspc keep it on;
// anything else ends it. Space is handled by caps_word_space_track() below
// (space -> _, double space -> real space + exit), so KC_SPC deliberately does
// NOT appear here: if a space ever reaches this hook the intercept missed it,
// and ending the word is the right fallback.
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT)); // shift the letter
            return true;
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_MINS:
        case KC_UNDS:
            return true;  // keep Caps Word active, don't shift
        default:
            return false; // turn Caps Word off
    }
}

// --- Caps Word space handling (SCREAMING_SNAKE) ------------------------------
// space        -> `_`, Caps Word stays on
// space space  -> deletes the `_`, sends a real space, Caps Word turns off
//
// This lives in process_record_user rather than caps_word_press_user because
// that hook can't suppress a keypress — it can only allow it or end the word.
//
// NOTE: the space key is LT(L_NAV, KC_SPC) on both bases, so this only sees
// KC_SPC because process_record_user strips mod-taps/layer-taps to their tap
// keycode first. Keep this call *after* that strip.
static bool cw_space_pending = false; // did the last press emit a `_`?

// QMK calls this whenever Caps Word turns on or off (including idle timeout),
// so the pending-space state can never outlive the word.
void caps_word_set_user(bool active) {
    cw_space_pending = false;
}

// returns true if it consumed the key press (caller should stop processing).
bool caps_word_space_track(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed || !is_caps_word_on()) {
        return false;
    }
    if (keycode != KC_SPC) {
        cw_space_pending = false; // any other key ends the double-space window
        return false;
    }
    if (cw_space_pending) {       // second space in a row: end the word
        RAMO_TAP(KC_BSPC);        // remove the `_` we just typed
        caps_word_off();          // clears cw_space_pending via caps_word_set_user
        RAMO_TAP(KC_SPC);         // ...and leave a real space behind
        return true;
    }
    RAMO_TAP16(KC_UNDS);          // first space: SCREAMING_SNAKE separator
    cw_space_pending = true;
    return true;
}
