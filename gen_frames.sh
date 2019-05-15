#!/bin/bash

FRAME_GEN=./frame_gen/build/frame_gen
FRAME_GEN_OUT_DIR=frame_gen/build

rm -f ./*.R210
rm -f ./*.R12B
rm -f ./*.R12L

PATTERNS="solid checker horiz_ramp vert_ramp diag_ramp"
PACKS="R12L R210"
FORMATS="1280:720 1920:1080 2048:1080 3840:2160 4096:2160"

for FORMAT in ${FORMATS}
do
  WIDTH=`echo ${FORMAT} | cut -d ":" -f 1`
  HEIGHT=`echo ${FORMAT} | cut -d ":" -f 2`

  for PACK in ${PACKS}
  do
    PATT_NUM=0
    for PATT in ${PATTERNS}
    do
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 0    0    0    -- ${WIDTH}x${HEIGHT}_${PATT}_black.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 0    0    4096 -- ${WIDTH}x${HEIGHT}_${PATT}_blue.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 0    4096 0    -- ${WIDTH}x${HEIGHT}_${PATT}_green.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 0    4096 4096 -- ${WIDTH}x${HEIGHT}_${PATT}_cyan.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 4096 0    0    -- ${WIDTH}x${HEIGHT}_${PATT}_red.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 4096 0    4096 -- ${WIDTH}x${HEIGHT}_${PATT}_purple.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 4096 4096 0    -- ${WIDTH}x${HEIGHT}_${PATT}_yellow.${PACK}
      ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} ${PATT_NUM} 4096 4096 4096 -- ${WIDTH}x${HEIGHT}_${PATT}_white.${PACK}

      PATT_NUM=$((PATT_NUM + 1))
    done;

    ${FRAME_GEN} ${WIDTH} ${HEIGHT} ${PACK} 5 0 0 0 -- ${WIDTH}x${HEIGHT}_prbs15.${PACK}
  done;
done;

echo "done"