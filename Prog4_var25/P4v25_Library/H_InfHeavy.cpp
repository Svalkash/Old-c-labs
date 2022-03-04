#include "stdafx.h"
#include "H_InfHeavy.h"
#include "Level.h"

using namespace std;

namespace P4v25
{
	void H_InfHeavy::globalAC(Level &L)
	{
		for (Level::iterator it = L.begin(); it != L.end(); ++it)
			(**it).auraCheck(L); //hero can change everything
	}

	ostream &H_InfHeavy::print(ostream &f) const
	{
		/*
		IL-0
		HIL-1
		IH-2
		HIH-3
		A-4
		HA-5
		*/
		const int type = 3;
		f << type << ' ' << name << ' ' << p.x << ' ' << p.y << ' ' << statsBase.maxHP << ' ' << stats.curHP << ' ' << statsBase.regHP << ' ' << statsBase.damC << ' ' << statsBase.movS << ' ' << timeW;
		f << endl;
		f << getAura();
		return f;
	}

	H_InfHeavy *H_InfHeavy::clone() const
	{
		return new H_InfHeavy(*this);
	}
}