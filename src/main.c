/* Josef de Joanelli (jmdejoanelli@gmail.com) */

#include "frame_gen.h"
#include "fill_patterns.h"
#include "pack.h"

void print_usage(void);

int main(int argc, char *argv[]){
  if (argc != 10){
    printf("%s : ERROR : Require 9 arguments, only %d provided...\n", argv[0], argc-1);
    print_usage();
    return 1;
  }

  printf("Frame Gen v0.1\n");

  frame_t *frame;
  packed_frame_t *packed_frame;
  frame = frame_create(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

  if (frame->n_pixels % 8 != 0){
    printf("%s : ERROR : Total number of pixels must be divisable by 8...\n", argv[0]);
    return 2;
  }

  printf("Frame width     : %d\n", frame->width);
  printf("Frame height    : %d\n", frame->height);
  printf("Colour depth    : %d bits\n", frame->colour_depth);
  printf("Pixels          : %d\n", frame->n_pixels);

  switch (atoi(argv[4])) {
    case 0:
      printf("Pattern         : solid\n");
      break;
    case 1:
      printf("Pattern         : checker\n");
      break;
    case 2:
      printf("Pattern         : horizontal ramp\n");
      break;
    case 3:
      printf("Pattern         : vertical ramp\n");
      break;
    case 4:
      printf("Pattern         : diagonal ramp\n");
      break;
    default:
      break;
  }

  printf("File size       : %d bytes (%0.3f MB)\n", frame->n_pixels*6, (float)frame->n_pixels*6/1024/1024);

  colour_t colour = {atoi(argv[5]), atoi(argv[6]), atoi(argv[7])};
  
  switch (atoi(argv[4])) {
    case 0:
      fill_solid(frame, colour);
      break;
    case 1:
      fill_checker(frame, colour);
      break;
    case 2:
      fill_ramp_h(frame, colour, ((1 << frame->colour_depth) - 1));
      break;
    case 3:
      fill_ramp_v(frame, colour, ((1 << frame->colour_depth) - 1));
      break;
    case 4:
      fill_ramp_d(frame, colour, ((1 << frame->colour_depth) - 1));
      break;
    default:
      break;
  }

  frame_clamp(frame, ((1 << frame->colour_depth) - 1));
  write_binary(frame, argv[8]);

  switch (frame->colour_depth){
    case 10:
      packed_frame = pack_r210(frame);
      break;
    case 12:
      packed_frame = pack_r12b(frame);
    default:
      break;
  }

  printf("Packed file size: %d bytes (%0.3f MB)\n", packed_frame->len_32*4, (float)packed_frame->len_32*4/1024/1024);

  frame_destroy(frame);

  write_packed_binary(packed_frame, argv[9]);
  packed_frame_destroy(packed_frame);

  return 0;
}

void print_usage(void){
  printf("Usage: ./frame_gen width height depth pattern r g b frame_path packed_frame_path\n");
  printf("  width               frame width\n");
  printf("  height              frame height\n");
  printf("  depth               frame bit depth - 10 and 12 bits are valid\n");
  printf("  pattern             frame pattern:\n");
  printf("                        0 solid fill\n");
  printf("                        1 checker fill\n");
  printf("                        2 horizontal ramp\n");
  printf("                        3 vertical ramp\n");
  printf("                        4 diagonal ramp\n");
  printf("  r g b               RGB values used to fill the patterns\n");
  printf("  frame_path          path to output unpacked frame to\n");
  printf("  packed_frame_path   path to output packed frame to\n");
};
