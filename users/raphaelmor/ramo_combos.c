/*
 * ramo_combos.c
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

// combos
#ifdef USE_HD_H_DIGRAPH_COMBO
const uint16_t PROGMEM HD_Th_combo[] = {HD_Th_keys, COMBO_END}; // TYPE "th" in hands down
#endif
const uint16_t PROGMEM HD_Qu_combo[] = {HD_Q_keys, COMBO_END}; // TYPE "qu" (Linger deletes u)
const uint16_t PROGMEM HD_Z_combo[] = {HD_Z_keys, COMBO_END}; // TYPE "z"
const uint16_t PROGMEM CMK_Th_combo[] = {CMK_Th_keys, COMBO_END}; // TYPE "th" in colemak

// --- Hands Down base only (gated in combo_should_trigger) -------------------
// All at moutis' CANONICAL PM positions (layout now matches). Diacritics/ligatures
// assume the stock US Mac layout (⌥-dead-keys).
// 2g diacritics — tap the combo (dead key / glyph), then type the letter:
const uint16_t PROGMEM HD_acut_combo[]  = {HDP_RM1, HDP_RM2, COMBO_END}; // A+E → ´ (⌥E) é
const uint16_t PROGMEM HD_grv_combo[]   = {HDP_RM2, HDP_RM3, COMBO_END}; // E+I → ` (⌥`) è
const uint16_t PROGMEM HD_circ_combo[]  = {HDP_RM1, HDP_RM3, COMBO_END}; // A+I → ˆ (⌥I) ê
const uint16_t PROGMEM HD_umla_combo[]  = {HDP_RB1, HDP_RB2, COMBO_END}; // U+O → ¨ (⌥U) ü
const uint16_t PROGMEM HD_tilde_combo[] = {HDP_LT4, HDP_LT0, COMBO_END}; // V+J → ˜ (⌥N) ñ
const uint16_t PROGMEM HD_cedil_combo[] = {HDP_LM4, HDP_LM2, COMBO_END}; // S+T → ç (⌥C direct)
// 2f ligatures:
const uint16_t PROGMEM HD_oe_combo[]    = {HDP_RM2, HDP_RB2, COMBO_END}; // E+O → œ (⌥Q)
const uint16_t PROGMEM HD_ae_combo[]    = {HDP_RM1, HDP_RB1, COMBO_END}; // A+U → æ (⌥')
// 2c whitespace (Tab/Enter already on thumbs → only these):
const uint16_t PROGMEM HD_stab_combo[]  = {HDP_LT4, HDP_LT3, HDP_LT2, COMBO_END}; // V+W+G → Shift-Tab
const uint16_t PROGMEM HD_app_combo[]   = {HDP_LT1, HDP_LT0, COMBO_END}; // M+J → App menu
// 2e symbols — canonical top-right punctuation cluster (# . / " '):
const uint16_t PROGMEM HD_scln_combo[]  = {HDP_RM0, HDP_RM1, COMBO_END}; // ,+A → ;
const uint16_t PROGMEM HD_coln_combo[]  = {HDP_RT1, HDP_RT3, COMBO_END}; // .+" → :
const uint16_t PROGMEM HD_exlm_combo[]  = {HDP_RT1, HDP_RT2, COMBO_END}; // .+/ → !
const uint16_t PROGMEM HD_ques_combo[]  = {HDP_RT2, HDP_RT3, COMBO_END}; // /+" → ?
// 2h caps word — canonical position (moutis: H_CAPW_combo on HD_RB3+HD_RB4).
// Same two keys as the YB → IB adaptive, but combos are resolved in
// pre_process_record_quantum before process_record_user ever runs: a
// simultaneous press is a combo, a sequential roll is the adaptive.
const uint16_t PROGMEM HD_capw_combo[]  = {HDP_RB3, HDP_RB4, COMBO_END}; // Y+B → Caps Word

enum ramo_combos {
    NO_COMBO = 0, // Sentinel value (0) to signify no combo is currently pressed
#ifdef USE_HD_H_DIGRAPH_COMBO
    HD_COMBO_Th,
#endif
    HD_COMBO_QU,
    HD_COMBO_Z,
    CMK_COMBO_Th,
    // Hands Down base only (contiguous — see combo_should_trigger)
    HD_COMBO_ACUT, HD_COMBO_GRV, HD_COMBO_CIRC, HD_COMBO_UMLA, HD_COMBO_TILDE, HD_COMBO_CEDIL,
    HD_COMBO_OE, HD_COMBO_AE,
    HD_COMBO_STAB, HD_COMBO_APP,
    HD_COMBO_SCLN, HD_COMBO_COLN, HD_COMBO_QUES, HD_COMBO_EXLM,
    HD_COMBO_CAPW, // keep last: combo_should_trigger gates ACUT ... CAPW
};

combo_t key_combos[] = {
    // Hands Down (Gold and Promethium) combos
#ifdef USE_HD_H_DIGRAPH_COMBO
    [HD_COMBO_Th] = COMBO_ACTION(HD_Th_combo),
    #endif
    [HD_COMBO_QU] = COMBO_ACTION(HD_Qu_combo),
    [HD_COMBO_Z] = COMBO(HD_Z_combo, KC_Z),
    // Colemak combos
    [CMK_COMBO_Th] = COMBO_ACTION(CMK_Th_combo),
    // Hands Down base only (gated in combo_should_trigger)
    [HD_COMBO_ACUT]  = COMBO(HD_acut_combo,  A(KC_E)),
    [HD_COMBO_GRV]   = COMBO(HD_grv_combo,   A(KC_GRV)),
    [HD_COMBO_CIRC]  = COMBO(HD_circ_combo,  A(KC_I)),
    [HD_COMBO_UMLA]  = COMBO(HD_umla_combo,  A(KC_U)),
    [HD_COMBO_TILDE] = COMBO(HD_tilde_combo, A(KC_N)),
    [HD_COMBO_CEDIL] = COMBO(HD_cedil_combo, A(KC_C)),
    [HD_COMBO_OE]    = COMBO(HD_oe_combo,    A(KC_Q)),
    [HD_COMBO_AE]    = COMBO(HD_ae_combo,    A(KC_QUOT)),
    [HD_COMBO_STAB]  = COMBO(HD_stab_combo,  S(KC_TAB)),
    [HD_COMBO_APP]   = COMBO(HD_app_combo,   KC_APP),
    [HD_COMBO_SCLN]  = COMBO(HD_scln_combo,  KC_SCLN),
    [HD_COMBO_COLN]  = COMBO(HD_coln_combo,  KC_COLN),
    [HD_COMBO_QUES]  = COMBO(HD_ques_combo,  KC_QUES),
    [HD_COMBO_EXLM]  = COMBO(HD_exlm_combo,  KC_EXLM),
    [HD_COMBO_CAPW]  = COMBO(HD_capw_combo,  CW_TOGG),
};

// Restrict the added diacritic/ligature/symbol/whitespace combos to the Hands
// Down base; qu/z/th keep their existing (both-base) behavior.
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch (combo_index) {
        case HD_COMBO_ACUT ... HD_COMBO_CAPW:
            return get_highest_layer(default_layer_state) == L_HANDSDOWN;
    }
    return true;
}

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
                RAMO_TAP16(S(KC_T)); // send "T"
                RAMO_TAP16(S(KC_H)); // send "H"
            } else {
                RAMO_TAP(KC_T); // send "T" honoring caps
                unregister_mods(MOD_MASK_SHIFT);
                RAMO_TAP(KC_H); // send "h" honoring CAPSLK state
            }
            break;
        case HD_COMBO_QU:
            println("RAMO: W + M -> qu");
            if (is_caps_word_on()) {
                RAMO_TAP16(S(KC_Q));
                RAMO_TAP16(S(KC_U));
            } else {
                RAMO_TAP16(KC_Q);
                unregister_mods(MOD_MASK_SHIFT);
                RAMO_TAP16(KC_U);
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
        case HD_COMBO_QU:
            println("RAMO: released W + M (qu)");
    }
}

void combo_check_for_linger(void) {
    if ((timer_elapsed(linger_timer) > RAMO_COMBO_HOLD) && current_combo) {
        println("RAMO: TRIGGERING COMBO_HOLD");
        uint16_t saved_mods = get_mods();
        clear_mods();
        switch (current_combo) {
        case HD_COMBO_QU:
            RAMO_TAP16(KC_BSPC); // held, so delete u
            break;
        }
        current_combo = NO_COMBO;
        set_mods(saved_mods);
    }
}
