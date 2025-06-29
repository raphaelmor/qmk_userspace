// This is called everytime a key is pressed or released
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool result = true;
    // uint8_t saved_modifiers;
    // saved_modifiers = get_mods(); // share modifiers

    // Do we need to filter multi-function keys?
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            if (!record->tap.count) // if not tapped yet…
                return true; // let QMK do that first
            keycode &= QK_BASIC_MAX; // mods & taps have been handled.
    }

    if (record->event.pressed) {
        if (!process_adaptive_key(keycode, record)) { // false means we handled the key press
            prior_keycode = keycode &= QK_BASIC_MAX; // this keycode is stripped of mods+taps
            prior_keydown_timer = timer_read(); // reset the adaptive timer
            return false; // took care of that key
        }

        // RAMO: is this needed ?
        // prior_keycode = keycode &= QK_BASIC_MAX; // this keycode is stripped of mods+taps
        // prior_keydown_timer = timer_read(); // reset the adaptive timer
    } else {

    }
    return result;
}
