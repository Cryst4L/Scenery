#pragma once

#include "Scenery/NormalMap.h"

GSFrame extract3DEdges(Mesh& mesh, GSFrame depth_map, int radius = 1, bool isometric = false)
{
	int width  = depth_map.width();
	int height = depth_map.height();

	NormalMap normal_map(mesh, depth_map, isometric);
	
	GSFrame edge_map(width, height);
	edge_map.fill(1);

	// Objects edges

	for (int x = radius; x < (width - radius); x++)
		for (int y = radius; y < (height - radius); y++)
		{
			edge_map(x, y) = (abs(dot(normal_map(x - radius, y), normal_map(x + radius, y))) *
				              abs(dot(normal_map(x, y - radius), normal_map(x, y + radius))));
		}

	// Objects separations

	for (int x = 0; x < (width - 1); x++)
		for (int y = 0; y < (height - 1); y++)
		{
			double offset = abs(depth_map(x, y) - depth_map(x + 1, y)) + 
	                        abs(depth_map(x, y) - depth_map(x, y + 1));

			if (offset > (0.02 * height)) // TODO fix this 0.01 -> 0.1
				edge_map(x, y) = 0;
		}


	return edge_map;
}
