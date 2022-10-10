#pragma once

#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cmath>

#include "Color.h"
#include "GSFrame.h"

class RGBFrame 
{
 public :

	int m_width;
	int m_height;

	std::vector <Color> m_data;

 public :
	// Construction ////////////////////////////////////////
	RGBFrame()
	:	m_width(0), m_height(0), m_data(0)
	{}

	RGBFrame(int width, int height)
		: m_width(width), m_height(height), m_data(m_width * m_height) 
	{}

	RGBFrame& operator=(const RGBFrame &frame) {
		if (this != &frame) {
			m_width = frame.m_width;
			m_height = frame.m_height;
			m_data = frame.m_data;
		}
		return *this;
	}

	RGBFrame(const RGBFrame &frame)
	{  
		*this = frame; 
	}

	RGBFrame& operator=(GSFrame &frame) {
		
		m_width = frame.width();
		m_height = frame.height();

		m_data.clear();

		for (int i = 0; i < frame.size(); i++)
		{
			double value = frame(i);
			Color c(value, value, value);
			m_data.push_back(c);
		}
		
		return (*this);
	}

	RGBFrame(GSFrame &frame) {  
		*this = frame;
	}

	// Base functions //////////////////////////////////////

	int width() { return m_width; }
	
	int height() { return m_height; }

	int size() { return m_width * m_height; }

	Color& operator()(int x, int y) {
		return m_data[m_width * y + x];
	}

	Color& operator()(int i) {
		return m_data[i];
	}

	std::vector <Color>& data() 
	{
		return m_data; 
	}

	// Advanced functions //////////////////////////////////

	RGBFrame& realloc(int width, int height) {
		m_width = width;
		m_height = height;
		m_data.resize(width * height);
		return *this;  
	}

	RGBFrame& fill(Color color) {
		for (int i = 0; i < size(); i++)
			m_data[i] = color;
		return *this;  
	}
/*
	RGBFrame operator*(double scalar)
	{
		RGBFrame result = (*this);
	
		for (int i = 0; i < m_width * m_height; i++)
		{
			result(i).r() *= scalar;
			result(i).g() *= scalar;
			result(i).b() *= scalar;
		}
	
		return result;
	}
*/
	RGBFrame operator*=(double value) 
	{
		for (int i = 0; i < size(); i++)
		{
			m_data[i].r() *= value;
			m_data[i].g() *= value;
			m_data[i].b() *= value;
		}
		return *this;  
	}

	RGBFrame& sqrt() {
		for (int i = 0; i < size(); i++)
		{
			m_data[i].r() = std::sqrt(m_data[i].r());
			m_data[i].g() = std::sqrt(m_data[i].g());
			m_data[i].b() = std::sqrt(m_data[i].b());
		}
		return *this;  
	}

	// Destruction /////////////////////////////////////////
	~RGBFrame() {}
};
/*
RGBFrame operator*(double scalar, RGBFrame f)
{
	return f * scalar;
}
*/


