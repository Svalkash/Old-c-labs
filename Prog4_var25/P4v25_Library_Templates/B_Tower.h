#pragma once

#include "Building.h"

namespace P4v25_T
{
	class Level;

	struct TStats
	{
		int rad;
		double dmg;
		int shootS;
		int upgradeCost;
		TStats() : rad(0), dmg(0), shootS(0), upgradeCost(0) {}
		TStats(const int &, const double &, const int &, const int &);
	};

	class B_Tower :
		public Building
	{
	private:
		int lvl;
		TStats stats;
		std::string name;
		double timeW;
	public:
		B_Tower() : Building(), lvl(0), stats(), name(""), timeW(0) {}
		B_Tower(const Pos &, const int &, const TStats &, const std::string &, const double &);
		B_Tower(const int &, const int &, const int &, const TStats &, const std::string &, const double &);
		void setStats(const TStats &nts) { stats = nts; } //or gain access to level
		void setLvl(const int &nl) { lvl = nl; }
		int getLvl() const { return lvl; }
		void addTW(const double &); //adding timeQ to time W
		bool isReady() const;
		double remainingTime() const;
		int shoot(Level &);
		friend std::ostream &operator <<(std::ostream &, const B_Tower &);
		virtual ~B_Tower() {}
		virtual B_Tower *clone() const;
		std::string getName() const{ return name; }
	};
}