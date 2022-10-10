#pragma once
#include "Scenery/GSFrame.h"
#include "Scenery/RGBFrame.h"
#include "Scenery/Point.h"
#include "Scenery/Raster.h"
#include "Scenery/Color.h"

static double fract(double x)
{
	if (x > 0)	return (x - std::floor(x));
	else return (x - std::floor(x) - 1);
}


void drawThinLine(GSFrame& frame, Point a, Point b)
{
	int x0 = a.x(); int y0 = a.y();
	int x1 = b.x(); int y1 = b.y();

	bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

	if (steep) 
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	double dx = (x1 - x0);
	double dy = (y1 - y0);

	double grad = dy / dx;
	if (dx == 0) grad = 1;

	int xa = x0;
	int xb = x1;
	double intersect = y0;

	if (steep)
	{
		for (int x = xa; x <= xb; x++)
		{
     	frame((int) intersect, x) = 1 - fract(intersect);
			frame(((int) intersect) + 1, x) = fract(intersect);
			intersect += grad;
		}
	}
	else
	{	
		for (int x = xa; x <= xb; x++)
		{
			frame(x, (int) intersect) = 1 - fract(intersect);
			frame(x, ((int) intersect) + 1) = fract(intersect);
			intersect += grad;
		}
	}
}

void drawThinLine(RGBFrame& frame, Point a, Point b, Color color)
{
	if (a.x() < 0 || a.x() > frame.width())
		return;

	if (b.x() < 0 || b.x() > frame.width())
		return;

	if (a.y() < 0 || a.y() > frame.height())
		return;

	if (b.y() < 0 || b.y() > frame.height())
		return;

	if (a.x() == b.x())
		return;

	if (a.y() == b.y())
		return;

	int x0 = round(a.x()); int y0 = round(a.y());
	int x1 = round(b.x()); int y1 = round(b.y());

	bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

	if (steep) 
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	double dx = (x1 - x0);
	double dy = (y1 - y0);

	double grad = dy / dx;
	if (dx == 0) grad = 1;

	int xa = x0;
	int xb = x1;
	double intersect = y0;

	Color base = Color::White;
	
	if (steep)
	{
		xa = std::min(std::max(xa, 0), frame.height());
		xb = std::min(std::max(xb, 0), frame.height());

		for (int x = xa; x <= xb; x++)
		{
			if ((x > 0) && (x < frame.height())) 
			{
				base = frame((int) intersect, x);
			 	frame((int) intersect, x) = RGBMix(base, color, 1 - fract(intersect));

				base = frame(((int) intersect) + 1, x);
				frame(((int) intersect) + 1, x) = RGBMix(base, color, fract(intersect));

				intersect += grad;
			}
		}
	}
	else
	{	
		xa = std::min(std::max(xa, 0), frame.width());
		xb = std::min(std::max(xb, 0), frame.width());

		for (int x = xa; x <= xb; x++)
		{
			if ((x > 0) && (x < frame.height()) && (intersect > 0) && (intersect < frame.width() - 1))
			{
				base = frame(x, (int) intersect);
				frame(x, (int) intersect) = RGBMix(base, color, 1 - fract(intersect));

				base = frame(x, ((int) intersect) + 1);
				frame(x, ((int) intersect) + 1) = RGBMix(base, color, fract(intersect));

				intersect += grad;
			}
		}
	}
}

void drawSmoothTriangle(RGBFrame& frame, Point a, Point b, Point c, Color color)
{
	if (a.y() > b.y())
		std::swap(a, b);

	if (a.y() > c.y())
		std::swap(a, c);

	if (b.y() > c.y())
		std::swap(b, c);

	a.y() = (int) floor(a.y());
	b.y() = (int) round(b.y());
	c.y() = (int) ceil( c.y());

	drawTriangle(frame, a, b, c, color);

	drawThinLine(frame, a, b, color);
	drawThinLine(frame, b, c, color);
	drawThinLine(frame, c, a, color);

}

void drawSmoothLine(RGBFrame& frame, Point p1, Point p2, double width, Color color)
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

	std::cout << "a1 = (" << a1.x() << "; " << a1.y() << ")" << std::endl;
	std::cout << "a2 = (" << a2.x() << "; " << a2.y() << ")" << std::endl;

	std::cout << "b1 = (" << b1.x() << "; " << b1.y() << ")" << std::endl;
	std::cout << "b2 = (" << b2.x() << "; " << b2.y() << ")" << std::endl;

	drawSmoothTriangle(frame, a1, b1, a2, color);
	drawSmoothTriangle(frame, b1, a2, b2, color);
}





