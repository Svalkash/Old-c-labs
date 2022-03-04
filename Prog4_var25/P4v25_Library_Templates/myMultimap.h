#pragma once

#define QUOTA 5

namespace P4v25_T
{
	template<class T1, class T2>
	struct myPair
	{
		T1 first;
		T2 second;
		//
		myPair() : first(), second() {}
		myPair(const T1 &f, const T2 &s) : first(f), second(s) {}
		myPair &operator =(const myPair &b)
		{
			first = b.first;
			second = b.second;
			return *this;
		}
		bool operator ==(const myPair &b) const
		{
			return ((first == b.first) && (second == b.second));
		}
	};

	template<class Key, class T>
	class myMultimap
	{
	private:
		int maxSZ;
		int sz;
		myPair<const Key, T> *list;
	public:
		myMultimap() : maxSZ(0), sz(0), list(nullptr) {}
		~myMultimap() { delete list; } //other MUST be in ~Level()..
		//
		template <class U1, class V1> friend class mMit;
		typedef typename mMit<Key, T> iterator;
		iterator begin()
		{
			return iterator(list);
		}
		iterator end()
		{
			return iterator(list + sz);//TODO: may not work correctly.
		}
		//
		template <class U2, class V2> friend class c_mMit;
		typedef typename c_mMit<Key, T> const_iterator;
		const_iterator begin() const
		{
			return const_iterator(list);
		}
		const_iterator end() const
		{
			return const_iterator(list + sz);//TODO: may not work correctly.
		}
		//
		myMultimap(const myMultimap &b) :maxSZ(b.maxSZ), sz(b.sz)
		{
			list = new myPair<const Key, T>[maxSZ];
			for (int i = 0; i < sz; i++)
				list[i] = b.list[i];
		}
		myMultimap(myMultimap &&b) :maxSZ(b.maxSZ), sz(b.sz)
		{
			list = b.list;
			b.list = nullptr;
		}
		myMultimap &operator =(const myMultimap &b)
		{
			myPair<const Key, T>*nl = new myPair<const Key, T>[b.maxSZ];
			maxSZ = b.maxSZ;
			sz = b.sz;
			delete[] list;
			list = nl;
			for (int i = 0; i < sz; i++)
				list[i] = b.list[i];
			return *this;
		}
		myMultimap &operator =(myMultimap &&b)
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
		void insert(const myPair<const Key, T> &se)
		{
			if (sz == maxSZ)
			{
				myPair<Key, T> *nl = new myPair<const Key, T>[maxSZ + QUOTA];
				for (int i = 0; i < sz; ++i)
					nl[i] = list[i];
				delete[] list;
				list = nl;
				maxSZ += QUOTA;
			}
			list[sz] = se;
			++sz;
		}
		void erase(const iterator cit)
		{
			*(cit) = list[sz - 1];
			--sz;
		}
	};

	template<class Keyi, class Ti>
	class mMit
	{
	private:
		myPair<const Keyi, Ti> *cur;
	public:
		mMit(myPair<const Keyi, Ti> *ob) : cur(ob) {}
		mMit() : cur(nullptr) {}
		myPair<const Keyi, Ti> &operator *() const
		{
			if (cur)
				return *cur;
			throw std::exception("mMit:illegal_pointer");
		}
		mMit &operator ++()
		{
			++cur;
			return *this;
		}
		mMit operator ++(int)
		{
			mMit res(cur);
			++cur;
			return res;
		}
		bool operator !=(const mMit &it) const
		{
			return cur != it.cur;
		}
		bool operator ==(const mMit &it) const
		{
			return cur == it.cur;
		}
	};

	template<class Keyic, class Tic>
	class c_mMit
	{
	private:
		myPair<const Keyic, Tic> *cur;
	public:
		c_mMit(myPair<const Keyic, Tic> *ob) : cur(ob) {}
		c_mMit() : cur(nullptr) {}
		const myPair<const Keyic, Tic> &operator *() const
		{
			if (cur)
				return *cur;
			throw std::exception("c_mMit:illegal_pointer");
		}
		c_mMit &operator ++()
		{
			++cur;
			return *this;
		}
		c_mMit operator ++(int)
		{
			c_mMit res(cur);
			++cur;
			return res;
		}
		bool operator !=(const c_mMit &it) const
		{
			return cur != it.cur;
		}
		bool operator ==(const c_mMit &it) const
		{
			return cur == it.cur;
		}
	};
}