#include "stdafx.h"
#include "SEList.h"

using namespace std;

namespace P4v25
{
	SEList::SEList(const SEList &b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = new SEItem[maxSZ];
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
	}

	SEList::SEList(SEList &&b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = b.list;
		b.list = nullptr;
	}

	SEList &SEList::operator =(const SEList &b)
	{
		SEItem*nl = new SEItem[b.maxSZ];
		maxSZ = b.maxSZ;
		size = b.size;
		delete[] list;
		list = nl;
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
		return *this;
	}

	SEList &SEList::operator =(SEList &&b)
	{
		maxSZ = b.maxSZ;
		size = b.size;
		list = b.list;
		b.list = nullptr;
		return *this;
	}

	void SEList::addSItem(const SEItem &se)
	{
		if (size == maxSZ)
		{
			SEItem *nl = new SEItem[maxSZ + QUOTA];
			for (int i = 0; i < size; ++i)
				nl[i] = list[i];
			delete[] list;
			list = nl;
			maxSZ += QUOTA;
		}
		list[size] = se;
		++size;
	}

	void SEList::removeSItem(const iterator cit)
	{
		*(cit) = list[size - 1];
		--size;
	}

	SEit SEList::begin()
	{
		return SEit(list);
	}

	SEit SEList::end()
	{
		return SEit(list + size);//TODO: may not work correctly.
	}
	/*
	C_SEit SEList::begin() const
	{
	return C_SEit(list);
	}

	C_SEit SEList::end() const
	{
	return C_SEit(list + size);//TODO: may not work correctly.
	}
	*/

	//SEit

	SEItem &SEit::operator *() const
	{
		if (cur)
			return *cur;
		throw exception("SEit:illegal_pointer");
	}

	SEit &SEit::operator ++()
	{
		++cur;
		return *this;
	}

	SEit SEit::operator ++(int)
	{
		SEit res(cur);
		++cur;
		return res;
	}

	bool SEit::operator !=(const SEit &it) const
	{
		return cur != it.cur;
	}

	bool SEit::operator ==(const SEit &it) const
	{
		return cur == it.cur;
	}
	/*
	//C_SEit

	const SEItem &C_SEit::operator *()
	{
	if (cur)
	return *cur;
	throw exception("SEit:illegal_pointer");
	}

	C_SEit &C_SEit::operator ++()
	{
	++cur;
	return *this;
	}

	C_SEit C_SEit::operator ++(int)
	{
	C_SEit res(cur);
	++cur;
	return res;
	}

	bool C_SEit::operator !=(const C_SEit &it) const
	{
	return cur != it.cur;
	}

	bool C_SEit::operator ==(const C_SEit &it) const
	{
	return cur == it.cur;
	}
	*/
}