#pragma once

// Override keys with HRM and Layer keys

#define FUN_NO KC_NO

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

#define FUN_LT5 FUN_NO
#define FUN_LT4 KC_F12
#define FUN_LT3 KC_F7
#define FUN_LT2 KC_F8
#define FUN_LT1 KC_F9
#define FUN_LT0 KC_PSCR

#define FUN_RT0 FUN_NO
#define FUN_RT1 FUN_NO
#define FUN_RT2 FUN_NO
#define FUN_RT3 FUN_NO
#define FUN_RT4 FUN_NO
#define FUN_RT5 FUN_NO

#define FUN_LM5 FUN_NO
#define FUN_LM4 KC_F11
#define FUN_LM3 KC_F4
#define FUN_LM2 KC_F5
#define FUN_LM1 KC_F6
#define FUN_LM0 KC_SCRL
#define FUN_RM0 FUN_NO
#define FUN_RM1 KC_RSFT
#define FUN_RM2 KC_RGUI
#define FUN_RM3 KC_RALT
#define FUN_RM4 KC_RCTL
#define FUN_RM5 FUN_NO

#define FUN_LB5 FUN_NO
#define FUN_LB4 KC_F10
#define FUN_LB3 KC_F1
#define FUN_LB2 KC_F2
#define FUN_LB1 KC_F3
#define FUN_LB0 KC_PAUS
#define FUN_RB0 FUN_NO
#define FUN_RB1 FUN_NO
#define FUN_RB2 FUN_NO
#define FUN_RB3 KC_ALGR
#define FUN_RB4 FUN_NO
#define FUN_RB5 FUN_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define FUN_LH2 KC_APP
#define FUN_LH1 KC_SPC
#define FUN_LH0 KC_TAB

#define FUN_RH0 FUN_NO
#define FUN_RH1 FUN_NO
#define FUN_RH2 FUN_NO
