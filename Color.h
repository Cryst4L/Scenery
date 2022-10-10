#pragma once

#include <vector>
#include "Utils.h"
#include "Algebra.h"

static const double bt_709[3] = {0.21, 0.72, 0.07};

class Color
{
  private:

	double m_r, m_g, m_b;

  public:

	static const Color Black;
	static const Color Grey;
	static const Color White;
	static const Color Red;
	static const Color Blue;
	static const Color Green;
	static const Color Yellow;

	Color()
	 : m_r(0), m_g(0), m_b(0)
	{}

	Color(float r, float g, float b) 
	 : m_r(r), m_g(g), m_b(b)
	{}

	Color& operator=(const Color &color)
	{
		//std::cout << "op1 " << std::flush;
		if (this != &color) 
		{
			m_r = color.m_r;
			m_g = color.m_g;
			m_b = color.m_b;	
		}

		return *this;
	}
		
	Color(const Color &color)
	{  
		//std::cout << "op2 " << std::flush;
		*this  = color;	
	}

	Color operator+(Color a)
	{
		Color b = (*this);

		b.r() += a.r();
		b.g() += a.g();
		b.b() += a.b();

		return b;
	}

	Color operator*(Color a)
	{
		Color b = (*this);

		b.r() *= a.r();
		b.g() *= a.g();
		b.b() *= a.b();

		return b;
	}

	Color operator*(double lambda)
	{
		Color b = (*this);	

		b.r() *= lambda;
		b.g() *= lambda;
		b.b() *= lambda;

		return b;
	}

	int operator==(Color& lhs)
	{
		if (m_r == lhs.m_r && m_g == lhs.m_g && m_b == lhs.m_b)
			return 1;

		return 0;
	}

	Color invert()
	{
		m_r = 1 - m_r;
		m_g = 1 - m_g;
		m_b = 1 - m_b;

		return *this;		
	}

	double luminance(double gamma = 2.2) 
	{
		double l_r = bt_709[0] * std::pow(m_r, gamma);
		double l_g = bt_709[1] * std::pow(m_g, gamma);
		double l_b = bt_709[2] * std::pow(m_b, gamma);

		double sum = l_r + l_g + l_b;

		double luminance = std::pow(sum, 1.0 / gamma); 

		return luminance;
	}

	double hue()
	{
		double r = m_r;
		double g = m_g;
		double b = m_b;

		double min = std::min(r, std::min(g, b));
		double max = std::max(r, std::max(g, b));

		double delta = max - min;

		double epsilon = 1e-3;

		if (delta < epsilon) 
			return 0;

		double hue = 0;

		if ((max - r) < epsilon)
			hue = (g - b) / delta;
		else if ((max - g) < epsilon)
			hue = (b - r) / delta + 2.0;
		else
			hue = (r - g) / delta + 4.0;

		hue *= 60.0;

		if (hue < 0)
			hue += 360;

		if (hue > 360)
			hue -= 360;

		return hue;
	}

	double saturation()
	{
		double r = m_r;
		double g = m_g;
		double b = m_b;

		double min = std::min(r, std::min(g, b));
		double max = std::max(r, std::max(g, b));

		double s = (max - min) / max;

		return s;
	}

	double value()
	{
		return std::max(m_r, std::max(m_g, m_b));
	}

	double& r() { return m_r; }
	double& g() { return m_g; }
	double& b() { return m_b; }

	~Color() {}
};

double RGBDistance(Color a, Color b)
{
	double dr = (a.r() - b.r());
	double dg = (a.g() - b.g());
	double db = (a.b() - b.b());

	double sqd = dr * dr + dg * dg + db * db;

	return std::sqrt(sqd);
}

Color RGBMix(Color a, Color b, float ratio = 0.5)
{
	Color c;

	c.r() = (1 - ratio) * a.r() + ratio * b.r();
	c.g() = (1 - ratio) * a.g() + ratio * b.g();
	c.b() = (1 - ratio) * a.b() + ratio * b.b();

	return c;
}

Vector3D RGBtoHSV(Color color)
{
	Vector3D hsv;

	hsv.x = color.hue();
	hsv.y = color.saturation();
	hsv.z = color.value();

	return hsv;
}


Color HSVtoRGB(Vector3D hsv)
{
	double hue = hsv.x;
	double sat = hsv.y;
	double val = hsv.z;

	double r = 0;
	double g = 0;
	double b = 0;

	double chroma = sat * val;

	double offset = (hue / 60.) - 1;

	double i = 2 * round(offset / 2);

	offset = std::abs(offset - i);

	double x = chroma * (1. - offset);

	if (hue < 60)
	{
		r = chroma;
		g = x;
	}
	else if (hue < 120)
	{
		r = x;
		g = chroma;
	}
	else if (hue < 180)
	{
		g = chroma;
		b = x;
	}
	else if (hue < 240)
	{
		g = x;
		b = chroma;							
	}
	else if (hue < 300)
	{
		b = chroma;
		r = x;
	}
	else if (hue < 360)
	{
		b = x;
		r = chroma;
	}

	double m = val - chroma;

	r += m;
	g += m;
	b += m;

	return Color(r, g, b);
}

Color HSVMix(Color a, Color b, float ratio = 0.5, bool reversed = 0)
{
	Vector3D hsv_a = RGBtoHSV(a); // 270 89 90
	Vector3D hsv_b = RGBtoHSV(b); // 330 89 90

	double hue_1 = hsv_a.x;
	double hue_2 = hsv_b.x;

	if (hue_1 > hue_2)
	{
		double temp = hue_1;
		hue_1 = hue_2;
		hue_2 = temp;
		reversed = 1 - reversed;
	}

	double delta_1 = hue_2 - hue_1;
	double delta_2 = hue_1 + (360 - hue_2);

	if (delta_2 < delta_1)
	{
		double temp = hue_1;
		hue_1 = hue_2;
		hue_2 = 360 + temp;
		reversed = 1 - reversed;
	}

	if (reversed)
	{
		double temp = hue_1;
		hue_1 = hue_2;
		hue_2 = temp;
	}

	Vector3D hsv;

	hsv.x = (1 - ratio) * hue_1   + ratio * hue_2;
	hsv.y = (1 - ratio) * hsv_a.y + ratio * hsv_b.y;
	hsv.z = (1 - ratio) * hsv_a.z + ratio * hsv_b.z;

	if (hsv.x > 360)
		hsv.x -= 360;

	Color c = HSVtoRGB(hsv);

	return c;
}

Vector3D RGBtoXYZ(Color color)
{
	double r = std::pow(color.r(), 2.2) * 100;
	double g = std::pow(color.g(), 2.2) * 100;
	double b = std::pow(color.b(), 2.2) * 100;

	double x = r * 0.4124 + g * 0.3576 + b * 0.1805;
	double y = r * 0.2126 + g * 0.7152 + b * 0.0722;
	double z = r * 0.0193 + g * 0.1192 + b * 0.9505;

	Vector3D xyz = {x, y, z};

	return xyz;
}

Vector3D XYZtoLAB(Vector3D xyz)
{
	double ref_x =  95.05;
	double ref_y = 100.00;
	double ref_z = 108.89;

	double x = xyz.x / ref_x;
	double y = xyz.y / ref_y;
	double z = xyz.z / ref_z;
	
	double e = 1.0 / 3.0;

	x = std::pow(x, e);
	y = std::pow(y, e);
	z = std::pow(z, e);

	double l = (116 * y) - 16;
	double a = 500 * (x - y);
	double b = 200 * (y - z);

	Vector3D lab = {l, a, b};

	return lab;
}

Vector3D RGBtoLAB(Color color)
{
	Vector3D xyz = RGBtoXYZ(color);
	Vector3D lab = XYZtoLAB(xyz);
	return lab;
}

double LABDistance(Color a, Color b)
{
	Vector3D va = RGBtoLAB(a);
	Vector3D vb = RGBtoLAB(b);

	double dl = va.x - vb.x; // L
	double da = va.y - vb.y; // A
	double db = va.z - vb.z; // B

	double sqd = dl * dl + da * da + db * db;

	return std::sqrt(sqd);
}

Color GrayToRGB(double value)
{
	return Color(value, value, value);
}

double RGBtoGray(Color c)
{
	return (c.r() + c.g() + c.b()) / 3.;
}

const Color Color::Black( 0,  0,  0);
const Color Color::Grey( .5, .5, .5);
const Color Color::White( 1,  1,  1);
const Color Color::Red(   1,  0,  0);
const Color Color::Blue(  0,  0,  1);
const Color Color::Green( 0,  1,  0);
const Color Color::Yellow(1,  1,  0);



