#include "frame_gen.h"

pixel_t get_pixel(frame_t *frame, uint16_t x, uint16_t y){
  return frame->pixels[y * frame->width + x];
}

void set_pixel(frame_t *frame, pixel_t pixel, uint16_t x, uint16_t y){
  frame->pixels[y * frame->width + x] = pixel;
}

frame_t *frame_create(uint16_t width, uint16_t height, uint8_t colour_depth){
  frame_t *frame;
  frame = (frame_t *)malloc(sizeof(frame_t));

  frame->height = height;
  frame->width = width;
  frame->n_pixels = width * height;
  frame->colour_depth = colour_depth;
  frame->pixels = (pixel_t *)malloc(sizeof(pixel_t) * frame->n_pixels);

  return frame;
}

void frame_destroy(frame_t *frame){
  free(frame->pixels);
  free(frame);
}

void write_binary(frame_t *frame, const char *f_name){
  FILE *f;

  f = fopen(f_name, "wb");
  fwrite(frame->pixels, sizeof(pixel_t), frame->n_pixels, f);
  fclose(f);
}

pixel_t pixel_clamp(pixel_t *pixel, uint8_t colour_depth){
  pixel_t pixel_clamped = {
    .r = pixel->r & ((1 << colour_depth) - 1),
    .g = pixel->g & ((1 << colour_depth) - 1),
    .b = pixel->b & ((1 << colour_depth) - 1),
  };

  return pixel_clamped;
}

void frame_clamp(frame_t *frame){
  uint32_t i;
  
  for (i=0; i<frame->n_pixels; i++){
    frame->pixels[i] = pixel_clamp(&(frame->pixels[i]), frame->colour_depth);
  }
}

