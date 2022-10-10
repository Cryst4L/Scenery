#pragma once

#include <math.h>
#include "Algebra.h"
#include "Point.h"
#include "Color.h"
#include "Draw.h"

typedef struct {
	Vector3D position;
	Color color;
} Vertice;

typedef struct {
	Vertice a, b, c;
} Triangle;

//static const double DELTA_Z = 0.1;

void flatLinearLine(Vertice a, Vertice b, RGBFrame& frame, GSFrame& depth_map)
{
	if (a.position.x > b.position.x)
		std::swap(a, b);	

	int x1 = floor(a.position.x); 
	int x2 =  ceil(b.position.x);
	int y  = floor(a.position.y);

	double z1 = a.position.z; 
	double z2 = b.position.z;

	if (x1 == x2)
		return;

	if (y < 0 || y > (frame.height() - 1))
		return;

	if (x1 < 1) 
		x1 = 1;

	if (x2 > (frame.width() - 1))
		x2 = (frame.width() - 1);

	double dz = (z2 - z1) / (double) (x2 - x1);

	double dr = (b.color.r() - a.color.r()) / (x2 - x1);
	double dg = (b.color.g() - a.color.g()) / (x2 - x1);
	double db = (b.color.b() - a.color.b()) / (x2 - x1);

	double z = z1 - dz;

	double cr = a.color.r() - dr;
	double cg = a.color.g() - dg;
	double cb = a.color.b() - db;


	for(int x = x1; x < x2 ; x++)
	{
		z  += dz;
		cr += dr;
		cg += dg;
		cb += db;

		double t = depth_map(x, y);

		if (z < t || t == 0)
		{
			depth_map(x, y) = z;
			frame(x, y) = Color(cr, cg, cb);
		}
	}

}


///////////////////////////////////////////////////////////////////////////////

void rasterTriangle(Vertice a, Vertice b, Vertice c, RGBFrame& frame, GSFrame& depth_map)
{
/*
	if (a.position.y > b.position.y) std::swap(a, b);

	if (a.position.y > c.position.y) std::swap(a, c);

	if (b.position.y > c.position.y) std::swap(b, c);
*/
	if (a.position.y > b.position.y) 
		std::swap(a, b);

	if (a.position.y > c.position.y) 
		std::swap(a, c);

	if (b.position.y > c.position.y) 
		std::swap(b, c);

	// TODO Optional
	a.position.y = (int) floor(a.position.y);
	b.position.y = (int) round(b.position.y);
	c.position.y = (int) ceil( c.position.y);


	double dx1 = (b.position.x - a.position.x) / (b.position.y - a.position.y);
	double dx2 = (c.position.x - a.position.x) / (c.position.y - a.position.y);
	double dx3 = (c.position.x - b.position.x) / (c.position.y - b.position.y);

	double dz1 = (b.position.z - a.position.z) / (b.position.y - a.position.y);
	double dz2 = (c.position.z - a.position.z) / (c.position.y - a.position.y);
	double dz3 = (c.position.z - b.position.z) / (c.position.y - b.position.y);

	Vertice s = a, e = a;


	if (dx1 > dx2) {

		for (; s.position.y < b.position.y; 
			   s.position.y++, e.position.y++, 
			   s.position.x += dx2, e.position.x += dx1,
			   s.position.z += dz2, e.position.z += dz1)
		{	

			double s_ratio = (s.position.x - a.position.x) / (c.position.x - a.position.x);
			double e_ratio = (e.position.x - a.position.x) / (b.position.x - a.position.x);

			if ((c.position.x - a.position.x) == 0)
				s_ratio = (s.position.y - a.position.y) / (c.position.y - a.position.y);

			if ((b.position.x - a.position.x) == 0)
				e_ratio = (e.position.y - a.position.y) / (b.position.y - a.position.y);

			s.color = RGBMix(a.color, c.color, s_ratio);
			e.color = RGBMix(a.color, b.color, e_ratio); 

			flatLinearLine(s, e, frame, depth_map);
		}

		//std::cout << " ----------- " << std::endl;

		e = b;

		for (; s.position.y < c.position.y; 
		       s.position.y++, e.position.y++, 
		       s.position.x += dx2, e.position.x += dx3,
		       s.position.z += dz2, e.position.z += dz3)
		{

			double s_ratio = (s.position.x - a.position.x) / (c.position.x - a.position.x);
			double e_ratio = (e.position.x - b.position.x) / (c.position.x - b.position.x);

			if ((c.position.x - a.position.x) == 0)
				s_ratio = (s.position.y - a.position.y) / (c.position.y - a.position.y);

			if ((c.position.x - b.position.x) == 0)
				e_ratio = (e.position.y - b.position.y) / (c.position.y - b.position.y);

			s.color = RGBMix(a.color, c.color, s_ratio);
			e.color = RGBMix(b.color, c.color, e_ratio);
 
			flatLinearLine(s, e, frame, depth_map);
		}

	} else {
		

		for (; s.position.y < b.position.y; 
		       s.position.y++, e.position.y++, 
		       s.position.x += dx1, e.position.x += dx2,
		       s.position.z += dz1, e.position.z += dz2)
		{

			double s_ratio = (s.position.x - a.position.x) / (b.position.x - a.position.x);
			double e_ratio = (e.position.x - a.position.x) / (c.position.x - a.position.x);

			if ((b.position.x - a.position.x) == 0)
				s_ratio = (s.position.y - a.position.y) / (b.position.y - a.position.y);

			if ((c.position.x - a.position.x) == 0)
				e_ratio = (e.position.y - a.position.y) / (c.position.y - a.position.y);

			s.color = RGBMix(a.color, b.color, s_ratio);
			e.color = RGBMix(a.color, c.color, e_ratio); 

			flatLinearLine(s, e, frame, depth_map);
		}

		s = b;

		for (; s.position.y < c.position.y; 
		       s.position.y++, e.position.y++, 
		       s.position.x += dx3, e.position.x += dx2,
			   s.position.z += dz3, e.position.z += dz2)
		{

			double s_ratio = (s.position.x - b.position.x) / (c.position.x - b.position.x);
			double e_ratio = (e.position.x - a.position.x) / (c.position.x - a.position.x);

			if ((c.position.x - b.position.x) == 0)
				s_ratio = (s.position.y - b.position.y) / (c.position.y - b.position.y);

			if ((c.position.x - a.position.x) == 0)
				e_ratio = (e.position.y - a.position.y) / (c.position.y - a.position.y);

			s.color = RGBMix(b.color, c.color, s_ratio);
			e.color = RGBMix(a.color, c.color, e_ratio); 
			
			flatLinearLine(s, e, frame, depth_map);
		}
	}

}

