#ifndef COLOR_MODE_UTILS_H
#define COLOR_MODE_UTILS_H

extern bool arrayIsUnique(uint32_t colors[], uint8_t len_colors);

extern uint32_t getDelay(uint32_t low_time, uint32_t high_time);
extern bool wait(uint32_t low_millis, uint32_t high_millis);

extern void getNewColor(uint32_t &color);
extern void getTwoNewColors(uint32_t &color1, uint32_t &color2);
extern void getNewPixel(uint8_t &pixel);
extern uint32_t getRandomColor();

#endif