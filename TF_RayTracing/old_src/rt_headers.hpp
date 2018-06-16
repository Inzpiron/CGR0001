#ifndef __RT_HEADERS__
#define __RT_HEADERS__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "utilities.hpp"
#include "objects.hpp"

unsigned MAXTRACE = 2; // Maximum trace level

/* Actual raytracing! */

/**************/
// Determine whether an object is
// in direct eyesight on the given
// line, and determine exactly which
// point of the object is seen.
int RayFindObstacle	(const XYZ& eye, const XYZ& dir, double& HitDist, int& HitIndex,
	 XYZ& HitLoc, XYZ& HitNormal)
{
	double ERR_LOW = 1e-6;
	unsigned NumSpheres = Spheres.size();
	unsigned NumPlanes  = Planes.size();
	
	// Try intersecting the ray with
	// each object and see which one
	// produces the closest hit.
	int HitType = -1;
	for(unsigned i=0; i<NumSpheres; ++i)
	{
		XYZ V (eye - Spheres[i].center);
		double r = Spheres[i].radius,
			DV = dir.Dot(V),
			D2 = dir.Squared(),
			SQ = DV*DV
			   - D2*(V.Squared() - r*r);
		// Does the ray coincide
		// with the sphere?
		if(SQ < ERR_LOW) continue;
		// Determine where exactly
		double SQt = sqrt(SQ),
			Dist = dmin(-DV-SQt,
						-DV+SQt) / D2;
		if(Dist<ERR_LOW || Dist >= HitDist)
			continue;
		HitType = 1; HitIndex = i;
		HitDist = Dist;
		HitLoc = eye + (dir * HitDist);
		HitNormal =
			(HitLoc - Spheres[i].center)
				* (1/r);
	}
	for(unsigned i=0; i<NumPlanes; ++i)
	{
		double DV = -Planes[i].normal.Dot(dir);
		if(DV > -ERR_LOW) continue;
		double D2 =
			Planes[i].normal.Dot(eye),
			Dist = (D2+Planes[i].offset) / DV;
		if(Dist<ERR_LOW || Dist>HitDist)
			continue;
		HitType = 0; HitIndex = i;
		HitDist = Dist;
		HitLoc = eye + (dir * HitDist);
		HitNormal = -Planes[i].normal;
	}
	return HitType;
}

unsigned NumArealightVectors = 20;
XYZ *ArealightVectors;
void InitArealightVectors()
{
	if (ArealightVectors != NULL) delete ArealightVectors;
	ArealightVectors = new XYZ[NumArealightVectors];
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
	unsigned NumLights = Lights.size();

	double HitDist = 1e6;
	XYZ HitLoc, HitNormal;
	int HitIndex, HitType;
	HitType = RayFindObstacle(eye, dir, HitDist, HitIndex, HitLoc, HitNormal);
	if(HitType != -1)
	{
		// Found an obstacle. Next, find out how it is illuminated.
		// Shoot a ray to each lightsource, and determine if there
		// is an obstacle behind it. This is called "diffuse light".
		// To smooth out the infinitely sharp shadows caused by
		// infinitely small point-lightsources, assume the lightsource
		// is actually a cloud of small lightsources around its center.

		// DIFUSA
		XYZ DiffuseLight = {{0,0,0}}, SpecularLight = {{0,0,0}};
		XYZ Pigment = {{1, 0.98, 0.94}}; // default pigment
		for(unsigned i=0; i<NumLights; i++)
		{
			for(unsigned j=0; j<NumArealightVectors; ++j)
			{
				XYZ V((Lights[i].center + ArealightVectors[j]) - HitLoc);
				double LightDist = V.Len();
				V.Normalize();
				double DiffuseEffect = HitNormal.Dot(V) / (double)NumArealightVectors;
				double Attenuation = (1 + pow(LightDist / 34.0, 2.0));
				DiffuseEffect /= Attenuation;
				if(DiffuseEffect > 1e-3)
				{
					double ShadowDist = LightDist - 1e-4;
					XYZ a,b;
					int q, t = RayFindObstacle(HitLoc + V*1e-4, V, ShadowDist, q, a, b);
					if(t == -1) // No obstacle occluding the light?
						DiffuseLight += Lights[i].colour * DiffuseEffect;
				}
			}
		}
		// END DIFUSA

		// SPECULAR
		if(k > 1)
		{
			// Add specular light/reflection, unless recursion depth is at max
			XYZ V(-dir); V.MirrorAround(HitNormal);
			RayTrace(SpecularLight, HitLoc + V*1e-4, V, k-1);
		}
		// END SPECULAR

		// AQUI COMEÇA CÁLCULO DE LUMINOSIDADE BASEADO EM MATERIAIS
		switch(HitType)
		{
			case 0: // plane

				//DiffuseLight *= 0.9;
				//SpecularLight *= 0.5;
				// Color the different walls differently
				switch(HitIndex % 4)
				{
					case 0: Pigment.Set(0.9, 0.7, 0.6); break;
					case 1: Pigment.Set(0.6, 0.7, 0.7); break;
					case 2: Pigment.Set(0.5, 0.8, 0.3); break;
					case 3: Pigment.Set(0.1, 0.1, 0.1); SpecularLight *= 0.4; break;
				}
				break;
			case 1: // sphere
				DiffuseLight   *= 0.34;
				SpecularLight  *= 0.66;
				Pigment.Set(0.7, 0.73, 0.7);
				break;
		}
		resultcolor = (DiffuseLight + SpecularLight) * Pigment;
		// AQUI TERMINA CÁLCULO DE LUMINOSIDADE BASEADO EM MATERIAIS
	}
}

XYZ camangle	  = {{0,0,0}};
XYZ camangledelta = {{-.005, -.011, -.017}};
XYZ camlook       = {{0,0,0}};
XYZ camlookdelta  = {{-.001, .005, .004}};
XYZ campos        = {{ 0.0, -3.0, 16.0}};

//double zoom = 46.0, zoomdelta = 0.99;
double zoom = 3.0, zoomdelta = 0.99;
double contrast = 6.2, contrast_offset = -0.12;

// Determine the contrast ratio for this frame's pixels
double thisframe_min = 100;
double thisframe_max = -100;

Matrix camrotatematrix, camlookmatrix;

/* MAIN PROGRAM */
void render(unsigned W, unsigned H, sf::Uint8 *pixels)
{
	double AR = W/double(H);

	// Rotate it around the center
	camrotatematrix.InitRotate(camangle);
	camrotatematrix.Transform(campos);
	camlookmatrix.InitRotate(camlook);

	// Determine the contrast ratio for this frame's pixels
	thisframe_min = 100;
	thisframe_max = -100;

  #pragma omp parallel for collapse(2)
	for(unsigned y=0; y<H; ++y)
	{
		for(unsigned x=0; x<W; ++x)
		{
			XYZ camray = {{ x / double(W) - 0.5,
							y / double(H) - 0.5,
							zoom }};
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
			//campix = (campix + contrast_offset) * contrast;
			campix = campix * contrast;
			// Clamp, and compensate for display gamma (for dithering)
			campix.ClampWithDesaturation();
			//XYZ campixG = campix.Pow(Gamma);
			//XYZ qtryG = campixG;

			// Draw pixel
			{
				pixels[((y*W) + x)*4 + 0] = unsigned(campix.d[0]*255); // Red
				pixels[((y*W) + x)*4 + 1] = unsigned(campix.d[1]*255); // Green
				pixels[((y*W) + x)*4 + 2] = unsigned(campix.d[2]*255); // Blue
				pixels[((y*W) + x)*4 + 3] = 255; // Alpha
			}
		}
	}
}

void doSketch(unsigned W, unsigned H, sf::Uint8 *pixels)
{
	double AR = W/double(H);

	// Rotate it around the center
	camrotatematrix.InitRotate(camangle);
	camrotatematrix.Transform(campos);
	camlookmatrix.InitRotate(camlook);

	// Determine the contrast ratio for this frame's pixels
	thisframe_min = 100;
	thisframe_max = -100;

  #pragma omp parallel for collapse(2)
	for(unsigned y=0; y<H; ++y)
	{
		for(unsigned x=0; x<W; ++x)
		{
			XYZ camray = {{ x / double(W) - 0.5,
							y / double(H) - 0.5,
							zoom }};
			camray.d[0] *= AR; // Aspect ratio correction
			camray.Normalize();
			camlookmatrix.Transform(camray);
			XYZ campix;
			RayTrace(campix, campos, camray, 0);
			campix *= 0.5;
			/*
		  #pragma omp critical
		  {
			// Update frame luminosity info for automatic contrast adjuster
			double lum = campix.Luma();
			#pragma omp flush(thisframe_min,thisframe_max)
			if(lum < thisframe_min) thisframe_min = lum;
			if(lum > thisframe_max) thisframe_max = lum;
			#pragma omp flush(thisframe_min,thisframe_max)
		  }
		  	*/
			campix = campix * contrast;
			campix.ClampWithDesaturation();

			// Draw pixel
			{
				pixels[((y*W) + x)*4 + 0] = unsigned(campix.d[0]*255); // Red
				pixels[((y*W) + x)*4 + 1] = unsigned(campix.d[1]*255); // Green
				pixels[((y*W) + x)*4 + 2] = unsigned(campix.d[2]*255); // Blue
				pixels[((y*W) + x)*4 + 3] = 255; // Alpha
			}
		}
	}
}

#endif