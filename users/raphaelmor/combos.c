// combos
#ifdef USE_HD_H_DIGRAPH_COMBO
const uint16_t PROGMEM HD_Th_combo[] = {HD_Th_keys, COMBO_END}; // TYPE "th" in hands down
#endif
const uint16_t PROGMEM HD_Q_combo[] = {HD_Q_keys, COMBO_END}; // TYPE "q" (Qu & Linger deletes u)
const uint16_t PROGMEM HD_Z_combo[] = {HD_Z_keys, COMBO_END}; // TYPE "z"
const uint16_t PROGMEM CMK_Th_combo[] = {CMK_Th_keys, COMBO_END}; // TYPE "th" in colemak

combo_t key_combos[] = {
    // Hands Down (Gold and Promethium) combos
#ifdef USE_HD_H_DIGRAPH_COMBO
    [HD_COMBO_Th] = COMBO_ACTION(HD_Th_combo),
    #endif
    [HD_COMBO_Q] = COMBO_ACTION(HD_Q_combo),
    [HD_COMBO_Z] = COMBO(HD_Z_combo, KC_Z),
    // Colemak combos
    [CMK_COMBO_Th] = COMBO_ACTION(CMK_Th_combo),
};

uint8_t  current_combo = 0;           // for combo actions to hold before triggering

uint8_t ramo_process_combo_pressed(uint16_t combo_index);
void    ramo_process_combo_released(uint16_t combo_index);

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (pressed) {
        current_combo = ramo_process_combo_pressed(combo_index);
        if (current_combo != NO_COMBO) {
            linger_timer = timer_read();
        }
    } else {
        ramo_process_combo_released(combo_index);
        current_combo = NO_COMBO;
    }
}

// return the current combo, if it's a lingering combo
uint8_t ramo_process_combo_pressed(uint16_t combo_index) {
    if (saved_modifiers & (MOD_MASK_ALT | MOD_MASK_GUI | MOD_MASK_CTRL)) {
        // if alt, gui, or ctrl is pressed, don't handle combos
        return 0;
    }

    switch(combo_index) {
#ifdef USE_HD_H_DIGRAPH_COMBO
        case HD_COMBO_Th:
#endif
        case CMK_COMBO_Th:
            if (is_caps_word_on()) {
                tap_code16(S(KC_T)); // send "T"
                tap_code16(S(KC_H)); // send "H"
            } else {
                tap_code(KC_T); // send "T" honoring caps
                unregister_mods(MOD_MASK_SHIFT);
                tap_code(KC_H); // send "h" honoring CAPSLK state
            }
            break;
        case HD_COMBO_Q:
            println("RAMO: Pressed N + H to type qu");
            if (is_caps_word_on()) {
                tap_code16(S(KC_Q));
                tap_code16(S(KC_U));
            } else {
                tap_code16(KC_Q);
                unregister_mods(MOD_MASK_SHIFT);
                tap_code16(KC_U);
            }
            return combo_index; // if held, delete the 'u' in matrix_scan_user_process_combo
    }
    return 0;
}

void ramo_process_combo_released(uint16_t combo_index) {
    switch(combo_index) {
#ifdef USE_HD_H_DIGRAPH_COMBO
        case HD_COMBO_Th:
#endif
        case CMK_COMBO_Th:
            break;
        case HD_COMBO_Q:
            println("RAMO: Released N + H to type qu");
    }
}

void matrix_scan_user(void) {
    if ((timer_elapsed(linger_timer) > RAMO_COMBO_HOLD) && current_combo) {
        println("RAMO: TRIGGERING COMBO_HOLD");
        uint16_t saved_mods = get_mods();
        clear_mods();
        switch (current_combo) {
        case HD_COMBO_Q:
            tap_code16(KC_BSPC); // held, so delete u
            break;
        }
        current_combo = NO_COMBO;
        set_mods(saved_mods);
    }
}
