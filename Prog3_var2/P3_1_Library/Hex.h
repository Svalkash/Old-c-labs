#pragma once

#define SZ 4
#define STRMAXLEN 80

#include <iostream>

namespace P3_1 {
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
		std::istream &hexin(std::istream &);
		std::ostream &hexout(std::ostream &) const;
		//operations
		Hex sum(const Hex &) const;
		Hex subt(const Hex &) const;
		Hex &moveL(const int &);
		Hex &moveR(const int &);
		//info
		bool isEqual(const Hex &) const;
		bool isLesser(const Hex &) const;
		bool isBigger(const Hex &) const;
		bool isEven() const;
		//destr
		//~Hex();
		//get
		int getSign() const { return sign; }
		int getLen() const { return len; }
		char getDig(int i) const { return dig[i]; }
	};
}

