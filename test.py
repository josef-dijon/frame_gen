#!/usr/bin/env python3

import BMDTesting
import json
import sys
import os
import argparse
import subprocess

from pprint import pprint as pp

arg_parser = argparse.ArgumentParser()
arg_parser.add_argument('-o', '--output',
                        dest='output_port',
                        default=None,
                        choices=['sdi', 'hdmi'],
                        help='Output port select.')
arg_parser.add_argument('-i', '--input',
                        dest='input_port',
                        default=None,
                        choices=['sdi', 'hdmi'],
                        help='Input port select.')
arg_parser.add_argument('-l', '--level',
                        dest='sdi_level',
                        default='b',
                        choices=['b', 'a'],
                        help='SDI Level')
arg_parser.add_argument('-r, --resolution',
                        dest='resolution',
                        default='1080p',
                        choices=['pal', 'ntsc', '720p', '1080p', '2kdci', '2160p', '4kdci'],
                        help='Include burn-in timecode.')
arg_parser.add_argument('-f, --frame-rate',
                        dest='frame_rate',
                        default='1080p',
                        choices=['23', '24', '25', '29', '30', '50', '59', '60'],
                        help='Include burn-in timecode.')
arg_parser.add_argument('-p, --pixel-format',
                        dest='pixel_format',
                        default='yuv10',
                        choices=['yuv10', 'rgb8', 'rgb10', 'rgb12'],
                        help='Include burn-in timecode.')
arg_parser.add_argument('-t', '--test-pattern',
                        dest='test_pattern',
                        default='bars',
                        choices=['bars', 'custom'],
                        help='Test pattern.')
arg_parser.add_argument('--crosshairs',
                        dest='crosshairs',
                        default=False,
                        action='store_true',
                        help='Include uniform crosshairs.')
arg_parser.add_argument('--burn-timecode',
                        dest='burn_timecode',
                        default=False,
                        action='store_true',
                        help='Include burn-in timecode.')
arg_parser.add_argument('--list-decklinks',
                        dest='list_decklinks',
                        default=False,
                        action='store_true',
                        help='Print list of Decklinks available for use.')

args = arg_parser.parse_args()

BMDTesting.Init()
generator_devices = [device for device in BMDTesting.ContentGeneratorList() if 'DeckLink' in device]
analyser_devices = [device for device in BMDTesting.ContentAnalyserList() if 'DeckLink' in device]
# generator = BMDTesting.ContentGenerator(test_config_json['0']['Output']['Title'])

video_mode_list_4cc = BMDTesting.VideoModeFourCCs()
pixel_format_list_4cc = BMDTesting.PixelFormatFourCCs()


if args.list_decklinks:
  print('Outputs:')
  for dev in generator_devices:
    print('  - {}'.format(dev))
    generator_caps = json.loads(BMDTesting.ContentGenerator(dev).capabilities())['Generators']
    pp(generator_caps)
    # for generator_cap in generator_caps:
      # print('    - {}'.format(generator_cap))

  print()
  print('Inputs:')
  for dev in analyser_devices:
    print('  - {}'.format(dev))
    analyser_caps = json.loads(BMDTesting.ContentAnalyser(dev).capabilities())['Analytics']
    pp(analyser_caps)
    # for analyser_cap in analyser_caps:
      # print('    - {}'.format(analyser_cap))

  exit()

test_config_json = {}
test_config_json['0'] = {}

if args.output_port:
  generator = BMDTesting.ContentGenerator(generator_devices[0])

  test_config_json['0']['Output'] = {}
  test_config_json['0']['Output']['Audio Link'] = '{} Out'.format(args.output_port.upper())
  test_config_json['0']['Output']['Video Link'] = '{} Out'.format(args.output_port.upper())
  test_config_json['0']['Output']['HDMI LTC'] = 'Canon'
  test_config_json['0']['Output']['Link Count'] = '1'
  test_config_json['0']['Output']['SDI Level'] = 'B-DL' if args.sdi_level == 'b' else 'A'
  test_config_json['0']['Output']['SDI Keying'] = 'No'
  test_config_json['0']['Output']['Title'] = generator_devices[0]
  test_config_json['0']['Output']['Format'] = {}
  test_config_json['0']['Output']['Format']['Video'] = {}

  # if args.resolution == 'pal':
  # elif args.resolution == 'ntsc':
  # elif args.resolution == '720p':
  #   if args.frame_rate == '23':
  #   if args.frame_rate == '24':
  #   if args.frame_rate == '25':
  #   if args.frame_rate == '29':
  #   if args.frame_rate == '30':
  #   if args.frame_rate == '50':
  #   if args.frame_rate == '59':
  #   if args.frame_rate == '60':
  # elif args.resolution == '1080p':
  #   if args.frame_rate == '23':
  #   if args.frame_rate == '24':
  #   if args.frame_rate == '25':
  #   if args.frame_rate == '29':
  #   if args.frame_rate == '30':
  #   if args.frame_rate == '50':
  #   if args.frame_rate == '59':
  #   if args.frame_rate == '60':
  # elif args.resolution == '2kdci':
  #   if args.frame_rate == '23':
  #   if args.frame_rate == '24':
  #   if args.frame_rate == '25':
  #   if args.frame_rate == '29':
  #   if args.frame_rate == '30':
  #   if args.frame_rate == '50':
  #   if args.frame_rate == '59':
  #   if args.frame_rate == '60':
  # elif args.resolution == '2160p':
  #   if args.frame_rate == '23':
  #   if args.frame_rate == '24':
  #   if args.frame_rate == '25':
  #   if args.frame_rate == '29':
  #   if args.frame_rate == '30':
  #   if args.frame_rate == '50':
  #   if args.frame_rate == '59':
  #   if args.frame_rate == '60':
  # elif args.resolution == '4kdci':
  #   if args.frame_rate == '23':
  #   if args.frame_rate == '24':
  #   if args.frame_rate == '25':
  #   if args.frame_rate == '29':
  #   if args.frame_rate == '30':
  #   if args.frame_rate == '50':
  #   if args.frame_rate == '59':
  #   if args.frame_rate == '60':

  test_config_json['0']['Output']['Format']['Video']['Video Mode'] = '4k30'

  if args.pixel_format == 'yuv10':
    test_config_json['0']['Output']['Format']['Video']['Pixel Format'] = 'v210'
  elif args.pixel_format == 'rgb8':
    test_config_json['0']['Output']['Format']['Video']['Pixel Format'] = 'RGBA'
  elif args.pixel_format == 'rgb10':
    test_config_json['0']['Output']['Format']['Video']['Pixel Format'] = 'r210'
  elif args.pixel_format == 'rgb12':
    test_config_json['0']['Output']['Format']['Video']['Pixel Format'] = 'r12l'

  test_config_json['0']['Output']['Generators'] = []
  test_config_json['0']['Output']['Generators'].append({})

  if args.test_pattern == 'bars':
    test_config_json['0']['Output']['Generators'][-1]['Title'] = 'Video: Bars'
  elif args.test_pattern == 'custom':
    test_config_json['0']['Output']['Generators'][-1]['Title'] = 'Video: Raw File'
    test_config_json['0']['Output']['Generators'][-1]['URI'] = os.path.abspath(os.path.expanduser('frame.bin'))
    test_config_json['0']['Output']['Generators'][-1]['Packing'] = 0
    test_config_json['0']['Output']['Generators'][-1]['Depth'] = 0
    #####################
    subprocess.call(['build/frame_gen', '3840', '2160', 'R210', '1', '2048', '0', '2048', '--', 'frame.bin'])

  if args.crosshairs == True:
    test_config_json['0']['Output']['Generators'].append({})
    test_config_json['0']['Output']['Generators'][-1]['Title'] = 'Video: Crosshairs - Uniform'

  if args.burn_timecode == True:
    test_config_json['0']['Output']['Generators'].append({})
    test_config_json['0']['Output']['Generators'][-1]['Title'] = 'Video: Burn Timecode'

if args.input_port:
  analyser = BMDTesting.ContentAnalyser(analyser_devices[0])

  test_config_json['0']['Input'] = {}


pp(test_config_json)
exit()





test_cfg_json_str = json.dumps(test_config_json, indent=4)

if BMDTesting.ValidateConfig(test_cfg_json_str) == False:
  print("ERROR: invalid BMDTesting config")
  sys.exit(1)
if generator.validateConfig(test_cfg_json_str, "0.Output") == False:
  print("ERROR: invalid generator config")
  sys.exit(1)

if generator.sourceOpen(test_cfg_json_str, "0.Output"):
  if generator.sourceStart():
    try: input = raw_input # Use input() in Python3 or raw_input() in Python2
    except NameError: pass
    input("Press Enter to Stop...\n")
    generator.sourceClose()