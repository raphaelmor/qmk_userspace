#include QMK_KEYBOARD_H
#include "raphaelmor.h"


void ramo_TD_make_colemak_default(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        set_single_default_layer(L_COLEMAK);
        print("RAMO: switched to Colemak");
    }
};
void ramo_TD_make_hd_gold_default(tap_dance_state_t *state, void *user_data) {
    if (state->count == 2) {
        set_single_default_layer(L_HANDSDOWN);
        print("RAMO: switched to Handsdown Gold");
    }
};

tap_dance_action_t tap_dance_actions[] = {
    [RAMO_TD_CMK] = ACTION_TAP_DANCE_FN(ramo_TD_make_colemak_default),
    [RAMO_TD_HDG] = ACTION_TAP_DANCE_FN(ramo_TD_make_hd_gold_default),
};


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
};
