#pragma once

#include "Building.h"

namespace P4v25
{
	class Level;

	class B_Spawn :
		public Building
	{
	private:
		double timeW; //==current time: MUST NOT RESET!!
		std::multimap<double, Enemy*> spawnList;
	public:
		B_Spawn() : Building(), spawnList(), timeW(0) {}
		B_Spawn(const Pos &, const double &);
		B_Spawn(const int &, const int &, const double &);
		//
		void addTW(const double &); //adding timeQ to time W
		bool isReady();
		std::multimap<double, Enemy*>::iterator findNext(); //if size == 0, returns begin()
		double remainingTime(); //returns 0 if there's no enemies
		int release(Level &); //releases ONE enemy at time, returns 1 if released
		int releaseF(Level &); //releases ONE "next" enemy, FORCED, returns 0 if there's no enemies
		int size() const { return spawnList.size(); }
		void addEnemy(Enemy *pe, const double &t); //constructs SEItem(internal time check), adds it
		friend std::ostream &operator <<(std::ostream &, B_Spawn &);
		virtual ~B_Spawn();
		virtual B_Spawn *clone() const;
	};
}