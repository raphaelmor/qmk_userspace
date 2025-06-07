#pragma once

#include "tap_dance.h"

#include "layers/CMK-defs.h"
#include "layers/HDG-defs.h"
#include "layers/MED-defs.h"
#include "layers/NAV-defs.h"
#include "layers/MOS-defs.h"
#include "layers/NUM-defs.h"
#include "layers/SYM-defs.h"
#include "layers/FUN-defs.h"

#define RAMO_FOR_EACH_LAYER \
RAMO_DO(COLEMAK,      CMK) \
RAMO_DO(HANDSDOWN,    HDG) \
RAMO_DO(NAV,          NAV) \
RAMO_DO(MOUSE,        MOS) \
RAMO_DO(MEDIA,        MED) \
RAMO_DO(NUM,          NUM) \
RAMO_DO(SYM,          SYM) \
RAMO_DO(FUN,          FUN)


enum ramo_layers {
#define RAMO_DO(LAYER, STRING) L_##LAYER,
RAMO_FOR_EACH_LAYER
#undef RAMO_DO
};

