#pragma once

#include "GSFrame.h"
#include "RGBFrame.h"



GSFrame sample2XalongX(GSFrame input)
{
	int out_width = input.width() / 2;

	GSFrame output(out_width, input.height());

	for (int y = 0; y < input.height(); y++)
		for (int x = 0; x < out_width; x++)
			output(x, y) = 0.5 * (input(2 * x, y) + input(2 * x + 1, y));

	return output;
}


GSFrame sample2XalongY(GSFrame input)
{
	int out_height = input.height() / 2;

	GSFrame output(input.width(), out_height);

	for (int x = 0; x < input.width(); x++)
		for (int y = 0; y < out_height; y++)
			output(x, y) = 0.5 * (input(x, 2 * y), input(x + 1, 2 * y));

	return output;
}


GSFrame sample2X(GSFrame input)
{	
	GSFrame output = input;

	output = sample2XalongX(output);
	output = sample2XalongY(output);

	return output;
}

////////////////////////////////////////////////////////////////////////////////


RGBFrame sample2XalongX(RGBFrame input)
{
	int out_width = input.width() / 2;

	RGBFrame output(out_width, input.height());

	for (int y = 0; y < input.height(); y++)
		for (int x = 0; x < out_width; x++)
			output(x, y) = RGBMix(input(2 * x, y), input(2 * x + 1, y));

	return output;
}


RGBFrame sample2XalongY(RGBFrame input)
{
	int out_height = input.height() / 2;

	RGBFrame output(input.width(), out_height);

	for (int x = 0; x < input.width(); x++)
		for (int y = 0; y < out_height; y++)
			output(x, y) = RGBMix(input(x, 2 * y), input(x + 1, 2 * y));

	return output;
}

RGBFrame sample2X(RGBFrame input)
{	
	RGBFrame output = input;

	output = sample2XalongX(output);
	output = sample2XalongY(output);

	return output;
}

