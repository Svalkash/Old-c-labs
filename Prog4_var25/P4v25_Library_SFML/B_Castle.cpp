#include "stdafx.h"
#include "B_Castle.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{
	CStats::CStats(const double &gs, const int &pr, const double &m, const double &r, const int &uc)
	{
		if (gs <= 0)
			throw exception("CStats:illegal_giving_speed");
		if (pr <= 0)
			throw exception("CStats:illegal_profit");
		if (m <= 0)
			throw std::exception("CStats:illegal_maximum_HP");
		if (r < 0)
			throw exception("CStats:illegal_repair_speed");
		if (uc < 0)
			throw exception("CStats:illegal_upgrade_cost");
		givingS = gs;
		profit = pr;
		maxHP = m;
		regHP = r;
		upgradeCost = uc;
	}

	B_Castle::B_Castle(const Pos &p, const int &l, const CStats &cs, const std::string &str, const double &cur, const double &tw) :Building(p), stats(cs), name(str)
	{
		if (l < 0)
			throw exception("B_Castle:illegal_level");
		if (cur <= 0)
			throw exception("B_Castle:illegal_current_HP");
		if (cur > cs.maxHP)
			throw exception("B_Castle:current_HP>maximum_HP");
		if (tw < 0)
			throw exception("B_Castle:illegal_waiting_time");
		lvl = l;
		curHP = cur;
		timeW = tw;
	}

	B_Castle::B_Castle(const int &x, const int &y, const int &l, const CStats &cs, const std::string &str, const double &cur, const double &tw) :Building(x, y), stats(cs), name(str)
	{
		if (l < 0)
			throw exception("B_Castle:illegal_level");
		if (cur <= 0)
			throw exception("B_Castle:illegal_current_HP");
		if (cur > cs.maxHP)
			throw exception("B_Castle:current_HP>maximum_HP");
		if (tw < 0)
			throw exception("B_Castle:illegal_waiting_time");
		lvl = l;
		curHP = cur;
		timeW = tw;
	}

	void B_Castle::setStats(const CStats &ncs)
	{
		curHP += (ncs.maxHP - stats.maxHP);
		stats = ncs;
		//stats changed, updating bars
		setHPbarSize();
		setGOLDbarSize();
	}

	int B_Castle::hit(const double &dmg)
	{
		if (dmg < 0)
			throw exception("B_Castle:illegal_damage");
		curHP -= dmg;
		//cout << "Castle " << name << " is damaged, " << curHP << " HP left" << endl;
		if (curHP <= 0)
			return 1;
		setHPbarSize();
		return 0;
	}

	void B_Castle::regen(const double &tQ)
	{
		curHP = fmin(curHP + tQ*stats.regHP, stats.maxHP);
		setHPbarSize();
		//cout << "Castle " << name << " regens to " << curHP << " HP" << endl;
	}


	void B_Castle::addTW(const double &tQ)
	{
		if (tQ < 0)
			throw exception("B_Castle:illegal_time_quant");
		timeW += tQ;
		setGOLDbarSize();
	}

	bool B_Castle::isReady() const
	{
		return (timeW >= 1.0 / stats.givingS);
	}

	int B_Castle::giveMoney()
	{
		//cout << name << " gives money: " << stats.profit << endl;
		timeW = 0;
		setGOLDbarSize();
		return stats.profit;
	}

	double B_Castle::remainingTime() const
	{
		return fmax(1.0 / stats.givingS - timeW, 0);
	}

	ostream &operator <<(ostream &f, const B_Castle &bc)
	{
		f << bc.name << ' ' << bc.p.x << ' ' << bc.p.y << ' ' << bc.lvl << ' ' << bc.curHP << ' ' << bc.timeW;
		return f;
	}

	B_Castle *B_Castle::clone() const
	{
		return new B_Castle(*this);
	}

	void B_Castle::setHPbarSize()
	{
		HPbar.setSize(sf::Vector2f(HPbarBase.getSize().x * (curHP / stats.maxHP), HPbarBase.getSize().y));
	}

	void B_Castle::setGOLDbarSize()
	{
		GOLDbar.setSize(sf::Vector2f(GOLDbarBase.getSize().x * (timeW / (1.0 / stats.givingS)), GOLDbarBase.getSize().y));
	}

	void B_Castle::draw(sf::RenderWindow &window)
	{
		window.draw(HPbarBase);
		window.draw(HPbar);
		window.draw(GOLDbarBase);
		window.draw(GOLDbar);
	}

	double B_Castle::getHPpart() const
	{
		return fmax(curHP / stats.maxHP, 0.0);
	}

	std::string B_Castle::getInfo(const Level &L) const
	{
		const CStats *next = L.getNextCastleLevel(lvl);
		stringstream s;
		s << setprecision(PRINT_PRECISION) << fixed;
		s << endl;
		s << "Name: " << name;
		s << endl << "HP: " << curHP << "/" << stats.maxHP;
		if (next)
			s << " (" << next->maxHP << ')';
		s << endl << "Regeneration: " << stats.regHP;
		if (next)
			s << " (" << next->regHP << ')';
		s << endl << "Giving speed: " << stats.givingS;
		if (next)
			s << " (" << next->givingS << ')';
		s << endl << "Profit: " << stats.profit;
		if (next)
			s << " (" << next->profit << ')';
		if (next)
			s << endl << "UPGRADE COST: " << next->upgradeCost;
		return s.str();
	}

	std::string B_Castle::getInfoHP() const
	{
		stringstream s;
		s << setprecision(PRINT_PRECISION) << fixed;
		s << endl << "HP: " << curHP << "/" << stats.maxHP;
		s << endl << "Regeneration: " << stats.regHP;
		return s.str();
	}
}