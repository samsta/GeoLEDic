#include "Fire.hpp"
#include "Palettes.hpp"
#include "ProgramHelpers.hpp"
#include "colorutils.h"
#include <math.h>

namespace {

const TProgmemRGBPalette16* palette(Fire::Palette palette)
{
   switch (palette)
   {
   default:
   case Fire::Palette_Fire:
      return &HeatColors_p;
   case Fire::Palette_Ocean:
      return &OceanFromBlack_p;
   case Fire::Palette_Cloud:
      return &Clouds_p;
   case Fire::Palette_Forest:
      return &BlackToForestColors_p;
   case Fire::Palette_Lava:
      return &LavaColors_p;
   case Fire::Palette_Rainbow:
      return &BlackToRainbowColors_p;
   case Fire::Palette_Party:
      return &BlackToPartyColors_p;
   }
}

}

Fire::Fire(const DomeWrapper& dome):
   m_dome(dome),
   m_h_offset(0)
{
   memset(m_heat, 0, sizeof(m_heat));
}

void Fire::calcFire()
{
   int cooling = getCooling();
   int sparking = getSparking();
   for (unsigned h = 0; h < NUM_H; h++)
   {
      // Step 1.  Cool down every cell a little
      for(unsigned v = 0; v < NUM_V; v++)
      {
         m_heat[h][v] = qsub8(m_heat[h][v],  random8(0, ((cooling * 10) / NUM_V) + 2));
      }
        
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little sideways
      for (unsigned v = NUM_V-1; v >= 2; v--)
      {
         unsigned hh = h;
         uint8_t r = random8();
         if (r < 8)
         {
            hh += r & 1 ? 1 : -1;
            hh = (hh + NUM_H) % NUM_H;
         }
         m_heat[h][v] = (m_heat[h][v - 1] + m_heat[h][v - 2] + m_heat[hh][v - 1]) / 3;
      }
          
      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if(random8() < sparking)
      {
         int v = random8(7);
         m_heat[h][v] = qadd8(m_heat[h][v], random8(160,255));
      }
      
      // ignite new sparks on note press.
      // ensure note stay in the same place even if rotation is on
      uint8_t hnote = NUM_H + h - m_h_offset;
      if (hnote >= NUM_H) hnote -= NUM_H;
      uint8_t n = hnote + NOTE_C1;
      if (note(n))
      {
         uint8_t heat = note(n)*2;
         m_heat[h][0] = heat;
         m_heat[h][1] = heat;
      }
   }
}

void Fire::runProgram()
{
   calcFire();
   auto* pal = palette(getPalette());

   for (Triangle& t: m_dome)
   {
      for (unsigned k = 0; k < 3; k++)
      {
         const Edge& e(t.edge(k));
         const Vertex& c0(t.corner(k));
         const Vertex& c1(t.corner((k + 1) % 3));

         int led_ix = 0;
         for (CRGB& led: e)
         {
            float v = float(interpolateTheta(c0, c1, led_ix, e.size())) / (Vertex::NUM_THETA_STEPS/NUM_V);
            float h = float(interpolatePhi(c0, c1, led_ix, e.size())) / (Vertex::NUM_PHI_STEPS/(NUM_H-1));
            h += m_h_offset;
            if (h  >= NUM_H ) h = h - NUM_H;

            if (isDownwards())
            {
               v = NUM_V - v;
            }
            
            uint8_t index = interpolate(m_heat, h, v);

            if (isReversePalette())
            {
               index = 255 - index;
            }
            
            led = ColorFromPalette(*pal, index, getBrightness());
            
            led_ix++;
         }
      }
   }
   m_h_offset += float(getRotationSpeed())/50.f;
   if (m_h_offset >= NUM_H) m_h_offset = 0;
}
