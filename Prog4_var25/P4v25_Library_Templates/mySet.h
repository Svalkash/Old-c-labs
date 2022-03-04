#pragma once

#define QUOTA 5

namespace P4v25_T
{
	template<class T>
	class mySet
	{
	private:
		int maxSZ;
		int sz;
		T *list;
	public:
		mySet() : maxSZ(0), sz(0), list(nullptr) {}
		~mySet() { delete[]list; }
		//
		mySet(const mySet &b) :maxSZ(b.maxSZ), sz(b.sz)
		{
			list = new T[maxSZ];
			for (int i = 0; i < sz; i++)
				list[i] = b.list[i];
		}
		mySet(mySet &&b) :maxSZ(b.maxSZ), sz(b.sz)
		{
			list = b.list;
			b.list = nullptr;
		}
		mySet &operator =(const mySet &b)
		{
			T *nl = new T[b.maxSZ];
			maxSZ = b.maxSZ;
			sz = b.sz;
			delete[] list;
			list = nl;
			for (int i = 0; i < sz; i++)
				list[i] = b.list[i];
			return *this;
		}
		mySet &operator =(mySet &&b)
		{
			maxSZ = b.maxSZ;
			sz = b.sz;
			list = b.list;
			b.list = nullptr;
			return *this;
		}
		//
		int size() const
		{
			return sz;
		}
		void insert(T b)
		{
			//anti-copy
			for (int i = 0; i < sz; i++)
				if (list[i] == b)
					return;
			if (sz == maxSZ)
			{
				T *nl = new T[maxSZ + QUOTA];
				for (int i = 0; i < sz; ++i)
					nl[i] = list[i];
				delete[] list;
				list = nl;
				maxSZ += QUOTA;
			}
			list[sz] = b;
			++sz;
		}
		int erase(T b)
		{
			for (int i = 0; i < sz; i++)
				if (list[i] == b)
				{
					list[i] = list[sz - 1];
					--sz;
					return 1;
				}
			return 0; //if not found
		}
		//
		template <class U1> friend class mSit;
		typedef typename mSit<T> iterator;
		iterator begin()
		{
			return iterator(list);
		}
		iterator end()
		{
			return iterator(list + sz);//TODO: may not work correctly.
		}
		//
		template <class U2> friend class c_mSit;
		typedef typename c_mSit<T> const_iterator;
		const_iterator begin() const
		{
			return const_iterator(list);
		}
		const_iterator end() const
		{
			return const_iterator(list + sz);//TODO: may not work correctly.
		}
	};

	template<class Ti>
	class mSit
	{
	private:
		Ti *cur;
	public:
		mSit(Ti *ob) : cur(ob) {}
		mSit() : cur(nullptr) {}
		Ti &operator *() const
		{
			if (cur)
				return *cur;
			throw exception("mSit:illegal_pointer");
		}
		mSit &operator ++()
		{
			++cur;
			return *this;
		}
		mSit operator ++(int)
		{
			mSit res(cur);
			++cur;
			return res;
		}
		bool operator !=(const mSit &it) const
		{
			return cur != it.cur;
		}
		bool operator ==(const mSit &it) const
		{
			return cur == it.cur;
		}
	};

	template<class Tic>
	class c_mSit
	{
	private:
		Tic *cur;
	public:
		c_mSit(Tic *ob) : cur(ob) {}
		c_mSit() : cur(nullptr) {}
		const Tic &operator *() const
		{
			if (cur)
				return *cur;
			throw exception("c_mSit:illegal_pointer");
		}
		c_mSit &operator ++()
		{
			++cur;
			return *this;
		}
		c_mSit operator ++(int)
		{
			mSit res(cur);
			++cur;
			return res;
		}
		bool operator !=(const c_mSit &it) const
		{
			return cur != it.cur;
		}
		bool operator ==(const c_mSit &it) const
		{
			return cur == it.cur;
		}
	};
}