#pragma once

#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cmath>

#include "BFrame.h"

class GSFrame 
{
 private :

	int m_width;
	int m_height;
	double* m_data;

 public :
	// Construction ////////////////////////////////////////
	GSFrame()
	  :	m_width(0), m_height(0), m_data(NULL)
	{}

	GSFrame(int width, int height)
		: m_width(width), m_height(height)
	{
		m_data = new double[m_width * m_height];

		for (int i = 0; i < m_width * m_height; i++)
			m_data[i] = 0; 
	}

	GSFrame(GSFrame const &frame)
	  :	m_width(0), m_height(0), m_data(NULL)
	{  
		operator=(frame); // overloaded '=' is used
	}

	GSFrame& operator=(const GSFrame& frame) 
	{
		if (this != &frame)
		{
			m_width = frame.m_width;
			m_height = frame.m_height;

			delete[] m_data;
			m_data = NULL; 

			m_data = new double[frame.m_width * frame.m_height];
			std::copy(frame.m_data, frame.m_data + m_width * m_height, m_data);
		}

		return *this;
	}

	// TODO Recheck this
	GSFrame& operator=(BFrame& frame) 
	{
		GSFrame temp(frame);
		(*this) = temp;

		return (*this);
	}

	GSFrame(BFrame &frame) 
	{  
		m_width = frame.width();
		m_height = frame.height();

		m_data = new double[m_width * m_height];
		for (int i = 0; i < m_width * m_height; i++)
			m_data[i] = frame(i) / 256.0;
	}

	// Base functions //////////////////////////////////////
	int width() { return m_width; }
	
	int height() { return m_height; }

	int size() { return m_width * m_height; }

	double& operator()(int x, int y) {
		return m_data[m_width * y + x];
	}

	double& operator()(int i) {
		return m_data[i];
	}

	// Raw data manipulation ///////////////////////////////
	void load(double* data) {
		std::copy(data, data + m_width * m_height, m_data);
	}

	double* data() { return m_data; }

	// Advanced functions //////////////////////////////////

	GSFrame& realloc(int width, int height) {
		m_width = width;
		m_height = height;
		delete [] m_data;
		m_data =  new double[width * height];
		return *this;  
	}

	GSFrame& fill(double value) {
		std::fill(m_data, m_data + m_width * m_height, value);
		return *this;  
	}
/*
	GSFrame operator*(double scalar)
	{
		GSFrame result = (*this);
	
		for (int i = 0; i < m_width * m_height; i++)
			result(i) *= scalar;
	
		return result;
	}

	GSFrame& operator*=(double value) {
		for (int i = 0; i < size(); i++)
			m_data[i] *= value;
		return *this;  
	}
*/
	GSFrame& sqrt() {
		for (int i = 0; i < size(); i++)
			m_data[i] = std::sqrt(m_data[i]);
		return *this;  
	}

	GSFrame& crop(int anchor_x, int anchor_y, int width, int height) 
	{

		if (anchor_x >= 0 && (anchor_x +  width) <= m_width && 
			anchor_y >= 0 && (anchor_y + height) <= m_height) {
			GSFrame output(width, height);

			for (int x = 0; x < width; x++)	
				for (int y = 0; y < height; y++)
					output(x, y) = operator()(anchor_x + x, anchor_y + y);

			*this = output;		
		} else {
			std::cout << "# [error] Cropping out of the input boundaries !\n";
			std::exit(1);
		}
		return *this;  
	}

	// Destruction /////////////////////////////////////////
	~GSFrame() {
		if (m_data)
			delete [] m_data;
		m_data = NULL;
	}
};

/*
GSFrame operator*(double scalar, GSFrame f)
{
	return f * scalar;
}
*/


