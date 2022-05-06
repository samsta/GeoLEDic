#include "Vertex.hpp"
#include "Vector.hpp"
#include <math.h>

namespace {

int roundedIntegerDiv(int dividend, int divisor)
{
   if (dividend > 0)
   {
      return (dividend + divisor/2)/divisor;
   }
   else
   {
      return (dividend - divisor/2)/divisor;
   }
}

}

Vertex::Vertex(): x(0), y(0), z(0), theta(0)
{}

Vertex::Vertex(float x, float y, float z): x(x), y(y), z(z), theta(0), phi(0)
{
   updateAngles();
}

Vertex::Vertex(const Vector& v): x(v.x), y(v.y), z(v.z), theta(0), phi(0)
{
   updateAngles();
}
  
Vertex Vertex::operator+(const Vector& v) const
{
  return Vertex(x + v.x, y + v.y, z + v.z);
}

Vertex Vertex::operator-(const Vector& v) const
{
  return Vertex(x - v.x, y - v.y, z - v.z);
}

Vector Vertex::operator-(const Vertex& v) const
{
  return Vector(x - v.x, y - v.y, z - v.z);
}

void Vertex::updateAngles()
{
   float r = sqrt(x*x + y*y + z*z);
   float theta_f = asin(y/r);
   unsigned theta_u = round(theta_f/(M_PI_2/NUM_THETA_STEPS));
   theta = theta_u < THETA_MAX ? theta_u : THETA_MAX;
   
   r = sqrt(x*x + z*z);
   float phi_f = acos(z/r);
   if (x > 0)
   {
      phi_f = 2*M_PI - phi_f;
   }
   phi = isnan(phi_f) ? 0 : round(phi_f/(2*M_PI/NUM_PHI_STEPS));
   if (phi > PHI_MAX) phi = PHI_MAX;
}

uint8_t interpolateTheta(const Vertex& from, const Vertex& to, int step, int num_steps)
{
   int d = int(to.theta)-int(from.theta);
   return from.theta + roundedIntegerDiv(step*d, num_steps-1);
}

uint16_t interpolatePhi(const Vertex& from, const Vertex& to, int step, int num_steps)
{
   int d_phi = int(to.phi) - int(from.phi);
   if (d_phi < -Vertex::NUM_PHI_STEPS/2) d_phi += Vertex::NUM_PHI_STEPS;
   else if (d_phi > Vertex::NUM_PHI_STEPS/2) d_phi -=  Vertex::NUM_PHI_STEPS;

   int phi = int(from.phi) + roundedIntegerDiv(step*d_phi, num_steps-1);

   if (phi < 0) phi += Vertex::NUM_PHI_STEPS;
   else if (phi >= Vertex::NUM_PHI_STEPS) phi -= Vertex::NUM_PHI_STEPS;
   return phi;
}

Vertex interpolate(const Vertex& from, const Vertex& to, int step, int num_steps)
{
   Vector d = to - from;
   return from + (d * step) / num_steps;
}

float distance_square(const Vertex& a, const Vertex& b)
{
   Vector d = a - b;
   return d.x*d.x + d.y*d.y + d.z*d.z;
}

float distance(const Vertex& a, const Vertex& b)
{
   return sqrt(distance_square(a, b));
}

#ifdef WITH_GFX
Vertex::operator gfx::LED() const
{
   return {x, y, z};
}
#endif


