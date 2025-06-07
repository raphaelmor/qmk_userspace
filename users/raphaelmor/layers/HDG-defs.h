#pragma once

// Override keys with HRM and Layer keys

#define HDG_A RSFT_T(KC_A)
#define HDG_B KC_B
#define HDG_C KC_C
#define HDG_D LSFT_T(KC_D)
#define HDG_E RGUI_T(KC_E)
#define HDG_F KC_F
#define HDG_G KC_G
#define HDG_H RCTL_T(KC_H)
#define HDG_I RALT_T(KC_I)
#define HDG_J KC_J
#define HDG_K KC_K
#define HDG_L KC_L
#define HDG_M HYPR_T(KC_M)
#define HDG_N LGUI_T(KC_N)
#define HDG_O KC_O
#define HDG_P MEH_T(KC_P)
#define HDG_Q KC_Q
#define HDG_R LCTL_T(KC_R)
#define HDG_S LALT_T(KC_S)
#define HDG_T LT(L_MOUSE,KC_T)
#define HDG_U KC_U
#define HDG_V KC_V
#define HDG_W KC_W
#define HDG_X KC_X
#define HDG_Y KC_Y
#define HDG_Z KC_Z

#define HDG_SCLN KC_SCLN
#define HDG_DOT  MEH_T(KC_DOT)
#define HDG_SLSH HYPR_T(KC_SLSH)
#define HDG_DQUO KC_DQUO
#define HDG_HYPR KC_HYPR
#define HDG_QUOT KC_QUOT
#define HDG_COMM KC_COMM
#define HDG_MINS KC_MINS
#define HDG_MEH  KC_MEH

#define HDG_TAB  KC_TAB
#define HDG_ESC  LT(L_MEDIA, KC_ESC)
#define HDG_SPC  LT(L_NAV,KC_SPC)
#define HDG_ENT  LT(L_SYM,KC_ENT)
#define HDG_BSPC LT(L_NUM,KC_BSPC)
#define HDG_DEL  LT(L_FUN,KC_DEL)

// Now let's place these HD keycodes on the keymap
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
//
//    Base (alpha) Layer Hands Down Gold (HRMs /+ thumb mods)
//     ╭─────────────────────────╮  ╭─────────────────────────╮
//     │ ESC  J   G   M   P   V  │  │  ;   .   /   "   '  HYP │
//     │ TAB  R   S   N   D   B  │  │  ,   A   E   I   H  MEH │
//     │  Z   X   F   L   C   W  │  │  -   U   O   Y   K   Q  │
//     ╰───────────╮ ESC SPC  T  │  │ ENT BSP DEL ╭───────────╯
//                 ╰─────────────╯  ╰─────────────╯
//

#define HDG_LT5 HDG_ESC
#define HDG_LT4 HDG_J
#define HDG_LT3 HDG_G
#define HDG_LT2 HDG_M
#define HDG_LT1 HDG_P
#define HDG_LT0 HDG_V
#define HDG_RT0 HDG_SCLN
#define HDG_RT1 HDG_DOT
#define HDG_RT2 HDG_SLSH
#define HDG_RT3 HDG_DQUO
#define HDG_RT4 HDG_QUOT
// #define HDG_RT5 HDG_HYPR
#define HDG_RT5 DF(L_COLEMAK)

#define HDG_LM5 HDG_TAB
#define HDG_LM4 HDG_R
#define HDG_LM3 HDG_S
#define HDG_LM2 HDG_N
#define HDG_LM1 HDG_D
#define HDG_LM0 HDG_B
#define HDG_RM0 HDG_COMM
#define HDG_RM1 HDG_A
#define HDG_RM2 HDG_E
#define HDG_RM3 HDG_I
#define HDG_RM4 HDG_H
#define HDG_RM5 HDG_MEH

#define HDG_LB5 HDG_Z
#define HDG_LB4 HDG_X
#define HDG_LB3 HDG_F
#define HDG_LB2 HDG_L
#define HDG_LB1 HDG_C
#define HDG_LB0 HDG_W
#define HDG_RB0 HDG_MINS
#define HDG_RB1 HDG_U
#define HDG_RB2 HDG_O
#define HDG_RB3 HDG_Y
#define HDG_RB4 HDG_K
#define HDG_RB5 HDG_Q

//Primary Thumbs 1-3 (others are unique to the board)
#define HDG_LH2 HDG_ESC
#define HDG_LH1 HDG_SPC
#define HDG_LH0 HDG_T
#define HDG_RH0 HDG_ENT
#define HDG_RH1 HDG_BSPC
#define HDG_RH2 HDG_DEL
