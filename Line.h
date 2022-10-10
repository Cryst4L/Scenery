#pragma once

#include <cmath>
#include <iostream>
#include "Algebra.h"
#include "GSFrame.h"

void drawLine(Vector2D a, Vector2D b, GSFrame& screen)
{
	int x0 = a.x, y0 = a.y;
	int x1 = b.x, y1 = b.y;

	int dx =  std::abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;

	int dy = -std::abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;

	int err = dx + dy, e2;

	while (x0 != x1 || y0 != y1) {
		screen(x0, y0) = 1;
		e2 = 2 * err;

		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

