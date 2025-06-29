
bool process_adaptive_key(uint16_t keycode, const keyrecord_t *record) {
    bool result = true;

    if (timer_elapsed(prior_keydown_timer) > RAMO_ADAPTIVE_TERM) { // outside adaptive threshhold
        prior_keycode = prior_keydown_timer = 0; // turn off Adaptives.
        return true; // no adaptive conditions, so return.
    }
    // We're within Adaptive time window

    switch (keycode) {
        case KC_E:
            switch (prior_keycode) {
                case KC_A: // "AE" yields "AU" (8x more common) keeping it on home row
                    tap_code(KC_U);
                    result = false; // done.
                    break;
            }
            break;
    }

    // true: did not trigger adaptive. Continue processing.
    // false: did trigger adaptive. Do not continue.
    return result;
}
/*
PBD -> PWD

PF -> PS

KG -> KL
WMG -> WML
MG -> LG
JG -> JPG
WG -> WD

KH -> KN

GJ -> GTH
VJ -> VL
WJ -> WL

MK -> LK
HK -> NK

GM -> GL
VM -> VL
MWM -> MWL
(.)WM -> (.)LM

FP -> SP

GV -> GT
MV -> LV

GW -> GD
MW -> MP

YB -> IB
AE -> AU
*/
