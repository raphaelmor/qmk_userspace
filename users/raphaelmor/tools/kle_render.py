#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Raphaël Mor
"""Draw a KLE document as an SVG, in KLE's own style, with the legends included.

KLE's "Download SVG" produces good-looking keycaps and **no text at all** — the
export carries 296 keycap groups and zero <text> nodes. So this module emits the
same markup KLE does, copied from one of its exports, and then draws the legends
on top.

    python3 tools/kle_render.py corne-v4-1-ramo.json -o /tmp/board.svg

Everything below is at KLE's native 54 px key unit, so the numbers match its
output one for one:

    shell   x*54 + 1,  y*54 + 1,  w*54 - 2,  h*54 - 2   rx 5
    plate   shell + (6, 3),       shell - (12, 12)      rx 5, lightness + 0.05
    ghost   shell only, no plate
    decal   nothing but its legends
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from xml.sax.saxutils import escape

sys.path.insert(0, str(Path(__file__).resolve().parent))
from kle_export import LABEL_MAP  # noqa: E402

UNIT = 54              # KLE's key unit, in pixels
PLATE_DX, PLATE_DY = 6, 3
PLATE_DW, PLATE_DH = 12, 12
OUTER_PAD = 10         # around the plate rectangle
INNER_PAD = 5          # between the plate rectangle and the keys
TEXT_PAD = 1.5
FRONT_SCALE = 0.78     # the front lip is much shorter than the keytop
DESCENDER = 0.22       # of an em; the tail of a comma has to stay on the keytop
BOTTOM_LIFT = 0.45     # of an em, on top of the descender. The front lip carries
                       # visual weight below the plate, so a legend pinned to the
                       # plate's bottom edge reads lower than it measures.

# Copied verbatim from a KLE export, so the caps render the way KLE draws them.
STYLE = """    <style type='text/css'>
    .keycap .border { stroke: black; stroke-width: 2; }
    .keycap .inner.border { stroke: rgba(0,0,0,.1); }
    </style>
    <defs>
      <linearGradient id="DCS">
        <stop offset="0%" stop-color="black" stop-opacity="0"/>
        <stop offset="40%" stop-color="black" stop-opacity="0.1"/>
        <stop offset="60%" stop-color="black" stop-opacity="0.1"/>
        <stop offset="100%" stop-color="black" stop-opacity="0"/>
      </linearGradient>
      <radialGradient id="DSA">
        <stop offset="0%" stop-color="black" stop-opacity="0.1"/>
        <stop offset="10%" stop-color="black" stop-opacity="0.1"/>
        <stop offset="100%" stop-color="black" stop-opacity="0"/>
      </radialGradient>
      <radialGradient id="SA" xlink:href="#DSA" />
    </defs>"""


def deserialize(document: list) -> tuple[dict, list[dict]]:
    """KLE document -> (metadata, keys with absolute x/y/w and their properties)."""
    meta = document[0] if isinstance(document[0], dict) else {}
    rows = document[1:] if isinstance(document[0], dict) else document

    keys: list[dict] = []
    state = {"c": "#cccccc", "t": "#000000", "a": 4, "w": 1.0, "f": 3,
             "d": False, "g": False}
    y = 0.0
    for row in rows:
        if isinstance(row, dict):
            continue
        x = 0.0
        for item in row:
            if isinstance(item, dict):
                x += item.get("x", 0)
                y += item.get("y", 0)
                for prop in ("c", "t", "a", "d", "g", "w", "f"):
                    if prop in item:
                        state[prop] = item[prop]
                continue
            keys.append({"x": x, "y": y, "w": state["w"], "labels": item,
                         "c": state["c"], "t": state["t"], "a": state["a"],
                         "f": state["f"], "d": state["d"], "g": state["g"]})
            x += state["w"]
            state["w"] = 1.0      # KLE resets width and decal after every key
            state["d"] = False
        y += 1.0
    return meta, keys


def place(labels: str, align: int) -> dict[int, str]:
    """Label string -> {visual position: text}, using KLE's label map."""
    out = {}
    for slot, text in enumerate(labels.split("\n")):
        if not text or slot >= 12:
            continue
        visual = LABEL_MAP[align][slot]
        if visual >= 0:
            out[visual] = text
    return out


def keytop_color(color: str) -> str:
    """KLE's keytop is the cap colour with 0.05 added to its HSL lightness.

    Checked against its own export: #424242 becomes #4f4f4f.
    """
    if not (color.startswith("#") and len(color) == 7):
        return color
    rgb = [int(color[i:i + 2], 16) / 255 for i in (1, 3, 5)]
    low, high = min(rgb), max(rgb)
    lightness = (low + high) / 2
    target = min(1.0, lightness + 0.05)
    if high == low:                       # grey: no hue to preserve
        return "#" + "%02x" % round(target * 255) * 3
    delta = high - low
    sat = delta / (2 - high - low if lightness > 0.5 else high + low)
    hue = {high: 0.0}
    if high == rgb[0]:
        hue = ((rgb[1] - rgb[2]) / delta) % 6
    elif high == rgb[1]:
        hue = (rgb[2] - rgb[0]) / delta + 2
    else:
        hue = (rgb[0] - rgb[1]) / delta + 4
    chroma = (1 - abs(2 * target - 1)) * sat
    second = chroma * (1 - abs(hue % 2 - 1))
    base = target - chroma / 2
    parts = [(chroma, second, 0), (second, chroma, 0), (0, chroma, second),
             (0, second, chroma), (second, 0, chroma), (chroma, 0, second)]
    return "#" + "".join("%02x" % round((c + base) * 255)
                         for c in parts[int(hue)])


def wrap(text: str, size: float, room: float) -> list[str]:
    """Split on a space when a legend is too wide. SVG will not wrap for us."""
    if len(text) * size * 0.55 <= room or " " not in text:
        return [text]
    words = text.split(" ")
    best, score = 1, None
    for cut in range(1, len(words)):
        spread = abs(len(" ".join(words[:cut])) - len(" ".join(words[cut:])))
        if score is None or spread < score:
            best, score = cut, spread
    return [" ".join(words[:best]), " ".join(words[best:])]


def draw_text(x, y, anchor, text, size, color, room) -> list[str]:
    lines = wrap(text, size, room)
    step = size * 1.05
    top = y - step * (len(lines) - 1) / 2
    return [f'<text x="{x:.1f}" y="{top + i * step:.1f}" font-size="{size:.1f}" '
            f'fill="{color}" text-anchor="{anchor}">{escape(line)}</text>'
            for i, line in enumerate(lines)]


def keycap(key: dict) -> list[str]:
    """One keycap group, in the markup KLE's own SVG export uses."""
    x, y = key["x"] * UNIT + 1, key["y"] * UNIT + 1
    w, h = key["w"] * UNIT - 2, UNIT - 2
    classes = ("ghosted  keycap" if key["g"]
               else " decal keycap" if key["d"] else "  keycap")
    out = [f"<g class='{classes}'>"]
    if not key["d"]:
        shell = (f'x="{x:g}" y="{y:g}" width="{w:g}" height="{h:g}" '
                 f'rx="5" fill="{key["c"]}"')
        out += [f'  <rect {shell} class="outer border"/>',
                f'  <rect {shell}/>']
        if not key["g"]:
            top = (f'x="{x + PLATE_DX:g}" y="{y + PLATE_DY:g}" '
                   f'width="{w - PLATE_DW:g}" height="{h - PLATE_DH:g}" '
                   f'rx="5" fill="{keytop_color(key["c"])}"')
            out += [f'  <rect {top} class="inner border"/>',
                    f'  <rect {top}/>']

    # KLE stops here. Everything below is the text it does not export.
    px, py = x + PLATE_DX, y + PLATE_DY
    pw, ph = w - PLATE_DW, h - PLATE_DH
    size = 6 + 2 * key["f"]
    color = key["t"] if isinstance(key["t"], str) else "#000000"
    color = color.split("\n")[0] or "#000000"
    columns = [(px + TEXT_PAD, "start"), (px + pw / 2, "middle"),
               (px + pw - TEXT_PAD, "end")]
    # The front lip is only 9 px tall on a 52 px cap, so its legends are smaller
    # than the ones on the keytop or they spill over the plate edge.
    lip_top, lip_h = py + ph, h - PLATE_DY - ph
    lip_size = size * FRONT_SCALE
    rows = [(py + TEXT_PAD + size * 0.85, size),        # top of the keytop
            (py + ph / 2 + size * 0.35, size),          # its middle
            (py + ph - TEXT_PAD - size * (DESCENDER + BOTTOM_LIFT), size),
            (lip_top + lip_h / 2 + lip_size * 0.35, lip_size)]
    for visual, text in place(key["labels"], key["a"]).items():
        cx, anchor = columns[visual % 3]
        baseline, font = rows[visual // 3]
        room = pw - 2 * TEXT_PAD if visual % 3 == 1 else pw
        out += ["  " + line for line in
                draw_text(cx, baseline, anchor, text, font, color, room)]
    out.append("</g>")
    return out


def render(document: list) -> str:
    meta, keys = deserialize(document)
    content_w = max(k["x"] + k["w"] for k in keys) * UNIT
    content_h = (max(k["y"] for k in keys) + 1) * UNIT
    plate_w = content_w + 2 * INNER_PAD
    plate_h = content_h + 2 * INNER_PAD
    width = plate_w + 2 * OUTER_PAD
    height = plate_h + 2 * OUTER_PAD

    parts = [
        f"<svg width='{width:g}px' height='{height:g}px' "
        f"viewBox='0 0 {width:g} {height:g}' "
        f"xmlns='http://www.w3.org/2000/svg' "
        f'xmlns:xlink="http://www.w3.org/1999/xlink" '
        f"font-family='Helvetica Neue, Helvetica, Arial, sans-serif'>",
        STYLE,
        f"    <g transform='translate({OUTER_PAD},{OUTER_PAD})'>",
        f'      <rect width="{plate_w:g}" height="{plate_h:g}" stroke="#ddd" '
        f'stroke-width="1" fill="{meta.get("backcolor", "#eeeeee")}" rx="6"/>',
        f"      <g transform='translate({INNER_PAD},{INNER_PAD})'>",
    ]
    for key in keys:
        parts += ["        " + line for line in keycap(key)]
    parts += ["      </g>", "    </g>", "</svg>"]
    return "\n".join(parts) + "\n"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("input", type=Path)
    ap.add_argument("-o", "--output", type=Path, default=Path("/tmp/kle.svg"))
    args = ap.parse_args()

    args.output.write_text(render(json.loads(args.input.read_text())))
    print(f"wrote {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
