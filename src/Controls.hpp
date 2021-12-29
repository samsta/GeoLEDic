#ifndef GEOLEDIC_CONTROLS_HPP
#define GEOLEDIC_CONTROLS_HPP

#include "Program.hpp"
#include <stdint.h>

class Controls: public Program
{
public:
   enum SpecialControllers {
      BRIGHTNESS_CC = 7,
      FORCE_BLANK_CC = 80,
      RECORD_CC = 81,
      STOP_CC = 82,
      PROGRAM_CHANGE_CC = 119
   };

   Controls();
   virtual void controlChange(uint8_t cc_num, uint8_t value);
   
   static bool& getForceBlank();

#ifdef WITH_GFX
   virtual void sendSnapshotWithTrigger(MidiSource::MidiSender* sender);
#endif

protected:
   uint8_t getBrightness() const;

   uint8_t getControlValue(uint8_t cc_num) const;
   void setControlValue(uint8_t cc_num, uint8_t value);

   uint8_t m_control_values[128];
   static uint8_t m_brightness_raw;
   static bool m_force_blank;
};

#endif // GEOLEDIC_PROGRAM_HPP
