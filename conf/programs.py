from string import Template

def fader_cc(num):
    return 16 + num

def button_cc(num):
    # gotta skip 32 as it's used for 'bank select', and
    # Ableton Live doesn't record it
    return 33 + num

def enum_cc(num):
    return 48 + num

keyzones = {
   'ShapesFromNotes': 
   [
      [ # channel 0
        {
            'name': 'Pentagon 0',
            'from': 12,
            'to': 17,
            'color': 'Red'
        },
        {
            'name': 'Pentagon 1',
            'from': 24,
            'to': 33,
            'color': 'Red'
        },
        {
            'name': 'Pentagon 2',
            'from': 36,
            'to': 42,
            'color': 'Red'
        },
        {
            'name': 'Pentagon 3',
            'from': 48,
            'to': 57,
            'color': 'Red'
        },
        {
            'name': 'Blobs, Row 0',
            'from': 60,
            'to': 71,
            'color': 'Blue'
        },
        {
            'name': 'Blobs, Row 1',
            'from': 72,
            'to': 84,
            'color': 'Purple'
        },
        {
            'name': 'Blobs, Row 2',
            'from': 85,
            'to': 95,
            'color': 'Blue'
        },
        {
            'name': 'Blobs, Row 3',
            'from': 96,
            'to': 105,
            'color': 'Purple'
        },
        {
            'name': 'Blobs, Row 5',
            'from': 106,
            'to': 110,
            'color': 'Blue'
        },
        {
            'name': 'Blobs, Row 6',
            'from': 111,
            'to': 111,
            'color': 'Purple'
        }
      ],
      [ # channel 1
        {
            'name': 'Triangles',
            'from': 0,
            'to': 123,
            'color': 'Orange'
        }
      ]
   ],
}

programs = [
    {
        'program':'DefaultRainbow',
        'name':'Default Rainbow',
        'base': 'DecayingShapesFromNotes',
        'keyzones': keyzones['ShapesFromNotes'],
        'controls': [
            {
                'name': 'Speed',
                'number': fader_cc(0),
                'default': 4
            },
            {
                'name': 'Stride',
                'number': fader_cc(1),
                'default': 4,
                'max': 63,
                'description': 'Larger stride means faster cycling through the color wheel, i.e. more repetitions of the rainbow'
            },
            {
                'name': 'Sparkle Probability',
                'number': fader_cc(2),
                'default': 0
            },
            {
                'name': 'Keys Only',
                'number': button_cc(0),
                'type': 'toggle',
                'description': 'If set, only the triangles/shapes corresponding to the pressed keys are turned on. If unset, all triangles are turned on, but the ones corresponding to the pressed keys sparkle hard.'
            }
        ]
    },
    {
        'program':'SparklesAndTriangles',
        'name':'Sparkles and Triangles',
        'base': 'ShapesFromNotes',
        'keyzones' : keyzones['ShapesFromNotes'],
        'controls': [
            {
                'name': 'Red Worm',
                'number': fader_cc(0),
                'description': 'Brightness of the red worm chasing around the triangles'
            },
            {
                'name': 'Green Worm',
                'number': fader_cc(1),
                'description': 'Brightness of the green worm chasing around the triangles'
            },
            {
                'name': 'Blue Worm',
                'number': fader_cc(2),
                'description': 'Brightness of the blue worm chasing around the triangles'
            },
            {
                'name': 'Sparkle Probability',
                'number': fader_cc(3),
            }
        ]
    },
    {
        'program':'Fire',
        'base': 'Notes',
        'keyzones' : [
            [ # channel 0
                {
                    'name': 'Sparks',
                    'from': 36,
                    'to': 84,
                    'channel': 0,
                    'transpose': 0
                },
            ]
        ],
        'controls': [
            {
                'name': 'Cooling',
                'number': fader_cc(0),
                'default': 50,
                'description': 'Rate at which the flame cools down. Higher values mean faster cooling, or decay'
            },
            {
                'name': 'Sparking',
                'number': fader_cc(1),
                'default': 100,
                'description': 'Rate at which new flames are sparked. Higher values means more flames'
            },
            {
               'name': 'Rotation Speed',
               'number': fader_cc(2),
               'description': 'Speed at which the entire program turns',
               'max': 63
            },
            {
                'name': 'Reverse Palette',
                'number': button_cc(0),
                'type': 'toggle',
                'description': 'If set, the color palette is reversed'
            },
            {
                'name': 'Downwards',
                'number': button_cc(1),
                'type': 'toggle',
                'description': 'If set, flames start at the top instead of the bottom'
            },
            {
                'name': 'Palette',
                'short_name': 'Pal',
                'type': 'enum',
                'number': enum_cc(0),
                'enums': [
                    'Fire',
                    'Ocean',
                    'Cloud',
                    'Forest',
                    'Lava',
                    'Rainbow',
                    'Party'
                ]
            }
        ]

    },
    {
        'program':'MovingRainbow',
        'name':'Moving Rainbow',
        'base': 'Notes',
        'controls': [
            {
                'name': 'Speed Vertical',
                'number': fader_cc(0),
                'default': 2
            },
            {
                'name': 'Stride Vertical',
                'number': fader_cc(1),
                'default': 10,
                'description': 'Larger stride means faster cycling through the color wheel, i.e. more vertical repetitions of the rainbow'
            },
            {
                'name': 'Speed Horizontal',
                'number': fader_cc(2),
            },
            {
                'name': 'Stride Horizontal',
                'number': fader_cc(3),
                'description': 'Larger stride means faster cycling through the color wheel, i.e. more horizontal repetitions of the rainbow',
                'max': 63
            }
        ]
    },
    {
        'program':'Lightning',
        'base': 'Notes',
        'keyzones' : [
            [ # channel 0
                {
                    'name': 'Lightning Bolts',
                    'from': 48,
                    'to': 57,
                    'channel': 0,
                    'transpose': 0
                }
            ]
        ],
        'controls': [
            {
                'name': 'Lightning Probability',
                'number': 16,
                'default': 2,
            },
            {
                'name': 'Forking Probability',
                'number': 17,
                'default': 80,
                'description': 'If higher, lightning bolt is more likely to fork and get to the bottom of the dome'
            }
        ]
    },
    {
      'program':'Noise',
      'base': 'DecayingShapesFromNotes',
      'keyzones': keyzones['ShapesFromNotes'],
      'controls': [
         {
            'name': 'Speed X',
            'number': fader_cc(0),
            'description': 'Speed of the movement along the X axis of the noise field'
         },
         {
            'name': 'Scale X',
            'number': fader_cc(1),
            'default': 50,
            'description': 'Size of the steps along the X axis of the noise field'
         },
         {
            'name': 'Speed Y',
            'number': fader_cc(2),
            'description': 'Speed of the movement along the Y axis of the noise field'
         },
         {
            'name': 'Scale Y',
            'number': fader_cc(3),
            'default': 50,
            'description': 'Size of the steps along the Y axis of the noise field'
         },
         {
            'name': 'Speed T',
            'number': fader_cc(4),
            'default': 12,
            'description': 'Speed of the movement along the time axis of the noise field'
         },
         {
            'name': 'Rotation Speed',
            'number': fader_cc(5),
            'description': 'Speed at which the entire program rotates',
            'max': 63
         },
         {
             'name': 'Keys Decay',
             'number': fader_cc(6),
             'default': 3,
             'description': 'Rate at which keyboard triggered shapes decay'
         },
         {
             'name': 'Palette',
             'short_name': 'Pal',
             'description': 'Color palette used for the background',
             'type': 'enum',
             'number': enum_cc(0),
             'enums': [
                 'Party',
                 'PartyStripes',
                 'Rainbow',
                 'RainbowStripes',
                 'Ocean',
                 'OceanStripes',
                 'Fire',
                 'Cloud',
                 'Forest',
                 'Lava',
             ]
         },
         {
             'name': 'Keyboard Palette',
             'short_name': 'KPal',
             'description': 'Color palette used for the keyboard triggered shapes (unless \'Keys Invert Color\' is enabled)',
             'type': 'enum',
             'number': enum_cc(1),
             'default': 'Ocean',
             'enums': [
                 'Party',
                 'PartyStripes',
                 'Rainbow',
                 'RainbowStripes',
                 'Ocean',
                 'OceanStripes',
                 'Fire',
                 'Cloud',
                 'Forest',
                 'Lava',
             ]
         },
        {
             'name': 'Keys Invert Color',
             'number': button_cc(0),
             'type': 'toggle',
             'description': 'If set, the shapes lit using the keyboard invert the background palette color by rotating it 180 degrees around the color wheel. If not set, the shapes use the separate keyboard palette color'
         },
         {
             'name': 'X/Y Swapped',
             'number': button_cc(1),
             'type': 'toggle',
             'description': 'X and Y axis of the noise field mapping are swapped. Hard to explain, just try it! Mandala-like patterns can be achieved with this'
         },
         {
             'name': 'No Interpolation',
             'number': button_cc(2),
             'type': 'toggle',
             'description': 'Turn off the interpolation, or smoothing. Gives a more `patchy` appearance'
         },

      ]
    },
    {
       'program': 'OrganicLine',
       'name': 'Organic Line',
       'base': 'Notes',
       'controls': [
          {
             'name': 'Speed',
             'number': fader_cc(0),
             'default': 8,
             'description': 'Speed at which the blobs move around'
          },
          {
             'name': 'Size',
             'number': fader_cc(1),
             'default': 30,
             'description': 'Size of the blobs'
          },
          {
             'name': 'Line Width',
             'number': fader_cc(2),
             'default': 0,
             'description': 'Width of the line'
          },
          {
             'name': 'Hue 0',
             'number': fader_cc(3),
             'description': 'Hue of the first color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Hue 1',
             'number': fader_cc(4),
             'description': 'Hue of the second color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Hue 2',
             'number': fader_cc(5),
             'description': 'Hue of the third color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Hue 3',
             'number': fader_cc(6),
             'description': 'Hue of the fourth color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Use Hues',
             'number': button_cc(0),
             'type': 'toggle',
             'description': 'Instead of the preset palette, use black stripes alternating with stripes of a hue as defined by the hue controls'
          },
          {
             'name': 'Disable Hue1',
             'number': button_cc(1),
             'type': 'toggle',
             'description': 'Set color normally controlled by hue 1 to black'
          },
          {
             'name': 'Disable Hue3',
             'number': button_cc(2),
             'type': 'toggle',
             'description': 'Set color normally controlled by hue 3 to black'
          },


       ]
    },
    {
       'program': 'Edgy',
       'base': 'ShapesFromNotes',
       'keyzones': keyzones['ShapesFromNotes'],
       'controls': [
          {
             'name': 'Edge Rate',
             'description': 'Rate at which edges appear',
             'number': fader_cc(0),
             'default': 3,
             'max': 31
          },
          {
             'name': 'Keyboard Edge Rate',
             'description': 'Rate at which edges appear for keyboard-triggered shapes',
             'number': fader_cc(1),
             'default': 100
          },
          {
             'name': 'Decay',
             'description': 'How fast edges fade to black',
             'number': fader_cc(2),
             'default': 50
          },
          {
             'name': 'Min Hue',
             'number': fader_cc(3),
             'default': 0,
             'description': 'Minimal hue value used for the randomised colors'
          },
          {
             'name': 'Max Hue',
             'number': fader_cc(4),
             'default': 127,
             'description': 'Maximal hue value used for the randomised colors. There\'s no point in setting this below `Min Hue`'
          },
          {
             'name': 'Min Saturation',
             'number': fader_cc(5),
             'default': 0,
             'description': 'Minimal saturation used for the randomised colors'
          },
          {
             'name': 'Max Saturation',
             'number': fader_cc(6),
             'default': 127,
             'description': 'Maximal saturation used for the randomised colors. There\'s no point in setting this below `Min Saturation`'
          }
       ]
    },
    {
       'program': 'BetterEdgy',
       'name': 'Better Edgy',
       'base': 'ShapesFromNotes',
       'keyzones': keyzones['ShapesFromNotes'],
       'controls': [
          {
             'name': 'Edge Rate',
             'description': 'Rate at which edges appear',
             'number': fader_cc(0),
             'default': 3,
             'max': 31
          },
          {
             'name': 'Keyboard Edge Rate',
             'description': 'Rate at which edges appear for keyboard-triggered shapes',
             'number': fader_cc(1),
             'default': 100
          },
          {
             'name': 'Decay',
             'description': 'How fast edges fade to black',
             'number': fader_cc(2),
             'default': 50
          },
          {
             'name': 'Hue',
             'number': fader_cc(3),
             'default': 64,
             'description': 'Center hue value used for the randomised colors'
          },
          {
             'name': 'Hue Range',
             'number': fader_cc(4),
             'default': 127,
             'description': 'Amount by which the hue deviates randomly'
          },
          {
             'name': 'Saturation',
             'number': fader_cc(5),
             'default': 64,
             'description': 'Center saturation used for the randomised colors'
          },
          {
             'name': 'Saturation Range',
             'number': fader_cc(6),
             'default': 127,
             'description': 'Amount by which the saturation deviates randomly'
          },
          {
             'name': 'Keys Hue',
             'number': fader_cc(7),
             'default': 110,
             'description': 'Center hue value used for the randomised colors for keyboard-triggered shapes'
          },
          {
             'name': 'Keys Hue Range',
             'number': fader_cc(8),
             'default': 10,
             'description': 'Range within which the hue deviates randomly for keyboard-triggered shapes'
          },
          {
             'name': 'Keys Saturation',
             'number': fader_cc(9),
             'default': 100,
             'description': 'Center saturation used for the randomised colors for keyboard-triggered shapes'
          },
          {
             'name': 'Keys Saturation Range',
             'number': fader_cc(10),
             'default': 20,
             'description': 'Range within which the saturation deviates randomly for keyboard-triggered shapes'
          },
          {
             'name': 'Use Keys Color',
             'number': button_cc(0),
             'type': 'toggle',
             'default': True,
             'description': 'Use separate hue and saturation settings for keybpard-triggered shapes (below)'
          },

       ]
    },
    {
       'program': 'WarpDriveMandala',
       'name': 'Warp Drive Mandala',
       'base': 'Notes',
        'keyzones' : [
            [ # channel 0
                {
                    'name': 'Rings',
                    'from': 36,
                    'to': 59,
                    'channel': 0,
                    'transpose': 0
                }
            ]
        ],
       'controls': [
          {
             'name': 'Spawn Rate',
             'description': 'Rate at which rings appear',
             'number': fader_cc(0),
             'default': 30
          },
          {
              'name': 'Speed',
              'description': 'Speed at which rings move',
              'number': fader_cc(1),
              'default': 1,
              'max': 30
          },
          {
              'name': 'Thickness',
              'description': 'Thickness of rings',
              'number': fader_cc(2),
              'default': 0,
              'max': 30
          },
          {
             'name': 'Hue',
             'number': fader_cc(3),
             'default': 64,
             'description': 'Center hue value used for the randomised colors'
          },
          {
             'name': 'Hue Range',
             'number': fader_cc(4),
             'default': 127,
             'description': 'Amount by which the hue deviates randomly'
          },
          {
             'name': 'Saturation',
             'number': fader_cc(5),
             'default': 64,
             'description': 'Center saturation used for the randomised colors'
          },
          {
             'name': 'Saturation Range',
             'number': fader_cc(6),
             'default': 127,
             'description': 'Amount by which the saturation deviates randomly'
          },
          {
              'name': 'Upwards',
              'description': 'Move upwards instead of downwards',
              'type': 'toggle',
              'number': button_cc(0)
          },
          {
              'name': 'Skip Horizontals',
              'description': 'Skip horizontal edges for a less flickering animation',
              'type': 'toggle',
              'default': True,
              'number': button_cc(1)
          },
       ]
    }
]



def get():    
    # fill in defaults and expand values where needed (e.g. enums)
    for i in range(0, len(programs)):

        if not 'name' in programs[i]:
            programs[i]['name'] = programs[i]['program']

        if not 'keyzones' in programs[i]:
            programs[i]['keyzones'] = [[]]

        if not 'controls' in programs[i]:
            programs[i]['controls'] = []
        else:
            for cc in programs[i]['controls']:
                cc['min'] = max(0, cc['min'] if 'min' in cc else 0)
                cc['max'] = min(127, cc['max'] if 'max' in cc else 127)
                if not 'type' in cc:
                    cc['type'] = 'continuous'
                if cc['type'] == 'enum':
                    # spread enum values to make it easier to select them with a rotary knob
                    if 'step' not in cc:
                        cc['step'] = 10
                    n = len(cc['enums'])
                    if (n * cc['step'] > 127):
                        raise Exception(
                            Template("step size of $step too large for enum with $num values as $step x $num = $prod which is bigger than 127").substitute(
                                step=cc['step'],
                                num=n,
                                prod=n*cc['step']
                            )
                        )
                    cc['values'] = range(0, n*cc['step'], cc['step'])
    return programs
