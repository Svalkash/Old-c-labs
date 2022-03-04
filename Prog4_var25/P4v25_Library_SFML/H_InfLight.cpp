#include "stdafx.h"
#include "H_InfLight.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{
	void H_InfLight::globalAC(Level &L)
	{
		for (Level::iterator it = L.begin(); it != L.end(); ++it)
			(**it).auraCheck(L); //hero can change everything
		//GlobalAC launches when THIS enemy moves. So... it will be good to put here chenging aura pos.
		setAuraCirclePosition();
	}

	ostream &H_InfLight::print(ostream &f) const
	{
		/*
		IL-0
		HIL-1
		IH-2
		HIH-3
		A-4
		HA-5
		*/
		const int type = 1;
		f << type << ' ' << name << ' ' << p.x << ' ' << p.y << ' ' << statsBase.maxHP << ' ' << stats.curHP << ' ' << statsBase.regHP << ' ' << statsBase.damC << ' ' << statsBase.movS;
		f << endl;
		f << getAura();
		return f;
	}

	H_InfLight *H_InfLight::clone() const
	{
		return new H_InfLight(*this);
	}

	Pos H_InfLight::findGoal(Level &L)
	{
		const int type = 1;
		//OK, we're inside the tile. Its goal will be to the left, right, ...
		Pos tilePos = Pos(p); //round it to find "our" tile
		Pos tileNext = L[tilePos].nextL;
		//checking if we are moving from tile to tileGoal or from smth to tilePos
		/*
		1. (tileGoal.x - tilePos.x) XOR (tileGoal.y - tilePos.y) will be !=0 (if both == 0.. well,
		2. if it's the same direction, ...*(p.x-tilePos.x) [or y] will be >0.
		3. if tileNext == tilePos...
		4. if p == tilePos, next = tileNext.
		*/
		if ((p.x - tileNext.x) && (p.y - tileNext.y))
			goal = tilePos;
		else
			if ((tileNext.x - tilePos.x) * (p.x - tilePos.x) + (tileNext.y - tilePos.y) * (p.y - tilePos.y) >= 0)
				goal = tileNext;
			else
				goal = tilePos;
		//setting texture for sprite
		sprite.setTexture(L.getTexture(100 + 10 * type + direction()));
		return goal;
	}

	void H_InfLight::setAuraCirclePosition()
	{
		auraCircle.setPosition(sprite.getPosition()); //LOL))
	}
}