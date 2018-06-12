#include "rt_headers.hpp"

// Declarations for scene description
// Walls are planes. Planes have a
// normal vector and a distance.
const struct Plane
{ XYZ normal; double offset; }
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
Lights[] =
{
	{ {{-28,-14, 3}}, {{.4, .51, .9}} },
	{ {{-29,-29,-29}}, {{.95, .1, .1}} },
	{ {{ 14, 29,-14}}, {{.8, .8, .8}} },
	{ {{ 29, 29, 29}}, {{1,1,1}} },
	{ {{ 28,  0, 29}}, {{.5, .6,  .1}} }
};
#define NElems(x) sizeof(x)/sizeof(*x)
const unsigned
	NumPlanes = NElems(Planes),
	NumSpheres = NElems(Spheres),
	NumLights = NElems(Lights);
unsigned MAXTRACE = 2; // Maximum trace level

/* Actual raytracing! */

/**************/
// Determine whether an object is
// in direct eyesight on the given
// line, and determine exactly which
// point of the object is seen.
int RayFindObstacle
	(const XYZ& eye, const XYZ& dir,
	 double& HitDist, int& HitIndex,
	 XYZ& HitLoc, XYZ& HitNormal)
{
	// Try intersecting the ray with
	// each object and see which one
	// produces the closest hit.
	int HitType = -1;
   {for(unsigned i=0; i<NumSpheres; ++i)
	{
		XYZ V (eye - Spheres[i].center);
		double r = Spheres[i].radius,
			DV = dir.Dot(V),
			D2 = dir.Squared(),
			SQ = DV*DV
			   - D2*(V.Squared() - r*r);
		// Does the ray coincide
		// with the sphere?
		if(SQ < 1e-6) continue;
		// Determine where exactly
		double SQt = sqrt(SQ),
			Dist = dmin(-DV-SQt,
						-DV+SQt) / D2;
		if(Dist<1e-6 || Dist >= HitDist)
			continue;
		HitType = 1; HitIndex = i;
		HitDist = Dist;
		HitLoc = eye + (dir * HitDist);
		HitNormal =
			(HitLoc - Spheres[i].center)
				* (1/r);
	}}
   {for(unsigned i=0; i<NumPlanes; ++i)
	{
		double DV = -Planes[i].normal.Dot(dir);
		if(DV > -1e-6) continue;
		double D2 =
			Planes[i].normal.Dot(eye),
			Dist = (D2+Planes[i].offset) / DV;
		if(Dist<1e-6 || Dist>=HitDist)
			continue;
		HitType = 0; HitIndex = i;
		HitDist = Dist;
		HitLoc = eye + (dir * HitDist);
		HitNormal = -Planes[i].normal;
	}}
	return HitType;
}

const unsigned NumArealightVectors = 20;
XYZ ArealightVectors[NumArealightVectors];
void InitArealightVectors()
{
	// To smooth out shadows cast by lightsources,
	// I plan to approximate the lightsources with
	// a _cloud_ of lightsources around the point
	for(unsigned i=0; i<NumArealightVectors; ++i)
		for(unsigned n=0; n<3; ++n)
			ArealightVectors[i].d[n] =
				2.0 * (rand() / double(RAND_MAX) - 0.5);
}

// Shoot a camera-ray from the specified location
// to specified location, and determine the RGB
// color of the perception corresponding to that
// location.
void RayTrace(XYZ& resultcolor, const XYZ& eye, const XYZ& dir, int k)
{
	double HitDist = 1e6;
	XYZ HitLoc, HitNormal;
	int HitIndex, HitType;
	HitType = RayFindObstacle(eye,dir, HitDist,HitIndex, HitLoc,HitNormal);
	if(HitType != -1)
	{
		// Found an obstacle. Next, find out how it is illuminated.
		// Shoot a ray to each lightsource, and determine if there
		// is an obstacle behind it. This is called "diffuse light".
		// To smooth out the infinitely sharp shadows caused by
		// infinitely small point-lightsources, assume the lightsource
		// is actually a cloud of small lightsources around its center.
		XYZ DiffuseLight = {{0,0,0}}, SpecularLight = {{0,0,0}};
		XYZ Pigment = {{1, 0.98, 0.94}}; // default pigment
		for(unsigned i=0; i<NumLights; ++i)
			for(unsigned j=0; j<NumArealightVectors; ++j)
			{
				XYZ V((Lights[i].where + ArealightVectors[j]) - HitLoc);
				double LightDist = V.Len();
				V.Normalize();
				double DiffuseEffect = HitNormal.Dot(V) / (double)NumArealightVectors;
				double Attenuation = (1 + pow(LightDist / 34.0, 2.0));
				DiffuseEffect /= Attenuation;
				if(DiffuseEffect > 1e-3)
				{
					double ShadowDist = LightDist - 1e-4;
					XYZ a,b;
					int q,t = RayFindObstacle(HitLoc + V*1e-4, V, ShadowDist,q, a,b);
					if(t == -1) // No obstacle occluding the light?
						DiffuseLight += Lights[i].colour * DiffuseEffect;
				}
			}
		if(k > 1)
		{
			// Add specular light/reflection, unless recursion depth is at max
			XYZ V(-dir); V.MirrorAround(HitNormal);
			RayTrace(SpecularLight, HitLoc + V*1e-4, V, k-1);
		}

		// AQUI COMEÇA CÁLCULO DE LUMINOSIDADE BASEADO EM MATERIAIS

		switch(HitType)
		{
			case 0: // plane
				DiffuseLight *= 0.9;
				SpecularLight *= 0.5;
				// Color the different walls differently
				switch(HitIndex % 4)
				{
					case 0: Pigment.Set(0.9, 0.7, 0.6); break;
					case 1: Pigment.Set(0.6, 0.7, 0.7); break;
					case 2: Pigment.Set(0.5, 0.8, 0.3); break;
					case 3: Pigment.Set(0.1, 0.1, 0.1); break;
				}
				break;
			case 1: // sphere
				DiffuseLight  *= 1.0;
				SpecularLight  *= 0.74;
		}
		resultcolor = (DiffuseLight + SpecularLight) * Pigment;

		// AQUI TERMINA CÁLCULO DE LUMINOSIDADE BASEADO EM MATERIAIS
	}
}

/* MAIN PROGRAM */
int main(int argc, char **argv) // <<<<----- boa pratica
{
	bool toFile = false;
	char outfile[256];
	unsigned W = 320, H = 240;
	MAXTRACE = 3;

	if (argc > 1) { sscanf(argv[1], "%u", &W); sscanf(argv[2], "%u", &H); }
	if (argc > 3) { sscanf(argv[3], "%u", &MAXTRACE); }
	if (argc > 4) { toFile = true; sscanf(argv[4], "%s", outfile); }

	double AR = double(W)/double(H);

	sf::RenderWindow window(sf::VideoMode(W, H), "MultiThreaded SFML PathTracing", sf::Style::None);
	if (toFile) window.setVisible(false);
	else window.setVerticalSyncEnabled(true);

	sf::Image image;
	image.create(W, H, sf::Color::Black);
	sf::Texture texture;
	if (!texture.create(W, H))
		printf("Erro ao criar textura\n");

	sf::Sprite sprite;

	sf::Uint8 *pixels = new sf::Uint8[W * H * 4];
	for (unsigned i=0; i < W*H*4; i++) pixels[i] = 0;

	texture.update(pixels);
	//texture.update(image);
	sprite.setTexture(texture);


	InitArealightVectors();
	XYZ camangle	  = { {0,0,0} };
	XYZ camangledelta = { {-.005, -.011, -.017} };
	//XYZ camlook	   = { {0,0,0} };
	XYZ camlook       = {{0,0,0}};
	XYZ camlookdelta  = { {-.001, .005, .004} };

	//double zoom = 46.0, zoomdelta = 0.99;
	double zoom = 3.0, zoomdelta = 0.99;
	double contrast = 6.2, contrast_offset = -0.12;

	sf::Event event;
	for(unsigned frameno=0; frameno<9300; ++frameno)
	{
		fprintf(stderr, "Begins frame %u; contrast %g, contrast offset %g\n",
			frameno,contrast,contrast_offset);

		// Put camera between the central sphere and the walls
		XYZ campos = { { 0.0, -3.0, 16.0} };
		// Rotate it around the center
		Matrix camrotatematrix, camlookmatrix;
		camrotatematrix.InitRotate(camangle);
		camrotatematrix.Transform(campos);
		camlookmatrix.InitRotate(camlook);

		// Determine the contrast ratio for this frame's pixels
		double thisframe_min = 100;
		double thisframe_max = -100;

		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

	  #pragma omp parallel for collapse(2)
		for(unsigned y=0; y<H; ++y)
			for(unsigned x=0; x<W; ++x)
			{
				XYZ camray = { { x / double(W) - 0.5,
								 y / double(H) - 0.5,
								 zoom } };
				camray.d[0] *= AR; // Aspect ratio correction
				camray.Normalize();
				camlookmatrix.Transform(camray);
				XYZ campix;
				RayTrace(campix, campos, camray, MAXTRACE);
				campix *= 0.5;
			  #pragma omp critical
			  {
				// Update frame luminosity info for automatic contrast adjuster
				double lum = campix.Luma();
				#pragma omp flush(thisframe_min,thisframe_max)
				if(lum < thisframe_min) thisframe_min = lum;
				if(lum > thisframe_max) thisframe_max = lum;
				#pragma omp flush(thisframe_min,thisframe_max)
			  }
				// Exaggerate the colors to bring contrast better forth
				campix = (campix + contrast_offset) * contrast;
				// Clamp, and compensate for display gamma (for dithering)
				campix.ClampWithDesaturation();
				//XYZ campixG = campix.Pow(Gamma);
				//XYZ qtryG = campixG;

				// TODO Draw pixel
				{
					//printf("Cor: %f %f %f\n", campix.d[0], campix.d[1], campix.d[2]);
					
					pixels[((y*W) + x)*4 + 0] = unsigned(campix.d[0]*255); // Red
					pixels[((y*W) + x)*4 + 1] = unsigned(campix.d[1]*255); // Green
					pixels[((y*W) + x)*4 + 2] = unsigned(campix.d[2]*255); // Blue
					pixels[((y*W) + x)*4 + 3] = 255; // Alpha
				
					/*
					sf::Color px((campix.d[0] * 255),
								 (campix.d[1] * 255),
								 (campix.d[2] * 255));
					image.setPixel(x, y, px);
					*/
				}
			}

		if (toFile)
		{
			image.create(W, H, pixels);
			image.saveToFile(outfile);
			break;
		}
		else
		{
			// Atualizar textura
			texture.update(pixels);
			// Apresentar resultado
			window.draw(sprite);
			window.display();
		}

		printf("Terminado frame %4d\n", frameno);
		
		// Tweak coordinates / camera parameters for the next frame
		double much = 1.0;

		// In the beginning, do some camera action (play with zoom)
		if(zoom <= 1.1)
			zoom = 1.1;
		else
		{
			if(zoom > 40) { if(zoomdelta > 0.95) zoomdelta -= 0.001; }
			else if(zoom < 3) { if(zoomdelta < 0.99) zoomdelta += 0.001; }
			zoom *= zoomdelta;
			much = 1.1 / pow(zoom/1.1, 3);
		}

		// Update the rotation angle
		//camlook  += camlookdelta * much;
		//camangle += camangledelta * much;

		// Dynamically readjust the contrast based on the contents
		// of the last frame
		double middle = (thisframe_min + thisframe_max) * 0.5;
		double span   = (thisframe_max - thisframe_min);
		thisframe_min = middle - span*0.60; // Avoid dark tones
		thisframe_max = middle + span*0.37; // Emphasize bright tones
		double new_contrast_offset = -thisframe_min;
		double new_contrast		= 1 / (thisframe_max - thisframe_min);
		// Avoid too abrupt changes, though
		double l = 0.85;
		if(frameno == 0) l = 0.7;
		contrast_offset = (contrast_offset*l + new_contrast_offset*(1.0-l));
		contrast		= (contrast*l + new_contrast*(1.0-l));
	}

	return 0;
}
