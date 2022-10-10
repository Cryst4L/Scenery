#pragma once

#include "RGBFrame.h"
#include "Color.h"

class Gradient
{
  private :

	bool m_hsv;

	std::vector <Color> m_colors;

  public :

	Gradient(bool mode = 0)
	: m_hsv(mode), m_colors(0) {}

	void push(Color color)
	{
		m_colors.push_back(color);
	}

	Color color(double value)
	{
		int size = m_colors.size();

		if (size == 0)
			return Color::Black;				

		if (size == 1)
			return m_colors[0];				

		if (value == 1)
			return m_colors.back();

		int index = floor(value * (size - 1));
		double ratio = value * (size - 1) - index;

		Color c;

		if (m_hsv)
			c = HSVMix(m_colors[index], m_colors[index + 1], ratio); 
		else
			c = RGBMix(m_colors[index], m_colors[index + 1], ratio); 

		return c;
	}
		

};



// RGBFrame RGBGradientMap(GSFrame& map, Gradient gradient)

// RGBFrame HSVGradientMap(GSFrame& map, Gradient gradient)


RGBFrame RGBGradientMap(GSFrame& map, Color a, Color b) 
{
	RGBFrame output(map.width(), map.height());
	for (int i = 0; i < map.size(); i++)
		output(i) = RGBMix(a, b, map(i));
	return output;
}

RGBFrame HSVGradientMap(GSFrame& map, Color a, Color b, bool reversed = 0) 
{
	RGBFrame output(map.width(), map.height());
	for (int i = 0; i < map.size(); i++)
		output(i) = HSVMix(a, b, map(i), reversed);
	return output;
}

