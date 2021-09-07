keyzones = {
   'ShapesFromNotes': [
      {
          'name': 'Pentagon 0',
          'from': 12,
          'to': 17,
          'channel': 1,
          'transpose': 24
      },
      {
          'name': 'Pentagon 1',
          'from': 24,
          'to': 33,
          'channel': 1,
          'transpose': 24
      },
      {
          'name': 'Pentagon 2',
          'from': 36,
          'to': 42,
          'channel': 1,
          'transpose': 24
      },
      {
          'name': 'Pentagon 3',
          'from': 48,
          'to': 57,
          'channel': 1,
          'transpose': 24
      },
      {
          'name': 'blobs',
          'from': 60,
          'to': 111,
          'channel': 2,
          'transpose': -24
      }
   ],
}

programs = [
    {
        'program':'DefaultRainbow',
        'name':'Default Rainbow',
        'base': 'Notes',
        'controls': [
            {
                'name': 'Speed',
                'number': 16,
                'default': 1
            },
            {
                'name': 'Stride',
                'number': 17,
                'default': 1,
                'description': 'Larger stride means faster cycling through the color wheel, i.e. more repetitions of the rainbow'
            },
            {
                'name': 'Sparkle Probability',
                'number': 19,
                'default': 50
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
                'number': 16,
                'description': 'Brightness of the red worm chasing around the triangles'
            },
            {
                'name': 'Green Worm',
                'number': 17,
                'description': 'Brightness of the green worm chasing around the triangles'
            },
            {
                'name': 'Blue Worm',
                'number': 18,
                'description': 'Brightness of the blue worm chasing around the triangles'
            },
            {
                'name': 'Sparkle Probability',
                'number': 19,
            }
        ]
    },
    {
        'program':'Fire',
        'base': 'Notes',
        'keyzones' : [
            {
                'name': 'Sparks',
                'from': 36,
                'to': 85,
                'channel': 0,
                'transpose': 0
            },
        ],
        'controls': [
            {
                'name': 'Cooling',
                'number': 16,
                'default': 50,
                'description': 'Rate at which the flame cools down. Higher values mean faster cooling, or decay'
            },
            {
                'name': 'Sparking',
                'number': 17,
                'default': 100,
                'description': 'Rate at which new flames are sparked. Higher values means more flames'
            },
            {
               'name': 'Rotation Speed',
               'number': 18,
               'description': 'Speed at which the entire program turns'
            },
            {
                'name': 'Downwards',
                'number': 19,
                'type': 'toggle',
                'description': 'If set, flames start at the top instead of the bottom'
            },
            {
                'name': 'Reverse Palette',
                'number': 20,
                'type': 'toggle',
                'description': 'If set, the color palette is reversed'
            },
            {
                'name': 'Palette',
                'short_name': 'Pal',
                'type': 'enum',
                'number': 21,
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
                'number': 16,
                'default': 2
            },
            {
                'name': 'Stride Vertical',
                'number': 17,
                'default': 10,
                'description': 'Larger stride means faster cycling through the color wheel, i.e. more vertical repetitions of the rainbow'
            },
            {
                'name': 'Speed Horizontal',
                'number': 18,
            },
            {
                'name': 'Stride Horizontal',
                'number': 19,
                'description': 'Larger stride means faster cycling through the color wheel, i.e. more horizontal repetitions of the rainbow'
            }
        ]
    },
    {
        'program':'Lightning',
        'base': 'Notes',
        'keyzones' : [
            {
                'name': 'Lightning Bolts',
                'from': 48,
                'to': 57,
                'channel': 0,
                'transpose': 0
            },
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
      'base': 'ShapesFromNotes',
      'keyzones': keyzones['ShapesFromNotes'],
      'controls': [
         {
            'name': 'Speed X',
            'number': 16,
            'description': 'Speed of the movement along the X axis of the noise field'
         },
         {
            'name': 'Scale X',
            'number': 17,
            'default': 50,
            'description': 'Size of the steps along the X axis of the noise field'
         },
         {
            'name': 'Speed Y',
            'number': 18,
            'description': 'Speed of the movement along the Y axis of the noise field'
         },
         {
            'name': 'Scale Y',
            'number': 19,
            'default': 50,
            'description': 'Size of the steps along the Y axis of the noise field'
         },
         {
            'name': 'Speed T',
            'number': 20,
            'default': 3,
            'description': 'Speed of the movement along the time axis of the noise field'
         },
         {
            'name': 'Rotation Speed',
            'number': 21,
            'description': 'Speed at which the entire program rotates'
         },
         {
             'name': 'Key Activated',
             'number': 22,
             'type': 'toggle',
             'description': 'If set, the program is keyboard controlled in that only the triangles corresponding to the pressed keys light up'
         },
         {
             'name': 'X/Y Swapped',
             'number': 23,
             'type': 'toggle',
             'description': 'X and Y axis of the noise field mapping are swapped. Hard to explain, just try it! Mandala-like patterns can be achieved with this'
         },
         {
             'name': 'No Interpolation',
             'number': 24,
             'type': 'toggle',
             'description': 'Turn off the interpolation, or smoothing. Gives a more `patchy` appearance'
         },
         {
             'name': 'Palette',
             'short_name': 'Pal',
             'type': 'enum',
             'number': 25,
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
         }
      ]
    },
    {
       'program': 'OrganicLine',
       'name': 'Organic Line',
       'base': 'Notes',
       'controls': [
          {
             'name': 'Speed',
             'number': 16,
             'default': 8,
             'description': 'Speed at which the blobs move around'
          },
          {
             'name': 'Size',
             'number': 17,
             'default': 30,
             'description': 'Size of the blobs'
          },
          {
             'name': 'Use Hues',
             'number': 18,
             'type': 'toggle',
             'description': 'Instead of the preset palette, use black stripes alternating with stripes of a hue as defined by the hue controls'
          },
          {
             'name': 'Hue 0',
             'number': 19,
             'description': 'Hue of the first color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Hue 1',
             'number': 20,
             'description': 'Hue of the second color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Hue 2',
             'number': 21,
             'description': 'Hue of the third color. Only used if `Use Hues` is set'
          },
          {
             'name': 'Hue 3',
             'number': 22,
             'description': 'Hue of the fourth color. Only used if `Use Hues` is set'
          }
       ]
    },
    {
       'program': 'Edgy',
       'base': 'Notes',
       'controls': [
          {
             'name': 'Edge Rate',
             'description': 'Rate at which edges appear',
             'number': 16,
             'default': 3
          },
          {
             'name': 'Decay',
             'description': 'How fast edges fade to black',
             'number': 17,
             'default': 50
          },
          {
             'name': 'Min Hue',
             'number': 18,
             'default': 0,
             'description': 'Minimal hue value used for the randomised colors'
          },
          {
             'name': 'Max Hue',
             'number': 19,
             'default': 127,
             'description': 'Maximal hue value used for the randomised colors. There\'s no point in setting this below `Min Hue`'
          },
          {
             'name': 'Min Saturation',
             'number': 20,
             'default': 0,
             'description': 'Minimal saturation used for the randomised colors'
          },
          {
             'name': 'Max Saturation',
             'number': 21,
             'default': 127,
             'description': 'Maximal saturation used for the randomised colors. There\'s no point in setting this below `Min Saturation`'
          }
       ]
    }
]



def get():    
    # fill in defaults and expand values where needed (e.g. enums)
    for i in range(0, len(programs)):

        if not 'name' in programs[i]:
            programs[i]['name'] = programs[i]['program']

        if not 'keyzones' in programs[i]:
            programs[i]['keyzones'] = []

        if not 'controls' in programs[i]:
            programs[i]['controls'] = []
        else:
            for cc in programs[i]['controls']:
                if not 'type' in cc:
                    cc['type'] = 'continuous'
                if cc['type'] == 'enum':
                    # spread enum values on range 0..127 to make it easier to use them with a knob
                    n = len(cc['enums'])
                    cc['values'] = range(0, n*int(127/n), int(127/n))
    return programs
