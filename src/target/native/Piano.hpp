#ifndef Piano_h
#define Piano_h

#include "imgui/imgui.h"
#include "MidiSource.hpp"
#include <vector>
#include <map>
#include <string>

class Program;

struct KeyZone
{
    KeyZone(uint8_t from, uint8_t to, uint32_t color, const char* name);
    
    uint8_t from;
    uint8_t to;
    ImColor col;
    const char* name;
};

std::string numberToNoteName(uint8_t num);

class Piano {
public:
    Piano();
    void draw(Program* program, const std::vector<std::vector<KeyZone> >& key_zones, MidiSource::MidiSender* sender);
    void noteOn(uint8_t note, uint8_t velocity, uint8_t channel);
    void noteOff(uint8_t note, uint8_t channel);

private:
    void sendNoteIfClicked(int key, unsigned channel, Program* program, MidiSource::MidiSender* sender);

    bool m_show;
    int m_last_clicked_key;
    int m_last_clicked_channel;
    std::map<uint8_t, std::map<uint8_t, uint8_t> > m_channel_key_map;
    uint8_t m_velocity;
};

#endif /* Piano_h */
