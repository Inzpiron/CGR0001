#ifndef __RT_OBJECTS__
#define __RT_OBJECTS__

#include <vector>

// Declarations for scene description
// Walls are planes. Planes have a
// normal vector and a distance.

const struct Plane
{
	XYZ normal;
	double offset;
}

const struct Sphere
{
	XYZ center;
	double radius;
}

const struct LightSource
{
	XYZ where, colour;
}

std::vector<Plane> Planes;
std::vector<Sphere> Spheres;
std::vector<LightSource> Lights;

#endif
