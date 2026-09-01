/*
 * config.h
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

#pragma once

// default but used in macros
#undef TAPPING_TERM
#define TAPPING_TERM 200

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define QUICK_TAP_TERM 0

// Gap between register/unregister in tap_code(). At the default 0, back-to-back
// taps in the adaptives (del-then-retype, e.g. MG -> LG) can land in the same USB
// frame and go missing or out of sequence. 20 is moutis's value.
#undef TAP_CODE_DELAY
#define TAP_CODE_DELAY 20

// Mouse key speed and acceleration.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY          0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL       16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY    0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED      6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX    64

#define SPLIT_LAYER_STATE_ENABLE


// RAMO CUSTOM CONFIG
#define RAMO_COMBO_HOLD (TAPPING_TERM) // time to hold to trigger delayed combo
#define RAMO_LINGER_TERM (TAPPING_TERM) // hold time to auto-pair brackets
#define RAMO_USE_HD_PROMETHIUM // (Gold is the default)
// Adaptive rolling window: how long after a keydown the next key can still trigger
// an adaptive. Measured keydown-to-keydown (see prior_keydown_timer / event.time).
//   50ms  (TAPPING_TERM/4) = ~240 WPM — roll-only; missed most rules in practice
//   100ms                  = ~120 WPM — fine for fast rolls (PD, WG), but SFB and
//                            cross-row rules (KH same-finger, KG inner-column
//                            stretch) are inherently slower and never fired
//   175ms                  = ~69 WPM — current. Adaptives exist to fix SFBs and
//                            scissors, which are *slow by definition*, so the
//                            window has to clear those motions.
//   229ms (moutis, COMBO_HOLD*1.35) = ~52 WPM
// Collisions to watch at 175ms (same-row rolls in real words): GM in
// "pragma"/"progmem", PD in "update", YB in "keyboard", KG in "background".
// #define RAMO_ADAPTIVE_TERM (TAPPING_TERM/4) // original: 50ms
// #define RAMO_ADAPTIVE_TERM 100              // too tight for SFB rules
#define RAMO_ADAPTIVE_TERM 175


#define RAMO_ADAPT_SHIFT_TERM (TAPPING_TERM) // comma-leader one-shot-shift window (wider than adaptives)


// Disable led stuff
#undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_BREATHING
#undef ENABLE_RGB_MATRIX_BAND_SAT
#undef ENABLE_RGB_MATRIX_BAND_VAL
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#undef ENABLE_RGB_MATRIX_CYCLE_ALL
#undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#undef ENABLE_RGB_MATRIX_DUAL_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#undef ENABLE_RGB_MATRIX_FLOWER_BLOOMING
#undef ENABLE_RGB_MATRIX_RAINDROPS
#undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#undef ENABLE_RGB_MATRIX_HUE_BREATHING
#undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#undef ENABLE_RGB_MATRIX_HUE_WAVE
#undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#undef ENABLE_RGB_MATRIX_PIXEL_RAIN

#undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#undef ENABLE_RGB_MATRIX_DIGITAL_RAIN

#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#undef ENABLE_RGB_MATRIX_SPLASH
#undef ENABLE_RGB_MATRIX_MULTISPLASH
#undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
