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
/*   0 */ { strip( 0), {           { 204,  311},   { 312,  323, 0,   95},            {  96,  203}},   {{ -5.103,  1.658,  2.683 }, { -5.706,  1.658,  0.829 }, { -5.706,  0.000,  1.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   1 */ { strip( 0), {           { 516,  624}, { 625,  639, 324,  411},            { 412,  515}},   {{ -5.706,  0.000,  1.854 }, { -5.706,  1.658,  0.829 }, { -6.000,  0.000,  0.000 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*   2 */ { strip( 0), {           { 660,  761},            { 762,  869}, { 870,  954, 642,  659}},   {{ -5.706,  1.658,  0.829 }, { -5.674,  1.658, -1.025 }, { -6.000,  0.000,  0.000 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
#endif
/*   3 */ { strip( 1), {           {  93,  201},            { 202,  303},  { 304,  314,  0,   92}},   {{ -6.000,  0.000,  0.000 }, { -5.674,  1.658, -1.025 }, { -5.706,  0.000, -1.854 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*   4 */ { strip( 1), {{ 843,  916, 629,  640},            { 641,  741},            { 742,  842}},   {{ -5.674,  1.658, -1.025 }, { -5.193,  1.658, -2.506 }, { -5.706,  0.000, -1.854 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/*   5 */ { strip( 1), {           { 323,  424},            { 425,  533}, { 534,  628, 315,  322}},   {{ -5.706,  0.000, -1.854 }, { -5.193,  1.658, -2.506 }, { -4.854,  0.000, -3.526 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*   6 */ { strip( 2), {           { 841,  942}, { 943,  947, 633,  731},            { 732,  840}},   {{ -5.193,  1.658, -2.506 }, { -4.129,  1.658, -4.025 }, { -4.854,  0.000, -3.526 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*   7 */ { strip( 2), {           { 526,  628}, { 629,  632, 318,  421},            { 422,  525}},   {{ -4.854,  0.000, -3.526 }, { -4.129,  1.658, -4.025 }, { -3.526,  0.000, -4.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*   8 */ { strip( 2), {           {   2,  107},            { 108,  213},  { 214, 317,   0,    1}},   {{ -4.129,  1.658, -4.025 }, { -2.552,  1.658, -5.170 }, { -3.526,  0.000, -4.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   9 */ { strip( 3), {           {1033, 1140}, {1141, 1147, 832,  929},            { 930, 1032}},   {{ -3.526,  0.000, -4.854 }, { -2.552,  1.658, -5.170 }, { -1.854,  0.000, -5.706 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  10 */ { strip( 3), {           { 716,  817}, { 818,  831, 518,  612},            { 613,  715}},   {{ -2.552,  1.658, -5.170 }, { -0.779,  1.658, -5.713 }, { -1.854,  0.000, -5.706 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  11 */ { strip( 4), {           {1047, 1155}, {1156, 1162, 848,  943},            { 944, 1046}},   {{ -1.854,  0.000, -5.706 }, { -0.779,  1.658, -5.713 }, { -0.000,  0.000, -6.000 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
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
/*  25 */ { strip( 9), {           { 110,  217},  { 218,  314,  0,    7},            {   8,  109}},   {{ -3.988,  2.983,  3.345 }, { -4.854,  3.154,  1.577 }, { -5.103,  1.658,  2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  26 */ { strip( 9), {           { 436,  539}, { 540,  630, 317,  328},            { 329,  435}},   {{ -5.103,  1.658,  2.683 }, { -4.854,  3.154,  1.577 }, { -5.706,  1.658,  0.829 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  27 */ { strip( 9), {           { 728,  836},            { 837,  939}, { 940,  951, 636,  727}},   {{ -4.854,  3.154,  1.577 }, { -5.193,  2.983, -0.362 }, { -5.706,  1.658,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  28 */ { strip( 9), {{1229, 1240, 953, 1041},            {1042, 1126},            {1127, 1228}},   {{ -5.706,  1.658,  0.829 }, { -5.193,  2.983, -0.362 }, { -5.674,  1.658, -1.025 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  29 */ { strip( 0), {           {1030, 1101},            {1102, 1172}, {1173, 1183, 957, 1029}},   {{ -5.193,  2.983, -0.362 }, { -5.103,  2.683, -1.658 }, { -5.674,  1.658, -1.025 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
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
/*  52 */ { strip(13), {           { 399,  500},            { 501,  602},{ 603,  608,  319,  398}},   {{ -2.728,  3.802,  3.755 }, { -3.526,  4.341,  2.171 }, { -3.988,  2.983,  3.342 }}}, // 1.700, 1.700, 1.155: 4.5550m (~277 LEDs)
/*  53 */ { strip(13), {   {311,  315, 0,   97},            {  98,  201},            { 202,  310}},   {{ -3.988,  2.983,  3.342 }, { -3.526,  4.341,  2.171 }, { -4.854,  3.154,  1.577 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  54 */ { strip(14), {           { 200,  306},  { 307,  314,  0,   95},            {  96,  199}},   {{ -3.526,  4.341,  2.171 }, { -4.129,  4.341,  0.317 }, { -4.854,  3.154,  1.577 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  55 */ { strip(14), {{ 535,  630, 316,  322},            { 323,  425},            { 426,  534}},   {{ -4.854,  3.154,  1.577 }, { -4.129,  4.341,  0.317 }, { -5.193,  2.983, -0.362 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  56 */ { strip(14), {{ 915,  922, 633,  726},            { 727,  812},            { 813,  914}},   {{ -4.129,  4.341,  0.317 }, { -4.414,  3.802, -1.434 }, { -5.193,  2.983, -0.362 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  57 */ { strip(14), {{1142, 1149, 923,  998},            { 999, 1070},            {1071, 1141}},   {{ -5.193,  2.983, -0.362 }, { -4.414,  3.802, -1.434 }, { -5.103,  2.683, -1.658 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  58 */ { strip(15), {{1092, 1157, 928,  935},            { 936, 1019},            {1020, 1091}},   {{ -5.103,  2.683, -1.658 }, { -4.414,  3.802, -1.434 }, { -4.414,  2.983, -2.759 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/*  59 */ { strip(15), {           { 644,  746},            { 747,  848}, { 849,  927, 638,  643}},   {{ -4.414,  3.802, -1.434 }, { -3.154,  4.341, -2.683 }, { -4.414,  2.983, -2.759 }}}, // 1.700, 1.700, 1.155: 4.5550m (~277 LEDs)
/*  60 */ { strip(16), {           { 963, 1064},            {1065, 1168}, {1169, 1174, 860,  962}},   {{ -4.414,  2.983, -2.759 }, { -3.154,  4.341, -2.683 }, { -3.000,  3.154, -4.129 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  61 */ { strip(16), {           { 639,  746},            { 747,  850}, { 851,  858, 544,  638}},   {{ -3.154,  4.341, -2.683 }, { -1.577,  4.341, -3.829 }, { -3.000,  3.154, -4.129 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  62 */ { strip(16), {{ 535,  541, 228,  324},            { 325,  425},            { 426,  534}},   {{ -3.000,  3.154, -4.129 }, { -1.577,  4.341, -3.829 }, { -1.260,  2.983, -5.051 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  63 */ { strip(17), {           { 181,  280},   { 281,  288, 0,   76},            {  77,  180}},   {{ -1.577,  4.341, -3.829 }, { -0.000,  3.802, -4.641 }, { -1.260,  2.983, -5.051 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
/*  64 */ { strip(16), {  { 220,  226, 0,   76},            {  77,  148},            { 149,  219}},   {{ -1.260,  2.983, -5.051 }, { -0.000,  3.802, -4.641 }, { -0.000,  2.683, -5.366 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
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
/*  76 */ { strip(21), {           { 103,  206},            { 207,  308},  { 309,  314,  0,  102}},   {{  4.854,  3.154,  1.577 }, {  3.526,  4.341,  2.171 }, {  3.988,  2.983,  3.342 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  77 */ { strip(21), {           { 500,  602}, { 603,  608, 318,  397},            { 398,  499}},   {{  3.526,  4.341,  2.171 }, {  2.728,  3.802,  3.755 }, {  3.988,  2.983,  3.342 }}}, // 1.700, 1.155, 1.700: 4.5550m (~277 LEDs)
// row 3
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/*  78 */ { strip(13), {           {1148, 1046},            {1045,  938}, {937,  931, 1244, 1149}},   {{ -0.975,  4.341,  4.025 }, { -1.854,  5.103,  2.552 }, { -2.728,  3.802,  3.755 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  79 */ { strip(13), { { 918, 926, 612,  711},            { 712,  815},            { 816,  917}},   {{ -2.728,  3.802,  3.755 }, { -1.854,  5.103,  2.552 }, { -3.526,  4.341,  2.171 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  80 */ { strip(22), {           { 847,  950}, { 951,  958, 644,  743},            { 744,  846}},   {{ -1.854,  5.103,  2.552 }, { -2.552,  5.366,  0.829 }, { -3.526,  4.341,  2.171 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  81 */ { strip(22), {           { 524,  631}, { 632,  642, 318,  415},            { 416,  523}},   {{ -3.526,  4.341,  2.171 }, { -2.552,  5.366,  0.829 }, { -4.129,  4.341,  0.317 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  82 */ { strip(22), {           {  13,  116},            { 117,  220},   { 221,  314, 0,   12}},   {{ -2.552,  5.366,  0.829 }, { -3.000,  5.103, -0.975 }, { -4.129,  4.341,  0.317 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  83 */ { strip(15), {           {  93,  196},            { 197,  306},  { 307,  316,  0,   92}},   {{ -4.129,  4.341,  0.317 }, { -3.000,  5.103, -0.975 }, { -4.414,  3.802, -1.434 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  84 */ { strip(15), {{ 626,  636, 319,  419},            { 420,  523},            { 524,  625}},   {{ -4.414,  3.802, -1.434 }, { -3.000,  5.103, -0.975 }, { -3.154,  4.341, -2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  85 */ { strip(23), {           {   0,  101},            { 102,  208},            { 209,  313}},   {{ -3.000,  5.103, -0.975 }, { -1.577,  5.366, -2.171 }, { -3.154,  4.341, -2.683 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  86 */ { strip(23), {           { 314,  420},            { 421,  527},            { 528,  635}},   {{ -3.154,  4.341, -2.683 }, { -1.577,  5.366, -2.171 }, { -1.577,  4.341, -3.829 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  87 */ { strip(23), {           { 636,  737},            { 738,  839},            { 840,  949}},   {{ -1.577,  5.366, -2.171 }, { -0.000,  5.103, -3.154 }, { -1.577,  4.341, -3.829 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  88 */ { strip(17), {           { 404,  507}, { 508,  607, 293,  301},            { 302,  403}},   {{ -1.577,  4.341, -3.829 }, { -0.000,  5.103, -3.154 }, { -0.000,  3.802, -4.641 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  89 */ { strip(17), { { 913, 924, 608,  705},            { 706,  809},            { 810,  912}},   {{  0.000,  3.802, -4.641 }, {  0.000,  5.103, -3.154 }, {  1.577,  4.341, -3.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  90 */ { strip(24), {           { 202,  304},   { 305,  314, 0,   97},            {  98,  201}},   {{  0.000,  5.103, -3.154 }, {  1.577,  5.366, -2.171 }, {  1.577,  4.341, -3.829 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  91 */ { strip(24), {           { 522,  628}, { 629,  640, 317,  412},            { 413,  521}},   {{  1.577,  4.341, -3.829 }, {  1.577,  5.366, -2.171 }, {  3.154,  4.341, -2.683 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  92 */ { strip(24), {           { 653,  756},            { 757,  860}, { 861,  957, 643,  652}},   {{  1.577,  5.366, -2.171 }, {  3.000,  5.103, -0.975 }, {  3.154,  4.341, -2.683 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  93 */ { strip(19), {           { 488,  589},            { 590,  696},            { 697,  801}},   {{  3.154,  4.341, -2.683 }, {  3.000,  5.103, -0.975 }, {  4.414,  3.802, -1.434 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  94 */ { strip(19), {           { 802,  908},            { 909, 1010},            {1011, 1115}},   {{  4.414,  3.802, -1.434 }, {  3.000,  5.103, -0.975 }, {  4.129,  4.341,  0.317 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  95 */ { strip(25), {           { 199,  302},  { 303,  315,  0,   94},            {  95,  198}},   {{  3.000,  5.103, -0.975 }, {  2.552,  5.366,  0.829 }, {  4.129,  4.341,  0.317 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  96 */ { strip(25), {{ 546,  641, 317,  329},            { 330,  437},            { 438,  545}},   {{  4.129,  4.341,  0.317 }, {  2.552,  5.366,  0.829 }, {  3.526,  4.341,  2.171 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  97 */ { strip(25), {           { 657,  760},            { 761,  864}, { 865,  960, 645,  656}},   {{  2.552,  5.366,  0.829 }, {  1.854,  5.103,  2.552 }, {  3.526,  4.341,  2.171 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  98 */ { strip(21), {           { 723,  826}, { 827,  925, 610,  619},            { 620,  722}},   {{  3.526,  4.341,  2.171 }, {  1.854,  5.103,  2.552 }, {  2.728,  3.802,  3.755 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  99 */ { strip(21), {{1238, 1241, 927, 1031},            {1032, 1134},            {1135, 1237}},   {{  2.728,  3.802,  3.755 }, {  1.854,  5.103,  2.552 }, {  0.975,  4.341,  4.025 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 100 */ { strip(26), {           { 204,  307}, { 308,  314,   0,  100},            { 101,  203}},   {{  1.854,  5.103,  2.552 }, { -0.000,  5.366,  2.683 }, {  0.975,  4.341,  4.025 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 101 */ { strip(26), {           { 436,  329}, { 328,  317, 641,  545},            { 544,  437}},   {{  0.975,  4.341,  4.025 }, { -0.000,  5.366,  2.683 }, { -0.975,  4.341,  4.025 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/* 102 */ { strip(26), {           { 653,  756},            { 757,  859},  { 860, 957, 644,  652}},   {{ -0.000,  5.366,  2.683 }, { -1.854,  5.103,  2.552 }, { -0.975,  4.341,  4.025 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
// row 4
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/* 103 */ { strip(27), {           { 721,  823}, { 824,  834, 519,  616},            { 617,  720}},   {{ -0.000,  5.366,  2.683 }, { -0.779,  5.851,  1.072 }, { -1.854,  5.103,  2.552 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 104 */ { strip(27), {{1056, 1154, 838,  849},            { 850,  951},            { 952, 1055}},   {{ -1.854,  5.103,  2.552 }, { -0.779,  5.851,  1.072 }, { -2.552,  5.366,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 105 */ { strip(28), {           { 416,  501}, { 502,  513, 224,  313},            { 314,  415}},   {{ -0.779,  5.851,  1.072 }, { -1.260,  5.851, -0.409 }, { -2.552,  5.366,  0.829 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 106 */ { strip(28), {           { 717,  818}, { 819,  829, 514,  612},            { 613,  716}},   {{ -2.552,  5.366,  0.829 }, { -1.260,  5.851, -0.409 }, { -3.000,  5.103, -0.975 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 107 */ { strip(28), {           { 846,  947},            { 948, 1051}, {1052, 1147, 832,  845}},    {{ -1.260,  5.851, -0.409 }, { -1.577,  5.366, -2.171 }, { -3.000,  5.103, -0.975 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/* 108 */ { strip(29), {           { 423,  506}, { 507,  512, 225,  320},            { 321,  422}},   {{ -1.260,  5.851, -0.409 }, { -0.000,  5.851, -1.325 }, { -1.577,  5.366, -2.171 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 109 */ { strip(29), {           { 713,  815}, { 816,  828, 513,  608},            { 609,  712}},   {{ -1.577,  5.366, -2.171 }, { -0.000,  5.851, -1.325 }, { -0.000,  5.103, -3.154 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 110 */ { strip(29), {{1049, 1145, 830,  841},            { 842,  944},            { 945, 1048}},   {{ -0.000,  5.103, -3.154 }, { -0.000,  5.851, -1.325 }, {  1.577,  5.366, -2.171 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 111 */ { strip(30), {           { 423,  508}, { 509,  518, 229,  320},            { 321,  422}},   {{ -0.000,  5.851, -1.325 }, {  1.260,  5.851, -0.409 }, {  1.577,  5.366, -2.171 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 112 */ { strip(30), {           { 721,  822}, { 823,  853, 519,  616},            { 617,  720}},   {{  1.577,  5.366, -2.171 }, {  1.260,  5.851, -0.409 }, {  3.000,  5.103, -0.975 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 113 */ { strip(30), {{937,  836, 1152, 1144},            {1143, 1042},            {1041,  938}},   {{  3.000,  5.103, -0.975 }, {  1.260,  5.851, -0.409 }, {  2.552,  5.366,  0.829 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 114 */ { strip(31), {           { 421,  506}, { 507,  517, 228,  318},            { 319,  420}},   {{  1.260,  5.851, -0.409 }, {  0.779,  5.851,  1.072 }, {  2.552,  5.366,  0.829 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
/* 115 */ { strip(31), {           { 722,  824}, { 825,  834, 519,  617},            { 618,  721}},   {{  2.552,  5.366,  0.829 }, {  0.779,  5.851,  1.072 }, {  1.854,  5.103,  2.552 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 116 */ { strip(31), {{1052, 1151, 836,  845},            { 846,  947},            { 948, 1051}},   {{  1.854,  5.103,  2.552 }, {  0.779,  5.851,  1.072 }, { -0.000,  5.366,  2.683 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/* 117 */ { strip(27), {           { 425,  509}, { 510,  517, 228,  321},            { 322,  424}},   {{  0.779,  5.851,  1.072 }, { -0.779,  5.851,  1.072 }, { -0.000,  5.366,  2.683 }}}, // 1.155, 1.700, 1.700: 4.5550m (~277 LEDs)
// row 5
/* triangle   strip             LEDs on 1st edge         LEDs on 2nd edge          LEDs on 3rd edge      coordinates 1st corner      coordinates 2nd corner      coordinates 3rd corner  */
/* 118 */ { strip(27), {           { 133,   62},    { 61, 0,  226,  217},            {  216, 134}},   {{  0.779,  5.851,  1.072 }, { -0.000,  6.000,  0.000 }, { -0.779,  5.851,  1.072 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 119 */ { strip(28), {           { 148,   77},            {  76,    6},       { 5, 0, 223, 149}},   {{ -0.779,  5.851,  1.072 }, { -0.000,  6.000,  0.000 }, { -1.260,  5.851, -0.409 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 120 */ { strip(29), {           { 151,   80},            {  79,    9},       { 8, 0, 224, 152}},   {{ -1.260,  5.851, -0.409 }, { -0.000,  6.000,  0.000 }, { -0.000,  5.851, -1.325 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 121 */ { strip(30), {           { 134,   63},    { 62, 0,  227,  220},            { 219,  135}},   {{ -0.000,  5.851, -1.325 }, { -0.000,  6.000,  0.000 }, {  1.260,  5.851, -0.409 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
/* 122 */ { strip(31), {           { 133,   62},    { 61, 0,  227,  219},            { 218,  134}},   {{  1.260,  5.851, -0.409 }, { -0.000,  6.000,  0.000 }, {  0.779,  5.851,  1.072 }}}, // 1.155, 1.155, 1.155: 3.4650m (~211 LEDs)
};
