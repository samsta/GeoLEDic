#ifndef Strobe_hpp
#define Strobe_hpp

#include "DomeWrapper.hpp"
#include "generated/Strobe.hpp"

class Strobe: public generated::Strobe
{
public:
   Strobe(const DomeWrapper& dome);
   
   virtual void runProgram();
   
private:
   const DomeWrapper& m_dome;
};

#endif /* Strobe_hpp */
