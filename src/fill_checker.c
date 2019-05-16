#include "fill_patterns.h"

void fill_checker(frame_t *frame, colour_t colour){
  uint16_t x, y;
  uint32_t idx;

  for (y=0; y<frame->height; y++){
    for (x=0; x<frame->width; x++){
      idx = y * frame->width + x;

      if (y % 2 == 0) { /* Is even line */
        if (x % 2 == 0) { /* Is even pixel */
          frame->pixel_data[idx].r = colour.r;
          frame->pixel_data[idx].g = colour.g;
          frame->pixel_data[idx].b = colour.b;
        } else { /* Is odd pixel */
          frame->pixel_data[idx].r = 0;
          frame->pixel_data[idx].g = 0;
          frame->pixel_data[idx].b = 0;
        }
      } else { /* Is odd line */
        if (x % 2 == 1) { /* Is odd pixel */
          frame->pixel_data[idx].r = colour.r;
          frame->pixel_data[idx].g = colour.g;
          frame->pixel_data[idx].b = colour.b;
        } else { /* Is even pixel */
          frame->pixel_data[idx].r = 0;
          frame->pixel_data[idx].g = 0;
          frame->pixel_data[idx].b = 0;
        }
      }
    }
  }
}