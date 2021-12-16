#include "MidiMessage.hpp"

#ifndef NO_STREAM

std::ostream& operator<<(std::ostream& os, const MidiMessage& mm)
{
   switch (mm.type())
   {
      case MidiMessage::NOTE_OFF: os << "NOTE_OFF "; break;
      case MidiMessage::NOTE_ON: os << "NOTE_ON "; break;
      case MidiMessage::AFTERTOUCH: os << "AFTERTOUCH "; break;
      case MidiMessage::CONTROL_CHANGE: os << "CONTROL_CHANGE "; break;
      case MidiMessage::PROGRAM_CHANGE: os << "PROGRAM_CHANGE "; break;
      case MidiMessage::CHANNEL_PRESSURE: os << "CHANNEL_PRESSURE "; break;
      case MidiMessage::PITCH_WHEEL: os << "PITCH_WHEEL "; break;
      default: os << "unknown "; break;
   }
   os << "ch" << mm.channel() << " ";
   unsigned k = 1;
   while (k < mm.length)
   {
      os << +mm.data[k++] << " ";
   }
   return os;
}

#endif

int MidiMessage::lengthForStatusByte(uint8_t first_byte)
{
   switch (first_byte >> 4)
   {
      case NOTE_OFF:
      case NOTE_ON:
      case AFTERTOUCH:
      case CONTROL_CHANGE:
      case PITCH_WHEEL:
         return 3;
      case PROGRAM_CHANGE:
      case CHANNEL_PRESSURE:
         return 2;
      default:
         // discard remainder, SYSEX etc not handled
         return -1;
   }
}

MidiMessage MidiMessage::makeNote(uint8_t num, uint8_t velocity, uint8_t channel)
{
   MidiMessage msg;
   msg.length = 3;
   msg.data[0] = ((velocity ? MidiMessage::NOTE_ON : MidiMessage::NOTE_OFF) << 4) | (channel & 0xF);
   msg.data[1] = num;
   msg.data[2] = velocity;
   return msg;
}

MidiMessage MidiMessage::makeCC(uint8_t num, uint8_t val, uint8_t channel)
{
   MidiMessage msg;
   msg.length = 3;
   msg.data[0] = (MidiMessage::CONTROL_CHANGE << 4) | (channel & 0xF);
   msg.data[1] = num;
   msg.data[2] = val;
   return msg;
}
