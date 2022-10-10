#pragma once

#include <iostream>
#include <cmath>

#include "Scenery/RGBFrame.h"
#include "Scenery/Color.h"
#include "Scenery/Raster.h"
#include "Scenery/Mesh.h"

////////////////////////////////////////////////////////////////////////////////

Mesh createCylinder(Vector3D v1, Vector3D v2, double radius = 1.0, Color color = Color::White, int nb_segments = 64)
{
	Mesh cylinder;

	const double delta_theta = 2 * M_PI / nb_segments;

	Vector3D offset = sub(v2, v1);

	double size = norm(offset);

	for (int n = 0; n < nb_segments; n++)
	{
		double y1 = -0.5 * size;
		double y2 =  0.5 * size;

		double x1 = cos(n * delta_theta);
		double x2 = cos((n + 1) * delta_theta);

		double z1 = sin(n * delta_theta);
		double z2 = sin((n + 1) * delta_theta);

		Vertice a = {{x1, y1, z1}, color}; 
		Vertice b = {{x2, y1, z2}, color}; 
		Vertice c = {{x2, y2, z2}, color}; 
		Vertice d = {{x1, y2, z1}, color}; 

		Triangle t1 = {a, b, c};
		Triangle t2 = {a, c, d};

		cylinder.push(t1);
		cylinder.push(t2);
	}

	Vector3D dilation = {radius, 1, radius};
	cylinder.dilate(dilation);

	////////////////////////////////////////////////////////

	Vector3D reference = {0, 1, 0};
	Vector3D axis = cross(reference, offset);
	double sign = (offset.y > 0) ? 1 : -1;
	double angle  = sign * asin(norm(axis) / norm(offset));

	if (norm(axis) > 0.001)
		cylinder.rotate(axis, angle);

	////////////////////////////////////////////////////////

	Vector3D shift = multiply(add(v1, v2), 0.5);
	cylinder.move(shift);

	//cylinder.move(v1);

	return cylinder;
}

////////////////////////////////////////////////////////////////////////////////

Mesh createSphere(Vector3D center, double radius = 1.0, Color color = Color::White) 
{
	Mesh sphere;

//	color = pastelColorSampler();

	int nb_segments = 64;

	double theta = 2 * M_PI / nb_segments;

	Mesh ribbon;

	for (int i = 0; i < nb_segments - 1; i++)
	{	
		double alpha = ((i / (double) nb_segments) - 0.5) * M_PI;
		double beta  = (((i + 1) / (double) nb_segments) - 0.5) * M_PI;

		Vector3D a = {sin( 0.5 * theta) * cos(alpha), sin(alpha), cos( 0.5 * theta) * cos(alpha)};
		Vector3D b = {sin(-0.5 * theta) * cos(alpha), sin(alpha), cos(-0.5 * theta) * cos(alpha)};

		Vector3D c = {sin(-0.5 * theta) * cos(beta), sin(beta), cos(-0.5 * theta) * cos(beta)};
		Vector3D d = {sin( 0.5 * theta) * cos(beta), sin(beta), cos( 0.5 * theta) * cos(beta)};

		Vertice va = {a, color};
		Vertice vb = {b, color};
		Vertice vc = {c, color};
		Vertice vd = {d, color};
		
		Triangle t1 = {va, vb, vc};
		Triangle t2 = {va, vc, vd};

		ribbon.push(t1);
		ribbon.push(t2);
	}

	for (int i = 0; i < nb_segments - 1; i++)
	{
		Vector3D axis = {0, 1, 0};
		ribbon.rotate(axis, theta);
		sphere.append(ribbon);
	}

	Vector3D dilation = {radius, radius, radius};
	sphere.dilate(dilation);

	sphere.move(center);

	return sphere;
}

////////////////////////////////////////////////////////////////////////////////

Mesh createCube(Color ca = Color::Red, Color cb = Color::Blue, Color cc = Color::Green)
{
	Mesh mesh;
	Vertice a, b, c, d;
	Triangle triangle;

	// front ///////////////////////////////////////////////

	a.position.x = -0.5;
	a.position.y = -0.5;
	a.position.z = -0.5;
	a.color = ca;

	b.position.x =  0.5;
	b.position.y = -0.5;
	b.position.z = -0.5;
	b.color = ca;

	c.position.x = -0.5;
	c.position.y =  0.5;
	c.position.z = -0.5;
	c.color = ca;

	d.position.x =  0.5;
	d.position.y =  0.5;
	d.position.z = -0.5;
	d.color = ca;

	triangle.a = a;
	triangle.b = b;
	triangle.c = c;

	mesh.push(triangle);

	triangle.a = b;
	triangle.b = c;
	triangle.c = d;

	mesh.push(triangle);

	// back ////////////////////////////////////////////////

	a.position.x = -0.5;
	a.position.y = -0.5;
	a.position.z =  0.5;
	a.color = ca;

	b.position.x =  0.5;
	b.position.y = -0.5;
	b.position.z =  0.5;
	b.color = ca;

	c.position.x = -0.5;
	c.position.y =  0.5;
	c.position.z =  0.5;
	c.color = ca;

	d.position.x =  0.5;
	d.position.y =  0.5;
	d.position.z =  0.5;
	d.color = ca;

	triangle.a = a;
	triangle.b = b;
	triangle.c = c;

	mesh.push(triangle);

	triangle.a = b;
	triangle.b = c;
	triangle.c = d;

	mesh.push(triangle);

	// top /////////////////////////////////////////////////

	a.position.x = -0.5;
	a.position.y = -0.5;
	a.position.z =  0.5;
	a.color = cb;

	b.position.x =  0.5;
	b.position.y = -0.5;
	b.position.z =  0.5;
	b.color = cb;

	c.position.x = -0.5;
	c.position.y = -0.5;
	c.position.z = -0.5;
	c.color = cb;

	d.position.x =  0.5;
	d.position.y = -0.5;
	d.position.z = -0.5;
	d.color = cb;

	triangle.a = a;
	triangle.b = b;
	triangle.c = c;

	mesh.push(triangle);

	triangle.a = b;
	triangle.b = c;
	triangle.c = d;

	mesh.push(triangle);

	// bottom

	a.position.x = -0.5;
	a.position.y =  0.5;
	a.position.z =  0.5;
	a.color = cb;

	b.position.x =  0.5;
	b.position.y =  0.5;
	b.position.z =  0.5;
	b.color = cb;

	c.position.x = -0.5;
	c.position.y =  0.5;
	c.position.z = -0.5;
	c.color = cb;

	d.position.x =  0.5;
	d.position.y =  0.5;
	d.position.z = -0.5;
	d.color = cb;

	triangle.a = a;
	triangle.b = b;
	triangle.c = c;

	mesh.push(triangle);

	triangle.a = b;
	triangle.b = c;
	triangle.c = d;

	mesh.push(triangle);

	// left ////////////////////////////////////////////////

	a.position.x = -0.5;
	a.position.y = -0.5;
	a.position.z =  0.5;
	a.color = cc;

	b.position.x = -0.5;
	b.position.y = -0.5;
	b.position.z = -0.5;
	b.color = cc;

	c.position.x = -0.5;
	c.position.y =  0.5;
	c.position.z =  0.5;
	c.color = cc;

	d.position.x = -0.5;
	d.position.y =  0.5;
	d.position.z = -0.5;
	d.color = cc;

	triangle.a = a;
	triangle.b = b;
	triangle.c = c;

	mesh.push(triangle);

	triangle.a = b;
	triangle.b = c;
	triangle.c = d;

	mesh.push(triangle);

	// right ///////////////////////////////////////////////

	a.position.x =  0.5;
	a.position.y = -0.5;
	a.position.z =  0.5;
	a.color = cc;

	b.position.x =  0.5;
	b.position.y = -0.5;
	b.position.z = -0.5;
	b.color = cc;

	c.position.x =  0.5;
	c.position.y =  0.5;
	c.position.z =  0.5;
	c.color = cc;

	d.position.x =  0.5;
	d.position.y =  0.5;
	d.position.z = -0.5;
	d.color = cc;

	triangle.a = a;
	triangle.b = b;
	triangle.c = c;

	mesh.push(triangle);

	triangle.a = b;
	triangle.b = c;
	triangle.c = d;

	mesh.push(triangle);

	return mesh;
}

////////////////////////////////////////////////////////////////////////////////

Mesh createCone(Vector3D origin, Color color = Color::White, int nb_segments = 64) 
{
	Mesh mesh;

	double alpha = 2 * M_PI / nb_segments;

	Vector3D o = {0, 0, 0};
	Vector3D m = {0, 1, 0};

	for (int i = 0; i < nb_segments; i++)
	{
		Vector3D a = {0.5 * cos(i * alpha), 0, 0.5 * sin(i * alpha)};
		Vector3D b = {0.5 * cos((i + 1) * alpha), 0, 0.5 * sin((i + 1) * alpha)};

		Vertice va = {a, color};
		Vertice vb = {b, color};
		Vertice vo = {o, color};
		Vertice vm = {m, color};

		Triangle t1 = {va, vb, vo};
		mesh.push(t1);

		Triangle t2 = {va, vb, vm};
		mesh.push(t2);
	} 

	mesh.move(origin);

	return mesh;
}

////////////////////////////////////////////////////////////////////////////////

Mesh create3DPath(std::vector <Vector3D>& path, Color color, double radius = 0.2, int nb_segments = 64)
{
	Mesh mesh;

	for (int i = 0; i < (int) path.size() - 2; i++)
	{
		Vector3D a = path[i];
		Vector3D b = path[i + 1];

		Mesh cylinder = createCylinder(a, b, radius, color, nb_segments);
		mesh.append(cylinder);

		// Jointure (sphere)

		//Mesh sphere = createSphere(b, radius, color);
		//mesh.append(sphere);

		// Jointure (cylinder)

		Vector3D c = path[i + 2];

		double d  = dot(sub(b, a), sub(c, b));
		double n1 = norm(sub(b, a));
		double n2 = norm(sub(c, b));

		double alpha = acos(d / (n1 * n2));
		double length = 2 * sin(0.5 * alpha) * radius;

		Vector3D ab = sub(b, a);
		Vector3D bc = sub(c, b);

		Vector3D offset = add(multiply(ab, 0.5 / norm(ab)), multiply(bc, 0.5 / norm(bc))); 				

		Vector3D e = sub(b, multiply(offset, 0.5 * length));
		Vector3D f = add(b, multiply(offset, 0.5 * length));

		Mesh jointure = createCylinder(e, f, radius, color);
		mesh.append(jointure);
	}

	return mesh;
} 
