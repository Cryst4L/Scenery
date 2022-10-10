#pragma once
#include "BFrame.h"

void grow(BFrame &input, int iteration = 1, bool reversed = 0)
{
    int width  = input.width();
	int height = input.height();

    BFrame output(width, height);
	output.fill(0);

	for (int i = 0; i < iteration; i++)
	{
		int p0, p1, p2, p3, p4, p5, p6, p7, p8;

		for (int x = 1; x < (width - 1); x++)
			for (int y = 1; y < (height - 1); y++)
			{
		        p0 = input(x-1,y-1);  p1 = input( x ,y-1);  p2 = input(x+1,y-1);
		        p3 = input(x-1,y  );  p4 = input( x ,y  );  p5 = input(x+1,y  );
		        p6 = input(x-1,y+1);  p7 = input( x ,y+1);  p8 = input(x+1,y+1);

				if (p0 || p1 || p2 || p3 || p4 || p5 || p6 || p7 || p8)
					output(x, y) = 1;
			}
	}

	input = output;
}

void grow(GSFrame &input, int iteration = 1, bool reversed = 0)
{
    int width  = input.width();
	int height = input.height();

    BFrame output(width, height);
	output.fill(reversed);

	for (int i = 0; i < iteration; i++)
	{
		int p0, p1, p2, p3, p4, p5, p6, p7, p8;

		for (int x = 1; x < (width - 1); x++)
			for (int y = 1; y < (height - 1); y++)
			{
		        p0 = input(x-1,y-1);  p1 = input( x ,y-1);  p2 = input(x+1,y-1);
		        p3 = input(x-1,y  );  p4 = input( x ,y  );  p5 = input(x+1,y  );
		        p6 = input(x-1,y+1);  p7 = input( x ,y+1);  p8 = input(x+1,y+1);

				double value = (p0 * p1 * p2 * p3 * p4 * p5 * p6 * p7 * p8) 

				output(x, y) += acc;
			}
	}

	input = output;
}



