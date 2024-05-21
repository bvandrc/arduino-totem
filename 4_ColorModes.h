#ifndef COLOR_MODES_H
#define COLOR_MODES_H

enum ChaseDirection : uint8_t {
  FORWARD,
  BACKWARD,
};

extern void fillRainbowEdges(uint16_t first_pixel_hue, uint8_t num_cycles = 1);
extern void fillRainbowMiddles(uint16_t first_pixel_hue, uint8_t num_cycles = 2);
extern void rainbowChaseEdges(uint8_t num_cycles, uint16_t min_wait_millis, uint16_t max_wait_millis,
                              uint16_t rage_hue_step);
extern void rainbowChaseQuadrants(uint8_t num_cycles, uint16_t min_wait_millis, uint16_t max_wait_millis,
                                  uint16_t rage_hue_step);
extern void rainbowFade();
extern void theaterChaseCycle();
extern void tapFlash();

#endif