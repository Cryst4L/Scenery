#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream> 

#include "RGBFrame.h"

typedef unsigned char BYTE;

RGBFrame loadPPM(std::string path)
{
	std::ifstream file(path.c_str(), std::ifstream::ate | std::ios::binary);

	if (!file) {
		std::cout << "Could not open " << path << std::endl;
		return RGBFrame();
	}

	////////////////////////////////////////////////////////

	int size = file.tellg();
	file.seekg(0);

	////////////////////////////////////////////////////////

	std::vector <BYTE> data(size);
	file.read((char *) &data[0], size);

	////////////////////////////////////////////////////////

	char format[2];
	format[0] = data[0];
	format[1] = data[1];

	if (format[0] != 'P' && format[1] != '6') {
		std::cout << "Cannot return the data: " 
		          << "only P6 format is supported" << std::endl; 
		return RGBFrame();
	}
	
	////////////////////////////////////////////////////////

	std::vector <std::string> meta_data(3);
	int meta_data_index = 0;
	int i = 2;

	while (meta_data_index < 3) {

		// If we read a standard character /////////////////
		if (data[i] != 0x23) {

			if (data[i] != 0x20 && data[i] != 0x0a) {
				meta_data[meta_data_index] += data[i];
				i++;

			} else { 

				if (meta_data[0] != "")
					meta_data_index++;

				while (data[i] == 0x20 || data[i] == 0x0a)
					i++;
			}

		// If we read a comment character //////////////////
		} else {

			while (data[i] != 0x0a)
				i++;
			i++;
		}

	}

	////////////////////////////////////////////////////////

	int width, height, max_value;

	std::stringstream stream;

	stream.clear();
	stream << meta_data[0];
	stream >> width;

	stream.clear();
	stream << meta_data[1];
	stream >> height;

	stream.clear();
	stream << meta_data[2];
	stream >> max_value;
/*
	std::cout << "width : " << width 
	          << " | height : " << height
	          << " | max_value : " << max_value
			  << std::endl;
*/

	////////////////////////////////////////////////////////

	int nb_pixels = 3 * width * height;
	int header_size = size - nb_pixels;

	//Remove the header ////////////////////////////////////

	data.erase(data.begin(), data.begin() + header_size);

	// Convert the data to a RGBFrame //////////////////////

	RGBFrame frame(width, height);

	for (int i = 0; i < (width * height); i++)
	{
		Color color;

		color.r() = data[3 * i + 0] / 256.f;
		color.g() = data[3 * i + 1] / 256.f;
		color.b() = data[3 * i + 2] / 256.f;

		frame(i) = color;
	}

	return frame;
}


void savePPM(RGBFrame& frame, std::string filename, std::string comment = "")
{
	std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);

	if (!file) {
		std::cout << "Could not open the image file ..." << std::endl;
		return;
	}

	file << "P6\n";

	////////////////////////////////////////////////////////

	if (comment.size() != 0)
	{
		int size = comment.size();	
		if (comment[size - 1] != '\n')
		{
			comment += "\n";
			size++;
		}

		comment.insert(0, "#");

		int offset = 0;
		for (int c = 0 ; c < (size - 1); c++)
		{
			if (comment[c + offset] == '\n')
			{
				comment.insert(c + offset + 1, "#");
				offset++;
			}
		}

		//std::cout << comment << std::endl;

		file << comment;
	}

	////////////////////////////////////////////////////////
 
	file << frame.width() << " " << frame.height() << "\n255" << std::endl;

	std::vector <Color> colors = frame.data();
	
	//std::cout << frame.data().size() << std::endl; 

	char * buffer = new char[3 * frame.size()];
	
	for (int i = 0; i < frame.size(); i++)
	{
			//std::cout << i << std::endl;
			buffer[3 * i + 0] = 255 * colors[i].r();
			buffer[3 * i + 1] = 255 * colors[i].g();
			buffer[3 * i + 2] = 255 * colors[i].b();
	}

	file.write(buffer, 3 * frame.size());

	delete [] buffer; 
	file.close();
}

void savePPM(GSFrame& frame, std::string filename, std::string comment = "")
{
	RGBFrame rgb(frame);
	savePPM(rgb, filename, comment);
}
