# CLAUDE.md

## Conventions

Follow [`conventions/all.md`](conventions/all.md). It is synced from
[bvandrc/bvandrc-conventions](https://github.com/bvandrc/bvandrc-conventions) by
`.github/workflows/sync-conventions.yml` and is overwritten on every sync — never edit it here, and
never add project-specific rules to it. Anything below is an addition for this repo only; where the two
conflict, `conventions/all.md` wins.

## What this is

Arduino C++ firmware for a festival totem — a double-sided WS2812-lit sign on a pole. See
[`README.md`](README.md) for the hardware, strip layout and controls. Read it before changing anything
that touches pixel addressing or the dials.

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

## Building

There is no CI build and no test suite; changes are verified by flashing the board. Compile with the VS
Code Arduino extension (`.vscode/arduino.json` pins board, sketch and programmer) or the Arduino IDE,
targeting **Arduino Nano Every**. Requires the FastLED and Adafruit LIS3DH (1.2.2) libraries.

If you change something you can't flash, say so plainly rather than implying it was tested.
