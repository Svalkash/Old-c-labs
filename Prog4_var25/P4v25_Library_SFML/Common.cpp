#include "stdafx.h"
#include "Common.h"

using namespace std;

namespace P4v25_SFML
{
	Pos::Pos(const ePos &ep) : x(std::round(ep.x)), y(std::round(ep.y)) {}

	double Pos::dist(const ePos &p) const
	{
		return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y));
	}

	ePos::ePos(const Pos &p) :x(p.x), y(p.y) {}

	double ePos::dist(const Pos &p) const
	{
		return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y));
	}

	int sign(const float &f)
	{
		if (f > 0)
			return 1;
		if (f < 0)
			return -1;
		//f == 0
		return 0;
	}

	string getfolder(const std::string &address)
	{
		return address.substr(0, address.find_last_of('/') + 1);
	}

	sf::Texture *cloneTex(const sf::Texture *tex)
	{
		return new sf::Texture(*tex);
	}
}