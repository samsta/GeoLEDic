#ifndef LAUNCHPAD_HPP
#define LAUNCHPAD_HPP

#include "MidiMessage.hpp"
#define FASTLED_INTERNAL // get rid of annoying version pragma
#include "platform.h"
#include "FastLED.h"
#include <memory>
#include <map>
#include <vector>
#include <list>

class Fader;
class Button;
class MidiButton;

struct PadColor
{
    enum Type {
        FLASHING = 1,
        PULSING = 2,
        RGB = 3
    };

    enum PaletteColor {
        // indexed colors, see e.g. https://fael-downloads-prod.focusrite.com/customer/prod/s3fs-public/downloads/LPP3_prog_ref_guide_200415.pdf, page 9
        BLACK = 0,
        RED = 5,
        ORANGE = 9,
        YELLOW = 13,
        GREEN = 21,
        BLUE = 45,
        PINK = 57
    };

    PadColor();
    PadColor(const CRGB& color);
    PadColor(PaletteColor pulsing_color);
    PadColor(PaletteColor color1, PaletteColor color2);

    void operator=(const CRGB& color);
    bool m_dirty;
    CRGB m_color;
    PaletteColor m_pal_color[2];
    Type m_type;
};

template <unsigned cols, unsigned rows>
struct Page {
    PadColor m_pads[cols][rows];
    PadColor m_bottom_row[cols];
    void setDirty();
};

class LaunchPad
{
public:
    enum Model {
        MINI_MK3,
        PRO_MK3
    };

    LaunchPad(MidiMessageSink& to_launchpad, MidiMessageSink& to_geoledic, Model model);
    ~LaunchPad();

    void updateFromMidi(const MidiMessage& msg);
    void updateFromCtrl(const MidiMessage& msg);
    void refreshUi();

    enum
    {
        NUM_ROWS = 8,
        NUM_COLS = 8
    };

private:
    enum Mode {
        LIVE,
        PROGRAMMER
    };
    void enterMode(Mode mode);
    void sendText(const char* text, const CRGB& color = CRGB::Red);
    void sendColors();
    bool addPadColor(uint8_t*& p, PadColor& pad, int col, int row);

    void handleNextPageButton(uint8_t value);
    void handlePrevPageButton(uint8_t value);
    void handleUpButton(uint8_t value);
    void handleDownButton(uint8_t value);

    void handleSendSnapshotButton(uint8_t value);
    void handleProgramButton(uint8_t value);

    void toggleEnumPopup(uint8_t cc_num);

    MidiMessageSink& m_to_launchpad;
    MidiMessageSink& m_to_geoledic;
    Model  m_model;
    union SysexMsg;
    SysexMsg& m_sysex_message;



    PadColor m_top_row[NUM_COLS];
    PadColor m_right_col[NUM_ROWS];
    PadColor m_left_col[NUM_ROWS];
    PadColor m_logo;
    PadColor m_shift;

    std::list<Page<NUM_ROWS, NUM_COLS> > m_pages;
    std::list<Page<NUM_ROWS, NUM_COLS> >::iterator m_current_page;
    unsigned m_current_page_ix;

    bool m_force_blank;

    std::vector<std::shared_ptr<Fader> > m_faders;
    std::map<uint8_t, std::shared_ptr<Fader> > m_faders_by_cc;

    // static buttons are always there
    std::map<uint8_t, std::shared_ptr<MidiButton> > m_static_buttons_by_cc;
    // dynamic buttons change when program changes
    std::map<uint8_t, std::shared_ptr<MidiButton> > m_dynamic_buttons_by_cc;

    std::map<unsigned, std::shared_ptr<Button> > m_top_row_buttons;
    std::map<unsigned, std::shared_ptr<Button> > m_bottom_row_buttons;
    std::map<unsigned, std::shared_ptr<Button> > m_right_col_buttons;
    std::map<unsigned, std::shared_ptr<Button> > m_left_col_buttons;

    std::shared_ptr<Button> m_next_page_button;
    std::shared_ptr<Button> m_prev_page_button;
    std::shared_ptr<Button> m_fader_up_button;
    std::shared_ptr<Button> m_fader_down_button;
    std::shared_ptr<Button> m_program_button;
    std::shared_ptr<Button> m_shift_button;

};

#endif // LAUNCHPAD_HPP