#include "stdafx.h"
#include "BList.h"

using namespace std;

namespace P4v25
{
	BList::BList(const BList &b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = new Building*[maxSZ];
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
	}

	BList::BList(BList &&b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = b.list;
		b.list = nullptr;
	}

	BList &BList::operator =(const BList &b)
	{
		Building **nl = new Building*[b.maxSZ];
		maxSZ = b.maxSZ;
		size = b.size;
		delete[] list;
		list = nl;
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
		return *this;
	}

	BList &BList::operator =(BList &&b)
	{
		maxSZ = b.maxSZ;
		size = b.size;
		list = b.list;
		b.list = nullptr;
		return *this;
	}

	void BList::addBuilding(Building *b)
	{
		//anti-copy
		for (int i = 0; i < size; i++)
			if (list[i] == b)
				return;
		if (size == maxSZ)
		{
			Building **nl = new Building*[maxSZ + QUOTA];
			for (int i = 0; i < size; ++i)
				nl[i] = list[i];
			delete[] list;
			list = nl;
			maxSZ += QUOTA;
		}
		list[size] = b;
		++size;
	}

	int BList::removeBuilding(Building *b)
	{
		for (int i = 0; i < size; i++)
			if (list[i] == b)
			{
				list[i] = list[size - 1];
				--size;
				return 1;
			}
		return 0; //if not found
	}

	Bit BList::begin()
	{
		return Bit(list);
	}

	Bit BList::end()
	{
		return Bit(list + size);//TODO: may not work correctly.
	}

	/*
	C_Bit BList::begin() const
	{
		return C_Bit(list);
	}

	C_Bit BList::end() const
	{
		return C_Bit(list + size);//TODO: may not work correctly.
	}
	*/

	//Bit

	Building *&Bit::operator *() const
	{
		if (cur)
			return *cur;
		throw exception("Bit:illegal_pointer");
	}

	Bit &Bit::operator ++()
	{
		++cur;
		return *this;
	}

	Bit Bit::operator ++(int)
	{
		Bit res(cur);
		++cur;
		return res;
	}

	bool Bit::operator !=(const Bit &it) const
	{
		return cur != it.cur;
	}

	bool Bit::operator ==(const Bit &it) const
	{
		return cur == it.cur;
	}
	/*
	//C_Bit

	Building const * &C_Bit::operator *()
	{
		if (cur)
			return *cur;
		throw exception("Bit:illegal_pointer");
	}

	C_Bit &C_Bit::operator ++()
	{
		++cur;
		return *this;
	}

	C_Bit C_Bit::operator ++(int)
	{
		C_Bit res(cur);
		++cur;
		return res;
	}

	bool C_Bit::operator !=(const C_Bit &it) const
	{
		return cur != it.cur;
	}

	bool C_Bit::operator ==(const C_Bit &it) const
	{
		return cur == it.cur;
	}
	*/
}