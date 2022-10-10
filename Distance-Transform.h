#pragma once

#include <vector>
#include "GSFrame.h"

static int bowl(int x, int i, int g_i)
{
	return (x - i) * (x - i) + g_i * g_i;
}

static int sep(int i, int u, int g_i, int g_u)
{
	double r = std::floor((u * u - i * i + g_u * g_u - g_i * g_i) / (2 * (u - i)));
	return static_cast <int> (r);
}

static void firstPass(GSFrame &input)
{
	int m = input.width();
	int n = input.height();

	GSFrame output(m, n);
	output.fill(0);

	int upper_bound = m + n;

	for (int x = 0; x < m; x++) {
		if (input(x, 0) == 0) {
			output(x, 0) = upper_bound;
		}

		// forward scan
		for (int y = 1; y < n; y++) {
			output(x, y) = input(x, y) ? 0 : (1 + output(x, y - 1));
		}

		// backward scan
		for (int y = n - 1; y > 0; y--) {
			if (output(x, y) < output(x, y - 1))
				output(x, y - 1) = 1 + output(x, y);
		}
	}

	input = output;
}

static void secondPass(GSFrame &input)
{
	int m = input.width();
	int n = input.height();

	GSFrame output(m, n);
	output.fill(0);

	int q = 0;
	std::vector <int> s(m, 0);
	std::vector <int> t(n, 0);

	for (int y = 0; y < n; y++) {
		// forward scan
		q = 0;
		s[0] = 0;
		t[0] = 0;

		for (int u = 1; u < m; u++) {
			while (q >= 0 && bowl(t[q], s[q], input(s[q], y)) > bowl(t[q], u, input(u, y)))
				q--;

			if (q < 0) {
				q = 0;
				s[0] = u;
			} else {
				int w = 1 + sep(s[q], u, input(s[q], y), input(u, y));

				if (w < m) {
					q++;
					s[q] = u;
					t[q] = w;
				}
			}
		}

		// backward scan
		for (int u = m; u > 0; u--) {
			output(u - 1, y) = bowl(u - 1, s[q], input(s[q], y));

			if (u - 1 == t[q])
				q--;
		}
	}

	input = output;
}

static void sqrt(GSFrame &input)
{
	for (int i = 0; i < input.size(); i++)
		input.data()[i] = std::sqrt(input.data()[i]);
}

void distanceTransform(GSFrame &input)
{
	firstPass(input);
	secondPass(input);
	sqrt(input);
}


