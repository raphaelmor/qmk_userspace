/*
 * HDP-defs.h
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

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
//    Base (alpha) Layer Hands Down Promethium — canonical arrangement (moutis)
//     ╭───────────────────────────────╮    ╭───────────────────────────────╮
//     │  ·   V   W   G   M   J    LTA │    │ RTA    #   .   /   "   '   ·  │
//     │  ·   S   N   T   H   K    LMA │    │ RMA    ,   A   E   I   C   ·  │
//     │  ·   F   P   D   L   X  ╭─────╯    ╰─────╮  -   U   O   Y   B   ·  │
//     ╰───────────╮ ESC SPC TAB │                │  R  BSP ENT ╭───────────╯
//                 ╰─────────────╯                ╰─────────────╯
//   Home-row mods (my GACS): pinky ⌃ · ring ⌥ · middle ⌘ · index ⇧
//   Kept extras: Hyper on D/O, Meh on L/U · outer columns blank · Option-A thumbs
//

#define HDP_LT5 KC_NO   // outer column unused
#define HDP_LT4 KC_V
#define HDP_LT3 KC_W
#define HDP_LT2 HYPR_T(KC_G)
#define HDP_LT1 MEH_T(KC_M)
#define HDP_LT0 KC_J
#define HDP_LTA KC_NO // TODO: should be a real key once a use is found

#define HDP_RTA KC_NO // TODO: should be a real key once a use is found
#define HDP_RT0 KC_HASH
#define HDP_RT1 MEH_T(KC_DOT)
#define HDP_RT2 HYPR_T(KC_SLSH)
#define HDP_RT3 KC_DQUO
#define HDP_RT4 KC_QUOT
#define HDP_RT5 KC_NO   // outer column unused (Z via N+H combo)

#define HDP_LM5 KC_NO   // outer column unused (Tab is on thumb)
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
#define HDP_RM5 KC_NO   // outer column unused (Q via W+M combo)

#define HDP_LB5 KC_NO   // outer column unused (\ is on NUM layer)
#define HDP_LB4 KC_F
#define HDP_LB3 KC_P
#define HDP_LB2 KC_D
#define HDP_LB1 KC_L
#define HDP_LB0 KC_X

#define HDP_RB0 KC_MINS
#define HDP_RB1 KC_U
#define HDP_RB2 KC_O
#define HDP_RB3 KC_Y
#define HDP_RB4 KC_B
#define HDP_RB5 KC_NO   // outer column unused (Enter is on thumb)

//Primary Thumbs 1-3 (others are unique to the board)
// Option A: thumbs re-aligned to Colemak. Left hand now identical to Colemak;
// only R (inner-right, Promethium's signature) and Enter (outer-right) differ.
// Colemak's Del (outer-right) is not on a thumb here → use Shift+Bksp (see ramo_process_record.c).
#define HDP_LH2 LT(L_MEDIA, KC_ESC)   // Esc   (= Colemak)
#define HDP_LH1 LT(L_NAV, KC_SPC)     // Space (= Colemak)  [was Tab]
#define HDP_LH0 LT(L_MOUSE, KC_TAB)   // Tab   (= Colemak)  [was R]
#define HDP_RH0 LT(L_SYM, KC_R)       // R     (Promethium R-on-thumb) [was Space]
#define HDP_RH1 LT(L_NUM, KC_BSPC)    // Bksp  (= Colemak)  [was Enter]
#define HDP_RH2 LT(L_FUN, KC_ENT)     // Enter (Colemak has Del here)  [was Bksp]

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


// type w + m to type qu (canonical PM position)
#define HD_Q_keys HDP_LT3, HDP_LT1

// type n + h  to type z
#define HD_Z_keys HDP_LM3, HDP_LM1

// Enable the H-digraph roll-combo on Promethium
#define USE_HD_H_DIGRAPH_COMBO
#define HD_Th_keys HDP_LM2, HDP_LM1  // T + H -> "th"

