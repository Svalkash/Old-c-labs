#include "stdafx.h"
#include "AppStruct.h"

using namespace std;

namespace P5v2
{
	AppStruct::AppStruct(const std::string &fname)
	{
		try
		{
			ifstream f(fname);
			if (!f.is_open())
				throw exception("cannot_open_file");
			//place number
			int placeN;
			f >> placeN;
			if (placeN <= 0)
				throw exception("negative_place_number");
			//places
			for (int i = 0; i < placeN; ++i)
			{
				string name;
				int minD, maxD;
				f >> name >> minD >> maxD;
				if (name == "NULL")
					throw exception("NULL_reserved_for_exit");
				if (minD < 0)
					throw exception("negative_delay");
				if (maxD < 0)
					throw exception("negative_delay");
				Place *p = new Place(name, minD, maxD);
				//arc number
				int arcN;
				f >> arcN;
				if (arcN < 0)
					throw exception("negative_arc_number");
				for (int j = 0; j < arcN; ++j)
				{
					int id;
					f >> id;
					p->initArc(id);
				}
				sp.insert(p);
				p = nullptr;
			}
			//trans number
			int transN;
			f >> transN;
			if (transN <= 0)
				throw exception("negative_transition_number");
			//transitions
			for (int i = 0; i < transN; ++i)
			{
				string name;
				f >> name;
				Transition *t = new Transition(name);
				//out
				int outN;
				f >> outN;
				if (outN < 0)
					throw exception("illegal_out_connections_number");
				for (int j = 0; j < outN; ++j)
				{
					string outName;
					f >> outName;
					Place *ptr = searchForNameP(outName);
					if (!ptr)
						throw exception("wrong_link");
					t->addOut(ptr);
				}
				st.insert(t);
				t = nullptr;
			}
			//from p to t
			int FpTtN;
			f >> FpTtN;
			if (FpTtN <= 0)
				throw exception("illegal_fPtT_connections_number");
			for (int i = 0; i < FpTtN; ++i)
			{
				string pName, tName;
				int chance;
				f >> pName >> tName >> chance;
				Place *ptrP = searchForNameP(pName);
				if (!ptrP)
					throw exception("wrong_link");
				Transition *ptrT = searchForNameT(tName);
				if (!ptrT)
					throw exception("wrong_link");
				ptrP->addTrans(ptrT, chance);
				ptrT->addIn(ptrP);
			}

		}
		catch (exception &ex)
		{
			for (set<Place*>::iterator it = sp.begin(); it != sp.end(); ++it)
				delete (*it);
			for (set<Transition*>::iterator it = st.begin(); it != st.end(); ++it)
				delete (*it);
			throw ex;
		}
	}

	P5v2::Place *AppStruct::searchForNameP(const std::string &name) const
	{
		for (std::set<P5v2::Place*>::const_iterator it = sp.begin(); it != sp.end(); ++it)
			if ((**it).getName() == name)
				return (*it);
		return nullptr;
	}

	P5v2::Transition *AppStruct::searchForNameT(const std::string &name) const
	{
		for (std::set<P5v2::Transition*>::const_iterator it = st.begin(); it != st.end(); ++it)
			if ((**it).getName() == name)
				return (*it);
		return nullptr;
	}


	AppStruct::~AppStruct()
	{
		for (set<Place*>::iterator it = sp.begin(); it != sp.end(); ++it)
			delete (*it);
		for (set<Transition*>::iterator it = st.begin(); it != st.end(); ++it)
			delete (*it);
	}

	void AppStruct::merge(const std::string &fname) const
	{
		multimap<int, string> records;
		for (set<Place*>::iterator it = sp.begin(); it != sp.end(); ++it)
		{
			ifstream f;
			f.open((**it).getName() + ".txt");
			if (!f.is_open())
				throw exception("logs_are_lost");
			string nameStr = " [" + (**it).getName() + "]";
			while (!f.eof())
			{
				char cRec[MAX_STR_LEN];
				f.getline(cRec, MAX_STR_LEN);
				string sRec(cRec);
				istringstream iss(sRec);
				int RecTime;
				iss >> RecTime;
				char cRecTxt[MAX_STR_LEN];
				iss.getline(cRecTxt, MAX_STR_LEN);
				string sRecTxt(nameStr + cRecTxt);
				records.insert(pair<int, string>(RecTime, sRecTxt));
			}
			f.close();
		}
		//everything loaded
		ofstream mf(fname);
		for (multimap<int, string>::iterator it = records.begin(); it != records.end(); ++it)
			mf << (*it).first << (*it).second << endl;
		mf.close();
	}

	void checkAS_MT(AppStruct &as, int &placesWaiting, const int max_time, const std::chrono::steady_clock::time_point base)
	{
		try
		{
			as.base_time = base;
			for (set<Transition*>::iterator it = as.st.begin(); it != as.st.end(); ++it)
				(**it).base_time = base;
			while (placesWaiting)
			{
				if (getTimeMillis(base) > max_time)
				{
					placesWaiting = 0;
					continue;
				}
				for (set<Transition*>::iterator it = as.st.begin(); it != as.st.end(); ++it)
					(**it).checkT();
			}
		}
		catch (exception &ex)
		{
			cout << "Thread of transitions stopped, exception: " << ex.what();
			system("pause");
		}
	}
}