#pragma once

#include "E_Aviation.h"
#include "Hero.h"

namespace P4v25_SFML
{
	class H_Aviation :
		public E_Aviation,
		public Hero
	{
	public:
		H_Aviation() : E_Aviation(), Hero() {}
		H_Aviation(const EStats &bes, const EStats &es, const std::string &str, const ePos &ep, const HAura &ha) : E_Aviation(bes, es, str, ep), Hero(ha) {}
		H_Aviation(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_Aviation(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep), Hero(am, ar, acoeff, as, arad) {}
		H_Aviation(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_Aviation(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y), Hero(am, ar, acoeff, as, arad) {}
		//without base
		H_Aviation(const EStats &es, const std::string &str, const ePos &ep, const HAura &ha) : E_Aviation(es, str, ep), Hero(ha) {}
		H_Aviation(const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_Aviation(m, c, r, coeff, s, str, ep), Hero(am, ar, acoeff, as, arad) {}
		H_Aviation(const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_Aviation(m, c, r, coeff, s, str, x, y), Hero(am, ar, acoeff, as, arad) {}
		virtual std::ostream &print(std::ostream &) const;
		void globalAC(Level &);
		virtual ~H_Aviation() {};
		virtual H_Aviation *clone() const;
		virtual Pos findGoal(Level &);
		void setAuraCirclePosition();
		virtual void drawAura(sf::RenderWindow &window) { drawAuraCircle(window); }
		virtual std::string getAuraInfo() const { return getHeroAuraInfo(); }
		virtual std::string getTypeInfo() const { return std::string("Hero - Aviation"); }
	};
}