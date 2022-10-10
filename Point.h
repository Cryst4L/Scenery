#pragma once

class Point
{
  private :

	double m_x; 
	double m_y;

  public :

	Point()
	: m_x(0), m_y(0)
	{}

	Point(double x, double y)
	: m_x(x), m_y(y)
	{}

	////////////////////////////////////////////////////////

	double& x() { return m_x; }
	double& y() { return m_y; }

	////////////////////////////////////////////////////////

	Point operator*(Point& p)
	{
		Point result;

		result.m_x = m_x * p.m_x;
		result.m_y = m_y * p.m_y;

		return result;
	}

	Point operator*(double scalar)
	{
		Point result;

		result.m_x = m_x * scalar;
		result.m_y = m_y * scalar;

		return result;
	}

	Point& operator*=(double scalar)
	{
		(*this) = (*this) * scalar;

		return (*this);
	}

	////////////////////////////////////////////////////////

	Point operator+(Point p)
	{
		Point result;

		result.m_x = m_x + p.m_x;
		result.m_y = m_y + p.m_y;

		return result;
	}

	Point operator+=(Point p)
	{
		(*this) = (*this) + p;

		return (*this);
	}

	Point operator+(double scalar)
	{		
		Point result;

		result.m_x = m_x + scalar;
		result.m_y = m_y + scalar;

		return result;
	}

	Point& operator+=(double scalar)
	{
		(*this) = (*this) + scalar;

		return (*this);
	}

	Point operator-(Point p)
	{
		Point result;

		result.m_x = m_x - p.m_x;
		result.m_y = m_y - p.m_y;

		return result;
	}

	Point operator-=(Point p)
	{
		(*this) = (*this) - p;

		return (*this);
	}

	Point operator-(double scalar)
	{		
		Point result;

		result.m_x = m_x - scalar;
		result.m_y = m_y - scalar;

		return result;
	}

	Point& operator-=(double scalar)
	{
		(*this) = (*this) - scalar;

		return (*this);
	}

	////////////////////////////////////////////////////////

	void randomize()
	{
		m_x = (rand() % 1024) / (double) 1024;   
		m_y = (rand() % 1024) / (double) 1024;   
	}

};

////////////////////////////////////////////////////////////

Point operator*(double scalar, Point p)
{
	return p * scalar;
}

Point operator+(double scalar, Point p)
{
	return p + scalar;
}

Point operator-(double scalar, Point p)
{
	return -1.0 * (p - scalar);
}

////////////////////////////////////////////////////////////

Point floor(Point p)
{
	Point result(p);

	result.x() = std::floor(p.x());	
	result.y() = std::floor(p.y());

	return result;
}

double dot(Point a, Point b)
{
	return (a.x() * b.x()) + (a.y() * b.y());
}

double norm(Point p)
{
	return std::sqrt(dot(p, p));
}

double distance(Point a, Point b)
{
	double x_dist = (a.x() - b.x()) * (a.x() - b.x());
	double y_dist = (a.y() - b.y()) * (a.y() - b.y());

	return std::sqrt(x_dist + y_dist);
} 
