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
    void setDirty();
};

class LaunchPad
{
public:
    LaunchPad(MidiMessageSink& to_launchpad, MidiMessageSink& to_geoledic);
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
    bool addPadColor(uint8_t*& p, PadColor& pad, unsigned col, unsigned row);

    MidiMessageSink& m_to_launchpad;
    MidiMessageSink& m_to_geoledic;
    union SysexMsg;
    SysexMsg& m_sysex_message;



    PadColor m_top_row[NUM_COLS];
    PadColor m_side_col[NUM_ROWS];

    std::list<Page<NUM_ROWS, NUM_COLS> > m_pages;
    std::list<Page<NUM_ROWS, NUM_COLS> >::iterator m_current_page;
    unsigned m_current_page_ix;

    bool m_up_pushed;
    bool m_down_pushed;

    std::vector<std::shared_ptr<Fader> > m_faders;
    std::map<uint8_t, std::shared_ptr<Fader> > m_faders_by_cc;
};

#endif // LAUNCHPAD_HPP