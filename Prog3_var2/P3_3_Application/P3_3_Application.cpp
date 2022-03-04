// P3_3_Application.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\P3_3_Library\Hex.h"

#define LINE "________________________________________________________________________________"

using namespace P3_3;
using namespace std;

/*
OPERATIONS:
0 wrong
1 +
2 -
3 <<
4 >>
5 <
6 >
7 =
*/
int selector(char *op)
{
	if (!strcmp(op, "+"))
		return 1;
	if (!strcmp(op, "-"))
		return 2;
	if (!strcmp(op, "<<"))
		return 3;
	if (!strcmp(op, ">>"))
		return 4;
	if (!strcmp(op, "<"))
		return 5;
	if (!strcmp(op, ">"))
		return 6;
	if (!strcmp(op, "="))
		return 7;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Welcome!" << endl;
	cout << "Use this format: A op B" << endl;
	cout << "Supported operations: addition(+), subtraction(-), shift(>>/<<), comparison (</>/=)." << endl;
	cout << "Enter Ctrl + Z to stop." << endl;
	Hex a, b;
	cin >> a;
	cout << a << endl;
	return 0;
}

