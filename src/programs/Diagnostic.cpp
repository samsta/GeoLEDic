#include "Diagnostic.hpp"

namespace 
{
   const int NONE(-1);
}

Diagnostic::Diagnostic(Triangle* triangles, unsigned num_triangles, CRGB* strips, unsigned num_strips, unsigned leds_per_strip):
   m_triangles(triangles),
   m_num_triangles(num_triangles),
   m_strips(strips),
   m_num_strips(num_strips),
   m_leds_per_strip(leds_per_strip),
   m_state(INTRO),
   m_iteration(0),
   m_accumulator(0),
   m_lit_led(NONE),
   m_lit_triangle(NONE)
{
}

void Diagnostic::noteOn(uint8_t note, uint8_t velocity)
{
   (void)note;
   (void)velocity;
}

void Diagnostic::noteOff(uint8_t note)
{
   (void)note;
}

void Diagnostic::controlChange(uint8_t cc_num, uint8_t value)
{
   (void)cc_num;
   (void)value;
}

void Diagnostic::run()
{
   if (m_state == INTRO)
   {
      const int stride = 20;
      for (int i = 0; i < m_num_strips; i++)
      {
         for (int j = 0; j < m_leds_per_strip; j++)
         {
            CRGB& led(m_strips[i*m_leds_per_strip + j]);
            if (j >= m_iteration and j < (m_iteration+stride))
            {
               led = CRGB::White;
            }
            else
            {
               led = CRGB::Black;
            }
         }
      }
      m_iteration += stride;
      if (m_iteration > m_leds_per_strip)
      {
         m_state = WAIT;
      }
   }
   else
   {
      if (m_lit_led != NONE)
      {
         for (int j = 0; j < m_leds_per_strip; j++)
         {
            CRGB& led(m_strips[j]);
            if (j == m_lit_led)
            {
               led = CRGB::White;
            }
            else
            {
               led = CRGB::Black;
            }
         }
      }
      else
      {
         for (int i = 0; i < m_num_triangles; i++)
         {
            Triangle& t(m_triangles[i]);

            fill_solid(t.begin(), t.end() - t.begin(), 
                     i == m_lit_triangle ? CRGB::White : CRGB::Black);
         }
      }

   }
}

bool Diagnostic::processKeyboardInput(char c)
{
   bool input_accepted = false;
   switch (m_state)
   {         
      case WAIT:
         m_accumulator = 0;
         switch (c)
         {
            case 'l':
            case 'L':
               m_state = LED;
               input_accepted = true;
               break;
            case 't':
            case 'T':
               m_state = TRIANGLE;
               input_accepted = true;
               break;
            default:
               break;
         }
         break;
      
      case LED:
         if (isdigit(c))
         {
            m_accumulator = 10 * m_accumulator + (c - '0');
            input_accepted = true;
         }
         else if (c == '\r' or c == '\n')
         {
            m_lit_led = m_accumulator;
            m_lit_triangle = NONE;
            input_accepted = true;
            m_state = WAIT;
         }
         break;

      case TRIANGLE:
         if (isdigit(c))
         {
            m_accumulator = 10 * m_accumulator + (c - '0');
            input_accepted = true;
         }
         else if (c == '\n' or c == '\r')
         {
            m_lit_triangle = m_accumulator;
            m_lit_led = NONE;
            input_accepted = true;
            m_state = WAIT;
         }
         break;

      case INTRO:
      default:
         break;
   }
   return input_accepted;
}