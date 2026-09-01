/*
 * ramo_tap_dance.c
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

void ramo_TD_make_colemak_default(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        set_single_default_layer(L_COLEMAK);
        layer_off(L_HANDSDOWN);
        layer_on(L_COLEMAK);
    }
};

void ramo_TD_make_handsdown_default(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        set_single_default_layer(L_HANDSDOWN);
        layer_off(L_COLEMAK);
        layer_on(L_HANDSDOWN);
    }
};

tap_dance_action_t tap_dance_actions[] = {
    [RAMO_TD_CMK] = ACTION_TAP_DANCE_FN(ramo_TD_make_colemak_default),
    [RAMO_TD_HD] = ACTION_TAP_DANCE_FN(ramo_TD_make_handsdown_default),
};

