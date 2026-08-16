# CLAUDE.md

## Conventions

Follow [`conventions/all.md`](conventions/all.md). It is synced from
[bvandrc/bvandrc-conventions](https://github.com/bvandrc/bvandrc-conventions) by
`.github/workflows/sync-conventions.yml` and is overwritten on every sync — never edit it here, and
never add project-specific rules to it. Anything below is an addition for this repo only; where the two
conflict, `conventions/all.md` wins.

## What this is

The Arduino sketch for the repo owner's festival totem — a double-sided WS2812-lit sign on a pole. One
physical device, one person maintaining it, no users. See [`README.md`](README.md) for the hardware, strip
layout and controls; read it before changing anything that touches pixel addressing or the dials.

That context should shape what you build. The constants describe the totem that exists — don't
parameterize them for hypothetical other builds, don't add config layers or feature flags, and don't keep
old code paths working for compatibility's sake. If something is being replaced, delete it.

## Layout

Arduino concatenates every `.ino` in the sketch folder alphabetically, so files are number-prefixed to
keep that order meaningful — a lower-numbered file must not depend on a higher-numbered one. Every `.ino`
has a matching `.h` declaring what the rest of the sketch may use; when you add a function that another
file calls, declare it `extern` in that header. Headers use `#ifndef`/`#define` include guards named after
the file.

## Constraints

This is an ATmega4809 (Nano Every): 6 KB RAM, no hardware float unit, and a single-threaded loop that must
keep feeding the strip.

- Size integer types deliberately — `uint8_t` for pixel indices and counts, `uint16_t` for hues and dial
  reads, `uint32_t`/`unsigned long` for `millis()` arithmetic. Don't reach for `int` by default.
- Prefer `static const` locals for derived constants so they're computed once, not per frame.
- Avoid heap allocation in anything on the animation path. `new` appears once, in `showStrip()`'s one-time
  lookup-table init, and that is the only place it belongs.
- Avoid floating point in inner loops.

## Style

Match the surrounding code:

- `camelCase` functions, `snake_case` locals and members, `SCREAMING_SNAKE_CASE` constants.
- `const` on locals that don't change — this codebase does it consistently.
- 2-space indent, 120-column lines, braces on the same line.
- Trailing `//` on an argument line is a formatting marker that pins a multi-line call's layout; leave it
  in place when editing those calls.
- The `TODO`s scattered around are real notes, not cruft. Leave them unless you're doing the thing.

## Animation loop rules

Each mode in `4_ColorModes.ino` runs its own `while (true)` loop and only returns on
`WaitReturnCode::MODE_CHANGED`. Two things follow:

- **Poll inputs through `wait()` or `readAllInputs()`, never `delay()`**, in any loop that should stay
  responsive. Dropping out of that path makes the dials and button go dead until the animation finishes.
- **Handle every `WaitReturnCode`.** Returning early on `MODE_CHANGED` is what lets `loop()` switch modes;
  `RAGE_PRESSED` and `TAPPED` mean the strip was repainted by a flash effect, so the mode has to redraw
  its own frame.

Paint through the `setPixelColor*` helpers in `2_HardwareOutputs.ino` (edge / quadrant / middle indices)
rather than indexing `leds[]` directly — they handle the side-2 mirroring. Call `showStrip()`, not
`FastLED.show()`, so the unused middle pixels get blanked.

## Verifying

There is no CI, no test suite, and no emulator. The only way to know a change works is to flash the board
and look at it, which only the owner can do — you can't. Say what you changed and what needs eyes on it;
never imply you tested something you didn't.

The timing, brightness and color values were tuned by eye on the real hardware. Treat them as
measurements, not guesses — if a change alters how something looks or how fast it moves, flag it as a
judgement call rather than quietly retuning it.
