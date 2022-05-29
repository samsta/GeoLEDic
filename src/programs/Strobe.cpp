#include "Strobe.hpp"
#include "colorutils.h"
#include <algorithm>

Strobe::Strobe(const DomeWrapper& dome):
   m_dome(dome)
{
}   

void Strobe::runProgram()
{
   for (Triangle& t: m_dome)
   {
      std::fill(t.firstLed(), t.firstLed() + t.size(), CRGB::Black);
   }

   CRGB color(CHSV(getHue()<<1, getSaturation()<<1, 255));
   for (unsigned t_ix = 0; t_ix < m_dome.size(); t_ix++)
   {
      Triangle& t(m_dome[t_ix]);

      if (note(NOTE_C0))
      {
         std::fill(t.firstLed(), t.firstLed() + t.size(), color);
      }
   }
}
