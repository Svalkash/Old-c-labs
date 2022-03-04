#pragma once

#include "HAura.h"

namespace P4v25_SFML
{
	class Level;

	class Hero
	{
	protected:
		HAura aura;
	public:
		sf::CircleShape auraCircle;
		Hero() : aura() {}
		Hero(const HAura &ha) :aura(ha) {}
		Hero(const double &am, const double &ar, const double &acoeff, const double &as, const double &arad) :aura(am, ar, acoeff, as, arad) {}
		const HAura &getAura() const { return aura; }
		virtual ~Hero() {}
		virtual void setAuraCirclePosition() = 0;
		virtual void drawAuraCircle(sf::RenderWindow &);
		std::string getHeroAuraInfo() const { return aura.getInfo(); }
	};
}