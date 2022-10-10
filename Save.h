#pragma once

#include <fstream>
#include <iostream>
#include "GSFrame.h"
#include "RGBFrame.h"

void save(GSFrame& frame, const char * filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (!file) {
		std::cout << "Could not open the image file ..." << std::endl;
		return;
	}

	file << "P6\n" << frame.width() << " " << frame.height() << "\n255" << std::endl;

	char buffer[3 * frame.size()];

	for (int i = 0; i < frame.size(); i++)
		for (int c = 0; c < 3; c++)
			buffer[3 * i + c] = (unsigned char) (255 * frame.data()[i]);

	file.write(buffer, 3 * frame.size());
	file.close();
}


void save(RGBFrame& frame, const char * filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	if (!file) {
		std::cout << "Could not open the image file ..." << std::endl;
		return;
	}

	file << "P6\n" << frame.width() << " " << frame.height() << "\n255" << std::endl;

	std::vector <Color> colors = frame.data();
	
	//std::cout << frame.data().size() << std::endl; 

	char * buffer = new char[3 * frame.size()];

	/*std::cout << frame.size() << std::endl;*/
	
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
