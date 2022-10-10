#pragma once

#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cmath>

#include "BFrame.h"

typedef unsigned char BYTE;

class BFrame 
{
 private :

	int m_width;
	int m_height;
	BYTE* m_data;

 public :

	// Construction ////////////////////////////////////////

	BFrame()
	:	m_width(0), m_height(0), m_data(NULL)
	{}

	BFrame(int width, int height)
		: m_width(width), m_height(height) 
	{
		m_data = new BYTE[m_width * m_height];
		for (int i = 0; i < width * height; i++)
			m_data[i] = 0;
	}

	BFrame(const BFrame &frame)
	  :	m_width(0), m_height(0), m_data(NULL)
	{  
		operator=(frame); // overloaded '=' is used
	}

	BFrame& operator=(const BFrame& frame) 
	{
		if (this != &frame) 
		{
			m_width = frame.m_width;
			m_height = frame.m_height;

			delete [] m_data;
			m_data = NULL; 

			m_data = new BYTE[frame.m_width * frame.m_height];
			std::copy(frame.m_data, frame.m_data + m_width * m_height, m_data);
		}
		return *this;
	}

	// Base functions //////////////////////////////////////

	int width() { return m_width; }
	
	int height() { return m_height; }

	int size() { return m_width * m_height; }

	BYTE& operator()(int x, int y) {
		return m_data[m_width * y + x];
	}

	BYTE& operator()(int i) {
		return m_data[i];
	}

	// Raw data manipulation ///////////////////////////////

	void load(BYTE* data) 
	{
		std::copy(data, data + m_width * m_height, m_data);
	}

	BYTE* data() { return m_data; }

	// Advanced functions //////////////////////////////////

	BFrame& realloc(int width, int height) 
	{
		m_width = width;
		m_height = height;
		delete [] m_data;
		m_data =  new BYTE[width * height];
		return *this;  
	}

	BFrame& fill(BYTE value) 
	{
		std::fill(m_data, m_data + m_width * m_height, value);
		return *this;  
	}
/*
	BFrame operator*(double scalar)
	{
		BFrame result = (*this);
	
		for (int i = 0; i < m_width * m_height; i++)
			result(i) *= scalar;
	
		return result;
	}

	BFrame operator*=(BYTE value) 
	{
		for (int i = 0; i < size(); i++)
			m_data[i] *= value;
		return *this;  
	}
*/
	BFrame& crop(int anchor_x, int anchor_y, int width, int height) 
	{

		if (anchor_x >= 0 && (anchor_x +  width) <= m_width && 
			anchor_y >= 0 && (anchor_y + height) <= m_height) 
		{
			BFrame output(width, height);

			for (int x = 0; x < width; x++)	
				for (int y = 0; y < height; y++)
					output(x, y) = operator()(anchor_x + x, anchor_y + y);

			(*this) = output;		
		} 
		else 
		{
			std::cout << "# [error] Cropping out of the input boundaries !\n";
			std::exit(1);
		}
		return *this;  
	}

	// Destruction /////////////////////////////////////////

	~BFrame() 
	{
		delete [] m_data;
		m_data = NULL;
	}
};

/*
BFrame operator*(double scalar, BFrame& f)
{
	return f * scalar;
}
*/


