/*
 * MOS-defs.h
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

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
#define MOS_LTA KC_NO // TODO: should be a real key once a use is found

#define MOS_RTA KC_NO // TODO: should be a real key once a use is found
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
#define MOS_LMA KC_NO // TODO: should be a real key once a use is found

#define MOS_RMA KC_NO // TODO: should be a real key once a use is found
#define MOS_RM0 MOS_NO
#define MOS_RM1 MS_LEFT
#define MOS_RM2 MS_DOWN
#define MOS_RM3 MS_UP
#define MOS_RM4 MS_RGHT
#define MOS_RM5 MOS_NO

#define MOS_LB5 MOS_NO
#define MOS_LB4 MOS_NO
#define MOS_LB3 MOS_NO
#define MOS_LB2 KC_ALGR
#define MOS_LB1 MOS_NO
#define MOS_LB0 MOS_NO

#define MOS_RB0 MOS_NO
#define MOS_RB1 MS_WHLL
#define MOS_RB2 MS_WHLD
#define MOS_RB3 MS_WHLU
#define MOS_RB4 MS_WHLR
#define MOS_RB5 MOS_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define MOS_LH2 MOS_NO
#define MOS_LH1 MOS_NO
#define MOS_LH0 MOS_NO

#define MOS_RH0 MS_BTN2
#define MOS_RH1 MS_BTN1
#define MOS_RH2 MS_BTN3




#define MOS_RGB_LT5 BLACK
#define MOS_RGB_LT4 BLACK
#define MOS_RGB_LT3 BLACK
#define MOS_RGB_LT2 BLACK
#define MOS_RGB_LT1 BLACK
#define MOS_RGB_LT0 BLACK
#define MOS_RGB_LTA BLACK

#define MOS_RGB_RTA BLACK
#define MOS_RGB_RT0 BLACK
#define MOS_RGB_RT1 BLACK
#define MOS_RGB_RT2 BLACK
#define MOS_RGB_RT3 BLACK
#define MOS_RGB_RT4 BLACK
#define MOS_RGB_RT5 BLACK

#define MOS_RGB_LM5 BLACK
#define MOS_RGB_LM4 BLACK
#define MOS_RGB_LM3 BLACK
#define MOS_RGB_LM2 BLACK
#define MOS_RGB_LM1 BLACK
#define MOS_RGB_LM0 BLACK
#define MOS_RGB_LMA BLACK

#define MOS_RGB_RMA BLACK
#define MOS_RGB_RM0 BLACK
#define MOS_RGB_RM1 BLACK
#define MOS_RGB_RM2 BLACK
#define MOS_RGB_RM3 BLACK
#define MOS_RGB_RM4 BLACK
#define MOS_RGB_RM5 BLACK

#define MOS_RGB_LB5 BLACK
#define MOS_RGB_LB4 BLACK
#define MOS_RGB_LB3 BLACK
#define MOS_RGB_LB2 BLACK
#define MOS_RGB_LB1 BLACK
#define MOS_RGB_LB0 BLACK

#define MOS_RGB_RB0 BLACK
#define MOS_RGB_RB1 BLACK
#define MOS_RGB_RB2 BLACK
#define MOS_RGB_RB3 BLACK
#define MOS_RGB_RB4 BLACK
#define MOS_RGB_RB5 BLACK

#define MOS_RGB_LH2 BLACK
#define MOS_RGB_LH1 BLACK
#define MOS_RGB_LH0 BLACK

#define MOS_RGB_RH0 BLACK
#define MOS_RGB_RH1 BLACK
#define MOS_RGB_RH2 BLACK
