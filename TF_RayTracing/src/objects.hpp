#ifndef __RT_OBJECTS__
#define __RT_OBJECTS__

// Declarations for scene description
// Walls are planes. Planes have a
// normal vector and a distance.

const struct Plane
{ XYZ normal; double offset; }
std::vector<Plane> Planes;
Planes[] =
{
// Declare six planes, each looks 
// towards origo and is 30 units away.
	{ {{0,0,-1}}, -90 },
	{ {{0, 1,0}}, -30 },
	{ {{0,-1,0}}, -30 },
	{ {{ 1,0,0}}, -30 },
	{ {{0, 0,1}}, -90 },
	{ {{-1,0,0}}, -30 }
};

const struct Sphere
{ XYZ center; double radius; }
std::vector<Sphere> Spheres;
Spheres[] =
{
// Declare a few spheres.
	{ {{0,0,0}}, 7 },
	{ {{19.4, -19.4, 0}}, 2.1 },
	{ {{-19.4, 19.4, 0}}, 2.1 },
	{ {{13.1, 5.1, 0}}, 1.1 },
	{ {{ -5.1, -13.1, 0}}, 1.1 },
	{ {{-30,30,15}}, 11},
	{ {{15,-30,30}}, 6},
	{ {{30,15,-30}}, 6}
};
// Declare lightsources. A lightsource
// has a location and a RGB color.
const struct LightSource
{ XYZ where, colour; }
std::vector<LightSource> Lights;
Lights[] =
{
	{ {{-28,-14, 3}}, {{.4, .51, .9}} },
	{ {{-29,-29,-29}}, {{.95, .1, .1}} },
	{ {{ 14, 29,-14}}, {{.8, .8, .8}} },
	{ {{ 29, 29, 29}}, {{1,1,1}} },
	{ {{ 28,  0, 29}}, {{.5, .6,  .1}} }
};

#endif
