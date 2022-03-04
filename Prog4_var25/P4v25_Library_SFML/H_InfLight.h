#pragma once

#include "E_InfLight.h"
#include "Hero.h"

namespace P4v25_SFML
{
	class H_InfLight :
		public E_InfLight,
		public Hero
	{
	public:
		H_InfLight() : E_InfLight(), Hero() {}
		H_InfLight(const EStats &bes, const EStats &es, const std::string &str, const ePos &ep, const HAura &ha) : E_InfLight(bes, es, str, ep), Hero(ha) {}
		H_InfLight(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_InfLight(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, ep), Hero(am, ar, acoeff, as, arad) {}
		H_InfLight(const double &bm, const double &bc, const double &br, const double &bcoeff, const double &bs, const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_InfLight(bm, bc, br, bcoeff, bs, m, c, r, coeff, s, str, x, y), Hero(am, ar, acoeff, as, arad) {}
		//without base
		H_InfLight(const EStats &es, const std::string &str, const ePos &ep, const HAura &ha) : E_InfLight(es, str, ep), Hero(ha) {}
		H_InfLight(const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const ePos &ep, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_InfLight(m, c, r, coeff, s, str, ep), Hero(am, ar, acoeff, as, arad) {}
		H_InfLight(const double &m, const double &c, const double &r, const double &coeff, const double &s, const std::string &str, const double &x, const double &y, const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) : E_InfLight(m, c, r, coeff, s, str, x, y), Hero(am, ar, acoeff, as, arad) {}
		virtual std::ostream &print(std::ostream &) const;
		void globalAC(Level &);
		virtual ~H_InfLight() {};
		virtual H_InfLight *clone() const;
		virtual Pos findGoal(Level &);
		void setAuraCirclePosition();
		virtual void drawAura(sf::RenderWindow &window) { drawAuraCircle(window); }
		virtual std::string getAuraInfo() const { return getHeroAuraInfo(); }
		virtual std::string getTypeInfo() const { return std::string("Hero - Infantry Light"); }
	};
}