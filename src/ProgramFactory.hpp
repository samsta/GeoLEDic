#ifndef GEOLEDIC_PROGRAM_FACTORY
#define GEOLEDIC_PROGRAM_FACTORY

#ifdef WITH_GFX
#include <pthread.h>
#endif

#include "DomeWrapper.hpp"
#include "Strips.hpp"
#include "Program.hpp"

class Piano;

class ProgramFactory
{
public:
   ProgramFactory(const DomeWrapper& dome, const Strips& strips);
   ~ProgramFactory();
   void changeProgram(uint8_t program);
   Program& program();

   void lock();
   void unlock();

#ifdef WITH_GFX
   void drawMenu(MidiSource::MidiSender* sender, Piano* piano);
private:
   pthread_mutex_t m_program_mutex;
#endif

private:
   
   DomeWrapper m_dome;
   Strips m_strips;

   Program* m_current_program;
   uint8_t  m_program_number;
};

#endif // GEOLEDIC_PROGRAM_FACTORY
