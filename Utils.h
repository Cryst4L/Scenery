#pragma once
#include <cmath>
#include "Algebra.h"
#include "Color.h"
#include "GSFrame.h"



int dice(int range) 
{ 
	return rand() % range; 
}

double uniform() 
{ 
	return ((rand() % (1 << 12)) / ((double) (1 << 12))); 
}

void swap(double& a, double& b)
{
	double c = a;
	a = b; b = c;
}

void pick_a_line(int size, Vector2D& a, Vector2D& b, 
	             int width, int height)
{
	a.x = rand() % width;
	a.y = rand() % height;
	
	double angle = 0.01 * 2 * (rand() % 314);

	b.x = a.x + std::cos(angle) * size;
	b.y = a.y + std::sin(angle) * size;	
}

int is_inside(Vector2D a, int width, int height)
{
	return (a.x > 0 && a.x < width &&
	        a.y > 0 && a.y < height);
}

void threshold(GSFrame& frame, float threshold)
{
	for (int i = 0; i < frame.size(); i++)
		frame(i) = (frame(i) > threshold) ? 1 : 0;
}

void low_pass(GSFrame& frame, float threshold)
{
	for (int i = 0; i < frame.size(); i++)
		frame(i) = (frame(i) < threshold) ? frame(i) : 0;
}

