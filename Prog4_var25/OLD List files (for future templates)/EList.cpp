#include "stdafx.h"
#include "EList.h"

using namespace std;

namespace P4v25
{
	EList::EList(const EList &b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = new Enemy*[maxSZ];
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
	}

	EList::EList(EList &&b) :maxSZ(b.maxSZ), size(b.size)
	{
		list = b.list;
		b.list = nullptr;
	}

	EList &EList::operator =(const EList &b)
	{
		Enemy **nl = new Enemy*[b.maxSZ];
		maxSZ = b.maxSZ;
		size = b.size;
		delete[] list;
		list = nl;
		for (int i = 0; i < size; i++)
			list[i] = b.list[i];
		return *this;
	}

	EList &EList::operator =(EList &&b)
	{
		maxSZ = b.maxSZ;
		size = b.size;
		list = b.list;
		b.list = nullptr;
		return *this;
	}

	void EList::addEnemy(Enemy *ne)
	{
		//anti-copy
		for (int i = 0; i < size; i++)
			if (list[i] == ne)
				return;
		if (size == maxSZ)
		{
			Enemy **nl = new Enemy*[maxSZ + QUOTA];
			for (int i = 0; i < size; ++i)
				nl[i] = list[i];
			delete[] list;
			list = nl;
			maxSZ += QUOTA;
		}
		list[size] = ne;
		++size;
	}

	int EList::removeEnemy(Enemy *ne)
	{
		for (int i = 0; i < size; i++)
			if (list[i] == ne)
			{
				list[i] = list[size - 1];
				--size;
				return 1;
			}
		return 0; //if not found
	}

	Eit EList::begin()
	{
		return Eit(list);
	}

	Eit EList::end()
	{
		return Eit(list + size);//TODO: may not work correctly.
	}

	/*
	C_Eit Level::begin() const
	{
	return C_Eit(EList);
	}

	C_Eit Level::end() const
	{
	return C_Eit(EList + sizeE);//TODO: may not work correctly.
	}
	*/

	//Eit

	Enemy *&Eit::operator *() const
	{
		if (cur)
			return *cur;
		throw exception("Eit:illegal_pointer");
	}

	Eit &Eit::operator ++()
	{
		++cur;
		return *this;
	}

	Eit Eit::operator ++(int)
	{
		Eit res(cur);
		++cur;
		return res;
	}

	bool Eit::operator !=(const Eit &it) const
	{
		return cur != it.cur;
	}

	bool Eit::operator ==(const Eit &it) const
	{
		return cur == it.cur;
	}
	/*
	//C_Eit

	Enemy const * &C_Eit::operator *()
	{
	if (cur)
	return *cur;
	throw exception("Eit:illegal_pointer");
	}

	C_Eit &C_Eit::operator ++()
	{
	++cur;
	return *this;
	}

	C_Eit C_Eit::operator ++(int)
	{
	C_Eit res(cur);
	++cur;
	return res;
	}

	bool C_Eit::operator !=(const C_Eit &it) const
	{
	return cur != it.cur;
	}

	bool C_Eit::operator ==(const C_Eit &it) const
	{
	return cur == it.cur;
	}
	*/
}