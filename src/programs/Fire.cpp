#include "Fire.hpp"
#include <iostream>
#include <math.h>

Fire::Fire(const DomeWrapper& dome):
   m_dome(dome),
   m_sparking(10),
   m_cooling(55)
{
   memset(m_heat, 0, sizeof(m_heat));
}

void Fire::noteOn(uint8_t note, uint8_t velocity)
{
   (void)note;
   (void)velocity;
}

void Fire::noteOff(uint8_t note)
{
   (void)note;
}

void Fire::controlChange(uint8_t cc_num, uint8_t value)
{
   switch (cc_num)
   {
      case 16:
         m_cooling = value;
         break;
      case 17:
         m_sparking = value;
         break;
      default:
         break;
   }
}

void Fire::calcFire()
{
   for (unsigned h = 0; h < NUM_H; h++)
   {
      // Step 1.  Cool down every cell a little
      for(unsigned v = 0; v < NUM_V; v++)
      {
         m_heat[h][v] = qsub8(m_heat[h][v],  random8(0, ((m_cooling * 10) / NUM_V) + 2));
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
      if(random8() < m_sparking)
      {
         int v = random8(7);
         m_heat[h][v] = qadd8(m_heat[h][v], random8(160,255));
      }
   }
}

void Fire::run()
{
   calcFire();

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
            float h = float(interpolatePhi(c0, c1, led_ix, e.size())) / (Vertex::NUM_PHI_STEPS/NUM_H);
            
            int v0 = floor(v);
            int v1 = std::min(v0+1, NUM_V-1);
            float rv0 =  v1 - v;
            float rv1 = 1 - rv0;
            int h0 = floor(h);
            int h1 = h0 == NUM_H-1 ? 0 : h0+1;
            float rh0 = (h0+1) - h;
            float rh1 = 1 - rh0;
            
            led = HeatColor(m_heat[h0][v0] * rh0 * rv0 +
                            m_heat[h0][v1] * rh0 * rv1 +
                            m_heat[h1][v0] * rh1 * rv0 +
                            m_heat[h1][v1] * rh1 * rv1);
            led_ix++;
         }
      }
   }
}