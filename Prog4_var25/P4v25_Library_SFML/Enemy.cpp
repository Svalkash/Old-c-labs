#include "stdafx.h"
#include "Enemy.h"
#include "Building.h"
#include "Hero.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{

	EStats::EStats(const double &m, const double &c, const double &r, const double &coeff, const double &s) : regHP(r) //regen may be negative
	{
		if (m <= 0)
			throw std::exception("EStats:illegal_maximum_HP");
		if (c <= 0)
			throw std::exception("EStats:illegal_current_HP");
		//if (c > m)
		//	throw exception("Enemy:current_HP>maximum_HP");
		if (coeff <= 0)
			throw std::exception("EStats:illegal_damage_coefficient");
		if (s <= 0)
			throw std::exception("EStats:illegal_speed");
		maxHP = m;
		curHP = c;
		damC = coeff;
		movS = s;
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
				//cout << name << " is buffed by "<< (**it).name << endl;
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
		//cout << "Enemy " << name << " is damaged, " << stats.curHP << " HP left" << endl;
		if (stats.curHP <= 0)
			return 1;
		setHPbarSize();
		return 0;
	}

	void Enemy::regen(const double &tQ)
	{
		if (tQ < 0)
			throw exception("Enemy:illegal_time_quant");
		stats.curHP = fmin(stats.curHP + tQ*stats.regHP, stats.maxHP);
		setHPbarSize();
		//cout << "Enemy " << name << " regens to " << stats.curHP << " HP" << endl;
	}

	ostream &operator <<(ostream &f, const Enemy &e)
	{
		return e.print(f);
	}

	int Enemy::direction()
	{
		double dx = goal.x - p.x;
		double dy = goal.y - p.y;
		if (dx && dy)
			throw exception("Enemy:cannot_calculate_direction,unexpected_goal");
		if (dy > 0) //up
			return 0;
		if (dx < 0) //left
			return 1;
		if (dy < 0) //down
			return 2;
		if (dx > 0) //right
			return 3;
		return DEFAULT_DIRECTION;
	}

	void Enemy::setHPbarSize()
	{
		HPbar.setSize(sf::Vector2f(HPbarBase.getSize().x * (stats.curHP / stats.maxHP), HPbarBase.getSize().y));
	}

	void Enemy::setHPbarPosition(Level &L)
	{
		//I can calculate back sizes.. but it's too hard)
		HPbarBase.setPosition(p.x * L.getTileWidth(), p.y * L.getTileWidth() + (1 - BAR_HEIGHT_COEFF) * L.getTileWidth());
		HPbar.setPosition(p.x * L.getTileWidth(), p.y * L.getTileWidth() + (1 - BAR_HEIGHT_COEFF) * L.getTileWidth());
	}

	void Enemy::draw(sf::RenderWindow &window)
	{
		window.draw(sprite);
		window.draw(HPbarBase);
		window.draw(HPbar);
	}

	std::string Enemy::getInfo() const
	{
		stringstream s;
		s << setprecision(PRINT_PRECISION) << fixed;
		s << "Name: " << name;
		s << endl << getTypeInfo();
		s << endl << "HP: " << stats.curHP << '/' << stats.maxHP << " (" << statsBase.maxHP << ')';
		s << endl << "Regeneration: " << stats.regHP << " (" << statsBase.regHP << ')';
		s << endl << "Damage coeff: " << stats.damC << " (" << statsBase.damC << ')';
		s << endl << "Moving speed: " << stats.movS << " (" << statsBase.movS << ')';
		s << getAuraInfo();
		return s.str();
	}
}