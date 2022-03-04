#include "stdafx.h"
#include "E_InfHeavy.h"
#include "Hero.h"
#include "Level.h"

using namespace std;

namespace P4v25_T
{
	int E_InfHeavy::move(Level &L)
	{
		cout << name << " moves" << endl;
		//pathfind is DONE!
		//isReady must be checked before
		Pos nextP; //need to do that; otherwise I can't get next position if there's no building.
		Tile *nextT;
		//ALMOST reset waiting
		timeW -= 1.0 / stats.movS;
		nextP = L[p].nextH;
		nextT = &L[nextP];
		if (nextT->type == 11)  //damage wall if it's HEAVY infantry. CHANGE VALUES FOR HERO!!!
		{
			B_Wall *bw = static_cast<B_Wall*>(nextT->b);
			cout << name << " attacks " << "wall " << bw->getName() << endl;
			if (bw->getHP() >= stats.curHP * stats.damC) //enemy dies, building - maybe
			{
				if (bw->hit(stats.curHP * stats.damC)) //building first: enemy HP determines the damage
					L.destroyBuilding(*nextT);
				hit(stats.curHP); //unnecessary: dies anyway
				return 1;
			}
			else //building is destroyed, NOT enemy
			{
				hit(bw->getHP() / stats.damC); //enemy first: building HP determines the damage
				bw->hit(bw->getHP()); //unnecessary: destroyed anyway
				L.destroyBuilding(*nextT);
				return 0;
			}
		}
		if (nextT->type == 12) //if ANY infantry goes to tower, or LIGHT infantry goes to wall. CHANGE VALUES FOR HERO!!!
			throw exception("E_InfHeavy:move:illegal_destination");
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

	ostream &E_InfHeavy::print(ostream &f) const
	{
		/*
		IL-0
		HIL-1
		IH-2
		HIH-3
		A-4
		HA-5
		*/
		const int type = 2;
		f << type << ' ' << name << ' ' << p.x << ' ' << p.y << ' ' << statsBase.maxHP << ' ' << stats.curHP << ' ' << statsBase.regHP << ' ' << statsBase.damC << ' ' << statsBase.movS << ' ' << timeW;
		return f;
	}

	E_InfHeavy *E_InfHeavy::clone() const
	{
		return new E_InfHeavy(*this);
	}
}