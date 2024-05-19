#ifndef COLOR_MODES_H
#define COLOR_MODES_H

enum QuadrantChaseDirection : uint8_t {
  UP,
  DOWN,
};

extern void fillRainbowEdges(uint16_t first_pixel_hue, uint8_t num_cycles = 1);
extern void fillRainbowMiddles(uint16_t first_pixel_hue, uint8_t num_cycles = 2);
extern void rainbowChaseEdges(uint8_t num_cycles);
extern void rainbowChaseQuadrants(uint8_t num_cycles);
extern void rainbowFade();
extern void theaterChaseCycle();
extern void rageFlash();
extern void tapFlash();

#endif