#ifndef COLOR_MODE_UTILS_H
#define COLOR_MODE_UTILS_H

extern const uint32_t RED;
extern const uint32_t ORANGE;
extern const uint32_t YELLOW;
extern const uint32_t GREEN;
extern const uint32_t SEAFOAM;
extern const uint32_t BLUE;
extern const uint32_t PURPLE;
extern const uint32_t WHITE;
extern const uint32_t OFF;

extern const uint32_t COLORS[];
extern const uint8_t LEN_COLORS;

extern bool arrayIsUnique(uint32_t colors[], uint8_t len_colors);

extern uint32_t getDelay(uint32_t lowTime, uint32_t highTime);
extern bool wait(uint32_t lowMillis, uint32_t highMillis);

extern void stripSetPixelColorBothSides(uint16_t n, uint32_t c);
extern void stripSetPixelColorQuadrants(uint16_t n, uint32_t c);
extern void stripSetPixelColorQuadrant(uint8_t q, uint16_t n, uint32_t c);
extern void stripSetSolid(uint32_t c);
extern void getNewColor(uint32_t &color);
extern void getTwoNewColors(uint32_t &color1, uint32_t &color2);
extern void getNewPixel(uint8_t &pixel);
extern uint32_t getRandomColor();

#endif