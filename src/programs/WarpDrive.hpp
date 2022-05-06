#ifndef WarpDrive_hpp
#define WarpDrive_hpp

#include "DomeWrapper.hpp"
#include "generated/WarpDrive.hpp"

class WarpDrive: public generated::WarpDrive
{
public:
   WarpDrive(const DomeWrapper& dome);
   
   virtual void runProgram();
   
private:
   void shiftAndFillRings();
   bool findKey(uint8_t& hue, uint8_t& saturation);

   const DomeWrapper& m_dome;
   static const unsigned NUM_DISTANCE_STEPS = 256;
   CRGB m_rings[NUM_DISTANCE_STEPS];
};

#endif /* WarpDrive_hpp */
