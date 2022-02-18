#ifndef common_test_helpers_hpp
#define common_test_helpers_hpp

#include "Triangle.hpp"
#include <iostream>

void PrintTo(const CRGB& c, ::std::ostream* os);
extern const Vertex ARBITRARY_COORDINATES[3];
extern const Edge ARBITRARY_EDGES[3];
extern const int UNSET;


#endif /* common_test_helpers_hpp */
