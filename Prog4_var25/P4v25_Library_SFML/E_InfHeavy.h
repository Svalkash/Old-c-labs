#pragma once

#include "Enemy.h"

namespace P4v25_SFML
{
	class E_InfHeavy :
		public Enemy
	{
	public:
		E_InfHeavy() : Enemy() {}
		E_InfHeavy(const EStats &bes, const EStats &es, const std::string &str, const ePos &ep) : Enemy(bes, es, str, ep) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep) : Enemy(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y) : Enemy(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y) {}
		//without base
		E_InfHeavy(const EStats &bes, const std::string &str, const ePos &ep) : Enemy(bes, str, ep) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const std::string &str, const ePos &ep) : Enemy(bm, bc, br, bcoeff, bs, str, ep) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const std::string &str, const double &x, const double &y) : Enemy(bm, bc, br, bcoeff, bs, str, x, y) {}
		virtual int move(Level &);
		virtual std::ostream &print(std::ostream &) const;
		virtual ~E_InfHeavy() {}
		virtual E_InfHeavy *clone() const;
		double remainingTime(Level &) const; //remaining until we can reach our goal. Depending on type
		virtual Pos findGoal(Level &);
		virtual std::string getTypeInfo() const { return std::string("Enemy - Infantry Heavy"); }
	};
}