#pragma once
#include "BFrame.h"

/* even=0, odd=1 */

void thinningStep(BFrame &base_map, bool odd)
{
    int width  = base_map.width();
	int height = base_map.height();

    BFrame marker_map(width, height);
	marker_map.fill(0);

    int p2, p3, p4, p5, p6, p7, p8, p9;

    int a, b, m1, m2;

    for (int x = 1; x < width-1; x++)
    {
        for (int y = 1; y < height-1; y++)
        {
            if (base_map(x,y) == 1)
            {
                p9 = base_map(x-1,y-1);  p8 = base_map( x ,y-1);  p7 = base_map(x+1,y-1);
                p2 = base_map(x-1, y );                           p6 = base_map(x+1, y );
                p3 = base_map(x-1,y+1);  p4 = base_map( x ,y+1);  p5 = base_map(x+1,y+1);

                a  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);

                b  = (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9);

                m1 = odd ? (p2 * p4 * p6) : (p2 * p4 * p8);
                m2 = odd ? (p4 * p6 * p8) : (p2 * p6 * p8);

                if (a == 1 && (b >= 2 && b <= 6) && m1 == 0 && m2 == 0)
                    marker_map(x,y) = 1;
            }
        }
    }

	for (int i = 0; i < base_map.size(); i++)
    	base_map(i) -= marker_map(i);
}


void skeletonize(BFrame &base_map)
{
	int count = base_map.size();
	int pre_count = count + 1;

	while (count != pre_count)
	{
        thinningStep(base_map, 0);
        thinningStep(base_map, 1);

		////////////////////////////////////////////////////

		pre_count = count;

		count = 0;
		for (int i = 0; i < base_map.size(); i++)
			count += base_map(i);

		std::cout << "here" << std::endl;
	}
}

