#pragma once

#include "Common.h"

namespace P4v25_SFML
{
	class Level;
	class Enemy;

	class Building
	{
	protected:
		Pos p;
	public:
		Building() : p(0, 0) {}
		Building(const Pos &po) :p(po) {}
		Building(const int &x, const int &y) :p(x, y) {}
		Pos getPos() const { return p; }
		double dist(const Enemy &e) const;
		double dist(const Building &b) const;
		virtual ~Building() {}
		virtual Building *clone() const = 0;
		//
		virtual void draw(sf::RenderWindow &) = 0;
		virtual void drawRange(sf::RenderWindow &window) {}
		virtual std::string getInfo(const Level &L) const = 0;
	};

	struct Tile
	{
		/*
		01 - plains .
		02 - river ~
		03 - hills ^
		11 - wall #
		12 - tower 1-2-3
		13 - castle @
		21 - spawn *
		*/
		int type;
		Building *b;
		sf::Sprite sprite;
		Pos nextL, nextH, nextA;
		int distL, distH, distA;
		Tile(int t = 0) : type(t), b(nullptr), nextL(), nextH(), nextA(), distL(-1), distH(-1), distA(-1) {}
		//
		Tile(const Tile &);
		Tile(Tile &&);
		Tile &operator =(const Tile &);
		Tile &operator =(Tile &&);
		//
		~Tile() { delete b; } //Oh... I don't know how to do this right..
		//
		void draw(sf::RenderWindow &);
		void drawRange(sf::RenderWindow &window);
		virtual std::string getInfo(const Level &L) const;
	};
}