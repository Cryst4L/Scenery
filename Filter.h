#pragma once

#include "Scenery/Utils.h"

std::vector <double> gaussianFilter(std::vector <double> input, double radius, double sigma = 0.1)
{
	// Build the Kernel //////////////////////////

	int k_size = 2 * radius + 1;
	double kernel[k_size];

	double sum = 0;
	for (int x = 0; x < k_size; x++) 
	{
		double dx = (x - radius) / (sigma * k_size);
		kernel[x] = std::exp(-0.5 * (dx * dx));
		sum += kernel[x];
	}

	for (int x = 0; x < k_size; x++)
		kernel[x] /= sum;

	// Extend the input //////////////////////////

	std::vector <double> extended(input.size() + 2 * radius);

	for (int i = 0; i < radius; i++)
		extended[i] = 2 * input[0] - input[radius - i];

	for (int i = 0; i < (int) input.size(); i++)
		extended[i + radius] = input[i];

	for (int i = 0; i < radius; i++)
		extended[input.size() + radius + i] = 2 * input.back() - input[input.size() - i - 2];	

	// Perform the convolution

	std::vector <double> output(input.size());

	for (int i = 0; i < (int) input.size(); i++)
	{
		double acc = 0;
		for (int offset = 0; offset < k_size; offset++)
			acc += extended[i + offset] * kernel[offset];
		output[i] = acc;
	}

	return output;
}

