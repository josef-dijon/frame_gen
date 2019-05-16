#include "pack.h"

packed_frame_t *packed_frame_create(frame_t *frame, packing_t packing){
  packed_frame_t *packed_frame;
  packed_frame = (packed_frame_t *)malloc(sizeof(packed_frame_t));

  packed_frame->height = frame->height;
  packed_frame->width = frame->width;
  packed_frame->n_pixels = frame->n_pixels;
  packed_frame->packing = packing;
  packed_frame->cdepth = cdepth_from_packing(packing);
  packed_frame->cspace = cspace_from_packing(packing);

  switch (packing) {
    case BGRA:
    case ARGB:
    case R210:
      packed_frame->len_32 = frame->n_pixels;
      break;
    case V210:
      packed_frame->len_32 = (frame->n_pixels * 4) / 6;
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

packing_t packing_decode_str(const char *str){
  packing_t packing;

  if (strcmp(str, "v210") == 0) {
    packing = V210;
  } else if (strcmp(str, "ARGB") == 0) {
    packing = ARGB;
  } else if (strcmp(str, "BGRA") == 0) {
    packing = BGRA;
  } else if (strcmp(str, "R210") == 0) {
    packing = R210;
  } else if (strcmp(str, "R12B") == 0){
    packing = R12B;
  } else if (strcmp(str, "R12L") == 0){
    packing = R12L;
  }

  return packing;
}

uint8_t cdepth_from_packing(packing_t packing){
  switch (packing){
    case ARGB:
    case BGRA:
      return 8;
    case V210:
    case R210:
      return 10;
    case R12L:
    case R12B:
      return 12;
    default:
      break;
  }
}

cs_t cspace_from_packing(packing_t packing){
  switch (packing){
    case V210:
      return YUV422;
    case ARGB:
    case BGRA:
    case R210:
    case R12L:
    case R12B:
      return RGB444;
    default:
      break;
  }
}

packed_frame_t *pack_V210(frame_t *frame){
  packed_frame_t *packed_frame = packed_frame_create(frame, V210);
  uint32_t i;

  for (i=0; i<packed_frame->len_32; i++){

  }

  return packed_frame;
}

packed_frame_t *pack_ARGB(frame_t *frame){
  packed_frame_t *packed_frame = packed_frame_create(frame, ARGB);
  uint32_t i;

  for (i=0; i<packed_frame->len_32; i++){
    packed_frame->pixel_data[i] = 0;
    /* Red */
    packed_frame->pixel_data[i] += (frame->pixel_data[i].r & 0x00FF) << 8;
    /* Green */
    packed_frame->pixel_data[i] += (frame->pixel_data[i].g & 0x00FF) << 16;
    /* Blue */
    packed_frame->pixel_data[i] += (frame->pixel_data[i].b & 0x00FF) << 24;
  }

  return packed_frame;
}

packed_frame_t *pack_BGRA(frame_t *frame){
  packed_frame_t *argb_frame = pack_ARGB(frame);
  packed_frame_t *packed_frame = packed_frame_create(frame, BGRA);
  uint32_t i;

  for (i=0; i<argb_frame->len_32; i++){
    packed_frame->pixel_data[i] = 0;
    packed_frame->pixel_data[i] += (argb_frame->pixel_data[i] & 0xFF000000) >> 24;
    packed_frame->pixel_data[i] += (argb_frame->pixel_data[i] & 0x000000FF) << 24;
    packed_frame->pixel_data[i] += (argb_frame->pixel_data[i] & 0x00FF0000) >> 8;
    packed_frame->pixel_data[i] += (argb_frame->pixel_data[i] & 0x0000FF00) << 8;
  }

  packed_frame_destroy(argb_frame);

  return packed_frame;
}

packed_frame_t *pack_R210(frame_t *frame){
  packed_frame_t *packed_frame = packed_frame_create(frame, R210);
  uint32_t i;

  for (i=0; i<frame->n_pixels; i++){
    packed_frame->pixel_data[i] = 0;
    /* Red */
    packed_frame->pixel_data[i] += ((frame->pixel_data[i].r & 0x000F) << 12) + ((frame->pixel_data[i].r & 0x03F0) >> 4);
    /* Green */
    packed_frame->pixel_data[i] += ((frame->pixel_data[i].g & 0x003F) << 18) + ((frame->pixel_data[i].g & 0x03C0) << 2);
    /* Blue */
    packed_frame->pixel_data[i] += ((frame->pixel_data[i].b & 0x00FF) << 24) + ((frame->pixel_data[i].b & 0x0300) << 8);
  }

  return packed_frame;
}

packed_frame_t *pack_R12L(frame_t *frame){
  packed_frame_t *packed_frame = packed_frame_create(frame, R12L);

  uint32_t i;
  uint8_t j;

  for (i=0; i<frame->n_pixels/8; i+=1){
    for (j=0; j<9; j++){
      packed_frame->pixel_data[i*9+j] = 0;
    }

    /* Red */
    packed_frame->pixel_data[i*9+0] += ((frame->pixel_data[i*8+0].r & 0x0FFF) << 0);
    packed_frame->pixel_data[i*9+1] += ((frame->pixel_data[i*8+1].r & 0x0FFF) << 4);
    packed_frame->pixel_data[i*9+2] += ((frame->pixel_data[i*8+2].r & 0x0FFF) << 8);
    packed_frame->pixel_data[i*9+3] += ((frame->pixel_data[i*8+3].r & 0x0FFF) << 12);
    packed_frame->pixel_data[i*9+4] += ((frame->pixel_data[i*8+4].r & 0x0FFF) << 16);
    packed_frame->pixel_data[i*9+5] += ((frame->pixel_data[i*8+5].r & 0x0FFF) << 20);
    packed_frame->pixel_data[i*9+6] += ((frame->pixel_data[i*8+6].r & 0x00FF) << 24);
    packed_frame->pixel_data[i*9+7] += ((frame->pixel_data[i*8+6].r & 0x0F00) >> 8);
    packed_frame->pixel_data[i*9+7] += ((frame->pixel_data[i*8+7].r & 0x000F) << 28);
    packed_frame->pixel_data[i*9+8] += ((frame->pixel_data[i*8+7].r & 0x0FF0) >> 4);

    /* Green */
    packed_frame->pixel_data[i*9+0] += ((frame->pixel_data[i*8+0].g & 0x0FFF) << 12);
    packed_frame->pixel_data[i*9+1] += ((frame->pixel_data[i*8+1].g & 0x0FFF) << 16);
    packed_frame->pixel_data[i*9+2] += ((frame->pixel_data[i*8+2].g & 0x0FFF) << 20);
    packed_frame->pixel_data[i*9+3] += ((frame->pixel_data[i*8+3].g & 0x00FF) << 24);
    packed_frame->pixel_data[i*9+4] += ((frame->pixel_data[i*8+3].g & 0x0F00) >> 8);
    packed_frame->pixel_data[i*9+4] += ((frame->pixel_data[i*8+4].g & 0x000F) << 28);
    packed_frame->pixel_data[i*9+5] += ((frame->pixel_data[i*8+4].g & 0x0FF0) >> 4);
    packed_frame->pixel_data[i*9+6] += ((frame->pixel_data[i*8+5].g & 0x0FFF) << 0);
    packed_frame->pixel_data[i*9+7] += ((frame->pixel_data[i*8+6].g & 0x0FFF) << 4);
    packed_frame->pixel_data[i*9+8] += ((frame->pixel_data[i*8+7].g & 0x0FFF) << 8);

    /* Blue */
    packed_frame->pixel_data[i*9+0] += ((frame->pixel_data[i*8+0].b & 0x00FF) << 24);
    packed_frame->pixel_data[i*9+1] += ((frame->pixel_data[i*8+0].b & 0x0F00) >> 8);
    packed_frame->pixel_data[i*9+1] += ((frame->pixel_data[i*8+1].b & 0x000F) << 28);
    packed_frame->pixel_data[i*9+2] += ((frame->pixel_data[i*8+1].b & 0x0FF0) >> 4);
    packed_frame->pixel_data[i*9+3] += ((frame->pixel_data[i*8+2].b & 0x0FFF) << 0);
    packed_frame->pixel_data[i*9+4] += ((frame->pixel_data[i*8+3].b & 0x0FFF) << 4);
    packed_frame->pixel_data[i*9+5] += ((frame->pixel_data[i*8+4].b & 0x0FFF) << 8);
    packed_frame->pixel_data[i*9+6] += ((frame->pixel_data[i*8+5].b & 0x0FFF) << 12);
    packed_frame->pixel_data[i*9+7] += ((frame->pixel_data[i*8+6].b & 0x0FFF) << 16);
    packed_frame->pixel_data[i*9+8] += ((frame->pixel_data[i*8+7].b & 0x0FFF) << 20);
  }

  return packed_frame;
}

packed_frame_t *pack_R12B(frame_t *frame){
  packed_frame_t *r12l_frame = pack_R12L(frame);
  packed_frame_t *packed_frame = packed_frame_create(frame, R12B);
  uint32_t i;

  for (i=0; i<r12l_frame->len_32; i++){
    packed_frame->pixel_data[i] = 0;
    packed_frame->pixel_data[i] += (r12l_frame->pixel_data[i] & 0xFF000000) >> 24;
    packed_frame->pixel_data[i] += (r12l_frame->pixel_data[i] & 0x000000FF) << 24;
    packed_frame->pixel_data[i] += (r12l_frame->pixel_data[i] & 0x00FF0000) >> 8;
    packed_frame->pixel_data[i] += (r12l_frame->pixel_data[i] & 0x0000FF00) << 8;
  }

  packed_frame_destroy(r12l_frame);

  return packed_frame;
}

void write_packed_binary(packed_frame_t *packed_frame, const char *f_name){
  FILE *f;

  f = fopen(f_name, "wb");
  fwrite(packed_frame->pixel_data, sizeof(uint32_t), packed_frame->len_32, f);
  fclose(f);
}