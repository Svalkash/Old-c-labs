#include "stdafx.h"
#include "B_Spawn.h"
#include "Level.h"

using namespace std;

namespace P4v25
{
	B_Spawn::B_Spawn(const Pos &p, const double &tw) :Building(p)
	{
		if (tw < 0)
			throw exception("B_Spawn:illegal_waiting_time");
		timeW = tw;
	}

	B_Spawn::B_Spawn(const int &x, const int &y, const double &tw) :Building(x, y)
	{
		if (tw < 0)
			throw exception("B_Spawn:illegal_waiting_time");
		timeW = tw;
	}

	void B_Spawn::addTW(const double &tQ)
	{
		if (tQ <= 0)
			throw exception("B_Spawn:illegal_time_quant");
		timeW += tQ;
	}

	bool B_Spawn::isReady()
	{
		for (multimap<double, Enemy*>::iterator it = spawnList.begin(); it != spawnList.end(); ++it)
			if ((*it).first <= timeW)
				return 1;
		return 0;
	}

	multimap<double, Enemy*>::iterator B_Spawn::findNext()
	{
		multimap<double, Enemy*>::iterator next = spawnList.begin(); //if size == 0, returns begin()
		for (multimap<double, Enemy*>::iterator it = spawnList.begin(); it != spawnList.end(); ++it)
			if ((*it).first < (*next).first)
				next = it;
		return next;
	}

	double B_Spawn::remainingTime()
	{
		if (spawnList.size() == 0)
			return 0;
		return fmax((*findNext()).first - timeW, 0);
	}

	int B_Spawn::release(Level &L)
	{
		for (multimap<double, Enemy*>::iterator it = spawnList.begin(); it != spawnList.end(); ++it)
			if ((*it).first <= timeW)
			{
				Enemy *eptr = (*it).second;
				//(*it).second->setPos(p);
				cout << "spawn at (" << p.x << "," << p.y << ") releases " << eptr->getName() << endl;
				L.addEnemy((*it).second); //we MOVE ptr to the level. No need to clone enemy.
				spawnList.erase(it);
				eptr->globalAC(L);
				return 1;
			}
		return 0;
	}

	int B_Spawn::releaseF(Level &L)
	{
		if (spawnList.size() == 0)
			return 0;
		multimap<double, Enemy*>::iterator next = findNext();
		L.addEnemy((*next).second); //we MOVE ptr to the level. No need to clone enemy.
		spawnList.erase(next);
		return 1;
	}

	void B_Spawn::addEnemy(Enemy *pe, const double &t)
	{
		spawnList.insert(pair<double, Enemy*>(t, pe)); //that works?
	}

	ostream &operator <<(ostream &f, B_Spawn &bs)
	{
		f << bs.p.x << ' ' << bs.p.y << ' ' << bs.timeW << endl;
		//writing SItem number
		f << bs.spawnList.size(); //endl will be later
		//writing enemy list
		for (multimap<double, Enemy*>::iterator it = bs.spawnList.begin(); it != bs.spawnList.end(); ++it)
		{
			f << endl;
			f << *(*it).second << endl;
			f << (*it).first;
		}
		/*
		so we'll get something like this
		x y tw
		size {
		[e parameters]
		time} {
		[e2 param]
		2 time} {...
		n time}
		WITHOUT endl after that.
		*/
		return f;
	}

	B_Spawn::~B_Spawn()
	{
		//multimap<double, Enemy*> can't delete pointers, let's help it!
		for (multimap<double, Enemy*>::iterator it = spawnList.begin(); it != spawnList.end(); ++it)
			delete (*it).second;
	}

	B_Spawn *B_Spawn::clone() const
	{
		return new B_Spawn(*this);
	}
}