#pragma once

static double angleFromCorner(Point a, Point b, Point c)
{
	Point v1 = b - a;
	Point v2 = c - b;

	// dot product
	double d = v1.x() * v2.x() + v1.y() * v2.y(); 	

	// norms
	double n1 = sqrt(v1.x() * v1.x() + v1.y() * v1.y());
	double n2 = sqrt(v2.x() * v2.x() + v2.y() * v2.y());

	// closed form
	double cos_alpha = d / (n1 * n2);

	if (abs(cos_alpha - 1) < 1e-5)
		return 0;

	return acos(cos_alpha);
}

void drawPath(RGBFrame& frame, std::vector <Point> path, Color ca, Color cb, double thickness, double frequency, double phase, bool hsv_mix = 0)
{
	frequency *= 2.0 * M_PI / path.size();

	////////////////////////////////////////////////////////

	Point init = path[0] - (path[1] - path[0]); 
	path.insert(path.begin(), init);	

	////////////////////////////////////////////////////////

	Point last = path.back() + (path.back() - path[path.size() - 2]);
	path.push_back(last);

	////////////////////////////////////////////////////////

	for (int i = 1; i < (int) path.size() - 2; i++)
	{
		Point a = path[i - 1];
		Point b = path[i];
		Point c = path[i + 1];
		Point d = path[i + 2];

		double color_ratio = 0.5 * (1 - cos(frequency * i + phase));

		Color color;
		if (hsv_mix)
			color = HSVMix(ca, cb, color_ratio); 
		else
			color = RGBMix(ca, cb, color_ratio); 

		double theta;

		Point delta_a; 
		theta = angleFromCorner(a, b ,c);
		delta_a = (c - b) * (1 / norm(c - b));
		delta_a *= 0.5 * thickness * tan(0.5 * theta);

		Point delta_b;
		theta = angleFromCorner(b, c ,d);
		delta_b = (c - b) * (1 / norm(c - b));
		delta_b *= 0.5 * thickness * tan(0.5 * theta);

		drawLine(frame, b - delta_a, c + delta_b, thickness, color);
	}

	////////////////////////////////////////////////////////
}

void drawPath(RGBFrame& frame, std::vector <Point> path, Color color, double thickness)
{
	drawPath(frame, path, color, color, thickness, 0, 0);
}

