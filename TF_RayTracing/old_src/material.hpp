#ifndef __RT_MATERIAL__
#define __RT_MATERIAL__

#include "commons.hpp"

struct Material
{
	XYZ color;
	double shininess;

	Material(const XYZ &c, double s)
	{
		Material(c[0], c[1], c[2], s);
	}

	Material(double r, double g, double b, double s)
	{
		color.d[0] = r; color.d[1] = g; color.d[2] = b;
		shininess = s;
	}
};

#endif
