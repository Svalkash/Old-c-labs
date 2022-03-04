#pragma once

#include "Enemy.h"

namespace P4v25
{
	struct SEItem
	{
		Enemy *e;
		double time; //moment for releasing
		//
		SEItem() : e(nullptr), time(0) {}
		SEItem(Enemy *pe, const double &t)
		{
			if (!pe)
				throw std::exception("SEItem:illegal_pointer");
			if (t < 0)
				throw std::exception("SEItem:illegal_time");
			time = t;
			e = pe; //working right with heroes?
		}
	};

	class SEList
	{
	private:
		int maxSZ;
		int size;
		SEItem *list;
	public:
		//
		friend class SEit;
		typedef SEit iterator;
		iterator begin();
		iterator end();
		//
		SEList() : maxSZ(0), size(0), list(nullptr) {}
		~SEList() { delete list; } //other MUST be in ~Level()..
		//
		SEList(const SEList &);
		SEList(SEList &&);
		SEList &operator =(const SEList &);
		SEList &operator =(SEList &&);
		//
		int getSize() const { return size; }
		void addSItem(const SEItem &);
		void removeSItem(const iterator);
		/*
		friend class C_SEit;
		typedef C_SEit Constiterator;
		Constiterator begin() const;
		Constiterator end() const;
		*/
	};

	class SEit
	{
	private:
		SEItem *cur;
	public:
		SEit(SEItem *ob) : cur(ob) {}
		SEit() : cur(nullptr) {}
		SEItem &operator *() const;
		SEit &operator ++();
		SEit operator ++(int);
		bool operator !=(const SEit &) const;
		bool operator ==(const SEit &) const;
	};
	/*
	class C_SEit
	{
	private:
	SEItem *cur;
	public:
	C_SEit(SEItem *ob) : cur(ob) {}
	C_SEit() : cur(nullptr) {}
	const SEItem &operator *();
	C_SEit &operator ++();
	C_SEit operator ++(int);
	bool operator !=(const C_SEit &) const;
	bool operator ==(const C_SEit &) const;
	};
	*/
}




