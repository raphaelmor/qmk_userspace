#pragma once

// Override keys with HRM and Layer keys

#define SYM_NO KC_NO

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

#define SYM_LT5 SYM_NO
#define SYM_LT4 KC_LCBR
#define SYM_LT3 KC_AMPR
#define SYM_LT2 KC_ASTR
#define SYM_LT1 KC_LPRN
#define SYM_LT0 KC_RCBR

#define SYM_RT0 SYM_NO
#define SYM_RT1 SYM_NO
#define SYM_RT2 SYM_NO
#define SYM_RT3 SYM_NO
#define SYM_RT4 SYM_NO
#define SYM_RT5 SYM_NO

#define SYM_LM5 SYM_NO
#define SYM_LM4 KC_COLN
#define SYM_LM3 KC_DLR
#define SYM_LM2 KC_PERC
#define SYM_LM1 KC_CIRC
#define SYM_LM0 KC_PLUS

#define SYM_RM0 SYM_NO
#define SYM_RM1 KC_RSFT
#define SYM_RM2 KC_RGUI
#define SYM_RM3 KC_RALT
#define SYM_RM4 KC_RCTL
#define SYM_RM5 SYM_NO

#define SYM_LB5 SYM_NO
#define SYM_LB4 KC_TILD
#define SYM_LB3 KC_EXLM
#define SYM_LB2 KC_AT
#define SYM_LB1 KC_HASH
#define SYM_LB0 KC_PIPE

#define SYM_RB0 SYM_NO
#define SYM_RB1 SYM_NO
#define SYM_RB2 SYM_NO
#define SYM_RB3 KC_ALGR
#define SYM_RB4 SYM_NO
#define SYM_RB5 SYM_NO

//Primary Thumbs 1-3 (others are unique to the board)
#define SYM_LH2 KC_LPRN
#define SYM_LH1 KC_RPRN
#define SYM_LH0 KC_UNDS

#define SYM_RH0 SYM_NO
#define SYM_RH1 SYM_NO
#define SYM_RH2 SYM_NO
