#include "stdafx.h"
#include "E_InfLight.h"
#include "Hero.h"
#include "Level.h"

using namespace std;

namespace P4v25
{
	int E_InfLight::move(Level &L)
	{
		cout << name << " moves" << endl;
		//pathfind is DONE!
		//isReady must be checked before
		Pos nextP; //need to do that; otherwise I can't get next position if there's no building.
		Tile *nextT;
		//ALMOST reset waiting
		timeW -= 1.0 / stats.movS;
		nextP = L[p].nextL;
		nextT = &L[nextP];
		if ((nextT->type == 11) || (nextT->type == 12)) //if ANY infantry goes to tower, or LIGHT infantry goes to wall. CHANGE VALUES FOR HERO!!!
			throw exception("E_InfLight:move:illegal_destination");
		if (nextT->type == 13)
		{
			B_Castle *bc = static_cast<B_Castle*>(nextT->b);
			//enemy dies ANYWAY. Castle - preferably not))
			cout << name << " attacks " << "castle " << bc->getName() << endl;
			if (bc->hit(stats.curHP * stats.damC)) //building first: enemy HP determines the damage
				L.destroyBuilding(*nextT);
			hit(stats.curHP); //unnecessary: dies anyway
			return 1;
		}
		//nothing to damage, can just MOVE
		p = nextP;
		globalAC(L); //check auras... CHANGE FOR HEROES!!!
		return 0;
	}

	ostream &E_InfLight::print(ostream &f) const
	{
		/*
		IL-0
		HIL-1
		IH-2
		HIH-3
		A-4
		HA-5
		*/
		const int type = 0;
		f << type << ' ' << name << ' ' << p.x << ' ' << p.y << ' ' << statsBase.maxHP << ' ' << stats.curHP << ' ' << statsBase.regHP << ' ' << statsBase.damC << ' ' << statsBase.movS << ' ' << timeW;
		return f;
	}

	E_InfLight *E_InfLight::clone() const
	{
		return new E_InfLight(*this);
	}
}