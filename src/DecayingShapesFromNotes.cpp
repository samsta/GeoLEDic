#include "DecayingShapesFromNotes.hpp"

DecayingShapesFromNotes::DecayingShapesFromNotes():
   m_decay_rate(10),
   m_any_triangle_set(false)
{
   std::fill_n(m_decaying_triangles, DOME_NUM_TRIANGLES, 0);
}

void DecayingShapesFromNotes::run()
{
   ShapesFromNotes::run();
   bool triangle_set = false;
   for (uint8_t k = 0; k < DOME_NUM_TRIANGLES; k++)
   {
      uint8_t vel = ShapesFromNotes::getTriangleValue(k);

      if (vel == m_decaying_triangles[k])
      {
         triangle_set |= vel > 0;
         continue;
      }

      if (vel > 0)
      {
         m_decaying_triangles[k] = vel;
      }
      else
      {
         m_decaying_triangles[k] = std::max(0, int(m_decaying_triangles[k]) - m_decay_rate);
      }
      
      if (m_decaying_triangles[k] > 0)
      {
         triangle_set = true;
      }
   }
   m_any_triangle_set = triangle_set;
}

void DecayingShapesFromNotes::setDecayRate(uint8_t rate)
{
   m_decay_rate = rate;
}

uint8_t DecayingShapesFromNotes::getTriangleValue(uint8_t note) const
{
   return m_decaying_triangles[note];
}

bool DecayingShapesFromNotes::isAnyTriangleSet() const
{
   return m_any_triangle_set;
}
