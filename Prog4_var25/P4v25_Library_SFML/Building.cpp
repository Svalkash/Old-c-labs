#include "stdafx.h"
#include "Building.h"
#include "Enemy.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{
	Tile::Tile(const Tile &t) :type(t.type), nextL(t.nextL), nextH(t.nextH), nextA(t.nextA), distL(t.distL), distH(t.distH), distA(t.distA)
	{
		if (t.b)
			b = t.b->clone();
		else
			b = nullptr;
	}

	Tile::Tile(Tile &&t) : type(t.type), nextL(t.nextL), nextH(t.nextH), nextA(t.nextA), distL(t.distL), distH(t.distH), distA(t.distA)
	{
		delete b;
		b = t.b;
		t.b = nullptr;
	}

	Tile &Tile::operator =(const Tile &t)
	{
		//firstly dynamic
		Building *nb;
		if (t.b)
			nb = t.b->clone();
		else
			nb = nullptr;
		//if alright
		delete b;
		b = nb;
		type = t.type;
		nextL = t.nextL;
		nextH = t.nextH;
		nextA = t.nextA;
		distL = t.distL;
		distH = t.distH;
		distA = t.distA;
		return *this;
	}

	Tile &Tile::operator =(Tile &&t)
	{
		delete b;
		b = t.b;
		t.b = nullptr;
		type = t.type;
		nextL = t.nextL;
		nextH = t.nextH;
		nextA = t.nextA;
		distL = t.distL;
		distH = t.distH;
		distA = t.distA;
		return *this;
	}

	void Tile::draw(sf::RenderWindow &window)
	{
		window.draw(sprite);
		if (b)
			b->draw(window);
	}

	void Tile::drawRange(sf::RenderWindow &window)
	{
		if (b)
			b->drawRange(window);
	}

	double Building::dist(const Enemy &e) const
	{
		return p.dist(e.getPos());
	}

	double Building::dist(const Building &b) const
	{
		return p.dist(b.p);
	}

	std::string Tile::getInfo(const Level &L) const
	{
		stringstream s;
		s << setprecision(PRINT_PRECISION) << fixed;
		/*
		01 - plains .
		02 - river ~
		03 - hills ^
		11 - wall #
		12 - tower 1-2-3
		13 - castle @
		21 - spawn *
		*/
		s << "Type: ";
		switch (type)
		{
		case 01:
			s << "Plains";
			break;
		case 02:
			s << "Water";
			break;
		case 03:
			s << "Hills";
			break;
		case 11:
			s << "Wall";
			break;
		case 12:
			s << "Tower";
			break;
		case 13:
			s << "Castle";
			break;
		case 21:
			s << "Spawn";
			break;
		}
		if (b)
			s << b->getInfo(L);
		else if (type == 01)
		{
			s << endl << "TOWER COST: " << L.getTowerBuildCost();
			s << endl << "WALL COST: " << L.getWallBuildCost();
		}
		return s.str();
	}
}