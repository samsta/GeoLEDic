import math
import sys

fbase = sys.argv[1]+"/Dome"

leds_per_meter = 30

row0 = [
    # FULL DOME
    [ 0.974, 5.683, 1.658 ],
    [ 1.854, 5.706, 0.000 ],
    [ 2.728, 5.080, 1.658 ],
    [ 3.527, 4.854, 0.000 ],
    [ 3.988, 4.164, 1.658 ],
    [ 4.854, 3.526, 0.000 ],
    # LOW BROW DOME
    [ 5.103,  2.683, 1.658],
    [ 5.706,  1.854, 0.000],
    [ 5.706,  0.829, 1.658],
    [ 6.000,  0.000, 0.000],
    [ 5.674, -1.025, 1.658],
    [ 5.706, -1.854, 0.000],
    [ 5.193, -2.506, 1.658],
    [ 4.854, -3.526, 0.000],
    [ 4.129, -4.025, 1.658],
    [ 3.526, -4.854, 0.000],
    [ 2.552, -5.170, 1.658],
    [ 1.854, -5.706, 0.000],
    [ 0.779, -5.713, 1.658],
    [ 0.000, -6.000, 0.000],
    [-0.779, -5.713, 1.658],
    [-1.854, -5.706, 0.000],
    [-2.552, -5.170, 1.658],
    [-3.526, -4.854, 0.000],
    [-4.129, -4.025, 1.658],
    [-4.854, -3.526, 0.000],
    [-5.193, -2.506, 1.658],
    [-5.706, -1.854, 0.000],
    [-5.674, -1.025, 1.658],
    [-6.000,  0.000, 0.000],
    [-5.706,  0.829, 1.658],
    [-5.706,  1.854, 0.000],
    [-5.103,  2.683, 1.658],
    # FULL DOME
    [ -4.854, 3.527, 0.000 ],
    [ -3.988, 4.164, 1.658 ],
    [ -3.526, 4.854, 0.000 ],
    [ -2.728, 5.080, 1.658 ],
    [ -1.854, 5.706, 0.000 ],
    [ -0.975, 5.683, 1.658 ]
]

row1 = [
    # FULL DOME
    [ -0.000, 5.104, 3.154 ],
    [  0.974, 5.683, 1.658 ],
    [  1.950, 4.827, 2.983 ],
    [  2.728, 5.080, 1.658 ],
    [  3.154, 4.341, 2.683 ],
    [  3.988, 4.164, 1.658 ],
    # LOW BROW DOME
    [ 3.988,  3.345, 2.983],
    [ 5.103,  2.683, 1.658],
    [ 4.854,  1.577, 3.154],
    [ 5.706,  0.829, 1.658],
    [ 5.193, -0.362, 2.983],
    [ 5.674, -1.025, 1.658],
    [ 5.103, -1.658, 2.683],
    [ 5.193, -2.506, 1.658],
    [ 4.414, -2.759, 2.983],
    [ 4.129, -4.025, 1.658],
    [ 3.000, -4.129, 3.154],
    [ 2.552, -5.170, 1.658],
    [ 1.260, -5.051, 2.983],
    [ 0.779, -5.713, 1.658],
    [ 0.000, -5.366, 2.683],
    [-0.779, -5.713, 1.658],
    [-1.260, -5.051, 2.983],
    [-2.552, -5.170, 1.658],
    [-3.000, -4.129, 3.154],
    [-4.129, -4.025, 1.658],
    [-4.414, -2.759, 2.983],
    [-5.193, -2.506, 1.658],
    [-5.103, -1.658, 2.683],
    [-5.674, -1.025, 1.658],
    [-5.193, -0.362, 2.983],
    [-5.706,  0.829, 1.658],
    [-4.854,  1.577, 3.154],
    [-5.103,  2.683, 1.658],
    [-3.988,  3.345, 2.983],
    # FULL DOME
    [ -3.988, 4.164, 1.658 ],
    [ -3.154, 4.341, 2.683 ],
    [ -2.728, 5.080, 1.658 ],
    [ -1.949, 4.827, 2.983 ],
    [ -0.975, 5.683, 1.658 ],
    [ -0.000, 5.104, 3.154 ]]

row20 = [
    [ 3.154,  4.341, 2.683], # < FULL DOME
    [ 2.728,  3.755, 3.802],
    [ 3.988,  3.342, 2.983],
    [ 3.526,  2.171, 4.341],
    [ 4.854,  1.577, 3.154],
    [ 4.129,  0.317, 4.341],
    [ 5.193, -0.362, 2.983],
    [ 4.414, -1.434, 3.802],
    [ 5.103, -1.658, 2.683]
]

row21 = [
    [ 5.103, -1.658, 2.683],
    [ 4.414, -1.434, 3.802],
    [ 4.414, -2.759, 2.983],
    [ 3.154, -2.683, 4.341],
    [ 3.000, -4.129, 3.154],
    [ 1.577, -3.829, 4.341],
    [ 1.260, -5.051, 2.983],
    [ 0.000, -4.641, 3.802],
    [ 0.000, -5.366, 2.683]
]

row22 = [
    [-0.000, -5.366, 2.683],
    [-0.000, -4.641, 3.802],
    [-1.260, -5.051, 2.983],
    [-1.577, -3.829, 4.341],
    [-3.000, -4.129, 3.154],
    [-3.154, -2.683, 4.341],
    [-4.414, -2.759, 2.983],
    [-4.414, -1.434, 3.802],
    [-5.103, -1.658, 2.683]
]

row23 = [
    [-5.103, -1.658, 2.683],
    [-4.414, -1.434, 3.802],
    [-5.193, -0.362, 2.983],
    [-4.129,  0.317, 4.341],
    [-4.854,  1.577, 3.154],
    [-3.526,  2.171, 4.341],
    [-3.988,  3.342, 2.983],
    [-2.728,  3.755, 3.802],
    # FULL DOME
    [-3.154,  4.341, 2.683]
]

row24 = [
    [ -3.154, 4.341, 2.683 ],
    [ -2.728, 3.755, 3.802 ],
    [ -1.949, 4.827, 2.983 ],
    [ -0.975, 4.024, 4.341 ],
    [ -0.000, 5.104, 3.154 ],
    [ 0.975, 4.025, 4.341 ],
    [ 1.950, 4.827, 2.983 ],
    [ 2.728, 3.755, 3.802 ],
    [ 3.154, 4.341, 2.683 ],
]

row30 = [
    [ 0.975,  4.025, 4.341],
    [ 2.728,  3.755, 3.802],
    [ 1.854,  2.552, 5.103],
    [ 3.526,  2.171, 4.341],
    [ 2.552,  0.829, 5.366],
    [ 4.129,  0.317, 4.341],
    [ 3.000, -0.975, 5.103],
    [ 4.414, -1.434, 3.802]
]

row31 = [
    [ 4.414, -1.434, 3.802],
    [ 3.000, -0.975, 5.103],
    [ 3.154, -2.683, 4.341],
    [ 1.577, -2.171, 5.366],
    [ 1.577, -3.829, 4.341],
    [ 0.000, -3.154, 5.103],
    [ 0.000, -4.641, 3.802]
]

row32 = [
    [-0.000, -4.641, 3.802],
    [-0.000, -3.154, 5.103],
    [-1.577, -3.829, 4.341],
    [-1.577, -2.171, 5.366],
    [-3.154, -2.683, 4.341],
    [-3.000, -0.975, 5.103],
    [-4.414, -1.434, 3.802]
]

row33 = [
    [-4.414, -1.434, 3.802],
    [-3.000, -0.975, 5.103],
    [-4.129,  0.317, 4.341],
    [-2.552,  0.829, 5.366],
    [-3.526,  2.171, 4.341],
    [-1.854,  2.552, 5.103],
    [-2.728,  3.755, 3.802],
    [-0.975,  4.025, 4.341]

]

row34 = [
    [-1.854,  2.552, 5.103],
    [-0.975,  4.025, 4.341],
    [ 0.000,  2.683, 5.366],
    [ 0.975,  4.025, 4.341],
    [ 1.854,  2.552, 5.103]
]

row40 = [
    [ 0.000,  2.683, 5.366],
    [ 1.854,  2.552, 5.103],
    [ 0.779,  1.072, 5.851],
    [ 2.552,  0.829, 5.366],
    [ 1.260, -0.409, 5.851],
    [ 3.000, -0.975, 5.103]
]

row41 = [
    [ 3.000, -0.975, 5.103],
    [ 1.260, -0.409, 5.851],
    [ 1.577, -2.171, 5.366],
    [ 0.000, -1.325, 5.851],
    [ 0.000, -3.154, 5.103]
]

row42 = [
    [ 0.000, -3.154, 5.103],
    [ 0.000, -1.325, 5.851],
    [-1.577, -2.171, 5.366],
    [-1.260, -0.409, 5.851],
    [-3.000, -0.975, 5.103]
]

row43 = [
    [-3.000, -0.975, 5.103],
    [-1.260, -0.409, 5.851],
    [-2.552,  0.829, 5.366],
    [-0.779,  1.072, 5.851],
    [-1.854,  2.552, 5.103]
]

row44 = [
    [-1.854,  2.552, 5.103],
    [-0.779,  1.072, 5.851],
    [ 0.000,  2.683, 5.366],
    [ 0.779,  1.072, 5.851]
]

row50 = [
    [-0.779,  1.072, 5.851],
    [ 0.779,  1.072, 5.851],
    [ 0.000,  0.000, 6.000],
    [ 1.260, -0.409, 5.851]
]

row51 = [
    [ 1.260, -0.409, 5.851],
    [ 0.000, -1.325, 5.851],
    [ 0.000,  0.000, 6.000],
    [-1.260, -0.409, 5.851],
    [-0.779,  1.072, 5.851]
]

# map exterior panel edge lengths of the (inaccurate) SketchUp model to the
# actual strut lengths
strut_map = {
   1.519: 1.155, # A  20
   1.771: 1.380, # B  22
   1.767: 1.677, # C  48
   1.877: 1.700, # D  54
   1.950: 1.772, # E  27
   1.792: 1.799, # F? 24
}

strips = [
    [   0,   1,   2,   3,   4 ],
    [   5,   6,   7,   8,   9 ],
    [  10,  11,  12,  13,  14 ],
    [  15,  16,  17,  18,  19 ],
    [  20,  21,  22,  23,  24 ],
    [  25,  26,  27,  28,  29 ],
    [  30,  31,  32,  33,  34 ],
    [  35,  36,  37,  38,  39 ],
    [  40,  41,  42,  43,  44 ],
    [  45,  46,  47,  48,  49 ],
    [  50,  51,  52,  53,  54 ],
    [  55,  56,  57,  58,  59 ],
    [  60,  61,  62,  63,  64 ],
    [  65,  66,  67,  68,  69 ],
    [  70,  71,  72,  73,  74 ],
    [  75,  76,  77,  78,  79 ],
    [  80,  81,  82,  83,  84 ],
    [  85,  86,  87,  88,  89 ],
    [  90,  91,  92,  93,  94 ],
    [  95,  96,  97,  98,  99 ],
    [ 100, 101, 102, 103, 104 ],
    [ 105, 106, 107, 108, 109 ],
    [ 110, 111, 112, 113, 114 ],
    [ 115, 116, 117, 118, 119 ],
    [ 120, 121, 122, 123, 124 ],
    [ 125, 126, 127, 128, 129 ],
    [ 130, 131, 132, 133, 134 ],
    [ 135, 136, 137, 138, 139 ],
    [ 140, 141, 142, 143, 144 ],
    [ 145, 146, 147, 148, 149 ],
    [ 150, 151, 152, 153, 154 ],
    [ 155, 156, 157, 158, 159 ],
]

led_counts = [0] * 32


def distance(a, b):
   global strut_map
   e = [b[0] - a[0], b[1] - a[1], b[2] - a[2]]
   panel_edge_length =  math.sqrt(e[0]**2 + e[1]**2 + e[2]**2)
   min_diff = 10000;
   strut_length = 0;
   for outer, strut in strut_map.items():
      d = abs(outer - panel_edge_length)
      if d < min_diff:
         min_diff = d
         strut_length = strut
   if (strut_length == 0):
      raise Exception("Couldn't find strut length for outer panel edge %.3f" % panel_edge_length)
   return strut_length

total_length = 0
triangle_ix = 0

def find_strip(triangle_ix):
   global strips
   for k in xrange(len(strips)):
      if triangle_ix in strips[k]:
         return k
   raise Exception("Couldn't find strip for triangle %d" % triangle_ix)
         

def rotate180(triangle):
   for c in triangle:
      c[0] = -c[0]
      c[2] = -c[2]

def angle(coords):
   if (coords[0] == 0 and coords[2] == 0):
      # at the apex, artificially high number to make sure it isn't the starting corner
      return 100
   return round(2*math.atan2(coords[0], -coords[2]),3)/2

def print_row(row):
    global total_length
    global triangle_ix
    global led_counts

    for k in xrange(len(row)-2):
        s = []
        
        triangle = []
        for m in xrange(3):
            # Note: Sketchup order of coordinates is x/z/y, so we'll reorder here
            #       Also, the x coordinate seems mirrored
            triangle.append([-row[k+m][0], row[k+m][2], row[k+m][1]])

        # rotate by 180 deg if necessary to steer clear of the singularity at +-pi
        triangle_rotated = abs(angle(triangle[0])) > math.pi*3/4
        if triangle_rotated:
            rotate180(triangle)

        # make triangles turn clock-wise:
        # first corner is left-most (lowest horizontal angle, lowest y if angle equal), second corner is top-most (highest y)
        triangle.sort(key=lambda c: (angle(c), c[1]))

        # second corner is the one higher up of the two remaining
        if triangle[1][1] < triangle[2][1]:
            tmp = triangle[2]
            triangle[2] = triangle[1]
            triangle[1] = tmp

        if triangle_rotated:
            rotate180(triangle)

        for m in xrange(3):
            s.append("{{ {:6.3f}, {:6.3f}, {:6.3f} }}".format(triangle[m][0], triangle[m][1], triangle[m][2]))

        edge_lengths = [distance(triangle[0], triangle[1]),
                        distance(triangle[1], triangle[2]),
                        distance(triangle[2], triangle[0])]
        triangle_length = sum(edge_lengths)
        total_length += triangle_length
        num_leds = int(math.ceil(triangle_length*leds_per_meter)) + 3 # adding 3 LEDs for the corners seems to be roughly right
        comment = "// {:.3f}, {:.3f}, {:.3f}: {:.4f}m (~{:d} LEDs)".format(edge_lengths[0], edge_lengths[1], edge_lengths[2], triangle_length, num_leds)
        vertices = ', '.join(s)
        strip_ix = find_strip(triangle_ix)
        edges_def = []
        leds_remaining = num_leds
        for e in xrange(3):
           start = led_counts[strip_ix]
           if e < 2:
              leds_on_edge = math.ceil(edge_lengths[e] * leds_per_meter)
              leds_remaining -= leds_on_edge
           else:
              leds_on_edge = leds_remaining
           end   = start + leds_on_edge - 1
           led_counts[strip_ix] += leds_on_edge
           edges_def.append("           {%4d, %4d}" % (start, end))
        triangle_def = "strip(%2d), {%s}" % (strip_ix, ', '.join(edges_def))
        print "/* %3d */ { %s,   {%s}}, %s" % (triangle_ix, triangle_def, vertices, comment)
        triangle_ix += 1
        
def printHeader():
    print("/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */")

with open(fbase + ".cpp", 'w') as f:
   original_stdout = sys.stdout
   sys.stdout = f
   print "#include \"Dome.hpp\""
   print ""
   print "CRGB leds[NUM_STRIPS * LEDS_PER_STRIP];"
   print ""
   print "namespace {"
   print "CRGB* strip(int ix){ return &leds[ix*LEDS_PER_STRIP]; }"
   print "}"
   print ""
   print "Triangle dome[DOME_NUM_TRIANGLES] = {"
   print "// row 0"
   printHeader()
   print_row(row0)
   print "// row 1"
   printHeader()
   print_row(row1)
   print "// row 2"
   printHeader()
   print_row(row20)
   print_row(row21)
   print_row(row22)
   print_row(row23)
   print_row(row24)
   print "// row 3"
   printHeader()
   print_row(row30)
   print_row(row31)
   print_row(row32)
   print_row(row33)
   print_row(row34)
   print "// row 4"
   printHeader()
   print_row(row40)
   print_row(row41)
   print_row(row42)
   print_row(row43)
   print_row(row44)
   print "// row 5"
   printHeader()
   print_row(row50)
   print_row(row51)
   print "};"
   sys.stdout = original_stdout

with open(fbase + "Defs.hpp", 'w') as f:
   original_stdout = sys.stdout
   sys.stdout = f
   print \
   "#ifndef GEOLEDIC_DOMEDEFS_HPP\n" \
   "#define GEOLEDIC_DOMEDEFS_HPP\n" \
   "\n" \
   "#define DOME_NUM_TRIANGLES 156\n" \
   "#define NUM_STRIPS %d\n" \
   "#define LEDS_PER_STRIP %d\n" \
   "#define FRAME_RATE 24\n" \
   "#endif\n" % (len(led_counts), max(led_counts))
   sys.stdout = original_stdout
