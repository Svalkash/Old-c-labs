#include "stdafx.h"
#include "B_Tower.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{
	TStats::TStats(const int &r, const double &d, const double &s, const int &uc)
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
		if (tQ < 0)
			throw exception("B_Tower:illegal_time_quant");
		timeW = fmin(timeW + tQ, 1.0 / stats.shootS);
		setRELOADbarSize();
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
		//is there any target?
		if (target == L.end())
			return 0;
		//AIM SUCCEED!!
		//set shootline
		shootLine.setRotation(aimAngle);
		shootLine.setSize(sf::Vector2f(dist(**target) * L.getTileWidth(), SHOOTLINE_THICKNESS)); //no problems: distance between centers equals to distance between corners.. I hope.
		//cout << name << " shoots at " << (**target).getName() << endl;
		timeW = 0;
		setRELOADbarSize();
		if ((**target).hit(stats.dmg))
			L.killEnemy(target);
		return 1;
	}

	int B_Tower::aim(Level &L)
	{
		//remove shootLine, if expired
		if (timeW > SHOOTLINE_DELAY)
			shootLine.setSize(sf::Vector2f(0, SHOOTLINE_THICKNESS));
		//ok, let's find the target
		target = L.end(); //"not found"
		//if level is empty from enemies
		if (L.size() == 0)
			return 0;
		double mindist = -1;
		for (Level::iterator it = L.begin(); it != L.end(); ++it)
			if ((dist(**it) <= stats.rad) && ((mindist == -1) || (dist(**it) < mindist) || ((dist(**it) == mindist) && ((**it).getHP() < (**target).getHP())))) //if dists are equal, choose enemy with less HP
			{
				target = it;
				mindist = dist(**it);
			}
		if (mindist == -1)
			return 0;
		//set aimAngle
		aimAngle = atan(((**target).getPos().y - p.y) / ((**target).getPos().x - p.x));
		if ((**target).getPos().x < p.x)
			aimAngle += M_PI;
		aimAngle = aimAngle * 180 / M_PI; //degree
		//turn "head"
		topSprite.setRotation(aimAngle);
		return 1;
	}

	ostream &operator <<(ostream &f, const B_Tower &bt)
	{
		f << bt.name << ' ' << bt.p.x << ' ' << bt.p.y << ' ' << bt.lvl << ' ' << bt.timeW;
		return f;
	}

	B_Tower *B_Tower::clone() const
	{
		return new B_Tower(*this);
	}

	void B_Tower::setRELOADbarSize()
	{
		RELOADbar.setSize(sf::Vector2f(RELOADbarBase.getSize().x * (timeW / (1.0 / stats.shootS)), RELOADbarBase.getSize().y));
	}

	void B_Tower::setRangeCircleSize(Level &L)
	{
		rangeCircle.setOrigin(stats.rad * L.getTileWidth(), stats.rad * L.getTileWidth()); //moving origin
		rangeCircle.setRadius(stats.rad * L.getTileWidth());
	}

	void B_Tower::setStats(const TStats &nts, Level &L)
	{
		stats = nts;
		//stats changed, need to check bar/range
		setRELOADbarSize();
		setRangeCircleSize(L);
	}

	void B_Tower::draw(sf::RenderWindow &window)
	{
		window.draw(topSprite);
		window.draw(shootLine);
		window.draw(RELOADbarBase);
		window.draw(RELOADbar);
	}

	void B_Tower::drawRange(sf::RenderWindow &window)
	{
		window.draw(rangeCircle);
	}

	std::string B_Tower::getInfo(const Level &L) const
	{
		const TStats *next= L.getNextTowerLevel(lvl);
		stringstream s;
		s << setprecision(PRINT_PRECISION) << fixed;
		s << endl;
		s << "Name: " << name;
		s << endl << "Radius: " << stats.rad;
		if (next)
			s << " (" << next->rad << ')';
		s << endl << "Damage: " << stats.dmg;
		if (next)
			s << " (" << next->dmg << ')';
		s << endl << "Shooting speed: " << stats.shootS;
		if (next)
			s << " (" << next->shootS << ')';
		if (next)
			s << endl << "UPGRADE COST: " << next->upgradeCost;
		return s.str();
	}
}