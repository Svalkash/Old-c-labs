#pragma once

#include "Common.h"

namespace P4v25_SFML
{
	class Level;
	class Building;

	struct EStats
	{
		double maxHP, curHP, regHP, damC, movS; //movS now will be double
		//
		EStats() : maxHP(0), curHP(0), regHP(0), damC(0), movS(0) {}
		EStats(const double &, const double &, const double &, const double &, const double &);
	};

	class Enemy
	{
	protected:
		EStats statsBase, stats;
		std::string name;
		ePos p;
		//new
		Pos goal;
	public:
		sf::RectangleShape HPbarBase;
		sf::RectangleShape HPbar;
		sf::Sprite sprite;
		//
		friend class HAura;
		//
		Enemy() : statsBase(), stats(), name(""), p(0, 0) {}
		//TODO: POSitION MUST BE CHECKED IN ANOTHER PLACE!!!!111!!11
		Enemy(const EStats &bes, const EStats &es, const std::string &str, const ePos &ep) : statsBase(bes), stats(es), name(str), p(ep), goal() {} //goal will be setted later
		Enemy(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep) : statsBase(bm, bc, br, bcoeff, bs), stats(m, c, r, coeff, s), name(str), p(ep), goal() {} //goal will be setted later
		Enemy(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y) : statsBase(bm, bc, br, bcoeff, bs), stats(m, c, r, coeff, s), name(str), p(x, y), goal() {} //goal will be setted later
		//without base
		Enemy(const EStats &es, const std::string &str, const ePos &ep) : statsBase(es), stats(es), name(str), p(ep), goal() {} //goal will be setted later
		Enemy(const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep) : statsBase(m, c, r, coeff, s), stats(m, c, r, coeff, s), name(str), p(ep), goal() {} //goal will be setted later
		Enemy(const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y) : statsBase(m, c, r, coeff, s), stats(m, c, r, coeff, s), name(str), p(x, y), goal() {} //goal will be setted later
		//
		ePos getPos() const { return p; }
		void setPos(const ePos &np) { p = np; } //position must be normal
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
		virtual double remainingTime(Level &) const = 0; //remaining until we can reach our goal. Depending on type
		//
		virtual ~Enemy() {}
		friend std::ostream &operator <<(std::ostream &, const Enemy &);
		virtual std::ostream &print(std::ostream &) const = 0;
		virtual Enemy *clone() const = 0;
		//new
		int direction();
		virtual Pos findGoal(Level &) = 0; //launched while loading, after releasing from the spawn OR when reached the previous goal. Also sets sprite texture (!!!!!!)
		void setHPbarSize();
		void setHPbarPosition(Level &);
		virtual void draw(sf::RenderWindow &);
		virtual void drawAura(sf::RenderWindow &) {}
		virtual std::string getInfo() const;
		virtual std::string getTypeInfo() const = 0;
		virtual std::string getAuraInfo() const { return std::string(""); }
	};
}