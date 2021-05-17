#include "DefaultRainbow.hpp"

DefaultRainbow::DefaultRainbow(Triangle* triangles, unsigned num_triangles):
   m_triangles(triangles),
   m_num_triangles(num_triangles),
   m_iteration(0)
{}

void DefaultRainbow::noteOn(uint8_t note, uint8_t velocity)
{
   (void)note;
   (void)velocity;
}

void DefaultRainbow::noteOff(uint8_t note)
{
   (void)note;
}

void DefaultRainbow::controlChange(uint8_t cc_num, uint8_t value)
{
   (void)cc_num;
   (void)value;
}

void DefaultRainbow::run()
{
   for (unsigned t_ix = 0; t_ix < m_num_triangles; t_ix++)
   {
      Triangle& t(m_triangles[t_ix]);
      fill_rainbow(t.begin(), t.end() - t.begin(), m_iteration);
   }
   m_iteration++;
}
