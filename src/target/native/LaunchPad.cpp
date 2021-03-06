#include "LaunchPad.hpp"
#include "ControllerInfo.hpp"
#include "Controls.hpp"
#include "GeoLEDic.hpp" // yuck!
#include <algorithm>
#include <iostream>
#include <functional>

namespace {
const uint8_t HEADER_LENGTH = 6;
const unsigned MAX_SYSEX_DATA_LENGTH = 256;
const uint8_t SYSEX_HEADER_MINI_MK3[HEADER_LENGTH] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0D};
const uint8_t SYSEX_HEADER_PRO_MK3[HEADER_LENGTH] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x0E};
const uint8_t SYSEX_END = 0xF7;

enum SysexCommand 
{
    PROGRAMMER_MODE = 0x0E,
    SET_COLORS = 0x03,
    SET_TEXT = 0x07
};

namespace LpMini {
    enum TopRowFunctions
    {
        FADER_UP = 0,
        FADER_DOWN = 1,
        PREV_PAGE = 2,
        NEXT_PAGE = 3
    };

    enum SideColumnFunctions
    {
        DYNAMIC_BUTTON_FIRST = 7,
        DYNAMIC_BUTTON_LAST = 2,
        SEND_SNAPSHOT = 1,
        FORCE_BLANK = 0
    };
}

namespace LpPro {
    enum TopRowFunctions
    {
        PREV_PAGE = 0,
        NEXT_PAGE = 1,
        PROGRAM = 7
    };

    enum LeftColumnFunctions
    {
        FADER_UP = 7,
        FADER_DOWN = 6,
        PAUSE_MIDI = 1,
        FORCE_BLANK = 0
    };

    enum RightColumnFunctions
    {
        DYNAMIC_BUTTON_FIRST = 7,
        DYNAMIC_BUTTON_LAST = 2,
        SEND_SNAPSHOT = 0
    };
}

const CRGB COLOR_PAGE_BTN(CRGB::Magenta);
const CRGB COLOR_FADER_FINE_BTN(CRGB::LightBlue);
const CRGB COLOR_FORCE_BLANK_BTN(CRGB::Red);
const CRGB COLOR_SEND_SNAPSHOT_BTN(CRGB::Green);
const CRGB COLOR_PAUSE_MIDI_BTN_PLAYING(CRGB::Green);
const CRGB COLOR_PAUSE_MIDI_BTN_PAUSED(CRGB::Orange);
const CRGB COLOR_PROGRAM_BTN(CRGB::Plum);
const CRGB COLOR_SHIFT_BTN(CRGB::White);

const CRGB dim(const CRGB& c, uint8_t brightness = 50)
{
    CRGB out(c);
    out.nscale8_video(brightness);
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
          PadColor* takeover_button,
          MidiMessageSink& to_geoledic):
        m_cc_num(cc_num),
        m_min(min),
        m_max(max),
        m_color(color),
        m_pads(pads),
        m_takeover_button_pad(takeover_button),
        m_to_geoledic(to_geoledic),
        m_value(0),
        m_takeover_active(false)
    {
    }

    virtual ~Fader()
    {
        deactivateTakeOver();
        for (unsigned k = 0; k < NUM_PADS; k++)
        {
            m_pads[k] = CRGB::Black;
        }
    }

    virtual void update(uint8_t value)
    {
        unsigned span      = m_max - m_min + 1;
        unsigned curr = m_min;
        for (unsigned k = 0; k < NUM_PADS; k++)
        {
            unsigned next = (k+1) * span / NUM_PADS + m_min;
            if (next <= value)
            {
                m_pads[k] = m_color;
            }
            else if (curr > value)
            {
                m_pads[k] = CRGB::Black;
            }
            else
            {
                // the scaler is not what you'd expect because the brightness quickly
                //  saturates on the pad LEDs
                unsigned scale = 128 * (value - curr) / (next - curr) + 16;
                m_pads[k] = dim(m_color, scale);
            }

            curr = next;
        }
        m_value = value;
    }

    enum FaderMode {
        NORMAL,
        FINE_UP,
        FINE_DOWN
    };

    void pushPad(uint8_t row, FaderMode mode)
    {
        if (row > NUM_PADS)
        {
            // the first bottom row on the launchpad pro mk3
            deactivateTakeOver();
            return;
        }

        switch (mode)
        {
        default:
        case NORMAL:
        {
            uint8_t span      = m_max - m_min + 1;
            uint8_t val       = m_min + (unsigned(row) * span) / NUM_PADS;
            uint8_t upper_val = m_min + (unsigned(row + 1) * span) / NUM_PADS;
            const unsigned STEPS_PER_PAD = 3;
            const unsigned NUM_STEPS = NUM_PADS * STEPS_PER_PAD;
            uint8_t step      = (span + NUM_STEPS-1) / NUM_STEPS;

            if (m_value >= val && m_value < upper_val)
            {
                m_value += step;
                if (row == NUM_PADS-1 and m_value > m_max and m_value < m_max + step)
                {
                    m_value = m_max;
                }
                else if (m_value >= upper_val)
                {
                    m_value = val;
                }
            }
            else
            {
                if (row == NUM_PADS-1)
                {
                    m_value = m_max;
                }
                else
                {
                    m_value = val;
                }
            }
            break;
        }

        case FINE_UP:
            if (m_value < m_max)
            {
                m_value++;
            }
            break;

        case FINE_DOWN:
            if (m_value > m_min)
            {
                m_value--;
            }
            break;
        }

        activateTakeOver();

        m_to_geoledic.sink(MidiMessage::makeCC(m_cc_num, m_value));
    }

    void activateTakeOver()
    {
        if (m_takeover_active) return;
        if (m_takeover_button_pad) *m_takeover_button_pad = m_color;
        getMidiSource().setCcFilter(m_cc_num);
        m_takeover_active = true;
    }

    void deactivateTakeOver()
    {
        if (not m_takeover_active) return;
        if (m_takeover_button_pad) *m_takeover_button_pad = CRGB::Black;
        getMidiSource().clearCcFilter(m_cc_num);
        m_takeover_active = false;
    }

protected:
    unsigned m_cc_num;
    unsigned m_min;
    unsigned m_max;
    const unsigned NUM_PADS = 8;
    CRGB m_color;
    PadColor* m_pads;
    PadColor* m_takeover_button_pad;
    MidiMessageSink& m_to_geoledic;
    uint8_t m_value;
    bool    m_takeover_active;
};


class HueFader: public Fader
{
public:
    HueFader(
            uint8_t cc_num,
            uint8_t min,
            uint8_t max,
            CRGB color,
            PadColor* pads,
            PadColor* takeover_button,
            MidiMessageSink& to_geoledic):
        Fader(cc_num, min, max, color, pads, takeover_button, to_geoledic)
    {}

    virtual void update(uint8_t value)
    {
        unsigned span      = m_max - m_min + 1;
        unsigned curr = m_min;
        for (unsigned k = 0; k < NUM_PADS; k++)
        {
            unsigned next = (k+1) * span / NUM_PADS + m_min;
            if (next <= value or curr > value)
            {
                m_pads[k] = CHSV((256/NUM_PADS) * k + (256/(2*NUM_PADS)), 255, 100);
            }
            else
            {
                m_pads[k] = CHSV(value*2, 255, 255);
            }

            curr = next;
        }
        m_value = value;
    }
};


class Button
{
public:

    enum State {
        ACTIVE,
        INACTIVE,
        DISABLED
    };

    Button(PadColor& pad, const CRGB& color):
        m_pad(pad),
        m_color(color),
        m_inactive_color(dim(color))
    {
        setState(DISABLED);
    }

    void setInactiveColor(const CRGB& color)
    {
        m_inactive_color = color;
    }

    virtual ~Button()
    {
        setState(DISABLED);
    }

    void setState(State state)
    {
        switch (state)
        {
            case ACTIVE:
                m_pad = m_color;
                break;
            case INACTIVE:
                m_pad = m_inactive_color;
                break;
            case DISABLED:
            default:
                m_pad = CRGB::Black;
        }

        m_state = state;
    }

    State getState()
    {
        return m_state;
    }

    bool is(State state)
    {
        return getState() == state;
    }

    virtual void receive(uint8_t value)
    {
        if (not is(DISABLED))
        {
            setState(value ? ACTIVE : INACTIVE);
        }
    }

private:
    PadColor&  m_pad;
    const CRGB m_color;
    CRGB       m_inactive_color;
    State      m_state;
};

template <class HandlerClass>
class ButtonWithHandler: public Button
{
public:
    typedef void (HandlerClass::*HandlerFunction)(uint8_t, Button&);

    ButtonWithHandler(PadColor& pad, const CRGB& color, HandlerClass& cls, HandlerFunction handler):
        Button(pad, color),
        m_cls(cls),
        m_handler(handler)
    {}

    virtual void receive(uint8_t value)
    {
        (m_cls.*m_handler)(value, *this);
    }

private:
    HandlerClass& m_cls;
    HandlerFunction m_handler;
};

class ButtonWithFunction: public Button
{
public:

    ButtonWithFunction(PadColor& pad, const CRGB& color, std::function<void(uint8_t)> function):
        Button(pad, color),
        m_function(function)
    {}

    virtual void receive(uint8_t value)
    {
        m_function(value);
    }

private:
    std::function<void(uint8_t)> m_function;
};


class MidiButton: public Button
{
public:
    MidiButton(PadColor& pad, const CRGB& color, uint8_t cc_num, MidiMessageSink& to_geoledic):
        Button(pad, color),
        m_cc_num(cc_num),
        m_to_geoledic(to_geoledic)
    {
    }

    virtual void receiveFromGeoledic(uint8_t value) = 0;

protected:
    const uint8_t m_cc_num;
    MidiMessageSink& m_to_geoledic;
};

class MidiButtonWithFunction: public MidiButton
{
public:

    MidiButtonWithFunction(PadColor& pad, const CRGB& color, uint8_t cc_num, MidiMessageSink& to_geoledic, std::function<void(uint8_t)> function):
        MidiButton(pad, color, cc_num, to_geoledic),
        m_function(function)
    {
    }

    virtual void receiveFromGeoledic(uint8_t value) {

    }

    virtual void receive(uint8_t value)
    {
        m_function(value);
    }

private:
    std::function<void(uint8_t)> m_function;
};

class MidiToggleButton: public MidiButton
{
public:
    MidiToggleButton(PadColor& pad, const CRGB& color, uint8_t cc_num, MidiMessageSink& to_geoledic, bool* active = nullptr):
        MidiButton(pad, color, cc_num, to_geoledic),
        m_active(active)
    {
    }

    virtual void receiveFromGeoledic(uint8_t value)
    {
        setState(value ? ACTIVE : INACTIVE);
        if (m_active)
        {
            *m_active = value > 0;
        }
    }

    virtual void receive(uint8_t value)
    {
        // only care about note on
        if (value == 0) return;

        m_to_geoledic.sink(
            MidiMessage::makeCC(m_cc_num, is(ACTIVE) ? 0 : 127)); // toggle, so request opposite of current state!
    }

private:
    bool* m_active;
};

union LaunchPad::SysexMsg
{
    SysexMsg(Model model)
    {
        std::copy_n(model == MINI_MK3 ? SYSEX_HEADER_MINI_MK3 : SYSEX_HEADER_PRO_MK3, HEADER_LENGTH, raw.header);
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
    m_color(CRGB::Black),
    m_type(RGB)
{
}

PadColor::PadColor(const CRGB& color):
    m_dirty(true),
    m_color(color),
    m_type(RGB)
{
}

PadColor::PadColor(PaletteColor pulsing_color):
    m_dirty(true),
    m_type(PULSING)
{
    m_pal_color[0] = pulsing_color;
}

PadColor::PadColor(PaletteColor color1, PaletteColor color2):
    m_dirty(true),
    m_type(FLASHING)
{
    m_pal_color[0] = color1;
    m_pal_color[1] = color2;
}

void PadColor::operator=(const CRGB& color)
{
    if (m_color != color or m_type != RGB)
    {
        m_dirty = true;
        m_color = color;
        m_type = RGB;
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
    for (PadColor& pad: m_bottom_row)
    {
        pad.m_dirty = true;
    }
}

LaunchPad::LaunchPad(MidiMessageSink& to_launchpad, MidiMessageSink& to_geoledic, Model model):
    m_to_launchpad(to_launchpad),
    m_to_geoledic(to_geoledic),
    m_model(model),
    m_sysex_message(*new SysexMsg(m_model)),
    m_top_row(),
    m_right_col(),
    m_pages(),
    m_current_page(m_pages.end()),
    m_current_page_ix(),
    m_force_blank(false)
{
    enterMode(PROGRAMMER);
    sendText("Yeehaw!");

    if (model == MINI_MK3)
    {
        m_fader_up_button   = std::make_shared<Button>(m_top_row[LpMini::FADER_UP], COLOR_FADER_FINE_BTN);
        m_fader_down_button = std::make_shared<Button>(m_top_row[LpMini::FADER_DOWN], COLOR_FADER_FINE_BTN);
        m_top_row_buttons[LpMini::FADER_UP]   = m_fader_up_button;
        m_top_row_buttons[LpMini::FADER_DOWN] = m_fader_down_button;

        m_prev_page_button  = std::make_shared<ButtonWithHandler<LaunchPad> >(m_top_row[LpMini::PREV_PAGE], COLOR_PAGE_BTN, *this, &LaunchPad::handlePrevPageButton);
        m_next_page_button  = std::make_shared<ButtonWithHandler<LaunchPad> >(m_top_row[LpMini::NEXT_PAGE], COLOR_PAGE_BTN, *this, &LaunchPad::handleNextPageButton);
        m_top_row_buttons[LpMini::PREV_PAGE]  = m_prev_page_button;
        m_top_row_buttons[LpMini::NEXT_PAGE]  = m_next_page_button;

        m_static_buttons_by_cc[Controls::FORCE_BLANK_CC] = std::make_shared<MidiToggleButton>(m_right_col[LpMini::FORCE_BLANK], COLOR_FORCE_BLANK_BTN, Controls::FORCE_BLANK_CC, m_to_geoledic, &m_force_blank);
        m_right_col_buttons[LpMini::FORCE_BLANK] = m_static_buttons_by_cc[Controls::FORCE_BLANK_CC];
        m_right_col_buttons[LpMini::FORCE_BLANK]->setState(Button::INACTIVE);
    }
    else
    {
        m_fader_up_button   = std::make_shared<ButtonWithHandler<LaunchPad> >(m_left_col[LpPro::FADER_UP], COLOR_FADER_FINE_BTN, *this, &LaunchPad::handleUpButton);
        m_fader_down_button = std::make_shared<ButtonWithHandler<LaunchPad> >(m_left_col[LpPro::FADER_DOWN], COLOR_FADER_FINE_BTN, *this, &LaunchPad::handleDownButton);
        m_left_col_buttons[LpPro::FADER_UP]   = m_fader_up_button;
        m_left_col_buttons[LpPro::FADER_DOWN] = m_fader_down_button;

        m_prev_page_button  = std::make_shared<ButtonWithHandler<LaunchPad> >(m_top_row[LpPro::PREV_PAGE], COLOR_PAGE_BTN, *this, &LaunchPad::handlePrevPageButton);
        m_next_page_button  = std::make_shared<ButtonWithHandler<LaunchPad> >(m_top_row[LpPro::NEXT_PAGE], COLOR_PAGE_BTN, *this, &LaunchPad::handleNextPageButton);
        m_program_button  = std::make_shared<ButtonWithHandler<LaunchPad> >(m_top_row[LpPro::PROGRAM], COLOR_PROGRAM_BTN, *this, &LaunchPad::handleProgramButton);
        m_program_button->setState(Button::INACTIVE);
        m_top_row_buttons[LpPro::PREV_PAGE] = m_prev_page_button;
        m_top_row_buttons[LpPro::NEXT_PAGE] = m_next_page_button;
        m_top_row_buttons[LpPro::PROGRAM]   = m_program_button;

        m_shift_button = std::make_shared<Button>(m_shift, COLOR_SHIFT_BTN);
        m_shift_button->setState(Button::INACTIVE);

        m_static_buttons_by_cc[Controls::FORCE_BLANK_CC] = std::make_shared<MidiToggleButton>(m_left_col[LpPro::FORCE_BLANK], COLOR_FORCE_BLANK_BTN, Controls::FORCE_BLANK_CC, m_to_geoledic, &m_force_blank);
        m_left_col_buttons[LpPro::FORCE_BLANK] = m_static_buttons_by_cc[Controls::FORCE_BLANK_CC];
        m_left_col_buttons[LpPro::FORCE_BLANK]->setState(Button::INACTIVE);

        m_right_col_buttons[LpPro::SEND_SNAPSHOT]  = std::make_shared<ButtonWithHandler<LaunchPad> >(m_right_col[LpPro::SEND_SNAPSHOT], COLOR_SEND_SNAPSHOT_BTN, *this, &LaunchPad::handleSendSnapshotButton);
        m_right_col_buttons[LpPro::SEND_SNAPSHOT]->setState(Button::ACTIVE);
        m_left_col_buttons[LpPro::PAUSE_MIDI] = std::make_shared<ButtonWithHandler<LaunchPad> >(m_left_col[LpPro::PAUSE_MIDI], COLOR_PAUSE_MIDI_BTN_PLAYING, *this, &LaunchPad::handlePauseMidiButton);
        MidiSource::MidiSender* sender(getMidiSource().getSender());
        m_left_col_buttons[LpPro::PAUSE_MIDI]->setInactiveColor(COLOR_PAUSE_MIDI_BTN_PAUSED);
        if (sender && not sender->enabled()) {
            m_left_col_buttons[LpPro::PAUSE_MIDI]->setState(Button::INACTIVE);
        } else {
            m_left_col_buttons[LpPro::PAUSE_MIDI]->setState(Button::ACTIVE);
        }
    }
    m_fader_up_button->setState(Button::INACTIVE);
    m_fader_down_button->setState(Button::INACTIVE);
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

bool LaunchPad::addPadColor(uint8_t*& p, PadColor& pad, int col, int row)
{
    if (p - m_sysex_message.raw.data + 6 > MAX_SYSEX_DATA_LENGTH)
    {
        // the 5 bytes for the color item plus the end marker won't fit,
        // send the remainder in the next message
        return false;
    }

    if (not pad.m_dirty) return true;

    *p++ = pad.m_type; // use RGB
    *p++ = (row + 1) * 10 + col + 1;
    switch (pad.m_type)
    {
    case PadColor::FLASHING:
        *p++ = pad.m_pal_color[0];
        *p++ = pad.m_pal_color[1];
        break;
    case PadColor::PULSING:
        *p++ = pad.m_pal_color[0];
        break;
    case PadColor::RGB:
    default:
        *p++ = pad.m_color.r/2;
        *p++ = pad.m_color.g/2;
        *p++ = pad.m_color.b/2;
        break;
    }

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
        if (not addPadColor(p, m_right_col[row], NUM_COLS, row)) break;
    }

    if (m_model == PRO_MK3)
    {
        for (unsigned row = 0; row < NUM_ROWS; row++)
        {
            if (not addPadColor(p, m_left_col[row], -1, row)) break;
        }
    }

    if (m_current_page != m_pages.end())
    {
        for (unsigned row = 0; row < NUM_ROWS; row++)
        {
            for (unsigned col = 0; col < NUM_COLS; col++)
            {
                if (not addPadColor(p, m_current_page->m_pads[col][row], col, row)) break;
            }
        }
        if (m_model == PRO_MK3)
        {
            for (unsigned col = 0; col < NUM_COLS; col++)
            {
                if (not addPadColor(p, m_current_page->m_bottom_row[col], col, NUM_ROWS + 1)) break;
            }
        }
    }

    if (m_model == PRO_MK3) {
        addPadColor(p, m_shift, -1, 8);
    }
    addPadColor(p, m_logo, 8, 8);

    if (p == m_sysex_message.raw.data) return; // no messages added

    *p++ = SYSEX_END;
    m_sysex_message.raw.length = p - m_sysex_message.raw.header;
    m_to_launchpad.sink(m_sysex_message.msg);
}

void LaunchPad::handlePrevPageButton(uint8_t value, Button&)
{
    // only care about note on
    if (value == 0) return;
    if (m_current_page_ix == 0) return;

    --m_current_page;
    --m_current_page_ix;
    m_current_page->setDirty();
    m_next_page_button->setState(Button::ACTIVE);
    if (m_current_page_ix == 0)
    {
        m_prev_page_button->setState(Button::INACTIVE);
    }
}

void LaunchPad::handleNextPageButton(uint8_t value, Button&)
{
    // only care about note on
    if (value == 0) return;
    if (m_current_page_ix == m_pages.size() - 1) return;

    ++m_current_page;
    ++m_current_page_ix;
    m_current_page->setDirty();
    m_prev_page_button->setState(Button::ACTIVE);
    if (m_current_page_ix == m_pages.size() - 1)
    {
        m_next_page_button->setState(Button::INACTIVE);
    }
}

void LaunchPad::handleUpButton(uint8_t value, Button& button) {
    button.setState(value ? Button::ACTIVE : Button::INACTIVE);

    if (value == 0) return;
    getProgramFactory().selectPrevPopupItem();
}

void LaunchPad::handleDownButton(uint8_t value, Button& button) {
    button.setState(value ? Button::ACTIVE : Button::INACTIVE);

    if (value == 0) return;
    getProgramFactory().selectNextPopupItem();
}


void LaunchPad::handleSendSnapshotButton(uint8_t value, Button&)
{
    // only care about note on
    if (value == 0) return;
    // the LaunchPad runs in a separate thread, we don't
    //  want the program change to be actioned upon while
    //  we're still using the program, hence we lock it
    getProgramFactory().lock();
    getProgramFactory().program().sendSnapshotWithTrigger(getMidiSource().getSender());
    getProgramFactory().unlock();
}

void LaunchPad::handlePauseMidiButton(uint8_t value, Button& button)
{
    // only care about note on
    if (value == 0) return;

    MidiSource::MidiSender* sender(getMidiSource().getSender());
    if (sender == nullptr) return;

    if (sender->enabled()) {
        sender->enable(false);
        button.setState(Button::INACTIVE);
    } else {
        sender->enable(true);
        button.setState(Button::ACTIVE);
        // the LaunchPad runs in a separate thread, we don't
        //  want the program change to be actioned upon while
        //  we're still using the program, hence we lock it
        getProgramFactory().lock();
        getProgramFactory().program().sendSnapshot(sender);
        getProgramFactory().unlock();
    }
}

void LaunchPad::handleProgramButton(uint8_t value, Button& button)
{
    // only care about note on
    if (value == 0) return;
    if (button.is(Button::ACTIVE) and not m_shift_button->is(Button::ACTIVE)) {
        getProgramFactory().activatePopupSelection(getMidiSource().getSender());
        button.setState(Button::INACTIVE);
    } else {
        button.setState(getProgramFactory().toggleProgramPopup() ?
            Button::ACTIVE : Button::INACTIVE);
    }
}

void LaunchPad::toggleEnumPopup(uint8_t cc_num)
{
    auto enum_button = m_dynamic_buttons_by_cc[cc_num];
    if (enum_button->is(Button::ACTIVE) and not m_shift_button->is(Button::ACTIVE)) {
        getProgramFactory().activatePopupSelection(getMidiSource().getSender());
        enum_button->setState(Button::INACTIVE);
    } else {
        enum_button->setState(getProgramFactory().toggleEnumPopup(cc_num) ?
            Button::ACTIVE : Button::INACTIVE);
    }
}


void LaunchPad::updateFromMidi(const MidiMessage& msg)
{
    if (msg.type() == MidiMessage::CONTROL_CHANGE)
    {
        if (m_static_buttons_by_cc[msg.data[1]])
        {
            m_static_buttons_by_cc[msg.data[1]]->receiveFromGeoledic(msg.data[2]);
            return;
        }
        if (m_dynamic_buttons_by_cc[msg.data[1]])
        {
            m_dynamic_buttons_by_cc[msg.data[1]]->receiveFromGeoledic(msg.data[2]);
            return;
        }
 
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

            if (p.style == ControlChangeParams::NORMAL)
            {
                m_faders_by_cc[p.cc_num] = std::make_shared<Fader>(
                    p.cc_num, 
                    p.min, 
                    p.max, 
                    CHSV(index*24, 255, 255), 
                    this_page->m_pads[index % NUM_COLS],
                    &this_page->m_bottom_row[index % NUM_COLS],
                    m_to_geoledic);
            }
            else
            {
                m_faders_by_cc[p.cc_num] = std::make_shared<HueFader>(
                    p.cc_num, 
                    p.min, 
                    p.max, 
                    CHSV(index*24, 255, 255), 
                    this_page->m_pads[index % NUM_COLS],
                    &this_page->m_bottom_row[index % NUM_COLS],
                    m_to_geoledic);
            }

            m_faders.push_back(m_faders_by_cc[p.cc_num]);
        }
        m_current_page = m_pages.begin();
        m_current_page_ix = 0;

        std::vector<ButtonParams> buttons(getButtonsForProgram(program_num));
        m_dynamic_buttons_by_cc.clear();
        for (unsigned i = 0; i < LpMini::DYNAMIC_BUTTON_FIRST - LpMini::DYNAMIC_BUTTON_LAST + 1; i++) // dynamic button numbering is backwards as we want them from top to bottom
        {
            uint8_t row = LpMini::DYNAMIC_BUTTON_FIRST - i;
            m_right_col_buttons[row].reset();
            if (i < buttons.size())
            {
                uint8_t cc_num = buttons[i].cc_num;
                if (buttons[i].type == ButtonParams::BUTTON) {
                    m_dynamic_buttons_by_cc[cc_num] = std::make_shared<MidiToggleButton>(m_right_col[row], CRGB::Orange, cc_num, m_to_geoledic);
                } else {
                    m_dynamic_buttons_by_cc[cc_num] = std::make_shared<MidiButtonWithFunction>(m_right_col[row], CRGB::Red, cc_num, m_to_geoledic,
                                                                                               [this, cc_num](uint8_t val){ 
                                                                                                   if (val > 0) toggleEnumPopup(cc_num);
                                                                                               });
                    m_dynamic_buttons_by_cc[cc_num]->setState(Button::INACTIVE);
                }
                m_right_col_buttons[row] = m_dynamic_buttons_by_cc[cc_num];
            }
        }

        // if we have more than 1 page, enable paging buttons in top row
        if (m_pages.size() > 1)
        {
            m_prev_page_button->setState(Button::INACTIVE);
            m_next_page_button->setState(Button::ACTIVE);
        }
        else
        {
            m_prev_page_button->setState(Button::DISABLED);
            m_next_page_button->setState(Button::DISABLED);
        }
    }
    else if (msg.type() == MidiMessage::COMMON_AND_RT)
    {
        if (msg.subType() == MidiMessage::START or msg.subType() == MidiMessage::CONTINUE)
        {
            m_logo = PadColor(PadColor::RED, PadColor::BLACK);
        }
        else if (msg.subType() == MidiMessage::STOP)
        {
            m_logo = CRGB::Black;
        }
        m_to_launchpad.sink(msg);
    }
}

void LaunchPad::updateFromCtrl(const MidiMessage& msg)
{
    if (msg.type() == MidiMessage::NOTE_ON || msg.type() == MidiMessage::CONTROL_CHANGE)
    {

        int row = int(msg.data[1] / 10) - 1;
        int col = int(msg.data[1] % 10) - 1;

        if (row == NUM_ROWS)
        {
            if (col == -1 && m_shift_button)
            {
                m_shift_button->receive(msg.data[2]);
            }           
            else if (m_top_row_buttons[col])
            {
                m_top_row_buttons[col]->receive(msg.data[2]);
            }
        }
        else if (col == NUM_COLS)
        {
            if (m_right_col_buttons[row])
            {
                m_right_col_buttons[row]->receive(msg.data[2]);
            }
        }
        else if (col == -1)
        {
            if (m_left_col_buttons[row])
            {
                m_left_col_buttons[row]->receive(msg.data[2]);
            }
        }
        else if (col < NUM_COLS and 
                 (row < NUM_ROWS or row == NUM_ROWS+1))
        {
            // only care about note on
            if (msg.data[2] == 0) return;

            col += NUM_COLS * m_current_page_ix;
            if (unsigned(col) < m_faders.size())
            {
                Fader::FaderMode mode = Fader::NORMAL;
                if (m_fader_up_button->is(Button::ACTIVE))
                {
                    mode = Fader::FINE_UP;
                }
                else if (m_fader_down_button->is(Button::ACTIVE))
                {
                    mode = Fader::FINE_DOWN;
                }

                m_faders[col]->pushPad(row, mode);
            }
        }
    }
}

void LaunchPad::refreshUi()
{
    sendColors();
}