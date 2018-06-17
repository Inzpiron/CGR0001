#ifndef __RT_GEOMETRY__
#define __RT_GEOMETRY__

#include "commons.hpp"

// Declarations for scene description
// Walls are planes. Planes have a
// normal vector and a distance.
inline double dmin(double a,double b) { return a<b ? a : b; }
struct XYZ
{
	double d[3];
	// Declare operators pertinent to vectors in general:
	inline void Set(double a,double b,double c) { d[0]=a; d[1]=b; d[2]=c;}
	#define do_op(o) \
		inline void operator o##= (const XYZ& b) { for(unsigned n=0; n<3; ++n) d[n] o##= b.d[n]; } \
		inline void operator o##= (double b)	 { for(unsigned n=0; n<3; ++n) d[n] o##= b; } \
		XYZ operator o (const XYZ& b) const	  { XYZ tmp(*this); tmp o##= b; return tmp; } \
		XYZ operator o (double b)	 const	  { XYZ tmp(*this); tmp o##= b; return tmp; }
	do_op(*)
	do_op(+)
	do_op(-)
	#undef do_op
	XYZ operator- () const { XYZ tmp = { { -d[0], -d[1], -d[2] } }; return tmp; }
	double operator[] (const int a) const { if(a > 2 || a < 0) return 0; return d[a]; }
	XYZ Pow(double b) const
		{ XYZ tmp = {{ pow(d[0],b), pow(d[1],b), pow(d[2],b) }}; return tmp; }
	// Operators pertinent to geometrical vectors:
	inline double Dot(const XYZ& b) const
		{ return d[0]*b.d[0] + d[1]*b.d[1] + d[2]*b.d[2]; }
	inline double Squared() const	 { return Dot(*this); }
	inline double Len()	 const	 { return sqrt(Squared()); }
	inline void Normalize()		   { *this *= 1.0 / Len(); }
	void MirrorAround(const XYZ& axis)
	{
		XYZ N = axis; N.Normalize();
		double v = Dot(N);
		*this = N * (v+v) - *this;
	}
	// Operators pertinent to colour vectors (RGB):
	inline double Luma() const { return d[0]*0.299 + d[1]*0.587 + d[2]*0.114; }
	void Clamp()
	{
		for(unsigned n=0; n<3; ++n)
		{
			if(d[n] < 0.0) d[n] = 0.0;
			else if(d[n] > 1.0) d[n] = 1.0;
		}
	}
	void ClampWithDesaturation()
	{
		// If the color represented by this triplet
		// is too bright or too dim, decrease the
		// saturation as much as required, while keeping
		// the luma unmodified.
		double l = Luma(), sat = 1.0;
		if(l > 1.0) { d[0] = d[1] = d[2] = 1.0; return; }
		if(l < 0.0) { d[0] = d[1] = d[2] = 0.0; return; }
		// If any component is over the bounds, calculate how
		// much the saturation must be reduced to achieve an
		// in-bounds value. Since the luma was verified to be
		// in 0..1 range, a maximum reduction of saturationto
		// 0% will always produce an in-bounds value, but
		// usually such a drastic reduction is not necessary.
		// Because we're only doing relative modifications,
		// we don't need to determine the original saturation
		// level of the pixel.
		for(int n=0; n<3; ++n)
			if(d[n] > 1.0) sat = dmin(sat, (l-1.0) / (l-d[n]));
			else if(d[n] < 0.0) sat = dmin(sat, l  / (l-d[n]));
		if(sat != 1.0)
			 { *this = (*this - l) * sat + l; Clamp(); }
	}
};

struct Matrix
{
	XYZ m[3];
	void InitRotate(const XYZ& angle)
	{
		double Cx = cos(angle.d[0]), Cy = cos(angle.d[1]), Cz = cos(angle.d[2]);
		double Sx = sin(angle.d[0]), Sy = sin(angle.d[1]), Sz = sin(angle.d[2]);
		double sxsz = Sx*Sz, cxsz = Cx*Sz;
		double cxcz = Cx*Cz, sxcz = Sx*Cz;
		Matrix result = {{ {{ Cy*Cz, Cy*Sz, -Sy }},
						   {{ sxcz*Sy - cxsz, sxsz*Sy + cxcz, Sx*Cy }},
						   {{ cxcz*Sy + sxsz, cxsz*Sy - sxcz, Cx*Cy }} }};
		*this = result;
	}
	void Transform(XYZ& vec)
	{
		vec.Set( m[0].Dot(vec), m[1].Dot(vec), m[2].Dot(vec) );
	}
};

#endif
