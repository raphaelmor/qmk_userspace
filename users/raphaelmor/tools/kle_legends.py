# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Raphaël Mor
"""Display data for kle_export.py: keycode legends and hand-written notes.

Everything here is presentation. The structure of the keymap comes from the C
sources; this module only decides how a resolved keycode should read on a keycap.
"""

# --- Key appearance ----------------------------------------------------------
# No colour. One neutral cap for everything, and a ghosted cap for a position
# that has nothing bound to it, so the shape of each layer reads at a glance.
# The greys are the ones from the original hand-made
# corne-v4-1-miryoku-adaptation.json.

KEY_COLOR = "#424242"
KEY_TEXT = "#ffffff"
# The key you hold to reach a layer, shown on that layer's own board. Ghosted,
# because it is how you got here rather than something the layer binds.
ACTIVATOR_COLOR = "#8c5656"

UNBOUND = {"KC_NO", "KC_TRNS"}


def is_unbound(keycode: str) -> bool:
    return keycode.strip() in UNBOUND


# --- Board chrome ------------------------------------------------------------

BOARD_BG = "#626262"
TITLE_COLOR = "#424242"   # a decal draws no cap, so this only matters if d is off
TITLE_TEXT = "#ffffff"


# --- Keycode legends ---------------------------------------------------------
# A legend is either a string (one centred label) or a (top, bottom) pair for a
# key whose shifted glyph is worth showing.

BASIC: dict[str, object] = {
    "KC_NO": "",
    "KC_TRNS": "▽",

    # letters and digits fill themselves in below
    "KC_QUOT": ('"', "'"),
    "KC_COMM": ("<", ","),
    "KC_DOT": (">", "."),
    "KC_SLSH": ("?", "/"),
    "KC_SCLN": (":", ";"),
    "KC_MINS": ("_", "-"),
    "KC_EQL": ("+", "="),
    "KC_LBRC": ("{", "["),
    "KC_RBRC": ("}", "]"),
    "KC_BSLS": ("|", "\\"),
    "KC_GRV": ("~", "`"),

    "KC_DQUO": '"',
    "KC_COLN": ":",
    "KC_UNDS": "_",
    "KC_PLUS": "+",
    "KC_TILD": "~",
    "KC_PIPE": "|",
    "KC_EXLM": "!",
    "KC_QUES": "?",
    "KC_AT": "@",
    "KC_HASH": "#",
    "KC_DLR": "$",
    "KC_PERC": "%",
    "KC_CIRC": "^",
    "KC_AMPR": "&",
    "KC_ASTR": "*",
    "KC_LPRN": "(",
    "KC_RPRN": ")",
    "KC_LCBR": "{",
    "KC_RCBR": "}",

    "KC_ESC": "Esc",
    "KC_TAB": "Tab",
    "KC_SPC": "␣",
    "KC_ENT": "⏎",
    "KC_BSPC": "⌫",
    "KC_DEL": "⌦",
    "KC_INS": "Ins",
    "KC_APP": "▤",
    "KC_PSCR": "PrtSc",
    "KC_SCRL": "ScrLk",
    "KC_PAUS": "Pause",

    "KC_LEFT": "←",
    "KC_DOWN": "↓",
    "KC_UP": "↑",
    "KC_RGHT": "→",
    "KC_RIGHT": "→",
    "KC_HOME": "Home",
    "KC_END": "End",
    "KC_PGUP": "PgUp",
    "KC_PGDN": "PgDn",

    "KC_LCTL": "⌃",
    "KC_RCTL": "⌃",
    "KC_LALT": "⌥",
    "KC_RALT": "⌥",
    "KC_ALGR": "AltGr",
    "KC_LGUI": "⌘",
    "KC_RGUI": "⌘",
    "KC_LSFT": "⇧",
    "KC_RSFT": "⇧",
    "KC_HYPR": "Hyper",
    "KC_MEH": "Meh",

    "KC_MPRV": "Prev",
    "KC_MNXT": "Next",
    "KC_MPLY": "Play",
    "KC_MSTP": "Stop",
    "KC_MUTE": "Mute",
    "KC_VOLD": "Vol−",
    "KC_VOLU": "Vol+",

    "MS_LEFT": "Ms ←",
    "MS_DOWN": "Ms ↓",
    "MS_UP": "Ms ↑",
    "MS_RGHT": "Ms →",
    "MS_WHLL": "Wh ←",
    "MS_WHLD": "Wh ↓",
    "MS_WHLU": "Wh ↑",
    "MS_WHLR": "Wh →",
    "MS_BTN1": "LMB",
    "MS_BTN2": "RMB",
    "MS_BTN3": "MMB",

    "RM_TOGG": "RGB on",
    "RM_NEXT": "RGB mode",
    "RM_HUEU": "RGB hue",
    "RM_SATU": "RGB sat",
    "RM_VALU": "RGB val",

    "CW_TOGG": "Caps Word",
    "QK_BOOT": "Boot",
}

for _i in range(26):
    BASIC["KC_" + chr(ord("A") + _i)] = chr(ord("A") + _i)
for _d, _shifted in zip("1234567890", "!@#$%^&*()"):
    BASIC["KC_" + _d] = (_shifted, _d)
for _n in range(1, 25):
    BASIC[f"KC_F{_n}"] = f"F{_n}"

# Modifier wrappers: keycode prefix -> the symbol shown on the front face.
MOD_TAPS = {
    "LCTL_T": "⌃", "RCTL_T": "⌃",
    "LALT_T": "⌥", "RALT_T": "⌥",
    "LGUI_T": "⌘", "RGUI_T": "⌘",
    "LSFT_T": "⇧", "RSFT_T": "⇧",
    "HYPR_T": "Hyper", "MEH_T": "Meh",
    "ALL_T": "Hyper",
}

# Chord wrappers: keycode prefix -> prefix string put in front of the inner legend.
CHORDS = {
    "LCTL": "⌃", "C": "⌃",
    "LALT": "⌥", "A": "⌥",
    "LGUI": "⌘", "G": "⌘", "LCMD": "⌘",
    "LSFT": "⇧", "S": "⇧",
    "SCMD": "⇧⌘", "SGUI": "⇧⌘",
    "RCTL": "⌃", "RALT": "⌥", "RGUI": "⌘", "RSFT": "⇧",
    "HYPR": "Hyper+", "MEH": "Meh+",
}

# Layer enum -> short name shown on a layer-tap front face.
LAYER_NAMES = {
    "L_COLEMAK": "Colemak",
    "L_HANDSDOWN": "HandsDown",
    "L_MEDIA": "Media",
    "L_NAV": "Nav",
    "L_MOUSE": "Mouse",
    "L_SYM": "Sym",
    "L_NUM": "Num",
    "L_FUN": "Fun",
}

# Tap-dance enum -> what a double tap does. Short: this goes on a 1u cap.
TAP_DANCES = {
    "RAMO_TD_CMK": "2× CMK",
    "RAMO_TD_HD": "2× HDP",
}


# --- Hand-written notes ------------------------------------------------------
# COMBO_ACTION combos run C code, so their effect cannot be read off key_combos[].
# Keyed by the enum name in ramo_combos.c.

COMBO_ACTION_NOTES = {
    "HD_COMBO_Th": "th",
    "CMK_COMBO_Th": "th",
    "HD_COMBO_QU": "qu",
}

# What each combo produces. Kept short — these go on a 1u keycap. The dead-key
# combos show the dead key and the letter it makes: "´é" is ⌥E, then e.
COMBO_RESULT_NOTES = {
    "HD_COMBO_ACUT": "´é",
    "HD_COMBO_GRV": "`è",
    "HD_COMBO_CIRC": "ˆê",
    "HD_COMBO_UMLA": "¨ü",
    "HD_COMBO_TILDE": "˜ñ",
    "HD_COMBO_CEDIL": "ç",
    "HD_COMBO_OE": "œ",
    "HD_COMBO_AE": "æ",
    "HD_COMBO_STAB": "⇧Tab",
    "HD_COMBO_APP": "▤",
    "HD_COMBO_CAPW": "CapsW",
}

# The long form, for the list board. Anything absent falls back to the short form.
COMBO_LONG_NOTES = {
    "HD_COMBO_ACUT": "acute dead key (⌥E) — é",
    "HD_COMBO_GRV": "grave dead key (⌥`) — è",
    "HD_COMBO_CIRC": "circumflex dead key (⌥I) — ê",
    "HD_COMBO_UMLA": "umlaut dead key (⌥U) — ü",
    "HD_COMBO_TILDE": "tilde dead key (⌥N) — ñ",
    "HD_COMBO_CEDIL": "ç (⌥C)",
    "HD_COMBO_OE": "œ (⌥Q)",
    "HD_COMBO_AE": "æ (⌥')",
    "HD_COMBO_STAB": "Shift-Tab",
    "HD_COMBO_APP": "App menu",
    "HD_COMBO_QU": "qu — hold past 200 ms to drop the u and leave q",
    "HD_COMBO_CAPW": "Caps Word on or off",
}

# Behaviors that are always on. These live in process_record / matrix_scan, not in
# any table a parser could read, so they are transcribed here.
#
# The last two fields are a file and a string to find in it. The generator turns
# them into a file:line anchor, so the line number cannot rot when the file moves
# — and if the string ever disappears, the export stops rather than lying.
# {TAPPING_TERM} is filled in from config.h.
ALWAYS_ON = [
    ("Linger brackets",
     "Tap ( or {{ for one bracket. Hold past {TAPPING_TERM} ms for the pair, "
     "caret inside: (|)",
     "ramo_linger.c", "bool linger_track("),
    ("Caps Word",
     "Letters get ⇧. Digits, -, _, ⌫ keep it on. "
     "Space → _, double space → real space + off.",
     "ramo_linger.c", "bool caps_word_press_user("),
    ("Shift + ⌫ = ⌦",
     "Keeps Delete reachable on Hands Down, where it left the thumb row.",
     "ramo_process_record.c", "keycode == KC_BSPC"),
    ("Comma-leader shift",
     "Hands Down only. Type , then a letter within {TAPPING_TERM} ms → "
     "the comma goes, the letter gets ⇧.",
     "ramo_process_record.c", "prior_keycode == KC_COMM"),
    ("Combo hold on qu",
     "Hold the W+M combo past {TAPPING_TERM} ms and the u is deleted, leaving q.",
     "ramo_combos.c", "void combo_check_for_linger("),
    ("Base switch",
     "NAV layer, two left keys. Double-tap only, so a stray press cannot "
     "change your base.",
     "ramo_tap_dance.c", "ramo_TD_make_colemak_default"),
]
