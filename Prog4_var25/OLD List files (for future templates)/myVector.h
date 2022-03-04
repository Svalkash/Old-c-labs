#pragma once

#include "Common.h"
#include "B_Castle.h"

namespace P4v25
{
	class CSList
	{
	private:
		int maxSZ;
		int size;
		CStats * list;
	public:
		CSList() : maxSZ(0), size(0), list(nullptr) {}
		~CSList() { delete[]list; }
		//
		CSList(const CSList &);
		CSList(CSList &&);
		CSList &operator =(const CSList &);
		CSList &operator =(CSList &&);
		//
		int getSize() const { return size; }
		CStats &operator [](const int &); //getting level parameters
		CStats operator [](const int &) const; //getting level parameters
		void addLevel(const CStats &); //adds new level. Doesn't check anything.
		//
		friend class CSit;
		typedef CSit iterator;
		iterator begin();
		iterator end();
		/*
		friend class C_CSit;
		typedef C_CSit Constiterator;
		Constiterator begin() const;
		Constiterator end() const;
		*/
	};

	class CSit
	{
	private:
		CStats *cur;
	public:
		CSit(CStats *ob) : cur(ob) {}
		CSit() : cur(nullptr) {}
		CStats &operator *() const;
		CSit &operator ++();
		CSit operator ++(int);
		bool operator !=(const CSit &) const;
		bool operator ==(const CSit &) const;
	};
	/*
	class C_CSit
	{
	private:
		const CStats *cur;
	public:
		C_CSit(const CStats *ob) : cur(ob) {}
		C_CSit() : cur(nullptr) {}
		const CStats &operator *();
		C_CSit &operator ++();
		C_CSit operator ++(int);
		bool operator !=(const C_CSit &) const;
		bool operator ==(const C_CSit &) const;
	};
	*/
}
