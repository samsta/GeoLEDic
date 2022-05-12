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

static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
{
    ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1,         pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32(0,0,0,alpha8));
    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x,             pos.y), half_sz,                              ImGuiDir_Right, IM_COL32(255,255,255,alpha8));
    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left,  IM_COL32(0,0,0,alpha8));
    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x,     pos.y), half_sz,                              ImGuiDir_Left,  IM_COL32(255,255,255,alpha8));
}

bool VSliderScalarWithText(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, SliderStyle slider_style)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const float value_label_height = 20;
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos + ImVec2(0, value_label_height), window->DC.CursorPos + size);
    const ImRect bb(frame_bb.Min - ImVec2(0, value_label_height), frame_bb.Max + ImVec2(label_size.y > 0.0f ? style.ItemInnerSpacing.x + label_size.y : 0.0f, 0.0f));

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

    if (slider_style == SliderStyle_NORMAL)
    {
        // Render grab
        if (grab_bb.Max.y > grab_bb.Min.y)
        {
            grab_bb.Max.y = frame_bb.Max.y - style.FramePadding.y;
            window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);
        }
    }
    else
    {
        uint8_t alpha = 255 * (hovered ? 0.5 : 0.3);
        const ImU32 COL_HUES[6 + 1] = {
            IM_COL32(255,0,0,alpha), 
            IM_COL32(255,0,255,alpha), 
            IM_COL32(0,0,255,alpha), 
            IM_COL32(0,255,255,alpha), 
            IM_COL32(0,255,0,alpha), 
            IM_COL32(255,255,0,alpha), 
            IM_COL32(255,0,0,alpha) 
            };

        // Render Hue Bar
        float bar_height = frame_bb.Max.y - frame_bb.Min.y;
        float bar_width = frame_bb.Max.x - frame_bb.Min.x;
        for (int i = 0; i < 6; ++i)
            window->DrawList->AddRectFilledMultiColor(ImVec2(frame_bb.Min.x, frame_bb.Min.y + i * (bar_height / 6)), 
                                                      ImVec2(frame_bb.Max.x, frame_bb.Min.y + (i + 1) * (bar_height / 6)), 
                                                      COL_HUES[i], COL_HUES[i], COL_HUES[i + 1], COL_HUES[i + 1]);
        float triangle_y = (grab_bb.Max.y + grab_bb.Min.y) / 2;
        RenderArrowsForVerticalBar(window->DrawList, ImVec2(frame_bb.Min.x, triangle_y), ImVec2(bar_width * 0.2 + 1, bar_width * 0.2), bar_width, 1);
    }
    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    // For the vertical slider we allow centered text to overlap the frame padding
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y - value_label_height), frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.0f));
    if (label_size.y > 0.0f)
    {
        AddTextVertical(window->DrawList, label, ImVec2(frame_bb.Max.x, frame_bb.Max.y), IM_COL32(255, 255, 255, 255));
    }

    return value_changed;
}

bool VSliderU8WithText(const char* label, const char* help, const ImVec2& size, uint8_t* v, uint8_t v_min, uint8_t v_max, uint8_t hue, SliderStyle style)
{
    if (style == SliderStyle_NORMAL) 
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(hue / 256.0f, 0.5f, 0.5f, 0.3f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(hue / 256.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(hue / 256.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(hue / 256.0f, 0.9f, 0.9f, 0.4f));
        ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 3);
    } 
    else
    {
        ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 1);
    }

    bool rv = VSliderScalarWithText(label, size, ImGuiDataType_U8, v, &v_min, &v_max, "%u", 0, style);
    if (help != nullptr  && *help != 0 && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(help);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::PopStyleVar();
    if (style == SliderStyle_NORMAL) 
    {
        ImGui::PopStyleColor(4);
    }
    return rv;
}

void OpenPopupByLabel(const char* label)
{
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiID id = window->GetID(label);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    OpenPopup(popup_id, 0);
}

}