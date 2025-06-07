#pragma once

// Override keys with HRM and Layer keys

#define MOS_NO KC_NO

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
#define MOS_LT5 NAV_NO
#define MOS_LT4 NAV_NO
#define MOS_LT3 NAV_NO
#define MOS_LT2 NAV_NO
#define MOS_LT1 NAV_NO
#define MOS_LT0 NAV_NO

#define MOS_RT0 SCMD(KC_Z)
#define MOS_RT1 LCMD(KC_V)
#define MOS_RT2 LCMD(KC_C)
#define MOS_RT3 LCMD(KC_X)
#define MOS_RT4 LCMD(KC_Z)
#define MOS_RT5 NAV_NO

#define MOS_LM5 NAV_NO
#define MOS_LM4 KC_LCTL
#define MOS_LM3 KC_LALT
#define MOS_LM2 KC_LGUI
#define MOS_LM1 KC_LSFT
#define MOS_LM0 NAV_NO

#define MOS_RM0 MOS_NO
#define MOS_RM1 KC_MS_L
#define MOS_RM2 KC_MS_D
#define MOS_RM3 KC_MS_U
#define MOS_RM4 KC_MS_R
#define MOS_RM5 MOS_NO

#define MOS_LB5 MOS_NO
#define MOS_LB4 MOS_NO
#define MOS_LB3 MOS_NO
#define MOS_LB2 KC_ALGR
#define MOS_LB1 MOS_NO
#define MOS_LB0 MOS_NO

#define MOS_RB0 MOS_NO
#define MOS_RB1 KC_WH_L
#define MOS_RB2 KC_WH_D
#define MOS_RB3 KC_WH_U
#define MOS_RB4 KC_WH_R
#define MOS_RB5 MOS_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define MOS_LH2 MOS_NO
#define MOS_LH1 MOS_NO
#define MOS_LH0 MOS_NO

#define MOS_RH0 KC_BTN2
#define MOS_RH1 KC_BTN1
#define MOS_RH2 KC_BTN3
