#pragma once

#include "Building.h"

namespace P4v25_T
{
	struct CStats
	{
		int givingS;
		int profit;
		double maxHP;
		double regHP;
		int upgradeCost;
		CStats() : givingS(0), profit(0), maxHP(0), regHP(0), upgradeCost(0) {}
		CStats(const int &, const int &, const double &, const double &, const int &);
	};

	class B_Castle :
		public Building
	{
	private:
		int lvl;
		CStats stats;
		std::string name;
		double curHP;
		double timeW;
	public:
		B_Castle() : Building(), lvl(0), stats(), name(""), curHP(0), timeW(0) {}
		B_Castle(const Pos &, const int &, const CStats &, const std::string &, const double &, const double &);
		B_Castle(const int &, const int &, const int &, const CStats &, const std::string &, const double &, const double &);
		double getHP() { return curHP; }
		void setStats(const CStats &); //ADD CURRENT HP
		void setLvl(const int &nl) { lvl = nl; }
		int getLvl() const { return lvl; }
		std::string getName() const{ return name; }
		int giveMoney(); //returning money. hehe
		int hit(const double &);
		void regen(const double &); //we just need to know timeQ
		void addTW(const double &); //adding timeQ to time W
		bool isReady() const;
		double remainingTime() const;
		friend std::ostream &operator <<(std::ostream &, const B_Castle &);
		virtual ~B_Castle() {}
		virtual B_Castle *clone() const;
	};
}