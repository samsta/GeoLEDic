#ifndef MidiMenu_hpp
#define MidiMenu_hpp

#include "gfx/Config.hpp"
#include "MidiSource.hpp"
#include "ProgramFactory.hpp"
#include "Piano.hpp"

class MidiMenu: public gfx::Config::MenuPresenter, public MidiNoteObserver
{
public:
   MidiMenu(MidiSource& midi_source, ProgramFactory& program_factory);
   
   virtual void drawMenu();
   virtual void noteOn(uint8_t note, uint8_t velocity, uint8_t channel);
   virtual void noteOff(uint8_t note, uint8_t channel);
   
private:
   typedef std::map<MidiSource::MidiPorts::PortId, std::string> MidiPortMap;
   
   void showMidiPorts(MidiSource::MidiPorts& ports, MidiPortMap& map);

   MidiSource& m_midi_source;
   ProgramFactory& m_factory;
   
   MidiPortMap m_midi_sources;
   MidiPortMap m_midi_destinations;
   MidiPortMap m_midi_controllers;

   Piano m_piano;
};

#endif /* MidiMenu_hpp */
