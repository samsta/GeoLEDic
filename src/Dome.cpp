#include "Dome.hpp"

CRGB leds[NUM_STRIPS * LEDS_PER_STRIP];

namespace {
CRGB* strip(int ix){ return &leds[ix*LEDS_PER_STRIP]; }
}

Triangle dome[DOME_NUM_TRIANGLES] = {
// row 0
/*   0 */ { strip( 0), {{   0,  106}, { 107,  213}, { 214,  321}},  {{ -5.104,  1.658,  2.683 }, { -5.706,  1.658,  0.829 }, { -5.706,  0.000,  1.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   1 */ { strip( 0), {{ 322,  428}, { 429,  530}, { 531,  635}},  {{ -5.706,  0.000,  1.854 }, { -5.706,  1.658,  0.829 }, { -6.000,  0.000,  0.000 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*   2 */ { strip( 0), {{ 636,  736}, { 737,  844}, { 845,  949}},  {{ -5.706,  1.658,  0.829 }, { -5.733,  1.507, -0.931 }, { -6.000,  0.000,  0.000 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/*   3 */ { strip( 1), {{   0,  107}, { 108,  208}, { 209,  313}},  {{ -6.000,  0.000,  0.000 }, { -5.733,  1.507, -0.931 }, { -5.706,  0.000, -1.854 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/*   4 */ { strip( 1), {{ 314,  396}, { 397,  497}, { 498,  601}},  {{ -5.733,  1.507, -0.931 }, { -5.185,  1.507, -2.616 }, { -5.706,  0.000, -1.854 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/*   5 */ { strip( 1), {{ 602,  702}, { 703,  810}, { 811,  915}},  {{ -5.706,  0.000, -1.854 }, { -5.185,  1.507, -2.616 }, { -4.854,  0.000, -3.527 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/*   6 */ { strip( 2), {{   0,  100}, { 101,  202}, { 203,  313}},  {{ -5.185,  1.507, -2.616 }, { -4.129,  1.658, -4.025 }, { -4.854,  0.000, -3.527 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*   7 */ { strip( 2), {{ 314,  415}, { 416,  522}, { 523,  627}},  {{ -4.854,  0.000, -3.527 }, { -4.129,  1.658, -4.025 }, { -3.527,  0.000, -4.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*   8 */ { strip( 2), {{ 628,  734}, { 735,  841}, { 842,  949}},  {{ -4.129,  1.658, -4.025 }, { -2.552,  1.658, -5.171 }, { -3.527,  0.000, -4.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*   9 */ { strip( 3), {{   0,  106}, { 107,  208}, { 209,  313}},  {{ -3.527,  0.000, -4.854 }, { -2.552,  1.658, -5.171 }, { -1.854,  0.000, -5.706 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  10 */ { strip( 3), {{ 314,  414}, { 415,  522}, { 523,  627}},  {{ -2.552,  1.658, -5.171 }, { -0.886,  1.507, -5.740 }, { -1.854,  0.000, -5.706 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/*  11 */ { strip( 4), {{   0,  107}, { 108,  208}, { 209,  313}},  {{ -1.854,  0.000, -5.706 }, { -0.886,  1.507, -5.740 }, { -0.000,  0.000, -6.000 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/*  12 */ { strip( 4), {{ 314,  396}, { 397,  497}, { 498,  601}},  {{ -0.886,  1.507, -5.740 }, {  0.886,  1.507, -5.740 }, { -0.000,  0.000, -6.000 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/*  13 */ { strip( 4), {{ 602,  702}, { 703,  810}, { 811,  915}},  {{ -0.000,  0.000, -6.000 }, {  0.886,  1.507, -5.740 }, {  1.854,  0.000, -5.706 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/*  14 */ { strip( 5), {{   0,  100}, { 101,  202}, { 203,  313}},  {{  0.886,  1.507, -5.740 }, {  2.552,  1.658, -5.171 }, {  1.854,  0.000, -5.706 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*  15 */ { strip( 5), {{ 314,  415}, { 416,  522}, { 523,  627}},  {{  1.854,  0.000, -5.706 }, {  2.552,  1.658, -5.171 }, {  3.527,  0.000, -4.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  16 */ { strip( 5), {{ 628,  734}, { 735,  841}, { 842,  949}},  {{  2.552,  1.658, -5.171 }, {  4.129,  1.658, -4.025 }, {  3.527,  0.000, -4.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  17 */ { strip( 6), {{   0,  106}, { 107,  208}, { 209,  313}},  {{  3.527,  0.000, -4.854 }, {  4.129,  1.658, -4.025 }, {  4.854,  0.000, -3.527 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  18 */ { strip( 6), {{ 314,  414}, { 415,  522}, { 523,  627}},  {{  4.129,  1.658, -4.025 }, {  5.185,  1.507, -2.616 }, {  4.854,  0.000, -3.527 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/*  19 */ { strip( 7), {{   0,  107}, { 108,  208}, { 209,  313}},  {{  4.854,  0.000, -3.527 }, {  5.185,  1.507, -2.616 }, {  5.706,  0.000, -1.854 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/*  20 */ { strip( 7), {{ 314,  396}, { 397,  497}, { 498,  601}},  {{  5.185,  1.507, -2.616 }, {  5.733,  1.507, -0.931 }, {  5.706,  0.000, -1.854 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/*  21 */ { strip( 7), {{ 602,  702}, { 703,  810}, { 811,  915}},  {{  5.706,  0.000, -1.854 }, {  5.733,  1.507, -0.931 }, {  6.000,  0.000,  0.000 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/*  22 */ { strip( 8), {{   0,  100}, { 101,  202}, { 203,  313}},  {{  5.733,  1.507, -0.931 }, {  5.706,  1.658,  0.829 }, {  6.000,  0.000,  0.000 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*  23 */ { strip( 8), {{ 314,  415}, { 416,  522}, { 523,  627}},  {{  6.000,  0.000,  0.000 }, {  5.706,  1.658,  0.829 }, {  5.706,  0.000,  1.854 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  24 */ { strip( 8), {{ 628,  734}, { 735,  841}, { 842,  949}},  {{  5.706,  1.658,  0.829 }, {  5.104,  1.658,  2.683 }, {  5.706,  0.000,  1.854 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
// row 1
/*  25 */ { strip( 9), {{   0,  107}, { 108,  209}, { 210,  313}},  {{ -4.090,  3.014,  3.192 }, { -4.854,  3.154,  1.577 }, { -5.104,  1.658,  2.683 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  26 */ { strip( 9), {{ 314,  415}, { 416,  517}, { 518,  627}},  {{ -5.104,  1.658,  2.683 }, { -4.854,  3.154,  1.577 }, { -5.706,  1.658,  0.829 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  27 */ { strip( 9), {{ 628,  735}, { 736,  836}, { 837,  941}},  {{ -4.854,  3.154,  1.577 }, { -5.185,  3.014, -0.178 }, { -5.706,  1.658,  0.829 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/*  28 */ { strip( 9), {{ 942, 1042}, {1043, 1125}, {1126, 1229}},  {{ -5.706,  1.658,  0.829 }, { -5.185,  3.014, -0.178 }, { -5.733,  1.507, -0.931 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
/*  29 */ { strip( 0), {{ 950, 1019}, {1020, 1089}, {1090, 1174}},  {{ -5.185,  3.014, -0.178 }, { -5.104,  2.683, -1.658 }, { -5.733,  1.507, -0.931 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/*  30 */ { strip( 1), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{ -5.733,  1.507, -0.931 }, { -5.104,  2.683, -1.658 }, { -5.185,  1.507, -2.616 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/*  31 */ { strip( 2), {{ 950, 1019}, {1020, 1102}, {1103, 1174}},  {{ -5.104,  2.683, -1.658 }, { -4.299,  3.014, -2.904 }, { -5.185,  1.507, -2.616 }}}, // 1.155, 1.380, 1.155: 3.6900m (~225 LEDs)
/*  32 */ { strip(10), {{   0,   82}, {  83,  183}, { 184,  287}},  {{ -5.185,  1.507, -2.616 }, { -4.299,  3.014, -2.904 }, { -4.129,  1.658, -4.025 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/*  33 */ { strip(10), {{ 288,  395}, { 396,  497}, { 498,  601}},  {{ -4.299,  3.014, -2.904 }, { -3.000,  3.154, -4.129 }, { -4.129,  1.658, -4.025 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  34 */ { strip(10), {{ 602,  703}, { 704,  805}, { 806,  915}},  {{ -4.129,  1.658, -4.025 }, { -3.000,  3.154, -4.129 }, { -2.552,  1.658, -5.171 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  35 */ { strip(10), {{ 916, 1023}, {1024, 1124}, {1125, 1229}},  {{ -3.000,  3.154, -4.129 }, { -1.433,  3.014, -4.986 }, { -2.552,  1.658, -5.171 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/*  36 */ { strip( 3), {{ 628,  728}, { 729,  811}, { 812,  915}},  {{ -2.552,  1.658, -5.171 }, { -1.433,  3.014, -4.986 }, { -0.886,  1.507, -5.740 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
/*  37 */ { strip( 3), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{ -1.433,  3.014, -4.986 }, { -0.000,  2.683, -5.367 }, { -0.886,  1.507, -5.740 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/*  38 */ { strip( 4), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{ -0.886,  1.507, -5.740 }, { -0.000,  2.683, -5.367 }, {  0.886,  1.507, -5.740 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/*  39 */ { strip( 5), {{ 950, 1019}, {1020, 1102}, {1103, 1174}},  {{ -0.000,  2.683, -5.367 }, {  1.433,  3.014, -4.986 }, {  0.886,  1.507, -5.740 }}}, // 1.155, 1.380, 1.155: 3.6900m (~225 LEDs)
/*  40 */ { strip(11), {{   0,   82}, {  83,  183}, { 184,  287}},  {{  0.886,  1.507, -5.740 }, {  1.433,  3.014, -4.986 }, {  2.552,  1.658, -5.171 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/*  41 */ { strip(11), {{ 288,  395}, { 396,  497}, { 498,  601}},  {{  1.433,  3.014, -4.986 }, {  3.000,  3.154, -4.129 }, {  2.552,  1.658, -5.171 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  42 */ { strip(11), {{ 602,  703}, { 704,  805}, { 806,  915}},  {{  2.552,  1.658, -5.171 }, {  3.000,  3.154, -4.129 }, {  4.129,  1.658, -4.025 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  43 */ { strip(11), {{ 916, 1023}, {1024, 1124}, {1125, 1229}},  {{  3.000,  3.154, -4.129 }, {  4.299,  3.014, -2.904 }, {  4.129,  1.658, -4.025 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/*  44 */ { strip( 6), {{ 628,  728}, { 729,  811}, { 812,  915}},  {{  4.129,  1.658, -4.025 }, {  4.299,  3.014, -2.904 }, {  5.185,  1.507, -2.616 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
/*  45 */ { strip( 6), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{  4.299,  3.014, -2.904 }, {  5.104,  2.683, -1.658 }, {  5.185,  1.507, -2.616 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/*  46 */ { strip( 7), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{  5.185,  1.507, -2.616 }, {  5.104,  2.683, -1.658 }, {  5.733,  1.507, -0.931 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/*  47 */ { strip( 8), {{ 950, 1019}, {1020, 1102}, {1103, 1174}},  {{  5.104,  2.683, -1.658 }, {  5.185,  3.014, -0.178 }, {  5.733,  1.507, -0.931 }}}, // 1.155, 1.380, 1.155: 3.6900m (~225 LEDs)
/*  48 */ { strip(12), {{   0,   82}, {  83,  183}, { 184,  287}},  {{  5.733,  1.507, -0.931 }, {  5.185,  3.014, -0.178 }, {  5.706,  1.658,  0.829 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/*  49 */ { strip(12), {{ 288,  395}, { 396,  497}, { 498,  601}},  {{  5.185,  3.014, -0.178 }, {  4.854,  3.154,  1.577 }, {  5.706,  1.658,  0.829 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  50 */ { strip(12), {{ 602,  703}, { 704,  805}, { 806,  915}},  {{  5.706,  1.658,  0.829 }, {  4.854,  3.154,  1.577 }, {  5.104,  1.658,  2.683 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  51 */ { strip(12), {{ 916, 1023}, {1024, 1124}, {1125, 1229}},  {{  4.854,  3.154,  1.577 }, {  4.090,  3.014,  3.192 }, {  5.104,  1.658,  2.683 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
// row 2
/*  52 */ { strip(13), {{   0,  100}, { 101,  201}, { 202,  287}},  {{ -2.657,  3.945,  3.657 }, { -3.527,  4.342,  2.171 }, { -4.090,  3.014,  3.192 }}}, // 1.677, 1.677, 1.380: 4.7340m (~288 LEDs)
/*  53 */ { strip(13), {{ 288,  388}, { 389,  490}, { 491,  601}},  {{ -4.090,  3.014,  3.192 }, { -3.527,  4.342,  2.171 }, { -4.854,  3.154,  1.577 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*  54 */ { strip(14), {{   0,  106}, { 107,  208}, { 209,  313}},  {{ -3.527,  4.342,  2.171 }, { -4.129,  4.342,  0.317 }, { -4.854,  3.154,  1.577 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  55 */ { strip(14), {{ 314,  415}, { 416,  516}, { 517,  627}},  {{ -4.854,  3.154,  1.577 }, { -4.129,  4.342,  0.317 }, { -5.185,  3.014, -0.178 }}}, // 1.700, 1.677, 1.799: 5.1760m (~314 LEDs)
/*  56 */ { strip(14), {{ 628,  728}, { 729,  811}, { 812,  915}},  {{ -4.129,  4.342,  0.317 }, { -4.299,  3.945, -1.397 }, { -5.185,  3.014, -0.178 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
/*  57 */ { strip(14), {{ 916,  998}, { 999, 1068}, {1069, 1140}},  {{ -5.185,  3.014, -0.178 }, { -4.299,  3.945, -1.397 }, { -5.104,  2.683, -1.658 }}}, // 1.380, 1.155, 1.155: 3.6900m (~225 LEDs)
/*  58 */ { strip(15), {{   0,   69}, {  70,  152}, { 153,  224}},  {{ -5.104,  2.683, -1.658 }, { -4.299,  3.945, -1.397 }, { -4.299,  3.014, -2.904 }}}, // 1.155, 1.380, 1.155: 3.6900m (~225 LEDs)
/*  59 */ { strip(15), {{ 225,  325}, { 326,  426}, { 427,  512}},  {{ -4.299,  3.945, -1.397 }, { -3.154,  4.342, -2.683 }, { -4.299,  3.014, -2.904 }}}, // 1.677, 1.677, 1.380: 4.7340m (~288 LEDs)
/*  60 */ { strip(16), {{   0,  100}, { 101,  202}, { 203,  313}},  {{ -4.299,  3.014, -2.904 }, { -3.154,  4.342, -2.683 }, { -3.000,  3.154, -4.129 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*  61 */ { strip(16), {{ 314,  420}, { 421,  522}, { 523,  627}},  {{ -3.154,  4.342, -2.683 }, { -1.577,  4.342, -3.829 }, { -3.000,  3.154, -4.129 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  62 */ { strip(16), {{ 628,  729}, { 730,  830}, { 831,  941}},  {{ -3.000,  3.154, -4.129 }, { -1.577,  4.342, -3.829 }, { -1.433,  3.014, -4.986 }}}, // 1.700, 1.677, 1.799: 5.1760m (~314 LEDs)
/*  63 */ { strip(17), {{   0,  100}, { 101,  183}, { 184,  287}},  {{ -1.577,  4.342, -3.829 }, { -0.000,  3.945, -4.521 }, { -1.433,  3.014, -4.986 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
/*  64 */ { strip(16), {{ 942, 1024}, {1025, 1094}, {1095, 1166}},  {{ -1.433,  3.014, -4.986 }, { -0.000,  3.945, -4.521 }, { -0.000,  2.683, -5.367 }}}, // 1.380, 1.155, 1.155: 3.6900m (~225 LEDs)
/*  65 */ { strip(18), {{   0,   69}, {  70,  152}, { 153,  224}},  {{  0.000,  2.683, -5.367 }, {  0.000,  3.945, -4.521 }, {  1.433,  3.014, -4.986 }}}, // 1.155, 1.380, 1.155: 3.6900m (~225 LEDs)
/*  66 */ { strip(17), {{ 288,  388}, { 389,  489}, { 490,  575}},  {{  0.000,  3.945, -4.521 }, {  1.577,  4.342, -3.829 }, {  1.433,  3.014, -4.986 }}}, // 1.677, 1.677, 1.380: 4.7340m (~288 LEDs)
/*  67 */ { strip(18), {{ 225,  325}, { 326,  427}, { 428,  538}},  {{  1.433,  3.014, -4.986 }, {  1.577,  4.342, -3.829 }, {  3.000,  3.154, -4.129 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*  68 */ { strip(18), {{ 539,  645}, { 646,  747}, { 748,  852}},  {{  1.577,  4.342, -3.829 }, {  3.154,  4.342, -2.683 }, {  3.000,  3.154, -4.129 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  69 */ { strip(18), {{ 853,  954}, { 955, 1055}, {1056, 1166}},  {{  3.000,  3.154, -4.129 }, {  3.154,  4.342, -2.683 }, {  4.299,  3.014, -2.904 }}}, // 1.700, 1.677, 1.799: 5.1760m (~314 LEDs)
/*  70 */ { strip(19), {{   0,  100}, { 101,  183}, { 184,  287}},  {{  3.154,  4.342, -2.683 }, {  4.299,  3.945, -1.397 }, {  4.299,  3.014, -2.904 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
/*  71 */ { strip(19), {{ 288,  370}, { 371,  440}, { 441,  512}},  {{  4.299,  3.014, -2.904 }, {  4.299,  3.945, -1.397 }, {  5.104,  2.683, -1.658 }}}, // 1.380, 1.155, 1.155: 3.6900m (~225 LEDs)
/*  72 */ { strip(20), {{   0,   69}, {  70,  152}, { 153,  224}},  {{  5.104,  2.683, -1.658 }, {  4.299,  3.945, -1.397 }, {  5.185,  3.014, -0.178 }}}, // 1.155, 1.380, 1.155: 3.6900m (~225 LEDs)
/*  73 */ { strip(20), {{ 225,  325}, { 326,  426}, { 427,  512}},  {{  4.299,  3.945, -1.397 }, {  4.129,  4.342,  0.317 }, {  5.185,  3.014, -0.178 }}}, // 1.677, 1.677, 1.380: 4.7340m (~288 LEDs)
/*  74 */ { strip(20), {{ 513,  613}, { 614,  715}, { 716,  826}},  {{  5.185,  3.014, -0.178 }, {  4.129,  4.342,  0.317 }, {  4.854,  3.154,  1.577 }}}, // 1.677, 1.700, 1.799: 5.1760m (~314 LEDs)
/*  75 */ { strip(20), {{ 827,  933}, { 934, 1035}, {1036, 1140}},  {{  4.129,  4.342,  0.317 }, {  3.527,  4.342,  2.171 }, {  4.854,  3.154,  1.577 }}}, // 1.772, 1.700, 1.700: 5.1720m (~314 LEDs)
/*  76 */ { strip(21), {{   0,  101}, { 102,  202}, { 203,  313}},  {{  4.854,  3.154,  1.577 }, {  3.527,  4.342,  2.171 }, {  4.090,  3.014,  3.192 }}}, // 1.700, 1.677, 1.799: 5.1760m (~314 LEDs)
/*  77 */ { strip(21), {{ 314,  414}, { 415,  497}, { 498,  601}},  {{  3.527,  4.342,  2.171 }, {  2.657,  3.945,  3.657 }, {  4.090,  3.014,  3.192 }}}, // 1.677, 1.380, 1.677: 4.7340m (~288 LEDs)
// row 3
/*  78 */ { strip(13), {{ 602,  703}, { 704,  811}, { 812,  915}},  {{ -0.975,  4.342,  4.025 }, { -1.854,  5.104,  2.552 }, { -2.657,  3.945,  3.657 }}}, // 1.700, 1.799, 1.677: 5.1760m (~314 LEDs)
/*  79 */ { strip(13), {{ 916, 1023}, {1024, 1125}, {1126, 1229}},  {{ -2.657,  3.945,  3.657 }, { -1.854,  5.104,  2.552 }, { -3.527,  4.342,  2.171 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  80 */ { strip(22), {{   0,  101}, { 102,  208}, { 209,  313}},  {{ -1.854,  5.104,  2.552 }, { -2.552,  5.367,  0.829 }, { -3.527,  4.342,  2.171 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  81 */ { strip(22), {{ 314,  420}, { 421,  527}, { 528,  635}},  {{ -3.527,  4.342,  2.171 }, { -2.552,  5.367,  0.829 }, { -4.129,  4.342,  0.317 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  82 */ { strip(22), {{ 636,  737}, { 738,  839}, { 840,  949}},  {{ -2.552,  5.367,  0.829 }, { -3.000,  5.104, -0.975 }, { -4.129,  4.342,  0.317 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  83 */ { strip(15), {{ 513,  614}, { 615,  722}, { 723,  826}},  {{ -4.129,  4.342,  0.317 }, { -3.000,  5.104, -0.975 }, { -4.299,  3.945, -1.397 }}}, // 1.700, 1.799, 1.677: 5.1760m (~314 LEDs)
/*  84 */ { strip(15), {{ 827,  934}, { 935, 1036}, {1037, 1140}},  {{ -4.299,  3.945, -1.397 }, { -3.000,  5.104, -0.975 }, { -3.154,  4.342, -2.683 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  85 */ { strip(23), {{   0,  101}, { 102,  208}, { 209,  313}},  {{ -3.000,  5.104, -0.975 }, { -1.577,  5.367, -2.171 }, { -3.154,  4.342, -2.683 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  86 */ { strip(23), {{ 314,  420}, { 421,  527}, { 528,  635}},  {{ -3.154,  4.342, -2.683 }, { -1.577,  5.367, -2.171 }, { -1.577,  4.342, -3.829 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  87 */ { strip(23), {{ 636,  737}, { 738,  839}, { 840,  949}},  {{ -1.577,  5.367, -2.171 }, { -0.000,  5.104, -3.154 }, { -1.577,  4.342, -3.829 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  88 */ { strip(17), {{ 576,  677}, { 678,  785}, { 786,  889}},  {{ -1.577,  4.342, -3.829 }, { -0.000,  5.104, -3.154 }, { -0.000,  3.945, -4.521 }}}, // 1.700, 1.799, 1.677: 5.1760m (~314 LEDs)
/*  89 */ { strip(17), {{ 890,  997}, { 998, 1099}, {1100, 1203}},  {{  0.000,  3.945, -4.521 }, {  0.000,  5.104, -3.154 }, {  1.577,  4.342, -3.829 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  90 */ { strip(24), {{   0,  101}, { 102,  208}, { 209,  313}},  {{  0.000,  5.104, -3.154 }, {  1.577,  5.367, -2.171 }, {  1.577,  4.342, -3.829 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  91 */ { strip(24), {{ 314,  420}, { 421,  527}, { 528,  635}},  {{  1.577,  4.342, -3.829 }, {  1.577,  5.367, -2.171 }, {  3.154,  4.342, -2.683 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  92 */ { strip(24), {{ 636,  737}, { 738,  839}, { 840,  949}},  {{  1.577,  5.367, -2.171 }, {  3.000,  5.104, -0.975 }, {  3.154,  4.342, -2.683 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  93 */ { strip(19), {{ 513,  614}, { 615,  722}, { 723,  826}},  {{  3.154,  4.342, -2.683 }, {  3.000,  5.104, -0.975 }, {  4.299,  3.945, -1.397 }}}, // 1.700, 1.799, 1.677: 5.1760m (~314 LEDs)
/*  94 */ { strip(19), {{ 827,  934}, { 935, 1036}, {1037, 1140}},  {{  4.299,  3.945, -1.397 }, {  3.000,  5.104, -0.975 }, {  4.129,  4.342,  0.317 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/*  95 */ { strip(25), {{   0,  101}, { 102,  208}, { 209,  313}},  {{  3.000,  5.104, -0.975 }, {  2.552,  5.367,  0.829 }, {  4.129,  4.342,  0.317 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/*  96 */ { strip(25), {{ 314,  420}, { 421,  527}, { 528,  635}},  {{  4.129,  4.342,  0.317 }, {  2.552,  5.367,  0.829 }, {  3.527,  4.342,  2.171 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/*  97 */ { strip(25), {{ 636,  737}, { 738,  839}, { 840,  949}},  {{  2.552,  5.367,  0.829 }, {  1.854,  5.104,  2.552 }, {  3.527,  4.342,  2.171 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
/*  98 */ { strip(21), {{ 602,  703}, { 704,  811}, { 812,  915}},  {{  3.527,  4.342,  2.171 }, {  1.854,  5.104,  2.552 }, {  2.657,  3.945,  3.657 }}}, // 1.700, 1.799, 1.677: 5.1760m (~314 LEDs)
/*  99 */ { strip(21), {{ 916, 1023}, {1024, 1125}, {1126, 1229}},  {{  2.657,  3.945,  3.657 }, {  1.854,  5.104,  2.552 }, {  0.975,  4.342,  4.025 }}}, // 1.799, 1.700, 1.677: 5.1760m (~314 LEDs)
/* 100 */ { strip(26), {{   0,  101}, { 102,  208}, { 209,  313}},  {{  1.854,  5.104,  2.552 }, { -0.000,  5.367,  2.683 }, {  0.975,  4.342,  4.025 }}}, // 1.700, 1.772, 1.700: 5.1720m (~314 LEDs)
/* 101 */ { strip(26), {{ 314,  420}, { 421,  527}, { 528,  635}},  {{  0.975,  4.342,  4.025 }, { -0.000,  5.367,  2.683 }, { -0.975,  4.342,  4.025 }}}, // 1.772, 1.772, 1.772: 5.3160m (~322 LEDs)
/* 102 */ { strip(26), {{ 636,  737}, { 738,  839}, { 840,  949}},  {{ -0.000,  5.367,  2.683 }, { -1.854,  5.104,  2.552 }, { -0.975,  4.342,  4.025 }}}, // 1.700, 1.700, 1.772: 5.1720m (~314 LEDs)
// row 4
/* 103 */ { strip(27), {{   0,  100}, { 101,  208}, { 209,  313}},  {{ -0.000,  5.367,  2.683 }, { -0.886,  5.808,  1.219 }, { -1.854,  5.104,  2.552 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/* 104 */ { strip(27), {{ 314,  421}, { 422,  522}, { 523,  627}},  {{ -1.854,  5.104,  2.552 }, { -0.886,  5.808,  1.219 }, { -2.552,  5.367,  0.829 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/* 105 */ { strip(28), {{   0,   82}, {  83,  183}, { 184,  287}},  {{ -0.886,  5.808,  1.219 }, { -1.433,  5.808, -0.466 }, { -2.552,  5.367,  0.829 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/* 106 */ { strip(28), {{ 288,  388}, { 389,  496}, { 497,  601}},  {{ -2.552,  5.367,  0.829 }, { -1.433,  5.808, -0.466 }, { -3.000,  5.104, -0.975 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/* 107 */ { strip(28), {{ 602,  709}, { 710,  810}, { 811,  915}},  {{ -3.000,  5.104, -0.975 }, { -1.433,  5.808, -0.466 }, { -1.577,  5.367, -2.171 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/* 108 */ { strip(29), {{   0,   82}, {  83,  183}, { 184,  287}},  {{ -1.433,  5.808, -0.466 }, { -0.000,  5.808, -1.507 }, { -1.577,  5.367, -2.171 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/* 109 */ { strip(29), {{ 288,  388}, { 389,  496}, { 497,  601}},  {{ -1.577,  5.367, -2.171 }, { -0.000,  5.808, -1.507 }, { -0.000,  5.104, -3.154 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/* 110 */ { strip(29), {{ 602,  709}, { 710,  810}, { 811,  915}},  {{  0.000,  5.104, -3.154 }, {  0.000,  5.808, -1.507 }, {  1.577,  5.367, -2.171 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/* 111 */ { strip(30), {{   0,   82}, {  83,  183}, { 184,  287}},  {{  0.000,  5.808, -1.507 }, {  1.433,  5.808, -0.466 }, {  1.577,  5.367, -2.171 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/* 112 */ { strip(30), {{ 288,  388}, { 389,  496}, { 497,  601}},  {{  1.577,  5.367, -2.171 }, {  1.433,  5.808, -0.466 }, {  3.000,  5.104, -0.975 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/* 113 */ { strip(30), {{ 602,  709}, { 710,  810}, { 811,  915}},  {{  3.000,  5.104, -0.975 }, {  1.433,  5.808, -0.466 }, {  2.552,  5.367,  0.829 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/* 114 */ { strip(31), {{   0,   82}, {  83,  183}, { 184,  287}},  {{  1.433,  5.808, -0.466 }, {  0.886,  5.808,  1.219 }, {  2.552,  5.367,  0.829 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
/* 115 */ { strip(31), {{ 288,  388}, { 389,  496}, { 497,  601}},  {{  2.552,  5.367,  0.829 }, {  0.886,  5.808,  1.219 }, {  1.854,  5.104,  2.552 }}}, // 1.677, 1.799, 1.700: 5.1760m (~314 LEDs)
/* 116 */ { strip(31), {{ 602,  709}, { 710,  810}, { 811,  915}},  {{  1.854,  5.104,  2.552 }, {  0.886,  5.808,  1.219 }, { -0.000,  5.367,  2.683 }}}, // 1.799, 1.677, 1.700: 5.1760m (~314 LEDs)
/* 117 */ { strip(27), {{ 628,  710}, { 711,  811}, { 812,  915}},  {{  0.886,  5.808,  1.219 }, { -0.886,  5.808,  1.219 }, { -0.000,  5.367,  2.683 }}}, // 1.380, 1.677, 1.677: 4.7340m (~288 LEDs)
// row 5
/* 118 */ { strip(27), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{  0.886,  5.808,  1.219 }, { -0.000,  6.000,  0.000 }, { -0.886,  5.808,  1.219 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/* 119 */ { strip(28), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{ -0.886,  5.808,  1.219 }, { -0.000,  6.000,  0.000 }, { -1.433,  5.808, -0.466 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/* 120 */ { strip(29), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{ -1.433,  5.808, -0.466 }, { -0.000,  6.000,  0.000 }, { -0.000,  5.808, -1.507 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/* 121 */ { strip(30), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{ -0.000,  5.808, -1.507 }, { -0.000,  6.000,  0.000 }, {  1.433,  5.808, -0.466 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
/* 122 */ { strip(31), {{ 916,  985}, { 986, 1055}, {1056, 1140}},  {{  1.433,  5.808, -0.466 }, { -0.000,  6.000,  0.000 }, {  0.886,  5.808,  1.219 }}}, // 1.155, 1.155, 1.380: 3.6900m (~225 LEDs)
};
// TOTAL LEDS: 36342
