#include "stdafx.h"
#include "E_Aviation.h"
#include "Hero.h"
#include "Level.h"

using namespace std;

namespace P4v25
{
	int E_Aviation::move(Level &L)
	{
		cout << name << " moves" << endl;
		//pathfind is DONE!
		//isReady must be checked before
		Pos nextP; //need to do that; otherwise I can't get next position if there's no building.
		Tile *nextT;
		//ALMOST reset waiting
		timeW -= 1.0 / stats.movS;
		nextP = L[p].nextA;
		nextT = &L[nextP];
		if (nextT->type == 13)
		{
			B_Castle *bc = static_cast<B_Castle*>(nextT->b);
			//enemy dies ANYWAY. Castle - preferably not))
			cout << name << " attacks " << "castle "<< bc->getName() << endl;
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
		f << type << ' ' << name << ' ' << p.x << ' ' << p.y << ' ' << statsBase.maxHP << ' ' << stats.curHP << ' ' << statsBase.regHP << ' ' << statsBase.damC << ' ' << statsBase.movS << ' ' << timeW;
		return f;
	}

	E_Aviation *E_Aviation::clone() const
	{
		return new E_Aviation(*this);
	}
}