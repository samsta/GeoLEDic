#ifndef GEOLEDIC_VERTEX_HPP
#define GEOLEDIC_VERTEX_HPP

#ifdef WITH_GFX
#define FASTLED_INTERNAL // get rid of annoying version pragma
#include "gfx/Gfx.hpp"
#endif
#include <stdint.h>

struct Vector;

struct Vertex {
   
  enum {
      THETA_MAX = 255,
      NUM_THETA_STEPS = 256,
      PHI_MAX = 1023,
      NUM_PHI_STEPS = 1024
  };
   
  Vertex();
  Vertex(float x, float y, float z);
  explicit Vertex(const Vector& v);
  
  float x, y, z;

  // 0..255: 0deg..90deg
  uint8_t theta;
  
  // 0..1024: 0deg..360deg
  uint16_t phi;

  Vertex operator+(const Vector& v) const;
  Vertex operator-(const Vector& v) const;
  Vector operator-(const Vertex& v) const;
   
  void updateAngles();
   
#ifdef WITH_GFX
  operator gfx::LED() const;
#endif
};

uint8_t interpolateTheta(const Vertex& from, const Vertex& to, int step, int num_steps);
uint16_t interpolatePhi(const Vertex& from, const Vertex& to, int step, int num_steps);
Vertex interpolate(const Vertex& from, const Vertex& to, int step, int num_steps);

float distance_square(const Vertex& a, const Vertex& b);
float distance(const Vertex& a, const Vertex& b);

#endif
