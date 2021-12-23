import sys
import programs
from colors import colors
from string import Template

def getProgram(classname):
    program_number = 0
    for prog in programs.get():
        if prog['program'] == classname:
            return prog, program_number
        program_number = program_number + 1
    raise Exception("Could not find program named %s" % classname)

def getPrototypes(program, classname):
    prototypes = []
    for cc in program['controls']:
        name = ''.join(e for e in cc['name'].title() if e.isalnum())
        if cc['type'] == 'toggle':
            cc['prototype'] = "bool %s::is%s() const" % (classname, name)
            prototypes.append("bool is%s() const;" % name)
        elif cc['type'] == 'enum':
            cc['enum_type'] = name
            cc['prototype'] = "%s::%s %s::get%s() const" % (classname, name, classname, name)
            prototypes.append("%s get%s() const;" % (name, name))
        else:
            cc['prototype'] = "uint8_t %s::get%s() const" % (classname, name)
            prototypes.append("uint8_t get%s() const;" % name)
    return prototypes

def getEnums(program):
    enums = []
    for cc in program['controls']:
        if cc['type'] != 'enum':
            continue
        enum = "enum %s\n    {\n" % cc['enum_type']
        for i in range(0, len(cc['enums'])):
            enum = enum + "        %s_%s" % (cc['enum_type'], cc['enums'][i])
            if i < len(cc['enums']) - 1:
                enum = enum + ",\n"
            else:
                enum = enum + "\n    };"
        enums.append(enum)
    return enums
        
def getImplementations(program):
    implementations = []
    snapshots = ""
    defaults = ""
    for cc in program['controls']:
        getter = "getControlValue(%d)" % cc['number']
        impl = cc['prototype'] + "\n{\n"
        if cc['type'] == 'toggle':
            impl = impl + "    return %s > 63;\n}" % getter
            if 'default' in cc and cc['default']:
                defaults = defaults + "    setControlValue(%u, %u); // default for %s\n" % (cc['number'], 127 ,cc['name'])
        elif cc['type'] == 'enum':
            impl = impl + "    uint8_t v = %s;\n   " % getter
            for i in range(len(cc['enums']) - 1, 0, -1):
                impl = impl + " if (v >= %d)\n    {\n" % cc['values'][i]
                impl = impl + "        return %s_%s;\n    }\n    else" % (cc['enum_type'], cc['enums'][i])
            impl = impl + "\n    {\n"
            impl = impl + "        return %s_%s;\n    }\n}" % (cc['enum_type'], cc['enums'][0])
            if 'default' in cc and cc['default']:
                defaults = defaults + "    setControlValue(%u, %d); // default for %s\n" % (cc['number'], cc['values'][cc['enums'].index(cc['default'])] ,cc['name'])

        else:
            impl = impl + "    return %s;\n}" % getter
            if 'default' in cc:
                defaults = defaults + "    setControlValue(%u, %u); // default for %s\n" % (cc['number'],cc['default'],cc['name'])
        snapshots = snapshots + "    sender->sendControlChange(%u, m_control_values[%u]);\n" % (cc['number'], cc['number'])
        implementations.append(impl)
    # add constructor with defaults at beginning
    implementations.insert(0, "%s::%s()\n{\n%s}" % (program['program'], program['program'], defaults))
    return implementations, snapshots

IMGUI_SLIDER_TEMPLATE = '''
    {
        const uint8_t cmin = $min;
        const uint8_t cmax = $max;
        if (++ix % 8 != 0) ImGui::SameLine();
        if (ImGui::VSliderU8WithText("$name", "$description",
                                     ImVec2(20, 200), &m_control_values[$cc_num], cmin, cmax, ix * HUE_STEP_PER_SLIDER))
        {
            if (sender) sender->sendControlChange($cc_num, m_control_values[$cc_num]);
        }
    }
'''

IMGUI_TOGGLE_TEMPLATE = '''
    {
        bool v = m_control_values[$cc_num] >= 64;
        bool prev = v;
        ImGui::Checkbox("$name", &v);
        if (v != prev)
        {
            m_control_values[$cc_num] = v ? 127 : 0;
           if (sender) sender->sendControlChange($cc_num, m_control_values[$cc_num]);
        }
    }
'''

IMGUI_ENUM_TEMPLATE = '''
    {
        const char* items[] = {
            $enums
        };
        const uint8_t values[] = { $values };
        int index = 0;
        uint8_t value = m_control_values[$cc_num];
        for (index = sizeof(values)-1; index >= 0; index--)
        {
           if (values[index] <= value)
           {
              break;
           }
        }
        
        const char* combo_preview_value = items[index];
        if (ImGui::BeginCombo("$name", combo_preview_value))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (index == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    index = n;
                    m_control_values[$cc_num] = values[index];
                    if (sender) sender->sendControlChange($cc_num, m_control_values[$cc_num]);
                }
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
'''

def getKeyZones(program):
    if not 'keyzones' in program:
        return ''

    channels = []
    for chan in program['keyzones']:
        zones = []
        for z in chan:
            col = z['color'] if 'color' in z else 'Red'
            zones.append("KeyZone(%d, %d, 0x%06x, \"%s\")" % (z['from'], z['to'], colors[col], z['name']))
        
        channels.append("{\n   " + ",\n   ".join(zones) + "\n}")
    return ",\n".join(channels)

def getMenu(program):
    menu = ""
    for cc in program['controls']:
        name = "CC%d: %s" % (cc['number'], cc['name'])
        if cc['type'] == 'toggle':
            menu = menu + Template(IMGUI_TOGGLE_TEMPLATE).substitute(
               name=name,
               cc_num=cc['number'])
            if 'description' in cc:
                menu = menu + '    ImGui::SameLine(); HelpMarker("%s");\n' % cc['description']

        elif cc['type'] == 'enum':
            enum_strings = []
            for k in range(0, len(cc['enums'])):
                enum_strings.append("%s (%u)" % (cc['enums'][k], cc['values'][k]))
            menu = menu + Template(IMGUI_ENUM_TEMPLATE).substitute(
               name=name,
               cc_num=cc['number'],
               enums='"' + '",\n            "'.join(enum_strings) + '"',
               values=', '.join(str(x) for x in cc['values']))
            if 'description' in cc:
                menu = menu + '    ImGui::SameLine(); HelpMarker("%s");\n' % cc['description']

        else:
            menu = menu + Template(IMGUI_SLIDER_TEMPLATE).substitute(
               name=name,
               description=cc['description'] if 'description' in cc else '',
               cc_num=cc['number'],
               min=cc['min'],
               max=cc['max'])
        
    return menu

command = sys.argv[2] if len(sys.argv) > 2 else ''
basename = sys.argv[1]

if command == '--sources':
    filenames = ''
    for p in programs.get():
        filenames = filenames + "%s/%s.generated.cpp;" % (basename, p['program'])
    # don't use print as we don't want a newline
    sys.stdout.write(filenames[:-1])
    exit(1)


header    = basename.replace('.generated.cpp','.hpp')
classname = basename.replace('.generated.cpp','').split('/')[-1]

CPP_TEMPLATE = '''
#include "${classname}.hpp"
#ifdef WITH_GFX
#include "ImGui.hpp"
#include "Piano.hpp"
#include <unistd.h>

namespace {
std::vector<std::vector<KeyZone> > key_zones = {
$keyzones
};

const uint8_t HUE_STEP_PER_SLIDER = 24;
}

#endif


namespace generated {

$implementations

void ${classname}::run()
{
    ${base}::run();
    runProgram();
}

#ifdef WITH_GFX
void ${classname}::drawMenu(MidiSource::MidiSender* sender, Piano* piano)
{
    uint8_t ix = 0;
    {
        const uint8_t cmin = 0;
        const uint8_t cmax = 127;
        if (ImGui::VSliderU8WithText("CC7: Brightness", "The brightness is preserved on program change, unlike the other controllers",
                                     ImVec2(20, 200), &m_brightness_raw, cmin, cmax, ix * HUE_STEP_PER_SLIDER))
        {
            if (sender) sender->sendControlChange(BRIGHTNESS_CC, m_brightness_raw);
        }
    }
$menu
    {
        bool v = getControlValue(FORCE_BLANK_CC) > 0;
        bool prev = v;
        ImGui::Checkbox("CC80: Force Blank", &v);
        if (v != prev)
        {
            uint8_t val = v ? 127 : 0;
            setControlValue(FORCE_BLANK_CC, val);
            if (sender) sender->sendControlChange(FORCE_BLANK_CC, val);
        }
        ImGui::SameLine(); HelpMarker("Force all LEDs to black, resume program when un-forced");
    }

    if (piano)
    {
        piano->draw(this, key_zones, sender);
    }
}

void ${classname}::sendSnapshot(MidiSource::MidiSender* sender)
{
    if (sender == nullptr) return;
    
    // dummy controller to trigger overdub. Must be learned in DAW!
    sender->sendControlChange(ARM_OVERDUB_CC, 127);

    sender->sendProgramChange($program_number);
    sender->sendControlChange(BRIGHTNESS_CC, m_brightness_raw);
$control_snapshot
}

#endif

}
'''

HPP_TEMPLATE = '''
#ifndef _GENERATED_${classname}_HPP
#define _GENERATED_${classname}_HPP

#include "$base.hpp"

namespace generated {

class $classname: public $base
{
public:
    $classname();
    virtual ~$classname(){}
    virtual void runProgram() = 0;

    $enums

    $prototypes

    virtual void run();
#ifdef WITH_GFX
    virtual void drawMenu(MidiSource::MidiSender* sender, Piano* piano);
    virtual void sendSnapshot(MidiSource::MidiSender* sender);
#endif
};

}
#endif
'''

program, program_number = getProgram(classname)
prototypes = getPrototypes(program, classname)
enums = getEnums(program)
impls, control_snapshots = getImplementations(program)
menu  = getMenu(program)
base = program['base'] if 'base' in program else 'Controls'

with open(basename, 'w') as file:
    file.write(Template(CPP_TEMPLATE).substitute(
        classname=classname,
        base=base,
        menu=menu,
        keyzones=getKeyZones(program),
        implementations="\n".join(impls),
        program_number=program_number,
        control_snapshot=control_snapshots))

with open(header, 'w') as file:
    file.write(Template(HPP_TEMPLATE).substitute(
        classname=classname,
        base=base,
        prototypes="\n    ".join(prototypes),
        enums="\n    ".join(enums)))
