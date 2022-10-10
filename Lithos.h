#pragma once
/*
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

#include "Scenery/RGBFrame.h"
#include "Scenery/PPM.h"
#include "Scenery/Color.h"
#include "Scenery/Raster.h"
#include "Scenery/Mesh.h"
*/

Mesh createLithos(Color ca = Color::Red, Color cb = Color::Blue)
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
	c.color = cb;

	d.position.x =  0.5;
	d.position.y =  0.5;
	d.position.z =  0.5;
	d.color = cb;

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
	a.color = ca;

	b.position.x =  0.5;
	b.position.y = -0.5;
	b.position.z =  0.5;
	b.color = ca;

	c.position.x = -0.5;
	c.position.y = -0.5;
	c.position.z = -0.5;
	c.color = ca;

	d.position.x =  0.5;
	d.position.y = -0.5;
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
	a.color = ca;

	b.position.x = -0.5;
	b.position.y = -0.5;
	b.position.z = -0.5;
	b.color = ca;

	c.position.x = -0.5;
	c.position.y =  0.5;
	c.position.z =  0.5;
	c.color = cb;

	d.position.x = -0.5;
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

	// right ///////////////////////////////////////////////

	a.position.x =  0.5;
	a.position.y = -0.5;
	a.position.z =  0.5;
	a.color = ca;

	b.position.x =  0.5;
	b.position.y = -0.5;
	b.position.z = -0.5;
	b.color = ca;

	c.position.x =  0.5;
	c.position.y =  0.5;
	c.position.z =  0.5;
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

	return mesh;
}


