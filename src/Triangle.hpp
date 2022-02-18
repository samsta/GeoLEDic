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

// move edges towards triangle center by 'distance'
void inset(Vertex (&corners)[3], float distance);

class Triangle {
public:
   Triangle(
            CRGB* led_strip,
            const Edge (&edges)[3],
            const Vertex (&vertices)[3]);
   
#ifdef WITH_GFX
   void createLeds(std::vector<gfx::LED>& leds, std::vector<gfx::Triangle>& triangles, int triangle_num) const;
   const Vertex& centroid() const;
#endif
   
   // begin() starts on first corner (bottom left by convention), the iterator then goes clockwise to end()
   CRGB_iterator begin();
   CRGB_iterator end();
   
   // unlike begin(), firstLed() points to the first LED on the strip that's part of the triangle,
   //  so it could be anywhere on the triangle
   CRGB* firstLed();
   
   unsigned size() const;
   
   const Edge& edge(unsigned ix) const;
   const Vertex& corner(unsigned ix) const;
   
   // LEDs on strip run in the opposite direction of the corner vertices
   bool isReverse() const;
   
private:
   Edge   m_edges[3];
   Vertex m_led_corners[3];
#ifdef WITH_GFX
   Vertex m_triangle_corners[3];
   Vertex m_centroid;
#endif
   unsigned m_first_corner_led;
   unsigned m_first_led;
   unsigned m_last_led;
   bool m_reverse;
   CRGB* m_strip;
};

#endif
