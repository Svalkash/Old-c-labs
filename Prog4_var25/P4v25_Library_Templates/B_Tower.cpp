#include "stdafx.h"
#include "B_Tower.h"
#include "Level.h"

using namespace std;

namespace P4v25_T
{
	TStats::TStats(const int &r, const double &d, const int &s, const int &uc)
	{
		if (r <= 0)
			throw exception("TStats:illegal_radius");
		if (d < 0)
			throw exception("TStats:illegal_damage");
		if (s <= 0)
			throw exception("TStats:illegal_shooting_speed");
		if (uc < 0)
			throw exception("TStats:illegal_upgrade_cost");
		rad = r;
		dmg = d;
		shootS = s;
		upgradeCost = uc;
	}

	B_Tower::B_Tower(const Pos &p, const int &l, const TStats &ts, const std::string &str, const double &tw) :Building(p), stats(ts), name(str)
	{
		if (l < 0)
			throw exception("B_Castle:illegal_level");
		if (tw < 0)
			throw exception("B_Castle:illegal_waiting_time");
		lvl = l;
		timeW = tw;
	}

	B_Tower::B_Tower(const int &x, const int &y, const int &l, const TStats &ts, const std::string &str, const double &tw) :Building(x, y), stats(ts), name(str)
	{
		if (l < 0)
			throw exception("B_Castle:illegal_level");
		if (tw < 0)
			throw exception("B_Castle:illegal_waiting_time");
		lvl = l;
		timeW = tw;
	}

	void B_Tower::addTW(const double &tQ)
	{
		if (tQ <= 0)
			throw exception("B_Tower:illegal_time_quant");
		timeW = fmin(timeW + tQ, 1.0 / stats.shootS);
	}

	bool B_Tower::isReady() const
	{
		return (timeW >= 1.0 / stats.shootS);
	}

	double B_Tower::remainingTime() const
	{
		return fmax(1.0 / stats.shootS - timeW, 0);
	}

	int B_Tower::shoot(Level &L)
	{
		//if level is empty from enemies
		if (L.size() == 0)
		{
			cout << name << " can't find any targets" << endl;
			return 0;
		}
		//isReady must be checked before
		Level::iterator target;
		double mindist = -1;
		for (Level::iterator it = L.begin(); it != L.end(); ++it)
			if ((dist(**it) <= stats.rad) && ((mindist == -1) || (dist(**it) < mindist) || ((dist(**it) == mindist) && ((**it).getHP() < (**target).getHP())))) //if dists are equal, choose enemy with less HP
				{
					target = it;
					mindist = dist(**it);
				}
		if (mindist == -1) //no targets
		{
			cout << name << " can't find any targets" << endl;
			return 0;
		}
		//target found
		cout << name << " shoots at " << (**target).getName() << endl;
		timeW -= 1.0 / stats.shootS;
		if ((**target).hit(stats.dmg))
			L.killEnemy(*target);
		return 1;
	}

	ostream &operator <<(ostream &f, const B_Tower &bt)
	{
		f << bt.name << ' ' << bt.p.x << ' ' << bt.p.y << ' ' << bt.lvl<<' '<<bt.timeW;
		return f;
	}

	B_Tower *B_Tower::clone() const
	{
		return new B_Tower(*this);
	}
}