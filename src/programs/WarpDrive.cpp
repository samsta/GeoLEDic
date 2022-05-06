#include "WarpDrive.hpp"
#include "colorutils.h"
#include "Palettes.hpp"

WarpDrive::WarpDrive(const DomeWrapper& dome):
   m_dome(dome),
   m_rings()
{
   std::fill_n(m_rings, sizeof(m_rings)/sizeof(*m_rings), CRGB::Black);
}   

bool WarpDrive::findKey(uint8_t& hue, uint8_t& saturation)
{
   const unsigned NUM_NOTES = 24;
   for (unsigned n = NOTE_C1; n < NOTE_C1 + NUM_NOTES; n++)
   {
      uint8_t velocity = note(n);
      if (velocity)
      {
         saturation = 255 - velocity * 2;
         hue = 255*(n - NOTE_C1)/NUM_NOTES;
         return true;
      }
   }
   return false;
}


void WarpDrive::shiftAndFillRings()
{
   const uint8_t center_hue = getHue()*2;
   const uint8_t range_hue  = getHueRange();
   const uint8_t center_sat = getSaturation()*2;
   const uint8_t range_sat  = getSaturationRange();
   const uint8_t min_sat = center_sat < range_sat ? 0 : center_sat - range_sat;
   const uint8_t max_sat = (255 - center_sat < range_sat) ? 255 : center_sat + range_sat;
   
   uint8_t advance_width = getSpeed();
   uint8_t fill_width = getThickness() + 1;

   CRGB* shift_from = m_rings + (isReverse() ? advance_width : 0);
   CRGB* shift_to   = m_rings + (isReverse() ? 0             : advance_width);

   CRGB* fill_from  = m_rings + (isReverse() ? NUM_DISTANCE_STEPS - fill_width : 0);
   CRGB* blank_advance_from = m_rings + (isReverse() ? NUM_DISTANCE_STEPS - advance_width : 0);

   memmove(shift_to, shift_from, sizeof(m_rings) - advance_width*sizeof(*m_rings));
   std::fill_n(blank_advance_from,
               advance_width,
               CRGB::Black);

   uint8_t hue, sat;
   if (findKey(hue, sat))
   {
      std::fill_n(fill_from, 
                  fill_width,
                  CHSV(hue,
                       sat,
                       getBrightness()));

   }
   else if (random8() < getSpawnRate())
   {
      // hue is circular, so it wraps around...
      uint8_t hue = random8(0, 2*range_hue) + center_hue - range_hue;
      
      std::fill_n(fill_from, 
                  fill_width,
                  CHSV(hue,
                       random8(min_sat, max_sat),
                       getBrightness()));
   }
}

void WarpDrive::runProgram()
{
   shiftAndFillRings();

   float theta = M_PI_2 * getCenterTheta() / 127;
   float phi   = M_PI * (getCenterPhi() - 64) / 64;

   float scaler = 147 - getRadius();

   Vertex warp_center(
     -6.0 * sin(phi) * cos(theta),
      6.0 * sin(theta),
     -6.0 * cos(phi) * cos(theta)
   );

   for (unsigned t_ix = 0; t_ix < m_dome.size(); t_ix++)
   {
      Triangle& t(m_dome[t_ix]);

      for (unsigned e_ix = 0; e_ix < 3; e_ix++)
      {
         const Edge& e(t.edge(e_ix));
         const Vertex& c0(t.corner(e_ix));
         const Vertex& c1(t.corner((e_ix+1) % 3));

         if (isSkipConcentric())
         {
            // skip concentric edges
            if (abs(distance(c0, warp_center) - distance(c1, warp_center)) < 0.2)
            {
               std::fill(e.begin(), e.end(), CRGB::Black);
               continue;
            }
         }

         int led_ix = 0;
         for (CRGB& led: e)
         {
            unsigned d = unsigned(distance(warp_center, interpolate(c0, c1, led_ix, e.size())) * scaler);
            if (d >= sizeof(m_rings)/sizeof(*m_rings))
            {
               led = CRGB::Black;
            }
            else
            {
               led = m_rings[d];
            }
            led_ix++;
         }
      }
   }
}
