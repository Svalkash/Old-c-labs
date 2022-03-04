#pragma once

#include "E_InfHeavy.h"
#include "Hero.h"

namespace P4v25
{
	class H_InfHeavy :
		public E_InfHeavy,
		public Hero
	{
	public:
		H_InfHeavy() : E_InfHeavy(), Hero() {}
		H_InfHeavy(const EStats &bes, const EStats &es, const std::string &str, const Pos &ep, const double &tw, const HAura &ha) : E_InfHeavy(bes, es, str, ep, tw), Hero(ha) {}
		H_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfHeavy(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep, tw), Hero(am, ar, acoeff, as, arad) {}
		H_InfHeavy(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfHeavy(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y, tw), Hero(am, ar, acoeff, as, arad) {}
		//without base
		H_InfHeavy(const EStats &es, const std::string &str, const Pos &ep, const double &tw, const HAura &ha) : E_InfHeavy(es, str, ep, tw), Hero(ha) {}
		H_InfHeavy(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfHeavy(m, c, r, coeff, s, str, ep, tw), Hero(am, ar, acoeff, as, arad) {}
		H_InfHeavy(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfHeavy(m, c, r, coeff, s, str, x, y, tw), Hero(am, ar, acoeff, as, arad) {}
		virtual std::ostream &print(std::ostream &) const;
		void globalAC(Level &);
		virtual ~H_InfHeavy() {};
		virtual H_InfHeavy *clone() const;
	};
}