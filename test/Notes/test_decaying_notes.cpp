#include <gmock/gmock.h>
#include "DecayingShapesFromNotes.hpp"

namespace {
const unsigned NUM_TRIANGLES = 123;

enum {
   CHANNEL_SHAPES = 1,
   CHANNEL_SINGLE_NOTES = 0,
};

class TestDecayingShapesFromNotes: public testing::Test, public DecayingShapesFromNotes
{
public:
   TestDecayingShapesFromNotes()
   {}
   
   struct ExpectedNotes{
      ExpectedNotes(uint8_t velocity, const std::vector<uint8_t>& notes):
         velocity(velocity),
         notes(notes)
      {}
      
      uint8_t velocity;
      std::vector<uint8_t> notes;
   };
   
   void expectNotesSet(const std::vector<ExpectedNotes>& expected_notes)
   {
      for (unsigned k = 0; k < NUM_TRIANGLES; k++)
      {
         bool found = false;
         for (const ExpectedNotes& en: expected_notes)
         {
            for (uint8_t expected_note: en.notes)
            {
               if (expected_note == k)
               {
                  EXPECT_EQ(en.velocity, getTriangleValue(k)) << "expected note " << k << " to be set to " << +en.velocity;
                  found = true;
                  break;
               }
            }
            if (found) break;
         }
         if (found) continue;
         EXPECT_EQ(0, getTriangleValue(k)) << "didn't expect note " << k << " to be set";
      }
   }
   
   virtual void controlChange(uint8_t, uint8_t){};
};



// Set blob 0, then blob 1, then clear blob 0 and expect all triangles of blob 1 remaining set
TEST_F(TestDecayingShapesFromNotes, twoOverlappingBlobsReleasedInOrder)
{
   const uint8_t NOTE_VELOCITY_1 = 100;
   const uint8_t NOTE_VELOCITY_2 = 50;
   const uint8_t DECAY_RATE = 40;

   setDecayRate(DECAY_RATE);

   noteOn(NOTE_C3, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_C3 + 1, NOTE_VELOCITY_2, CHANNEL_SHAPES);

   run();
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 26, 27}},  // triangles 2 and 28 are covered by blob 1
                   {NOTE_VELOCITY_2, {2, 3, 4, 28, 29, 30}}
                  });

   // update doesn't change things
   run();
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 26, 27}},  // triangles 2 and 28 are covered by blob 1
                   {NOTE_VELOCITY_2, {2, 3, 4, 28, 29, 30}}
                  });

   
   noteOff(NOTE_C3 + 1, CHANNEL_SHAPES);
   run();
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 2, 26, 27, 28}},  // triangles 2 and 28 are no longer covered by blob 1
                   {NOTE_VELOCITY_2 - DECAY_RATE, {3, 4, 29, 30}} // remainder of blob 1 is fading out
                  });

   noteOff(NOTE_C3, CHANNEL_SHAPES);
   run();
   expectNotesSet({{NOTE_VELOCITY_1 - DECAY_RATE, {0, 1, 2, 26, 27, 28}} 
                  });
}

TEST_F(TestDecayingShapesFromNotes, anyTrianglesSet)
{
   const uint8_t NOTE_VELOCITY_1 = 100;
   const uint8_t NOTE_VELOCITY_2 = 50;
   const uint8_t DECAY_RATE = 40;
   setDecayRate(DECAY_RATE);

   EXPECT_FALSE(isAnyTriangleSet());

   noteOn(NOTE_C3, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_C3 + 1, NOTE_VELOCITY_2, CHANNEL_SHAPES);

   run();
   EXPECT_TRUE(isAnyTriangleSet());

   noteOff(NOTE_C3 + 1, CHANNEL_SHAPES);
   noteOff(NOTE_C3, CHANNEL_SHAPES);

   run();
   run();

   EXPECT_TRUE(isAnyTriangleSet());

   run();

   EXPECT_FALSE(isAnyTriangleSet());
}


}
