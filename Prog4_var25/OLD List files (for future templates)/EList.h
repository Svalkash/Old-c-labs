#pragma once

#include "Enemy.h"

namespace P4v25
{
	class EList
	{
	private:
		Enemy **list;
		int size;
		int maxSZ;
	public:
		EList() : maxSZ(0), size(0), list(nullptr) {}
		~EList() { delete[] list; } //other MUST be in ~Level()..
		//
		EList(const EList &);
		EList(EList &&);
		EList &operator =(const EList &);
		EList &operator =(EList &&);
		//
		int getSize() { return size; }
		//
		void addEnemy(Enemy *); //if element is already here, then we just don't insert it. Nu a hule.
		int removeEnemy(Enemy *); //MOVES THE LAST ELEMENT TO THE PLACE OF THE DELETED! When iterating, check "deleted" place AGAIN!!! returns 1 if removed, 0 if not found.
		friend class Eit;
		typedef Eit iterator;
		iterator begin();
		iterator end();
		/*
		friend class C_Eit;
		typedef C_Eit Constiterator;
		Constiterator begin() const;
		Constiterator end() const;
		*/
	};

	class Eit
	{
	private:
		Enemy **cur;
	public:
		Eit(Enemy **ob) : cur(ob) {}
		Eit() : cur(nullptr) {}
		Enemy *&operator *() const;
		Eit &operator ++();
		Eit operator ++(int);
		bool operator !=(const Eit &) const;
		bool operator ==(const Eit &) const;
	};
	/*
	class C_Eit
	{
	private:
	Enemy const * *cur;
	public:
	C_Eit(Enemy const * *ob) : cur(ob) {}
	C_Eit() : cur(nullptr) {}
	Enemy const * &operator *();
	C_Eit &operator ++();
	C_Eit operator ++(int);
	bool operator !=(const C_Eit &) const;
	bool operator ==(const C_Eit &) const;
	};
	*/
}
