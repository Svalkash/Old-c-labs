#pragma once

#include "E_InfLight.h"
#include "Hero.h"

namespace P4v25_T
{
	class H_InfLight :
		public E_InfLight,
		public Hero
	{
	public:
		H_InfLight() : E_InfLight(), Hero() {}
		H_InfLight(const EStats &bes, const EStats &es, const std::string &str, const Pos &ep, const double &tw, const HAura &ha) : E_InfLight(bes, es, str, ep, tw), Hero(ha) {}
		H_InfLight(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfLight(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep, tw), Hero(am, ar, acoeff, as, arad) {}
		H_InfLight(const double &bm, const double &bc, const double &br, const double &bcoeff, const int &bs, const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfLight(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y, tw), Hero(am, ar, acoeff, as, arad) {}
		//without base
		H_InfLight(const EStats &es, const std::string &str, const Pos &ep, const double &tw, const HAura &ha) : E_InfLight(es, str, ep, tw), Hero(ha) {}
		H_InfLight(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const Pos &ep, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfLight(m, c, r, coeff, s, str, ep, tw), Hero(am, ar, acoeff, as, arad) {}
		H_InfLight(const double &m, const double &c, const double &r, const double &coeff, const int &s, const std::string &str, const int &x, const int &y, const double &tw, const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) : E_InfLight(m, c, r, coeff, s, str, x, y, tw), Hero(am, ar, acoeff, as, arad) {}
		virtual std::ostream &print(std::ostream &) const;
		void globalAC(Level &);
		virtual ~H_InfLight() {};
		virtual H_InfLight *clone() const;
	};
}