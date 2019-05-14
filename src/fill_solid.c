#include "fill_patterns.h"

void fill_solid(frame_t *frame, colour_t colour){
  uint16_t x, y;
  uint32_t idx;

  for (y=0; y<frame->height; y++){
    for (x=0; x<frame->width; x++){
      idx = y * frame->width + x;

      frame->pixels[idx].r = colour.r;
      frame->pixels[idx].g = colour.g;
      frame->pixels[idx].b = colour.b;
    }
  }
}