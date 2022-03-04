#include "stdafx.h"
#include "HAura.h"
#include "Enemy.h"

using namespace std;

namespace P4v25_SFML
{
	HAura::HAura(const double &m, const double &r, const double &coeff, const double &s, const double &ra)
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

	std::string HAura::getInfo() const
	{
		stringstream s;
		s << setprecision(PRINT_PRECISION) << fixed;
		s;
		s << endl << string("Aura-Radius: ") << rad;
		s << endl << string("Aura-Maximum HP: ") << maxHP;
		s << endl << string("Aura-Regeneration: ") << regHP;
		s << endl << string("Aura-Damage coeff.: ") << damC;
		s << endl << string("Aura-Moving speed: ") << movS;
		return s.str();
	}
}