#ifndef GELOEDIC_MidiMessage_h
#define GELOEDIC_MidiMessage_h

#include <stdint.h>

struct MidiMessage
{
   enum Type {
      NOTE_OFF         = 0x8,
      NOTE_ON          = 0x9,
      AFTERTOUCH       = 0xA,
      CONTROL_CHANGE   = 0xB,
      PROGRAM_CHANGE   = 0xC,
      CHANNEL_PRESSURE = 0xD,
      PITCH_WHEEL      = 0xE
   };
   
   Type type() const { return Type(data[0] >> 4); }
   unsigned channel() const { return data[0] & 0xF; }
   
   uint8_t data[4];
   unsigned length;
};

#endif /* GELOEDIC_MidiMessage_h */
