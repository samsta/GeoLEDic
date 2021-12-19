#include "OrganicLine.hpp"
#include "colorutils.h"
#include "Palettes.hpp"

OrganicLine::OrganicLine(const DomeWrapper& dome):
   m_dome(dome),
   m_time(0)
{
}   

void OrganicLine::runProgram()
{
   CRGBPalette16 hue_palette =
   {
      CRGB::Black,
      CHSV(getHue0()*2, 255, 255),
      CRGB::Black,
      CRGB::Black,

      CRGB::Black,
      CHSV(getHue1()*2, 255, isDisableHue1() ? 0 : 255),
      CRGB::Black,
      CRGB::Black,

      CRGB::Black,
      CHSV(getHue2()*2, 255, 255),
      CRGB::Black,
      CRGB::Black,

      CRGB::Black,
      CHSV(getHue3()*2, 255, isDisableHue3() ? 0 : 255),
      CRGB::Black,
      CRGB::Black,
   };
   
   // MIN and MAX PHI define the extent of the thin line between the first two
   //  rows of triangles
   const unsigned MIN_PHI = 178;
   const unsigned MAX_PHI = 846;
   uint8_t line_noise[MAX_PHI - MIN_PHI + 1];
   for (unsigned i = 0; i < sizeof(line_noise); i++)
   {
      line_noise[i] = 128 + inoise16_raw((i * (140 - getSize())) << 4, m_time)/100;
   }
   
   int line_width = 2 * getLineWidth();

   for (unsigned t_ix = 0; t_ix < m_dome.size(); t_ix++)
   {
      Triangle& t(m_dome[t_ix]);

      if (line_width == 0)
      {
         // only the first two lines, and only even triangles
         if (t_ix > 50 or t_ix & 1)
         {
            std::fill(t.begin(), t.end(), CRGB::Black);
            continue;
         } 
      }

      for (unsigned k = 0; k < 3; k++)
      {
         const Edge& e(t.edge(k));
         const Vertex& c0(t.corner(k));
         const Vertex& c1(t.corner((k + 1) % 3));

         if (line_width == 0)
         {
            // on first row, the first edge is the horizontal one, on the second row it's the last edge
            if ((t_ix <= 24 and k != 0) or
                (t_ix >  24 and k != 2))
            {
               std::fill(e.begin(), e.end(), CRGB::Black);
               continue;
            }
         }

         int led_ix = 0;
         for (CRGB& led: e)
         {
            unsigned brightness = getBrightness();
            if (line_width > 0)
            {
               const unsigned THETA_BASE_LINE = 46;
               int v = int(interpolateTheta(c0, c1, led_ix, e.size()) - THETA_BASE_LINE);
               if (v < 0) v = -v;
               
               if (v > line_width)
               {
                  led = CRGB::Black;
                  led_ix++;
                  continue;
               }
               
               const int half_line_width = line_width / 2;
               if (v > half_line_width)
               {
                  brightness = (brightness * (line_width - v)) / (half_line_width + 1);
               }
            }

            unsigned h = interpolatePhi(c0, c1, led_ix, e.size());
            if (h < MIN_PHI)
            {
               h = MIN_PHI - h;
            }
            else
            {
               h = h - MIN_PHI;
               if (h >= sizeof(line_noise))
               {
                  h = sizeof(line_noise) - (h - sizeof(line_noise)) - 1;
               }
            }

            if (isUseHues())
            {
               led = ColorFromPalette(hue_palette, line_noise[h], brightness);
            }
            else
            {
               led = ColorFromPalette(PartyStripes_p, line_noise[h], brightness);
            }
            led_ix++;
         }
      }
   }
   
   m_time += getSpeed()<<4;
}
