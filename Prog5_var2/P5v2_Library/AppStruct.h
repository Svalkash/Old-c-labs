#pragma once

#include "Arc.h"
#include "Place.h"
#include "Transition.h"

#define MAX_STR_LEN 800

namespace P5v2
{
	class AppStruct
	{
	public:
		std::set<Place*> sp;
		std::set<Transition*> st;
		AppStruct(const std::string &fname);
		P5v2::Place *searchForNameP(const std::string &name) const;
		P5v2::Transition *searchForNameT(const std::string &name) const;
		void merge(const std::string &fname) const;
		std::chrono::steady_clock::time_point base_time;
		friend void checkAS_MT(AppStruct &, int &, const int, const std::chrono::steady_clock::time_point);
		~AppStruct();
	};
}

