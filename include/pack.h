#ifndef PACK_H
#define PACK_H

#include "frame_gen.h"

typedef enum packing_t {
  V210,
  ARGB,
  BGRA,
  R210,
  R12B,
  R12L
} packing_t;

typedef struct packed_frame_t {
  uint16_t height;
  uint16_t width;
  uint32_t n_pixels;
  packing_t packing;
  cs_t cspace;
  uint16_t cdepth;
  uint32_t len_32;
  uint32_t *pixel_data;
} packed_frame_t;


packed_frame_t *packed_frame_create(frame_t *frame, packing_t packing);
void packed_frame_destroy(packed_frame_t *packed_frame);
packing_t packing_decode_str(const char *str);
uint8_t cdepth_from_packing(packing_t packing);
cs_t cspace_from_packing(packing_t packing);
packed_frame_t *pack_V210(frame_t *frame);
packed_frame_t *pack_ARGB(frame_t *frame);
packed_frame_t *pack_BGRA(frame_t *frame);
packed_frame_t *pack_R210(frame_t *frame);
packed_frame_t *pack_R12B(frame_t *frame);
packed_frame_t *pack_R12L(frame_t *frame);
void write_packed_binary(packed_frame_t *packed_frame, const char *f_name);

#endif /* PACK_H */