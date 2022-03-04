#pragma once

#define SZ 8
#define STRMAXLEN 80

#include <iostream>

namespace P3_2 {
	int hToD(const char &);
	char dToH(const int &);

	class Hex
	{
	private:
		//data
		int sign;
		int len;
		char dig[SZ];
		//functions
		Hex &transform();
		void calcLen(); 
		bool checkpkdk();
	public:
		//constr
		Hex();
		Hex(const int &);
		Hex(const char *);
		Hex(const Hex &);
		//io
		friend std::istream &operator >>(std::istream &, Hex &);
		friend std::ostream &operator <<(std::ostream &, const Hex &);
		//operations
		const Hex operator +(const Hex &) const;
		const Hex operator -(const Hex &) const;
		Hex &operator <<=(const int &);
		Hex &operator >>=(const int &);
		//info
		const bool operator ==(const Hex &) const;
		const bool operator <(const Hex &) const;
		const bool operator >(const Hex &) const;
		const bool isEven() const;
		//destr
		//~Hex();
		int getSign() const { return sign; }
		int getLen() const { return len; }
		char getDig(int i) const { return dig[i]; }
	};
}

