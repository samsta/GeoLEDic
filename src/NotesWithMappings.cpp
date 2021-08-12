#include "NotesWithMappings.hpp"
#include <vector>
#include <algorithm>

namespace {

typedef std::vector<std::vector<uint8_t> >  PentagonNoteMap;
PentagonNoteMap pentagon0 =
{
   {118, 119, 120, 121, 122},
   {103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117},
   {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102},
   {52, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77},
   {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51},
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24}
};

PentagonNoteMap pentagon1 =
{
   {29, 30, 31, 57, 58},
   {2, 3, 4, 5, 6, 32, 33, 60, 59, 84, 83, 56, 55, 27, 28},
   {0, 1, 7, 8, 34, 35, 62, 61, 86, 85, 107, 106, 82, 81, 54, 53, 25, 26},
   {9, 10, 36, 37, 64, 63, 88, 87, 109, 108, 120, 119, 105, 104, 80, 79, 52},
   {11, 12, 38, 39, 65, 66, 89, 90, 110, 111, 121, 122, 118, 117, 103, 102, 78},
   {13, 14, 40, 41, 67, 68, 91, 92, 112, 113, 114, 115, 116, 100, 101},
   {15, 16, 42, 43, 69, 70, 93, 94, 95, 96, 97, 98, 99},
   {17, 18, 44, 45, 71, 72, 73, 74, 75, 76, 77},
   {19, 20, 46, 47, 48, 49, 50, 51},
   {21, 22, 23, 24}
};

PentagonNoteMap pentagon2 =
{
   {37, 38, 39, 65, 64},
   {10, 11, 12, 13, 14, 40, 41, 67, 66, 89, 88, 63, 62, 35, 36},
   {15, 16, 42, 43, 69, 68, 91, 90, 110, 109, 87, 86, 61, 60, 33, 34, 8, 9},
   {17, 18, 44, 45, 71, 70, 93, 92, 112, 111, 121, 120, 108, 107, 85, 84, 59, 58, 31, 32, 6, 7},
   {19, 20, 46, 47, 72, 73, 94, 95, 113, 114, 122, 118, 119, 105, 106, 82, 83, 56, 57, 29, 30, 4, 5},
   {21, 22, 48, 49, 74, 75, 96, 97, 115, 116, 117, 103, 104, 80, 81, 54, 55, 27, 28, 2, 3},
   {23, 24, 50, 51, 76, 77, 98, 99, 100, 101, 102, 78, 79, 52, 53, 25, 26, 0, 1},
};

PentagonNoteMap pentagon3 =
{
   {45, 46, 47, 72, 71},
   {18, 19, 20, 21, 22, 48, 49, 74, 73, 94, 93, 70, 69, 43, 44},
   {23, 24, 50, 51, 76, 75, 96, 95, 113, 112, 92, 91, 68, 67, 41, 42, 16, 17},
   {77, 98, 97, 115, 114, 122, 121, 111, 110, 90, 89, 66, 65, 39, 40, 14, 15},
   {99, 100, 116, 117, 118, 119, 120, 108, 109, 87, 88, 63, 64, 37, 38, 12, 13},
   {101, 102, 103, 104, 105, 106, 107, 85, 86, 61, 62, 35, 36, 10, 11},
   {78, 79, 80, 81, 82, 83, 84, 59, 60, 33, 34, 8, 9},
   {52, 53, 54, 55, 56, 57, 58, 31, 32, 6, 7},
   {25, 26, 27, 28, 29, 30, 4, 5},
   {0, 1, 2, 3}
};

typedef std::vector<std::vector<uint8_t> >  BlobNoteMap;
BlobNoteMap blobs =
{
   // first row
   {0, 1, 2, 26, 27, 28},
   {2, 3, 4, 28, 29, 30},
   {4, 5, 6, 30, 31, 32},
   {6, 7, 8, 32, 33, 34},
   {8, 9, 10, 34, 35, 36},
   {10, 11, 12, 36, 37, 38},
   {12, 13, 14,38, 39, 40},
   {14, 15, 16, 40, 41, 42},
   {16, 17, 18, 42, 43, 44},
   {18, 19, 20, 44, 45, 46},
   {20, 21, 22, 46, 47, 48},
   {22, 23, 24, 48, 49, 50},
   // second row
   {25, 26, 27, 53, 54, 55},
   {27, 28, 29, 55, 56, 57},
   {29, 30, 31, 57, 58},
   {31, 32, 33, 58, 59, 60},
   {33, 34, 35, 60, 61, 62},
   {35, 36, 37, 62, 63, 64},
   {37, 38, 39, 64, 65},
   {39, 40, 41, 65, 66, 67},
   {41, 42, 43, 67, 68, 69},
   {43, 44, 45, 69, 70, 71},
   {45, 46, 47, 71, 72},
   {47, 48, 49, 72, 73, 74},
   {49, 50, 51, 74, 75, 76},
   // third row
   {52, 53, 54, 79, 80, 81},
   {54, 55, 56, 81, 82, 83},
   {56, 57, 58, 59, 83, 84},
   {59, 60, 61, 84, 85, 86},
   {61, 62, 63, 86, 87, 88},
   {63, 64, 65, 66, 88, 89},
   {66, 67, 68, 89, 90, 91},
   {68, 69, 70, 91, 92, 93},
   {70, 71, 72, 73, 93, 94},
   {73, 74, 75, 94, 95, 96},
   {75, 76, 77, 96, 97, 98},
   // fourth row
   {78, 79, 80, 102, 103, 104},
   {80, 81, 82, 104, 105, 106},
   {82, 83, 84, 85, 106, 107},
   {85, 86, 87, 107, 108, 109},
   {87, 88, 89, 90, 109, 110},
   {90, 91, 92, 110, 111, 112},
   {92, 93, 94, 95, 112, 113},
   {95, 96, 97, 113, 114, 115},
   {97, 98, 99, 100, 115, 116},
   {100, 101, 102, 103, 116, 117},
   // fifth row
   {117, 118, 119, 103, 104, 105},
   {105, 106, 107, 108, 119, 120},
   {108, 109, 110, 111, 120, 121},
   {111, 112, 113, 114, 121, 122},
   {114, 115, 116, 117, 118, 122},
   // top pentagon
   {118, 119, 120, 121, 122}
};

}

NotesWithMappings::NotesWithMappings()
{
   std::fill(m_notes_old, m_notes_old+NOTE_HIGHEST+1, 0);
}

uint8_t NotesWithMappings::note(uint8_t note) const
{
   if (note > NOTE_HIGHEST) return 0;
   return m_notes_old[note];
}

void NotesWithMappings::noteOn(uint8_t note, uint8_t velocity, uint8_t channel)
{
   switch (channel)
   {
      case 0:
      default:
         setNote(note, velocity, CHANNEL_SINGLE_TRIANGLE, note);
         break;
      case 1:
         setPentagon(note, velocity);
         break;
      case 2:
         setBlob(note, velocity);
         break;
   }
}

void NotesWithMappings::noteOff(uint8_t note, uint8_t channel)
{
   noteOn(note, 0, channel);
}

void NotesWithMappings::setNote(uint8_t note, uint8_t velocity, Channel channel, uint8_t index_in_channel)
{
   if (note > NOTE_HIGHEST) return;
   
   if (velocity == 0)
   {
      clearNote(note, channel, index_in_channel);
      return;
   }
   
   m_notes[note].set({channel, index_in_channel, velocity});
   m_notes_old[note] = m_notes[note].velocity();
}

void NotesWithMappings::clearNote(uint8_t note, Channel channel, uint8_t index_in_channel)
{
   if (m_notes.count(note) == 0) return;
   
   m_notes[note].clear(channel, index_in_channel);
   m_notes_old[note] = m_notes[note].velocity();
   
   // if the velocity is 0, then there's no setter remaining, so we can remove the item
   if (m_notes[note].velocity() == 0)
   {
      m_notes.erase(note);
   }
}

void NotesWithMappings::setBlob(uint8_t note, uint8_t velocity)
{
   if (note < NOTE_C2) return;
   note -= NOTE_C2;
   
   if (note >= blobs.size()) return;
      
   for (uint8_t mapped_note: blobs[note])
   {
      setNote(mapped_note, velocity, CHANNEL_BLOBS, note);
   }
}

void NotesWithMappings::setPentagon(uint8_t note, uint8_t velocity)
{
   PentagonNoteMap* pentagon = nullptr;
   if (note >= NOTE_C2 && note < NOTE_C2 + pentagon0.size())
   {
      note -= NOTE_C2;
      pentagon = &pentagon0;
   }
   else if (note >= NOTE_C3 && note < NOTE_C3 + pentagon1.size())
   {
      note -= NOTE_C3;
      pentagon = &pentagon1;
   }
   else if (note >= NOTE_C4 && note < NOTE_C4 + pentagon2.size())
   {
      note -= NOTE_C4;
      pentagon = &pentagon2;
   }
   else if (note >= NOTE_C5 && note < NOTE_C5 + pentagon3.size())
   {
      note -= NOTE_C5;
      pentagon = &pentagon3;
   }
   else
   {
      return;
   }
   
   for (uint8_t mapped_note: pentagon->at(note))
   {
      setNote(mapped_note, velocity, CHANNEL_BLOBS, note);
   }
}

NotesWithMappings::Setter::Setter():
   channel(), index(), velocity()
{
}

NotesWithMappings::Setter::Setter(Channel channel, uint8_t index, uint8_t velocity):
   channel(channel), index(index), velocity(velocity)
{
}

NotesWithMappings::Setters::Setters(): num_setters(0)
{
}

void NotesWithMappings::Setters::set(const Setter& setter)
{
   if (num_setters == MAX_SETTERS) return; // can't set another one, 'polyphony' exhausted
   setters[num_setters] = setter;
   num_setters++;
}

void NotesWithMappings::Setters::clear(Channel channel, uint8_t index_in_channel)
{
   bool found = false;
   for (unsigned k = 0; k < num_setters; k++)
   {
      if (setters[k].channel == channel and setters[k].index == index_in_channel)
      {
         found = true;
         num_setters--;
         /*
          If it's the last one, we're done and there's no need to compact the list
          */
         if (k == num_setters) break;
      }
      
      if (found)
      {
         // we have removed an item, so shift the remainder of the list
         setters[k] = setters[k+1];
      }
   }
}

uint8_t NotesWithMappings::Setters::velocity() const
{
   if (num_setters == 0 or num_setters > MAX_SETTERS) return 0; // not set, or invalid
   
   // return the velocity of the latest setter
   return setters[num_setters-1].velocity;
}
