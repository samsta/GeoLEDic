#include "ImGui.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

namespace ImGui {

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void AddTextVertical(ImDrawList* DrawList, const char *text, ImVec2 pos, ImU32 text_color) {
    pos.x = IM_ROUND(pos.x);
    pos.y = IM_ROUND(pos.y);
    ImFont *font = GImGui->Font;
    const ImFontGlyph *glyph;
    char c;
    while ((c = *text++)) {
        glyph = font->FindGlyph(c);
        if (!glyph) continue;

        DrawList->PrimReserve(6, 4);
        DrawList->PrimQuadUV(
                ImVec2(pos.x + glyph->Y0, pos.y - glyph->X0),
                ImVec2(pos.x + glyph->Y0, pos.y - glyph->X1),
                ImVec2(pos.x + glyph->Y1, pos.y - glyph->X1),
                ImVec2(pos.x + glyph->Y1, pos.y - glyph->X0),

                ImVec2(glyph->U0, glyph->V0),
                ImVec2(glyph->U1, glyph->V0),
                ImVec2(glyph->U1, glyph->V1),
                ImVec2(glyph->U0, glyph->V1),
                    text_color);
        pos.y -= glyph->AdvanceX;
    }
}

bool VSliderScalarWithText(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.y > 0.0f ? style.ItemInnerSpacing.x + label_size.y : 0.0f, 0.0f));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id);
    if ((hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || g.NavActivateInputId == id)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Up) | (1 << ImGuiDir_Down);
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags | ImGuiSliderFlags_Vertical, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.y > grab_bb.Min.y)
    {
        grab_bb.Max.y = frame_bb.Max.y - style.FramePadding.y;
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);
    }
    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    // For the vertical slider we allow centered text to overlap the frame padding
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.0f));
    if (label_size.y > 0.0f)
    {
        AddTextVertical(window->DrawList, label, ImVec2(frame_bb.Max.x, frame_bb.Max.y - style.FramePadding.y), IM_COL32(255, 255, 255, 255));
    }

    return value_changed;
}

bool VSliderU8WithText(const char* label, const char* help, const ImVec2& size, uint8_t* v, uint8_t v_min, uint8_t v_max, uint8_t hue)
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(hue / 256.0f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(hue / 256.0f, 0.6f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(hue / 256.0f, 0.7f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(hue / 256.0f, 0.9f, 0.9f, 0.4f));
    ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 3);

    bool rv = VSliderScalarWithText(label, size, ImGuiDataType_U8, v, &v_min, &v_max, "%u", 0);
    if (help != nullptr  && *help != 0 && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(help);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);
    return rv;
}



}