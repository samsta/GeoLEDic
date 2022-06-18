#include "Triangle.hpp"
#include <climits>
#include <algorithm>
#include <math.h>
#include "assert.hpp"

void inset(Vertex (&corners)[3], float distance)
{
   Vertex new_corners[3];
   for (unsigned k = 0; k < 3; k++)
   {
      Vector e1 = corners[(k+1)%3] - corners[k];
      Vector e2 = corners[(k+2)%3] - corners[k];
      
      float theta = angleRad(e1, e2);
      float scale = distance/sin(theta);
      
      new_corners[k] = corners[k] + normalise(e1) *scale + normalise(e2) * scale;
   }
   
   std::copy(&new_corners[0], &new_corners[3], &corners[0]);
}

Triangle::Triangle(
   CRGB* strip,
   const Edge (&edges)[3],
   const Vertex (&vertices)[3]):
      m_edges(),
      m_led_corners(),
#ifdef WITH_GFX
      m_triangle_corners(),
#endif
      m_centroid((Vector(vertices[0]) + Vector(vertices[1]) + Vector(vertices[2]))/3.0),
      m_first_corner_led(edges[0].firstLedOnEdge()),
      m_first_led(INT_MAX),
      m_last_led(0),
      m_reverse(edges[0].isReverse()),
      m_strip(strip)
{
   std::copy(&edges[0], &edges[3], &m_edges[0]);
#ifdef WITH_GFX
   std::copy(&vertices[0], &vertices[3], &m_triangle_corners[0]);
#endif
   std::copy(&vertices[0], &vertices[3], &m_led_corners[0]);
   inset(m_led_corners, 0.0254); // half the width of a 2" beam
 
   bool has_split_edge = false;
   // find the first and the last led of strip
   for (Edge& e: m_edges)
   {
      // we're passing this in here because it would have made the constructor more verbose,
      //  leading to even more stuff to supply in Dome.cpp
      e.assign(strip);
      m_first_led = std::min(e.firstLedOnStrip(), m_first_led);
      m_last_led  = std::max(e.lastLedOnStrip(), m_last_led);
      has_split_edge |= e.isSplit();
   }
   
   //Consistency check.
   int edge_ix = 0;
   Edge* prev = &m_edges[2];
   for (Edge& e: m_edges)
   {
      ASSERT_CMP(m_reverse, ==, e.isReverse(), "All edges must go in the same direction, but edge " << edge_ix << " doesn't");
      
      if (not m_reverse and e.firstLedOnEdge() != m_first_led)
      {
         ASSERT_CMP(e.firstLedOnEdge(), ==, prev->lastLedOnEdge()+1, "Corners must be continuous, but corner " << edge_ix << " isn't");
      }
      if (m_reverse and e.firstLedOnEdge() != m_last_led)
      {
         ASSERT_CMP(e.firstLedOnEdge(), ==, prev->lastLedOnEdge()-1, "Corners must be continuous, but corner " << edge_ix << " isn't");
      }
      prev = &e;
      edge_ix++;
   }
}

CRGB_iterator Triangle::begin()
{
   return {&m_strip[m_first_corner_led],
           &m_strip[m_first_led],
           &m_strip[m_last_led],
           m_reverse ? CRGB_iterator::BACKWARD : CRGB_iterator::FORWARD
   };
}

CRGB* Triangle::firstLed()
{
   return &m_strip[m_first_led];
}

CRGB_iterator Triangle::end()
{
   return {&m_strip[m_first_corner_led], CRGB_iterator::invalid_iterator_tag()};
}

unsigned Triangle::size() const
{
   return m_last_led - m_first_led + 1;
}

bool Triangle::isReverse() const
{
   return m_reverse;
}

const Edge& Triangle::edge(unsigned edge_ix) const
{
   return m_edges[edge_ix % 3];
}

const Vertex& Triangle::corner(unsigned corner_ix) const
{
   return m_led_corners[corner_ix % 3];
}

const Vertex& Triangle::centroid() const 
{
   return m_centroid;
}

#ifdef WITH_GFX
void Triangle::createLeds(std::vector<gfx::LED>& leds, std::vector<gfx::Triangle>& triangles, int triangle_num) const
{
   const float SCALE_AWAY_FROM_ORIGIN = 1.001;
   gfx::Triangle led_triangle;   // the triangle with the diffused LED lighting
   gfx::Triangle outer_triangle; // the triangle that's part of the outer shell
   led_triangle.vertices[0].start_led_ix = leds.size();
   
   auto i = 0;
   for (const Edge& e: m_edges)
   {
      // distribute LEDs along edge
      const Vertex& start_corner = m_led_corners[i];
      const Vertex& end_corner = m_led_corners[(i+1)%3];
      const Vector edge_vector = end_corner - start_corner;

      int num_leds = e.size();
      int led_ix = 0;
      for (CRGB& led: e)
      {
         Vertex led_vertex(start_corner + edge_vector * ((float(led_ix)+0.5)/float(num_leds)));
         leds.push_back(led_vertex);
         leds.back().color = &led;
         led_ix++;
      }
      
      led_triangle.vertices[i].x = start_corner.x;
      led_triangle.vertices[i].y = start_corner.y;
      led_triangle.vertices[i].z = start_corner.z;
      
      // scale the outer triangle away from origin to prevent z-fighting
      outer_triangle.vertices[i].x = m_triangle_corners[i].x * SCALE_AWAY_FROM_ORIGIN;
      outer_triangle.vertices[i].y = m_triangle_corners[i].y * SCALE_AWAY_FROM_ORIGIN;
      outer_triangle.vertices[i].z = m_triangle_corners[i].z * SCALE_AWAY_FROM_ORIGIN;

      i++;
   }
   led_triangle.vertices[0].num_leds = leds.size() - led_triangle.vertices[0].start_led_ix;
   // insert outer (semi-transparent) triangles at the back so they get
   //   drawn after the solid ones or else transparency looks funky
   triangles.insert(triangles.begin(), led_triangle);
   triangles.push_back(outer_triangle);
}

#endif
