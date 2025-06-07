#pragma once

// Override keys with HRM and Layer keys

#define NAV_NO KC_NO

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

#define NAV_LT5 NAV_NO
#define NAV_LT4 NAV_NO
#define NAV_LT3 NAV_NO
#define NAV_LT2 TD(RAMO_TD_CMK)
#define NAV_LT1 TD(RAMO_TD_HDG)
#define NAV_LT0 NAV_NO

#define NAV_RT0 SCMD(KC_Z)
#define NAV_RT1 LCMD(KC_V)
#define NAV_RT2 LCMD(KC_C)
#define NAV_RT3 LCMD(KC_X)
#define NAV_RT4 LCMD(KC_Z)
#define NAV_RT5 NAV_NO

#define NAV_LM5 NAV_NO
#define NAV_LM4 KC_LCTL
#define NAV_LM3 KC_LALT
#define NAV_LM2 KC_LGUI
#define NAV_LM1 KC_LSFT
#define NAV_LM0 NAV_NO

#define NAV_RM0 CW_TOGG
#define NAV_RM1 KC_LEFT
#define NAV_RM2 KC_DOWN
#define NAV_RM3 KC_UP
#define NAV_RM4 KC_RIGHT
#define NAV_RM5 NAV_NO

#define NAV_LB5 NAV_NO
#define NAV_LB4 NAV_NO
#define NAV_LB3 NAV_NO
#define NAV_LB2 KC_ALGR
#define NAV_LB1 NAV_NO
#define NAV_LB0 NAV_NO

#define NAV_RB0 KC_INS
#define NAV_RB1 KC_HOME
#define NAV_RB2 KC_PGDN
#define NAV_RB3 KC_PGUP
#define NAV_RB4 KC_END
#define NAV_RB5 NAV_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define NAV_LH2 NAV_NO
#define NAV_LH1 NAV_NO
#define NAV_LH0 NAV_NO

#define NAV_RH0 KC_ENT
#define NAV_RH1 KC_BSPC
#define NAV_RH2 KC_DEL
