#pragma once

#include <math.h>
#include "Point.h"

void drawFlatLine(RGBFrame& frame, double x1, double x2, int y, Color color)
{
	if (x1 == x2)
		return;

	if ((y < 0) || (y >= frame.height()))
		return;

	if (x1 > x2)
		std::swap(x1, x2);

	x1 = ceil(x1);
	x2 = floor(x2);

	x1 = std::max(1.0, std::min(x1, (double) frame.width()));
	x2 = std::max(1.0, std::min(x2, (double) frame.width()));

	for(int x = x1; x <= x2 ; x++)
		frame(x, y) = color;
}

////////////////////////////////////////////////////////////////////////////////

void flatLinearLine(RGBFrame& frame, int x1, int x2, int y, Color c1, Color c2)
{
	if ((y < 0) || (y >= frame.height()))
		return;

	if (x1 == x2)
		return;

	if (x1 > x2)
		std::swap(x1, x2);

	if ((x1 >= frame.width()) || (x2 < 0))
		return;

	x1 = floor(x1);
	x2 = ceil(x2);

	///////////////////////////////////////////////

	double dr = (c2.r() - c1.r()) / (x2 - x1);
	double dg = (c2.g() - c1.g()) / (x2 - x1);
	double db = (c2.b() - c1.b()) / (x2 - x1);

	double cr = c1.r() - dr;
	double cg = c1.g() - dg;
	double cb = c1.b() - db;

	///////////////////////////////////////////////

	if (x1 < 0)
	{
		int delta = -x1;

		cr += delta * dr;
		cg += delta * dg;
		cb += delta * db;

		x1 = 0;
	}

	if (x2 >= frame.width())
		x2 = frame.width() - 1;

	///////////////////////////////////////////////

	for(int x = x1; x < (x2 + 1) ; x++) // XXX HERE
	{
		cr += dr;
		cg += dg;
		cb += db;

		frame(x, y) = Color(cr, cg, cb);	
	}
}

////////////////////////////////////////////////////////////////////////////////

void drawTriangle(RGBFrame& frame, Point a, Point b, Point c, Color color)
{
	if (a.y() > b.y())
		std::swap(a, b);

	if (a.y() > c.y())
		std::swap(a, c);

	if (b.y() > c.y())
		std::swap(b, c);

	//////////////////////////////////////////////

	a.y() = (int) floor(a.y());
	b.y() = (int) round(b.y());
	c.y() = (int) ceil( c.y());

	//////////////////////////////////////////////

	double dx1 = (b.x() - a.x()) / (b.y() - a.y());
	double dx2 = (c.x() - a.x()) / (c.y() - a.y());
	double dx3 = (c.x() - b.x()) / (c.y() - b.y());

	//////////////////////////////////////////////

	Point s = a, e = a;

	if (dx1 > dx2) {

		if (a.y() == b.y())
			drawFlatLine(frame, a.x(), b.x(), a.y(), color);
		else
		{
			for (; s.y() < b.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx2, e.x() += dx1)
			{	
				drawFlatLine(frame, s.x(), e.x(), s.y(), color);
			}
		}

		e = b;

		if (b.y() == c.y())
			drawFlatLine(frame, b.x(), c.x(), b.y(), color);
		else
		{
			for (; s.y() < c.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx2, e.x() += dx3)
			{
				drawFlatLine(frame, s.x(), e.x(), s.y(), color);
			}
		}

	} else {
	
		if (a.y() == b.y())
			drawFlatLine(frame, a.x(), b.x(), a.y(), color);
		else
		{
			for (; s.y() < b.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx1, e.x() += dx2)
			{
				drawFlatLine(frame, s.x(), e.x(), s.y(), color);
			}
		}
						
		s = b;

		if (b.y() == c.y())
			drawFlatLine(frame, b.x(), c.x(), b.y(), color);
		else
		{
			for (; s.y() < c.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx3, e.x() += dx2)
			{
				drawFlatLine(frame, s.x(), e.x(), s.y(), color);
			}
		}

	}
}

////////////////////////////////////////////////////////////////////////////////

void drawTriangle(RGBFrame& frame, Point a, Point b, Point c, Color ca, Color cb, Color cc)
{
	if (a.y() > b.y())
	{
		std::swap(a, b);
		std::swap(ca, cb);
	}

	if (a.y() > c.y())
	{
		std::swap(a, c);
		std::swap(ca, cc);
	}

	if (b.y() > c.y())
	{
		std::swap(b, c);
		std::swap(cb, cc);
	}

	//////////////////////////////////////////////

	a.y() = (int) floor(a.y());
	b.y() = (int) round(b.y());
	c.y() = (int) ceil( c.y());

	//////////////////////////////////////////////

	double dx1 = (b.x() - a.x()) / (b.y() - a.y());
	double dx2 = (c.x() - a.x()) / (c.y() - a.y());
	double dx3 = (c.x() - b.x()) / (c.y() - b.y());

	//////////////////////////////////////////////

	Point s = a, e = a;
	Color cs = ca, ce = ca;

	if (dx1 > dx2) {

		if (a.y() == b.y())
			flatLinearLine(frame, a.x(), b.x(), a.y(), ca, cb);
		else
		{
			for (; s.y() < b.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx2, e.x() += dx1)
			{	
				double s_ratio = (s.y() - a.y()) / (c.y() - a.y());
				double e_ratio = (e.y() - a.y()) / (b.y() - a.y());

				cs = RGBMix(ca, cc, s_ratio);
				ce = RGBMix(ca, cb, e_ratio);

				flatLinearLine(frame, s.x(), e.x(), s.y(), cs, ce);
			}
		}

		e = b;

		if (b.y() == c.y())
			flatLinearLine(frame, b.x(), c.x(), b.y(), cb, cc);
		else
		{
			for (; s.y() < c.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx2, e.x() += dx3)
			{
				double s_ratio = (s.y() - a.y()) / (c.y() - a.y());
				double e_ratio = (e.y() - b.y()) / (c.y() - b.y());

				cs = RGBMix(ca, cc, s_ratio);
				ce = RGBMix(cb, cc, e_ratio);

				flatLinearLine(frame, s.x(), e.x(), s.y(), cs, ce);
			}
		}

	} else {
	
		if (a.y() == b.y())
			flatLinearLine(frame, a.x(), b.x(), a.y(), ca, cb);
		else
		{
			for (; s.y() < b.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx1, e.x() += dx2)
			{
				double s_ratio = (s.y() - a.y()) / (b.y() - a.y());
				double e_ratio = (e.y() - a.y()) / (c.y() - a.y());

				cs = RGBMix(ca, cb, s_ratio);
				ce = RGBMix(ca, cc, e_ratio); 

				flatLinearLine(frame, s.x(), e.x(), s.y(), cs, ce);
			}
		}
						
		s = b;

		if (b.y() == c.y())
			flatLinearLine(frame, b.x(), c.x(), b.y(), cb, cc);
		else
		{
			for (; s.y() < c.y(); 
				   s.y()++, e.y()++, 
				   s.x() += dx3, e.x() += dx2)
			{

				double s_ratio = (s.y() - b.y()) / (c.y() - b.y());
				double e_ratio = (e.y() - a.y()) / (c.y() - a.y());

				cs = RGBMix(cb, cc, s_ratio);
				ce = RGBMix(ca, cc, e_ratio); 

				flatLinearLine(frame, s.x(), e.x(), s.y(), cs, ce);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void drawLine(RGBFrame& frame, Point p1, Point p2, double width, Color color, double outline_width = 0, Color outline_color = Color::Black)
{
	Point a1, b1, a2, b2;

	if (p1.x() == p2.x())
	{
		a1 = Point(p1.x() - 0.5 * width, p1.y());
		b1 = Point(p1.x() + 0.5 * width, p1.y());

		a2 = Point(p2.x() - 0.5 * width, p2.y());
		b2 = Point(p2.x() + 0.5 * width, p2.y());
	}
	else if (p1.y() == p2.y())
	{
		a1 = Point(p1.x(), p1.y() - 0.5 * width);
		b1 = Point(p1.x(), p1.y() + 0.5 * width);

		a2 = Point(p2.x(), p2.y() - 0.5 * width);
		b2 = Point(p2.x(), p2.y() + 0.5 * width);
	}
	else
	{
		double slope = (p2.y() - p1.y()) / (p2.x() - p1.x());

		Point offset(slope, -1);
		offset *= 0.5 * width / norm(offset);
		
		a1 = p1 + offset;
		b1 = p1 - offset;

		a2 = p2 + offset;
		b2 = p2 - offset;
	}
/*
	std::cout << "a1 = (" << a1.x() << "; " << a1.y() << ")" << std::endl;
	std::cout << "a2 = (" << a2.x() << "; " << a2.y() << ")" << std::endl;

	std::cout << "b1 = (" << b1.x() << "; " << b1.y() << ")" << std::endl;
	std::cout << "b2 = (" << b2.x() << "; " << b2.y() << ")" << std::endl;
*/
	drawTriangle(frame, a1, b1, a2, color);
	drawTriangle(frame, b1, a2, b2, color);


	if (outline_width != 0)
	{
		Point offset = (a1 - p1);
		offset *= 0.5 * outline_width / norm(a1 -p1);

		Point m1 = a1 + offset;
		Point n1 = b1 - offset;

		drawLine(frame, m1, n1, outline_width, outline_color, 0);

		Point m2 = a2 + offset;
		Point n2 = b2 - offset;

		drawLine(frame, m2, n2, outline_width, outline_color, 0);

		drawLine(frame, a1, a2, outline_width, outline_color, 0);
		drawLine(frame, b1, b2, outline_width, outline_color, 0);
	}
}


////////////////////////////////////////////////////////////////////////////////

void drawLine(RGBFrame& frame, Point p1, Point p2,  double width, Color c1, Color c2, double outline_width = 0, Color outline_color = Color::Black)
{
	Point a1, b1, a2, b2;

	if (p1.x() == p2.x())
	{
		a1 = Point(p1.x() - 0.5 * width, p1.y());
		b1 = Point(p1.x() + 0.5 * width, p1.y());

		a2 = Point(p2.x() - 0.5 * width, p2.y());
		b2 = Point(p2.x() + 0.5 * width, p2.y());
	}
	else if (p1.y() == p2.y())
	{
		a1 = Point(p1.x(), p1.y() - 0.5 * width);
		b1 = Point(p1.x(), p1.y() + 0.5 * width);

		a2 = Point(p2.x(), p2.y() - 0.5 * width);
		b2 = Point(p2.x(), p2.y() + 0.5 * width);
	}
	else
	{
		double slope = (p2.y() - p1.y()) / (p2.x() - p1.x());

		Point offset(slope, -1);
		offset *= 0.5 * width / norm(offset);
		
		a1 = p1 + offset;
		b1 = p1 - offset;

		a2 = p2 + offset;
		b2 = p2 - offset;
	}

	drawTriangle(frame, a1, b1, a2, c1, c1, c2);
	drawTriangle(frame, b1, a2, b2, c1, c2, c2);

	if (outline_width != 0)
	{
		Point offset = (a1 - p1);
		offset *= 0.5 * outline_width / norm(a1 -p1);

		Point m1 = a1 + offset;
		Point n1 = b1 - offset;

		drawLine(frame, m1, n1, outline_width, outline_color, 0);

		Point m2 = a2 + offset;
		Point n2 = b2 - offset;

		drawLine(frame, m2, n2, outline_width, outline_color, 0);

		drawLine(frame, a1, a2, outline_width, outline_color, 0);
		drawLine(frame, b1, b2, outline_width, outline_color, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////

void drawCircle(RGBFrame& frame, Point center, double radius, double width, 
	            Color color = Color::White, int segments = 64)
{
	Point a, b;

	a = center + Point(radius, 0);

	for (int i = 1; i < (segments + 1); i++)
	{
		double angle = ((double) i) / segments * 2 * M_PI;

		Point b = center + Point(cos(angle), sin(angle)) * radius;

		Point offset = (b - a) * (1 / norm(b - a));
		double alpha = 1.0 / segments * 2 * M_PI;
		offset *= 0.5 * width * tan(0.5 * alpha);
		
		//std::cout << offset.x() << " " << offset.y() << std::endl;

		drawLine(frame, a - offset, b + offset, width, color);

		a = b;
	}

}

////////////////////////////////////////////////////////////////////////////////

void drawDisk(RGBFrame& frame, Point center, double radius, Color color = Color::White)
{
	for (int y = -radius; y < radius; y++)
	{
		int xb = radius * sqrt(1.0 - (y / radius) * (y / radius));
		int xa = center.x() - xb;
		xb += center.x();
		drawFlatLine(frame, xa, xb, y + center.y(), color);
	}

}
