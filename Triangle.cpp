#include "Triangle.hpp"

Triangle::Triangle(
   const Edge (&edges)[3],
   const Vertex (&vertices)[3]):
      m_edges(),
      m_vertices(),
      m_led_corners()
{
   float fac = 0.95;
   std::copy(&edges[0], &edges[3], &m_edges[0]);
   std::copy(&vertices[0], &vertices[3], &m_vertices[0]);
   // the LEDs are not on the triangle's edge, but slightly inset.
   m_led_corners[0] = inset(m_vertices[0], m_vertices[1], m_vertices[2], fac);
   m_led_corners[1] = inset(m_vertices[1], m_vertices[2], m_vertices[0], fac);
   m_led_corners[2] = inset(m_vertices[2], m_vertices[0], m_vertices[1], fac);
}

Vertex Triangle::inset(const Vertex& v1, const Vertex& v2, const Vertex& v3, float fac)
{
   return Vertex(Vector(v1) * fac +
                 Vector(v2) * ((1-fac)/2) +
                 Vector(v3) * ((1-fac)/2));
}

void Triangle::createLeds(std::vector<flogl::LED>& leds) const
{
   auto i = 0;
   for (const Edge& e: m_edges)
   {
      // distribute LEDs along edge
      const Vertex& start_corner = m_led_corners[i];
      const Vertex& end_corner = m_led_corners[(i+1)%3];
      const Vector edge_vector = end_corner - start_corner;

      int num_leds = e.last_led - e.first_led + 1;
      for (int led_ix = 0; led_ix < num_leds; led_ix++)
      {
         Vertex led_vertex(start_corner + edge_vector * ((float(led_ix)+0.5)/float(num_leds)));
         leds.push_back(led_vertex);
      }
      
      i++;
   }
}