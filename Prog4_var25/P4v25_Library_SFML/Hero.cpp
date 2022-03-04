#include "stdafx.h"
#include "Hero.h"

namespace P4v25_SFML
{
	void Hero::drawAuraCircle(sf::RenderWindow &window)
	{
		window.draw(auraCircle);
	}
}