#pragma once

class NormalMap;

void flatNormalLine(Vector3D a, Vector3D b, Vector3D normal, 
	          NormalMap& normal_map, GSFrame& depth_map);

void rasterNormalTriangle(Vector3D a, Vector3D b, Vector3D c, 
	                      Vector3D normal, NormalMap& normal_map, 
	                      GSFrame& depth_map);

Vector3D computeNormalVector(Vector3D a, Vector3D b, Vector3D c);

class NormalMap 
{
 public :

	bool m_isometric;	

	int m_width;
	int m_height;

	std::vector <Vector3D> m_data;

 public :

	NormalMap()
	:	m_width(0), m_height(0), m_data(0)
	{}

	NormalMap(int width, int height)
		: m_width(width), m_height(height), m_data(m_width * m_height) 
	{}

	NormalMap& operator=(const NormalMap &frame) {
		if (this != &frame) {
			m_width = frame.m_width;
			m_height = frame.m_height;
			m_data = frame.m_data;
		}
		return *this;
	}

	NormalMap(const NormalMap &frame)
	{  
		*this = frame; 
	}

	int width() { return m_width; }
	
	int height() { return m_height; }

	int size() { return m_width * m_height; }

	Vector3D& operator()(int x, int y) {
		return m_data[m_width * y + x];
	}

	Vector3D& operator()(int i) {
		return m_data[i];
	}

	std::vector <Vector3D>& data() 
	{
		return m_data; 
	}

	NormalMap& fill(Vector3D vector) {
		for (int i = 0; i < size(); i++)
			m_data[i] = vector;
		return *this;  
	}

	NormalMap(Mesh mesh, GSFrame depth_map, bool isometric = false)
	: m_isometric(isometric),
	  m_width(depth_map.width()), 
	  m_height(depth_map.height()),
	  m_data(m_width * m_height)
	{
		Vector3D uz = {0, 0, 1};
		(*this).fill(uz);

		for (int i = 0; i < (int) mesh.size(); i++)
		{
			Triangle triangle = mesh.triangles(i);

			Vector3D a = triangle.a.position;
			Vector3D b = triangle.b.position;
			Vector3D c = triangle.c.position;

			////////////////////////////////////////////////////

			Vector3D normal = computeNormalVector(a, b, c);

			////////////////////////////////////////////////////

			// Z projection

			if (!isometric)
			{
				a.x /= a.z; a.y /= a.z;
				b.x /= b.z; b.y /= b.z;
				c.x /= c.z; c.y /= c.z;
			}

			// Calibration

			//double aspect_ratio = frame.height() / frame.width();

			Vector3D offset = {1., 1., 1.};

			a = add(a, offset);
			b = add(b, offset);
			c = add(c, offset);

			a = multiply(a, 0.5 * m_height);
			b = multiply(b, 0.5 * m_height);
			c = multiply(c, 0.5 * m_height);

			rasterNormalTriangle(a, b, c, normal, (*this), depth_map);
		}

	}
};

////////////////////////////////////////////////////////////////////////////////

void flatNormalLine(Vector3D a, Vector3D b, Vector3D normal, 
	          NormalMap& normal_map, GSFrame& depth_map)
{
	const double DELTA = 0.01;

	if (a.x > b.x)
		std::swap(a, b);

	int x1 = floor(a.x); 
	int x2 =  ceil(b.x);
	int y  = floor(a.y);

	double z1 = a.z; 
	double z2 = b.z;

	if (x1 == x2)
		return;

	if (y < 0 || y > (normal_map.height() - 1))
		return;

	if (x1 < 1) 
		x1 = 1;

	if (x2 > (normal_map.width() - 1))
		x2 = (normal_map.width() - 1);

	double dz = (z2 - z1) / (double) (x2 - x1);

	double z = z1 - dz;

	for(int x = x1; x < x2 ; x++)
	{
		z += dz;

		if (abs(z - depth_map(x, y)) < DELTA)
			normal_map(x, y) = normal;
	}

}

////////////////////////////////////////////////////////////////////////////////

void rasterNormalTriangle(Vector3D a, Vector3D b, Vector3D c, 
	                      Vector3D normal, NormalMap& normal_map,
	                      GSFrame& depth_map)
{

	if (a.y > b.y) std::swap(a, b);

	if (a.y > c.y) std::swap(a, c);

	if (b.y > c.y) std::swap(b, c);

	// TODO Optional
	a.y = (int) floor(a.y);
	b.y = (int) round(b.y);
	c.y = (int) ceil( c.y);

	double dx1 = (b.x - a.x) / (b.y - a.y);
	double dx2 = (c.x - a.x) / (c.y - a.y);
	double dx3 = (c.x - b.x) / (c.y - b.y);

	double dz1 = (b.z - a.z) / (b.y - a.y);
	double dz2 = (c.z - a.z) / (c.y - a.y);
	double dz3 = (c.z - b.z) / (c.y - b.y);

	Vector3D s = a, e = a;

	if (dx1 > dx2) {

		for (; s.y < b.y; 
			   s.y++, e.y++, 
			   s.x += dx2, e.x += dx1,
			   s.z += dz2, e.z += dz1)
		{
			flatNormalLine(s, e, normal, normal_map, depth_map);
		}

		e = b;

		for (; s.y < c.y; 
		       s.y++, e.y++, 
		       s.x += dx2, e.x += dx3,
		       s.z += dz2, e.z += dz3)
		{
			flatNormalLine(s, e, normal, normal_map, depth_map);
		}

	} else {
		

		for (; s.y < b.y; 
		       s.y++, e.y++, 
		       s.x += dx1, e.x += dx2,
		       s.z += dz1, e.z += dz2)
		{
			flatNormalLine(s, e, normal, normal_map, depth_map);
		}

		s = b;

		for (; s.y < c.y; 
		       s.y++, e.y++, 
		       s.x += dx3, e.x += dx2,
			   s.z += dz3, e.z += dz2)
		{
			flatNormalLine(s, e, normal, normal_map, depth_map);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

Vector3D computeNormalVector(Vector3D a, Vector3D b, Vector3D c)
{
	Vector3D v1 = sub(b, a);
	Vector3D v2 = sub(c, a);

	Vector3D normal;

	normal.x = v1.y * v2.z - v1.z * v2.y;
	normal.y = v1.z * v2.x - v1.x * v2.z;
	normal.z = v1.x * v2.y - v1.y * v2.x;

	normalize(normal);

	//TODO if normal.z < 0 ....

	if (normal.z < 0)
	{
		normal.x *= -1;
		normal.y *= -1; 
		normal.z *= -1; 
	} 

	return normal;
}

////////////////////////////////////////////////////////////////////////////////

NormalMap normalMap(Mesh mesh, GSFrame depth_map)
{
	int width  = depth_map.width();
	int height = depth_map.height();

	NormalMap normal_map(width, height);
	
	Vector3D uz = {0, 0, 1};
	normal_map.fill(uz);

	for (int i = 0; i < (int) mesh.size(); i++)
	{

		Triangle triangle = mesh.triangles(i);

		Vector3D a = triangle.a.position;
		Vector3D b = triangle.b.position;
		Vector3D c = triangle.c.position;

		////////////////////////////////////////////////////

		Vector3D normal = computeNormalVector(a, b, c);

		////////////////////////////////////////////////////

		// Z projection
/*
		a.x /= a.z; a.y /= a.z;

		b.x /= b.z; b.y /= b.z;

		c.x /= c.z; c.y /= c.z;
*/
		// Calibration

		//double aspect_ratio = frame.height() / frame.width();

		Vector3D offset = {1., 1., 1.};

		a = add(a, offset);
		b = add(b, offset);
		c = add(c, offset);

		a = multiply(a, 0.5 * height);
		b = multiply(b, 0.5 * height);
		c = multiply(c, 0.5 * height);

		rasterNormalTriangle(a, b, c, normal, normal_map, depth_map);
	}

	return normal_map;
}

RGBFrame normalMapToRGBFrame(NormalMap normal_map)
{
	int width  = normal_map.width();
	int height = normal_map.height();

	RGBFrame frame(width, height);

	for (int i = 0; i < frame.size(); i++)
	{
		Vector3D vector = normal_map(i);
		double r = 0.5 * (1 + vector.x); 
		double g = 0.5 * (1 + vector.y); 
		double b = 0.5 * (1 + vector.z);
		frame(i) = Color(r, g, b); 
	}

	return frame;	
}


