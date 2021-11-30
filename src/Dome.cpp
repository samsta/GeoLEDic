#include "Dome.hpp"

CRGB leds[NUM_STRIPS * LEDS_PER_STRIP];

namespace {
CRGB* strip(int ix){ return &leds[ix*LEDS_PER_STRIP]; }
}

//#define SAMS_LEDS

Triangle dome[DOME_NUM_TRIANGLES] = {
// row 0
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
#ifdef SAMS_LEDS
/*   0 */ { strip( 0), {           { 218,  299},            {   0,  109},            { 110,  217}},   {{ -5.103,  1.658,  2.683 }, { -5.706,  1.658,  0.829 }, { -5.706,  0.000,  1.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   1 */ { strip( 0), {           { 491,  599},            { 300,  407},            { 408,  490}},   {{ -5.706,  0.000,  1.854 }, { -5.706,  1.658,  0.829 }, { -6.000,  0.000,  0.000 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*   2 */ { strip( 0), {           { 899,  815},            { 814,  708},            { 707,  600}},   {{ -5.706,  1.658,  0.829 }, { -5.674,  1.658, -1.025 }, { -6.000,  0.000,  0.000 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
#else
/*   0 */ { strip( 0), {           {   0,  106},            { 107,  213},            { 214,  321}},   {{ -5.103,  1.658,  2.683 }, { -5.706,  1.658,  0.829 }, { -5.706,  0.000,  1.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   1 */ { strip( 0), {           { 322,  428},            { 429,  530},            { 531,  635}},   {{ -5.706,  0.000,  1.854 }, { -5.706,  1.658,  0.829 }, { -6.000,  0.000,  0.000 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*   2 */ { strip( 0), {           { 636,  737},            { 738,  844},            { 845,  949}},   {{ -5.706,  1.658,  0.829 }, { -5.674,  1.658, -1.025 }, { -6.000,  0.000,  0.000 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
#endif
/*   3 */ { strip( 1), {           {  93,  201},            { 202,  303},  { 304,  314,  0,   92}},   {{ -6.000,  0.000,  0.000 }, { -5.674,  1.658, -1.025 }, { -5.706,  0.000, -1.854 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*   4 */ { strip( 1), {{ 843,  916, 629,  640},            { 641,  741},            { 742,  842}},   {{ -5.674,  1.658, -1.025 }, { -5.193,  1.658, -2.506 }, { -5.706,  0.000, -1.854 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*   5 */ { strip( 1), {           { 323,  424},            { 425,  533}, { 534,  628, 315,  322}},   {{ -5.706,  0.000, -1.854 }, { -5.193,  1.658, -2.506 }, { -4.854,  0.000, -3.526 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*   6 */ { strip( 2), {           { 841,  942}, { 943,  947, 633,  731},            { 732,  840}},   {{ -5.193,  1.658, -2.506 }, { -4.129,  1.658, -4.025 }, { -4.854,  0.000, -3.526 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*   7 */ { strip( 2), {           { 526,  628}, { 629,  632, 318,  421},            { 422,  525}},   {{ -4.854,  0.000, -3.526 }, { -4.129,  1.658, -4.025 }, { -3.526,  0.000, -4.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*   8 */ { strip( 2), {           {   3,  107},            { 108,  213},  { 214, 317,   0,    2}},   {{ -4.129,  1.658, -4.025 }, { -2.552,  1.658, -5.170 }, { -3.526,  0.000, -4.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   9 */ { strip( 3), {           {1033, 1140}, {1141, 1147, 832,  929},            { 930, 1032}},   {{ -3.526,  0.000, -4.854 }, { -2.552,  1.658, -5.170 }, { -1.854,  0.000, -5.706 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  10 */ { strip( 3), {           { 716,  817}, { 818,  831, 518,  612},            { 613,  715}},   {{ -2.552,  1.658, -5.170 }, { -0.779,  1.658, -5.713 }, { -1.854,  0.000, -5.706 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  11 */ { strip( 4), {           {1043, 1151},  {1152, 1158, 844, 939},            { 940, 1042}},   {{ -1.854,  0.000, -5.706 }, { -0.779,  1.658, -5.713 }, { -0.000,  0.000, -6.000 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  12 */ { strip( 4), {           { 561,  646},            { 647,  748},  { 749,  833, 544, 560}},   {{ -0.779,  1.658, -5.713 }, {  0.779,  1.658, -5.713 }, { -0.000,  0.000, -6.000 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*  13 */ { strip( 4), {           { 197,  299},   { 300,  314, 0,   93},            {  94,  196}},   {{ -0.000,  0.000, -6.000 }, {  0.779,  1.658, -5.713 }, {  1.854,  0.000, -5.706 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  14 */ { strip( 5), {           { 938,  836},            { 835,  733}, { 732,  643, 957,  939}},   {{  0.779,  1.658, -5.713 }, {  2.552,  1.658, -5.170 }, {  1.854,  0.000, -5.706 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  15 */ { strip( 5), {           { 522,  626}, { 627,  642, 327,  418},            { 419,  521}},   {{  1.854,  0.000, -5.706 }, {  2.552,  1.658, -5.170 }, {  3.526,  0.000, -4.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  16 */ { strip( 5), {           {  20,  127},            { 128,  236},  { 237,  324,  0,   19}},   {{  2.552,  1.658, -5.170 }, {  4.129,  1.658, -4.025 }, {  3.526,  0.000, -4.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  17 */ { strip( 6), {           { 193,  300},   { 301,  316, 1,   88},            {  89,  192}},   {{  3.526,  0.000, -4.854 }, {  4.129,  1.658, -4.025 }, {  4.854,  0.000, -3.526 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  18 */ { strip( 6), {           { 515,  617}, { 618,  631, 317,  411},            { 412,  514}},   {{  4.129,  1.658, -4.025 }, {  5.193,  1.658, -2.506 }, {  4.854,  0.000, -3.526 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  19 */ { strip( 7), {           { 194,  302},  { 303,  316,  0,   88},            {  89,  193}},   {{  4.854,  0.000, -3.526 }, {  5.193,  1.658, -2.506 }, {  5.706,  0.000, -1.854 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  20 */ { strip( 7), {           { 331,  415},            { 416,  516}, { 517,  604, 317,  330}},   {{  5.193,  1.658, -2.506 }, {  5.674,  1.658, -1.025 }, {  5.706,  0.000, -1.854 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*  21 */ { strip( 7), {           {1036, 1137}, {1138, 1146, 832,  932},            { 933, 1035}},   {{  5.706,  0.000, -1.854 }, {  5.674,  1.658, -1.025 }, {  6.000,  0.000,  0.000 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  22 */ { strip( 8), {           { 435,  537}, { 538,  543, 230,  326},            { 327,  434}},   {{  5.674,  1.658, -1.025 }, {  5.706,  1.658,  0.829 }, {  6.000,  0.000,  0.000 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  23 */ { strip( 8), {           { 753,  855}, { 856,  863, 548,  648},            { 649,  752}},   {{  6.000,  0.000,  0.000 }, {  5.706,  1.658,  0.829 }, {  5.706,  0.000,  1.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  24 */ { strip( 8), {           { 878,  985},            { 986, 1092}, {1093, 1190, 868,  877}},   {{  5.706,  1.658,  0.829 }, {  5.103,  1.658,  2.683 }, {  5.706,  0.000,  1.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
// row 1
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/*  25 */ { strip( 9), {           {   0,  106},            { 107,  208},            { 209,  313}},   {{ -3.988,  2.983,  3.345 }, { -4.854,  3.154,  1.577 }, { -5.103,  1.658,  2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  26 */ { strip( 9), {           { 314,  415},            { 416,  517},            { 518,  627}},   {{ -5.103,  1.658,  2.683 }, { -4.854,  3.154,  1.577 }, { -5.706,  1.658,  0.829 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  27 */ { strip( 9), {           { 628,  734},            { 735,  836},            { 837,  941}},   {{ -4.854,  3.154,  1.577 }, { -5.193,  2.983, -0.362 }, { -5.706,  1.658,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  28 */ { strip( 9), {           { 942, 1043},            {1044, 1113},            {1114, 1218}},   {{ -5.706,  1.658,  0.829 }, { -5.193,  2.983, -0.362 }, { -5.674,  1.658, -1.025 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  29 */ { strip( 0), {           { 950, 1019},            {1020, 1089},            {1090, 1160}},   {{ -5.193,  2.983, -0.362 }, { -5.103,  2.683, -1.658 }, { -5.674,  1.658, -1.025 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  30 */ { strip( 1), {           { 988, 1059},            {1060, 1130}, {1131, 1143, 917,  987}},   {{ -5.674,  1.658, -1.025 }, { -5.103,  2.683, -1.658 }, { -5.193,  1.658, -2.506 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  31 */ { strip( 2), {           {1018, 1089},            {1090, 1173}, {1174, 1177, 948, 1017}},   {{ -5.103,  2.683, -1.658 }, { -4.414,  2.983, -2.759 }, { -5.193,  1.658, -2.506 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  32 */ { strip(10), {           { 113,  197},   { 198,  287, 0,   10},            {  11,  112}},   {{ -5.193,  1.658, -2.506 }, { -4.414,  2.983, -2.759 }, { -4.129,  1.658, -4.025 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*  33 */ { strip(10), {           { 403,  511}, { 512,  602, 288,  299},            { 300,  402}},   {{ -4.414,  2.983, -2.759 }, { -3.000,  3.154, -4.129 }, { -4.129,  1.658, -4.025 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  34 */ { strip(10), { { 909, 918, 603,  696},            { 697,  800},            { 801,  908}},   {{ -4.129,  1.658, -4.025 }, { -3.000,  3.154, -4.129 }, { -2.552,  1.658, -5.170 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  35 */ { strip(10), {           {1014, 1122},            {1123, 1225}, {1226, 1235, 919, 1013}},   {{ -3.000,  3.154, -4.129 }, { -1.260,  2.983, -5.051 }, { -2.552,  1.658, -5.170 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  36 */ { strip( 3), {           { 323,  424},            { 425,  509}, { 510,  517, 229,  322}},   {{ -2.552,  1.658, -5.170 }, { -1.260,  2.983, -5.051 }, { -0.779,  1.658, -5.713 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  37 */ { strip( 3), {           {  73,  144},            { 145,  216}, { 217,  228,   0,   72}},   {{ -1.260,  2.983, -5.051 }, { -0.000,  2.683, -5.366 }, { -0.779,  1.658, -5.713 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  38 */ { strip( 4), {           { 459,  530},  { 531, 543, 315,  373},            { 374,  458}},   {{ -0.779,  1.658, -5.713 }, { -0.000,  2.683, -5.366 }, {  0.779,  1.658, -5.713 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  39 */ { strip( 5), {{1174, 1186, 958, 1016},            {1017, 1101},            {1102, 1173}},   {{ -0.000,  2.683, -5.366 }, {  1.260,  2.983, -5.051 }, {  0.779,  1.658, -5.713 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  40 */ { strip(11), {           { 116,  201},   { 202,  289,  0,  12},            {  13,  115}},   {{  0.779,  1.658, -5.713 }, {  1.260,  2.983, -5.051 }, {  2.552,  1.658, -5.170 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*  41 */ { strip(11), {           { 377,  485},            { 486,  589}, { 590,  605, 290,  376}},   {{  1.260,  2.983, -5.051 }, {  3.000,  3.154, -4.129 }, {  2.552,  1.658, -5.170 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  42 */ { strip(11), {{ 906,  924, 609,  693},            { 694,  796},            { 797,  905}},   {{  2.552,  1.658, -5.170 }, {  3.000,  3.154, -4.129 }, {  4.129,  1.658, -4.025 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  43 */ { strip(11), {           {1018, 1127},            {1128, 1230}, {1231, 1245, 929, 1017}},   {{  3.000,  3.154, -4.129 }, {  4.414,  2.983, -2.759 }, {  4.129,  1.658, -4.025 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  44 */ { strip( 6), {           { 734,  836},            { 837,  921}, { 922,  934, 645,  733}},   {{  4.129,  1.658, -4.025 }, {  4.414,  2.983, -2.759 }, {  5.193,  1.658, -2.506 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  45 */ { strip( 6), {           {1010, 1081},            {1082, 1154}, {1155, 1166, 938, 1009}},   {{  4.414,  2.983, -2.759 }, {  5.103,  2.683, -1.658 }, {  5.193,  1.658, -2.506 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  46 */ { strip( 7), {{ 825, 831,  606,  668},            { 669,  740},            { 741,  824}},   {{  5.193,  1.658, -2.506 }, {  5.103,  2.683, -1.658 }, {  5.674,  1.658, -1.025 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  47 */ { strip( 8), {           {  65,  136},            { 137,  220},  { 221,  227,  0,   64}},   {{  5.103,  2.683, -1.658 }, {  5.193,  2.983, -0.362 }, {  5.674,  1.658, -1.025 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  48 */ { strip(12), {           { 198,  282},  { 283,  288,  0,   94},            {  95,  197}},   {{  5.674,  1.658, -1.025 }, {  5.193,  2.983, -0.362 }, {  5.706,  1.658,  0.829 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*  49 */ { strip(12), {           { 491,  599}, { 600,  604, 289,  387},            { 388,  490}},   {{  5.193,  2.983, -0.362 }, {  4.854,  3.154,  1.577 }, {  5.706,  1.658,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  50 */ { strip(12), {           { 813,  916}, { 917,  920, 607,  705},            { 706,  812}},   {{  5.706,  1.658,  0.829 }, {  4.854,  3.154,  1.577 }, {  5.103,  1.658,  2.683 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  51 */ { strip(12), {           { 934, 1042},            {1043, 1144}, {1145, 1239, 925,  933}},   {{  4.854,  3.154,  1.577 }, {  3.988,  2.983,  3.345 }, {  5.103,  1.658,  2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
// row 2
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/*  52 */ { strip(13), {           {   0,  101},            { 102,  203},            { 204,  276}},   {{ -2.728,  3.802,  3.755 }, { -3.526,  4.341,  2.171 }, { -3.988,  2.983,  3.342 }}}, // 1.700, 1.700, 1.155: 4.5550m (~277 LEDs)
/*  53 */ { strip(13), {           { 277,  378},            { 379,  480},            { 481,  590}},   {{ -3.988,  2.983,  3.342 }, { -3.526,  4.341,  2.171 }, { -4.854,  3.154,  1.577 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  54 */ { strip(14), {           {   0,  106},            { 107,  208},            { 209,  313}},   {{ -3.526,  4.341,  2.171 }, { -4.129,  4.341,  0.317 }, { -4.854,  3.154,  1.577 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  55 */ { strip(14), {           { 314,  415},            { 416,  517},            { 518,  627}},   {{ -4.854,  3.154,  1.577 }, { -4.129,  4.341,  0.317 }, { -5.193,  2.983, -0.362 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  56 */ { strip(14), {           { 628,  729},            { 730,  799},            { 800,  904}},   {{ -4.129,  4.341,  0.317 }, { -4.414,  3.802, -1.434 }, { -5.193,  2.983, -0.362 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  57 */ { strip(14), {           { 905,  974},            { 975, 1044},            {1045, 1115}},   {{ -5.193,  2.983, -0.362 }, { -4.414,  3.802, -1.434 }, { -5.103,  2.683, -1.658 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  58 */ { strip(15), {           {   0,   69},            {  70,  139},            { 140,  210}},   {{ -5.103,  2.683, -1.658 }, { -4.414,  3.802, -1.434 }, { -4.414,  2.983, -2.759 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  59 */ { strip(15), {           { 211,  312},            { 313,  414},            { 415,  487}},   {{ -4.414,  3.802, -1.434 }, { -3.154,  4.341, -2.683 }, { -4.414,  2.983, -2.759 }}}, // 1.700, 1.700, 1.155: 4.5550m (~277 LEDs)
/*  60 */ { strip(16), {           {   0,  101},            { 102,  203},            { 204,  313}},   {{ -4.414,  2.983, -2.759 }, { -3.154,  4.341, -2.683 }, { -3.000,  3.154, -4.129 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  61 */ { strip(16), {           { 314,  420},            { 421,  522},            { 523,  627}},   {{ -3.154,  4.341, -2.683 }, { -1.577,  4.341, -3.829 }, { -3.000,  3.154, -4.129 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  62 */ { strip(16), {           { 628,  729},            { 730,  831},            { 832,  941}},   {{ -3.000,  3.154, -4.129 }, { -1.577,  4.341, -3.829 }, { -1.260,  2.983, -5.051 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  63 */ { strip(17), {           { 181,  280},   { 281,  288, 0,   76},            {  77,  180}},   {{ -1.577,  4.341, -3.829 }, { -0.000,  3.802, -4.641 }, { -1.260,  2.983, -5.051 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  64 */ { strip(16), {           { 942, 1011},            {1012, 1081},            {1082, 1152}},   {{ -1.260,  2.983, -5.051 }, { -0.000,  3.802, -4.641 }, { -0.000,  2.683, -5.366 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  65 */ { strip(17), {{1149, 1155, 928,  992},            { 993, 1076},            {1077, 1148}},   {{  0.000,  2.683, -5.366 }, {  0.000,  3.802, -4.641 }, {  1.260,  2.983, -5.051 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  66 */ { strip(18), {           {  80,  181},            { 182,  282},   { 283,  288, 0,   79}},   {{  0.000,  3.802, -4.641 }, {  1.577,  4.341, -3.829 }, {  1.260,  2.983, -5.051 }}}, // 1.700, 1.700, 1.155: 4.5550m (~277 LEDs)
/*  67 */ { strip(18), {           { 393,  494},            { 495,  597},  { 598, 603, 290,  392}},   {{  1.260,  2.983, -5.051 }, {  1.577,  4.341, -3.829 }, {  3.000,  3.154, -4.129 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  68 */ { strip(18), {           { 704,  811},            { 812,  914},  { 915, 919, 606,  703}},   {{  1.577,  4.341, -3.829 }, {  3.154,  4.341, -2.683 }, {  3.000,  3.154, -4.129 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  69 */ { strip(18), {{1229, 1234, 920, 1016},            {1017, 1119},            {1120, 1228}},   {{  3.000,  3.154, -4.129 }, {  3.154,  4.341, -2.683 }, {  4.414,  2.983, -2.759 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  70 */ { strip(19), {           {   0,  101},            { 102,  171},            { 172,  276}},   {{  3.154,  4.341, -2.683 }, {  4.414,  3.802, -1.434 }, {  4.414,  2.983, -2.759 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  71 */ { strip(19), {           { 277,  346},            { 347,  416},            { 417,  487}},   {{  4.414,  2.983, -2.759 }, {  4.414,  3.802, -1.434 }, {  5.103,  2.683, -1.658 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  72 */ { strip(20), {           {  66,  137},            { 138,  222},   { 223,  227, 0,   65}},   {{  5.103,  2.683, -1.658 }, {  4.414,  3.802, -1.434 }, {  5.193,  2.983, -0.362 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  73 */ { strip(20), {           { 309,  410},            { 411,  512}, { 513,  517, 229,  308}},   {{  4.414,  3.802, -1.434 }, {  4.129,  4.341,  0.317 }, {  5.193,  2.983, -0.362 }}}, // 1.700, 1.700, 1.155: 4.5550m (~277 LEDs)
/*  74 */ { strip(20), {{ 825,  834, 519,  611},            { 612,  714},            { 715,  824}},   {{  5.193,  2.983, -0.362 }, {  4.129,  4.341,  0.317 }, {  4.854,  3.154,  1.577 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  75 */ { strip(20), {{1148, 1152, 837,  939},            { 940, 1043},            {1044, 1147}},   {{  4.129,  4.341,  0.317 }, {  3.526,  4.341,  2.171 }, {  4.854,  3.154,  1.577 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  76 */ { strip(21), {           {   0,  101},            { 102,  203},            { 204,  313}},   {{  4.854,  3.154,  1.577 }, {  3.526,  4.341,  2.171 }, {  3.988,  2.983,  3.342 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  77 */ { strip(21), {           { 314,  415},            { 416,  485},            { 486,  590}},   {{  3.526,  4.341,  2.171 }, {  2.728,  3.802,  3.755 }, {  3.988,  2.983,  3.342 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
// row 3
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/*  78 */ { strip(13), {           { 591,  692},            { 693,  799},            { 800,  904}},   {{ -0.975,  4.341,  4.025 }, { -1.854,  5.103,  2.552 }, { -2.728,  3.802,  3.755 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  79 */ { strip(13), {           { 905, 1011},            {1012, 1113},            {1114, 1218}},   {{ -2.728,  3.802,  3.755 }, { -1.854,  5.103,  2.552 }, { -3.526,  4.341,  2.171 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  80 */ { strip(22), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{ -1.854,  5.103,  2.552 }, { -2.552,  5.366,  0.829 }, { -3.526,  4.341,  2.171 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  81 */ { strip(22), {           { 314,  420},            { 421,  527},            { 528,  635}},   {{ -3.526,  4.341,  2.171 }, { -2.552,  5.366,  0.829 }, { -4.129,  4.341,  0.317 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  82 */ { strip(22), {           { 636,  737},            { 738,  839},            { 840,  949}},   {{ -2.552,  5.366,  0.829 }, { -3.000,  5.103, -0.975 }, { -4.129,  4.341,  0.317 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  83 */ { strip(15), {           { 488,  589},            { 590,  696},            { 697,  801}},   {{ -4.129,  4.341,  0.317 }, { -3.000,  5.103, -0.975 }, { -4.414,  3.802, -1.434 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  84 */ { strip(15), {           { 802,  908},            { 909, 1010},            {1011, 1115}},   {{ -4.414,  3.802, -1.434 }, { -3.000,  5.103, -0.975 }, { -3.154,  4.341, -2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  85 */ { strip(23), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{ -3.000,  5.103, -0.975 }, { -1.577,  5.366, -2.171 }, { -3.154,  4.341, -2.683 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  86 */ { strip(23), {           { 314,  420},            { 421,  527},            { 528,  635}},   {{ -3.154,  4.341, -2.683 }, { -1.577,  5.366, -2.171 }, { -1.577,  4.341, -3.829 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  87 */ { strip(23), {           { 636,  737},            { 738,  839},            { 840,  949}},   {{ -1.577,  5.366, -2.171 }, { -0.000,  5.103, -3.154 }, { -1.577,  4.341, -3.829 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  88 */ { strip(17), {           { 404,  507}, { 508,  607, 293,  301},            { 302,  403}},   {{ -1.577,  4.341, -3.829 }, { -0.000,  5.103, -3.154 }, { -0.000,  3.802, -4.641 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  89 */ { strip(17), { { 913, 924, 608,  705},            { 706,  809},            { 810,  912}},    {{  0.000,  3.802, -4.641 }, {  0.000,  5.103, -3.154 }, {  1.577,  4.341, -3.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  90 */ { strip(24), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{  0.000,  5.103, -3.154 }, {  1.577,  5.366, -2.171 }, {  1.577,  4.341, -3.829 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  91 */ { strip(24), {           { 314,  420},            { 421,  527},            { 528,  635}},   {{  1.577,  4.341, -3.829 }, {  1.577,  5.366, -2.171 }, {  3.154,  4.341, -2.683 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  92 */ { strip(24), {           { 636,  737},            { 738,  839},            { 840,  949}},   {{  1.577,  5.366, -2.171 }, {  3.000,  5.103, -0.975 }, {  3.154,  4.341, -2.683 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  93 */ { strip(19), {           { 488,  589},            { 590,  696},            { 697,  801}},   {{  3.154,  4.341, -2.683 }, {  3.000,  5.103, -0.975 }, {  4.414,  3.802, -1.434 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  94 */ { strip(19), {           { 802,  908},            { 909, 1010},            {1011, 1115}},   {{  4.414,  3.802, -1.434 }, {  3.000,  5.103, -0.975 }, {  4.129,  4.341,  0.317 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  95 */ { strip(25), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{  3.000,  5.103, -0.975 }, {  2.552,  5.366,  0.829 }, {  4.129,  4.341,  0.317 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  96 */ { strip(25), {           { 314,  420},            { 421,  527},            { 528,  635}},   {{  4.129,  4.341,  0.317 }, {  2.552,  5.366,  0.829 }, {  3.526,  4.341,  2.171 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  97 */ { strip(25), {           { 636,  737},            { 738,  839},            { 840,  949}},   {{  2.552,  5.366,  0.829 }, {  1.854,  5.103,  2.552 }, {  3.526,  4.341,  2.171 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  98 */ { strip(21), {           { 591,  692},            { 693,  799},            { 800,  904}},   {{  3.526,  4.341,  2.171 }, {  1.854,  5.103,  2.552 }, {  2.728,  3.802,  3.755 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  99 */ { strip(21), {           { 905, 1011},            {1012, 1113},            {1114, 1218}},   {{  2.728,  3.802,  3.755 }, {  1.854,  5.103,  2.552 }, {  0.975,  4.341,  4.025 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 100 */ { strip(26), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{  1.854,  5.103,  2.552 }, { -0.000,  5.366,  2.683 }, {  0.975,  4.341,  4.025 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 101 */ { strip(26), {           { 314,  420},            { 421,  527},            { 528,  635}},   {{  0.975,  4.341,  4.025 }, { -0.000,  5.366,  2.683 }, { -0.975,  4.341,  4.025 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/* 102 */ { strip(26), {           { 636,  737},            { 738,  839},            { 840,  949}},   {{ -0.000,  5.366,  2.683 }, { -1.854,  5.103,  2.552 }, { -0.975,  4.341,  4.025 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
// row 4
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/* 103 */ { strip(27), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{ -0.000,  5.366,  2.683 }, { -0.779,  5.851,  1.072 }, { -1.854,  5.103,  2.552 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 104 */ { strip(27), {           { 314,  420},            { 421,  522},            { 523,  627}},   {{ -1.854,  5.103,  2.552 }, { -0.779,  5.851,  1.072 }, { -2.552,  5.366,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 105 */ { strip(28), {           {   0,   69},            {  70,  171},            { 172,  276}},   {{ -0.779,  5.851,  1.072 }, { -1.260,  5.851, -0.409 }, { -2.552,  5.366,  0.829 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 106 */ { strip(28), {           { 277,  378},            { 379,  485},            { 486,  590}},   {{ -2.552,  5.366,  0.829 }, { -1.260,  5.851, -0.409 }, { -3.000,  5.103, -0.975 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 107 */ { strip(28), {           { 591,  692},            { 693,  794},            { 795,  904}},   {{ -1.260,  5.851, -0.409 }, { -1.577,  5.366, -2.171 }, { -3.000,  5.103, -0.975 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/* 108 */ { strip(29), {           {   0,   69},            {  70,  171},            { 172,  276}},   {{ -1.260,  5.851, -0.409 }, { -0.000,  5.851, -1.325 }, { -1.577,  5.366, -2.171 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 109 */ { strip(29), {           { 277,  378},            { 379,  485},            { 486,  590}},   {{ -1.577,  5.366, -2.171 }, { -0.000,  5.851, -1.325 }, { -0.000,  5.103, -3.154 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 110 */ { strip(29), {           { 591,  697},            { 698,  799},            { 800,  904}},   {{ -0.000,  5.103, -3.154 }, { -0.000,  5.851, -1.325 }, {  1.577,  5.366, -2.171 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 111 */ { strip(30), {           {   0,   69},            {  70,  171},            { 172,  276}},   {{ -0.000,  5.851, -1.325 }, {  1.260,  5.851, -0.409 }, {  1.577,  5.366, -2.171 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 112 */ { strip(30), {           { 277,  378},            { 379,  485},            { 486,  590}},   {{  1.577,  5.366, -2.171 }, {  1.260,  5.851, -0.409 }, {  3.000,  5.103, -0.975 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 113 */ { strip(30), {           { 591,  697},            { 698,  799},            { 800,  904}},   {{  3.000,  5.103, -0.975 }, {  1.260,  5.851, -0.409 }, {  2.552,  5.366,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 114 */ { strip(31), {           {   0,   69},            {  70,  171},            { 172,  276}},   {{  1.260,  5.851, -0.409 }, {  0.779,  5.851,  1.072 }, {  2.552,  5.366,  0.829 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 115 */ { strip(31), {           { 277,  378},            { 379,  485},            { 486,  590}},   {{  2.552,  5.366,  0.829 }, {  0.779,  5.851,  1.072 }, {  1.854,  5.103,  2.552 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 116 */ { strip(31), {           { 591,  697},            { 698,  799},            { 800,  904}},   {{  1.854,  5.103,  2.552 }, {  0.779,  5.851,  1.072 }, { -0.000,  5.366,  2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 117 */ { strip(27), {           { 628,  697},            { 698,  799},            { 800,  904}},   {{  0.779,  5.851,  1.072 }, { -0.779,  5.851,  1.072 }, { -0.000,  5.366,  2.683 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
// row 5
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/* 118 */ { strip(27), {           { 905,  974},            { 975, 1044},            {1045, 1115}},   {{  0.779,  5.851,  1.072 }, { -0.000,  6.000,  0.000 }, { -0.779,  5.851,  1.072 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 119 */ { strip(28), {           { 905,  974},            { 975, 1044},            {1045, 1115}},   {{ -0.779,  5.851,  1.072 }, { -0.000,  6.000,  0.000 }, { -1.260,  5.851, -0.409 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 120 */ { strip(29), {           { 905,  974},            { 975, 1044},            {1045, 1115}},   {{ -1.260,  5.851, -0.409 }, { -0.000,  6.000,  0.000 }, { -0.000,  5.851, -1.325 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 121 */ { strip(30), {           { 905,  974},            { 975, 1044},            {1045, 1115}},   {{ -0.000,  5.851, -1.325 }, { -0.000,  6.000,  0.000 }, {  1.260,  5.851, -0.409 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 122 */ { strip(31), {           { 905,  974},            { 975, 1044},            {1045, 1115}},   {{  1.260,  5.851, -0.409 }, { -0.000,  6.000,  0.000 }, {  0.779,  5.851,  1.072 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
};
