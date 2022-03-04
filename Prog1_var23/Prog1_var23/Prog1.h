#ifndef PROG1_H
#define PROG1_H

#define trash 0 //trash number
#define useAlternativeOutput 1

struct Item {
	int pos;
	int num;
	Item *next;
};

struct Line {
	int n;
	Item *head;
};

struct Matrix {
	int m;
	Line *matr;
};

int mInput(Matrix &);
Matrix mCut(const Matrix &);
void mOutput(const Matrix &);
void mOutput_alt(const Matrix &);
void mDelete(Matrix &);

#endif