#include "pack.h"

packed_frame_t *packed_frame_create(frame_t *frame, packing_t packing){
  packed_frame_t *packed_frame;
  packed_frame = (packed_frame_t *)malloc(sizeof(packed_frame_t));

  packed_frame->height = frame->height;
  packed_frame->width = frame->width;
  packed_frame->n_pixels = frame->n_pixels;
  packed_frame->packing = packing;

  switch (packing) {
    case R210:
      packed_frame->len_32 = frame->n_pixels;
      break;
    case R12B:
      packed_frame->len_32 = (frame->n_pixels * 9) / 8;
      break;
    case R12L:
      packed_frame->len_32 = (frame->n_pixels * 9) / 8;
    default:
      break;
  }

  packed_frame->pixel_data = (uint32_t *)malloc(sizeof(uint32_t) * packed_frame->len_32);

  return packed_frame;
}

void packed_frame_destroy(packed_frame_t *packed_frame){
  free(packed_frame->pixel_data);
  free(packed_frame);
}

packed_frame_t *pack_r210(frame_t *frame){
  packed_frame_t *packed_frame = packed_frame_create(frame, R210);
  uint32_t i;

  for (i=0; i<frame->n_pixels; i++){
    packed_frame->pixel_data[i] = 0;
    /* Red */
    packed_frame->pixel_data[i] += ((frame->pixels[i].r & 0x000F) << 12) + ((frame->pixels[i].r & 0x03F0) >> 4);
    /* Green */
    packed_frame->pixel_data[i] += ((frame->pixels[i].g & 0x003F) << 18) + ((frame->pixels[i].g & 0x03C0) << 2);
    /* Blue */
    packed_frame->pixel_data[i] += ((frame->pixels[i].b & 0x00FF) << 24) + ((frame->pixels[i].b & 0x0300) << 8);
  }

  return packed_frame;
}

packed_frame_t *pack_r12b(frame_t *frame){
  packed_frame_t *packed_frame = packed_frame_create(frame, R12B);

  uint32_t i;
  uint8_t j;

  for (i=0; i<frame->n_pixels/8; i+=1){
    for (j=0; j<9; j++){
      packed_frame->pixel_data[i*9+j] = 0;
    }

    /* Red */
    packed_frame->pixel_data[i*9+0] += ((frame->pixels[i*8+0].r & 0x0FFF) << 0);
    packed_frame->pixel_data[i*9+1] += ((frame->pixels[i*8+1].r & 0x0FFF) << 4);
    packed_frame->pixel_data[i*9+2] += ((frame->pixels[i*8+2].r & 0x0FFF) << 8);
    packed_frame->pixel_data[i*9+3] += ((frame->pixels[i*8+3].r & 0x0FFF) << 12);
    packed_frame->pixel_data[i*9+4] += ((frame->pixels[i*8+4].r & 0x0FFF) << 16);
    packed_frame->pixel_data[i*9+5] += ((frame->pixels[i*8+5].r & 0x0FFF) << 20);
    packed_frame->pixel_data[i*9+6] += ((frame->pixels[i*8+6].r & 0x00FF) << 24);
    packed_frame->pixel_data[i*9+7] += ((frame->pixels[i*8+6].r & 0x0F00) >> 8);
    packed_frame->pixel_data[i*9+7] += ((frame->pixels[i*8+7].r & 0x000F) << 28);
    packed_frame->pixel_data[i*9+8] += ((frame->pixels[i*8+7].r & 0x0FF0) >> 4);

    /* Green */
    packed_frame->pixel_data[i*9+0] += ((frame->pixels[i*8+0].g & 0x0FFF) << 12);
    packed_frame->pixel_data[i*9+1] += ((frame->pixels[i*8+1].g & 0x0FFF) << 16);
    packed_frame->pixel_data[i*9+2] += ((frame->pixels[i*8+2].g & 0x0FFF) << 20);
    packed_frame->pixel_data[i*9+3] += ((frame->pixels[i*8+3].g & 0x00FF) << 24);
    packed_frame->pixel_data[i*9+4] += ((frame->pixels[i*8+3].g & 0x0F00) >> 8);
    packed_frame->pixel_data[i*9+4] += ((frame->pixels[i*8+4].g & 0x000F) << 28);
    packed_frame->pixel_data[i*9+5] += ((frame->pixels[i*8+4].g & 0x0FF0) >> 4);
    packed_frame->pixel_data[i*9+6] += ((frame->pixels[i*8+5].g & 0x0FFF) << 0);
    packed_frame->pixel_data[i*9+7] += ((frame->pixels[i*8+6].g & 0x0FFF) << 4);
    packed_frame->pixel_data[i*9+8] += ((frame->pixels[i*8+7].g & 0x0FFF) << 8);

    /* Blue */
    packed_frame->pixel_data[i*9+0] += ((frame->pixels[i*8+0].b & 0x00FF) << 24);
    packed_frame->pixel_data[i*9+1] += ((frame->pixels[i*8+0].b & 0x0F00) >> 8);
    packed_frame->pixel_data[i*9+1] += ((frame->pixels[i*8+1].b & 0x000F) << 28);
    packed_frame->pixel_data[i*9+2] += ((frame->pixels[i*8+1].b & 0x0FF0) >> 4);
    packed_frame->pixel_data[i*9+3] += ((frame->pixels[i*8+2].b & 0x0FFF) << 0);
    packed_frame->pixel_data[i*9+4] += ((frame->pixels[i*8+3].b & 0x0FFF) << 4);
    packed_frame->pixel_data[i*9+5] += ((frame->pixels[i*8+4].b & 0x0FFF) << 8);
    packed_frame->pixel_data[i*9+6] += ((frame->pixels[i*8+5].b & 0x0FFF) << 12);
    packed_frame->pixel_data[i*9+7] += ((frame->pixels[i*8+6].b & 0x0FFF) << 16);
    packed_frame->pixel_data[i*9+8] += ((frame->pixels[i*8+7].b & 0x0FFF) << 20);
  }

  return packed_frame;
}

packed_frame_t *pack_r12l(frame_t *frame){
  packed_frame_t *r12b_frame = pack_r12b(frame);
  packed_frame_t *packed_frame = packed_frame_create(frame, R12L);
  uint32_t i;

  for (i=0; i<r12b_frame->len_32; i++){
    packed_frame->pixel_data[i] = 0;
    packed_frame->pixel_data[i] += (r12b_frame->pixel_data[i] & 0xFF000000) >> 24;
    packed_frame->pixel_data[i] += (r12b_frame->pixel_data[i] & 0x000000FF) << 24;
    packed_frame->pixel_data[i] += (r12b_frame->pixel_data[i] & 0x00FF0000) >> 8;
    packed_frame->pixel_data[i] += (r12b_frame->pixel_data[i] & 0x0000FF00) << 8;
  }

  packed_frame_destroy(r12b_frame);

  return packed_frame;
}

void write_packed_binary(packed_frame_t *packed_frame, const char *f_name){
  FILE *f;

  f = fopen(f_name, "wb");
  fwrite(packed_frame->pixel_data, sizeof(uint32_t), packed_frame->len_32, f);
  fclose(f);
}