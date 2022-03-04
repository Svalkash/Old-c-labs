#include "stdafx.h"
#include "HAura.h"
#include "Enemy.h"

using namespace std;

namespace P4v25_T
{
	HAura::HAura(const double &m, const double &r, const double &coeff, const int &s, const double &ra)
	{
		if (m < 0)
			throw exception("HAura:negative_maximum_HP_buff");
		if (r < 0)
			throw exception("HAura:negative_regen_HP_buff");
		if (coeff < 0)
			throw exception("HAura:negative_damage_coefficient_buff");
		if (s < 0)
			throw exception("HAura:negative_speed_buff");
		if (ra < 0)
			throw exception("HAura:negative_radius");
		maxHP = m;
		damC = coeff;
		regHP = r;
		movS = s;
		rad = ra;
	}

	void HAura::buff(Enemy &e) const
	{
		e.stats.maxHP += maxHP;
		e.stats.regHP += regHP;
		e.stats.damC += damC;
		e.stats.movS += movS;
	}

	ostream &operator <<(ostream &f, const HAura &a)
	{
		f << a.maxHP << ' ' << a.regHP << ' ' << a.damC << ' ' << a.movS << ' ' << a.rad;
		return f;
	}
}