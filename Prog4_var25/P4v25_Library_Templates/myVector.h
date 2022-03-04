#pragma once

#define QUOTA 5

namespace P4v25_T
{
	template<class T>
	class myVector
	{
	private:
		int maxSZ;
		int sz;
		T *list;
	public:
		myVector() : maxSZ(0), sz(0), list(nullptr) {}
		~myVector() { delete[] list; }
		//
		myVector(const myVector &b) :maxSZ(b.maxSZ), sz(b.sz)
		{
			list = new T[maxSZ];
			for (int i = 0; i < sz; i++)
				list[i] = b.list[i];
		}
		myVector(myVector &&b) :maxSZ(b.maxSZ), sz(b.sz)
		{
			list = b.list;
			b.list = nullptr;
		}
		myVector &operator =(const myVector &b)
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
		myVector &operator =(myVector &&b)
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
		T &operator [](const int &index) //getting level parameters
		{
			return list[index];
		}
		T operator [](const int &index) const //getting level parameters
		{
			return list[index];
		}
		void push_back(const T &nts)
		{
			if (sz == maxSZ)
			{
				T *nl = new T[maxSZ + QUOTA];
				for (int i = 0; i < sz; ++i)
					nl[i] = list[i];
				delete[] list;
				list = nl;
				maxSZ += QUOTA;
			}
			list[sz] = nts;
			++sz;
		}
		//
		template <class U1> friend class mVit;
		typedef typename mVit<T> iterator;
		iterator begin()
		{
			return mVit<T>(list);
		}
		iterator end()
		{
			return iterator(list + sz);//TODO: may not work correctly.
		}
		//
		template <class U2> friend class c_mVit;
		typedef typename c_mVit<T> const_iterator;
		const_iterator begin() const
		{
			return iterator(list);
		}
		const_iterator end() const
		{
			return iterator(list + sz);//TODO: may not work correctly.
		}
	};

	template<class Ti>
	class mVit
	{
	private:
		Ti *cur;
	public:
		mVit(Ti *ob) : cur(ob) {}
		mVit() : cur(nullptr) {}
		Ti &operator *() const
		{
			if (cur)
				return *cur;
			throw exception("mVit:illegal_pointer");
		}
		mVit &operator ++()
		{
			++cur;
			return *this;
		}
		mVit operator ++(int)
		{
			mVit res(cur);
			++cur;
			return res;
		}
		bool operator !=(const mVit &it) const
		{
			return cur != it.cur;
		}
		bool operator ==(const mVit &it) const
		{
			return cur == it.cur;
		}
	};

	template<class Tic>
	class c_mVit
	{
	private:
		Tic *cur;
	public:
		c_mVit(Tic *ob) : cur(ob) {}
		c_mVit() : cur(nullptr) {}
		const Tic &operator *() const
		{
			if (cur)
				return *cur;
			throw exception("c_mVit:illegal_pointer");
		}
		c_mVit &operator ++()
		{
			++cur;
			return *this;
		}
		c_mVit operator ++(int)
		{
			c_mVit res(cur);
			++cur;
			return res;
		}
		bool operator !=(const c_mVit &it) const
		{
			return cur != it.cur;
		}
		bool operator ==(const c_mVit &it) const
		{
			return cur == it.cur;
		}
	};
}