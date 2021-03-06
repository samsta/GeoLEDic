#ifndef GeoLEDic_h
#define GeoLEDic_h

#include "Dome.hpp"
#include "MidiSource.hpp"
#include "ProgramFactory.hpp"

void loopGeoLEDic();

MidiSource& getMidiSource();
ProgramFactory& getProgramFactory();

void registerNoteObserver(MidiNoteObserver* observer);

#endif /* GeoLEDic_h */
