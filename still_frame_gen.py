#!/usr/bin/python3

import struct
import numpy as np
import timeit
import argparse
import os
import sys

def time_form(secs):
  t_sec = round(secs)
  (t_min, t_sec) = divmod(t_sec,60)
  (t_hour,t_min) = divmod(t_min,60)

  return (t_hour, t_min, t_sec)


def is_odd(n):
  return True if n % 2 == 1 else False


def is_even(n):
  return True if n % 2 == 0 else False


def pack_r12l(chunk_x8_rgb):
  r = [chunk_x8_rgb[i*3+0] for i in range(8)]
  g = [chunk_x8_rgb[i*3+1] for i in range(8)]
  b = [chunk_x8_rgb[i*3+2] for i in range(8)]

  word0 = ((b[0] << 24) & 0xFF000000) + ((g[0] << 12) & 0x00FFF000) + ((r[0] << 0) & 0x00000FFF)
  word1 = ((b[1] << 28) & 0xF0000000) + ((g[1] << 16) & 0x0FFF0000) + ((r[1] << 4) & 0x0000FFF0) + ((b[0] >> 8) & 0x0000000F)
  word2 = ((g[2] << 20) & 0xFFF00000) + ((r[2] << 8) & 0x000FFF00) + ((b[1] >> 4) & 0x000000FF)

  word3 = ((g[3] << 24) & 0xFF000000) + ((r[3] << 12) & 0x00FFF000) + ((b[2] << 0) & 0x00000FFF)
  word4 = ((g[4] << 28) & 0xF0000000) + ((r[4] << 16) & 0x0FFF0000) + ((b[3] << 4) & 0x0000FFF0) + ((g[3] >> 8) & 0x0000000F)
  word5 = ((r[5] << 20) & 0xFFF00000) + ((b[4] << 8) & 0x000FFF00) + ((g[4] >> 4) & 0x000000FF)

  word6 = ((r[6] << 24) & 0xFF000000) + ((b[5] << 12) & 0x00FFF000) + ((g[5] << 0) & 0x00000FFF)
  word7 = ((r[7] << 28) & 0xF0000000) + ((b[6] << 16) & 0x0FFF0000) + ((g[6] << 4) & 0x0000FFF0) + ((r[6] >> 8) & 0x0000000F)
  word8 = ((b[7] << 20) & 0xFFF00000) + ((g[7] << 8) & 0x000FFF00) + ((r[7] >> 4) & 0x000000FF)

  return np.array([word0, word1, word2, word3, word4, word5, word6, word7, word8], dtype=np.uint32)


def std_msg(text, end='\n'):
  sys.stdout.write('{}{}'.format(
    text, end
  ))
  sys.stdout.flush()

# Add arguments via arg_parser
arg_parser = argparse.ArgumentParser()
arg_parser.add_argument('--out-file', dest='out_file', default=None, help="Path to output file.")
arg_parser.add_argument('--width', dest='width', default=None, help="Frame width.")
arg_parser.add_argument('--height', dest='height', default=None, help="Frame height.")
arg_parser.add_argument('--pattern', dest='pattern', default=None, help="Frame generation pattern.")
arg_parser.add_argument('--packing', dest='packing', default=None, help="Pixel packing format")
args = arg_parser.parse_args()

t_start = timeit.default_timer()

width = 3840 if args.width == None else int(args.width)
height = 2160 if args.height == None else int(args.height)
pattern_sel = 'diag-fade' if args.pattern == None else args.pattern
colour_chs = 3
bit_depth = 12

f_name = './test_still_frame.bin' if args.out_file == None else args.out_file
f_name_abs = os.path.abspath(os.path.expanduser(f_name))

if width * height % 8 != 0:
  std_msg('ERROR frame must be multiple of 8 number of pixels.')
  exit()

num_pixels = width * height
num_bits = num_pixels * colour_chs * bit_depth
num_bytes = int(num_bits / 8)
num_uint32s = int(num_bytes / 4)
packing_type = 'R12L' if args.packing == None else args.packing

std_msg('Frame      : {} x {}'.format(width, height))
std_msg('Num pixels : {:,} pix'.format(num_pixels))
std_msg('Num bits   : {:,} bits'.format(num_bits))
std_msg('Num bytes  : {:,} bytes'.format(num_bytes))
std_msg('Num uint32s: {:,} uint32s'.format(num_uint32s))
std_msg('Packing    : {}'.format(packing_type))
std_msg('Pattern    : {}'.format(pattern_sel))
std_msg('')

std_msg('Generating pixel array...', end='')
pixel_array = np.zeros((height, width, colour_chs), dtype=np.uint32)
std_msg(' DONE')
n = 0

std_msg('Calculating pixel values...', end='')

if pattern_sel == "bw-checker":
  for y in range(height):
    for x in range(width):
      if is_even(y):
        (r, g, b) = (0xFFF, 0xFFF, 0xFFF) if is_even(x) else (0x000, 0x000, 0x000)
      else:
        (r, g, b) = (0xFFF, 0xFFF, 0xFFF) if is_odd(x) else (0x000, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "red-checker":
  for y in range(height):
    for x in range(width):
      if is_even(y):
        (r, g, b) = (0xFFF, 0x000, 0x000) if is_even(x) else (0x000, 0x000, 0x000)
      else:
        (r, g, b) = (0xFFF, 0x000, 0x000) if is_odd(x) else (0x000, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "green-checker":
  for y in range(height):
    for x in range(width):
      if is_even(y):
        (r, g, b) = (0x000, 0xFFF, 0x000) if is_even(x) else (0x000, 0x000, 0x000)
      else:
        (r, g, b) = (0x000, 0xFFF, 0x000) if is_odd(x) else (0x000, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "blue-checker":
  for y in range(height):
    for x in range(width):
      if is_even(y):
        (r, g, b) = (0x000, 0x000, 0xFFF) if is_even(x) else (0x000, 0x000, 0x000)
      else:
        (r, g, b) = (0x000, 0x000, 0xFFF) if is_odd(x) else (0x000, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "bw-diag-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = ((x+y) % 4096, (x+y) % 4096, (x+y) % 4096)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "red-diag-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = ((x+y) % 4096, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "green-diag-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (0x000, (x+y) % 4096, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "blue-diag-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (0x000, 0x000, (x+y) % 4096)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "bw-horizontal-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (x % 4096, x % 4096, x % 4096)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "red-horizontal-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (x % 4096, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "green-horizontal-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (0x000, x % 4096, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "blue-horizontal-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (0x000, 0x000, x % 4096)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "bw-vertical-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (y % 4096, y % 4096, y % 4096)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "red-vertical-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (y % 4096, 0x000, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "green-vertical-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (0x000, y % 4096, 0x000)

      pixel_array[y][x] = (r, g, b)
      n += 1

elif pattern_sel == "blue-vertical-fade":
  for y in range(height):
    for x in range(width):
      (r, g, b) = (0x000, 0x000, y % 4096)

      pixel_array[y][x] = (r, g, b)
      n += 1

std_msg(' DONE')

std_msg('Flattening pixel array into stream...', end='')
pixel_array_flat = pixel_array.flatten()
std_msg(' DONE')

num_chunks = int(num_pixels*3/24)

std_msg('Packing pixels into {} format...'.format(packing_type), end='')
packed_pixel_array_flat = np.zeros(num_uint32s, dtype=np.uint32)

for idx in range(num_chunks):
  chunk = pixel_array_flat[(idx*24):(idx*24+24)]
  packed_pixel_array_flat[(idx*9):(idx*9+9)] = pack_r12l(chunk)

std_msg(' DONE')
# Write data direct to file
std_msg('Writing output file: {}...'.format(f_name_abs), end='')
packed_pixel_array_flat.tofile(f_name_abs)
std_msg(' DONE')

t_end = timeit.default_timer()
t_delta = t_end - t_start
(t_hour, t_min, t_sec) = time_form(t_delta)
std_msg('Total time: {} mins, {} secs\n'.format(t_min, t_sec))

