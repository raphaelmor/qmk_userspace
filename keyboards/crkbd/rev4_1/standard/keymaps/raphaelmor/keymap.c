/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "raphaelmor.h"
//
// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     [L_COLEMAK] = LAYOUT_split_3x6_3(
//   //,-------------------------------------------------.  ,------------------------------------------------.
//       CM_LT5, CM_LT4, CM_LT3, CM_LT2, CM_LT1, CM_LT0,      CM_RT0, CM_RT1, CM_RT2, CM_RT3, CM_RT4, CM_RT5,
//   //|-------+-------+-------+-------+-------+---------|  |-------+-------+-------+-------+-------+--------|
//       CM_LM5, CM_LM4, CM_LM3, CM_LM2, CM_LM1, CM_LM0,      CM_RM0, CM_RM1, CM_RM2, CM_RM3, CM_RM4, CM_RM5,
//   //|-------+-------+-------+-------+-------+---------|  |-------+-------+-------+-------+-------+--------|
//       CM_LB5, CM_LB4, CM_LB3, CM_LB2, CM_LB1, CM_LB0,      CM_RB0, CM_RB1, CM_RB2, CM_RB3, CM_RB4, CM_RB5,
//   //|-------+-------+-------+-------+-------+---------|  |-------+-------+-------+-------+-------+--------|
//                               CM_LH2, CM_LH1, CM_LH0,      CM_RH0, CM_RH1, CM_RH2
//                             //`-----------------------'  `-----------------------'
//   ),
//     [L_HANDSDOWN] = LAYOUT_split_3x6_3(
//   //,-------------------------------------------------.  ,------------------------------------------------.
//       HD_LT5, HD_LT4, HD_LT3, HD_LT2, HD_LT1, HD_LT0,      HD_RT0, HD_RT1, HD_RT2, HD_RT3, HD_RT4, HD_RT5,
//   //|-------+-------+-------+-------+-------+---------|  |-------+-------+-------+-------+-------+--------|
//       HD_LM5, HD_LM4, HD_LM3, HD_LM2, HD_LM1, HD_LM0,      HD_RM0, HD_RM1, HD_RM2, HD_RM3, HD_RM4, HD_RM5,
//   //|-------+-------+-------+-------+-------+---------|  |-------+-------+-------+-------+-------+--------|
//       HD_LB5, HD_LB4, HD_LB3, HD_LB2, HD_LB1, HD_LB0,      HD_RB0, HD_RB1, HD_RB2, HD_RB3, HD_RB4, HD_RB5,
//   //|-------+-------+-------+-------+-------+---------|  |-------+-------+-------+-------+-------+--------|
//                               HD_LH2, HD_LH1, HD_LH0,      HD_RH0, HD_RH1, HD_RH2
//                             //`-----------------------'  `-----------------------'
//   ),
//
// }
//
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#define RAMO_DO(LAYER, STR) [L_##LAYER] = LAYOUT_split_3x6_3( \
      STR##_LT5 , STR##_LT4 , STR##_LT3 , STR##_LT2 , STR##_LT1 , STR##_LT0 ,      STR##_RT0 , STR##_RT1 , STR##_RT2 , STR##_RT3 , STR##_RT4 , STR##_RT5 , \
      STR##_LM5 , STR##_LM4 , STR##_LM3 , STR##_LM2 , STR##_LM1 , STR##_LM0 ,      STR##_RM0 , STR##_RM1 , STR##_RM2 , STR##_RM3 , STR##_RM4 , STR##_RM5 , \
      STR##_LB5 , STR##_LB4 , STR##_LB3 , STR##_LB2 , STR##_LB1 , STR##_LB0 ,      STR##_RB0 , STR##_RB1 , STR##_RB2 , STR##_RB3 , STR##_RB4 , STR##_RB5 , \
                                          STR##_LH2 , STR##_LH1 , STR##_LH0 ,      STR##_RH0 , STR##_RH1 , STR##_RH2 \
  ),
RAMO_FOR_EACH_LAYER
#undef RAMO_DO
};
