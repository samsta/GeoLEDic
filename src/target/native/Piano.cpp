#include "Piano.hpp"
#include "Program.hpp"
#include <iostream>

namespace {

bool is_black(int key)
{
    switch (key % 12)
    {
        case 0:
        case 2:
        case 4:
        case 5:
        case 7:
        case 9:
        case 11:
            return false;
        default:
            return true;
    }    
}

unsigned getKeyX(int key, int width)
{
    unsigned x = (key / 12) * 7 * width;
    const float offsets[12] = 
    {
        0, 0.75, 1, 1.75, 2, 3, 3.75, 4, 4.75, 5, 5.75, 6
    };
    return x + offsets[key % 12] * width;
}

const char* noteName(int key)
{
    static char name[16];
    char letters[] = "CCDDEFFGGAAB";
    snprintf(name, sizeof(name), "%c%s%d",letters[key%12], is_black(key) ? "#":"", key/12 - 2);
    return name;
}

}

std::string numberToNoteName(uint8_t num)
{
    return noteName(num);
}

KeyZone::KeyZone(uint8_t from, uint8_t to, uint32_t color, const char* name):
    from(from), to(to), name(name)
{
    col = ImColor(int(color >> 16), (color >> 8) & 0xff, color & 0xff);
}

Piano::Piano():
    m_show(false),
    m_last_clicked_key(-1),
    m_last_clicked_channel(-1),
    m_velocity(127)
{

}

void Piano::draw(Program* program, const std::vector<std::vector<KeyZone> >& key_zones, MidiSource::MidiSender* sender)
{
    ImGui::Separator();
    ImGui::Checkbox("Show Keyboard", &m_show);
    if (not m_show) return;

    ImU32 Black = IM_COL32(0, 0, 0, 255);
    ImU32 White = IM_COL32(255, 255, 255, 255);
    ImU32 Red = IM_COL32(255, 0, 0, 255);
    ImU32 Green = IM_COL32(0, 255, 0, 255);
    ImGui::SetNextWindowBgAlpha(0.2f);
    ImGui::Begin("Keyboard", &m_show, ImGuiWindowFlags_NoResize);

    int initial_y = ImGui::GetCursorPosY();

    const uint8_t cmin = 0;
    const uint8_t cmax = 127;
    ImGui::SliderScalar("Velocity", ImGuiDataType_U8, &m_velocity, &cmin, &cmax);

    unsigned num_channels = std::max(1u, unsigned(key_zones.size()));
    const int white_height = 60;
    const int black_height = 40;
    const int key_width = 12;
    const int zone_height = 3;
    const int num_keys = 128;
    const int window_padding = 32;

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    const ImVec2 m = ImGui::GetIO().MousePos;

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && m_last_clicked_key != -1)
    {
        program->noteOff(m_last_clicked_key, m_last_clicked_channel);
        if (sender)
        {
            sender->sendNote(m_last_clicked_key, 0, m_last_clicked_channel);
        }
        m_last_clicked_key = -1;
        m_last_clicked_channel = -1;
    }

    for (unsigned channel = 0; channel < num_channels; channel++)
    {
        ImGui::Text("Channel %u:", channel+1);
        ImVec2 p = ImGui::GetCursorScreenPos();
        int y = p.y + zone_height;
        int hovered_key = -1;
        for (int key = 0; key < num_keys; key++) {
            int x = p.x + getKeyX(key, key_width);
            ImU32 col = White;
            if (is_black(key)) continue;

            if (m.x > x and m.x < x + key_width and
                m.y > y + black_height and m.y < y + white_height)
            {
                col = Green;
                hovered_key = key;
                sendNoteIfClicked(key, channel, program, sender);
            }
            else if (m_channel_key_map[channel][key])
            {
                col = Red;
            }

            draw_list->AddRectFilled(
                    ImVec2(x, y),
                    ImVec2(x + key_width, y + white_height),
                    col, 0, ImDrawCornerFlags_All);
            draw_list->AddRect(
                    ImVec2(x, y),
                    ImVec2(x + key_width, y + white_height),
                    Black, 0, ImDrawCornerFlags_All);
        }

        for (int key = 0; key < num_keys; key++) {
            if (not is_black(key)) continue;
            
            int x = p.x + getKeyX(key, key_width);

            ImU32 col = Black;
            if (m.x > x and m.x < x + key_width and
                m.y > y and m.y < y + black_height)
            {
                col = Green;
                hovered_key = key;
                sendNoteIfClicked(key, channel, program, sender);
            }
            else if (m_channel_key_map[channel][key])
            {
                col = Red;
            }

            draw_list->AddRectFilled(
                    ImVec2(x, y),
                    ImVec2(x + key_width/2 , y + black_height),
                    col, 0, ImDrawCornerFlags_All);
            draw_list->AddRect(
                    ImVec2(x, y),
                    ImVec2(x + key_width/2 , y + black_height),
                    Black, 0, ImDrawCornerFlags_All);
            x += key_width/2;
        }

        const char* zone_name = "unmapped";
        for (const KeyZone& kz: key_zones[channel])
        {
            int y1 = p.y;
            int y2 = p.y + white_height + zone_height;
            int from = p.x + getKeyX(kz.from, key_width);
            int to   = p.x + getKeyX(kz.to, key_width) + (is_black(kz.to) ? key_width/2 : key_width);
            draw_list->AddRectFilled(
                    ImVec2(from, y1),
                    ImVec2(to, y1 + zone_height),
                    kz.col, 
                    0, ImDrawCornerFlags_All);
            draw_list->AddRectFilled(
                    ImVec2(from, y2),
                    ImVec2(to, y2 + zone_height),
                    kz.col, 
                    0, ImDrawCornerFlags_All);
            ImColor col_transparent = kz.col;
            col_transparent.Value.w = 0.2;
            draw_list->AddRectFilled(
                    ImVec2(from, y1 + zone_height),
                    ImVec2(to, y2),
                    col_transparent, 
                    0, ImDrawCornerFlags_All);

            if (hovered_key >= kz.from && hovered_key <= kz.to)
            {
                zone_name = kz.name;
            }
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + white_height + 2*zone_height + 1);
        if (hovered_key >= 0)
        {
            ImGui::Text("%s (%d) - %s", noteName(hovered_key), hovered_key, zone_name);
        }
        else
        {
            ImGui::Text(" ");
        }

        if (channel + 1 < num_channels)
        {
            ImGui::Separator();
        }
    }
    ImGui::SetWindowSize(ImVec2(getKeyX(num_keys, key_width) + window_padding, window_padding +ImGui::GetCursorPosY() - initial_y));
    ImGui::End();
}

void Piano::sendNoteIfClicked(int key, unsigned channel, Program* program, MidiSource::MidiSender* sender)
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (m_last_clicked_key != -1)
        {
            program->noteOff(m_last_clicked_key, m_last_clicked_channel);
            if (sender)
            {
                sender->sendNote(m_last_clicked_key, 0, m_last_clicked_channel);
            }
        }
        program->noteOn(key, m_velocity, channel);
        if (sender)
        {
            sender->sendNote(key, m_velocity, channel);
        }
        m_last_clicked_key = key;
        m_last_clicked_channel = channel;
    }
}

void Piano::noteOn(uint8_t note, uint8_t velocity, uint8_t channel)
{
    m_channel_key_map[channel][note] = velocity;
}

void Piano::noteOff(uint8_t note, uint8_t channel)
{
    m_channel_key_map[channel][note] = 0;
}
