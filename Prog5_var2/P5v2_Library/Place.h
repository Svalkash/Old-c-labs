#pragma once

#include "Arc.h"

namespace P5v2
{
	struct linkInfo
	{
		int chance; // %
		std::vector<Arc> readyArcs;
		bool blocked;
		linkInfo(const int &c) : chance(c), blocked(0) {}
	};

	class Transition;

	class Place
	{
	private:
		typedef std::map<const Transition*, linkInfo> TransMap;
		typedef std::multiset<Arc> ArcQueue;
		TransMap trans;
		ArcQueue waiting;
		std::string name;
		int maxDelay, minDelay;
		std::chrono::steady_clock::time_point base_time;
	public:
		void setBaseTime(const std::chrono::steady_clock::time_point &bt) { base_time = bt; }
		std::ofstream log;
		mutable std::recursive_mutex p_lock;
		Place(const std::string &str = "noname", const int &minD = 1000, const int &maxD = 10000) : name(str), minDelay(minD), maxDelay(maxD) {}
		const Transition *choose() const;
		void receiveArc(const Arc &narc);
		Arc preSendArc(const Transition *target);
		Arc sendArc(const Transition *target);
		//bool isEmpty() const;
		//bool isBlocked() const;
		bool isWaiting() const;
		std::string getName() const { return name; }
		void setState(const int &ns);
		linkInfo &getLI(const Transition *tr) { return trans.at(tr); }
		void checkP();
		bool checkChances() const;
		void addTrans(const Transition *tr, const int &chance);
		friend void checkP_MT(Place &, int &, const int, const std::chrono::steady_clock::time_point, int);
		void write();
		void initArc(const int &id);
	};
}