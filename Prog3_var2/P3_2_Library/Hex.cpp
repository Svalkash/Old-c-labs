#include "stdafx.h"
#include "Hex.h"

using namespace std;

namespace P3_2 {
	bool Hex::checkpkdk()
	{
		if (sign != 3) return 0;
		for (int i = 0; i<SZ; i++)
		if (dig[i] != '0')
			return 0;
		return 1;
	}

	int hToD(const char &h)
	{
		if ((h >= '0') && (h <= '9')) return (int)h - (int)'0';
		else return (int)h - (int)'A' + 10;
	}

	char dToH(const int &d)
	{
		if ((d >= 0) && (d <= 9)) return (char)(d + (int)'0');
		else return (char)(d - 10 + (int)'A');
	}

	Hex::Hex() :sign(0), len(0)
	{
		for (int i = 0; i < SZ; i++)
			dig[i] = '0';
	}

	Hex::Hex(const int &n)
	{
		//len
		int i, k;
		for (i = 0, k = 1; k <= abs(n); i++, k *= 16)
			;
		len = i;
		if (len > SZ)
			throw exception("too big number!");
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
		if (len < SZ)
			for (int i = len; i < SZ; i++)
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
		if (len > SZ)
			throw exception("too big number");
		for (i = 0; i < len; i++)
			dig[i] = s[len - 1 + sign - i];
		if (len < SZ)
			for (int i = len; i < SZ; i++)
				dig[i] = '0';
		calcLen();
		if ((len == 0) && (sign))
			sign = 0;
	}

	Hex::Hex(const Hex &a)
	{
		sign = a.sign;
		len = a.len;
		for (int i = 0; i < SZ; i++)
			dig[i] = a.dig[i];
	}

	istream &operator >>(std::istream &c, Hex &a)
	{
		char s[STRMAXLEN];
		c >> s;
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
			if (a.len > SZ)
			{
				c.setstate(ios::failbit);
				return c;
			}
			for (i = 0; i < a.len; i++)
				a.dig[i] = s[a.len - 1 + a.sign - i];
			if (a.len < SZ)
				for (int i = a.len; i < SZ; i++)
					a.dig[i] = '0';
			a.calcLen();
			if ((a.len == 0) && (a.sign))
				a.sign = 0;
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
		for (i = SZ - 1; (i >= 0) && (dig[i] == '0'); i--)
			;
		len = i + 1;
	}

	Hex &Hex::transform()
	{
		if (!sign)
			return *this;
		for (int i = 0; i < SZ; i++)
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
		//3:NEED TO FIX ARRAY LENGTH!!!!!!
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
		if ((ml < SZ) && p)
		{
			da.len = ml + 1;
			da.dig[ml] = '1';
			da.sign = (3 * da.sign + 3 * db.sign) % 4; //MDK
		}
		else
		{
			da.calcLen();
			da.sign = (3 * da.sign + 3 * db.sign + p) % 4; //MDK
		}
		//PK
		if (da.checkpkdk())
			da.sign = 2;
		if (da.sign == 1) throw exception("positive overflow");
		if (da.sign == 2) throw exception("negative overflow");
		da.sign /= 3;
		da.transform();
		return da;
	}

	const Hex Hex::operator -(const Hex &b) const
	{
		Hex mb(b);
		if (mb.len != 0)
			mb.sign = !b.sign;
		return (*this+mb);
	}

	Hex &Hex::operator <<=(const int &d)
	{
		if (d < 0)
			throw exception("invalid offset");
		if (d == 0)
			return *this;
		if (len == 0)
			return *this;
		if (len + d > SZ)
			throw exception("too big offset");
		len = len + d;
		for (int i = len - 1; i >= d; i--)
			dig[i] = dig[i - d];
		for (int i = d - 1; i >= 0; i--)
			dig[i] = '0';
		return *this;
	}

	Hex &Hex::operator >>=(const int &d)
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
