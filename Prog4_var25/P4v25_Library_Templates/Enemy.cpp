#include "stdafx.h"
#include "Enemy.h"
#include "Building.h"
#include "Hero.h"
#include "Level.h"

using namespace std;

namespace P4v25_T
{

	EStats::EStats(const double &m, const double &c, const double &r, const double &coeff, const int &s) : regHP(r) //regen may be negative
	{
		if (m <= 0)
			throw std::exception("EStats:illegal_maximum_HP");
		if (c <= 0)
			throw std::exception("EStats:illegal_current_HP");
		if (c > m)
			throw exception("Enemy:current_HP>maximum_HP");
		if (coeff <= 0)
			throw std::exception("EStats:illegal_damage_coefficient");
		if (s <= 0)
			throw std::exception("EStats:illegal_speed");
		maxHP = m;
		curHP = c;
		damC = coeff;
		movS = s;
	}

	Enemy::Enemy(const EStats &bes, const EStats &es, const std::string &str, const Pos &ep, const double &tw) : statsBase(bes), stats(es), name(str), p(ep)
	{
		if (tw < 0)
			throw exception("Enemy:illegal_waiting_time");
		timeW = tw;
	}
	
	Enemy::Enemy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw) : statsBase(bm, bc, br, bcoeff, bs), stats(m, c, r, coeff, s), name(str), p(ep)
	{
		if (tw < 0)
			throw exception("Enemy:illegal_waiting_time");
		timeW = tw;
	}

	Enemy::Enemy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw) : statsBase(bm, bc, br, bcoeff, bs), stats(m, c, r, coeff, s), name(str), p(x, y)
	{
		if (tw < 0)
			throw exception("Enemy:illegal_waiting_time");
		timeW = tw;
	}

	Enemy::Enemy(const EStats &es, const std::string &str, const Pos &ep, const double &tw) : statsBase(es), stats(es), name(str), p(ep)
	{
		if (tw < 0)
			throw exception("Enemy:illegal_waiting_time");
		timeW = tw;
	}

	Enemy::Enemy(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw) : statsBase(m, c, r, coeff, s), stats(m, c, r, coeff, s), name(str), p(ep)
	{
		if (tw < 0)
			throw exception("Enemy:illegal_waiting_time");
		timeW = tw;
	}

	Enemy::Enemy(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw) : statsBase(m, c, r, coeff, s), stats(m, c, r, coeff, s), name(str), p(x, y)
	{
		if (tw < 0)
			throw exception("Enemy:illegal_waiting_time");
		timeW = tw;
	}

	int Enemy::auraCheck(Level &L)
	{
		//reset stats
		double tempHP = stats.curHP;
		stats = statsBase;
		stats.curHP = tempHP;
		int c = 0;
		for (Level::iterator it = L.begin(); it != L.end(); ++it)
		{
			if (*it == this)
				continue;
			Hero *eh = dynamic_cast<Hero*>(*it);
			if (!eh)
				continue;
			if (dist(**it) < eh->getAura().getRad())
			{
				eh->getAura().buff(*this);
				cout << name << " is buffed by "<< (**it).name << endl;
				++c;
			}
		}
		//curHP check
		if (stats.curHP > stats.maxHP)
			stats.curHP = stats.maxHP;
		return c;
	}

	void Enemy::globalAC(Level &L)
	{
		auraCheck(L); //for a regular enemy
	}

	double Enemy::dist(const Enemy &e) const
	{
		return p.dist(e.p);
	}

	double Enemy::dist(const Building &b) const
	{
		return p.dist(b.getPos());
	}

	int Enemy::hit(const double &dmg)
	{
		if (dmg < 0)
			throw exception("Enemy:illegal_damage");
		stats.curHP -= dmg;
		cout << "Enemy " << name << " is damaged, " << stats.curHP << " HP left" << endl;
		if (stats.curHP <= 0)
			return 1;
		return 0;
	}

	void Enemy::regen(const double &tQ)
	{
		if (tQ <= 0)
			throw exception("Enemy:illegal_time_quant");
		stats.curHP = fmin(stats.curHP + tQ*stats.regHP, stats.maxHP);
		cout << "Enemy " << name << " regens to " << stats.curHP << " HP" << endl;
	}

	void Enemy::addTW(const double &tQ)
	{
		if (tQ <= 0)
			throw exception("Enemy:illegal_time_quant");
		timeW += tQ;
	}

	bool Enemy::isReady() const
	{
		return (timeW >= 1.0 / stats.movS);
	}

	double Enemy::remainingTime() const
	{
		return fmax(1.0 / stats.movS - timeW, 0);
	}

	ostream &operator <<(ostream &f, const Enemy &e)
	{
		return e.print(f);
	}
}