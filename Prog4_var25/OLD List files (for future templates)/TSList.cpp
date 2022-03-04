#include "stdafx.h"
#include "TSList.h"

using namespace std;

namespace P4v25
{
	TSList::TSList(const TSList &b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = new TStats[maxSZ];
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
	}

	TSList::TSList(TSList &&b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = b.list;
		b.list = nullptr;
	}

	TSList &TSList::operator =(const TSList &b)
	{
		TStats *nl = new TStats[b.maxSZ];
		maxSZ = b.maxSZ;
		size = b.size;
		delete[] list;
		list = nl;
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
		return *this;
	}

	TSList &TSList::operator =(TSList &&b)
	{
		maxSZ = b.maxSZ;
		size = b.size;
		list = b.list;
		b.list = nullptr;
		return *this;
	}

	TStats &TSList::operator [](const int &lvl) //getting level parameters
	{
		return list[lvl];
	}

	TStats TSList::operator [](const int &lvl) const //getting level parameters
	{
		return list[lvl];
	}
	
	void TSList::addLevel(const TStats &nts)
	{
		if (size == maxSZ)
		{
			TStats *nl = new TStats[maxSZ + QUOTA];
			for (int i = 0; i < size; ++i)
				nl[i] = list[i];
			delete[] list;
			list = nl;
			maxSZ += QUOTA;
		}
		list[size] = nts;
		++size;
	}
	
	TSit TSList::begin()
	{
		return TSit(list);
	}

	TSit TSList::end()
	{
		return TSit(list + size);//TODO: may not work correctly.
	}
	/*
	C_TSit TSList::begin() const
	{
		return C_TSit(list);
	}

	C_TSit TSList::end() const
	{
		return C_TSit(list + size);//TODO: may not work correctly.
	}
	*/
	//TSit

	TStats &TSit::operator *() const
	{
		if (cur)
			return *cur;
		throw exception("TSit:illegal_pointer");
	}

	TSit &TSit::operator ++()
	{
		++cur;
		return *this;
	}

	TSit TSit::operator ++(int)
	{
		TSit res(cur);
		++cur;
		return res;
	}

	bool TSit::operator !=(const TSit &it) const
	{
		return cur != it.cur;
	}

	bool TSit::operator ==(const TSit &it) const
	{
		return cur == it.cur;
	}
	/*
	//C_TSit

	const TStats &C_TSit::operator *()
	{
		if (cur)
			return *cur;
		throw exception("TSit:illegal_pointer");
	}

	C_TSit &C_TSit::operator ++()
	{
		++cur;
		return *this;
	}

	C_TSit C_TSit::operator ++(int)
	{
		C_TSit res(cur);
		++cur;
		return res;
	}

	bool C_TSit::operator !=(const C_TSit &it) const
	{
		return cur != it.cur;
	}

	bool C_TSit::operator ==(const C_TSit &it) const
	{
		return cur == it.cur;
	}
	*/
}