#ifndef __RT_SCENE__
#define __RT_SCENE__

/*
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

std::vector<LightSource> Lights;
Lights[] =
{
	{ {{-28,-14, 3}}, {{.4, .51, .9}} },
	{ {{-29,-29,-29}}, {{.95, .1, .1}} },
	{ {{ 14, 29,-14}}, {{.8, .8, .8}} },
	{ {{ 29, 29, 29}}, {{1,1,1}} },
	{ {{ 28,  0, 29}}, {{.5, .6,  .1}} }
};
*/

void initScene()
{
	Planes.push_back({{{ 0, 0,-1}}, -90});
	Planes.push_back({{{ 0, 1, 0}}, -30});
	Planes.push_back({{{ 0,-1, 0}}, -30});
	Planes.push_back({{{ 1, 0, 0}}, -30});
	Planes.push_back({{{ 0, 0, 1}}, -90});
	Planes.push_back({{{-1, 0, 0}}, -30});

	/*
	Sphere.push_back({{{ 0, 0,-1}}, -90});
	Sphere.push_back({{{ 0, 1, 0}}, -30});
	Sphere.push_back({{{ 0,-1, 0}}, -30});
	Sphere.push_back({{{ 1, 0, 0}}, -30});
	Sphere.push_back({{{ 0, 0, 1}}, -90});
	Sphere.push_back({{{-1, 0, 0}}, -30});
	*/
}

#endif
