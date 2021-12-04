#ifndef TARGET_NATIVE_CONTROLLERINFO_HPP
#define TARGET_NATIVE_CONTROLLERINFO_HPP

#include <vector>
#include <stdint.h>

struct ControlChangeParams 
{
    ControlChangeParams(uint8_t cc_num, uint8_t min, uint8_t max):
        cc_num(cc_num),
        min(min),
        max(max)
    {}
    uint8_t cc_num;
    uint8_t min;
    uint8_t max;
};

std::vector<ControlChangeParams> getFadersForProgram(uint8_t program_num);

#endif // TARGET_NATIVE_CONTROLLERINFO_HPP