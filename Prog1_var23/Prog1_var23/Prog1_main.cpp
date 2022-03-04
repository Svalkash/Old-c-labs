#include <iostream>

#include "myIO.h"
#include "Prog1.h"

using namespace std;

int main()
{
	Matrix m;
	int rc = mInput(m);
	if (rc == -1) {
		cout << "CRITICAL ERROR: Bad IOstream! Program is stopped." << endl;
		cin.get();
		return -1;
	}
	if (rc == -2) {
		cout << "CRITICAL ERROR: Memory allocation error! Program is stopped." << endl;
		cin.get();
		return -2;
	}
	Matrix n = mCut(m);
	if (useAlternativeOutput) {
		cout << "Initial matrix:" << endl;
		mOutput_alt(m);
		cout << "Resultant matrix:" << endl;
		mOutput_alt(n);
	}
	else {
		cout << "Initial matrix:" << endl;
		mOutput(m);
		cout << "Resultant matrix:" << endl;
		mOutput(n);
	}
	mDelete(m);
	mDelete(n);
	system("pause");
	return 0;
}


