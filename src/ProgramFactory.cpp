
// prune some stupid FastLED dependencies
#define __INC_PLATFORMS_H
#define HAS_HARDWARE_PIN_SUPPORT

#include "ProgramFactory.hpp"
#include "programs/Diagnostic.hpp"
#ifdef WITH_GFX
#include "ImGui.hpp"
#include "imgui/imgui_internal.h"
#include <iostream>
#endif


ProgramFactory::ProgramFactory(const DomeWrapper& dome, const Strips& strips):
      m_dome(dome),
      m_strips(strips),
      m_current_program(nullptr)
{
#ifdef WITH_GFX
    m_program_popup_state = CLOSED;
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
        for (unsigned n = 0; n < NUM_PROGRAMS; n++)
        {
            const bool is_selected = (m_program_number == n);
            if (m_program_popup_state == OPEN && n == m_selected_popup_item) {
               ImGui::PushStyleColor(ImGuiCol_Text, 0xFF0000FF);
            }
            if (ImGui::Selectable(PROGRAM_NAMES[n], is_selected))
            {
               changeProgram(n);
               if (sender) m_current_program->sendSnapshot(sender);
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
            if (m_program_popup_state == OPEN && n == m_selected_popup_item) {
               ImGui::PopStyleColor();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::Separator();

   switch (m_program_popup_state)
   {
      case CLOSE_REQUESTED:
         ImGui::ClosePopupsExceptModals();
         m_program_popup_state = CLOSED;
         break;
      case OPEN_REQUESTED:
      {
         ImGui::OpenPopupByLabel("Program");
         m_program_popup_state = OPEN;
         break;
      }
      default:
         break;
   }
    
    program().drawMenu(sender, piano);
}

bool ProgramFactory::toggleProgramPopup()
{
   // need to remember action for next ImGui loop iteration
   //  as we can't interact with it outside of the loop
   switch (m_program_popup_state)
   {
      case OPEN:
         m_program_popup_state = CLOSE_REQUESTED;
         m_selected_popup_item = m_program_number;
         return false;
      case CLOSED:
         m_program_popup_state = OPEN_REQUESTED;
         return true;
      default:
         return false;
   }
}

bool ProgramFactory::toggleEnumPopup(uint8_t cc_num)
{
   if (m_current_program == nullptr) return false;
   return m_current_program->toggleEnumPopup(cc_num);
}


void ProgramFactory::selectNextPopupItem()
{
   if (m_program_popup_state == OPEN) {
      if (m_selected_popup_item + 1 < NUM_PROGRAMS) {
         m_selected_popup_item++;
      } else {
         m_selected_popup_item = 0;
      }
   } else if (m_current_program) {
      m_current_program->selectNextPopupItem();
   }
}

void ProgramFactory::selectPrevPopupItem()
{
   if (m_program_popup_state == OPEN) {
      if (m_selected_popup_item != 0) {
         m_selected_popup_item--;
      } else {
         m_selected_popup_item = NUM_PROGRAMS - 1;
      }
   } else if (m_current_program) {
      m_current_program->selectPrevPopupItem();
   }
}

void ProgramFactory::activatePopupSelection(MidiSource::MidiSender* sender)
{
   if (m_program_popup_state == OPEN)
   {
      changeProgram(m_selected_popup_item);
      if (sender) m_current_program->sendSnapshot(sender);
      ImGui::ClosePopupsExceptModals();
      m_program_popup_state = CLOSED;
   } else if (m_current_program) {
      m_current_program->activatePopupSelection();
   }
}


#endif

ProgramFactory::~ProgramFactory()
{
   delete m_current_program;
}

