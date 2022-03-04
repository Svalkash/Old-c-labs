#pragma once

#include "Enemy.h"

namespace P4v25_T
{
	class E_InfHeavy :
		public Enemy
	{
	public:
		E_InfHeavy() : Enemy() {}
		E_InfHeavy(const EStats &bes, const EStats &es, const std::string &str, const Pos &ep, const double &tw) : Enemy(bes, es, str, ep, tw) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw) : Enemy(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep, tw) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw) : Enemy(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y, tw) {}
		//without base
		E_InfHeavy(const EStats &bes, const std::string &str, const Pos &ep, const double &tw) : Enemy(bes, str, ep, tw) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const std::string &str, const Pos &ep, const double &tw) : Enemy(bm, bc, br, bcoeff, bs, str, ep, tw) {}
		E_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const std::string &str, const int &x, const int &y, const double &tw) : Enemy(bm, bc, br, bcoeff, bs, str, x, y, tw) {}
		virtual int move(Level &);
		virtual std::ostream &print(std::ostream &) const;
		virtual ~E_InfHeavy() {}
		virtual E_InfHeavy *clone() const;
	};
}