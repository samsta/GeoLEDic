#include "FlexibleNoise.hpp"
#include "colorutils.h"
#include "ProgramHelpers.hpp"
#include <math.h>
#include <algorithm>

FlexibleNoise::FlexibleNoise(const DomeWrapper& dome):
   m_dome(dome),
   m_x(0),
   m_y(0),
   m_t(0),
   m_h_offset(0)
{
   memset(m_noise, 0, sizeof(m_noise));
}

void FlexibleNoise::calcNoise()
{
   unsigned sx = unsigned(getScaleX()) << 9;
   unsigned sy = unsigned(getScaleY()) << 9;
   for (unsigned h = 0; h < NUM_H; h++)
   {
      for (unsigned v = 0; v < NUM_V; v++)
      {
         int noise = 128 + inoise16_raw(m_x + h*sx, m_y + v*sy, m_t)/100;
         noise = std::max(std::min(noise, 255), 0);
         if (isXYSwapped())
         {
            (&m_noise[0][0])[h + v * NUM_H] = noise;
         }
         else
         {
            m_noise[h][v] = noise;
         }
      }
   }
}

void FlexibleNoise::runProgram()
{
   setDecayRate(getKeysDecay());
   calcNoise();

   CRGBPalette16 palette =
   {
      CHSV(getHue0()*2, getSaturation0()*2, getBrightness0()*2),
      CHSV(getHue1()*2, getSaturation1()*2, getBrightness1()*2),
      CHSV(getHue2()*2, getSaturation2()*2, getBrightness2()*2),
      CHSV(getHue3()*2, getSaturation3()*2, getBrightness3()*2),
      CHSV(getHue4()*2, getSaturation4()*2, getBrightness4()*2),
      CHSV(getHue5()*2, getSaturation5()*2, getBrightness5()*2),
      CHSV(getHue6()*2, getSaturation6()*2, getBrightness6()*2),
      CHSV(getHue7()*2, getSaturation7()*2, getBrightness7()*2),

      CRGB::Black,
      CRGB::Black,
      CRGB::Black,
      CRGB::Black,
      CRGB::Black,
      CRGB::Black,
      CRGB::Black,
      CRGB::Black
   };

   if (isBlackStripes())
   {
      for (int k = 7; k > 0; k--)
      {
         std::swap(palette[k*2], palette[k]);
      }
   }

   for (unsigned t_ix = 0; t_ix < m_dome.size(); t_ix++)
   {
      Triangle& t(m_dome[t_ix]);
      uint8_t velocity = getTriangleValue(t_ix);
      
      for (unsigned k = 0; k < 3; k++)
      {
         const Edge& e(t.edge(k));
         const Vertex& c0(t.corner(k));
         const Vertex& c1(t.corner((k + 1) % 3));

         int led_ix = 0;
         for (CRGB& led: e)
         {
            float v = float(interpolateTheta(c0, c1, led_ix, e.size())) / (Vertex::NUM_THETA_STEPS/(NUM_V-1));
            float h = float(interpolatePhi(c0, c1, led_ix, e.size())) / (Vertex::NUM_PHI_STEPS/(NUM_H-1));
            h += m_h_offset;
            if (h  >= NUM_H ) h = h - NUM_H;

            uint8_t index = interpolate(m_noise, h, v);
            if (not isBlackStripes())
            {
               // we only use the bottom half of the palette
               index /= 2;
            }
            
            if (velocity > 0)
            {
               CHSV inverted = rgb2hsv_approximate(ColorFromPalette(palette, index, velocity*2));
               inverted.hue += 128;
               led = inverted;

               if (velocity < 255/3)
               {
                  led = blend(ColorFromPalette(palette, index, getBrightness()), 
                              led, 
                              velocity*3);
               }

            }
            else
            {
               led = ColorFromPalette(palette, index, getBrightness());
            }
            
            led_ix++;
         }
      }
   }
   m_x += unsigned(getSpeedX()) << 6;
   m_y += unsigned(getSpeedY()) << 6;
   m_t += unsigned(getSpeedT()) << 6;
   m_h_offset += float(getRotationSpeed())/100.f;
   if (m_h_offset >= NUM_H) m_h_offset = 0;
}
