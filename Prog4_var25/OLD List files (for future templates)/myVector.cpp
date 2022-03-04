#include "stdafx.h"
#include "CSList.h"

using namespace std;

namespace P4v25
{
	CSList::CSList(const CSList &b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = new CStats[maxSZ];
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
	}

	CSList::CSList(CSList &&b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = b.list;
		b.list = nullptr;
	}

	CSList &CSList::operator =(const CSList &b)
	{
		CStats *nl = new CStats[b.maxSZ];
		maxSZ = b.maxSZ;
		size = b.size;
		delete[] list;
		list = nl;
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
		return *this;
	}

	CSList &CSList::operator =(CSList &&b)
	{
		maxSZ = b.maxSZ;
		size = b.size;
		list = b.list;
		b.list = nullptr;
		return *this;
	}

	CStats &CSList::operator [](const int &lvl) //getting level parameters
	{
		return list[lvl];
	}

	CStats CSList::operator [](const int &lvl) const //getting level parameters
	{
		return list[lvl];
	}

	void CSList::addLevel(const CStats &nts)
	{
		if (size == maxSZ)
		{
			CStats *nl = new CStats[maxSZ + QUOTA];
			for (int i = 0; i < size; ++i)
				nl[i] = list[i];
			delete[] list;
			list = nl;
			maxSZ += QUOTA;
		}
		list[size] = nts;
		++size;
	}

	CSit CSList::begin()
	{
		return CSit(list);
	}

	CSit CSList::end()
	{
		return CSit(list + size);//TODO: may not work correctly.
	}
	/*
	C_CSit CSList::begin() const
	{
		return C_CSit(list);
	}

	C_CSit CSList::end() const
	{
		return C_CSit(list + size);//TODO: may not work correctly.
	}
	*/

	//CSit

	CStats &CSit::operator *() const
	{
		if (cur)
			return *cur;
		throw exception("CSit:illegal_pointer");
	}
	CSit &CSit::operator ++()
	{
		++cur;
		return *this;
	}

	CSit CSit::operator ++(int)
	{
		CSit res(cur);
		++cur;
		return res;
	}

	bool CSit::operator !=(const CSit &it) const
	{
		return cur != it.cur;
	}

	bool CSit::operator ==(const CSit &it) const
	{
		return cur == it.cur;
	}
	/*
	//C_CSit

	const CStats &C_CSit::operator *()
	{
		if (cur)
			return *cur;
		throw exception("CSit:illegal_pointer");
	}

	C_CSit &C_CSit::operator ++()
	{
		++cur;
		return *this;
	}

	C_CSit C_CSit::operator ++(int)
	{
		C_CSit res(cur);
		++cur;
		return res;
	}

	bool C_CSit::operator !=(const C_CSit &it) const
	{
		return cur != it.cur;
	}

	bool C_CSit::operator ==(const C_CSit &it) const
	{
		return cur == it.cur;
	}
	*/
}