#pragma once

#include "HAura.h"

namespace P4v25
{
	class Level;

	class Hero
	{
	protected:
		HAura aura;
	public:
		Hero() : aura() {}
		Hero(const HAura &ha) :aura(ha) {}
		Hero(const double &am, const double &ar, const double &acoeff, const int &as, const double &arad) :aura(am, ar, acoeff, as, arad) {}
		const HAura &getAura() const { return aura; }
		virtual ~Hero() {}
	};
}