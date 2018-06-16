#ifndef __RT_OBJECTS__
#define __RT_OBJECTS__

#include <vector>

#include "commons.hpp"
#include "material.hpp"

enum ObjType {PLANE, SPHERE};

struct Plane { XYZ normal; Material *mtl; double offset; };
struct Sphere { XYZ center; Material *mtl; double radius; };
struct LightSource { XYZ center, colour; };

class Object
{
public:
	ObjType type;
	XYZ pos;
	XYZ color;
	XYZ aux;			// aux[0] é: offset para PLANE, radius para SPHERE
	double shinyness;
};

std::vector<Plane> Planes;
std::vector<Sphere> Spheres;
std::vector<LightSource> Lights;
std::vector<Material> Materials;

#endif
