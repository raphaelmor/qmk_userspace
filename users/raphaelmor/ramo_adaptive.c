/*
 * ramo_adaptive.c
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

bool process_adaptive_key(uint16_t keycode, const keyrecord_t *record) {
    bool result = true;

    // Adaptives are tuned to Hands Down Promethium's physical rolls,
    // so only run them while the Hands Down base is the active default layer.
    // (On Colemak they'd "fix" bigrams that aren't even neighbors.)
    if (get_highest_layer(default_layer_state) != L_HANDSDOWN) {
        return true;
    }

    // Window is measured keydown-to-keydown using record->event.time, NOT
    // timer_read(). For a mod-tap/layer-tap, process_record_user only sees the
    // record once the tap resolves (on release, or after TAPPING_TERM), so
    // timer_read() here would measure prior-keydown → *this key's release* and
    // charge the hold time against the window. event.time is stamped when the
    // key was physically pressed, so 12 mod-tap keys (G M . / + both home rows)
    // get the same window as plain keys.
    if (TIMER_DIFF_16(record->event.time, prior_keydown_timer) > RAMO_ADAPTIVE_TERM) { // outside adaptive window
        prior_keycode = preprior_keycode = prior_keydown_timer = 0; // turn off adaptives, reset
        return true; // no adaptive conditions, continue processing.
    }
    // We're within the adaptive rolling window.


    switch (keycode) {

    // ---------------- left hand ----------------
        case KC_D:
            switch (prior_keycode) {
                case KC_B:
                    if (preprior_keycode == KC_P) {                  // P B D -> PWD
                        RAMO_TAP(KC_BSPC); RAMO_TAP(KC_W);           // del B, W, D continues
                    }
                    break;
                case KC_P: RAMO_TAP(KC_W); break;                    // PD -> PWD (insert W, D continues)
            }
            break;
        case KC_F:
            switch (prior_keycode) {
                case KC_P: RAMO_TAP(KC_S); result = false; break;    // PF -> PS
            }
            break;
        case KC_G:
            switch (prior_keycode) {
                case KC_K: RAMO_TAP(KC_L); result = false; break;    // KG -> KL
                case KC_M:
                    if (preprior_keycode == KC_W) {                  // W M G -> WML
                        RAMO_TAP(KC_L); result = false;              // G becomes L
                        break;
                    }
                    RAMO_TAP(KC_BSPC); RAMO_TAP(KC_L); break;        // MG -> LG (del M, L, G continues)
                case KC_J: RAMO_TAP(KC_P); break;                    // JG -> JPG (insert P, G continues)
                case KC_W: RAMO_TAP(KC_D); result = false; break;    // WG -> WD
            }
            break;
        case KC_H:
            switch (prior_keycode) {
                case KC_K: RAMO_TAP(KC_N); result = false; break;    // KH -> KN
            }
            break;
        case KC_J:
            switch (prior_keycode) {
                case KC_G: RAMO_SEND_STRING("th"); result = false; break; // GJ -> ...g + "th" (e.g. "length")
                // V J removed: V+J is the ˜ tilde combo (HD_tilde_combo), same
                // unreachability as W M above. W J kept — no combo on those keys.
                case KC_W: RAMO_TAP(KC_L); result = false; break;    // WJ -> WL
            }
            break;
        case KC_K:
            switch (prior_keycode) {
                case KC_M: RAMO_TAP(KC_BSPC); RAMO_TAP(KC_L); break; // MK -> LK (del M, L, K continues)
                case KC_H: RAMO_TAP(KC_BSPC); RAMO_TAP(KC_N); break; // HK -> NK (del H, N, K continues)
            }
            break;
        case KC_M:
            switch (prior_keycode) {
                case KC_G:                                           // GM -> GL
                case KC_V: RAMO_TAP(KC_L); result = false; break;    // VM -> VL
                // W M (and the 3-key M W M) removed: W+M is the `qu` combo
                // (HD_Q_keys). Fast enough to roll for the adaptive is also fast
                // enough to land inside COMBO_TERM, so `qu` won and the adaptive
                // was unreachable. Kept the combo — Q needs a home.
            }
            break;
        case KC_P:
            switch (prior_keycode) {
                case KC_F: RAMO_TAP(KC_BSPC); RAMO_TAP(KC_S); break; // FP -> SP (del F, S, P continues)
            }
            break;
        case KC_V:
            switch (prior_keycode) {
                case KC_G: RAMO_TAP(KC_T); result = false; break;    // GV -> GT
                case KC_M: RAMO_TAP(KC_BSPC); RAMO_TAP(KC_L); break; // MV -> LV (del M, L, V continues)
            }
            break;
        case KC_W:
            switch (prior_keycode) {
                case KC_G: RAMO_TAP(KC_D); result = false; break;    // GW -> GD
                case KC_M: RAMO_TAP(KC_P); result = false; break;    // MW -> MP
            }
            break;

    // ---------------- right hand ----------------
        case KC_B:
            switch (prior_keycode) {
                case KC_Y: RAMO_TAP(KC_BSPC); RAMO_TAP(KC_I); break; // YB -> IB (del Y, I, B continues)
            }
            break;
        case KC_E:
            switch (prior_keycode) {
                case KC_A: RAMO_TAP(KC_U); result = false; break;    // AE -> AU (keeps home row)
            }
            break;
    }

    // true: no adaptive triggered, continue processing.
    // false: adaptive replaced the key, stop processing.
    return result;
}

/*
 Skipped by choice (low value on this HDP arrangement):
   P/B/S then L -> unshifted L

 Removed — unreachable, the same two keys are a combo (combos resolve in
 pre_process_record_quantum, before this engine ever runs):
   W M -> lm   and  M W M -> MWL   (W+M is the `qu` combo)
   V J -> VL                       (V+J is the ˜ tilde combo)
*/
