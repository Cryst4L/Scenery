#pragma once

#include "Color.h"

typedef struct {
	double x, y;
} Vector2D;

typedef struct {
	double x, y, z;
} Vector3D;

////////////////////////////////////////////////////////////////////////////////

Vector2D add(const Vector2D a, const Vector2D b)
{
	Vector2D r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	return r;
} 

Vector3D add(const Vector3D a, const Vector3D b)
{
	Vector3D r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	return r;
} 


////////////////////////////////////////////////////////////////////////////////

double dot(const Vector2D a, const Vector2D b)
{
	return (a.x * b.x) + (a.y * b.y);
} 

double dot(const Vector3D a, const Vector3D b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
} 

////////////////////////////////////////////////////////////////////////////////

Vector2D add(const Vector2D a, const double lambda)
{
	Vector2D r;
	r.x = a.x + lambda;
	r.y = a.y + lambda;
	return r;
} 

Vector3D add(const Vector3D a, const double lambda)
{
	Vector3D r;
	r.x = a.x + lambda;
	r.y = a.y + lambda;
	r.z = a.z + lambda;
	return r;
} 

////////////////////////////////////////////////////////////////////////////////

Vector2D sub(const Vector2D a, const Vector2D b)
{
	Vector2D r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	return r;
} 

Vector3D sub(const Vector3D a, const Vector3D b)
{
	Vector3D r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	return r;
} 

////////////////////////////////////////////////////////////////////////////////

Vector2D multiply(const Vector2D a, const double lambda)
{
	Vector2D r;
	r.x = a.x * lambda;
	r.y = a.y * lambda;
	return r;
} 

Vector3D multiply(const Vector3D a, const double lambda)
{
	Vector3D r;
	r.x = a.x * lambda;
	r.y = a.y * lambda;
	r.z = a.z * lambda;
	return r;
} 

////////////////////////////////////////////////////////////////////////////////

double norm(const Vector2D v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

double norm(const Vector3D v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

////////////////////////////////////////////////////////////////////////////////

void normalize(Vector2D& v)
{
	double n = norm(v);
	v.x = v.x / n; 
	v.y = v.y / n;
}

void normalize(Vector3D& v)
{
	double n = norm(v);
	v.x = v.x / n; 
	v.y = v.y / n;
	v.z = v.z / n; 
}

////////////////////////////////////////////////////////////////////////////////

Vector3D cross(Vector3D& v1, Vector3D& v2)
{
	Vector3D r;
	r.x = v1.y * v2.z - v1.z * v2.y;
	r.y = v1.z * v2.x - v1.x * v2.z;
	r.z = v1.x * v2.y - v1.y * v2.x;
	return r;
}

////////////////////////////////////////////////////////////////////////////////

typedef struct {
	double data[3][3];
} Matrix3D;

Matrix3D multiply(const Matrix3D a, const Matrix3D b)
{
	Matrix3D r;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			r.data[i][j] = 0;
			for (int k = 0; k < 3; k++)
				r.data[i][j] += a.data[i][k] * b.data[k][j];
		}
	}
	return r;
}

Vector3D multiply(const Matrix3D m, const Vector3D v)
{
	Vector3D r;

	r.x = m.data[0][0] * v.x + m.data[0][1] * v.y + m.data[0][2] * v.z;
	r.y = m.data[1][0] * v.x + m.data[1][1] * v.y + m.data[1][2] * v.z;
	r.z = m.data[2][0] * v.x + m.data[2][1] * v.y + m.data[2][2] * v.z;

	return r;
}

////////////////////////////////////////////////////////////////////////////////


Matrix3D axisAngleMatrix(Vector3D axis, double angle)
{
	normalize(axis);

	Vector3D u = axis;

	double ux = u.x;
	double uy = u.y;
	double uz = u.z;

	Matrix3D m;

	double a = cos(angle);
 	double b = (1 - cos(angle));
	double c = sin(angle);	

	m.data[0][0] = a + ux * ux * b;
	m.data[0][1] = ux * uy * b - uz * c;
	m.data[0][2] = ux * uz * b + uy * c;

	m.data[1][0] = uy * ux * b + uz * c;
	m.data[1][1] = a + uy * uy * b;
	m.data[1][2] = uy * uz * b - ux * c;

	m.data[2][0] = uz * ux * b - uy * c;
	m.data[2][1] = uz * uy * b + ux * c;
	m.data[2][2] = a + uz * uz * b;

	return m;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3D eulerMatrixAlpha(double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	Matrix3D m = {{{   1,   0,   0},
	               {   0,   c,  -s},
	               {   0,   s,   c}}};
	return m;
}

Matrix3D eulerMatrixBeta(double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	Matrix3D m = {{{   c,   0,   s},
	               {   0,   1,   0},
	               {  -s,   0,   c}}};
	return m;
}

Matrix3D eulerMatrixGamma(double angle)
{
	double c = cos(angle);
	double s = sin(angle);

	Matrix3D m = {{{   c,  -s,   0},
	               {   s,   c,   0},
	               {   0,   0,   1}}};
	return m;
}



/*
class Matrix3d
{
	float m_data[3][3];

	public:

	float& operator()(int i, int j)
	{
		return m_data[i][j];
	}

	Vector3D operator*(Vector3D& v)
	{
		Vector3D r;
		for (int i = 0; i < 3; i++)
		{
			float acc = 0;

			acc += m_data[i][0] * v.x;
			acc += m_data[i][1] * v.y;
			acc += m_data[i][2] * v.z;

			if (i == 0) r.x = acc;
			if (i == 1) r.y = acc;
			if (i == 2) r.z = acc;
		}
		return r;
	}
	
	Matrix3d operator*(Matrix3d& m)
	{
		Matrix3d r;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				float acc = 0;
				for (int k = 0; k < 3; k++)
					acc += (*this)(i, k) * m(k, j);
				r(i, j) = acc;
			}		
		return r;
	}
};
*/


