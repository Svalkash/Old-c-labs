#pragma once

#include "Common.h"

namespace P4v25
{
	class Level;
	class Building;

	struct EStats
	{
		double maxHP, curHP, regHP, damC;
		int movS;
		//
		EStats() : maxHP(0), curHP(0), regHP(0), damC(0), movS(0) {}
		EStats(const double &, const double &, const double &, const double &, const int &);
	};

	class Enemy
	{
	protected:
		EStats statsBase, stats;
		std::string name;
		Pos p;
		double timeW;
	public:
		friend class HAura;
		//
		Enemy() : statsBase(), stats(), name(""), p(0, 0), timeW(0) {}
		//TODO: POSitION MUST BE CHECKED IN ANOTHER PLACE!!!!111!!11
		Enemy(const EStats &, const EStats &, const std::string &, const Pos &, const double &);
		Enemy(const double &, const double &, const double &, const double &, const int &, const double &, const double &, const double &, const double &, const int &, const std::string &, const Pos &, const double &);
		Enemy(const double &, const double &, const double &, const double &, const int &, const double &, const double &, const double &, const double &, const int &, const std::string &, const int &, const int &, const double &);
		//without base
		Enemy(const EStats &, const std::string &, const Pos &, const double &);
		Enemy(const double &, const double &, const double &, const double &, const int &, const std::string &, const Pos &, const double &);
		Enemy(const double &, const double &, const double &, const double &, const int &, const std::string &, const int &, const int &, const double &);
		//
		Pos getPos() const { return p; }
		void setPos(const Pos np) { p = np; } //position must be normal
		double getTimeW() const { return timeW; }
		double getHP() const { return stats.curHP; }
		std::string getName() const { return name; }
		//
		virtual int move(Level &) = 0; //returns 1 if the enemy dies
		virtual void globalAC(Level &); //move - AuraCheck
		int auraCheck(Level &); //returns auras number
		double dist(const Enemy &e) const;
		double dist(const Building &b) const;
		int hit(const double &);
		void regen(const double &); //we just need to know timeQ
		void addTW(const double &);
		bool isReady() const;
		double remainingTime() const;
		//
		virtual ~Enemy() {}
		friend std::ostream &operator <<(std::ostream &, const Enemy &);
		virtual std::ostream &print(std::ostream &) const = 0;
		virtual Enemy *clone() const = 0;
	};
}