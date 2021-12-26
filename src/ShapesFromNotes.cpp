#include "ShapesFromNotes.hpp"
#include <algorithm>

namespace {

/*
 These data structures would be way simpler and less error prone if I could just use std::vector, but I really want this data
 to reside in Flash to avoid wasting precious RAM. Vectors are off the table for Flash.
 */

const uint8_t TERMINATOR = 255;

const uint8_t NUM_SHAPES_PENTAGON0 = 6;
const uint8_t NUM_TRIANGLES_PER_SHAPE_PENTAGON0 = 28;
const uint8_t pentagon0[NUM_SHAPES_PENTAGON0][NUM_TRIANGLES_PER_SHAPE_PENTAGON0] =
{
   {118, 119, 120, 121, 122, TERMINATOR},
   {103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, TERMINATOR},
   { 78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, TERMINATOR},
   { 52,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65, 66, 67, 68, 69, 70, 71, 72,  73,  74,  75, 76, 77, TERMINATOR},
   { 25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39, 40, 41, 42, 43, 44, 45, 46,  47,  48,  49, 50, 51, TERMINATOR},
   {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14, 15, 16, 17, 18, 19, 20, 21,  22,  23,  24, TERMINATOR}
};

const uint8_t NUM_SHAPES_PENTAGON1 = 10;
const uint8_t NUM_TRIANGLES_PER_SHAPE_PENTAGON1 = 19;
const uint8_t pentagon1[NUM_SHAPES_PENTAGON1][NUM_TRIANGLES_PER_SHAPE_PENTAGON1] =
{
   {29, 30, 31, 57, 58, TERMINATOR},
   { 2,  3,  4, 5,   6, 32, 33, 60,  59,  84,  83,  56,  55,  27,  28, TERMINATOR},
   { 0,  1,  7, 8,  34, 35, 62, 61,  86,  85, 107, 106,  82,  81,  54,  53, 25, 26, TERMINATOR},
   { 9, 10, 36, 37, 64, 63, 88, 87, 109, 108, 120, 119, 105, 104,  80,  79, 52, TERMINATOR},
   {11, 12, 38, 39, 65, 66, 89, 90, 110, 111, 121, 122, 118, 117, 103, 102, 78, TERMINATOR},
   {13, 14, 40, 41, 67, 68, 91, 92, 112, 113, 114, 115, 116, 100, 101, TERMINATOR},
   {15, 16, 42, 43, 69, 70, 93, 94,  95,  96,  97,  98,  99, TERMINATOR},
   {17, 18, 44, 45, 71, 72, 73, 74,  75,  76,  77, TERMINATOR},
   {19, 20, 46, 47, 48, 49, 50, 51, TERMINATOR},
   {21, 22, 23, 24, TERMINATOR}
};

const uint8_t NUM_SHAPES_PENTAGON2 = 7;
const uint8_t NUM_TRIANGLES_PER_SHAPE_PENTAGON2 = 24;
const uint8_t pentagon2[NUM_SHAPES_PENTAGON2][NUM_TRIANGLES_PER_SHAPE_PENTAGON2] =
{
   {37, 38, 39, 65, 64, TERMINATOR},
   {10, 11, 12, 13, 14, 40, 41, 67,  66,  89,  88,  63,  62,  35,  36, TERMINATOR},
   {15, 16, 42, 43, 69, 68, 91, 90, 110, 109,  87,  86,  61,  60,  33, 34,  8,  9, TERMINATOR},
   {17, 18, 44, 45, 71, 70, 93, 92, 112, 111, 121, 120, 108, 107,  85, 84, 59, 58, 31, 32,  6, 7, TERMINATOR},
   {19, 20, 46, 47, 72, 73, 94, 95, 113, 114, 122, 118, 119, 105, 106, 82, 83, 56, 57, 29, 30, 4, 5, TERMINATOR},
   {21, 22, 48, 49, 74, 75, 96, 97, 115, 116, 117, 103, 104,  80,  81, 54, 55, 27, 28,  2,  3, TERMINATOR},
   {23, 24, 50, 51, 76, 77, 98, 99, 100, 101, 102,  78,  79,  52,  53, 25, 26,  0,  1, TERMINATOR},
};

const uint8_t NUM_SHAPES_PENTAGON3 = 10;
const uint8_t NUM_TRIANGLES_PER_SHAPE_PENTAGON3 = 19;
const uint8_t pentagon3[NUM_SHAPES_PENTAGON3][NUM_TRIANGLES_PER_SHAPE_PENTAGON3] =
{
   { 45,  46,  47,  72,  71, TERMINATOR},
   { 18,  19,  20,  21,  22,  48,  49,  74,  73,  94, 93, 70, 69, 43, 44, TERMINATOR},
   { 23,  24,  50,  51,  76,  75,  96,  95, 113, 112, 92, 91, 68, 67, 41, 42, 16, 17, TERMINATOR},
   { 77,  98,  97, 115, 114, 122, 121, 111, 110,  90, 89, 66, 65, 39, 40, 14, 15, TERMINATOR},
   { 99, 100, 116, 117, 118, 119, 120, 108, 109,  87, 88, 63, 64, 37, 38, 12, 13, TERMINATOR},
   {101, 102, 103, 104, 105, 106, 107,  85,  86,  61, 62, 35, 36, 10, 11, TERMINATOR},
   { 78,  79,  80,  81,  82,  83,  84,  59,  60,  33, 34,  8,  9, TERMINATOR},
   { 52,  53,  54,  55,  56,  57,  58,  31,  32,   6,  7, TERMINATOR},
   { 25,  26,  27,  28,  29,  30,   4,   5, TERMINATOR},
   {  0,   1,   2,   3, TERMINATOR}
};

const uint8_t NUM_BLOBS = 52;
const uint8_t NUM_ELEMENTS_PER_BLOB = 6; // we have hexagons and pentagons, so maximum 6 triangles
const uint8_t blobs[NUM_BLOBS][NUM_ELEMENTS_PER_BLOB] =
{
   // first row
   { 0,  1,  2, 26, 27, 28},
   { 2,  3,  4, 28, 29, 30},
   { 4,  5,  6, 30, 31, 32},
   { 6,  7,  8, 32, 33, 34},
   { 8,  9, 10, 34, 35, 36},
   {10, 11, 12, 36, 37, 38},
   {12, 13, 14, 38, 39, 40},
   {14, 15, 16, 40, 41, 42},
   {16, 17, 18, 42, 43, 44},
   {18, 19, 20, 44, 45, 46},
   {20, 21, 22, 46, 47, 48},
   {22, 23, 24, 48, 49, 50},
   // second row
   {25, 26, 27, 53, 54, 55},
   {27, 28, 29, 55, 56, 57},
   {29, 30, 31, 57, 58, TERMINATOR}, // only 5 in a pentagon
   {31, 32, 33, 58, 59, 60},
   {33, 34, 35, 60, 61, 62},
   {35, 36, 37, 62, 63, 64},
   {37, 38, 39, 64, 65, TERMINATOR}, // only 5 in a pentagon
   {39, 40, 41, 65, 66, 67},
   {41, 42, 43, 67, 68, 69},
   {43, 44, 45, 69, 70, 71},
   {45, 46, 47, 71, 72, TERMINATOR}, // only 5 in a pentagon
   {47, 48, 49, 72, 73, 74},
   {49, 50, 51, 74, 75, 76},
   // third row
   {52, 53, 54, 79, 80, 81},
   {54, 55, 56, 81, 82, 83},
   {56, 57, 58, 59, 83, 84},
   {59, 60, 61, 84, 85, 86},
   {61, 62, 63, 86, 87, 88},
   {63, 64, 65, 66, 88, 89},
   {66, 67, 68, 89, 90, 91},
   {68, 69, 70, 91, 92, 93},
   {70, 71, 72, 73, 93, 94},
   {73, 74, 75, 94, 95, 96},
   {75, 76, 77, 96, 97, 98},
   // fourth row
   { 78,  79,  80, 102, 103, 104},
   { 80,  81,  82, 104, 105, 106},
   { 82,  83,  84,  85, 106, 107},
   { 85,  86,  87, 107, 108, 109},
   { 87,  88,  89,  90, 109, 110},
   { 90,  91,  92, 110, 111, 112},
   { 92,  93,  94,  95, 112, 113},
   { 95,  96,  97, 113, 114, 115},
   { 97,  98,  99, 100, 115, 116},
   {100, 101, 102, 103, 116, 117},
   // fifth row
   {117, 118, 119, 103, 104, 105},
   {105, 106, 107, 108, 119, 120},
   {108, 109, 110, 111, 120, 121},
   {111, 112, 113, 114, 121, 122},
   {114, 115, 116, 117, 118, 122},
   // top pentagon
   {118, 119, 120, 121, 122, TERMINATOR} // only 5 in a pentagon
};

bool nonzero(uint8_t v)
{
   return v != 0;
}

}

ShapesFromNotes::ShapesFromNotes()
{
   std::fill(m_triangles, m_triangles+DOME_NUM_TRIANGLES, 0);
}

uint8_t ShapesFromNotes::getTriangleValue(uint8_t note) const
{
   if (note >= DOME_NUM_TRIANGLES) return 0;
   return m_triangles[note];
}

bool ShapesFromNotes::isAnyTriangleSet() const
{
   return std::any_of(m_triangles, m_triangles + DOME_NUM_TRIANGLES, nonzero);
}

void ShapesFromNotes::noteOn(uint8_t note, uint8_t velocity, uint8_t channel)
{
   if (channel == 1)
   {
      if (note < NOTE_C3)
      {
         setPentagon(note, velocity);
      }
      else
      {
         setBlob(note, velocity);
      }
   }
   else
   {
      associateShapeWithTriangle(SHAPE_TRIANGLE, note, note, velocity);
   }
}

void ShapesFromNotes::noteOff(uint8_t note, uint8_t channel)
{
   noteOn(note, 0, channel);
}

void ShapesFromNotes::associateShapeWithTriangle(Shape shape_type, uint8_t shape_id, uint8_t triangle_ix, uint8_t velocity)
{
   if (triangle_ix >= DOME_NUM_TRIANGLES) return;
   
   if (velocity == 0)
   {
      dissociateShapeFromTriangle(shape_type, shape_id, triangle_ix);
      return;
   }
   
   m_triangle_associations[triangle_ix].set({shape_type, shape_id, velocity});
   m_triangles[triangle_ix] = m_triangle_associations[triangle_ix].velocity();
}

void ShapesFromNotes::dissociateShapeFromTriangle(Shape shape_type, uint8_t shape_id, uint8_t triangle_ix)
{
   if (m_triangle_associations.count(triangle_ix) == 0) return;
   
   m_triangle_associations[triangle_ix].clear(shape_type, shape_id);
   m_triangles[triangle_ix] = m_triangle_associations[triangle_ix].velocity();
   
   // if the velocity is 0, then there's no association remaining, so we can remove the item
   if (m_triangle_associations[triangle_ix].velocity() == 0)
   {
      m_triangle_associations.erase(triangle_ix);
   }
}

void ShapesFromNotes::setBlob(uint8_t note, uint8_t velocity)
{
   if (note < NOTE_C3) return;
   note -= NOTE_C3;
   
   if (note >= NUM_BLOBS) return;
      
   for (uint8_t index = 0; index < NUM_ELEMENTS_PER_BLOB; index++)
   {
      uint8_t triangle_ix = blobs[note][index];
      if (triangle_ix == TERMINATOR) break;
      associateShapeWithTriangle(SHAPE_BLOB, note, triangle_ix, velocity);
   }
}

void ShapesFromNotes::setPentagon(uint8_t note, uint8_t velocity)
{
   const uint8_t* pentagon = nullptr;
   uint8_t pentagon_index = 0;
   uint8_t num_triangles_per_shape = 0;
   if (note >= NOTE_Cneg1 && note < NOTE_Cneg1 + NUM_SHAPES_PENTAGON0)
   {
      pentagon_index = note - NOTE_Cneg1;
      pentagon = pentagon0[0];
      num_triangles_per_shape = NUM_TRIANGLES_PER_SHAPE_PENTAGON0;
   }
   else if (note >= NOTE_C0 && note < NOTE_C0 + NUM_SHAPES_PENTAGON1)
   {
      pentagon_index = note -NOTE_C0;
      pentagon = pentagon1[0];
      num_triangles_per_shape = NUM_TRIANGLES_PER_SHAPE_PENTAGON1;
   }
   else if (note >= NOTE_C1 && note < NOTE_C1 + NUM_SHAPES_PENTAGON2)
   {
      pentagon_index = note - NOTE_C1;
      pentagon = pentagon2[0];
      num_triangles_per_shape = NUM_TRIANGLES_PER_SHAPE_PENTAGON2;
   }
   else if (note >= NOTE_C2 && note < NOTE_C2 + NUM_SHAPES_PENTAGON3)
   {
      pentagon_index = note - NOTE_C2;
      pentagon = pentagon3[0];
      num_triangles_per_shape = NUM_TRIANGLES_PER_SHAPE_PENTAGON3;
   }
   else
   {
      return;
   }
   
   const uint8_t* triangle = pentagon + pentagon_index * num_triangles_per_shape;
   while(*triangle != TERMINATOR)
   {
      associateShapeWithTriangle(SHAPE_PENTAGON, note, *triangle, velocity);
      triangle++;
   }
}

ShapesFromNotes::Association::Association():
   shape_type(), shape_id(), velocity()
{
}

ShapesFromNotes::Association::Association(Shape channel, uint8_t index, uint8_t velocity):
   shape_type(channel), shape_id(index), velocity(velocity)
{
}

ShapesFromNotes::Associations::Associations(): num_associations(0)
{
}

void ShapesFromNotes::Associations::set(const Association& setter)
{
   if (num_associations == MAX_ASSOCIATIONS) return; // can't set another one, 'polyphony' exhausted
   
   // reject duplicates
   for (unsigned k = 0; k < num_associations; k++)
   {
      if (associations[k].shape_id == setter.shape_id and associations[k].shape_type == setter.shape_type)
      {
         return;
      }
   }

   associations[num_associations] = setter;
   num_associations++;
}

void ShapesFromNotes::Associations::clear(Shape shape_type, uint8_t shape_id)
{
   bool found = false;
   for (unsigned k = 0; k < num_associations; k++)
   {
      if (associations[k].shape_type == shape_type and associations[k].shape_id == shape_id)
      {
         found = true;
         num_associations--;
         /*
          If it's the last one, we're done and there's no need to compact the list
          */
         if (k == num_associations) break;
      }
      
      if (found)
      {
         // we have removed an item, so shift the remainder of the list
         associations[k] = associations[k+1];
      }
   }
}

uint8_t ShapesFromNotes::Associations::velocity() const
{
   if (num_associations == 0 or num_associations > MAX_ASSOCIATIONS) return 0; // not set, or invalid
   
   // return the velocity of the latest setter
   return associations[num_associations-1].velocity;
}
