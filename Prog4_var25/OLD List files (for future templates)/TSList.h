#pragma once

#include "B_Tower.h"

namespace P4v25
{
	class TSList
	{
	private:
		int maxSZ;
		int size; // = maximum level
		TStats *list; //list is already sorted, list[i].lvl == i+1
	public:
		TSList() :maxSZ(0), size(0), list(nullptr) {}
		~TSList() { delete[]list; }
		//
		TSList(const TSList &);
		TSList(TSList &&);
		TSList &operator =(const TSList &);
		TSList &operator =(TSList &&);
		//
		int getSize() const { return size; }
		TStats &operator [](const int &); //getting level parameters
		TStats operator [](const int &) const; //getting level parameters
		void addLevel(const TStats &); //adds new level. Doesn't check anything.
		//
		friend class TSit;
		typedef TSit iterator;
		iterator begin();
		iterator end();
		/*
		friend class C_TSit;
		typedef C_TSit Constiterator;
		Constiterator begin() const;
		Constiterator end() const;
		*/
	};

	class TSit
	{
	private:
		TStats *cur;
	public:
		TSit(TStats *ob) : cur(ob) {}
		TSit() : cur(nullptr) {}
		TStats &operator *() const;
		TSit &operator ++();
		TSit operator ++(int);
		bool operator !=(const TSit &) const;
		bool operator ==(const TSit &) const;
	};
	/*
	class C_TSit
	{
	private:
		const TStats *cur;
	public:
		C_TSit(const TStats *ob) : cur(ob) {}
		C_TSit() : cur(nullptr) {}
		const TStats &operator *();
		C_TSit &operator ++();
		C_TSit operator ++(int);
		bool operator !=(const C_TSit &) const;
		bool operator ==(const C_TSit &) const;
	};
	*/
}