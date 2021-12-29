#include "BetterEdgy.hpp"
#include "colorutils.h"
#include <algorithm>

BetterEdgy::BetterEdgy(const DomeWrapper& dome):
   m_dome(dome)
{
}   

void BetterEdgy::runProgram()
{
   const bool keyed = isAnyTriangleSet();
   const bool key_colors = keyed and isUseKeysColor();

   const uint8_t center_hue = key_colors ? getKeysHue()*2           : getHue()*2;
   const uint8_t range_hue  = key_colors ? getKeysHueRange()        : getHueRange();
   const uint8_t center_sat = key_colors ? getKeysSaturation()*2    : getSaturation()*2;
   const uint8_t range_sat  = key_colors ? getKeysSaturationRange() : getSaturationRange();
   const uint8_t min_sat = center_sat < range_sat ? 0 : center_sat - range_sat;
   const uint8_t max_sat = (255 - center_sat < range_sat) ? 255 : center_sat + range_sat;

   const uint8_t base_edge_rate = keyed ? 0 : getEdgeRate();

   for (unsigned t_ix = 0; t_ix < m_dome.size(); t_ix++)
   {
      Triangle& t(m_dome[t_ix]);

      uint8_t edge_rate = getTriangleValue(t_ix) ? getKeyboardEdgeRate()*2 : base_edge_rate;
      uint8_t brightness = getTriangleValue(t_ix) ? getTriangleValue(t_ix)*2 : getBrightness();

      for (unsigned e_ix = 0; e_ix < 3; e_ix++)
      {
         const Edge& e(t.edge(e_ix));
         
         if (random16(512) < edge_rate)
         {
            // hue is circular, so it wraps around...
            uint8_t hue = random8(0, 2*range_hue) + center_hue - range_hue;
            
            std::fill(e.begin(), e.end(), CHSV(hue,
                                               random8(min_sat, max_sat),
                                               brightness));
         }
         else
         {
            CRGB new_col = *e.begin();
            fadeToBlackBy(&new_col, 1, getDecay());
            std::fill(e.begin(), e.end(), new_col);
         }
      }
   }
}
