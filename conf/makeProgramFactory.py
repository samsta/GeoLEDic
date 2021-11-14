import sys
import programs
from string import Template


FACTORY_TEMPLATE = '''
// autogenerated by $scriptname

// prune some stupid FastLED dependencies
#define __INC_PLATFORMS_H
#define HAS_HARDWARE_PIN_SUPPORT

#include "ProgramFactory.hpp"
#include "programs/Diagnostic.hpp"
#ifdef WITH_GFX
#include "imgui/imgui.h"
#endif

$includes

ProgramFactory::ProgramFactory(const DomeWrapper& dome, const Strips& strips):
      m_dome(dome),
      m_strips(strips),
      m_current_program(nullptr)
{
}

void ProgramFactory::changeProgram(uint8_t program)
{
   delete m_current_program;
   // clear all LEDs
   m_strips.clear();
   switch (program)
   {
$cases
   default:
      m_current_program = new Diagnostic(m_dome, m_strips);
   }
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

#ifdef WITH_GFX
void ProgramFactory::drawMenu(MidiSource::MidiSender* sender, Piano* piano)
{
    const char* program_names[] = {
        $names
    };
    const char* combo_preview_value = "Diagnostic";
    if (m_program_number < IM_ARRAYSIZE(program_names))
    {
        combo_preview_value = program_names[m_program_number];
    }
    
    if (ImGui::BeginCombo("Program", combo_preview_value))
    {
        for (int n = 0; n < IM_ARRAYSIZE(program_names); n++)
        {
            const bool is_selected = (m_program_number == n);
            if (ImGui::Selectable(program_names[n], is_selected))
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

'''

CASE_TEMPLATE = '''
   case $num:
      m_current_program = new $program(m_dome);
      break;

'''

filename = sys.argv[1]
includes = ''
cases = ''

program_number = 0
names = []
for program in programs.get():
    includes += '#include "programs/%s.hpp"\n' % program['program']
    cases += Template(CASE_TEMPLATE).substitute(
        num=program_number, 
        program=program['program'])
    # program number in the UI is usually 1-based
    names.append("%3d - %s" % ((program_number+1), program['name']))
    program_number += 1

with open(filename, 'w') as file:
    file.write(Template(FACTORY_TEMPLATE).substitute(
        scriptname=sys.argv[0],
        includes=includes,
        names='"' + '",\n        "'.join(names) + '"',
        cases=cases))
