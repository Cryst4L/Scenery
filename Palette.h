#pragma once 

#include <map>
#include <string.h>

#include "Color.h"

class Palette {

  private:

	typedef struct {
		std::string name;
		Color color;
	} Shade;

	std::vector <Shade> m_shades;

  public:

	Palette()	
	: m_shades(0)
	{}
	

	Palette(std::string path)
	: m_shades(0)
	{
		load(path);
	}

	void load(std::string path)
	{
		std::ifstream palette_file(path.c_str());

		if (!palette_file) 
		{
			std::cerr << " Could not open '" << path << "'" << std::endl;
			return;
		}

		std::string line;

		while (std::getline(palette_file, line))
		{
			if (line.size() < 26)
				break;

			std::string name = line.substr(0, 15);

			double r = std::atoi(line.substr(15, 3).c_str());
			double g = std::atoi(line.substr(19, 3).c_str());
			double b = std::atoi(line.substr(23, 3).c_str());

			r /= 256.; g /= 256.; b /= 256.;

			Color color(r, g, b);

			Shade shade = {name, color};

			m_shades.push_back(shade);
	}

		palette_file.close();

	}

	void add(std::string name, Color color)
	{
		Shade shade = {name, color};
		m_shades.push_back(shade);
	}

	void add(Palette& palette)
	{
		std::vector <Shade> shades = palette.m_shades;
		m_shades.insert(m_shades.end(), shades.begin(), shades.end());
    }

	void erase(int index)
	{
		m_shades.erase(m_shades.begin() + index);			
	}

	Color color(int index)
	{
		return m_shades[index].color;
	}

	std::string name(int index)
	{
		return m_shades[index].name;
	}

	int size()
	{
		return m_shades.size();
	}


/*
	std::vector<Shade>& shades()
	{
		return m_shades;
	}
*/

};
