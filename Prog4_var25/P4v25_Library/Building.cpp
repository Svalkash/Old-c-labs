#include "stdafx.h"
#include "Building.h"
#include "Enemy.h"

namespace P4v25
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

	double Building::dist(const Enemy &e) const
	{
		return p.dist(e.getPos());
	}

	double Building::dist(const Building &b) const
	{
		return p.dist(b.p);
	}
}