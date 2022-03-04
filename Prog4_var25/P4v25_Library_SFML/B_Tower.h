#pragma once

#include "Building.h"

namespace P4v25_SFML
{
	class Level;

	struct TStats
	{
		int rad;
		double dmg;
		double shootS;
		int upgradeCost;
		TStats() : rad(0), dmg(0), shootS(0), upgradeCost(0) {}
		TStats(const int &, const double &, const double  &, const int &);
	};

	class B_Tower :
		public Building
	{
	private:
		int lvl;
		TStats stats;
		std::string name;
		double timeW;
		double aimAngle; //DEGREES!!
		std::set<Enemy*>::iterator target;
	public:
		sf::RectangleShape RELOADbarBase;
		sf::RectangleShape RELOADbar;
		sf::Sprite topSprite; //don't want to add setters for unknown functions.
		sf::CircleShape rangeCircle;
		sf::RectangleShape shootLine;
		B_Tower() : Building(), lvl(0), stats(), name(""), timeW(0) {}
		B_Tower(const Pos &, const int &, const TStats &, const std::string &, const double &);
		B_Tower(const int &, const int &, const int &, const TStats &, const std::string &, const double &);
		void setStats(const TStats &, Level &); //need level to find tileWidth
		void setLvl(const int &nl) { lvl = nl; }
		int getLvl() const { return lvl; }
		double getTimeW() const { return timeW; }
		TStats getStats() const { return stats; }
		void addTW(const double &); //adding timeQ to time W
		bool isReady() const;
		double remainingTime() const;
		int aim(Level &);
		int shoot(Level &);
		friend std::ostream &operator <<(std::ostream &, const B_Tower &);
		virtual ~B_Tower() {}
		virtual B_Tower *clone() const;
		std::string getName() const{ return name; }
		//new
		void setRELOADbarSize();
		void setRangeCircleSize(Level &L);
		virtual void draw(sf::RenderWindow &);
		virtual void drawRange(sf::RenderWindow &);
		virtual std::string getInfo(const Level &L) const;
	};
}