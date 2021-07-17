#ifndef GEOLEDIC_TRIANGLE_HPP
#define GEOLEDIC_TRIANGLE_HPP

#include "Vector.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "CRGB_iterator.hpp"

#define FASTLED_INTERNAL // get rid of annoying version pragma
#ifdef WITH_GFX
#include "gfx/Gfx.hpp"
#endif
#include "FastLED.h"

class Triangle {
public:
   Triangle(
            CRGB* led_strip,
            const Edge (&edges)[3],
            const Vertex (&vertices)[3]);
   
#ifdef WITH_GFX
   void createLeds(std::vector<gfx::LED>& leds, std::vector<gfx::Triangle>& triangles, int triangle_num) const;
#endif
   
   CRGB_iterator begin();
   CRGB_iterator end();
   unsigned size() const;
   
   const Edge& edge(unsigned ix) const;
   const Vertex& corner(unsigned ix) const;
   
   // LEDs on strip run in the opposite direction of the corner vertices
   bool isReverse() const;
   
private:
   Vertex inset(const Vertex& v1, const Vertex& v2, const Vertex& v3, float fac);
      
   Edge   m_edges[3];
   Vertex m_led_corners[3];
   unsigned m_first_corner_led;
   unsigned m_first_led;
   unsigned m_last_led;
   bool m_reverse;
   CRGB* m_strip;
};

#endif
