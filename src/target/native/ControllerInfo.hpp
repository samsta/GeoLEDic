#ifndef TARGET_NATIVE_CONTROLLERINFO_HPP
#define TARGET_NATIVE_CONTROLLERINFO_HPP

#include <vector>
#include <stdint.h>

struct ControlChangeParams 
{
    enum Style {
        NORMAL,
        HUE
    };

    ControlChangeParams(uint8_t cc_num, uint8_t min, uint8_t max, Style style = NORMAL):
        cc_num(cc_num),
        min(min),
        max(max),
        style(style)
    {}
    uint8_t cc_num;
    uint8_t min;
    uint8_t max;
    Style style;
};

struct ButtonParams
{
    enum Type {
        BUTTON,
        POPUP_TOGGLE
    };

    ButtonParams(uint8_t cc_num, Type type):
        cc_num(cc_num),
        type(type)
    {}
    uint8_t cc_num;
    Type type;
};

std::vector<ControlChangeParams> getFadersForProgram(uint8_t program_num);
std::vector<ButtonParams> getButtonsForProgram(uint8_t program_num);

#endif // TARGET_NATIVE_CONTROLLERINFO_HPP