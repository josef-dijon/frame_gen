#include "fill_patterns.h"

void fill_ramp_v(frame_t *frame, colour_t colour, uint16_t max_ramp){
  uint16_t x, y;
  uint32_t idx;

  for (y=0; y<frame->height; y++){
    for (x=0; x<frame->width; x++){
      idx = y * frame->width + x;

      frame->pixel_data[idx].r = colour.r ? (y % max_ramp) : 0;
      frame->pixel_data[idx].g = colour.g ? (y % max_ramp) : 0;
      frame->pixel_data[idx].b = colour.b ? (y % max_ramp) : 0;
    }
  }
}