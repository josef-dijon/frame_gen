#ifndef FILL_PATTERNS_H
#define FILL_PATTERNS_H

#include "frame_gen.h"

void fill_solid(frame_t *frame, colour_t colour);
void fill_checker(frame_t *frame, colour_t colour);
void fill_ramp_h(frame_t *frame, colour_t colour);
void fill_ramp_v(frame_t *frame, colour_t colour);
void fill_ramp_d(frame_t *frame, colour_t colour);

#endif /* FILL_PATTERNS_H */