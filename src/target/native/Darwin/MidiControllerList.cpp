#include "MidiControllerList.hpp"
#include "LaunchPad.hpp"
#include <iostream>
#include <set>
#include <sstream>

MidiControllerList::MidiControllerList(MidiMessageSink& midi_dest):
   m_controller(nullptr),
   m_midi_dest(midi_dest)
{
    pthread_mutex_init(&m_midi_mutex, NULL);
    pthread_mutex_init(&m_device_mutex, NULL);
}

MidiControllerList::~MidiControllerList()
{
   delete m_controller;
}


void MidiControllerList::createPorts(MIDIClientRef client)
{
    OSStatus result =  MIDIOutputPortCreate(client, CFSTR("Ctrl Out"), &m_out_port);
    if (result != noErr) {
        std::cerr << "MidiControllerList::createPorts(): MIDIOutputPortCreate() error:" << result;
        return;
    }

    result =  MIDIInputPortCreate(client, CFSTR("Ctrl In"), processMidi, this,  &m_in_port);
    if (result != noErr) {
        std::cerr << "MidiControllerList::createPorts(): MIDIInputPortCreate() error:" << result;
        return;
    }
}

void MidiControllerList::processMidi(const MIDIPacketList *packets,
                        void *ref_con, void  *data)
{
    if (ref_con != nullptr) static_cast<MidiControllerList*>(ref_con)->processMidiPackets(packets);
}

void MidiControllerList::processMidiPackets(const MIDIPacketList *packets)
{
    const MIDIPacket *packet = static_cast<const MIDIPacket *>(packets->packet);
   
    // push messages into a queue to hand them over to the working thread which
    // will consume them via read()
    //pthread_mutex_lock(&m_midi_mutex);
    for (unsigned int i = 0; i < packets->numPackets; ++i)
    {
        int remaining_length = packet->length;
        const uint8_t* p = packet->data;
        while (remaining_length > 0)
        {
            int length = MidiMessage::lengthForStatusByte(*p);
            
            if (length < 0) break; // skip remainder of message
            if (length > remaining_length) break; 

            MidiMessage msg;
            msg.length = length;
            std::copy_n(p, length, msg.data);
            p += length;
            remaining_length -= length;
            if (m_controller) m_controller->updateFromCtrl(msg);
            //m_packets.push(msg);
            //std::cout << msg << std::endl;
        }
        packet = MIDIPacketNext(packet);
    }
    //pthread_mutex_unlock(&m_midi_mutex);
}

void MidiControllerList::refreshPorts()
{
   pthread_mutex_lock(&m_device_mutex);

   std::set<MIDIDeviceRef> to_remove;
   for (const auto& pair: m_devices)
   {
      to_remove.insert(pair.first);
   }

   for (unsigned dev_ix = 0; dev_ix < MIDIGetNumberOfDevices(); dev_ix++)
   {
      MIDIDeviceRef dev = MIDIGetDevice(dev_ix);
      if (m_devices.count(dev))
      {
         to_remove.erase(dev);
         continue;
      }
      
      SInt32 device_absent = 1;
      MIDIObjectGetIntegerProperty(dev, kMIDIPropertyOffline, &device_absent);
      if (device_absent) continue;
      
      std::stringstream description;
      CFStringRef name;
      char tmp[64];
      if (MIDIObjectGetStringProperty(dev, kMIDIPropertyModel, &name) == noErr)
      {
         CFStringGetCString(name, tmp, sizeof(tmp), 0);
         description << tmp;
      }
      if (description.str().size() == 0)
      {
         description << "Unknown device " << dev_ix;
      }

      // at the minute we only support the launch pad
      if (description.str().find("Launchpad") != 0) continue;
      
      m_devices[dev] = description.str();
   }
   
   // remove the ones that were no longer present
   for (MIDIDeviceRef dev: to_remove)
   {
      if (m_selected_device == dev)
      {
         disconnect(dev);
         m_selected_device = 0;
      }
      m_devices.erase(dev);
   }
   
   pthread_mutex_unlock(&m_device_mutex);
}

void MidiControllerList::updateAvailablePorts(std::map<PortId, std::string>& port_map, PortId& selected_port)
{
   selected_port = m_selected_device;
   
   pthread_mutex_lock(&m_device_mutex);
   // remove non-existing ones
   for (auto it = port_map.cbegin(); it != port_map.cend(); )
   {
      if (m_devices.count(it->first))
      {
         ++it;
      }
      else
      {
         port_map.erase(it++);
      }
   }
   
   // add new ones
   for (const auto& pair: m_devices)
   {
      if (port_map.count(pair.first) == 0)
      {
         port_map[pair.first] = pair.second;
      }
   }
   pthread_mutex_unlock(&m_device_mutex);
}

void MidiControllerList::selectPort(PortId selected_port)
{
   pthread_mutex_lock(&m_device_mutex);
   if (selected_port != m_selected_device)
   {
      disconnect(m_selected_device);
      m_selected_device = 0;
   }
   
   if (not m_devices.count(selected_port))
   {
      pthread_mutex_unlock(&m_device_mutex);
      return;
   }
   
   if (not connect(selected_port))
   {
      selected_port = 0;
   }
   m_selected_device = selected_port;
   pthread_mutex_unlock(&m_device_mutex);
}

bool MidiControllerList::connect(MIDIDeviceRef dev)
{
   for (unsigned ent_ix = 0; ent_ix < MIDIDeviceGetNumberOfEntities(dev); ent_ix++)
   {
      MIDIEntityRef entity = MIDIDeviceGetEntity(dev, ent_ix);
      MIDIEndpointRef source = MIDIEntityGetSource(entity, 0);
      
      CFStringRef name;
      char tmp[64];
      if (MIDIObjectGetStringProperty(source, kMIDIPropertyName, &name) == noErr)
      {
         CFStringGetCString(name, tmp, sizeof(tmp), 0);
         // skip the DAW port
         if (strstr(tmp, "DAW")) continue;
      }

      int result = MIDIPortConnectSource(m_in_port, source, this);
      if (result != noErr)
      {
         std::cerr << "MIDIPortConnectSource() error:" << result;
         return false;
      }
      
      m_in_endpoint  = source;
      m_out_endpoint = MIDIEntityGetDestination(entity, 0);

      CFStringGetCString(name, tmp, sizeof(tmp), 0);
      if (strcmp(tmp, "LPProMK3 MIDI") == 0)
      {
         m_controller = new LaunchPad(*this, m_midi_dest, LaunchPad::PRO_MK3);
      }
      else
      {
         m_controller = new LaunchPad(*this, m_midi_dest, LaunchPad::MINI_MK3);
      }

      return true;
   }
   return false;
}

void MidiControllerList::disconnect(MIDIDeviceRef dev)
{
   delete m_controller;
   m_controller = nullptr;
   MIDIPortDisconnectSource(m_in_port, m_in_endpoint);
   m_out_endpoint = 0;
}

void MidiControllerList::sink(const MidiMessage& msg)
{
   if (m_out_endpoint == 0) return;

    std::cout << "OUT: " << msg << std::endl;

   uint8_t buf[512];
   MIDIPacketList* packet_list = reinterpret_cast<MIDIPacketList*>(buf);
   MIDIPacket     *pkt = MIDIPacketListInit(packet_list);
   MIDIPacketListAdd(packet_list, sizeof(buf), pkt, 0, msg.length, msg.data);
   MIDISend(m_out_port, m_out_endpoint, packet_list);
}

void MidiControllerList::updateController(const MIDIPacketList* packets)
{
   if (m_controller == nullptr) return;
   const MIDIPacket *packet = static_cast<const MIDIPacket *>(packets->packet);
    for (unsigned int i = 0; i < packets->numPackets; ++i)
    {
        int remaining_length = packet->length;
        const uint8_t* p = packet->data;
        while (remaining_length > 0)
        {
            int length = MidiMessage::lengthForStatusByte(*p);
            
            if (length < 0) break; // skip remainder of message
            if (length > remaining_length) break; 

            MidiMessage msg;
            msg.length = length;
            std::copy_n(p, length, msg.data);
            p += length;
            remaining_length -= length;
            m_controller->updateFromMidi(msg);
        }
        packet = MIDIPacketNext(packet);
    }
}

void MidiControllerList::refreshUi()
{
   if (m_controller) m_controller->refreshUi();
}
