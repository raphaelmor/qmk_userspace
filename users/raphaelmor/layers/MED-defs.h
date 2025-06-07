#pragma once

// Override keys with HRM and Layer keys

#define MED_NO KC_NO

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

#define MED_LT5 MED_NO
#define MED_LT4 MED_NO
#define MED_LT3 MED_NO
#define MED_LT2 MED_NO
#define MED_LT1 MED_NO
#define MED_LT0 MED_NO

#define MED_RT0 RGB_TOG
#define MED_RT1 RGB_MOD
#define MED_RT2 RGB_HUI
#define MED_RT3 RGB_SAI
#define MED_RT4 RGB_VAI
#define MED_RT5 MED_NO

#define MED_LM5 NAV_NO
#define MED_LM4 KC_LCTL
#define MED_LM3 KC_LALT
#define MED_LM2 KC_LGUI
#define MED_LM1 KC_LSFT
#define MED_LM0 NAV_NO

#define MED_RM0 MED_NO
#define MED_RM1 MED_NO
#define MED_RM2 KC_MPRV
#define MED_RM3 KC_VOLD
#define MED_RM4 KC_VOLU
#define MED_RM5 KC_MNXT

#define MED_LB5 MED_NO
#define MED_LB4 MED_NO
#define MED_LB3 MED_NO
#define MED_LB2 KC_ALGR
#define MED_LB1 MED_NO
#define MED_LB0 MED_NO

#define MED_RB0 MED_NO
#define MED_RB1 MED_NO
#define MED_RB2 MED_NO
#define MED_RB3 MED_NO
#define MED_RB4 MED_NO
#define MED_RB5 MED_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define MED_LH2 MED_NO
#define MED_LH1 MED_NO
#define MED_LH0 MED_NO

#define MED_RH0 KC_MSTP
#define MED_RH1 KC_MPLY
#define MED_RH2 KC_MUTE
