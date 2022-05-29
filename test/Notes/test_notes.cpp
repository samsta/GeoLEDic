#include <gmock/gmock.h>
#include "ShapesFromNotes.hpp"

namespace {
const unsigned NUM_TRIANGLES = 123;
const unsigned NUM_BLOBS = 52;

enum {
   CHANNEL_SHAPES = 1,
   CHANNEL_SINGLE_NOTES = 0
};

class TestShapesFromNotes: public testing::Test, public ShapesFromNotes
{
public:
   TestShapesFromNotes()
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

TEST_F(TestShapesFromNotes, singleNotes)
{
   const uint8_t NOTE_VELOCITY_1 = 5;
   const uint8_t NOTE_VELOCITY_2 = 6;
   
   noteOn(NOTE_LOWEST,     NOTE_VELOCITY_1, CHANNEL_SINGLE_NOTES);
   noteOn(NOTE_LOWEST+NUM_TRIANGLES-1, NOTE_VELOCITY_2, CHANNEL_SINGLE_NOTES);

   expectNotesSet({{NOTE_VELOCITY_1, {0}},
                   {NOTE_VELOCITY_2, {NUM_TRIANGLES-1}}});

   run();
   noteOff(NOTE_LOWEST, CHANNEL_SINGLE_NOTES);
   expectNotesSet({{NOTE_VELOCITY_2, {NUM_TRIANGLES-1}}});

   noteOff(NOTE_LOWEST+NUM_TRIANGLES-1, CHANNEL_SINGLE_NOTES);
   expectNotesSet({});

}

TEST_F(TestShapesFromNotes, dontTurnOffImmediatelyToEnsureNotesAreSetForAtLeast1Frame)
{
   const uint8_t NOTE_VELOCITY_1 = 5;
   const uint8_t NOTE_VELOCITY_2 = 6;
   
   noteOn(NOTE_LOWEST,     NOTE_VELOCITY_1, CHANNEL_SINGLE_NOTES);
   noteOn(NOTE_LOWEST+NUM_TRIANGLES-1, NOTE_VELOCITY_2, CHANNEL_SINGLE_NOTES);

   expectNotesSet({{NOTE_VELOCITY_1, {0}},
                   {NOTE_VELOCITY_2, {NUM_TRIANGLES-1}}});

   noteOff(NOTE_LOWEST, CHANNEL_SINGLE_NOTES);
   noteOff(NOTE_LOWEST+NUM_TRIANGLES-1, CHANNEL_SINGLE_NOTES);

   expectNotesSet({{NOTE_VELOCITY_1, {0}},
                   {NOTE_VELOCITY_2, {NUM_TRIANGLES-1}}});
   
   run();
   run();

   expectNotesSet({});

}

// Set and clear blob 0 (bottom left)
TEST_F(TestShapesFromNotes, singleBlob)
{
   const uint8_t NOTE_VELOCITY = 1;
   noteOn(NOTE_C3, NOTE_VELOCITY, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY, {0, 1, 2, 26, 27, 28}}});
   
   run();
   noteOff(NOTE_C3, CHANNEL_SHAPES);
   expectNotesSet({});
}

// Set and clear blob 0 (bottom left) and blob 51 (the pentagon at the top)
TEST_F(TestShapesFromNotes, twoBlobs)
{
   const uint8_t NOTE_VELOCITY_1 = 1;
   const uint8_t NOTE_VELOCITY_2 = 100;
   noteOn(NOTE_C3, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_C3 + NUM_BLOBS - 1, NOTE_VELOCITY_2, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 2, 26, 27, 28}},
                   {NOTE_VELOCITY_2, {118, 119, 120, 121, 122}}
                  });
   
   run();
   noteOff(NOTE_C3, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_2, {118, 119, 120, 121, 122}}});

   noteOff(NOTE_C3 + NUM_BLOBS - 1, CHANNEL_SHAPES);
   expectNotesSet({});
}

// Set blob 0, then blob 1, then clear blob 1 and expect all triangles of blob 0 remaining set
TEST_F(TestShapesFromNotes, twoOverlappingBlobsReleasedInRevers)
{
   const uint8_t NOTE_VELOCITY_1 = 1;
   const uint8_t NOTE_VELOCITY_2 = 100;
   noteOn(NOTE_C3, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_C3 + 1, NOTE_VELOCITY_2, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 26, 27}},  // triangles 2 and 28 are covered by blob 1
                   {NOTE_VELOCITY_2, {2, 3, 4, 28, 29, 30}}
                  });
   
   run();
   noteOff(NOTE_C3 + 1, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 2, 26, 27, 28}}});

   noteOff(NOTE_C3, CHANNEL_SHAPES);
   expectNotesSet({});
}

// Set blob 0, then blob 1, then clear blob 0 and expect all triangles of blob 1 remaining set
TEST_F(TestShapesFromNotes, twoOverlappingBlobsReleasedInOrder)
{
   const uint8_t NOTE_VELOCITY_1 = 1;
   const uint8_t NOTE_VELOCITY_2 = 100;
   noteOn(NOTE_C3, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_C3 + 1, NOTE_VELOCITY_2, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_1, {0, 1, 26, 27}},  // triangles 2 and 28 are covered by blob 1
                   {NOTE_VELOCITY_2, {2, 3, 4, 28, 29, 30}}
                  });
   
   run();
   noteOff(NOTE_C3, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_2, {2, 3, 4, 28, 29, 30}}});

   noteOff(NOTE_C3 + 1, CHANNEL_SHAPES);
   expectNotesSet({});
}

// Set and clear pentagon 0
TEST_F(TestShapesFromNotes, singlePentagon)
{
   const uint8_t NOTE_VELOCITY = 1;
   noteOn(NOTE_Cneg1, NOTE_VELOCITY, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY, {118, 119, 120, 121, 122}}});
   
   run();
   noteOff(NOTE_Cneg1, CHANNEL_SHAPES);
   expectNotesSet({});
}

// Set and clear pentagon 0 and pentagon 0's second ring
TEST_F(TestShapesFromNotes, singlePentagonAndRing)
{
   const uint8_t NOTE_VELOCITY_1 = 1;
   const uint8_t NOTE_VELOCITY_2 = 2;
   noteOn(NOTE_Cneg1, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_Cneg1+2, NOTE_VELOCITY_2, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_1, {118, 119, 120, 121, 122}},
                   {NOTE_VELOCITY_2, {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102}}});

   run();
   noteOff(NOTE_Cneg1, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_2, {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102}}});
   
   noteOff(NOTE_Cneg1+2, CHANNEL_SHAPES);
   expectNotesSet({});
}

// Set pentagon 0's second ring, then pentagon 1's second ring, and then clear them in reverse order
TEST_F(TestShapesFromNotes, overlappingPentagonRings)
{
   const uint8_t VELOCITY_1 = 1;
   const uint8_t VELOCITY_2 = 2;
   noteOn(NOTE_Dneg1, VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_D0, VELOCITY_2, CHANNEL_SHAPES);
   
   expectNotesSet({{VELOCITY_1, {78, 79, 80, /*81, 82,*/ 83, 84, /*85, 86,*/ 87, 88, 89, 90, 91, 92, 93,  94,  95, 96, 97, 98, 99, 100, 101, 102}},
                   //                           \- covered by 2nd shape -/
                   {VELOCITY_2, { 0,  1,  7,  8, 25, 26, 34, 35, 53, 54, 62, 61, 81, 82, 85, 86, 107, 106}}});

   run();
   noteOff(NOTE_D0, CHANNEL_SHAPES);
   expectNotesSet({{VELOCITY_1, {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102}}});

   noteOff(NOTE_Dneg1, CHANNEL_SHAPES);
   expectNotesSet({});
}

// overlap single note with pentagon and blob
TEST_F(TestShapesFromNotes, overlappingSingleNotePentagonAndBlob)
{
   const uint8_t NOTE_VELOCITY_1 = 1;
   const uint8_t NOTE_VELOCITY_2 = 127;
   const uint8_t NOTE_VELOCITY_3 = 2;
   
   noteOn(NOTE_LOWEST+83, NOTE_VELOCITY_1, CHANNEL_SINGLE_NOTES);
   noteOn(NOTE_Cneg1+2,      NOTE_VELOCITY_2, CHANNEL_SHAPES);
   noteOn(NOTE_C3+26,     NOTE_VELOCITY_3, CHANNEL_SHAPES);
   expectNotesSet({/*{NOTE_VELOCITY_1, {83}},*/  // single triangle covered by the other two
                   {NOTE_VELOCITY_2, {78, 79, 80, /*81, 82, 83,*/ 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102}},
                   //                                ^--v  ring partially covered by blob
                   {NOTE_VELOCITY_3, {54, 55, 56,   81, 82, 83}}});

   run();
   noteOff(NOTE_C3+26, CHANNEL_SHAPES);
   expectNotesSet({/*{NOTE_VELOCITY_1, {83}},*/  // single triangle covered by RING
                   {NOTE_VELOCITY_2, {78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102}}});
   
   noteOn(NOTE_C3+26, NOTE_VELOCITY_3, CHANNEL_SHAPES);
   run();
   noteOff(NOTE_Cneg1+2, CHANNEL_SHAPES);

   expectNotesSet({/*{NOTE_VELOCITY_1, {83}},*/  // single triangle covered by blob
                   {NOTE_VELOCITY_3, {54, 55, 56,   81, 82, 83}}});

   noteOff(NOTE_C3+26, CHANNEL_SHAPES);
   expectNotesSet({{NOTE_VELOCITY_1, {83}}});

   noteOff(NOTE_LOWEST+83, CHANNEL_SINGLE_NOTES);
   expectNotesSet({});
}

// set all notes on all channels, then clear them all and ensure we're with nothing left
TEST_F(TestShapesFromNotes, setAllClearAll)
{
   for (unsigned channel: {CHANNEL_SHAPES, CHANNEL_SINGLE_NOTES})
   {
      for (unsigned note = NOTE_LOWEST; note <= NOTE_HIGHEST; note++)
      {
         noteOn(note, channel+5, channel);
      }
   }
   run();
   for (unsigned channel: {CHANNEL_SHAPES, CHANNEL_SINGLE_NOTES})
   {
      for (unsigned note = NOTE_LOWEST; note <= NOTE_HIGHEST; note++)
      {
         noteOff(note, channel);
      }
   }
   expectNotesSet({});
}

// Native Instrument's Komplete Kontrol seems buggy in that it sends duplicate note on messages
// occasionally. Let's not end up with stuck notes because of that!
TEST_F(TestShapesFromNotes, duplicateNoteOn)
{
   const uint8_t NOTE_VELOCITY = 1;
   noteOn(NOTE_C2, NOTE_VELOCITY, CHANNEL_SHAPES);
   noteOn(NOTE_C2, NOTE_VELOCITY, CHANNEL_SHAPES);
   
   run();
   noteOff(NOTE_C2, CHANNEL_SHAPES);
   expectNotesSet({});
}

// fuzz inputs (occasionally invalid on purpose) and see if it crashes
TEST_F(TestShapesFromNotes, fuzz)
{
   srand(0);
   unsigned iterations = 100000;
   while (iterations--)
   {
      noteOn(rand() % (NOTE_HIGHEST + 10), rand(), rand() % 2);
      run();
      noteOff(rand() % (NOTE_HIGHEST + 10), rand() % 2);
   }
}

TEST_F(TestShapesFromNotes, anyTrianglesSet)
{
   const uint8_t NOTE_VELOCITY_1 = 100;
   const uint8_t NOTE_VELOCITY_2 = 50;

   EXPECT_FALSE(isAnyTriangleSet());

   noteOn(NOTE_C3, NOTE_VELOCITY_1, CHANNEL_SHAPES);
   noteOn(NOTE_C3 + 1, NOTE_VELOCITY_2, CHANNEL_SHAPES);

   EXPECT_TRUE(isAnyTriangleSet());

   run();
   noteOff(NOTE_C3 + 1, CHANNEL_SHAPES);

   EXPECT_TRUE(isAnyTriangleSet());

   noteOff(NOTE_C3, CHANNEL_SHAPES);

   EXPECT_FALSE(isAnyTriangleSet());
}

}
