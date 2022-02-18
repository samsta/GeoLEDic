#include "common-test-helpers.hpp"

void PrintTo(const CRGB& c, ::std::ostream* os)
{
   if (c == CRGB(UNSET))
   {
      *os << "UNSET";
   }
   else
   {
      *os << "RGB(" << +c.r << "," << +c.g << "," << +c.b << ")";
   }
}

const Vertex ARBITRARY_COORDINATES[3] = {{0, 0, 0}, {0, 1, 0}, {1, 0, 0}};
const Edge ARBITRARY_EDGES[3] = {{0, 2}, {3, 5}, {6, 8}};

const int UNSET(0xBADBAD);
