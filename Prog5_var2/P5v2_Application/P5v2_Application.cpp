// P5v2_Application.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <sstream>
#include "../P5v2_Library/AppStruct.h"
#include <chrono>

using namespace P5v2;
using namespace std;
using namespace std::chrono;


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		cout << "Enter input file name: ";
		string fn;
		cin >> fn;
		AppStruct AS(fn);
		int pW = 0;
		for (set<Place*>::iterator it = AS.sp.begin(); it != AS.sp.end(); ++it)
			if ((*it)->isWaiting())
				++pW;
		cout << "Enter maximum time: ";
		int mt;
		cin >> mt;
		std::chrono::steady_clock::time_point bt = std::chrono::steady_clock::now();
		vector<thread*> thr;
		for (set<Place*>::iterator it = AS.sp.begin(); it != AS.sp.end(); ++it)
		{
			(**it).setBaseTime(bt);
			//open file
			(**it).log.open((**it).getName() + ".txt");
			(**it).log << getTimeMillis(bt) << " " << "inits";
			(**it).write();
		}
		for (set<Place*>::iterator it = AS.sp.begin(); it != AS.sp.end(); ++it)
		{
			int startState = ((**it).isWaiting());
			thread *nthr = new thread(checkP_MT, ref(**it), ref(pW), mt, bt, startState);
			thr.push_back(nthr);
			nthr = nullptr;
		}
		//AS
		thread *nthr = new thread(checkAS_MT, ref(AS), ref(pW), mt, bt);
		thr.push_back(nthr);
		nthr = nullptr;
		//cout << getTimeMillis(bt);
		for (vector<thread*>::iterator it = thr.begin(); it != thr.end(); ++it)
			(**it).join();
		for (vector<thread*>::iterator it = thr.begin(); it != thr.end(); ++it)
			delete (*it);
		//all done, merging
		cout << "Modeling stopped. Enter merged log file name: ";
		string mlf;
		cin >> mlf;
		AS.merge(mlf);
	}
	catch (exception &ex)
	{
		cout << ex.what() << endl;
	}
	system("pause");
	return 0;
}