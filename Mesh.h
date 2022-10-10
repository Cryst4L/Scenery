#pragma once

#include <math.h>
#include "Algebra.h"
#include "Point.h"
#include "Color.h"
#include "Draw.h"
#include "Raster.h"

//typedef std::vector <Triangle> Mesh; 

class Mesh 
{
  private:
	std::vector <Triangle> m_triangles;

  public:

//	Mesh() {}

	void push(Triangle triangle)
	{
		m_triangles.push_back(triangle);
	}

	void append(Mesh& mesh)
	{
		for (int i = 0; i < (int) mesh.m_triangles.size(); i++)
			m_triangles.push_back(mesh.m_triangles[i]);
	}

	int size()
	{
		return m_triangles.size();
	}

	Triangle& triangles(int i)
	{
		return m_triangles[i];
	}

	void render(RGBFrame& frame, GSFrame& depth_map, bool isometric = false)
	{
		for (int i = 0; i < (int) m_triangles.size(); i++)
		{
			Vertice a = m_triangles[i].a;
			Vertice b = m_triangles[i].b;
			Vertice c = m_triangles[i].c;

			// Z projection

			if (!isometric)
			{	
				a.position.x /= a.position.z;
				a.position.y /= a.position.z;

				b.position.x /= b.position.z;
				b.position.y /= b.position.z;

				c.position.x /= c.position.z;
				c.position.y /= c.position.z;
			}			

			// Calibration

			//double aspect_ratio = frame.height() / frame.width();

			Vector3D offset = {1., 1., 1.};

			a.position = add(a.position, offset);
			b.position = add(b.position, offset);
			c.position = add(c.position, offset);

			a.position = multiply(a.position, 0.5 * frame.height());
			b.position = multiply(b.position, 0.5 * frame.height());
			c.position = multiply(c.position, 0.5 * frame.height());

			rasterTriangle(a, b, c, frame, depth_map);
		}
	}

	void move(Vector3D vector)
	{
		for (int i = 0; i < (int) m_triangles.size(); i++)
		{
			Vector3D a = m_triangles[i].a.position;
			Vector3D b = m_triangles[i].b.position;
			Vector3D c = m_triangles[i].c.position;

			m_triangles[i].a.position = add(a, vector);
			m_triangles[i].b.position = add(b, vector);
			m_triangles[i].c.position = add(c, vector);
		}
	}

	void rotate(Vector3D axis, double angle)
	{
		Matrix3D m = axisAngleMatrix(axis, angle);

		for (int i = 0; i < (int) m_triangles.size(); i++)
		{
			Vector3D a = m_triangles[i].a.position;
			Vector3D b = m_triangles[i].b.position;
			Vector3D c = m_triangles[i].c.position;

			m_triangles[i].a.position = multiply(m, a);
			m_triangles[i].b.position = multiply(m, b);
			m_triangles[i].c.position = multiply(m, c);
		}
	}

	void dilate(Vector3D ratio)
	{
		for (int i = 0; i < (int) m_triangles.size(); i++)
		{
			Vector3D a = m_triangles[i].a.position;
			Vector3D b = m_triangles[i].b.position;
			Vector3D c = m_triangles[i].c.position;

			a.x *= ratio.x;
			a.y *= ratio.y;
			a.z *= ratio.z;

			b.x *= ratio.x;
			b.y *= ratio.y;
			b.z *= ratio.z;

			c.x *= ratio.x;
			c.y *= ratio.y;
			c.z *= ratio.z;

			m_triangles[i].a.position = a;
			m_triangles[i].b.position = b;
			m_triangles[i].c.position = c;
		}
	}


};
