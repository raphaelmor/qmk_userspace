#pragma once

// Override keys with HRM and Layer keys

#define NUM_NO KC_NO

// Now let's place these Colemak keycodes on the keymap
// for variation independent spatial referencing by key position
//
//     Key Position Names for a 42 key split form factor
//        Should cover Corne v4.1
//     ╭─────────────────────────╮  ╭─────────────────────────╮
//     │ LT5 LT4 LT3 LT2 LT1 LT0 │  │ RT0 RT1 RT2 RT3 RT4 RT5 │
//     │ LM5 LM4 LM3 LM2 LM1 LM0 │  │ RM0 RM1 RM2 RM3 RM4 RM5 │
//     │ LB5 LB4 LB3 LB2 LB1 LB0 │  │ RB0 RB1 RB2 RB3 RB4 RB5 │
//     ╰───────────╮ LH2 LH1 LH0 │  │ RH0 RH1 RH2 ╭───────────╯
//                 ╰─────────────╯  ╰─────────────╯
//

#define NUM_LT5 NUM_NO
#define NUM_LT4 KC_LBRC
#define NUM_LT3 KC_7
#define NUM_LT2 KC_8
#define NUM_LT1 KC_9
#define NUM_LT0 KC_RBRC
#define NUM_RT0 NUM_NO
#define NUM_RT1 NUM_NO
#define NUM_RT2 NUM_NO
#define NUM_RT3 NUM_NO
#define NUM_RT4 NUM_NO
#define NUM_RT5 NUM_NO

#define NUM_LM5 NUM_NO
#define NUM_LM4 KC_SCLN
#define NUM_LM3 KC_4
#define NUM_LM2 KC_5
#define NUM_LM1 KC_6
#define NUM_LM0 KC_EQL
#define NUM_RM0 NUM_NO
#define NUM_RM1 KC_RSFT
#define NUM_RM2 KC_RGUI
#define NUM_RM3 KC_RALT
#define NUM_RM4 KC_RCTL
#define NUM_RM5 NUM_NO

#define NUM_LB5 KC_GRV
#define NUM_LB4 KC_1
#define NUM_LB3 KC_2
#define NUM_LB2 KC_3
#define NUM_LB1 KC_BSLS
#define NUM_LB0 NUM_NO
#define NUM_RB0 NUM_NO
#define NUM_RB1 NUM_NO
#define NUM_RB2 NUM_NO
#define NUM_RB3 KC_ALGR
#define NUM_RB4 NUM_NO
#define NUM_RB5 NUM_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define NUM_LH2 KC_DOT
#define NUM_LH1 KC_0
#define NUM_LH0 KC_MINS

#define NUM_RH0 NUM_NO
#define NUM_RH1 NUM_NO
#define NUM_RH2 NUM_NO
