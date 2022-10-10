#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>	

typedef struct {
	int id;
	int size;
	double sum_x;
	double sum_y;
	double sum_z;	
} ClusterData;

static bool isGreater(ClusterData a, ClusterData b) 
{ 
	return (a.size > b.size); 
}

std::vector <Color> colorClustering(std::vector <Color> colors, int nb_centroids = 8, int nb_epoch = 8, bool verbose = 0)
{
	// Init the data vertices //////////////////////////////////////////

	double max_lightness = 0;

	for (int i = 0; i < (int) colors.size(); i++)
	{
		Color c = colors[i];

		double lightness = (c.r() + c.g() + c.b()) / 3.;

		if (lightness > max_lightness)
			max_lightness = lightness;
	}

	double threshold = 0.98 * max_lightness;

	std::vector <Vertice> vertices;

	for (int i = 0; i < (int) colors.size(); i++)
	{
		Color c = colors[i];
		Vertice v(c.r(), c.g(), c.b());

		double lightness = (c.r() + c.g() + c.b()) / 3.;

		if (lightness < threshold)
			vertices.push_back(v);
	}

	//std::cout << "data size : " << data.size() << " vertices size : " << vertices.size() << std::endl;


	// Init the centroids ////////////////////////////////////////////

	std::vector <Vertice> centroids(nb_centroids);

	for (int i = 0; i < nb_centroids; i++)
	{
		centroids[i].x() = (rand() % 100) / (float) 100;   
		centroids[i].y() = (rand() % 100) / (float) 100;   
		centroids[i].z() = (rand() % 100) / (float) 100;   
	}

	// Make a container for the cluster data /////////////////////////

	std::vector <ClusterData> cluster_data = std::vector <ClusterData> (nb_centroids);

	for (int c = 0; c < nb_centroids; c++)
		cluster_data[c].id = c;

	// Start of the loop /////////////////////////////////////////////

	for (int e = 0; e < nb_epoch; e++)
	{

		// 1. Assign to each vertice its best centroid /////////////////
	
		for (int c = 0; c < nb_centroids; c++)
		{
			int cluster_id = c; 
			Vertice centroid = centroids[c];

			for (int v = 0; v < (int) vertices.size(); v++) 
			{
				Vertice vertice = vertices[v];

				double dist = squaredDistance(vertice, centroid);

				if (dist < vertice.minDistance())
				{
					vertices[v].clusterId() = cluster_id;
					vertices[v].minDistance() = dist;
				}			
			}
		}

		// 2. Reinit the cluster's data /////////////////////////////

		cluster_data = std::vector <ClusterData> (nb_centroids);

		for (int c = 0; c < nb_centroids; c++)
			cluster_data[c].id = c;

		// 2. Fill up the clusters with their assigned vertices ////////

		for (int v = 0; v < (int) vertices.size(); v++)
		{
			Vertice vertice = vertices[v];

			int cluster_id = vertice.clusterId();
		
			cluster_data[cluster_id].size++;
			cluster_data[cluster_id].sum_x += vertice.x();
			cluster_data[cluster_id].sum_y += vertice.y();
			cluster_data[cluster_id].sum_z += vertice.z();
		}

		// 3. Compute the new centroids based on the clusters ////////

		for (int c = 0; c < nb_centroids; c++)
		{
			int size = cluster_data[c].size;

			centroids[c].x() = cluster_data[c].sum_x / size;
			centroids[c].y() = cluster_data[c].sum_y / size;
			centroids[c].z() = cluster_data[c].sum_z / size;
		}

		// 4. Reinit the vertices distance /////////////////////////////

		for (int v = 0; p < (int) vertices.size(); p++)
			vertices[v].minDistance() = __DBL_MAX__;
	}

	// Sort the centroids ////////////////////////////////////////////

	std::sort (cluster_data.begin(), cluster_data.end(), isGreater);

	// Convert the centroids into a palette //////////////////////////

	std::vector <Color> palette;

	for (int i = 0; i < nb_centroids; i++) 
	{
		Vertice v = centroids[cluster_data[i].id];
		Color c(v.x(), v.y(), v.z());
		palette.push_back(c);
	}

	// Remove empty clusters /////////////////////////////////////////

	int idx = nb_centroids - 1;
	while (cluster_data[idx].size == 0) {
		palette.pop_back();
		idx--;
	}	

	if (verbose) {
	for (int i = 0; i < (int) palette.size(); i++)
		std::cout << " id = " << cluster_data[i].id 
		          << " | size = " << cluster_data[i].size 
		          << std::endl; 
	}

	return palette;
}


/*
	RGBFrame frame = loadPPM("../test.ppm");

	std::vector <Color> data = frame.data();

	std::vector <Vertice> vertices(data.size());

	for (int i = 0; i < (int) data.size(); i++)
	{
		vertices[i].x() = data[i].r(); 
		vertices[i].y() = data[i].g(); 
		vertices[i].z() = data[i].b(); 
	}
*/
