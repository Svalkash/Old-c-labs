#include "stdafx.h"
#include "Transition.h"
#include "Place.h"

using namespace std;

namespace P5v2
{
	void Transition::addIn(Place *link)
	{
		if (link)
			inList.insert(link);
	}

	void Transition::addOut(Place *link)
	{
		if (link)
			outList.insert(link);
	}

	bool Transition::isReady(Place *asker) const
	{
		t_lock.lock();
		//take control on all in
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
			(*itI)->p_lock.lock();
		bool ret = 1;
		for (set<Place*>::const_iterator it = inList.begin(); (it != inList.end()) && ret; ++it)
		{
			if (*it == asker)
				continue;
			if (!(*it)->getLI(this).readyArcs.size())
				ret = 0;
		}
		if (ret)
			for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
				(*itI)->getLI(this).blocked = 0;
		else
			for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
				(*itI)->getLI(this).blocked = 1;
		//take control on all in
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
			(*itI)->p_lock.unlock();
		t_lock.unlock();
		return ret;
	}

	void Transition::transfer() const
	{
		t_lock.lock();
		//take control on all in/out
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
			(*itI)->p_lock.lock();
		for (set<Place*>::const_iterator itO = outList.begin(); itO != outList.end(); ++itO)
			(*itO)->p_lock.lock();
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
		{
			Arc tempA = (*itI)->sendArc(this);
			(*itI)->log << endl << getTimeMillis(base_time) << " Arc " << tempA.getID() << " is sent to " << name;
			(*itI)->write();
			//can't get "blocked all". last unblocked is locked))
			if (!outList.size())
				(*itI)->log << endl << getTimeMillis(base_time) << " Arc " << tempA.getID() << " leaves the net" << name;
			else for (set<Place*>::const_iterator itO = outList.begin(); itO != outList.end(); ++itO)
			{
				(*itO)->log << endl << getTimeMillis(base_time) << " Arc " << tempA.getID() << " is received from " << name;
				(*itO)->receiveArc(tempA);
				(*itO)->write();
			}
		}
		//current "blocking" state
		isReady();
		//release in-out
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
			(*itI)->p_lock.unlock();
		for (set<Place*>::const_iterator itO = outList.begin(); itO != outList.end(); ++itO)
			(*itO)->p_lock.unlock();
		t_lock.unlock();
	}

	void Transition::checkT() const
	{
		t_lock.lock();
		//control In
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
			(*itI)->p_lock.lock();
		if (isReady())
		{
			//out will be controlled here
			transfer();
		}
		//release In
		for (set<Place*>::const_iterator itI = inList.begin(); itI != inList.end(); ++itI)
			(*itI)->p_lock.unlock();
		t_lock.unlock();
	}
}