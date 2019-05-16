#ifndef FRAME_GEN_H
#define FRAME_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum cs_t {
  YUV422,
  RGB444
} cs_t;

typedef struct pixel_t {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} pixel_t;

typedef struct frame_t {
  uint16_t height;
  uint16_t width;
  uint32_t n_pixels;
  uint8_t cdepth;
  cs_t cspace;
  pixel_t *pixel_data;
} frame_t;

typedef pixel_t colour_t;

frame_t *frame_create(uint16_t width, uint16_t height, uint8_t colour_depth, cs_t cspace);
void frame_destroy(frame_t *frame);
pixel_t get_pixel(frame_t *frame, uint16_t x, uint16_t y);
void set_pixel(frame_t *frame, pixel_t pixel, uint16_t x, uint16_t y);
void write_binary(frame_t *frame, const char *f_name);
pixel_t pixel_clamp(pixel_t *pixel, uint16_t min_clamp_value, uint16_t max_clamp_value);
void frame_clamp(frame_t *frame, uint16_t min_clamp_value, uint16_t max_clamp_value);

#endif /* FRAME_GEN_H */
