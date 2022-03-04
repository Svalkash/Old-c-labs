#include "stdafx.h"
#include "E_Aviation.h"
#include "Hero.h"
#include "Level.h"

using namespace std;

namespace P4v25_SFML
{
	int E_Aviation::move(Level &L)
	{
		//cout << name << " moves" << endl;
		//pathfind is DONE!
		//isReady must be checked before
		//goal is found already
		Tile *nextT = &L[goal];
		if (nextT->type == 13)
		{
			B_Castle *bc = static_cast<B_Castle*>(nextT->b);
			//enemy dies ANYWAY. Castle - preferably not))
			//cout << name << " attacks " << "castle " << bc->getName() << endl;
			if (bc->hit(stats.curHP * stats.damC)) //building first: enemy HP determines the damage
				L.destroyBuilding(*nextT);
			hit(stats.curHP); //unnecessary: dies anyway
			//no need to found a goal
			return 1;
		}
		//nothing to damage, can just MOVE
		//only one will change... or nothing (if cannot move)
		p.x += sign(goal.x - p.x) * stats.movS * L.getTimeQ();
		p.y += sign(goal.y - p.y) * stats.movS * L.getTimeQ();
		//what if we have already reached the goal? (we can't miss it: timeQ was calculated using this data)
		if (p == goal)
			findGoal(L); //well, find the new goal)
		globalAC(L); //check auras... CHANGE FOR HEROES!!!
		setHPbarPosition(L);
		setHPbarSize();
		//check sprite POSITION
		sprite.setPosition(p.x * L.getTileWidth() + L.getTileWidth() / 2, p.y * L.getTileWidth() + L.getTileWidth() / 2); //gets our position->corrects it->multiplies->transforms to vector->setsPosition. Origin MUST be already set.
		return 0;
	}

	ostream &E_Aviation::print(ostream &f) const
	{
		/*
		IL-0
		HIL-1
		IH-2
		HIH-3
		A-4
		HA-5
		*/
		const int type = 4;
		f << type << ' ' << name << ' ' << p.x << ' ' << p.y << ' ' << statsBase.maxHP << ' ' << stats.curHP << ' ' << statsBase.regHP << ' ' << statsBase.damC << ' ' << statsBase.movS;
		return f;
	}

	E_Aviation *E_Aviation::clone() const
	{
		return new E_Aviation(*this);
	}

	Pos E_Aviation::findGoal(Level &L)
	{
		const int type = 4;
		//OK, we're inside the tile. Its goal will be to the left, right, ...
		Pos tilePos = Pos(p); //round it to find "our" tile
		Pos tileNext = L[tilePos].nextA;
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

	double E_Aviation::remainingTime(Level &L) const
	{
		//if target is my position
		if (p == goal)
			return 0;
		//if target can be attacked
		if (L[goal].type == 13)
			return 0;
		//well, let's go
		return p.dist(goal) / stats.movS; //returning time)
	}
}