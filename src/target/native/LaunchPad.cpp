#include "LaunchPad.hpp"
#include "ControllerInfo.hpp"
#include <algorithm>
#include <iostream>

namespace {
const uint8_t HEADER_LENGTH = 6;
const unsigned MAX_SYSEX_DATA_LENGTH = 256;
const uint8_t SYSEX_HEADER[HEADER_LENGTH] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D};
const uint8_t SYSEX_END = 0xF7;

enum SysexCommand 
{
    PROGRAMMER_MODE = 0x0E,
    SET_COLORS = 0x03,
    SET_TEXT = 0x07
};

enum TopRowFunctions
{
    PREV_PAGE = 2,
    NEXT_PAGE = 3
};

const CRGB PAGE_ACTIVE(CRGB::Magenta);
const CRGB dim(const CRGB& c, uint8_t brightness = 10)
{
    CRGB out(c);
    out.nscale8(brightness);
    return out;
}


}

class Fader
{
public:
    Fader(uint8_t cc_num,
          uint8_t min,
          uint8_t max,
          CRGB color,
          PadColor* pads,
          MidiMessageSink& to_geoledic):
        m_cc_num(cc_num),
        //m_min(min),
        //m_max(max),
        m_color(color),
        m_pads(pads),
        m_to_geoledic(to_geoledic),
        m_value(0)
    {
        (void)min;
        (void)max;
    }

    ~Fader()
    {
        for (unsigned k = 0; k < 8; k++)
        {
            m_pads[k] = CRGB::Black;
        }
    }

    void update(uint8_t value)
    {
        for (unsigned k = 0; k < 8; k++)
        {
            if (value/18 > k)
            {
                m_pads[k] = m_color;
            }
            else if (value/18 == k)
            {
                m_pads[k] = dim(m_color, (value % 18)*14 + 17);
            }
            else
            {
                m_pads[k] = CRGB::Black;
            }
        }
        m_value = value;
    }

    void pushPad(uint8_t row, bool fine_resolution)
    {
        uint8_t val =       row == 7 ? 127 : row * 18;
        uint8_t upper_val = row >= 6 ? 127 : (row+1) * 18;
        if (m_value >= val && m_value < upper_val)
        {
            m_value += (fine_resolution ? 1 : 6);
            if (m_value >= upper_val)
            {
                m_value = val;
            }
        }
        else
        {
            m_value = val;
        }

        MidiMessage msg;
        msg.data[0] = MidiMessage::CONTROL_CHANGE << 4;
        msg.data[1] = m_cc_num;
        msg.data[2] = m_value;
        msg.length = 3;
        m_to_geoledic.sink(msg);
    }

private:
    unsigned m_cc_num;
    //unsigned m_min;
    //unsigned m_max;
    CRGB m_color;
    PadColor* m_pads;
    MidiMessageSink& m_to_geoledic;
    uint8_t m_value;
};


union LaunchPad::SysexMsg
{
    SysexMsg()
    {
        std::copy_n(SYSEX_HEADER, HEADER_LENGTH, raw.header);
    }

    MidiMessage msg;
    struct {
        uint16_t length;
        uint8_t header[HEADER_LENGTH];
        uint8_t command;
        uint8_t data[MAX_SYSEX_DATA_LENGTH];
    } raw;
}  __attribute__((packed));

PadColor::PadColor():
    m_dirty(true),
    m_color(CRGB::Black)
{
}

void PadColor::operator=(const CRGB& color)
{
    if (m_color != color)
    {
        m_dirty = true;
        m_color = color;
    }
}
  
template <>
void Page<LaunchPad::NUM_COLS, LaunchPad::NUM_ROWS>::setDirty()
{
    for (PadColor (&row)[LaunchPad::NUM_COLS]: m_pads)
    {
        for (PadColor& pad: row)
        {
            pad.m_dirty = true;
        }
    }
}

LaunchPad::LaunchPad(MidiMessageSink& to_launchpad, MidiMessageSink& to_geoledic):
    m_to_launchpad(to_launchpad),
    m_to_geoledic(to_geoledic),
    m_sysex_message(*new SysexMsg()),
    m_top_row(),
    m_side_col(),
    m_pages(),
    m_current_page(),
    m_current_page_ix(),
    m_fine_fader_resolution(false)
{
    enterMode(PROGRAMMER);
    sendText("Yeehaw!");
}

LaunchPad::~LaunchPad()
{
    enterMode(LIVE);
    delete &m_sysex_message;
}

void LaunchPad::enterMode(Mode mode)
{
    m_sysex_message.raw.command = PROGRAMMER_MODE;
    m_sysex_message.raw.data[0] = mode == PROGRAMMER ? 1 : 0;
    m_sysex_message.raw.data[1] = SYSEX_END;
    m_sysex_message.raw.length = HEADER_LENGTH+3;    
    m_to_launchpad.sink(m_sysex_message.msg);
}

void LaunchPad::sendText(const char* text, const CRGB& color)
{
    m_sysex_message.raw.command = SET_TEXT;
    m_sysex_message.raw.data[0] = 0; // don't loop
    m_sysex_message.raw.data[1] = 20; // speed
    m_sysex_message.raw.data[2] = 1; // use RGB
    m_sysex_message.raw.data[3] = color.r/2;
    m_sysex_message.raw.data[4] = color.g/2;
    m_sysex_message.raw.data[5] = color.b/2;
    uint8_t* p = reinterpret_cast<uint8_t*>(stpcpy(reinterpret_cast<char*>(m_sysex_message.raw.data+6), text));
    *p++ = SYSEX_END;
    m_sysex_message.raw.length = p - m_sysex_message.raw.header;
    m_to_launchpad.sink(m_sysex_message.msg);
}

bool LaunchPad::addPadColor(uint8_t*& p, PadColor& pad, unsigned col, unsigned row)
{
    if (p - m_sysex_message.raw.data + 6 > MAX_SYSEX_DATA_LENGTH)
    {
        // the 5 bytes for the color item plus the end marker won't fit,
        // send the remainder in the next message
        return false;
    }

    if (not pad.m_dirty) return true;

    *p++ = 0x3; // use RGB
    *p++ = (row + 1) * 10 + col + 1;
    *p++ = pad.m_color.r/2;
    *p++ = pad.m_color.g/2;
    *p++ = pad.m_color.b/2;

    pad.m_dirty = false;
    return true;
}

void LaunchPad::sendColors()
{
    m_sysex_message.raw.command = SET_COLORS;
    uint8_t* p = m_sysex_message.raw.data;

    for (unsigned col = 0; col < NUM_COLS; col++)
    {
        if (not addPadColor(p, m_top_row[col], col, NUM_ROWS)) break;
    }

    for (unsigned row = 0; row < NUM_ROWS; row++)
    {
        if (not addPadColor(p, m_side_col[row], NUM_COLS, row)) break;
    }

    for (unsigned row = 0; row < NUM_ROWS; row++)
    {
        for (unsigned col = 0; col < NUM_COLS; col++)
        {
            if (not addPadColor(p, m_current_page->m_pads[col][row], col, row)) break;
        }
    }
    if (p == m_sysex_message.raw.data) return; // no messages added

    *p++ = SYSEX_END;
    m_sysex_message.raw.length = p - m_sysex_message.raw.header;
    m_to_launchpad.sink(m_sysex_message.msg);
}

void LaunchPad::updateFromMidi(const MidiMessage& msg)
{
    if (msg.type() == MidiMessage::CONTROL_CHANGE)
    {
        if (m_faders_by_cc.count(msg.data[1]) == 0) return;

        m_faders_by_cc[msg.data[1]]->update(msg.data[2]);
    }
    else if (msg.type() == MidiMessage::PROGRAM_CHANGE)
    {
        uint8_t program_num = msg.data[1];
        m_faders.clear();
        m_faders_by_cc.clear();
        m_pages.clear();
        auto this_page = &m_pages.back();
        for (const ControlChangeParams& p: getFadersForProgram(program_num))
        {
            unsigned index = m_faders.size();
            if (index % NUM_COLS == 0)
            {
                m_pages.resize(m_pages.size() + 1);
                this_page = &m_pages.back();
            }

            m_faders_by_cc[p.cc_num] = std::make_shared<Fader>(
                p.cc_num, 
                p.min, 
                p.max, 
                CHSV(index*24, 255, 255), 
                this_page->m_pads[index % NUM_COLS], 
                m_to_geoledic);
            m_faders.push_back(m_faders_by_cc[p.cc_num]);
        }
        m_current_page = m_pages.begin();
        m_current_page_ix = 0;
        // if we have more than 1 page, enable paging buttons in top row
        if (m_pages.size() > 1)
        {
            m_top_row[PREV_PAGE] = dim(PAGE_ACTIVE);
            m_top_row[NEXT_PAGE] = PAGE_ACTIVE;
        }
        else
        {
            m_top_row[PREV_PAGE] = CRGB::Black;
            m_top_row[NEXT_PAGE] = CRGB::Black;
        }
    }
}

void LaunchPad::updateFromCtrl(const MidiMessage& msg)
{
    if (msg.type() == MidiMessage::NOTE_ON || msg.type() == MidiMessage::CONTROL_CHANGE)
    {
        if (msg.data[1] == 29)
        {
            m_fine_fader_resolution = msg.data[2] > 0;
            return;
        }

        if (msg.data[2] == 0) return;

        uint8_t row = (msg.data[1] / 10) - 1;
        uint8_t col = (msg.data[1] % 10) - 1;

        if (row == NUM_ROWS)
        {
            // top row
            if (col == 2 and m_current_page_ix != 0)
            { 
                --m_current_page;
                --m_current_page_ix;
                m_current_page->setDirty();
                m_top_row[NEXT_PAGE] = PAGE_ACTIVE;
                if (m_current_page_ix == 0)
                {
                    m_top_row[PREV_PAGE] = dim(PAGE_ACTIVE);
                }
            }
            else if (col == 3 and m_current_page_ix != m_pages.size() - 1) 
            {
                ++m_current_page;
                ++m_current_page_ix;
                m_current_page->setDirty();
                m_top_row[PREV_PAGE] = PAGE_ACTIVE;
                if (m_current_page_ix == m_pages.size() - 1)
                {
                    m_top_row[NEXT_PAGE] = dim(PAGE_ACTIVE);
                }
            }
        }
        else if (col < NUM_COLS and 
                 row < NUM_ROWS)
        {
            col += NUM_COLS * m_current_page_ix;
            if (col < m_faders.size())
            {
                m_faders[col]->pushPad(row, m_fine_fader_resolution);
            }
        }
    }
}

void LaunchPad::refreshUi()
{
    sendColors();
}