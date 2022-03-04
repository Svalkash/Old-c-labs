#pragma once

#include "E_Aviation.h"
#include "Hero.h"

namespace P4v25_T
{
	class H_Aviation :
		public E_Aviation,
		public Hero
	{
	public:
		H_Aviation() : E_Aviation(), Hero() {}
		H_Aviation(const EStats &bes, const EStats &es, const std::string &str, const Pos &ep, const double &tw, const HAura &ha) : E_Aviation(bes, es, str, ep, tw), Hero(ha) {}
		H_Aviation(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_Aviation(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep, tw), Hero(am, ar, acoeff, as, arad) {}
		H_Aviation(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_Aviation(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y, tw), Hero(am, ar, acoeff, as, arad) {}
		//without base
		H_Aviation(const EStats &es, const std::string &str, const Pos &ep, const double &tw, const HAura &ha) : E_Aviation(es, str, ep, tw), Hero(ha) {}
		H_Aviation(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_Aviation(m, c, r, coeff, s, str, ep, tw), Hero(am, ar, acoeff, as, arad) {}
		H_Aviation(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_Aviation(m, c, r, coeff, s, str, x, y, tw), Hero(am, ar, acoeff, as, arad) {}
		virtual std::ostream &print(std::ostream &) const;
		void globalAC(Level &);
		virtual ~H_Aviation() {};
		virtual H_Aviation *clone() const;
	};
}