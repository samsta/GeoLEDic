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
public:
    PadColor();
    void operator=(const CRGB& color);
    bool m_dirty;
    CRGB m_color;    
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
    void handleSendSnapshotButton(uint8_t value);

    MidiMessageSink& m_to_launchpad;
    MidiMessageSink& m_to_geoledic;
    Model  m_model;
    union SysexMsg;
    SysexMsg& m_sysex_message;



    PadColor m_top_row[NUM_COLS];
    PadColor m_right_col[NUM_ROWS];
    PadColor m_left_col[NUM_ROWS];

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
};

#endif // LAUNCHPAD_HPP