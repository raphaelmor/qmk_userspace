#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Raphaël Mor
"""Render the QMK keymap in users/raphaelmor as a keyboard-layout-editor document.

The keymap is built out of X-macros, so the only honest way to read it is to run
the C preprocessor over it exactly as the firmware build does. This tool does
that, then lays the result out as one KLE document: a board per layer, plus
boards for the combos, the adaptive rules, and the always-on behaviors.

    python3 tools/kle_export.py                 # writes corne-v4-1-ramo.json
    python3 tools/kle_export.py -o /tmp/x.json  # somewhere else
    python3 tools/kle_export.py --gold          # render Hands Down Gold instead

Paste the output into keyboard-layout-editor.com under Raw data.

There are no unit tests. Instead every structural assumption is a check() call
that aborts the export, so a wrong file never reaches disk.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import kle_legends as L  # noqa: E402

USERSPACE = Path(__file__).resolve().parent.parent
KEYMAP_C = (USERSPACE.parent.parent / "keyboards/crkbd/rev4_1/standard"
            / "keymaps/raphaelmor/keymap.c")


class ExportError(Exception):
    """A structural assumption about the C sources no longer holds."""


def check(condition: bool, message: str) -> None:
    if not condition:
        raise ExportError(message)


# --- Geometry ----------------------------------------------------------------
# Positions are named by the diagram every layer header carries:
#
#     ╭───────────────────────────────╮    ╭───────────────────────────────╮
#     │ LT5 LT4 LT3 LT2 LT1 LT0   LTA │    │ RTA   RT0 RT1 RT2 RT3 RT4 RT5 │
#     │ LM5 LM4 LM3 LM2 LM1 LM0   LMA │    │ RMA   RM0 RM1 RM2 RM3 RM4 RM5 │
#     │ LB5 LB4 LB3 LB2 LB1 LB0 ╭─────╯    ╰─────╮ RB0 RB1 RB2 RB3 RB4 RB5 │
#     ╰───────────╮ LH2 LH1 LH0 │                │ RH0 RH1 RH2 ╭───────────╯
#                 ╰─────────────╯                ╰─────────────╯

ROWS = ["T", "M", "B"]
LEFT_COLS = ["5", "4", "3", "2", "1", "0"]
RIGHT_COLS = ["0", "1", "2", "3", "4", "5"]

# The board has 46 switches, but 10 of them carry nothing on any layer: the outer
# pinky column (LT5 LM5 LB5 RT5 RM5 RB5) and the four ex2 inner keys (LTA LMA RTA
# RMA). They are not drawn. What is left is a plain 3x5 + 3 per half.
SKIP_COLS = {"5"}
SKIP_POSITIONS = {"LTA", "LMA", "RTA", "RMA"}
DRAWN_LEFT_COLS = [c for c in LEFT_COLS if c not in SKIP_COLS]
DRAWN_RIGHT_COLS = [c for c in RIGHT_COLS if c not in SKIP_COLS]

# Column stagger, decoded from the hand-made corne-v4-1-miryoku-adaptation.json.
# Index matches DRAWN_LEFT_COLS: ring first, index inner last.
COL_Y = [0.3, 0.1, 0.0, 0.1, 0.2]
ROW_PITCH = 1.0
HALF_GAP = 3.0         # blank units between the two halves
RIGHT_X0 = len(DRAWN_LEFT_COLS) + HALF_GAP
THUMB_DX = 0.5         # thumbs sit half a unit inboard of the columns above them
THUMB_Y = 3.35
BOARD_PITCH = 6.6      # vertical distance between two stacked boards
TITLE_Y = 2.6          # the wide title decal sits in the gap, bottom-row height


def build_geometry() -> dict[str, tuple[float, float]]:
    """Position name -> (x, y) in key units, relative to the board origin."""
    geo: dict[str, tuple[float, float]] = {}
    for r_i, row in enumerate(ROWS):
        for c_i, col in enumerate(DRAWN_LEFT_COLS):
            geo[f"L{row}{col}"] = (float(c_i), COL_Y[c_i] + r_i * ROW_PITCH)
        for c_i, col in enumerate(DRAWN_RIGHT_COLS):
            # The right half mirrors the left, so its stagger reads backwards.
            geo[f"R{row}{col}"] = (RIGHT_X0 + c_i, COL_Y[-1 - c_i] + r_i * ROW_PITCH)
    left_thumb0 = len(DRAWN_LEFT_COLS) - 3 + THUMB_DX
    for i, name in enumerate(("LH2", "LH1", "LH0")):
        geo[name] = (left_thumb0 + i, THUMB_Y)
    for i, name in enumerate(("RH0", "RH1", "RH2")):
        geo[name] = (RIGHT_X0 - THUMB_DX + i, THUMB_Y)
    return geo


GEOMETRY = build_geometry()

# The order the LAYOUT macro takes its arguments in. Checked against keymap.c.
POSITIONS = (
    [f"LT{c}" for c in LEFT_COLS] + ["LTA", "RTA"] + [f"RT{c}" for c in RIGHT_COLS]
    + [f"LM{c}" for c in LEFT_COLS] + ["LMA", "RMA"] + [f"RM{c}" for c in RIGHT_COLS]
    + [f"LB{c}" for c in LEFT_COLS] + [f"RB{c}" for c in RIGHT_COLS]
    + ["LH2", "LH1", "LH0", "RH0", "RH1", "RH2"]
)

# The subset that gets drawn.
DRAWN = [p for p in POSITIONS
         if p not in SKIP_POSITIONS and p[-1] not in SKIP_COLS]


def check_positions_match_keymap() -> None:
    """The LAYOUT call in keymap.c is the real argument order. Follow it."""
    if not KEYMAP_C.exists():          # a bare checkout of just the userspace
        return
    source = KEYMAP_C.read_text()
    body = source.split("LAYOUT_split_3x6_3_ex2(", 1)
    check(len(body) > 1, f"no LAYOUT_split_3x6_3_ex2 call in {KEYMAP_C}")
    found = re.findall(r"STR##_([LR][TMBH][0-5A])\b", body[1])
    check(found == POSITIONS,
          "POSITIONS no longer matches the LAYOUT call in keymap.c.\n"
          f"  keymap.c: {found}\n  here:     {POSITIONS}")


# --- Preprocessor ------------------------------------------------------------

def expand(lines: list[str], *, promethium: bool, preamble: str = "") -> str:
    """Run the C preprocessor over `lines` with the layer headers in scope."""
    document = '#include "ramo_layers.h"\n' + preamble + "\n".join(lines) + "\n"
    cmd = ["clang", "-E", "-P", "-x", "c", "-I", str(USERSPACE)]
    if promethium:
        cmd.append("-DRAMO_USE_HD_PROMETHIUM")
    cmd.append("-")
    done = subprocess.run(cmd, input=document, capture_output=True, text=True)
    check(done.returncode == 0,
          f"the preprocessor failed:\n{done.stderr.strip()}")
    return done.stdout


def read_layers(promethium: bool) -> list[tuple[str, str]]:
    """[(enum name, prefix)], in layer-index order, straight from ramo_layers.h.

    ramo_layers.h undefines RAMO_DO after building the enum, so expanding
    RAMO_FOR_EACH_LAYER on its own yields literal RAMO_DO(NAME, PREFIX) calls.
    """
    out = expand(["RAMO_FOR_EACH_LAYER"], promethium=promethium)
    layers = re.findall(r"RAMO_DO\(\s*(\w+)\s*,\s*(\w+)\s*\)", out)
    check(len(layers) >= 2, f"could not read the layer list, got: {out!r}")
    return layers


def read_keys(layers, promethium: bool) -> dict[str, dict[str, str]]:
    """Keycode per layer prefix and position."""
    lines = []
    for _, prefix in layers:
        for pos in POSITIONS:
            lines.append(f"KEY({prefix},{pos},{prefix}_{pos})")
    out = expand(lines, promethium=promethium)

    keys: dict[str, dict[str, str]] = {p: {} for _, p in layers}
    for prefix, pos, value in re.findall(
            r"\bKEY\((\w+),(\w+),(.*?)\)\s*$", out, re.MULTILINE):
        keys[prefix][pos] = value.strip()

    for _, prefix in layers:
        check(len(keys[prefix]) == len(POSITIONS),
              f"{prefix}: expanded {len(keys[prefix])} keys, want {len(POSITIONS)}")
    return keys


def read_combo_gates(source: str, promethium: bool) -> dict[str, str]:
    """Combo enum name -> the base layer it is limited to, from combo_should_trigger.

    The gate is written as a range (`case HD_COMBO_ACUT ... HD_COMBO_CAPW:`), so
    the enum order decides which combos it covers. The enum has an #ifdef in it,
    so let the preprocessor resolve it rather than guessing.
    """
    block = re.search(r"enum ramo_combos \{(.*?)\n\};", source, re.S)
    check(block is not None, "no `enum ramo_combos` found in ramo_combos.c")
    expanded = expand(["enum ramo_combos {" + block.group(1) + "\n};"],
                      promethium=promethium)
    tail = expanded.split("enum ramo_combos {", 1)[1].split("};", 1)[0]
    order = [n for n in re.findall(r"\b(\w+)\b(?:\s*=\s*\d+)?\s*,", tail)
             if not n.isdigit()]
    check("NO_COMBO" in order, f"could not read the combo enum, got {order}")

    gates: dict[str, str] = {}
    for first, last, layer in re.findall(
            r"case (\w+)\s*\.\.\.\s*(\w+):\s*\n\s*return get_highest_layer\("
            r"default_layer_state\) == (L_\w+);", source):
        check(first in order and last in order,
              f"gate range {first}..{last} names a combo that is not in the enum")
        lo, hi = order.index(first), order.index(last)
        check(lo <= hi, f"gate range {first}..{last} runs backwards")
        for name in order[lo:hi + 1]:
            gates[name] = layer
    return gates


def read_combos(layers, promethium: bool) -> list[dict]:
    """Combos from ramo_combos.c, with each member resolved to a key position.

    Position macros are redefined to markers before the combo arrays expand, so an
    alias like HD_Q_keys resolves to @@LT3@@, @@LT1@@ instead of to keycodes.
    """
    source = (USERSPACE / "ramo_combos.c").read_text()
    gates = read_combo_gates(source, promethium)

    arrays = dict(re.findall(
        r"const uint16_t PROGMEM (\w+)\[\]\s*=\s*\{(.*?),\s*COMBO_END\s*\}",
        source, re.S))
    check(arrays, "no combo key arrays found in ramo_combos.c")

    # Redefine every position macro to a marker that survives expansion.
    redef = []
    for _, prefix in layers:
        for pos in POSITIONS:
            redef.append(f"#undef {prefix}_{pos}")
            redef.append(f"#define {prefix}_{pos} @@{prefix}_{pos}@@")
    preamble = "\n".join(redef) + "\n"

    names = list(arrays)
    out = expand([f"MEMBERS({n},{arrays[n]})" for n in names],
                 promethium=promethium, preamble=preamble)
    members: dict[str, list[str]] = {}
    raw: dict[str, str] = {}
    for name, body in re.findall(r"MEMBERS\((\w+),(.*?)\)\s*$", out, re.MULTILINE):
        members[name] = re.findall(r"@@(\w+)_([LR][TMBH][0-5A])@@", body)
        raw[name] = body.strip()
    check(set(members) == set(names),
          f"combo members did not resolve: {set(names) - set(members)}")

    # key_combos[] gives the enum name, the array, and the output.
    combos = []
    for enum, kind, array, arg in re.findall(
            r"\[(\w+)\]\s*=\s*(COMBO_ACTION|COMBO)\(\s*(\w+)\s*(?:,\s*([^)]*?))?\s*\)",
            source):
        check(array in members, f"combo {enum} uses unknown array {array}")
        resolved = members[array]
        check(len(resolved) >= 2,
              f"combo {enum}: {array} resolved to {len(resolved)} key positions, "
              f"want 2 or more. The preprocessor made it {raw[array]!r}, so it "
              f"names a position macro that this build does not define.")
        prefixes = {p for p, _ in resolved}
        check(len(prefixes) == 1,
              f"combo {enum} mixes layers: {prefixes}")
        combos.append({
            "enum": enum,
            "prefix": prefixes.pop(),
            "positions": [pos for _, pos in resolved],
            "output": (L.COMBO_ACTION_NOTES.get(enum) if kind == "COMBO_ACTION"
                       else L.COMBO_RESULT_NOTES.get(enum) or legend_text(arg)),
            "action": kind == "COMBO_ACTION",
            "gate": gates.get(enum),
        })
        check(combos[-1]["output"] is not None,
              f"combo {enum} is a COMBO_ACTION with no note in kle_legends.py")

    declared = len(re.findall(r"^\s*\[(\w+)\]\s*=\s*COMBO", source, re.MULTILINE))
    check(len(combos) == declared,
          f"parsed {len(combos)} combos but key_combos[] declares {declared}")
    return combos


def read_adaptives() -> list[dict]:
    """Adaptive rules from ramo_adaptive.c.

    The nested switch gives the trigger key and the prior key. The effect comes
    from the trailing // comment every rule already carries, which is both exact
    and already maintained. A comment describes a rule when it contains "->";
    the other comments in the file explain a step or record a removal.

    One inner case can hold two rules: a three-key rule inside an
    `if (preprior_keycode == …)` guard, then the two-key rule after it.
    """
    source = (USERSPACE / "ramo_adaptive.c").read_text()
    body = source.split("switch (keycode) {", 1)
    check(len(body) > 1, "no outer switch found in ramo_adaptive.c")
    body = body[1].split("\n    }\n", 1)[0]

    rules: list[dict] = []
    described = 0
    trigger = prior = None
    for line in body.splitlines():
        outer = re.match(r"\s{8}case (KC_\w+):", line)
        if outer:
            trigger, prior = outer.group(1), None
            continue
        inner = re.match(r"\s{16}case (KC_\w+):(.*)", line)
        if inner:
            prior = inner.group(1)
            line = inner.group(2)
        comment = re.search(r"//\s*(.+?)\s*$", line)
        if not comment or "->" not in comment.group(1):
            continue
        described += 1
        check(trigger is not None and prior is not None,
              f"rule comment outside any case: {comment.group(1)!r}")
        guard = re.search(r"preprior_keycode == (KC_\w+)", line)
        rules.append({"trigger": trigger, "prior": prior,
                      "note": comment.group(1),
                      "preprior": guard.group(1) if guard else None})

    # A fallthrough case (`case KC_G:` with the body on the next `case`) carries
    # its own comment, so every "->" comment must have produced exactly one rule.
    check(len(rules) == described,
          f"parsed {len(rules)} rules from {described} rule comments")
    check(rules, "no adaptive rules parsed from ramo_adaptive.c")
    return rules


# --- Keycode to legend -------------------------------------------------------

def split_args(text: str) -> list[str]:
    """Split a macro argument list on commas that are not inside parentheses."""
    args, depth, current = [], 0, ""
    for ch in text:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            args.append(current.strip())
            current = ""
        else:
            current += ch
    args.append(current.strip())
    return args


def parse_keycode(keycode: str) -> tuple[object, str | None]:
    """(tap legend, hold legend). The hold legend goes on the front face."""
    keycode = keycode.strip()
    if keycode in L.BASIC:
        return L.BASIC[keycode], None

    call = re.fullmatch(r"(\w+)\((.*)\)", keycode, re.S)
    if not call:
        raise ExportError(f"no legend for keycode {keycode!r} — add it to kle_legends.py")
    name, args = call.group(1), split_args(call.group(2))

    if name in L.MOD_TAPS:
        tap, _ = parse_keycode(args[0])
        return tap, L.MOD_TAPS[name]
    if name == "LT":
        tap, _ = parse_keycode(args[1])
        layer = L.LAYER_NAMES.get(args[0])
        check(layer is not None, f"unknown layer {args[0]!r} in {keycode!r}")
        return tap, layer
    if name == "DF":
        layer = L.LAYER_NAMES.get(args[0])
        check(layer is not None, f"unknown layer {args[0]!r} in {keycode!r}")
        return f"Base: {layer}", None
    if name == "TD":
        note = L.TAP_DANCES.get(args[0])
        check(note is not None, f"unknown tap dance {args[0]!r} — add it to kle_legends.py")
        return note, None
    if name in L.CHORDS:
        inner, _ = parse_keycode(args[0])
        if isinstance(inner, tuple):
            inner = inner[1]
        return L.CHORDS[name] + inner, None

    raise ExportError(f"no legend for keycode {keycode!r} — add it to kle_legends.py")


def legend_text(keycode: str) -> str:
    """One flat string for a keycode. Used where a key has no room for two lines."""
    tap, hold = parse_keycode(keycode)
    if isinstance(tap, tuple):
        tap = tap[1]
    return f"{tap} ({hold})" if hold else tap


# --- KLE serialization -------------------------------------------------------
# KLE stores a label list of 12 slots whose meaning depends on the key's align
# flag. labelMap[align][slot] is the visual position that slot renders at:
#
#     0  1  2      top row of the keycap
#     3  4  5      middle
#     6  7  8      bottom
#     9 10 11      the sloped front face
#
# Taken from the keyboard-layout-editor deserializer.

LABEL_MAP = [
    [0, 6, 2, 8, 9, 11, 3, 5, 1, 4, 7, 10],   # 0 = nothing centred
    [1, 7, -1, -1, 9, 11, 4, -1, -1, -1, -1, 10],
    [3, -1, 5, -1, 9, 11, -1, -1, 4, -1, -1, 10],
    [4, -1, -1, -1, 9, 11, -1, -1, -1, -1, -1, 10],
    [0, 6, 2, 8, 10, -1, 3, 5, 1, 4, 7, -1],
    [1, 7, -1, -1, 10, -1, 4, -1, -1, -1, -1, -1],   # 5 = centre x + front
    [3, -1, 5, -1, 10, -1, -1, -1, 4, -1, -1, -1],
    [4, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1],  # 7 = centre x, y + front
]

TOP, MIDDLE, BOTTOM, FRONT = 1, 4, 7, 10
TOP_LEFT, CENTER = 0, 4
# A 1u cap has a 3x3 label grid on its top face plus a 3-slot front face. These
# are the stacks that do not collide: one centred column of four, and two columns
# of three for short strings.
CENTER_STACK = (1, 4, 7, 10)
TWO_COLUMNS = (0, 2, 3, 5, 6, 8)
FRONT_CENTER = 10


def labels(align: int, placed: dict[int, str]) -> str:
    """Build the \\n-joined label string putting each text at a visual position."""
    slot_of = {}
    for slot, visual in enumerate(LABEL_MAP[align]):
        if visual >= 0 and visual not in slot_of:
            slot_of[visual] = slot
    out = [""] * 12
    for visual, text in placed.items():
        check(visual in slot_of,
              f"visual position {visual} is unreachable with align {align}")
        out[slot_of[visual]] = text
    while out and not out[-1]:
        out.pop()
    return "\n".join(out)


class Sheet:
    """Collects absolutely-positioned keys and serializes them as KLE rows."""

    def __init__(self) -> None:
        self.keys: list[dict] = []

    def add(self, x, y, text, *, color, text_color, width=1.0, align=7,
            font=3, decal=False, ghost=False) -> None:
        self.keys.append(dict(x=x, y=y, text=text, color=color, tcolor=text_color,
                              width=width, align=align, font=font,
                              decal=decal, ghost=ghost))

    def rows(self) -> list:
        """KLE rows. Keys are grouped by y; x and y are emitted as deltas.

        KLE keeps colour, text colour, align, font and ghost until they change,
        but resets width and decal to their defaults after every key. So the
        first group is emitted on change and the second on every key that needs it.
        """
        sticky = (("c", "color"), ("t", "tcolor"), ("a", "align"),
                  ("f", "font"), ("g", "ghost"))
        per_key = (("w", "width", 1.0), ("d", "decal", False))
        out = []
        # Start every sticky property as unknown, not as its default. KLE carries
        # them across rows, and these rows get concatenated after another sheet's,
        # so the first key must state all of them.
        state = dict(color=None, tcolor=None, align=None, font=None, ghost=None)
        y_cursor = 0.0
        for y in sorted({round(k["y"], 4) for k in self.keys}):
            row, x_cursor = [], 0.0
            pending = {}
            if abs(y - y_cursor) > 1e-9:
                pending["y"] = round(y - y_cursor, 4)
            for key in sorted((k for k in self.keys if round(k["y"], 4) == y),
                              key=lambda k: k["x"]):
                if abs(key["x"] - x_cursor) > 1e-9:
                    pending["x"] = round(key["x"] - x_cursor, 4)
                for prop, field in sticky:
                    if key[field] != state[field]:
                        pending[prop] = key[field]
                        state[field] = key[field]
                for prop, field, default in per_key:
                    if key[field] != default:
                        pending[prop] = key[field]
                if pending:
                    row.append(pending)
                    pending = {}
                row.append(key["text"])
                x_cursor = key["x"] + key["width"]
            out.append(row)
            y_cursor = y + 1.0
        return out


def deserialize(rows: list) -> list[tuple[float, float, float]]:
    """Minimal KLE reader, used to check that Sheet.rows() round-trips.

    Mirrors KLE: x and y accumulate, and width resets to 1 after every key.
    """
    found, y, width = [], 0.0, 1.0
    for row in rows:
        if isinstance(row, dict):
            continue
        x = 0.0
        for item in row:
            if isinstance(item, dict):
                x += item.get("x", 0)
                y += item.get("y", 0)
                width = item.get("w", 1.0)
            else:
                found.append((round(x, 4), round(y, 4), round(width, 4)))
                x += width
                width = 1.0
        y += 1.0
    return found


# --- Boards ------------------------------------------------------------------

def title(sheet: Sheet, y0: float, text: str, sub: str = "") -> None:
    sheet.add(len(DRAWN_LEFT_COLS), y0 + TITLE_Y,
              labels(7, {CENTER: text, FRONT: sub} if sub else {CENTER: text}),
              color=L.TITLE_COLOR, text_color=L.TITLE_TEXT, width=HALF_GAP,
              align=7, decal=True)


def display_name(layer_enum: str) -> str:
    name = L.LAYER_NAMES.get("L_" + layer_enum)
    check(name is not None,
          f"no display name for layer {layer_enum} — add it to kle_legends.py")
    return name


def read_activators(layers, keys: dict) -> dict[str, dict[str, list[str]]]:
    """Layer enum -> {position: the tap legend that position has on each base}.

    A layer-tap on a base layer is how every layer here is reached, so each
    layer's own board can mark the key you hold to get there.
    """
    found: dict[str, dict[str, list[str]]] = {}
    for _, prefix in layers[:2]:
        for pos in DRAWN:
            call = re.fullmatch(r"LT\(\s*(L_\w+)\s*,(.*)\)",
                                keys[prefix][pos].strip(), re.S)
            if not call:
                continue
            layer = call.group(1).removeprefix("L_")
            tap, _ = parse_keycode(call.group(2).strip())
            if isinstance(tap, tuple):
                tap = tap[1]
            found.setdefault(layer, {}).setdefault(pos, []).append(tap)
    return found


def layer_board(sheet: Sheet, y0: float, keys: dict, activators: set[str]) -> None:
    for pos in DRAWN:
        x, y = GEOMETRY[pos]
        tap, hold = parse_keycode(keys[pos])
        if isinstance(tap, tuple):
            placed = {TOP: tap[0], BOTTOM: tap[1]}
            align = 5
        else:
            placed = {CENTER: tap}
            align = 7
        if hold:
            placed[FRONT] = hold
        reaches_here = pos in activators
        sheet.add(x, y0 + y, labels(align, placed),
                  color=L.ACTIVATOR_COLOR if reaches_here else L.KEY_COLOR,
                  text_color=L.KEY_TEXT, align=align,
                  ghost=reaches_here or L.is_unbound(keys[pos]))


def alpha_of(keycode: str) -> str:
    """The bare tap legend, for the combo and adaptive boards."""
    tap, _ = parse_keycode(keycode)
    return tap[1] if isinstance(tap, tuple) else tap


def combos_on(combos: list[dict], layer_enum: str) -> list[dict]:
    """The combos that fire while `layer_enum` is the default layer.

    A combo is a set of key positions, not of letters, so an ungated combo fires
    on both bases — at whatever letters that base puts under those positions.
    Two entries on the same positions with the same output are one chord to the
    typist (HD_COMBO_Th and CMK_COMBO_Th are literally the same two keys), so
    they collapse.
    """
    seen, out = set(), []
    for combo in combos:
        if combo["gate"] not in (None, "L_" + layer_enum):
            continue
        signature = (tuple(sorted(combo["positions"])), combo["output"])
        if signature in seen:
            continue
        seen.add(signature)
        out.append(combo)
    return out


def read_terms() -> dict[str, int]:
    """The timing constants, from config.h. They appear in the notes."""
    text = (USERSPACE / "config.h").read_text()
    out = {}
    for name in ("TAPPING_TERM", "RAMO_ADAPTIVE_TERM"):
        found = re.findall(rf"^#define {name}\s+(\d+)", text, re.M)
        check(found, f"could not read {name} from config.h")
        out[name] = int(found[-1])     # the last definition is the live one
    return out


# --- Documentation sections --------------------------------------------------
# Each returns a list of lines. Three composers below pick the ones their target
# needs, so the KLE Notes field, the layout repo and this userspace README all
# come from one description and cannot disagree.

def table(head: list[str], rows: list[list[str]]) -> list[str]:
    """A fixed-width table inside a code fence. Columns sized from the content."""
    width = [max(len(h), *(len(r[i]) for r in rows)) if rows else len(h)
             for i, h in enumerate(head)]

    def line(values: list[str]) -> str:
        return "  " + "  ".join(v.ljust(w) for v, w in zip(values, width)).rstrip()

    return ["```", line(head),
            "  " + "-" * (sum(width) + 2 * (len(width) - 1))] + \
           [line(r) for r in rows] + ["```"]


def section_grid() -> list[str]:
    """The position names. This is the vocabulary both implementations share."""
    return [
        "## Key positions",
        "",
        "Every layer below is described by position, never by what a particular",
        "board calls a switch. Both implementations use these same names —",
        "`users/raphaelmor/layers/*-defs.h` in QMK, `config/keynames_totem.h` in ZMK.",
        "",
        "```",
        "  ╭─────────────────────────╮  ╭─────────────────────────╮",
        "  │ LT4 LT3 LT2 LT1 LT0     │  │     RT0 RT1 RT2 RT3 RT4 │",
        "  │ LM4 LM3 LM2 LM1 LM0     │  │     RM0 RM1 RM2 RM3 RM4 │",
        "  │ LB4 LB3 LB2 LB1 LB0     │  │     RB0 RB1 RB2 RB3 RB4 │",
        "  ╰───────────╮ LH2 LH1 LH0 │  │ RH0 RH1 RH2 ╭───────────╯",
        "              ╰─────────────╯  ╰─────────────╯",
        "```",
        "",
        "T/M/B are the top, middle and bottom rows; H is the thumb cluster. The",
        "digit counts inward from the pinky, so 0 is the index column.",
    ]


def section_reaching(layers, activators) -> list[str]:
    bases = layers[:2]
    rows = []
    for enum, _ in layers:
        if enum not in activators:
            continue
        # activators[enum][pos] lists the tap legend per base, in base order.
        taps = [" / ".join(dict.fromkeys(activators[enum][pos]))
                for pos in sorted(activators[enum])]
        rows.append([display_name(enum), ", ".join(taps),
                     " ".join(sorted(activators[enum]))])

    out = [
        "## Reaching a layer",
        "",
        "Each layer's picture marks the key you **hold** to get there in red, and",
        "ghosts it, because it is not something that layer binds. Where the two",
        f"bases differ the column reads {display_name(bases[0][0])} / "
        f"{display_name(bases[1][0])}.",
        "",
    ] + table(["layer", "hold", "position"], rows)

    unreachable = [display_name(e) for e, _ in layers[2:] if e not in activators]
    if unreachable:
        out += ["",
                "No layer-tap reaches " + ", ".join(unreachable)
                + " — check how you get there."]
    return out


def section_combos(layers, keys, combos) -> list[str]:
    bases = layers[:2]
    # One row per distinct chord. key_combos[] can register the same two
    # positions twice — HD_COMBO_Th and CMK_COMBO_Th are literally the same
    # chord — and combos_on() already collapses those, so count what is shown.
    both = [c for c in combos
            if all(c in combos_on(combos, enum) for enum, _ in bases)]
    per_base: list[tuple[str, list[dict]]] = []
    shown = list(both)
    for enum, _ in bases:
        rest = [c for c in combos_on(combos, enum)
                if id(c) not in {id(b) for b in both}]
        if rest:
            per_base.append((enum, rest))
            shown += rest

    def row(combo: dict) -> list[str]:
        cells = ["+".join(alpha_of(keys[prefix][p]) for p in combo["positions"])
                 if combo in combos_on(combos, enum) else ""
                 for enum, prefix in bases]
        return cells + [L.COMBO_LONG_NOTES.get(combo["enum"], combo["output"]),
                        "+".join(combo["positions"])]

    head = [display_name(e) for e, _ in bases] + ["sends", "positions"]
    rendered = table(head, [row(c) for c in shown])
    # table() emits one block; re-open it to head the per-base groups.
    body = rendered[:-1][:3 + len(both)]
    index = len(both)
    for enum, rest in per_base:
        body += ["", f"  {display_name(enum)} base only"]
        body += rendered[3 + index:3 + index + len(rest)]
        index += len(rest)
    body.append("```")

    out = [
        f"## Combos — {len(shown)}",
        "",
        "A combo is a set of key *positions*, not of letters. Most are limited to",
        f"the {display_name(bases[1][0])} base, but {len(both)} are not, so those "
        f"also fire on",
        f"{display_name(bases[0][0])} — at whatever letters sit under those positions.",
        "",
    ] + body

    duplicates = len(combos) - len(shown)
    if duplicates:
        out += ["",
                f"`key_combos[]` declares {len(combos)} entries. "
                + (f"{duplicates} of them repeat a chord"
                   if duplicates > 1 else "One of them repeats a chord"),
                "already listed above — same positions, same output."]
    return out


def section_adaptives(layers, adaptives, terms) -> list[str]:
    out = [
        f"## Adaptive keys — {len(adaptives)}",
        "",
        f"Only on the {display_name(layers[1][0])} base. Roll two keys inside "
        f"{terms['RAMO_ADAPTIVE_TERM']} ms and the",
        "second one rewrites the pair. Rules that name three letters need the",
        "third-from-last key too.",
        "",
        "```",
    ]
    for rule in adaptives:
        after = (f"{rule['preprior'][3:]}+{rule['prior'][3:]}" if rule["preprior"]
                 else rule["prior"][3:])
        out.append(f"  {rule['note']:<52} (fires on {rule['trigger'][3:]}"
                   f" after {after})")
    out.append("```")
    return out


def anchor(filename: str, needle: str) -> str:
    """`file:line` for the line that holds `needle`. Never a stale number."""
    path = USERSPACE / filename
    check(path.exists(), f"{filename} is gone — fix the anchor in kle_legends.py")
    for number, line in enumerate(path.read_text().splitlines(), 1):
        if needle in line:
            return f"{filename}:{number}"
    raise ExportError(
        f"{filename} no longer contains {needle!r} — the behaviour moved or was "
        f"renamed. Fix the anchor in kle_legends.py.")


def section_always_on(terms) -> list[str]:
    return ["## Always on", ""] + [
        f"- **{name}** — {body.format(**terms)}  `{anchor(filename, needle)}`"
        for name, body, filename, needle in L.ALWAYS_ON]


def section_not_drawn(hidden) -> list[str]:
    out = [
        "## Not drawn",
        "",
        "The board has 46 switches. These "
        f"{len(SKIP_POSITIONS) + len(SKIP_COLS) * len(ROWS) * 2} are left off:",
        "",
        "- Outer pinky column `LT5 LM5 LB5 RT5 RM5 RB5` — `KC_NO` on every layer.",
        "- Inner ex2 keys `LTA LMA RTA RMA`.",
    ]
    if hidden:
        out += ["", "Careful: these hidden positions do have something bound.", ""]
        out += [f"- {h}" for h in hidden]
    return out


def build_notes(layers, keys: dict, combos: list[dict], adaptives: list[dict],
                activators: dict[str, dict[str, list[str]]],
                hidden: list[str]) -> str:
    """The keyboard's Notes field in the KLE document.

    Combos and adaptives used to sit on the caps. They never fit: a key can take
    four short lines at best, and the text has to be cut down until it stops
    meaning anything. Notes has no such limit and KLE shows it under the board.
    """
    terms = read_terms()
    out = [f"# {display_name(layers[0][0])} + {display_name(layers[1][0])} "
           "· Corne v4.1",
           "",
           "Generated by `users/raphaelmor/tools/kle_export.py` from the QMK sources.",
           "Do not hand-edit — regenerate instead.",
           ""]
    for block in (section_reaching(layers, activators),
                  section_combos(layers, keys, combos),
                  section_adaptives(layers, adaptives, terms),
                  section_always_on(terms),
                  section_not_drawn(hidden)):
        out += block + [""]
    return "\n".join(out[:-1])


# --- Export ------------------------------------------------------------------

def round_trip(sheet: Sheet, rows: list, what: str) -> None:
    """The x/y/width deltas are the easiest thing to get wrong. Read them back."""
    want = [(round(k["x"], 4), round(k["y"], 4), round(k["width"], 4))
            for k in sorted(sheet.keys, key=lambda k: (round(k["y"], 4), k["x"]))]
    got = deserialize(rows)
    check(got == want,
          f"the {what} rows do not round-trip. First difference: "
          + next((f"want {w}, got {g}" for w, g in zip(want, got) if w != g),
                 f"{len(want)} keys in, {len(got)} keys out"))


def read_all(promethium: bool) -> dict:
    """Everything the C sources have to say. Shared by every output format."""
    check_positions_match_keymap()
    layers = read_layers(promethium)
    keys = read_keys(layers, promethium)
    return {
        "layers": layers,
        "keys": keys,
        "combos": read_combos(layers, promethium),
        "adaptives": read_adaptives(),
        "activators": read_activators(layers, keys),
        # Nothing gets dropped quietly. If a position we do not draw has
        # something bound on some layer, say so in the notes.
        "hidden": [f"`{prefix}_{pos}` is `{keys[prefix][pos]}`"
                   for _, prefix in layers for pos in POSITIONS
                   if pos not in DRAWN and not L.is_unbound(keys[prefix][pos])],
        "promethium": promethium,
    }


def section_alphas(layers, keys) -> list[str]:
    """The two alpha bases as text grids, so the doc is useful without images."""
    out = ["## The two bases", "",
           "Switch between them with a double tap on the Nav layer. Everything",
           "else — layers, combos, adaptives — is shared.", ""]
    for enum, prefix in layers[:2]:
        cells = {p: alpha_of(keys[prefix][p]) for p in DRAWN}
        pad = max(len(v) for v in cells.values())
        half = (pad + 1) * len(DRAWN_LEFT_COLS) + 1     # a half's inner width
        thumb = (pad + 1) * 3 + 1                       # the thumb block's width
        shoulder = half - thumb                         # what the thumbs sit under

        def cell_row(side: str, row: str, cols: list[str]) -> str:
            return " ".join(cells[f"{side}{row}{c}"].center(pad) for c in cols)

        out += [f"### {display_name(enum)}", "", "```",
                "  ╭" + "─" * half + "╮  ╭" + "─" * half + "╮"]
        for row in ROWS:
            out.append(f"  │ {cell_row('L', row, DRAWN_LEFT_COLS)} │"
                       f"  │ {cell_row('R', row, DRAWN_RIGHT_COLS)} │")
        thumbs_l = " ".join(cells[p].center(pad) for p in ("LH2", "LH1", "LH0"))
        thumbs_r = " ".join(cells[p].center(pad) for p in ("RH0", "RH1", "RH2"))
        out += ["  ╰" + "─" * shoulder + "╮ " + thumbs_l + " │"
                "  │ " + thumbs_r + " ╭" + "─" * shoulder + "╯",
                "  " + " " * (shoulder + 1) + "╰" + "─" * thumb + "╯"
                "  ╰" + "─" * thumb + "╯",
                "```", ""]
    return out[:-1]


def section_board() -> list[str]:
    return [
        "## The board",
        "",
        f"Corne v4.1, `LAYOUT_split_3x6_3_ex2` (`keymap.c`): {len(POSITIONS)} "
        "switches — 3x6 per",
        "half, three thumbs, plus the four ex2 inner keys `LTA LMA RTA RMA`.",
        "",
        f"Only {len(DRAWN)} of them carry anything, so the pictures in the layout",
        "repo draw a 3x5 + 3. See *Not drawn* below for the rest.",
    ]


def section_flags(promethium: bool, terms: dict) -> list[str]:
    rows = [["RAMO_USE_HD_PROMETHIUM",
             "set" if promethium else "unset",
             "Hands Down Promethium" if promethium else "Hands Down Gold"]]
    rows += [[name, str(value), note] for name, value, note in (
        ("TAPPING_TERM", terms["TAPPING_TERM"],
         "hold to get the mod, the layer, the bracket pair"),
        ("RAMO_ADAPTIVE_TERM", terms["RAMO_ADAPTIVE_TERM"],
         "keydown to keydown, for an adaptive to fire"))]
    return ["## Build flags and timings", "",
            "All from `config.h`, read at generation time.", ""] + \
        table(["define", "value", "effect"], rows)


def section_files() -> list[str]:
    return ["## Where each feature lives", "",
            "All paths are relative to `users/raphaelmor/`.", ""] + \
        table(["feature", "file"], [
            ["layer list and order", "ramo_layers.h"],
            ["key and colour per position", "layers/{CMK,HDP,HDG,MED,NAV,MOS,"
                                            "SYM,NUM,FUN}-defs.h"],
            ["the keymap itself", "../../keyboards/crkbd/rev4_1/standard/"
                                  "keymaps/raphaelmor/keymap.c"],
            ["combos", "ramo_combos.c"],
            ["adaptive keys", "ramo_adaptive.c"],
            ["linger brackets, Caps Word", "ramo_linger.c"],
            ["Shift+Bksp, comma-leader shift", "ramo_process_record.c"],
            ["base switch (tap dance)", "ramo_tap_dance.c"],
            ["per-key RGB", "ramo_ledmap.c"],
        ])


def section_tooling() -> list[str]:
    return [
        "## Regenerating the docs",
        "",
        "```",
        "python3 tools/kle_docs.py       # this file and the layout repo",
        "python3 tools/kle_export.py     # corne-v4-1-ramo.json, for KLE",
        "```",
        "",
        "`kle_export.py` runs the C preprocessor over `ramo_layers.h`, so it reads",
        "the same keymap the firmware builds. It aborts instead of writing a wrong",
        "file: an unknown keycode, a combo whose keys do not resolve, or a legend",
        "too long for a cap all stop the export.",
        "",
        "To open it in [keyboard-layout-editor.com](http://www.keyboard-layout-editor.com),",
        "use the **Upload** button and pick `corne-v4-1-ramo.json`. Do not paste that",
        "file into the **Raw data** tab — that tab wraps whatever you type in `[ ]` of",
        "its own, so a whole `.json` file ends up double-wrapped and draws one empty",
        "key. Paste this instead:",
        "",
        "```",
        "python3 tools/kle_export.py --raw -o /tmp/kle-raw.txt && pbcopy < /tmp/kle-raw.txt",
        "```",
    ]


def notes_for(source: dict) -> str:
    return build_notes(source["layers"], source["keys"], source["combos"],
                       source["adaptives"], source["activators"],
                       source["hidden"])


def document_name(promethium: bool) -> str:
    return ("Corne v4.1 · raphaelmor "
            + ("Colemak + Hands Down Promethium" if promethium
               else "Colemak + Hands Down Gold"))


# GitHub URLs use blob/HEAD so they follow whatever the default branch is.
QMK_REPO = "https://github.com/raphaelmor/qmk_userspace"
ZMK_REPO = "https://github.com/raphaelmor/zmk-config"
QMK_DOC = f"{QMK_REPO}/blob/HEAD/users/raphaelmor/README.md"


def layout_doc(source: dict, image: str) -> str:
    """The layout itself, with no board in it. For the keyboard-layout repo.

    `image` is a format string taking the layer prefix, so the caller decides
    where the pictures sit relative to the file it is writing.
    """
    layers, keys = source["layers"], source["keys"]
    terms = read_terms()
    base_prefixes = [p for _, p in layers[:2]]

    out = section_grid() + [""] + section_alphas(layers, keys) + [""]
    out += ["## Every layer", ""]
    for enum, prefix in layers:
        role = "base" if prefix in base_prefixes else "layer"
        out += [f"### {display_name(enum)} — {role}", "",
                f"![{display_name(enum)} layer]({image.format(prefix=prefix.lower())})",
                ""]
    for block in (section_reaching(layers, source["activators"]),
                  section_combos(layers, keys, source["combos"]),
                  section_adaptives(layers, source["adaptives"], terms),
                  section_always_on(terms)):
        out += block + [""]

    out += [
        "## Implementations",
        "",
        f"- **Corne v4.1 on QMK** — [{QMK_REPO.split('/')[-1]}]({QMK_DOC}). "
        "The reference implementation; this page is generated from it.",
        f"- **Totem on ZMK** — [{ZMK_REPO.split('/')[-1]}]({ZMK_REPO}). "
        "38 keys, so it keeps `LB5` and `RB5`. Its own doc is still to write.",
    ]
    return "\n".join(out)


def qmk_doc(source: dict) -> str:
    """What is true of this board and this firmware, and nothing else."""
    out: list[str] = []
    for block in (section_board(),
                  section_not_drawn(source["hidden"]),
                  section_flags(source["promethium"], read_terms()),
                  section_files(),
                  section_tooling()):
        out += block + [""]
    return "\n".join(out[:-1])


def one_board(source: dict, enum: str, prefix: str) -> list:
    """A KLE document holding a single layer and no title decal.

    The README wants one image per layer with a markdown heading above it, so
    the decal that labels a board inside the stacked document is left out here.
    """
    sheet = Sheet()
    layer_board(sheet, 0.0, source["keys"][prefix],
                set(source["activators"].get(enum, {})))
    rows = sheet.rows()
    round_trip(sheet, rows, f"{prefix} board")
    return [{"backcolor": L.BOARD_BG,
             "name": f"{display_name(enum)} · {prefix}",
             "plate": True}] + rows


def build(promethium: bool) -> list:
    source = read_all(promethium)
    layers, keys = source["layers"], source["keys"]
    base_prefixes = [p for _, p in layers[:2]]

    sheet = Sheet()
    y = 0.0
    for enum, prefix in layers:
        role = "base" if prefix in base_prefixes else "layer"
        title(sheet, y, display_name(enum), f"{role} · {prefix}")
        layer_board(sheet, y, keys[prefix],
                    set(source["activators"].get(enum, {})))
        y += BOARD_PITCH

    rows = sheet.rows()
    round_trip(sheet, rows, "board")

    head = {
        "backcolor": L.BOARD_BG,
        "name": document_name(promethium),
        "notes": notes_for(source),
        "plate": True,
    }
    return [head] + rows


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-o", "--output", type=Path,
                    default=USERSPACE / "corne-v4-1-ramo.json")
    ap.add_argument("--raw", action="store_true",
                    help="write the form KLE's Raw data tab wants: the same rows "
                         "with no enclosing brackets. That tab adds its own, so a "
                         "whole .json file pasted there parses as one empty key.")
    ap.add_argument("--gold", action="store_true",
                    help="render Hands Down Gold instead of Promethium")
    args = ap.parse_args()

    try:
        document = build(promethium=not args.gold)
    except ExportError as err:
        print(f"kle_export: {err}", file=sys.stderr)
        return 1

    if args.raw:
        # One row per line, no outer [ ]. This is what the Raw data tab shows.
        text = ",\n".join(json.dumps(row, ensure_ascii=False)
                          for row in document) + "\n"
    else:
        text = json.dumps(document, indent=2, ensure_ascii=False) + "\n"
    args.output.write_text(text)

    keys = sum(1 for row in document[1:] for item in row if isinstance(item, str))
    print(f"wrote {args.output} — {keys} keys"
          + (" (Raw data form)" if args.raw else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
