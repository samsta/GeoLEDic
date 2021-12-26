#include "Controls.hpp"
#include <algorithm>

uint8_t Controls::m_brightness_raw = 50;
bool Controls::m_force_blank = false;

Controls::Controls()
{
   std::fill_n(m_control_values, sizeof(m_control_values), 0);
}

void Controls::controlChange(uint8_t cc_num, uint8_t value)
{
   setControlValue(cc_num, value);
}

uint8_t Controls::getBrightness() const
{
#ifdef WITH_GFX
   if (m_brightness_raw == 0)
   {
      return 0;
   }
   if (m_brightness_raw < 64)
   {
      // brighten dark settings up on screen by flattening the slope on the lower half of the curve
      return 64 + m_brightness_raw;
   }
#endif
   return m_brightness_raw * 2; // brightness is 0..255 (well, 254, realistically)
}

bool& Controls::getForceBlank()
{
   return m_force_blank;
}


uint8_t Controls::getControlValue(uint8_t cc_num) const
{
   if (cc_num >= sizeof(m_control_values)) return 0;
   if (cc_num == BRIGHTNESS_CC) return m_brightness_raw;
   if (cc_num == FORCE_BLANK_CC) return m_force_blank ? 127: 0;
   return m_control_values[cc_num];
}

void Controls::setControlValue(uint8_t cc_num, uint8_t value)
{
   if (cc_num >= sizeof(m_control_values)) return;
   if (cc_num == BRIGHTNESS_CC)
   {
      m_brightness_raw = value;
      return;
   }
   if (cc_num == FORCE_BLANK_CC)
   {
      m_force_blank = value > 0;
      return;
   }

   m_control_values[cc_num] = value;
}

#ifdef WITH_GFX
#include <unistd.h>
void Controls::sendSnapshotWithTrigger(MidiSource::MidiSender* sender)
{
   if (sender == nullptr) return;
    
   sender->sendControlChange(RECORD_CC, 127);
   usleep(50000);
   sendSnapshot(sender);
   // Yuck! Ableton needs this sleep between start and stop or else it won't start :-(
   usleep(300000);
   sender->sendControlChange(STOP_CC, 127);
}
#endif
