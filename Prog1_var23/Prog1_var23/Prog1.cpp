#include <iostream>

#include "myIO.h"
#include "Prog1.h"

using namespace std;

int listInput(Line *);
int listInsert(Line *, int, int);
void listDelete(Item *);
int findMaxPos(Line *);
int findTrashPos(Line *);
Item *listCut(Item *, int);
void listOutput(Item *);
void listOutput_alt(Line *);
void printer(int);

int mInput(Matrix &a)
{
	int m, n;
	a.matr = nullptr;
	//params
	cout << "Enter the number of lines: ";
	if (getNat(m) < 0)
		return -1;
	cout << "Enter the number of columns: ";
	if (getNat(n) < 0)
		return -1;
	//matrix creation
	a.m = m;
	try {
		a.matr = new Line[m];
	}
	catch (bad_alloc &ba) {
		cout << "Memory allocation error" << ba.what() << endl;
		return -2;
	}
	Line *ptr = a.matr;
	int i = 0;
	while (i < a.m) {
		cout << "LINE " << i << endl;
		ptr->n = n;
		ptr->head = nullptr;
		if (listInput(ptr) < 0) {
			mDelete(a);
			return -1;
		}
		ptr++;
		i++;
	}
	return 0;
}

int listInput(Line *ptr)
{
	ptr->head = nullptr;
	int place = 0, data, res;
	do {
		cout << "Enter item position; enter -1 to stop: ";
		if (getNum(place) < 0)
			return -1;
		if (place < 0)
			break;
		cout << "Enter data: ";
		if (getNum(data) < 0)
			return -1;
		res = listInsert(ptr, place, data);
		switch (res) {
		case -1:
			cout << "ERROR: too big position!" << endl;
			break;
		case -2:
			cout << "ERROR: duplicate position!" << endl;
			break;
		default:
			cout << "Done!" << endl;
			break;
		}
	} while (place >= 0);
	return 0;
}

int listInsert(Line *lp, int place, int data)
{
	if (data == trash) return 0;
	if (place >= lp->n) return -1; //toobig
	Item *ptr = lp->head;
	if ((!lp->head) || (place < lp->head->pos)) {
		Item *ni = new Item;
		ni->num = data;
		ni->pos = place;
		ni->next = lp->head;
		lp->head = ni;
		return 0;
	}
	while ((ptr->next) && (ptr->next->pos <= place))
		ptr = ptr->next;
	if (ptr->pos == place) return -2; //copy
	Item *ni = new Item;
	ni->num = data;
	ni->pos = place;
	ni->next = ptr->next;
	ptr->next = ni;
	return 0;
}

Matrix mCut(const Matrix &a)
{
	Matrix b;
	b.m = a.m;
	b.matr = new Line[b.m];
	Line *aptr = a.matr;
	Line *bptr = b.matr;
	int cutPos;
	int i = 0;
	while (i < a.m) {
		cutPos = findMaxPos(aptr);
		bptr->head = listCut(aptr->head, cutPos);
		bptr->n = cutPos + 1;
		aptr++;
		bptr++;
		i++;
	}
	return b;
}

Item *listCut(Item *ahead, int cutPos)
{
	if (!ahead) return nullptr;
	if (ahead->pos > cutPos) return nullptr;
	Item *bhead = new Item;
	bhead->num = ahead->num;
	bhead->pos = ahead->pos;
	bhead->next = nullptr;
	Item *aptr = ahead, *bptr = bhead, *bnext = nullptr;
	while ((aptr->next) && (aptr->next->pos <= cutPos)) {
		bnext = new Item;
		bnext->num = aptr->next->num;
		bnext->pos = aptr->next->pos;
		bnext->next = nullptr;
		bptr->next = bnext;
		aptr = aptr->next;
		bptr = bptr->next;
	}
	return bhead;
}

int findMaxPos(Line *pl)
{
	Item *head = pl->head;
	if (!head) return 0;
	Item *ptr = head;
	int max = head->num, maxPos = head->pos, trashPos = -1;
	while (ptr) {
		if (ptr->num > max) {
			max = ptr->num;
			maxPos = ptr->pos;
		}
		ptr = ptr->next;
	}
	if (max <= trash) {
		trashPos = findTrashPos(pl);
		if ((trashPos == -1) || ((max == trash) && (maxPos < trashPos))) return maxPos;
		else return trashPos;
	}
	return maxPos;
}

int findTrashPos(Line *pl)
{
	Item *head = pl->head;
	if (!head) return 0;
	if (head->pos > 0) return 0;
	int prev = head->pos;
	Item *ptr = head;
	while (ptr)
	{
		if (ptr->pos - prev > 1) return (prev + 1);
		prev = ptr->pos;
		ptr = ptr->next;
	}
	if (prev < pl->n - 1) return prev + 1;
	return -1;
}

void mOutput(const Matrix &a)
{
	cout << "Number of lines: " << a.m << "." << endl;
	cout << "Trash = " << trash << "." << endl;
	cout << "Element: (position | data)." << endl;
	int i = 0;
	Line *ptr = a.matr;
	while (i < a.m) {
		cout << "________________________________________________________________________________" << endl;
		cout << "LINE " << i << "." << endl;
		cout << "Length: " << ptr->n << "." << endl;
		listOutput(ptr->head);
		ptr++;
		i++;
	}
}

void listOutput(Item *head)
{
	if (!head) {
		cout << "Only trash." << endl;
		return;
	}
	Item *ptr = head;
	while (ptr) {
		cout << "(" << ptr->pos << " | " << ptr->num << ")  ";
		ptr = ptr->next;
	}
	cout << endl;
}

void mOutput_alt(const Matrix &a)
{
	cout << "Number of lines: " << a.m << "." << endl;
	cout << "Trash = " << trash << "." << endl;
	int i = 0;
	Line *ptr = a.matr;
	while (i < a.m) {
		listOutput_alt(ptr);
		ptr++;
		i++;
	}
}

void listOutput_alt(Line *pl)
{
	if (!pl->head) {
		printer(pl->n);
		cout << endl;
		return;
	}
	Item *ptr = pl->head;
	int prev = -1;
	while (ptr) {
		printer(ptr->pos - prev - 1);
		cout << ptr->num << " ";
		prev = ptr->pos;
		ptr = ptr->next;
	}
	printer(pl->n - prev - 1);
	cout << endl;
}

void printer(int k)
{
	for (int i = 0; i < k; i++)
		cout << trash << " ";
}

void listDelete(Item *ptr)
{
	if (ptr) {
		listDelete(ptr->next);
		delete ptr;
	}
}

void mDelete(Matrix &a)
{
	Line *ptr = a.matr;
	int i = 0;
	while (i < a.m) {
		listDelete(ptr->head);
		ptr++;
		i++;
	}
	delete[]a.matr;
}