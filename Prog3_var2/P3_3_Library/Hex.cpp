#include "stdafx.h"
#include "Hex.h"

using namespace std;

namespace P3_3 {

	int hToD(const char h)
	{
		if ((h >= '0') && (h <= '9')) return (int)h - (int)'0';
		else return (int)h - (int)'A' + 10;
	}

	char dToH(const int d)
	{
		if ((d >= 0) && (d <= 9)) return (char)(d + (int)'0');
		else return (char)(d - 10 + (int)'A');
	}

	void Hex::expand(int nsz)
	{
		char *nd = new char[nsz];
		for (int i = 0; i < sz; i++)
			nd[i] = dig[i];
		for (int i = sz; i < nsz; i++)
			nd[i] = '0';
		delete[] dig;
		sz = nsz;
		dig = nd;
	}

	void Hex::cut()
	{
		int nsz = (len / DSZ + 1) * DSZ;
		char *nd = new char[nsz];
		for (int i = 0; (i < nsz) && (i < sz); i++)
			nd[i] = dig[i];
		for (int i = sz; i < nsz; i++)
			nd[i] = '0';
		delete[] dig;
		sz = nsz;
		dig = nd;
	}

	Hex::Hex() :sign(0), len(0)
	{
		sz = DSZ;
		dig = new char[sz];
		//CATCH LATER
		for (int i = 0; i < sz; i++)
			dig[i] = '0';
	}

	Hex::Hex(const int n)
	{
		//len
		int i, k;
		for (i = 0, k = 1; k <= abs(n); i++, k *= 16)
			;
		len = i;
		sz = (len / DSZ + 1) * DSZ;
		dig = new char[sz];
		//sign
		if (n < 0)
			sign = 1;
		else
			sign = 0;
		//arr
		k = abs(n);
		for (i = 0; i < len;/*k > 0;*/ i++)
		{
			dig[i] = dToH(k % 16);
			k /= 16;
		}
		if (len < sz)
			for (int i = len; i < sz; i++)
				dig[i] = '0';
	}

	Hex::Hex(const char * const s)
	{
		int i;
		//checkstr
		for (int i = 0; i < strlen(s); i++)
			if (!(((s[i] >= '0') && (s[i] <= '9')) || ((s[i] >= 'A') && (s[i] <= 'F')) || ((s[i] == '-') && (i == 0))))
				throw exception("wrong symbol");
		//sign
		if (s[0] == '-')
			sign = 1;
		else
			sign = 0;
		//len
		len = strlen(s) - sign;
		sz = (len / DSZ + 1) * DSZ;
		dig = new char[sz];
		for (i = 0; i < len; i++)
			dig[i] = s[len - 1 + sign - i];
		if (len < sz)
			for (int i = len; i < sz; i++)
				dig[i] = '0';
		calcLen();
		if ((len == 0) && (sign))
			sign = 0;
		cut();
	}

	Hex::Hex(const Hex &a)
	{
		len = a.len;
		sz = a.sz;
		dig = new char[sz];
		sign = a.sign;
		for (int i = 0; i < sz; i++)
			dig[i] = a.dig[i];
	}

	Hex::Hex(Hex &&a)
	{
		dig = a.dig;
		a.dig = nullptr;
		len = a.len;
		sz = a.sz;
		sign = a.sign;
	}


	Hex &Hex::operator =(const Hex &a) //A=A;
	{
		if (this == &a)
			return *this;
		char *ndig = new char[a.sz];
		delete[] dig;
		dig = ndig;
		sz = a.sz;
		len = a.len;
		sign = a.sign;
		for (int i = 0; i < sz; i++)
			dig[i] = a.dig[i];
		return *this;
	}

	Hex &Hex::operator =(Hex &&a)
	{
		if (this == &a)
			return *this;
		delete[] dig;
		dig = a.dig;
		a.dig = nullptr;
		sz = a.sz;
		len = a.len;
		sign = a.sign;
		return *this;
	}

	istream &operator >>(std::istream &c, Hex &a)
	{
		char s[STRMAXLEN];
#if (inmode == 1)
		c >> s;
#endif
#if (inmode == 0)
		c.getline(s, STRMAXLEN - 1);
#endif
		if (c.good())
		{
			int i;
			//checkstr
			for (int i = 0; i < strlen(s); i++)
				if (!(((s[i] >= '0') && (s[i] <= '9')) || ((s[i] >= 'A') && (s[i] <= 'F')) || ((s[i] == '-') && (i == 0))))
				{
					c.setstate(ios::failbit);
					return c;
				}
			//sign
			if (s[0] == '-')
				a.sign = 1;
			else
				a.sign = 0;
			//len
			a.len = strlen(s) - a.sign;
			if (a.len > a.sz)
				a.expand((a.len / DSZ + 1) * DSZ);
			for (i = 0; i < a.len; i++)
				a.dig[i] = s[a.len - 1 + a.sign - i];
			if (a.len < a.sz)
				for (int i = a.len; i < a.sz; i++)
					a.dig[i] = '0';
			a.calcLen();
			if ((a.len == 0) && (a.sign))
				a.sign = 0;
			a.cut();
		}
		return c;
	}

	ostream &operator <<(std::ostream &c, const Hex &b)
	{
		if (b.sign)
			c << '-';
		if (b.len == 0) c << 0;
		for (int i = b.len - 1; i >= 0; i--)
			c << b.dig[i];
		return c;
	}

	void Hex::calcLen()
	{
		int i;
		for (i = sz - 1; (i >= 0) && (dig[i] == '0'); i--)
			;
		len = i + 1;
	}

	Hex &Hex::transform()
	{
		if (!sign)
			return *this;
		for (int i = 0; i < sz; i++)
			dig[i] = dToH(15 - hToD(dig[i]));
		for (int i = 0, p = 1; p; i++)
		{
			p = (hToD(dig[i]) + 1) / 16;
			dig[i] = dToH((hToD(dig[i]) + 1) % 16);
		} //overflow is impossible
		calcLen();
		return *this;
	}

	const Hex Hex::operator +(const Hex &b) const
	{
		Hex da(*this), db(b);
		if (da.sz > db.sz)
			db.expand(da.sz);
		if (da.sz < db.sz)
			da.expand(db.sz);
		da.transform();
		db.transform();
		int p = 0;
		int s;
		int ml = db.len > da.len ? db.len : da.len; //max. len
		for (int i = 0; i < ml; i++)
		{
			s = hToD(da.dig[i]) + hToD(db.dig[i]) + p;
			p = s / 16;
			da.dig[i] = dToH(s % 16);
		}
		if ((ml < da.sz) && p)
		{
			da.len = ml + 1;
			da.dig[ml] = '1';
			da.sign = (da.sign + db.sign) % 2; //MDK
		}
		else
		{
			//da.calcLen();
			da.sign = (da.sign + db.sign + p) % 2; //MDK
			//don't need PK: overflow is impossible
		}
		da.transform();
		da.calcLen();
		da.cut();
		return da;
	}

	const Hex Hex::operator -(const Hex &b) const
	{
		Hex mb(b);
		if (mb.len != 0)
			mb.sign = !b.sign;
		return (*this+mb);
	}

	Hex &Hex::operator <<=(const int d)
	{
		if (d < 0)
			throw exception("invalid offset");
		if (d == 0)
			return *this;
		if (len == 0)
			return *this;
		len = len + d;
		if (len > sz)
			expand((len / DSZ + 1) * DSZ);
		for (int i = len - 1; i >= d; i--)
			dig[i] = dig[i - d];
		for (int i = d - 1; i >= 0; i--)
			dig[i] = '0';
		return *this;
	}

	Hex &Hex::operator >>=(const int d)
	{
		if (d < 0)
			throw exception("invalid offset");
		if (len == 0)
			return *this;
		if (d == 0)
			return *this;
		for (int i = 0; i < len - d; i++)
			dig[i] = dig[i + d];
		for (int i = len - 1; (i >= len - d) && (i >= 0); i--)
			dig[i] = '0';
		len -= d;
		if (len < 0)
			len = 0;
		cut();
		return *this;
	}

	const bool Hex::operator ==(const Hex &b) const
	{
		if (sign != b.sign)
			return 0;
		if (len != b.len)
			return 0;
		for (int i = len - 1; i >= 0; i--)
			if (dig[i] != b.dig[i])
				return 0;
		return 1;
	}
	
	const bool Hex::operator >(const Hex &b) const
	{
		if ((!sign) && (b.sign))
			return 1;
		if (sign)
		{
			if (len < b.len)
				return 1;
			for (int i = len - 1; i >= 0; i--)
			{
				if (dig[i] < b.dig[i])
					return 1;
				if (dig[i] > b.dig[i])
					return 0;
			}
			return 0;
		}
		else
		{
			if (len > b.len)
				return 1;
			for (int i = len - 1; i >= 0; i--)
			{
				if (dig[i] > b.dig[i])
					return 1;
				if (dig[i] < b.dig[i])
					return 0;
			}
			return 0;
		}
	}

	const bool Hex::operator <(const Hex &b) const
	{
		return b>*this;
	}

	const bool Hex::isEven() const
	{
		return !(hToD(dig[0]) % 2);
	}

}
