#pragma once

#include "Building.h"

namespace P4v25
{
	class BList
	{
	private:
		int maxSZ;
		int size;
		Building **list;
	public:
		BList() : maxSZ(0), size(0), list(nullptr) {}
		~BList() { delete[]list; }
		//
		BList(const BList &);
		BList(BList &&);
		BList &operator =(const BList &);
		BList &operator =(BList &&);
		//
		int getSize() const { return size; }
		void addBuilding(Building *); //if element is already here, then we just don't insert it. Nu a hule.
		int removeBuilding(Building *); //MOVES THE LAST ELEMENT TO THE PLACE OF THE DELETED! When iterating, check "deleted" place AGAIN!!! returns 1 if removed, 0 if not found.
		//
		friend class Bit;
		typedef Bit iterator;
		iterator begin();
		iterator end();
		/*
		friend class C_Bit;
		typedef C_Bit Constiterator;
		Constiterator begin() const;
		Constiterator end() const;
		*/
	};

	class Bit
	{
	private:
		Building **cur;
	public:
		Bit(Building **ob) : cur(ob) {}
		Bit() : cur(nullptr) {}
		Building *&operator *() const;
		Bit &operator ++();
		Bit operator ++(int);
		bool operator !=(const Bit &) const;
		bool operator ==(const Bit &) const;
	};
	/*
	class C_Bit
	{
	private:
		Building const * *cur;
	public:
		C_Bit(Building const * *ob) { cur = ob; }
		C_Bit() : cur(nullptr) {}
		Building const * &operator *();
		C_Bit &operator ++();
		C_Bit operator ++(int);
		bool operator !=(const C_Bit &) const;
		bool operator ==(const C_Bit &) const;
	};
	*/
}