#ifndef Notes_hpp
#define Notes_hpp

#include "Controls.hpp"
#include "MidiDefs.hpp"
#include <set>

class Notes: public Controls
{
public:
   Notes();
   virtual void noteOn(uint8_t note, uint8_t velocity, uint8_t channel);
   virtual void noteOff(uint8_t note, uint8_t channel);
   
   uint8_t note(uint8_t note) const;
   bool isAnyNoteSet() const;

   virtual void run();
   
protected:
   uint8_t m_notes[NOTE_HIGHEST+1];
   std::set<uint8_t> m_notes_set_this_run;
   std::set<uint8_t> m_notes_to_clear_next_run;
   std::set<uint8_t> m_notes_to_clear_this_run;
};

#endif /* Notes_hpp */
