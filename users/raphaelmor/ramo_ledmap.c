
// Macro that defines a ledmap per layer (e.g. CMK_RGB_LT5, NAV_RGB_LB1)
// Then each layer simply has to define the derired color per key
// e.g. #define CMK_RGB_LT5 RED
const ledmap_color_t PROGMEM ledmap[][MATRIX_ROWS][MATRIX_COLS] = {
#define RAMO_DO(LAYER, STR) [L_##LAYER] = LAYOUT_split_3x6_3_ex2( \
      STR##_RGB_LT5 , STR##_RGB_LT4 , STR##_RGB_LT3 , STR##_RGB_LT2 , STR##_RGB_LT1 , STR##_RGB_LT0 , STR##_RGB_LTA ,      STR##_RGB_RTA , STR##_RGB_RT0 , STR##_RGB_RT1 , STR##_RGB_RT2 , STR##_RGB_RT3 , STR##_RGB_RT4 , STR##_RGB_RT5 , \
      STR##_RGB_LM5 , STR##_RGB_LM4 , STR##_RGB_LM3 , STR##_RGB_LM2 , STR##_RGB_LM1 , STR##_RGB_LM0 , STR##_RGB_LMA ,      STR##_RGB_RMA , STR##_RGB_RM0 , STR##_RGB_RM1 , STR##_RGB_RM2 , STR##_RGB_RM3 , STR##_RGB_RM4 , STR##_RGB_RM5 , \
      STR##_RGB_LB5 , STR##_RGB_LB4 , STR##_RGB_LB3 , STR##_RGB_LB2 , STR##_RGB_LB1 , STR##_RGB_LB0 ,                              STR##_RGB_RB0 , STR##_RGB_RB1 , STR##_RGB_RB2 , STR##_RGB_RB3 , STR##_RGB_RB4 , STR##_RGB_RB5 , \
                                                      STR##_RGB_LH2 , STR##_RGB_LH1 , STR##_RGB_LH0 ,                              STR##_RGB_RH0 , STR##_RGB_RH1 , STR##_RGB_RH2 \
  ),
RAMO_FOR_EACH_LAYER
#undef RAMO_DO
};
