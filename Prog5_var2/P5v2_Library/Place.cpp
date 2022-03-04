#include "stdafx.h"
#include "Place.h"
#include "Transition.h"

using namespace std;

namespace P5v2
{
	const Transition *Place::choose() const
	{
		p_lock.lock();
		int r = rand() % 100;
		TransMap::const_iterator it;
		int c = 0;
		for (it = trans.begin(); it != trans.end(); ++it)
		{
			c += (*it).second.chance;
			if (c >= r)
				break;
		}
		//c >= r, it = chosen one
		const Transition *ptr = (*it).first;
		p_lock.unlock();
		return ptr;
	}

	void Place::receiveArc(const Arc &narc)
	{
		p_lock.lock();
		//randomizing time
		Arc carc(narc);
		carc.setWakeTime(getTimeMillis(base_time) + rand() % (maxDelay - minDelay + 1) + minDelay);
		log << endl << getTimeMillis(base_time) << " Arc " << carc.getID() << " will sleep until " << carc.getWakeTime() << " ms";
		write();
		waiting.insert(carc);
		p_lock.unlock();
	}

	void Place::initArc(const int &id)
	{
		waiting.insert(Arc(id, 0));
	}

	Arc Place::sendArc(const Transition *target)
	{
		p_lock.lock();
		if (!trans.at(target).readyArcs.size())
			throw exception("Place:sendArc:no_arcs_available_for_this_target");
		Arc ret = trans.at(target).readyArcs.front();
		trans.at(target).readyArcs.erase(trans.at(target).readyArcs.begin());
		//blocked will be defined later
		p_lock.unlock();
		return ret;
	}

	Arc Place::preSendArc(const Transition *target)
	{
		p_lock.lock();
		if (!trans.at(target).readyArcs.size())
			throw exception("Place:sendArc:no_arcs_available_for_this_target");
		Arc ret = trans.at(target).readyArcs.front();
		p_lock.unlock();
		return ret;
	}
	/*
	bool Place::isEmpty() const
	{
		p_lock.lock();
		if (waiting.size())
		{
			p_lock.unlock();
			return 0;
		}
		for (TransMap::const_iterator it = trans.begin(); it != trans.end(); ++it)
			if ((*it).second.readyArcs.size())
			{
				p_lock.unlock();
				return 0;
			}
		p_lock.unlock();
		return 1;
	}

	bool Place::isBlocked() const
	{
		p_lock.lock();
		if (waiting.size())
			return 0;
		for (TransMap::const_iterator it = trans.begin(); it != trans.end(); ++it)
			if (!((*it).second.blocked && (*it).second.readyArcs.size()))
			{
				p_lock.unlock();
				return 0;
			}
		p_lock.unlock();
		return 1;
	}
	*/

	bool Place::isWaiting() const
	{
		p_lock.lock();
		if (waiting.size())
		{
			p_lock.unlock();
			return 1;
		}
		for (TransMap::const_iterator it = trans.begin(); it != trans.end(); ++it)
			if ((!(*it).second.blocked) && (*it).second.readyArcs.size())
			{
				p_lock.unlock();
				return 1;
			}
		p_lock.unlock();
		return 0;
	}

	void Place::checkP()
	{
		p_lock.lock();
		if (waiting.size() && getTimeMillis(base_time) > waiting.begin()->getWakeTime())
		{
			const Transition *target = choose();
			p_lock.unlock();
			target->t_lock.lock(); //now it can't ask us.
			p_lock.lock();
			Arc marc = *waiting.begin();
			waiting.erase(waiting.begin());
			log << endl << getTimeMillis(base_time) << " Arc " << marc.getID() << " wakes up, goes to " << target->getName();
			trans.at(target).readyArcs.push_back(marc);
			p_lock.unlock();
			if (!target->isReady(this)) //blocked flag is already set
				log << endl << getTimeMillis(base_time) << " Arc " << marc.getID() << " is blocked, waiting for " << target->getName() << " opening";
			//that's all
			target->t_lock.unlock();
		}
		else
			p_lock.unlock();
	}

	bool Place::checkChances() const
	{
		p_lock.lock();
		int sum = 0;
		for (TransMap::const_iterator it = trans.begin(); it != trans.end(); ++it)
			sum += (*it).second.chance;
		p_lock.unlock();
		return (sum == 100) ? 1 : 0;
	}

	void Place::addTrans(const Transition *tr, const int &chance)
	{
		if (chance <= 0)
			throw exception("illegal_chance");
		trans.insert(pair<const Transition*, linkInfo>(tr, linkInfo(chance)));
	}

	void Place::write()
	{
		p_lock.lock();
		log << endl << getTimeMillis(base_time) << " ";
		log << "Waiting: ";
		for (ArcQueue::const_iterator it = waiting.begin(); it != waiting.end(); ++it)
			log << (*it).getID() << "(" << (*it).getWakeTime() << ") ";
		log << "Ready: ";
		for (TransMap::const_iterator it = trans.begin(); it != trans.end(); ++it)
		{
			log << (*it).first->getName() << "(";
			for (std::vector<Arc>::const_iterator lit = (*it).second.readyArcs.begin(); lit != (*it).second.readyArcs.end(); ++lit)
				log << (*lit).getID() << ", ";
			log << ") ";
		}
		p_lock.unlock();
	}

	void checkP_MT(Place &p, int &placesWaiting, const int max_time, const std::chrono::steady_clock::time_point base, int startState)
	{
		try
		{
			//init srand for thread
			srand(time(nullptr));
			int curState = startState;
			while (placesWaiting)
			{
				if (getTimeMillis(base) > max_time)
				{
					p.log << endl << getTimeMillis(base) << " " << "Stopped: time is out.";
					placesWaiting = 0;
					continue;
				}
				int oldState = curState;
				p.checkP();
				curState = (p.isWaiting());
				if (!oldState && curState)
					++placesWaiting;
				else if (oldState && !curState)
					--placesWaiting;
				//p.write();
			}
			p.write();
		}
		catch (exception &ex)
		{
			cout << "Thread of place " << p.name << " stopped, exception:" << endl << ex.what();
			placesWaiting = 0;
		}
		p.log.close();
	}
}