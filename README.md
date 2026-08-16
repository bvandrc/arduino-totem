# Arduino Totem

Arduino C++ firmware for a festival totem — a double-sided, LED-lit sign carried on a pole, driven by
a WS2812 strip and controlled live with two rotary selectors, two potentiometers, a "rage" button and
a tap-sensitive accelerometer.

Both faces of the totem are lit identically (mirrored), so the totem looks the same from either side.

## Hardware

| Part | Notes |
| --- | --- |
| Arduino Nano Every | `arduino:megaavr:nona4809` |
| WS2812 LED strip | 202 pixels total, GRB, data on pin `5` |
| Rotary selector (top) | 6 positions, `A1` — picks the lighting mode |
| Rotary selector (bottom) | 6 positions, `A0` — picks the rage-button behavior |
| Potentiometer (brightness) | `A3` |
| Potentiometer (speed) | `A2` |
| "Rage" button | pin `2`, `INPUT_PULLUP`, wired normally-closed to GND (pressing it opens the circuit and reads HIGH) |
| Adafruit LIS3DH accelerometer | I2C `0x18`, interrupt on pin `3` — single-tap detection |
| Microphone | `A7`, wired but not currently used |

### Strip layout

The strip is one continuous run wired across both faces. Each face is 101 pixels:

```
             top edge (28)
          ┌──────────────────┐
   left   │                  │   right
 edge(10) │   middle (25)    │  edge (10)
          └──────────────────┘
            bottom edge (28)
```

- **Edge** — the 76-pixel border around one face (`NUM_AROUND_EDGE`).
- **Quadrant** — a quarter of the totem, 38 pixels (`NUM_IN_QUADRANT`). Quadrants 1 and 2 are on side 1,
  3 and 4 on side 2; 1 and 4 are the left-hand ones.
- **Middle** — the 25-pixel run behind the lettering. Only a cropped window of it is lit (indices 3–22
  on side 1, 6–19 on side 2) so light doesn't spill past the artwork; `showStrip()` force-blanks the rest
  on every frame.

`2_HardwareOutputs.ino` exists to hide all of this: you address pixels by *edge*, *quadrant* or *middle*
index and it maps that to the physical index, mirroring side 2 so animations run the same direction on
both faces.

### Power

`MAX_BRIGHTNESS` is capped at `20` — the brightness dial maps to `0..20`, not `0..255`. Measured current
draw at various brightness levels for the 2021 (72 px) and 2022 (156 px) builds is tabulated in the
comment at the top of `initStrip()` in `2_HardwareOutputs.ino`.

## Files

Arduino concatenates every `.ino` in the sketch folder in alphabetical order, so the files are
number-prefixed to keep that order meaningful. Each has a matching `.h` declaring what the others may use.

| File | Contents |
| --- | --- |
| `Totem.ino` | `setup()` / `loop()`, boot diagnostics, mode dispatch, and `readAllInputs()` — the single place every input is polled |
| `1_HardwareInputs.ino` | Pin definitions, dial/button reads, `MyMotionSensor` (LIS3DH subclass) |
| `2_HardwareOutputs.ino` | Strip init and the edge/quadrant/middle → physical index mapping |
| `3_ColorModeUtils.ino` | `wait()` (the speed-dial-aware frame delay), random color selection, gamma correction, `ColorHSV()` |
| `4_ColorModes.ino` | The animations themselves, plus rage and tap effects |
| `5_Debug.ino` | Runtime debug menu for toggling settings without a reflash |
| `Graphics/` | Cut/print artwork — dial faceplates and the `Wonka` and `Yacht` totem designs (SVG + PDF) |

## Controls

### Top dial — lighting mode

| Position | Mode |
| --- | --- |
| 1 | Rainbow chase around the edge, 1 hue cycle |
| 2 | Rainbow chase around the edge, 2 hue cycles |
| 3 | Rainbow chase around the edge, 4 hue cycles |
| 4 | Rainbow chase around each quadrant (direction alternates each rage press) |
| 5 | Theater chase — two random colors, re-rolled every 10s |
| 6 | Whole-totem rainbow fade |

The middle section runs its own rainbow fill underneath modes 1–4.

### Bottom dial — what the rage button does

Hold the rage button; the effect runs until you let go.

| Position | Effect |
| --- | --- |
| 1 | Accelerate — the chase ramps up while held, then spins down with a brightness flicker on release |
| 2–4 | Speed up to a fixed fast hue step |
| 5 | Flash the current pattern by pulsing brightness |
| 6 | Strobe a random solid color |

### Potentiometers

- **Brightness** — maps directly onto `FastLED.setBrightness()`.
- **Speed** — sets the per-frame delay between each mode's `min_wait_millis` and `max_wait_millis`.
  Turned below a threshold of `10` the animation **freezes** on the current frame.

### Tap

If the accelerometer is enabled, a single tap fires `tapFlash()`: the totem fills with one random color
while a 15-pixel "bullet" of a second color runs around all four quadrants at once.

## Debug and boot diagnostics

**At power-on** — hold the rage button for 5 seconds while the sketch boots. The dial positions select
a diagnostic; the normal lighting loop does not start for the top-dial options:

| Dial | Position | Result |
| --- | --- | --- |
| Top | 2 | Dim static rainbow on edges and middles (check every pixel) |
| Top | 3 | Dim solid green (check for dead pixels / wiring) |
| Bottom | 2 | Boot with tap detection disabled |

**At runtime** — press the rage button while flipping the top dial back and forth between positions 1 and
6. Five alternating presses opens debug mode: the totem goes solid blue for 5s, then acts on the bottom
dial position (position 2 toggles tap detection; green flash = now on, red = now off). Other positions
are reserved.

## Building

Built with the [VS Code Arduino extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino);
`.vscode/arduino.json` pins the board, sketch and programmer, and `.vscode/c_cpp_properties.json` wires up
IntelliSense. The Arduino IDE works too — open `Totem.ino`.

Libraries required:

- [FastLED](https://github.com/FastLED/FastLED)
- [Adafruit LIS3DH](https://github.com/adafruit/Adafruit_LIS3DH) 1.2.2 (pulls in Adafruit BusIO and
  Adafruit Unified Sensor)

Select **Arduino Nano Every** as the board, set the port, and upload.

## Conventions

`conventions/` holds coding conventions synced from
[bvandrc/bvandrc-conventions](https://github.com/bvandrc/bvandrc-conventions) by
`.github/workflows/sync-conventions.yml` (Mondays, or on demand). That directory is overwritten on every
sync — edit the conventions upstream, not here.

`CLAUDE.md` points Claude Code at those conventions and adds the rules specific to this sketch — file
ordering, the memory constraints of the board, and how the animation loops handle input.
