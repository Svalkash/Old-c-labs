#include "stdafx.h"
#include "B_Wall.h"

using namespace std;

namespace P4v25
{
	B_Wall::B_Wall(const Pos &po, const double &m, const double &c, const string &str) :Building(po), name(str)
	{
		if (m <= 0)
			throw exception("B_Wall:illegal_maximum_HP");
		if (c <= 0)
			throw exception("B_Wall:illegal_current_HP");
		if (c > m)
			throw exception("B_Wall:current_HP>maximum_HP");
		maxHP = m;
		curHP = c;
	}

	B_Wall::B_Wall(const int &x, const int &y, const double &m, const double &c, const string &str) :Building(x, y), name(str)
	{
		if (m <= 0)
			throw exception("B_Wall:illegal_maximum_HP");
		if (c <= 0)
			throw exception("B_Wall:illegal_current_HP");
		if (c > m)
			throw exception("B_Wall:current_HP>maximum_HP");
		maxHP = m;
		curHP = c;
	}

	int B_Wall::hit(const double &dmg)
	{
		if (dmg < 0)
			throw exception("B_Wall:illegal_damage");
		curHP -= dmg;
		cout << "Wall " << name << " is damaged, " << curHP << " HP left" << endl;
		if (curHP <= 0)
			return 1;
		return 0;
	}

	void B_Wall::heal(const double &dhp)
	{
		if (dhp<0)
			throw exception("B_Wall:illegal_healing_value");
		curHP = fmin(curHP + dhp, maxHP);
		cout << "Wall " << name << " is healed to " << curHP << " HP" << endl;
	}

	ostream &operator <<(ostream &f, const B_Wall &bw)
	{
		f << bw.name << ' ' << bw.p.x << ' ' << bw.p.y << ' ' << bw.curHP;
		return f;
	}

	B_Wall *B_Wall::clone() const
	{
		return new B_Wall(*this);
	}
}