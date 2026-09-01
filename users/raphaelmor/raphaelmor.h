/*
 * raphaelmor.h
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (c) 2026 Raphaël Mor
 *
 * Part of a QMK userspace whose design follows Hands Down Promethium by moutis
 * (https://github.com/moutis/HandsDown, GPL-3.0). See ./LICENSE.
 */

#pragma once

#include "ramo_tap_dance.h"

#include "ramo_layers.h"

// --- Firmware-verification logging ------------------------------------------
// Log the ACTUAL keycodes/strings sent to the host, as distinct from the
// "Pressed kc" logged in process_record_user (which is the keymap keycode —
// the intent — before combos/adaptives/linger transform it). This lets the
// touchtutor `tt verify` tool cross-check "what was pressed" vs "what was sent"
// and catch transformation bugs, including for keys that emit no character.
//
// Wrap tap_code/tap_code16/send_string at the userspace emission sites (combos,
// adaptives, linger, and the process_record transforms). Plain keys are sent by
// core QMK unchanged, so their output already equals the logged "Pressed kc".
//
// Uses uprintf (CONSOLE_ENABLE) + get_keycode_string (KEYCODE_STRING_ENABLE),
// both already enabled in rules.mk. Define RAMO_NO_SENT_LOG for a quiet build.
#ifdef RAMO_NO_SENT_LOG
#    define RAMO_TAP(kc)         tap_code(kc)
#    define RAMO_TAP16(kc)       tap_code16(kc)
#    define RAMO_SEND_STRING(s)  send_string(s)
#else
#    define RAMO_TAP(kc)         do { uprintf("RAMO: SENT: %s\n", get_keycode_string(kc)); tap_code(kc);   } while (0)
#    define RAMO_TAP16(kc)       do { uprintf("RAMO: SENT: %s\n", get_keycode_string(kc)); tap_code16(kc); } while (0)
#    define RAMO_SEND_STRING(s)  do { uprintf("RAMO: SENT_STR: %s\n", s); send_string(s); } while (0)
#endif

