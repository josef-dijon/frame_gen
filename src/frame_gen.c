#include "frame_gen.h"

pixel_t get_pixel(frame_t *frame, uint16_t x, uint16_t y){
  return frame->pixel_data[y * frame->width + x];
}

void set_pixel(frame_t *frame, pixel_t pixel, uint16_t x, uint16_t y){
  frame->pixel_data[y * frame->width + x] = pixel;
}

frame_t *frame_create(uint16_t width, uint16_t height, uint8_t cdepth, cs_t cspace){
  frame_t *frame;
  frame = (frame_t *)malloc(sizeof(frame_t));

  frame->height = height;
  frame->width = width;
  frame->n_pixels = width * height;
  frame->cdepth = cdepth;
  frame->cspace = cspace;
  frame->pixel_data = (pixel_t *)malloc(sizeof(pixel_t) * frame->n_pixels);

  return frame;
}

void frame_destroy(frame_t *frame){
  free(frame->pixel_data);
  free(frame);
}

void write_binary(frame_t *frame, const char *f_name){
  FILE *f;

  f = fopen(f_name, "wb");
  fwrite(frame->pixel_data, sizeof(pixel_t), frame->n_pixels, f);
  fclose(f);
}

pixel_t pixel_clamp(pixel_t *pixel, uint16_t min_clamp_value, uint16_t max_clamp_value){
  pixel_t pixel_clamped;

  if (pixel->r < min_clamp_value){
    pixel_clamped.r = min_clamp_value;
  } else if (pixel->r > max_clamp_value){
    pixel_clamped.r = max_clamp_value;
  } else{
    pixel_clamped.r = pixel->r;
  }

  if (pixel->g < min_clamp_value){
    pixel_clamped.g = min_clamp_value;
  } else if (pixel->g > max_clamp_value){
    pixel_clamped.g = max_clamp_value;
  } else{
    pixel_clamped.g = pixel->g;
  }

  if (pixel->b < min_clamp_value){
    pixel_clamped.b = min_clamp_value;
  } else if (pixel->b > max_clamp_value){
    pixel_clamped.b = max_clamp_value;
  } else{
    pixel_clamped.b = pixel->b;
  }

  return pixel_clamped;
}

void frame_clamp(frame_t *frame, uint16_t min_clamp_value, uint16_t max_clamp_value){
  uint32_t i;

  for (i=0; i<frame->n_pixels; i++){
    frame->pixel_data[i] = pixel_clamp(&(frame->pixel_data[i]), min_clamp_value, max_clamp_value);
  }
}

