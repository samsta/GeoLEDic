#ifndef ImGui_h
#define ImGui_h

#include "imgui/imgui.h"


namespace ImGui {

void HelpMarker(const char* message);

bool VSliderU8WithText(const char* label, const char* description, const ImVec2& size, uint8_t* v, uint8_t v_min, uint8_t v_max, uint8_t hue);

}

inline void HelpMarker(const char* message)
{
    return ImGui::HelpMarker(message);
}


#endif /* ImGui_h */
