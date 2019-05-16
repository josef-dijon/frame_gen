#include "fill_patterns.h"

void fill_prbs15(frame_t *frame, colour_t colour){
  uint32_t i;
  uint16_t a = 0x0001;
  uint8_t bit = 0;

  for (i=0; i<frame->n_pixels; i++){
    bit = ((a >> 14) ^ (a >> 13)) & 1;
    a = ((a << 1) | bit) & 0x7FFF;
    frame->pixel_data[i].r = colour.r ? a : 0;

    bit = ((a >> 14) ^ (a >> 13)) & 1;
    a = ((a << 1) | bit) & 0x7FFF;
    frame->pixel_data[i].g = colour.g ? a : 0;

    bit = ((a >> 14) ^ (a >> 13)) & 1;
    a = ((a << 1) | bit) & 0x7FFF;
    frame->pixel_data[i].b = colour.b ? a : 0;
  }
}