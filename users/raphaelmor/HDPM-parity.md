# Hands Down Promethium — feature parity checklist

Exhaustive list of everything the **canonical** moutis Hands Down repo supports for the
**Promethium (pm)** variant, for porting into this firmware. Sourced from
`../HandsDown/handsdown/pm-adaptive.c`, `../HandsDown/handsdown/pm-config.h`,
`../HandsDown/moutis_combo_def.c`, `../HandsDown/moutis.h`. Updated against **main @ `a46d847`** (SemKeys-refactor / QWERTY-optional era).

Legend: `[x]` already in this firmware · `[ ]` not yet · **(hold)** = distinct action when held past `COMBO_HOLD`/`LINGER_TIME`.

> **Status (implemented).** Adaptives: **complete PM set incl. the 3-key rules** via `preprior` tracking,
> gated to the Hands Down base (`ramo_adaptive.c`). **ADAPT_SHIFT** (comma-leader one-shot Shift), HDP-only.
> Combos: `qu`/`z`/`th` + Shift-Tab + App-menu + diacritics/ligatures + `; : ! ?`, all at canonical positions,
> HDP-gated (`ramo_combos.c`). Linger brackets `( {` + smart caps-word + Shift+Bksp→Del (`ramo_linger.c`,
> `ramo_process_record.c`). Option-A thumbs; HD outer columns blanked. Mac-only → Semantic-Keys engine
> intentionally **not** ported.

> **Porting note.** Our HDP base now uses moutis's **canonical arrangement** (V W G M J top-left,
> `# . / " '` top-right, F P D L X bottom-left, `- U O Y B` bottom-right), so combos map to his exact
> positions. Kept customizations: GACS home-row mods, Hyper/Meh on fixed top positions (G/M, ./,/),
> blanked outer pinky columns, Option-A thumbs (Colemak-aligned). Home rows match canonical (`S N T H K` / `, A E I C`).

Canonical PM reference layout (positions → letters):
```
 V   W   G   M   J        #$  .:  /*  "[  ']
 S   N   T   H   K    ()   ,;   A   E   I   C   Z(RM5)
 F   P   D   L   X   [ ]   -+   U   O   Y   B   Qu(RB5)
          bsp  R          spc ret
```

---

## 1. Adaptive keys (`pm-adaptive.c`)

Fire only if the previous key was within `ADAPTIVE_TERM` (rolling window). All eliminate an
SFB/scissor by rewriting the roll. "→ del X" = backspaces the already-typed key first.

### Left hand
- [x] **P D → PWD** — on `D` after `P`: insert `W` (then D). *(simplified from the old 3-key P·B·D in the refactor)*
- [x] **P F → PS** — on `F` after `P`: type `S` instead of F
- [x] **K G → KL** — on `G` after `K`: type `L` instead of G
- [x] **W M G → WML** — on `G` after `W,M`: type `L` instead of G *(3-key, via preprior)*
- [x] **M G → LG** — on `G` after `M`: del M, type `L`, then G
- [x] **J G → JPG** — on `G` after `J`: insert `P`, then G
- [x] **W G → WD** — on `G` after `W`: type `D` instead of G
- [x] **K H → KN** — on `H` after `K`: type `N` instead of H
- [x] **G J → G+"th"** — on `J` after `G`: type `th` (for "length")
- [x] **V J / W J → …L** — on `J` after `W`: type `L` instead of J. *(`V J` **removed** — V+J is the `˜` tilde combo)*
- [x] **M K → LK** — on `K` after `M`: del M, type `L`, then K
- [x] **H K → NK** — on `K` after `H`: del H, type `N`, then K
- [ ] **P/B/S then L → unshifted L** — skipped by choice (low value on this HDP arrangement)
- [x] **G M → GL** — on `M` after `G`: type `L` instead of M
- [x] **V M → VL** — on `M` after `V`: type `L` instead of M
- [ ] **M W M → …L** — **removed**: unreachable, W+M is the `qu` combo (see below)
- [ ] **W M → "lm"** — **removed**: unreachable, W+M is the `qu` combo. Combos resolve in
      `pre_process_record_quantum`, *before* the adaptive engine runs — a roll fast enough for the
      adaptive is also inside `COMBO_TERM`, so `qu` always won. Kept the combo; `Q` needs a home.
- [x] **F P → SP** — on `P` after `F`: del F, type `S`, then P
- [x] **G V → GT** — on `V` after `G`: type `T` instead of V
- [x] **M V → LV** — on `V` after `M`: del M, type `L`, then V
- [x] **G W → GD** — on `W` after `G`: type `D` instead of W
- [x] **M W → MP** — on `W` after `M`: type `P` instead of W

### Right hand
- [x] **Y B → IB** — on `B` after `Y`: del Y, type `I`, then B
- [x] **A E → AU** — on `E` after `A`: type `U` instead of E

### Adaptive infrastructure
- [x] **Base-gating** — adaptives run only when Hands Down is the active default layer (`ramo_adaptive.c`); Colemak types clean.
- [x] **preprior tracking** — engine keeps `prior` AND `preprior` (enables the 3-key rules above)
- [x] **ADAPT_SHIFT** (`,` leader) — HDP-only: `,` then a letter within `RAMO_ADAPT_SHIFT_TERM` → capitalizes it, deletes the comma (`ramo_process_record.c`)
- [ ] **HD_MAGIC key** (`#`) + **HD_MAGIC_B** (`Bksp`) — "magic"/repeat-style text expansion (`.com`, `.org`, `.edu`, etc. via `adapt_magic.c`). Not pursued. Note: canonical `,,` toggles **Caps Lock**.

---

## 2. Combos (`moutis_combo_def.c` → `key_combos[]`)

### 2a. Off-map alphas — *needed to type these letters ergonomically*
- [x] **qu** (`W+M`, canonical) **(hold: delete u → q)**
- [x] **z** (`N+H`)

### 2b. H-digraph combos — `th` is **enabled** (`USE_HD_H_DIGRAPH_COMBO`, `HDP-defs.h`); the rest are optional
- [x] **th** (`T+H`) — live on HDP; Colemak gets its own (`CMK_Th_keys`, `S+T` positions)
- [ ] **ch** (`I+C`, or `N+T`)
- [ ] **sh** (`S+N`)
- [ ] **wh** (`W+G`)
- [ ] **ph** (`Y+B`, or `P+L`)
- [ ] **gh** (`G+M`)
- [ ] **sch** (`S+N+T`)

### 2c. Whitespace / editing keys
- [x] **Tab** — on thumb (not a combo)
- [x] **Shift-Tab** (`V+W+G`) — combo
- [x] **App menu** (`M+J`) — combo
- [ ] **Space / Enter** — on thumbs already (no combo needed)
- [ ] **hard-Enter / page break** (`X+L+D`, `SK_HENT`) — needs SemKeys (skipped, Mac-only)
- [ ] **Config layer MO(L_CFG)** — no CFG layer in our scheme
- [ ] **Num-layer toggle** (`R + Space`) → `TG(L_NUM)` — optional, not added

### 2d. Semantic editing combos (all via SemKeys — see §3)
- [ ] **New** (`X+K`)
- [ ] **Open** (`(-)+(,)`)
- [ ] **Close** (`(#)+(,)`)
- [ ] **Quit** (`J+K`)
- [ ] **Find** (`S+K`) **(hold: find-selection)**
- [ ] **Select All** (`F+L`)
- [ ] **Select Word** (`F+X`)
- [ ] **Undo** (`F+P`)
- [ ] **Redo** (`F+P+D`)
- [ ] **Copy** (`P+D`) **(hold: Cut)**
- [ ] **Paste** (`D+L`) **(hold: Paste-and-Match)**

### 2e. Symbol / punctuation combos (canonical top-right cluster) — HDP base only
- [x] **;** (`,+A`)
- [x] **:** (`.+"`)
- [x] **!** (`.+/`)
- [x] **?** (`/+"`)
- [ ] `$ § @ _ ~ = –` — not added (belong on **SYM v2**, where there's room)

### 2f. Ligatures — HDP base only, Mac ⌥ dead-keys
- [x] **Œ** (`E+O` → ⌥Q)
- [x] **Æ** (`A+U` → ⌥')

### 2g. Dead-key diacritics (Mac ⌥) — HDP base only, canonical positions
- [x] **´ acute** (`A+E` → ⌥E)
- [x] **` grave** (`E+I` → ⌥`)
- [x] **ˆ circumflex** (`A+I` → ⌥I)
- [x] **¨ dieresis** (`U+O` → ⌥U)
- [x] **˜ tilde / ñ** (`V+J` → ⌥N)
- [x] **¸ cedilla** (`S+T` → ⌥C)
- [ ] **¯ macron** (`E+C`) / **˚ ring** (`A+C`) — not added (rare)

### 2h. Screen / system (via SemKeys)
- [ ] **Esc** (`S+H`)
- [ ] **Force-quit** (`(")+(#)`, `SK_KILL`)
- [ ] **Screenshot → file** (`W+G+M+J`)
- [ ] **Screenshot → clipboard** (`G+M+J`)
- [ ] **Platform search / Siri** (`T+H+K`)
- [ ] **Caps Word** (`Y+B`) — *combo only;* the feature itself is done, but **implemented differently** (see §3).
      ⚠️ `Y+B` is already the **YB → IB adaptive** (`ramo_adaptive.c`). Porting this combo means moving one of them.
- [ ] **Caps Lock** (`O+Y+B`)
- [ ] **kana / eisuu toggles** (`I+E+A` / `N+T+H`)

### 2i. Custom text-macro combos
- [ ] **LeftCombo** (`W+G+M`) → user string **(hold: alt string)**
- [ ] **RightCombo** (`(.)+(/)+(")`) → user string **(hold: alt string)**

### 2j. Numpad combos (on L_NUM; position-based, ~22)
- [ ] Enter, Bksp, Del, Tab, Shift-Tab on numpad
- [ ] `– ~ … ( ) : % °` and currency `€ ¥ $ ¢ £ ÷` , `± ≠`
- [ ] Clear (`C`), All-Clear (`Esc`), Num-Lock toggle

### 2k. Pronoun combos — *optional (`EN_PRONOUN_COMBOS`), moutis calls them "unnecessary"*
- [ ] `I` (`I+C`), `I'd` (`I+D`), `I'll` (`I+L`), `I'm` (`I+M`), `I've` (`I+V`) — most **(hold: +'ve)**
- [ ] (`_ALL`) you'd / you'll / you're / you've / your, they / their / they're / they'll / they'd
- [ ] (`EN_W_PRONOUNS`) we'd / we'll / we're / we've / where

### 2l. Japanese youon combos — *optional (`JP_MODE_ENABLE`), ~50 kana*
- [ ] きゃ〜りょ contracted-sound combos (skip unless you type Japanese)

---

## 3. Feature systems (`moutis.h` + engine files)

- [x] **Home-row mods** (all base-layer alphas)
- [x] **Thumb layer-taps** — Colemak: Esc→MEDIA, Spc→NAV, Tab→MOUSE, Ent→SYM, Bspc→NUM, Del→FUN. HDP (Option A): thumbs aligned to Colemak, only R→SYM (inner-right) & Ent→FUN (outer-right) differ; Del→**Shift+Bksp**.
- [x] **Caps Word** — **different implementation, same intent.** We use QMK's built-in `caps_word` + the
      `caps_word_press_user` hook (`ramo_linger.c`); moutis rolls his own engine (`moutis_casemods.c`,
      ~144 lines, from Andrew Rae's pre-QMK-builtin implementation). Behavioral differences worth knowing:
      | | Ours (QMK builtin) | Moutis (`moutis_casemods.c`) |
      |---|---|---|
      | Shifting | weak mods per letter | toggles real **Caps Lock** (`tap_code(KC_CAPS)`) — for iOS etc. |
      | Activation | `CW_TOGG` on NAV | `,,` adaptive + `Y+B` combo |
      | space → `_` | in `process_record` | in the engine, tracks `last_press_was_space` |
      | double-space | *(nothing special)* | exits and **rewrites** `_` back to a real space |
      | Auto-off | `CAPS_WORD_IDLE_TIMEOUT` (5 s default) | `STATE_RESET_TIME * 3` ≈ 1.8 s, in `matrix_scan` |
      His Caps-Lock approach survives host-side remapping that weak-mod shifting doesn't; ours needs no
      `matrix_scan` work and no LED-state round-trip. Only port his if the weak-mod version misbehaves somewhere.
- [ ] **Semantic keys (`SK_*`)** — cross-platform abstraction. **Intentionally skipped — you're Mac-only**, so `⌘C/⌘V/⌘Z` on NAV stay as-is. — `moutis_semantickeys.c`
- [x] **Linger keys** — hold `(` `{` → auto-paired with caret inside (`ramo_linger.c`, `RAMO_LINGER_TERM`). `"` is unmapped and `[` isn't on **any** layer yet (SYM has `{ } ( )` only) — both need a SYM v2 home.
- [ ] **Key overrides** — manual Shift/Alt variants of punctuation. (Only Shift+Bksp→Del is done, for the Option-A thumb change.)
- [ ] **OS platform switching** — N/A (Mac-only).
- [ ] **Adaptive on/off toggle** — not needed: adaptives are auto-gated to the HD base, off on Colemak.
- [ ] **Japanese input mode** — `JP_MODE_ENABLE` (not pursued).
- [~] **Layer structure** — unchanged: our 8-layer scheme (COLEMAK/HDP/MEDIA/NAV/MOUSE/SYM/NUM/FUN). Not adopting canonical HD's separate layers.

---

## 4. Timings (`config.h`) — ours vs. canonical

Deliberately **not** matched to moutis. His values are in `../HandsDown/config.h` + `moutis.h`.
WPM equivalent = `12000 / ms` (5 chars/word), i.e. the speed *above* which a window still catches you.

| Constant | Ours | Moutis | Note |
|---|---|---|---|
| `TAPPING_TERM` | 200 | 170 | |
| **Adaptive window** | **175 ms** (~69 WPM) | 229 ms (`COMBO_HOLD*1.35`, ~52 WPM) | see below |
| ADAPT_SHIFT window | 200 ms | 918 ms (`ADAPTIVE_TERM*4`) | |
| `COMBO_TERM` | 50 (QMK default) | 42 (`TT/4`) | |
| Combo hold / linger | 200 / 200 | 170 / 204 | |
| `TAP_CODE_DELAY` | **20** | 20 | adopted — at 0, back-to-back taps in del-then-retype adaptives can drop or reorder |
| `QUICK_TAP_TERM` | **0** | 170 (default) | intentional: disables double-tap-hold auto-repeat |
| `PERMISSIVE_HOLD` | off | on | intentional (for now) |

**Adaptive window — tuned empirically (`tt verify`), 50 → 100 → 175 ms.**
The key insight: **adaptives fix SFBs and scissors, which are slow motions by definition** — so a
window tuned to *roll* speed systematically fails the highest-value rules. At 100 ms, fast
adjacent-finger rolls passed (`PD`, `WG`) while `KH` (same-finger, index→index) and `KG`
(inner-column stretch) never fired at all. 175 ms clears those motions while staying well under
moutis's always-armed 229 ms.

**Window is measured with `record->event.time`, not `timer_read()`.** For a mod-tap/layer-tap,
`process_record_user` only sees the record once the tap resolves (on release, or after
`TAPPING_TERM`), so read-time would charge the hold duration against the window. 12 keys are
mod-taps (`G M . /` + both home rows), i.e. most adaptive triggers.

**3-key rules need two consecutive intervals inside the window**, so their failure rate is roughly
the square of the 2-key rate — expect them to be the last to become reliable.

**Known collisions to watch at 175 ms** — same-row rolls that appear in real words:
`GM` in *pragma* / *progmem* · `PD` in *update* · `YB` in *keyboard* · `MW` in *firmware* · `KG` in *background*.
None observed yet in practice. `MK` (*qmk*) is safe, being same-finger.

**Combo-vs-adaptive collisions:** combos resolve in `pre_process_record_quantum`, before the adaptive
engine. Any rule whose two keys are also a combo is unreachable — `W M`/`M W M` (`qu`) and `V J` (`˜`)
were removed for this. Check new adaptives against `ramo_combos.c` before adding.


---

## Done vs. remaining
**Done:** complete PM adaptives incl. 3-key rules (preprior) · ADAPT_SHIFT · `qu`/`z`/`th` · Shift-Tab · App-menu · diacritics + ligatures (`é è ê ü ñ ç œ æ`) · `; : ! ?` symbol combos · linger `( {` · smart caps-word · Shift+Bksp→Del · Option-A thumbs · canonical arrangement · blanked HD outer columns. **All HDP-gated where base-specific. Builds ~64 KB.**

**Still open (by choice / next):**

> **Current focus: learning the layout, not extending it.** The firmware is feature-complete for
> daily driving. Everything below is deferred until real use shows it's needed — including the
> adaptive-window collisions in §4.

1. **SYM "Programming v2"** — bracket/operator layer for Swift/Odin/ObjC (mock in `hd-colemak.html`). Would also give `[` (currently on no layer) and `"` a home for linger.
2. **Colemak-DH** — optional base swap (6 keys), mocked as a toggle.
3. Nice-to-have combos: `ch/sh/wh/ph/gh` digraphs (`th` already on); `$ § @ ~ = –` (better on SYM v2).
4. **Not pursued (Mac-only / design):** Semantic keys → so §2d editing & §2h screen combos (they need SemKeys), HD_MAGIC, macron/ring diacritics, pronouns (§2k), Japanese (§2l), numpad combos (§2j).
