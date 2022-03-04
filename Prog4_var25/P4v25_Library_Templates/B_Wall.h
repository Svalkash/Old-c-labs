#pragma once

#include "Building.h"

namespace P4v25_T
{
	class B_Wall :
		public Building
	{
	private:
		std::string name;
		double maxHP, curHP;
	public:
		B_Wall() : Building(), name(""), maxHP(0), curHP(0) {}
		B_Wall(const Pos &, const double &, const double &, const std::string &); //except if negative hp
		B_Wall(const int &, const int &, const double &, const double &, const std::string &);
		double getHP() { return curHP; }
		std::string getName() const{ return name; }
		int hit(const double &); //returns 1 if dead
		void heal(const double &);
		friend std::ostream &operator <<(std::ostream &, const B_Wall &);
		virtual ~B_Wall() {}
		virtual B_Wall *clone() const;
	};
}