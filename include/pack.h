#ifndef PACK_H
#define PACK_H

#include "frame_gen.h"

typedef enum packing_t {
  R210,
  R12B,
  R12L
} packing_t;

typedef struct packed_frame_t {
  uint16_t height;
  uint16_t width;
  uint32_t n_pixels;
  packing_t packing;
  uint32_t len_32;
  uint32_t *pixel_data;
} packed_frame_t;


packed_frame_t *packed_frame_create(frame_t *frame, packing_t packing);
void packed_frame_destroy(packed_frame_t *packed_frame);
packing_t packing_decode(const char *str);
packed_frame_t *pack_r210(frame_t *frame);
packed_frame_t *pack_r12b(frame_t *frame);
packed_frame_t *pack_r12l(frame_t *frame);
void write_packed_binary(packed_frame_t *packed_frame, const char *f_name);

#endif /* PACK_H */