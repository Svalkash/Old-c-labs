#pragma once

#include "Arc.h"

namespace P5v2
{
	class Place;
	class AppStruct;

	class Transition
	{
	private:
		std::string name;
		std::set<Place*> inList, outList;
		std::chrono::steady_clock::time_point base_time;
	public:
		mutable std::recursive_mutex t_lock;
		Transition(const std::string &str = "noname") :name(str) {}
		void transfer() const;
		void addIn(Place *link);
		void addOut(Place *link);
		bool isReady(Place *asker = nullptr) const;
		void checkT() const;
		std::string getName() const { return name; }
		friend void checkAS_MT(AppStruct &, int &, const int, const std::chrono::steady_clock::time_point);
	};
}