#pragma once

#include "GSFrame.h"

void blur(GSFrame& input, int radius, double sigma = 0.1) 
{
	double sum = 0;
	int k_size = 2 * radius + 1;
	
	// build the 1D filter
	double kernel[k_size];
	for (int x = 0; x < k_size; x++) {
		double dx = (x - radius) / (sigma * k_size);
		kernel[x] = std::exp(-0.5 * (dx * dx));
		sum += kernel[x];
	}
	
	for (int x = 0; x < k_size; x++)
	{
		kernel[x] /= sum;
//		std::cout << kernel[x] << std::endl;
	}
		
	// extend the input ///////////////////////////////////////////////////
	int x_w = input.width()  + k_size - 1;
	int x_h = input.height() + k_size - 1;
	
	GSFrame x_buffer(x_w, x_h);	
	for (int x = 0; x < x_w; x++) {
		for (int y = 0; y < x_h; y++) {
			bool v_margin = x < radius || x > (x_w - radius - 1);
			bool h_margin = y < radius || y > (x_h - radius - 1);
						
			if (v_margin || h_margin) {
				x_buffer(x, y) = 0.5; // 127
			} else {
				double value = input(x - radius, y - radius);
				x_buffer(x, y) = value;
			}
		}
	}
			
	// convolve it with the separated kernel //////////////////////////////

	GSFrame h_buffer(input.width(), x_h);
	for (int x = 0; x < h_buffer.width(); x++) {
		for (int y = 0; y < h_buffer.height(); y++) {
	
			double slice[k_size];
			for (int dx = 0; dx < k_size; dx++)
			{
				slice[dx] = x_buffer(x + dx, y);
			}
					
			double sum = 0;
			for (int dx = 0; dx < k_size; dx++)
				sum += slice[dx] * kernel[dx];
				
			h_buffer(x, y) = sum;

		}
	}
	
	GSFrame output(input.width(), input.height());
	for (int x = 0; x < output.width(); x++) {
		for (int y = 0; y < output.height(); y++) {

			double slice[k_size];
			for (int dy = 0; dy < k_size; dy++)
				slice[dy] = h_buffer(x, y + dy);
					
			double sum = 0;
			for (int dy = 0; dy < k_size; dy++)
				sum += slice[dy] * kernel[dy];
				
			output(x, y) = sum;
		}
	}

	input = output;
}

