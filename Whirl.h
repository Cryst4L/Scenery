#pragma once

#include "Scenery/RGBFrame.h"
#include "Scenery/PPM.h"
#include "Scenery/Color.h"
#include "Scenery/Raster.h"
#include "Scenery/Mesh.h"
#include "Scenery/NormalMap.h"
#include "Scenery/Cube.h"
#include "Scenery/Blur.h"
#include "Scenery/3DEdges.h"
#include "Scenery/Noise.h"


Point rotate(Point input, Point center, double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	input -= center;

	Point output;
	output.x() = input.x() * c - input.y() * s;
	output.y() = input.x() * s + input.y() * c;

	output += center;

	return output;
}

void whirl(RGBFrame& input, Point center, double radius, double strength)
{
	RGBFrame result = input;
	result.fill(Color::White);

	for (int x = 0; x < result.width(); x++)
		for (int y = 0; y < result.height(); y++)
		{
			// For each point of the input compute the average rotation center and amplitude
			double distance = norm(Point(x, y) - center);
			double angle = strength * exp(-distance / radius);

			Point origin(x, y);
			origin = rotate(origin, center, -angle);

			if ((origin.x() >= 0) && (origin.x() < input.width()) && 
			    (origin.y() >= 0) && (origin.y() < input.height()))
				result(x, y) = input(origin.x(), origin.y());	
			else
				result(x, y) = Color::Black;					
		}
	
	input = result;
}
