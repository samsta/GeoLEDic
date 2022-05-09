
// prune some stupid FastLED dependencies
#define __INC_PLATFORMS_H
#define HAS_HARDWARE_PIN_SUPPORT

#include "ProgramFactory.hpp"
#include "programs/Diagnostic.hpp"
#ifdef WITH_GFX
#include "imgui/imgui.h"
#endif


ProgramFactory::ProgramFactory(const DomeWrapper& dome, const Strips& strips):
      m_dome(dome),
      m_strips(strips),
      m_current_program(nullptr)
{
#ifdef WITH_GFX
    pthread_mutex_init(&m_program_mutex, nullptr);
#endif
}

void ProgramFactory::changeProgram(uint8_t program, Policy policy)
{
   if (policy == ONLY_ON_CHANGE and
       program == m_program_number)
   {
       return;
   }
   
   // give other threads a way to prevent the program instance
   //  they're currently using from getting destroyed
   lock();
   delete m_current_program;
   unlock();
   // clear all LEDs
   m_strips.clear();
   doChangeProgram(program);
   m_program_number = program;
}

Program& ProgramFactory::program()
{
   // create default on first access
   if (m_current_program == nullptr)
   {
       changeProgram(0);
   }
   return *m_current_program;
}

void ProgramFactory::lock()
{
#ifdef WITH_GFX
    pthread_mutex_lock(&m_program_mutex);
#endif
}

void ProgramFactory::unlock()
{
#ifdef WITH_GFX
    pthread_mutex_unlock(&m_program_mutex);
#endif
}

#ifdef WITH_GFX
void ProgramFactory::drawMenu(MidiSource::MidiSender* sender, Piano* piano)
{
    const char* combo_preview_value = "Diagnostic";
    if (m_program_number < NUM_PROGRAMS)
    {
        combo_preview_value = PROGRAM_NAMES[m_program_number];
    }
    
    if (ImGui::BeginCombo("Program", combo_preview_value, ImGuiComboFlags_HeightLarge))
    {
        for (int n = 0; n < NUM_PROGRAMS; n++)
        {
            const bool is_selected = (m_program_number == n);
            if (ImGui::Selectable(PROGRAM_NAMES[n], is_selected))
            {
               changeProgram(n);
               if (sender) m_current_program->sendSnapshot(sender);
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Separator();
    
    program().drawMenu(sender, piano);
}
#endif

ProgramFactory::~ProgramFactory()
{
   delete m_current_program;
}

