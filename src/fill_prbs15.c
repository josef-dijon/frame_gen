#include "fill_patterns.h"

void fill_prbs15(frame_t *frame){
  uint32_t i;
  uint16_t a = 0x0001;
  uint8_t bit = 0;

  for (i=0; i<frame->n_pixels; i++){
    bit = ((a >> 14) ^ (a >> 13)) & 1;
    a = ((a << 1) | bit) & 0x7FFF;
    frame->pixels[i].r = a;

    bit = ((a >> 14) ^ (a >> 13)) & 1;
    a = ((a << 1) | bit) & 0x7FFF;
    frame->pixels[i].g = a;

    bit = ((a >> 14) ^ (a >> 13)) & 1;
    a = ((a << 1) | bit) & 0x7FFF;
    frame->pixels[i].b = a;
  }
}