#include "GeoLEDic.hpp"
#include "programs/Diagnostic.hpp"
#include "Controls.hpp"
#ifdef WITH_GFX
#include "Serial.hpp"
#endif

namespace {
MidiSource midi_source;
ProgramFactory factory({dome, DOME_NUM_TRIANGLES}, {leds, NUM_STRIPS, LEDS_PER_STRIP});
MidiNoteObserver* note_observer = nullptr;
}

void loopGeoLEDic()
{
   const MidiMessage* msg;
   while ((msg = midi_source.read()))
   {
      switch (msg->type())
      {
         case MidiMessage::NOTE_ON:
            factory.program().noteOn(msg->data[1], msg->data[2], msg->channel());
            if (note_observer)
            {
               note_observer->noteOn(msg->data[1], msg->data[2], msg->channel());
            }
            break;
         case MidiMessage::NOTE_OFF:
            factory.program().noteOff(msg->data[1], msg->channel());
            if (note_observer)
            {
               note_observer->noteOff(msg->data[1], msg->channel());
            }
            break;
         case MidiMessage::CHANNEL_PRESSURE:
            // Ableton Live does not deal well with program changes, so we've encoded them as channel pressure.
            // Offset by 1 to make it a little less confusing
            factory.changeProgram(msg->data[1] - 1);
            break;
         case MidiMessage::PROGRAM_CHANGE:
            factory.changeProgram(msg->data[1]);
            break;
         case MidiMessage::CONTROL_CHANGE:
            // deleteme April 2022
            if (msg->data[1] == 119)
            {
               // Ableton Live does not deal well with program changes, so we've encoded them as control changes.
               // Offset by 1 to make it a little less confusing
               // NOTE: This is only for backwards compatibility, delete after April 2022. We're now doing this using channel pressure
               factory.changeProgram(msg->data[2] - 1);
            }
            else
            {
               factory.program().controlChange(msg->data[1], msg->data[2]);
            }
            break;
         default:
            break;
      }
   }

#if defined(USB_MIDI_SERIAL) or defined(WITH_GFX)
   static Diagnostic* diagnostic_mode = nullptr;
   while (Serial.available())
   {
      char c = Serial.read();
      if (c == 'd' or c == 'D')
      {
         factory.changeProgram(127);
         diagnostic_mode = reinterpret_cast<Diagnostic*>(&factory.program());
         Serial.println("Diagnostic mode entered");
      }
      else if (c == 'x' or c == 'X')
      {
         factory.changeProgram(0);
         Serial.println("Diagnostic mode exited");
         diagnostic_mode = nullptr;
      }
      else if (diagnostic_mode)
      {
         if (diagnostic_mode->processKeyboardInput(c))
         {
            if (c == '\r' or c == '\n')
            {
               Serial.println();
            }
            else
            {
               Serial.print(c);
            }
         }
      }
   }
#endif

   factory.program().run();
}

MidiSource& getMidiSource()
{
   return midi_source;
}

ProgramFactory& getProgramFactory()
{
   return factory;
}

void registerNoteObserver(MidiNoteObserver* observer)
{
   note_observer = observer;
}