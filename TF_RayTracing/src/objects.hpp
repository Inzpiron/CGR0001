#ifndef __RT_OBJECTS__
#define __RT_OBJECTS__

#include <vector>

#include "commons.hpp"
#include "material.hpp"

struct Plane { XYZ normal; double offset; };
struct Sphere { XYZ center; double radius; };
struct LightSource { XYZ where, colour; };

std::vector<Plane> Planes;
std::vector<Sphere> Spheres;
std::vector<LightSource> Lights;

#endif
