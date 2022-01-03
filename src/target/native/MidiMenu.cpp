#include "MidiMenu.hpp"
#include "ImGui.hpp"
#include "Controls.hpp"

MidiMenu::MidiMenu(MidiSource& midi_source, ProgramFactory& program_factory):
   m_midi_source(midi_source),
   m_factory(program_factory)
{
}

void MidiMenu::drawMenu()
{
   m_factory.drawMenu(m_midi_source.getSender(), &m_piano);

   ImGui::Separator();
   
   if (m_midi_source.getMidiInPorts() and ImGui::TreeNode("MIDI Source"))
   {
      showMidiPorts(*m_midi_source.getMidiInPorts(), m_midi_sources);
      ImGui::TreePop();
   }
   if (m_midi_source.getMidiOutPorts() and ImGui::TreeNode("MIDI Destination"))
   {
      showMidiPorts(*m_midi_source.getMidiOutPorts(), m_midi_destinations);
      ImGui::TreePop();
   }
   if (m_midi_source.getMidiControllers() and ImGui::TreeNode("Controllers"))
   {
      showMidiPorts(*m_midi_source.getMidiControllers(), m_midi_controllers);
      ImGui::TreePop();
   }

   if (m_midi_source.getSender())
   {
      MidiSource::MidiSender& sender(*m_midi_source.getSender());
      if (sender.enabled() && ImGui::Button("Pause MIDI Output"))
      {
         sender.enable(false);
      }
      else if (not sender.enabled() && ImGui::Button("Resume MIDI Output"))
      {
         sender.enable(true);
         m_factory.program().sendSnapshot(m_midi_source.getSender());
      }
      ImGui::SameLine(); HelpMarker("Pause the MIDI output to play with controls and change programs without sending "
                                    "to the output port. When you resume, a snapshot of the current program number "
                                    "and controller values is sent immediately");

      ImGui::BeginDisabled(not sender.enabled());
      {
         if (ImGui::Button("Send Snapshot"))
         {
            m_factory.program().sendSnapshotWithTrigger(&sender);
         }
         ImGui::SameLine(); HelpMarker("Send the current program number and all controller values");

         if (ImGui::Button("Learn 'record' trigger"))
         {
            sender.sendControlChange(Controls::RECORD_CC, 127);
         }
         ImGui::SameLine(); HelpMarker("Send the trigger CC that's sent as the first message in a snapshot. "
                                       "Assign this to a function suitable to start recording the snapshot, "
                                       "e.g. 'session record' in Ableton");
         if (ImGui::Button("Learn 'stop' trigger"))
         {
            sender.sendControlChange(Controls::STOP_CC, 127);
         }
         ImGui::SameLine(); HelpMarker("Send the trigger CC that's sent as the last message in a snapshot. "
                                       "Assign this to a function suitable to end recording the snapshot, "
                                       "e.g. 'stop' in Ableton");

      }
      ImGui::EndDisabled();
   }
}

void MidiMenu::showMidiPorts(MidiSource::MidiPorts& ports, MidiPortMap& port_map)
{
   MidiSource::MidiPorts::PortId initial_selected_port, selected_port = 0;
   ports.updateAvailablePorts(port_map, selected_port);
   initial_selected_port = selected_port;

   ImGui::Bullet();
   if (ImGui::Selectable("None", selected_port == 0))
   {
      selected_port = 0;
   }

   for (auto& port: port_map)
   {
      ImGui::Bullet();
      if (ImGui::Selectable(port.second.c_str(), selected_port == port.first))
      {
         selected_port = port.first;
      }
   }

   if (selected_port != initial_selected_port)
   {
      ports.selectPort(selected_port);
      if (selected_port != 0)
      {
         // send snapshot to update controller state upon connection
         m_factory.program().sendSnapshot(m_midi_source.getSender());
      }
   }
}

void MidiMenu::noteOn(uint8_t note, uint8_t velocity, uint8_t channel)
{
   m_piano.noteOn(note, velocity, channel);
}

void MidiMenu::noteOff(uint8_t note, uint8_t channel)
{
   m_piano.noteOff(note, channel);
}