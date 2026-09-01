# QMK feature audit — candidates for this firmware

Survey of **everything QMK offers**, judged against *this* keymap (Colemak + Hands Down
Promethium on a Corne v4.1). Companion to [`HDPM-parity.md`](HDPM-parity.md): that doc tracks
parity with **moutis' Hands Down**, this one tracks parity with **QMK core**.

Sources: QMK `../qmk` @ `3ef9860ee1` (`0.29.8-768`, 2026-07-22) · userspace `users/raphaelmor/`.

Legend: `[x]` in this firmware · `[ ]` not here · **T1/T2** = recommended tier · **✗** = examined
and rejected, with the reason.

> **The framing.** `HDPM-parity.md:235-237` freezes feature work ("learning the layout, not
> extending it"). So the bar below is not *"is this interesting"* but **"does it reduce friction
> while learning, or fix something already broken?"**
>
> **The one-line finding:** this firmware is unusually complete on the **typing-transformation**
> axis (adaptives, combos, linger, comma-shift, smart Caps Word) and near-empty on the
> **tap-hold-correctness** and **feedback** axes. 16 home-row mod-taps run with *zero* misfire
> protection. That asymmetry is where all the value is.

---

## 1. What's already on

**`rules.mk`:** Caps Word · Combos · Tap Dance · Console + `KEYCODE_STRING` · introspection.

**Inherited from `crkbd/rev4_1`:** RGB Matrix · split · Mouse Keys · OLED · encoders ·
bootmagic · NKRO · extrakey.

**Hand-rolled, with no QMK feature behind it:**

| Behaviour | Where | QMK equivalent |
|---|---|---|
| Adaptive keys (24 rules) | `ramo_adaptive.c` | none — genuinely custom |
| Linger brackets `( {` | `ramo_linger.c:10-38` | none |
| SCREAMING_SNAKE Caps Word | `ramo_linger.c:80-97` | none (extends builtin Caps Word) |
| Comma-leader one-shot Shift | `ramo_process_record.c:48-60` | loosely, one-shot mods |
| Shift+Bspc → Del | `ramo_process_record.c:35-42` | **Key Overrides** → §3.3 |
| Combo hold-to-linger | `ramo_combos.c:154-167` | none |

---

## 2. The full QMK catalog

### 2a. Tap/hold and timing — `docs/tap_hold.md`
- [x] `TAPPING_TERM` (200) · `QUICK_TAP_TERM` (0)
- [ ] **`CHORDAL_HOLD`** — same-hand nested key forces the tap. → **T1, §3.1**
- [ ] **`FLOW_TAP_TERM`** — suppress hold shortly after another key. → **T2, §4.1**
- [ ] `PERMISSIVE_HOLD` / `HOLD_ON_OTHER_KEY_PRESS` — → **T1, pairs with §3.1**
- [ ] **`DYNAMIC_TAPPING_TERM_ENABLE`** (`DT_UP`/`DT_DOWN`/`DT_PRNT`) → **T2, §4.3**
- [ ] `RETRO_TAPPING` / Retro Shift — ✗ fights home-row mods
- [ ] `SPECULATIVE_HOLD` (`docs/tap_hold.md:819`) — ✗ very new, perturbs the exact path the adaptives ride
- [ ] `TAPPING_TERM_PER_KEY` · selectable debounce — not needed yet
- **Achordion is not in QMK core.** It's Getreuer's userspace library; `CHORDAL_HOLD` is the
  in-tree replacement. Don't go looking for it.

### 2b. Text / typing aids
- [x] Caps Word (`docs/features/caps_word.md`) — see §4.5 for two free upgrades
- [x] Combos · Tap Dance · Send String
- [ ] **Repeat / Alt-Repeat Key** (`QK_REP`/`QK_AREP`) → **T1, §3.2**
- [ ] **Key Overrides** → **T1, §3.3**
- [ ] **Layer Lock** (`QK_LLCK`) → **T2, §4.2**
- [ ] Autocorrect — ✗ §5
- [ ] Leader Key — ✗ §5
- [ ] Unicode / Unicode Map / UCIS / `send_unicode_string()` — ✗ §5 (Mac ⌥ dead keys by design)
- [ ] Auto Shift · Space Cadet · one-shot/sticky mods · Dynamic Macros · Key Lock (`QK_LOCK`) ·
      Swap Hands · Secure · Grave Escape · Magic keycodes · Stenography — ✗ §5

### 2c. Input devices
- [x] Mouse Keys (tuned, `config.h:17-26`)
- [~] **Encoders** — compiled in, *inert*, and pin-blocked → **§4.7**
- [ ] Pointing Device · PS/2 Mouse · Digitizer · Joystick · Programmable Buttons · DIP Switch — no hardware / no need

### 2d. Output / feedback
- [x] RGB Matrix — static ledmap only, all 44 stock animations `#undef`'d (`config.h:56-101`)
- [~] **OLED** — enabled at keyboard level, **no `oled_task_user`** → **T1, §3.4**
- [ ] **WPM** (+ `SPLIT_WPM_ENABLE`) → **T2, §4.4**
- [ ] LED Matrix · Backlight · LED Indicators · ST7565/HD44780 · Quantum Painter · Audio/Music/Clicky ·
      MIDI · Sequencer · Haptic — not applicable or not wanted

### 2e. Infrastructure
- [x] Split + `SPLIT_LAYER_STATE_ENABLE` · introspection · Community Modules
      (`elpekenin/ledmap` + `elpekenin/generics`, 2 of ~25)
- [ ] Deferred execution — ✗ §5 (polling was a deliberate choice)
- [ ] Remaining `SPLIT_*_ENABLE` flags: `LED_STATE` `MODS` `WPM` `OLED` `POINTING` `HAPTIC` `ACTIVITY`
- [ ] EEPROM/NVM/wear-leveling · VIA · Raw HID · Tri Layer · OS Detection · Wireless/Battery — not needed

---

## 3. Tier 1 — worth doing

### 3.1 Chordal Hold — the standout gap
`#define CHORDAL_HOLD` · `docs/tap_hold.md:568`

**Why.** 16 home-row mod-taps across two base layers at `TAPPING_TERM 200`, with
`PERMISSIVE_HOLD` off, `HOLD_ON_OTHER_KEY_PRESS` off, and **no misfire protection whatsoever**
(`HDPM-parity.md:202` records this as "intentional (for now)"). Chordal Hold settles a mod-tap as
a *tap* whenever the next key is on the **same hand**, and leaves cross-hand chords to
`HOLD_ON_OTHER_KEY_PRESS`/`PERMISSIVE_HOLD`. Hands Down Promethium is built around same-hand
rolls, and the adaptive rules deliberately target same-hand SFBs and scissors (`KH`, `KG`, `MG`) —
exactly the motions that misfire a mod today.

**⚠️ It is not a one-liner on this board.** Three things verified in the tree:

**(a) The auto-generated handedness table misses 10 of your 46 keys.**
Handedness is generated as a *weak* `chordal_hold_layout`
(`../qmk/lib/python/qmk/cli/generate/keyboard_c.py:207-239`), but the generator **reads only the
first layout in `info.json`** — for `crkbd/rev4_1` that's `LAYOUT_split_3x5_3` (36 keys), while
this keymap uses `LAYOUT_split_3x6_3_ex2` (46). The 10 uncovered matrix positions stay `0`, which
is neither `'L'`, `'R'`, nor `'*'`:

| Uncovered | Positions | HDP | Colemak |
|---|---|---|---|
| Outer pinky columns | `(0,0) (1,0) (2,0) (4,0) (5,0) (6,0)` → `L/R T5 M5 B5` | `KC_NO` | `KC_NO` |
| The four `ex2` keys | `(0,6) (1,6) (4,6) (5,6)` → `LTA LMA RTA RMA` | `KC_NO` | **`LCTL_T(KC_A)`** |

Per `get_chordal_hold_default` (`../qmk/quantum/action_tapping.c:940-951`) a `0` compares unequal
to everything, so those keys always permit the hold — i.e. Chordal Hold is silently *off* for
them. Almost entirely harmless here, since 6 of the 10 are `KC_NO` on both bases; the only live
case is the four `ex2` keys on Colemak, which are all `LCTL_T(KC_A)` (`CMK-defs.h:77,79,93,95`).
If §3.2 puts `QK_REP`/`QK_AREP` on those keys they become non-tap-hold and the issue disappears
entirely.

**(b) The thumbs get real `'L'`/`'R'` handedness, and that breaks every layer.**
The geometry guess only assigns `'*'` to a spacebar or a dead-centre key; all six thumbs land
firmly L or R. Every layer here puts its *payload* on the hand opposite its activating thumb —
but its **same-side modifier block on the same hand as that thumb**. With a bare
`#define CHORDAL_HOLD`, chording the thumb with its own hand's mods within `TAPPING_TERM` settles
the layer-tap as a tap:

| Thumb | Same-hand keys on the target layer | Broken result |
|---|---|---|
| `LT(L_MEDIA, KC_ESC)` `HDP-defs.h:77` | `KC_LCTL/LALT/LGUI/LSFT` `MED-defs.h:37-40`, `KC_ALGR :55` | types `Esc` |
| `LT(L_NAV, KC_SPC)` `:78` | left mods `NAV-defs.h:37-40` **+ both base-switch tap dances** `:23-24` | types `Space` |
| `LT(L_MOUSE, KC_TAB)` `:79` | left mods `MOS-defs.h:36-39` | types `Tab` |
| `LT(L_SYM, KC_R)` `:80` | `KC_RSFT/RGUI/RALT/RCTL` `SYM-defs.h:46-49` | types `r` |
| `LT(L_NUM, KC_BSPC)` `:81` | right mirror mods `NUM-defs.h:46-49` | types `Backspace` |
| `LT(L_FUN, KC_ENT)` `:82` | right mirror mods `FUN-defs.h:46-49` | types `Enter` |

Scope, stated honestly: "Chordal Hold has no effect after the tapping term", so this only bites
when the second key lands within 200 ms of the thumb press — which is precisely normal fast layer
use. **Prerequisite: define `chordal_hold_layout` over `LAYOUT_split_3x6_3_ex2` with `'*'` on all
six thumbs** — the docs recommend exactly this for thumb keys, and it fixes (a) at the same time
by covering all 46 positions.

**(c) The 17 combos are safe.** `get_chordal_hold_default` returns `true` for non-`KEY_EVENT`
records (`action_tapping.c:941-943`), so combos are exempt from the opposite-hands rule.

**Pair it with `HOLD_ON_OTHER_KEY_PRESS`** — the docs state Chordal Hold is *intended* to be used
with that or Permissive Hold; alone, the cross-hand case is unchanged.

**If adopted, verify:** `tt verify` on the same-hand adaptives most at risk — `KH` (same-finger
index), `KG` (inner-column stretch), and the 3-key `WMG` — plus confirm cross-hand `⌘C`/`⌘V` and
`⇧`+letter still hold. Expect adaptives to get *more* reliable, not less: same-hand taps now
settle at the next keydown instead of at release/timeout, which shortens the very delay documented
at `ramo_adaptive.c:12-18`.

### 3.2 Repeat Key — and it answers the four dead keys
`REPEAT_KEY_ENABLE = yes` · `QK_REP` / `QK_AREP` · `docs/features/repeat_key.md`

Same problem domain as the adaptive engine — eliminating same-finger bigrams — but with **zero
collision risk**, because it never rewrites already-typed text and so can't fight combos the way
`W M`/`V J` did (`HDPM-parity.md:223-225`). moutis' own HD layouts lean on it.

**Placement is already decided by the keymap:** all four `ex2` positions carry
`// TODO: should be a real key once a use is found` — `KC_NO` on HDP (`HDP-defs.h:33,35,49,51`)
and a pointless duplicate `LCTL_T(KC_A)` on Colemak (`CMK-defs.h:77,79,93,95`). `QK_REP` and
`QK_AREP` are the answer to that TODO, and doing so also erases finding §3.1(a).
Alt-Repeat's context-dependent inverse (`←`→`→`, PgDn→PgUp) is a real NAV-layer win.

**Caveat to check, not assume:** `process_last_key` and `process_repeat_key` run at
`../qmk/quantum/quantum.c:343`, **upstream of `process_record_kb`/`process_record_user` (line
355)**. So the adaptive engine's `prior_keycode`/`preprior_keycode` history will see the
*repeated* keycode rather than `QK_REP`. That's probably what you want — a repeat should count as
the letter for adaptive purposes — but it needs a `tt verify` pass to confirm the interaction with
the del-then-retype rules.

### 3.3 Key Overrides — strictly less code than what's there
`KEY_OVERRIDE_ENABLE = yes` · `docs/features/key_overrides.md` · already tracked `[ ]` at
`HDPM-parity.md:180`

`ramo_process_record.c:35-42` hand-rolls Shift+Bspc→Del with manual `clear_mods()`/`set_mods()`.
`ko_make_with_layers_negmods_and_options` (`../qmk/quantum/process_keycode/process_key_override.h:135`)
expresses it declaratively, and `negative_mod_mask` (`:69`) is the exact equivalent of the current
`!(get_mods() & ~MOD_MASK_SHIFT)` guard — so ⌘⇧Bspc keeps *not* becoming Del. Also gives a proper
home for future shifted-punctuation variants.

**Behavioural difference to record:** `process_key_override` runs at `quantum/quantum.c:387`,
*after* `process_record_user`. Today's early `return false` means the adaptive dispatch never sees
the keypress; with an override it would run first and set `prior_keycode = KC_BSPC`. Harmless as
far as the current 24 rules go (none trigger on `KC_BSPC`), but it is a real change in state.

### 3.4 `oled_task_user` — the display is currently lying
No `oled_task_user` exists anywhere in the userspace, so stock `crkbd.c` renders: it prints
**"Lower" for Hands Down and "Undef" for every layer ≥ 4**, against an 8-layer keymap.

The DRY fix reuses `RAMO_FOR_EACH_LAYER` (`ramo_layers.h:16-24`) — already the single source for
both `keymaps[]` (`keymap.c:49-57`) and `ledmap[]` (`ramo_ledmap.c:6-13`) — to generate a
layer-name table from the same list, so a layer can't have a keymap without a name. Worth showing
alongside: active base (CMK vs HDP, from `default_layer_state`), Caps Word state, and WPM (§4.4).

`elpekenin/ui` is already in the `modules/` submodule if you'd rather not hand-roll the drawing.

---

## 4. Tier 2 — good fits, lower stakes

### 4.1 Flow Tap — `#define FLOW_TAP_TERM 150` · `docs/tap_hold.md:485`
Complement to Chordal Hold: suppresses the hold when a tap-hold key is pressed shortly after
another key. Same mental model as `RAMO_ADAPTIVE_TERM 175` (`config.h:35-49`) — a rolling
typing-speed window.

**⚠️ It would break the thumbs as shipped.** The default `is_flow_tap_key()`
(`docs/tap_hold.md:504`) matches `KC_A`–`KC_Z`, `KC_SPC`, `KC_COMM`, `KC_DOT`, `KC_SCLN`, `KC_SLSH`
— applied to `get_tap_keycode()`, so it captures `LT(L_NAV, KC_SPC)` (`HDP-defs.h:78`) and HDP's
`LT(L_SYM, KC_R)` (`:80`). A letter followed within `FLOW_TAP_TERM` by a reach for thumb-NAV or
thumb-SYM would emit a literal space or `r`. **Overriding `is_flow_tap_key()` to exclude thumb
layer-taps is mandatory, not optional** (the docs even call out removing `case KC_SPC`).

**Upside for the adaptives:** Flow Tap forces early tap resolution, which directly mitigates the
late-`event.time` problem documented at `ramo_adaptive.c:12-18`.

### 4.2 Layer Lock — `LAYER_LOCK_ENABLE = yes` · `QK_LLCK` · `docs/features/layer_lock.md`
Long numeric entry on NUM and extended mouse work on MOS without holding a thumb the whole time.
Both layers have plenty of `*_NO` slots to host it.

### 4.3 Dynamic Tapping Term — `DYNAMIC_TAPPING_TERM_ENABLE = yes` · `docs/tap_hold.md:79`
`DT_UP`/`DT_DOWN`/`DT_PRNT` tune the tapping term live and type out the current value. This is the
same empirical loop already used for `RAMO_ADAPTIVE_TERM` (50 → 100 → 175, `config.h:35-49`), but
without a reflash per iteration. FUN has free slots.

**Caveat:** it switches the *runtime* value to `g_tapping_term` while the `TAPPING_TERM` **macro**
stays 200 — so `RAMO_COMBO_HOLD`, `RAMO_LINGER_TERM` and `RAMO_ADAPT_SHIFT_TERM`
(`config.h:32,33,52`) would stay pinned at 200 while the real tapping term moved. Use
`GET_TAPPING_TERM(keycode, record)` if they should track it.

### 4.4 WPM — `WPM_ENABLE = yes` + `#define SPLIT_WPM_ENABLE`
Direct progress feedback during the learning phase, surfaced on the OLED from §3.4. Cheap, and it
is the one addition that measures whether the layout is actually being learned.

### 4.5 Caps Word activation extras — two defines, no code
Caps Word is used heavily already (`CW_TOGG` on `NAV_RM0`, plus the `Y+B` combo
`ramo_combos.c:76`).
- `BOTH_SHIFTS_TURNS_ON_CAPS_WORD` — both index home-row shifts (`LSFT_T(KC_H)` +
  `RSFT_T(KC_A)`), which is a **cross-hand** chord and so composes cleanly with §3.1.
- `CAPS_WORD_INVERT_ON_SHIFT` — shift inverts instead of breaking the word.
- Also worth knowing: `CAPS_WORD_IDLE_TIMEOUT` defaults to 5 s; moutis uses ~1.8 s
  (`HDPM-parity.md:175`).

### 4.6 `QK_BOOT` / `EE_CLR` — currently on no layer at all
Flashing relies entirely on the RP2040 double-tap-reset. Cheap insurance on FUN, which has free
slots. (Both appear in the `HDPM-layers.html` mock-up but exist on no real layer.)

### 4.7 Encoders — **check the hardware before spending any effort**
`info.json` declares 2 encoders per side, but there is no `encoder_map`, no `ENCODER_MAP_ENABLE`
and no `encoder_update_user` — so they're compiled in and inert.

**Verified pin conflict:** encoder pins are GP5/GP6/GP7 (left) and GP24/GP6/GP7 (right)
(`../qmk/keyboards/crkbd/rev4_1/info.json:36-37,65-66`) against `I2C1_SDA_PIN GP6` /
`I2C1_SCL_PIN GP7` (`../qmk/keyboards/crkbd/rev4_1/config.h:10-11`). **Encoders and the OLED
share the same pins on rev4.1 and are mutually exclusive.** Decide which you want before writing
an encoder map or an `oled_task_user`.

---

## 5. Examined and rejected

- **✗ Autocorrect** — collides with the adaptive engine conceptually: both are armed on every
  keystroke and both rewrite via backspace. Worse, during a learning phase it **masks the layout
  errors you need to feel**. Revisit once HDP is automatic.
- **✗ Leader Key** — redundant against 17 combos; a second chord vocabulary to memorise for no new
  capability.
- **✗ Auto Shift / Retro Shift** — fights home-row mods and the comma-leader shift
  (`ramo_process_record.c:48-60`).
- **✗ Deferred executors** — polling via `matrix_scan_user` was chosen *deliberately* to dodge
  roll-over ordering bugs; the reasoning is at `ramo_linger.c:5-8`. Leave it.
- **✗ Unicode / Unicode Map / UCIS / `send_unicode_string()`** — the whole diacritic set is Mac ⌥
  dead keys by design (`ramo_combos.c`, `HDPM-parity.md:181`). Only relevant if a non-Mac host
  ever appears, and then **OS Detection** (`docs/features/os_detection.md`) is the companion.
- **✗ Grave Escape** — Esc is a thumb layer-tap (`HDP-defs.h:77`), so `QK_GESC` has nowhere to go.
- **✗ Speculative Hold** (`docs/tap_hold.md:819`) — genuine latency win, but very new and it
  perturbs the exact tap-hold path the adaptives depend on. Not while also learning the layout.
- **✗ One-shot mods** — the comma-leader shift is better suited to this layout than `OSM(MOD_LSFT)`.
- **✗ Space Cadet · Swap Hands · Dynamic Macros · Secure · Key Lock · Tri Layer · Programmable
  Buttons · Digitizer · Joystick · MIDI · Sequencer · Audio · Haptic · Stenography · Quantum
  Painter · VIA · Raw HID** — no problem in this firmware that they solve.

---

## 6. Defects noticed during the survey (documented, not fixed)

1. **`saved_modifiers` is dead code.** Declared `raphaelmor.c:10` and read at `ramo_combos.c:108`
   as the "don't fire combos while ⌥/⌘/⌃ is held" guard — but **never assigned anywhere**. As a
   zero-initialised global it is permanently 0, so the guard never fires. Intent was presumably
   `get_mods()`.
2. **The HD Gold path does not compile.** Un-defining `RAMO_USE_HD_PROMETHIUM` (`config.h:34`)
   breaks the build: `HDG-defs.h:186,189` define `HD_Q_keys`/`HD_Z_keys` in terms of `HDP_*`
   positions, and `ramo_combos.c:13-34` references `HDP_*` unconditionally, but `ramo_layers.h:2-6`
   includes one defs file or the other, never both.
3. **`rules.mk:5`** is `// TEMP: for debugging adaptive`. `//` is not a Make comment, and the line
   contains a `:`, so Make parses it as a stray rule target with prerequisites. Harmless in
   practice (QMK sets its default goal first) — but it is not a comment.
4. **`HDPM-parity.md:138`** marks Caps Word `Y+B` as not done; it *is* implemented
   (`ramo_combos.c:34,76`).

---

## 7. Toolchain blocker

The `qmk` CLI is currently broken on this machine — `/opt/homebrew/bin/qmk` has a dangling
interpreter (`/opt/homebrew/Cellar/qmk/1.1.8/libexec/bin/python`). The userspace `Makefile`
resolves `QMK_FIRMWARE_ROOT` via `qmk config -ro user.qmk_home`, so **nothing here can be built
until that's repaired** (`brew reinstall qmk`). Unrelated to this audit, but it gates acting on
any of it.

---

## 8. Summary

| Priority | Feature | Effort | Note |
|---|---|---|---|
| **T1** | Chordal Hold + `HOLD_ON_OTHER_KEY_PRESS` | 2 defines **+ a 46-key handedness table with `'*'` thumbs** | biggest win; do not ship without the table |
| **T1** | Repeat / Alt-Repeat on the four `ex2` keys | 1 flag + 4 keycodes | answers an existing TODO; also erases §3.1(a) |
| **T1** | Key Overrides | 1 flag + 1 table; **deletes** `ramo_process_record.c:35-42` | net less code |
| **T1** | `oled_task_user` | ~30 lines via `RAMO_FOR_EACH_LAYER` | the OLED is currently wrong |
| **T2** | Flow Tap | 1 define + `is_flow_tap_key()` override | override is mandatory |
| **T2** | Layer Lock · Dynamic Tapping Term · WPM · Caps Word defines · `QK_BOOT`/`EE_CLR` | flags + keycodes | cheap |
| **T2** | Encoder map | medium | **blocked on the OLED-vs-encoder pin decision** |
