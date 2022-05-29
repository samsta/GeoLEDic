from string import Template

def fader_cc(num):
    assert num < 32, "can only do 32 faders, currently"
    if num < 16:
        return 16 + num
    else:
        return 48 + num

def button_cc(num):
    # gotta skip 32 as it's used for 'bank select', and
    # Ableton Live doesn't record it
    assert num < 15, "can only do 15 buttons, currently"
    return 33 + num

def enum_cc(num):
    assert num < 16, "can only do 16 enums, currently"
    return 48 + num


def colorFader(color_param_name, fader_num, color_num, default_value=0, slider_style='normal'):
    return {
        'name': '%s %d' % (color_param_name, color_num),
        'number': fader_cc(fader_num),
        'description': '%s of color %d' % (color_param_name, color_num),
        'default': default_value,
        'slider_style': slider_style
    }

keyzones = {
   'ShapesFromNotes': 
   [
      [ # channel 1
        {
            'name': 'Triangles',
            'from': 0,
            'to': 122,
            'color': 'Orange'
        }
      ],
      [ # channel 2
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
        'base': 'DecayingShapesFromNotes',
        'keyzones': keyzones['ShapesFromNotes'],
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
            },
            {
                'name': 'Keys Decay',
                'number': fader_cc(4),
                'default': 3,
                'description': 'Rate at which keyboard triggered shapes decay'
            },
            {
                'name': 'Keys Only',
                'number': button_cc(0),
                'type': 'toggle',
                'description': 'If set, only the triangles/shapes corresponding to the pressed keys are turned on. If unset, all triangles are turned on, but the ones corresponding to the pressed keys turn off.'
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
             'description': 'Hue of the first color. Only used if `Use Hues` is set',
             'slider_style': 'hue'
          },
          {
             'name': 'Hue 1',
             'number': fader_cc(4),
             'description': 'Hue of the second color. Only used if `Use Hues` is set',
             'slider_style': 'hue'
          },
          {
             'name': 'Hue 2',
             'number': fader_cc(5),
             'description': 'Hue of the third color. Only used if `Use Hues` is set',
             'slider_style': 'hue'
          },
          {
             'name': 'Hue 3',
             'number': fader_cc(6),
             'description': 'Hue of the fourth color. Only used if `Use Hues` is set',
             'slider_style': 'hue'
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
       'program': 'Strobe',
       'base': 'Notes',
       'keyzones': [
          [
            {
                'name': 'Entire Dome',
                'from': 24,
                'to': 24,
                'color': 'Orange'
            },
            {
                'name': 'Meridians',
                'from': 25,
                'to': 29,
                'color': 'Blue'
            },
            {
                'name': 'Parallels',
                'from': 30,
                'to': 40,
                'color': 'Red'
            }

          ]
        ],
       'controls': [    
          {
             'name': 'Hue',
             'number': fader_cc(0),
             'default': 0,
             'description': 'Strobe Hue',
             'slider_style': 'hue'
          },
          {
             'name': 'Saturation',
             'number': fader_cc(1),
             'default': 0,
             'description': 'Strobe Saturation'
          },
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
             'description': 'Center hue value used for the randomised colors',
             'slider_style': 'hue'
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
             'description': 'Center hue value used for the randomised colors for keyboard-triggered shapes',
             'slider_style': 'hue'
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
             'description': 'Center hue value used for the randomised colors',
             'slider_style': 'hue'
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
    },
    {
      'program': 'FlexibleNoise',
      'name': 'Flexible Noise',
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

         colorFader("Hue",  8, 0,   0, "hue"),
         colorFader("Hue",  9, 1,   0, "hue"),
         colorFader("Hue", 10, 2,   0, "hue"),
         colorFader("Hue", 11, 3, 100, "hue"),
         colorFader("Hue", 12, 4,   0, "hue"),
         colorFader("Hue", 13, 5,   0, "hue"),
         colorFader("Hue", 14, 6,   0, "hue"),
         colorFader("Hue", 15, 7,   0, "hue"),

         colorFader("Brightness", 24, 0),
         colorFader("Brightness", 25, 1),
         colorFader("Brightness", 26, 2),
         colorFader("Brightness", 27, 3, 127),
         colorFader("Brightness", 28, 4),
         colorFader("Brightness", 29, 5),
         colorFader("Brightness", 30, 6),
         colorFader("Brightness", 31, 7),

         colorFader("Saturation", 16, 0, 127),
         colorFader("Saturation", 17, 1, 127),
         colorFader("Saturation", 18, 2, 127),
         colorFader("Saturation", 19, 3, 127),
         colorFader("Saturation", 20, 4, 127),
         colorFader("Saturation", 21, 5, 127),
         colorFader("Saturation", 22, 6, 127),
         colorFader("Saturation", 23, 7, 127),

         {
             'name': 'Black Stripes',
             'number': button_cc(0),
             'type': 'toggle',
             'description': 'If set, the 8 colours are separated by black stripes'
         },
         {
             'name': 'X/Y Swapped',
             'number': button_cc(1),
             'type': 'toggle',
             'description': 'X and Y axis of the noise field mapping are swapped. Hard to explain, just try it! Mandala-like patterns can be achieved with this'
         }
      ]
    },
    {
       'program': 'WarpDrive',
       'name': 'Warp Drive',
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
             'description': 'Center hue value used for the randomised colors',
             'slider_style': 'hue'
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
             'name': 'Center Theta',
             'number': fader_cc(7),
             'default': 38,
             'description': 'Warp center angle from ground upwards. Ground is 0, up top is 127'
          },
          {
             'name': 'Center Phi',
             'number': fader_cc(8),
             'default': 64,
             'description': 'Warp center angle, center is 64, right is > 64, left is < 64'
          },
          {
             'name': 'Radius',
             'number': fader_cc(9),
             'default': 127,
             'description': 'Radius of the warp circle'
          },
          {
              'name': 'Reverse',
              'description': 'Move backwards',
              'type': 'toggle',
              'number': button_cc(0)
          },
          {
              'name': 'Skip Concentric',
              'description': 'Skip concentric edges for a less flickering animation',
              'type': 'toggle',
              'default': True,
              'number': button_cc(1)
          },
          {
              'name': 'Smoothen Phi',
              'description': 'Smoothen changes in phi to make them less jerky. Turn this off if you want instantaneous phi changes.',
              'type': 'toggle',
              'default': True,
              'number': button_cc(2)
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
