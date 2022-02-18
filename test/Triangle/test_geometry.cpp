#include <gmock/gmock.h>
#include "Triangle.hpp"
#include "common-test-helpers.hpp"

TEST(Triangle, inset)
{
   Vertex triangle[3] = {
      {0.0,    1.0,    0.0},
      {0.0,    1.0,    1.0},
      {1.0,    1.0,    0.0}
   };
   
   Vertex inset_triangle[3] = {
      {0.2,    1.0,    0.2   },
      {0.2,    1.0,    0.5172}, // 1 - sqrt(2 * 0.2*0.2) - 0.2
      {0.5172, 1.0,    0.2   }
   };
   
   inset(triangle, 0.2);
   
   for (unsigned k = 0; k < 3; k ++)
   {
      EXPECT_NEAR(inset_triangle[k].x, triangle[k].x, 0.0001);
      EXPECT_NEAR(inset_triangle[k].y, triangle[k].y, 0.0001);
      EXPECT_NEAR(inset_triangle[k].z, triangle[k].z, 0.0001);
   }
}

TEST(Triangle, centroid)
{
   CRGB strip[9];
   Triangle t(strip, ARBITRARY_EDGES, {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}});

   EXPECT_NEAR(1./3., t.centroid().x, 0.0001);
   EXPECT_NEAR(1./3., t.centroid().y, 0.0001);
   EXPECT_NEAR(1./3., t.centroid().z, 0.0001);
}

TEST(Triangle, centroid2)
{
   CRGB strip[9];
   Triangle t(strip, ARBITRARY_EDGES, {{0, 0, 1}, {0, -1, -1}, {3, 0, 0}});

   EXPECT_NEAR(1., t.centroid().x, 0.0001);
   EXPECT_NEAR(-1./3., t.centroid().y, 0.0001);
   EXPECT_NEAR(0., t.centroid().z, 0.0001);
}