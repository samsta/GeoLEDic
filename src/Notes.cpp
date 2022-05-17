#include "Notes.hpp"
#include <vector>
#include <algorithm>

Notes::Notes()
{
   std::fill(m_notes, m_notes+NOTE_HIGHEST+1, 0);
}

void Notes::noteOn(uint8_t note, uint8_t velocity, uint8_t channel)
{
   if (note <= NOTE_HIGHEST)
   {
      if (velocity > 0) 
      {
         m_notes_set_this_run.insert(note);
      }
      m_notes[note] = velocity;
   }
}

void Notes::run()
{
   m_notes_set_this_run.clear();
   for (uint8_t note: m_notes_to_clear_this_run)
   {
      noteOn(note, 0, 0);
   }
   m_notes_to_clear_this_run = m_notes_to_clear_next_run;
   m_notes_to_clear_next_run.clear();
}


void Notes::noteOff(uint8_t note, uint8_t channel)
{
   if (m_notes_set_this_run.count(note))
   {
      // postpone until next 
      m_notes_to_clear_next_run.insert(note);
   }
   else
   {
      noteOn(note, 0, channel);
   }
}

uint8_t Notes::note(uint8_t note) const
{
   if (note > NOTE_HIGHEST) return 0;
   return m_notes[note];
}
