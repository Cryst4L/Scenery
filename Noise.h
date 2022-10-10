#pragma once

#include "Point.h"
#include "GSFrame.h"
#include "RGBFrame.h"

double fract(double input)
{
	return input - std::floor(input);
}

static double clamp(double input)
{
	return std::max(0.0, std::min(input, 1.0));
}

double hash(double input)
{
	return fract(sin((input + 15.37) * 153.54) * 334.151);
}


Point hash2D(Point input, double seed = 0.5)
{
	double h0 = hash(seed + 0) * 500;
	double h1 = hash(seed + 1) * 500;
	double h2 = hash(seed + 2) * 500;
	double h3 = hash(seed + 3) * 500;

	Point axis_x(h0, h1);
	Point axis_y(h2, h3); 
/*
	Point axis_x(127.1, 311.7);
	Point axis_y(269.5, 183.3); 
*/

	double x = dot(input, axis_x);
	double y = dot(input, axis_y);

	x = fract(sin(x) * 43758.5453123);
	y = fract(sin(y) * 43758.5453123);

	Point result(x, y);

	result = 2 * result - 1.0;

	// result *= (1 / norm(result));

	return result;
}

double SimplexNoise(Point input, double seed = 0.5)
{
	const double F = 0.366025404;
	const double G = 0.211324865;

	Point a, b, c;

	double bias;

	////////////////////////////////////////////////////////

	bias = (input.x() + input.y()) * F;

	Point cell = floor(input + bias);

	bias = (cell.x() + cell.y()) * G;

	a = input - (cell - bias);
	
	////////////////////////////////////////////////////////

	Point offset = (a.x() > a.y()) ? Point(1, 0) : Point(0, 1);

	////////////////////////////////////////////////////////

	b = a - offset + G;
	c = a - 1. + 2 * G;

	///////////////////////////////////////////////////////

	Point da = hash2D(cell, seed);
	Point db = hash2D(cell + offset, seed);
	Point dc = hash2D(cell + 1.0, seed);

	///////////////////////////////////////////////////////

	double ca = 0.5 - dot(a, a);
	ca = std::max(ca, 0.0);
	ca = ca * ca * ca * ca;
	ca *= dot(a, da);

	double cb = 0.5 - dot(b, b);
	cb = std::max(cb, 0.0);
	cb = cb * cb * cb * cb;
	cb *= dot(b, db);

	double cc = 0.5 - dot(c, c);
	cc = std::max(cc, 0.0);
	cc = cc * cc * cc * cc;
	cc *= dot(c, dc);

	///////////////////////////////////////////////////////

	double value = (ca + cb + cc) * 70.0;

	value = 0.5 * value + 0.5;

	//std::cout << value << std::endl;

	return value;
}

void SimplexNoise(GSFrame& frame, double frequency = 4.0, double seed = 0.5)
{
	int w = frame.width();
	int h = frame.height(); 

	double scale = frequency / std::min(w, h);

	for (int i = 0; i < frame.width(); i++)
		for (int j = 0; j < frame.height(); j++)
		{
			Point input = Point(i, j) * scale;
			frame(i, j) =  SimplexNoise(input, seed);
		}
}

void clouds(GSFrame& frame, double base_frequency = 8, int nb_octaves = 5, double seed = 0.5)
{
	int w = frame.width();
	int h = frame.height();

	frame.fill(0.0);

	int ratio = 1;

	GSFrame temp(w, h);

	for (int o = 0; o < nb_octaves; o++)
	{
		SimplexNoise(temp, base_frequency * ratio, seed);

		for (int i = 0; i < frame.size(); i++)
			frame(i) += (2 * temp(i) - 1) / ratio;

		ratio *= 2;
	}

	for (int i = 0; i < frame.size(); i++)
		frame(i) = clamp(0.5 * (frame(i) + 1));

}

