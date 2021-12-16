import sys
import programs
from string import Template


CONTROLLER_INFO_TEMPLATE = '''
// autogenerated by $scriptname
#include "ControllerInfo.hpp"


std::vector<ControlChangeParams> getFadersForProgram(uint8_t program_num)
{
    switch (program_num)
    {
    $faders
    default:
        return {};
    }
}

std::vector<uint8_t> getButtonsForProgram(uint8_t program_num)
{
    switch (program_num)
    {
    $buttons
    default:
        return {};
    }
}

'''

CASE_TEMPLATE = '''
    case $num:
        $val

'''

filename = sys.argv[1]
includes = ''
fader_cases = ''
button_cases = ''

program_number = 0
for program in programs.get():

    faders = []
    faders.append("{ 7, 0, 127}")
    buttons  = []
    for cc in program['controls']:
        if cc['type'] == 'continuous':
            faders.append("{ %d, %d, %d}" % (cc['number'], cc['min'], cc['max']))
        elif cc['type'] == 'toggle':
            buttons.append("%d" % cc['number'])

    fader_cases += Template(CASE_TEMPLATE).substitute(
        num=program_number, 
        val = "return {" + ",".join(faders) + "};")
    button_cases += Template(CASE_TEMPLATE).substitute(
        num=program_number, 
        val = "return {" + ",".join(buttons) + "};")

    program_number += 1

with open(filename, 'w') as file:
    file.write(Template(CONTROLLER_INFO_TEMPLATE).substitute(
        scriptname=sys.argv[0],
        faders=fader_cases,
        buttons=button_cases))
