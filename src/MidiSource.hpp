#ifndef GEOLEDIC_MIDI_SOURCE_HPP
#define GEOLEDIC_MIDI_SOURCE_HPP

#include "MidiMessage.hpp"

#define MIDI_CLIENT_NAME "GeoLEDic Client"
#define MIDI_PORT_NAME "GeoLEDic In"
#define MIDI_OUT_PORT_NAME "GeoLEDic Out"

#ifdef WITH_GFX
#include <string>
#include <map>

class MidiControllerList;

#endif

class MidiNoteObserver
{
public:
   virtual ~MidiNoteObserver(){}
   virtual void noteOn(uint8_t note, uint8_t velocity, uint8_t channel) = 0;
   virtual void noteOff(uint8_t note, uint8_t channel) = 0;
};

class MidiSource
{
public:
   MidiSource();
   ~MidiSource();
   
   const MidiMessage* read();
   
#ifdef WITH_GFX
   class MidiPorts {
   public:
      typedef int PortId;
      virtual ~MidiPorts(){};
      virtual void updateAvailablePorts(std::map<PortId, std::string>& port_map, PortId& selected_port) = 0;
      virtual void selectPort(PortId selected_port) = 0;
   };

   class MidiSender
   {
   public:
      virtual ~MidiSender(){};
      virtual bool enabled() const = 0;
      virtual void enable(bool ena) = 0;
      virtual void sendControlChange(uint8_t cc_num, uint8_t val) = 0;
      virtual void sendNote(uint8_t note, uint8_t velocity, uint8_t channel) = 0;
      virtual void sendProgramChange(uint8_t program) = 0;
   };

   MidiPorts* getMidiInPorts();
   MidiPorts* getMidiOutPorts();
   MidiSender* getSender();
   MidiPorts* getMidiControllers();
#endif
   
private:
   class Impl;
   Impl& m_i;
};

#endif // GEOLEDIC_MIDI_SOURCE_HPP
