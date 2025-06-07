#pragma once

// Override keys with HRM and Layer keys

#define CMK_A LCTL_T(KC_A)
#define CMK_B KC_B
#define CMK_C KC_C
#define CMK_D KC_D
#define CMK_E RGUI_T(KC_E)
#define CMK_F KC_F
#define CMK_G KC_G
#define CMK_H KC_H
#define CMK_I RALT_T(KC_I)
#define CMK_J KC_J
#define CMK_K KC_K
#define CMK_L KC_L
#define CMK_M KC_M
#define CMK_N RSFT_T(KC_N)
#define CMK_O RCTL_T(KC_O)
#define CMK_P KC_P
#define CMK_Q KC_Q
#define CMK_R LALT_T(KC_R)
#define CMK_S LGUI_T(KC_S)
#define CMK_T LSFT_T(KC_T)
#define CMK_U KC_U
#define CMK_V KC_V
#define CMK_W KC_W
#define CMK_X KC_X
#define CMK_Y KC_Y
#define CMK_Z KC_Z


#define CMK_ESC  LT(L_MEDIA, KC_ESC)
#define CMK_SPC  LT(L_NAV,KC_SPC)
#define CMK_TAB  LT(L_MOUSE,KC_TAB)
#define CMK_ENT  LT(L_SYM,KC_ENT)
#define CMK_BSPC LT(L_NUM,KC_BSPC)
#define CMK_DEL  LT(L_FUN,KC_DEL)


#define CMK_QUOT KC_QUOT
#define CMK_COMM KC_COMM
#define CMK_DOT  KC_DOT
#define CMK_SLSH KC_SLSH
#define CMK_HYPR KC_HYPR
#define CMK_MEH  KC_MEH


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
//
//    Base (alpha) Layer  Hands Down Gold (HRMs /+ thumb mods)
//     ╭─────────────────────────╮  ╭─────────────────────────╮
//     │ ESC  Q   W   F   P   G  │  │  J   L   U   Y   '  HYP │
//     │ TAB  A   R   S   T   D  │  │  H   N   E   I   O  MEH │
//     │      Z   X   C   V   B  │  │  K   M   ,   .   /      │
//     ╰───────────╮ ESC SPC TAB │  │ ENT BSP DEL ╭───────────╯
//                 ╰─────────────╯  ╰─────────────╯
//
//qwfpg jluy;
//arstd hneio
//zxcvb km,./

#define CMK_LT5 CMK_ESC
#define CMK_LT4 CMK_Q
#define CMK_LT3 CMK_W
#define CMK_LT2 CMK_F
#define CMK_LT1 CMK_P
#define CMK_LT0 CMK_G

#define CMK_RT0 CMK_J
#define CMK_RT1 CMK_L
#define CMK_RT2 CMK_U
#define CMK_RT3 CMK_Y
#define CMK_RT4 CMK_QUOT
#define CMK_RT5 CMK_HYPR

#define CMK_LM5 CMK_TAB
#define CMK_LM4 CMK_A
#define CMK_LM3 CMK_R
#define CMK_LM2 CMK_S
#define CMK_LM1 CMK_T
#define CMK_LM0 CMK_D

#define CMK_RM0 CMK_H
#define CMK_RM1 CMK_N
#define CMK_RM2 CMK_E
#define CMK_RM3 CMK_I
#define CMK_RM4 CMK_O
#define CMK_RM5 CMK_MEH

#define CMK_LB5 KC_NO
#define CMK_LB4 CMK_Z
#define CMK_LB3 CMK_X
#define CMK_LB2 CMK_C
#define CMK_LB1 CMK_V
#define CMK_LB0 CMK_B

#define CMK_RB0 CMK_K
#define CMK_RB1 CMK_M
#define CMK_RB2 CMK_COMM
#define CMK_RB3 CMK_DOT
#define CMK_RB4 CMK_SLSH
// TEMPORARY
#define CMK_RB5 DF(L_HANDSDOWN)

//Primary Thumbs 1-3 (others are unique to the board)
#define CMK_LH2 CMK_ESC
#define CMK_LH1 CMK_SPC
#define CMK_LH0 CMK_TAB

#define CMK_RH0 CMK_ENT
#define CMK_RH1 CMK_BSPC
#define CMK_RH2 CMK_DEL
