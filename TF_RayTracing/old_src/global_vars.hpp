#ifndef __RT_GLOBAL__
#define __RT_GLOBAL__

#include "commons.hpp"
#include "objects.hpp"

unsigned W = 640, H = 480;
unsigned rW = W, rH = H;
char outfile[256];
bool toFile = false;
bool rerender = false;
bool die = false;
bool resize = false;
sf::Image image;
sf::Texture texture;
sf::Uint8 *pixels; // 0 - 255 para cada canal (RGBA)
sf::Sprite sprite;
std::mutex mut;
std::mutex rmut;
std::mutex dmut;

unsigned MAXTRACE = 2; // Maximum trace level
unsigned SURFACE_SAMPLES = 1; // For sampling roughness

XYZ camangle	  = {{0,0,0}};
XYZ camangledelta = {{-.005, -.011, -.017}};
XYZ camlook       = {{0,0,0}};
XYZ camlookdelta  = {{-.001, .005, .004}};
XYZ campos        = {{ 0.0, -3.0, 16.0}};

std::vector<Plane> Planes;
std::vector<Sphere> Spheres;
std::vector<LightSource> Lights;

#endif
