#pragma once

// Let's place these HD keycodes on the keymap
// for variation independent spatial referencing by key position
//
//     Key Position Names for a 42 key split form factor
//        Should cover Corne v4.1
//     ╭───────────────────────────────╮    ╭───────────────────────────────╮
//     │ LT5 LT4 LT3 LT2 LT1 LT0   LTA │    │ RTA   RT0 RT1 RT2 RT3 RT4 RT5 │
//     │ LM5 LM4 LM3 LM2 LM1 LM0   LMA │    │ RMA   RM0 RM1 RM2 RM3 RM4 RM5 │
//     │ LB5 LB4 LB3 LB2 LB1 LB0 ╭─────╯    ╰─────╮ RB0 RB1 RB2 RB3 RB4 RB5 │
//     ╰───────────╮ LH2 LH1 LH0 │                │ RH0 RH1 RH2 ╭───────────╯
//                 ╰─────────────╯                ╰─────────────╯
//
//
//    Base (alpha) Layer Hands Down Promethium (HRMs /+ thumb mods)
//     ╭───────────────────────────────╮    ╭───────────────────────────────╮
//     │ ESC  F   P   D   L   X    LTA │    │ RTA    ;   U   O   Y   B   Z  │
//     │ TAB  S   N   T   H   K    LMA │    │ RMA    ,   A   E   I   C   Q  │
//     │  \   V   W   G   M   J  ╭─────╯    ╰─────╮  -   .   '   =   /  ENT │
//     ╰───────────╮ ESC TAB  R  │                │ SPC ENT BSP ╭───────────╯
//                 ╰─────────────╯                ╰─────────────╯
//

#define HDP_LT5 KC_TAB
#define HDP_LT4 KC_F
#define HDP_LT3 KC_P
#define HDP_LT2 HYPR_T(KC_D)
#define HDP_LT1 MEH_T(KC_L)
#define HDP_LT0 KC_X
#define HDP_LTA KC_NO // TODO: should be a real key once a use is found

#define HDP_RTA KC_NO // TODO: should be a real key once a use is found
#define HDP_RT0 KC_SCLN
#define HDP_RT1 MEH_T(KC_U)
#define HDP_RT2 HYPR_T(KC_O)
#define HDP_RT3 KC_Y
#define HDP_RT4 KC_B
#define HDP_RT5 KC_Z

#define HDP_LM5 KC_TAB
#define HDP_LM4 LCTL_T(KC_S)
#define HDP_LM3 LALT_T(KC_N)
#define HDP_LM2 LGUI_T(KC_T)
#define HDP_LM1 LSFT_T(KC_H)
#define HDP_LM0 KC_K
#define HDP_LMA KC_NO // TODO: should be a real key once a use is found

#define HDP_RMA KC_NO // TODO: should be a real key once a use is found
#define HDP_RM0 KC_COMM
#define HDP_RM1 RSFT_T(KC_A)
#define HDP_RM2 RGUI_T(KC_E)
#define HDP_RM3 RALT_T(KC_I)
#define HDP_RM4 RCTL_T(KC_C)
#define HDP_RM5 KC_Q

#define HDP_LB5 KC_BSLS
#define HDP_LB4 KC_V
#define HDP_LB3 KC_W
#define HDP_LB2 KC_G
#define HDP_LB1 KC_M
#define HDP_LB0 KC_J

#define HDP_RB0 KC_MINS
#define HDP_RB1 KC_DOT
#define HDP_RB2 KC_QUOT
#define HDP_RB3 KC_EQL
#define HDP_RB4 KC_SLSH
#define HDP_RB5 KC_ENT

//Primary Thumbs 1-3 (others are unique to the board)
#define HDP_LH2 LT(L_MEDIA, KC_ESC)
#define HDP_LH1 LT(L_NAV, KC_TAB)
#define HDP_LH0 LT(L_MOUSE, KC_R)
#define HDP_RH0 LT(L_SYM, KC_SPC)
#define HDP_RH1 LT(L_NUM, KC_ENT)
#define HDP_RH2 LT(L_FUN, KC_BSPC)

#define HDP_RGB_LT5 BLACK
#define HDP_RGB_LT4 ORANGE
#define HDP_RGB_LT3 ORANGE
#define HDP_RGB_LT2 ORANGE
#define HDP_RGB_LT1 ORANGE
#define HDP_RGB_LT0 ORANGE
#define HDP_RGB_LTA BLACK

#define HDP_RGB_RTA BLACK
#define HDP_RGB_RT0 ORANGE
#define HDP_RGB_RT1 ORANGE
#define HDP_RGB_RT2 ORANGE
#define HDP_RGB_RT3 ORANGE
#define HDP_RGB_RT4 ORANGE
#define HDP_RGB_RT5 BLACK

#define HDP_RGB_LM5 BLACK
#define HDP_RGB_LM4 ORANGE
#define HDP_RGB_LM3 ORANGE
#define HDP_RGB_LM2 ORANGE
#define HDP_RGB_LM1 ORANGE
#define HDP_RGB_LM0 ORANGE
#define HDP_RGB_LMA BLACK

#define HDP_RGB_RMA BLACK
#define HDP_RGB_RM0 ORANGE
#define HDP_RGB_RM1 ORANGE
#define HDP_RGB_RM2 ORANGE
#define HDP_RGB_RM3 ORANGE
#define HDP_RGB_RM4 ORANGE
#define HDP_RGB_RM5 BLACK

#define HDP_RGB_LB5 BLACK
#define HDP_RGB_LB4 ORANGE
#define HDP_RGB_LB3 ORANGE
#define HDP_RGB_LB2 ORANGE
#define HDP_RGB_LB1 ORANGE
#define HDP_RGB_LB0 ORANGE

#define HDP_RGB_RB0 ORANGE
#define HDP_RGB_RB1 ORANGE
#define HDP_RGB_RB2 ORANGE
#define HDP_RGB_RB3 ORANGE
#define HDP_RGB_RB4 ORANGE
#define HDP_RGB_RB5 BLACK

#define HDP_RGB_LH2 MAGENTA
#define HDP_RGB_LH1 CHARTREUSE
#define HDP_RGB_LH0 CYAN

#define HDP_RGB_RH0 CYAN
#define HDP_RGB_RH1 CHARTREUSE
#define HDP_RGB_RH2 MAGENTA


// type p + l to type q
#define HD_Q_keys HDP_LT3, HDP_LT1

// type n + h  to type z
#define HD_Z_keys HDP_LM3, HDP_LM1

