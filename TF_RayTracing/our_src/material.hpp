#ifndef __RT_MATERIAL__
#define __RT_MATERIAL__

#include "commons.hpp"
#include "geometry.hpp"

struct Material
{
	XYZ color;
	double roughness,
	       shininess;
};

#endif
