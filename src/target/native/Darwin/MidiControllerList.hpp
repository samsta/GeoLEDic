#ifndef DARWIN_MIDICONTROLLERLIST_HPP
#define DARWIN_MIDICONTROLLERLIST_HPP

#include "MidiSource.hpp"
#include "MidiMessage.hpp"
#include <CoreMIDI/CoreMIDI.h>
#include <queue>

class LaunchPad;

class MidiControllerList: public MidiSource::MidiPorts, private MidiMessageSink
{
public:
    MidiControllerList(MidiMessageSink& midi_dest);
    ~MidiControllerList();

    virtual void updateAvailablePorts(std::map<PortId, std::string>& port_map, PortId& selected_port);
    virtual void selectPort(PortId selected_port);

    void createPorts(MIDIClientRef client);
    void refreshPorts();

    void updateController(const MIDIPacketList* packet_list);
    void refreshUi();
private:

   void processMidiPackets(const MIDIPacketList *packets);
   static void processMidi(const MIDIPacketList *packets, void *ref_con, void *data);

   bool connect(MIDIDeviceRef dev);
   void disconnect(MIDIDeviceRef dev);
   
   virtual void sink(const MidiMessage& msg);


   MIDIPortRef             m_in_port;
   MIDIPortRef             m_out_port;
   std::queue<MidiMessage> m_packets;
   pthread_mutex_t         m_device_mutex;
   std::map<MIDIDeviceRef, std::string> m_devices;
   MIDIDeviceRef           m_selected_device;
   MIDIEndpointRef         m_in_endpoint;
   MIDIEndpointRef         m_out_endpoint;

   LaunchPad*              m_controller;
   MidiMessageSink&        m_midi_dest;
};

#endif // DARWIN_MIDICONTROLLERLIST_HPP
