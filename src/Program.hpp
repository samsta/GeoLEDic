#ifndef GEOLEDIC_PROGRAM_HPP
#define GEOLEDIC_PROGRAM_HPP

#include <stdint.h>
#include "MidiSource.hpp"

#ifdef WITH_GFX
class Piano;
#endif

class Program: public MidiNoteObserver
{
public:
   virtual ~Program(){}
   
   virtual void controlChange(uint8_t cc_num, uint8_t value) = 0;
   virtual void run() = 0;

#ifdef WITH_GFX
   virtual void drawMenu(MidiSource::MidiSender*, Piano*){};
   virtual void sendSnapshot(MidiSource::MidiSender*){};
   virtual void sendSnapshotWithTrigger(MidiSource::MidiSender*){};
   virtual bool toggleEnumPopup(uint8_t cc_num){return false;}
   virtual void selectNextPopupItem(){};
   virtual void selectPrevPopupItem(){};
   virtual void activatePopupSelection(){};

#endif
};

#endif // GEOLEDIC_PROGRAM_HPP
