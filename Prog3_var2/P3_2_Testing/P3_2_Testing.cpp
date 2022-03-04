// P3_1_Testing.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include "..\P3_2_Library\Hex.h"
#include "include\gtest\gtest.h"

using namespace P3_2;

TEST(HConstructor, DefaultConstructor)
{
	Hex a;
	ASSERT_EQ(0, a.getSign());
	ASSERT_EQ(0, a.getLen());
	for (int i = 1; i < SZ; i++)
		ASSERT_EQ('0', a.getDig(i));
}

TEST(HConstructor, InitConstructors)
{
#if (SZ>=4)
	Hex a(0x0300);
	ASSERT_EQ(0, a.getSign());
	ASSERT_EQ(3, a.getLen());
	ASSERT_EQ('3', a.getDig(2));
#endif
	Hex b(-6);
	ASSERT_EQ(1, b.getSign());
	ASSERT_EQ(1, b.getLen());
	ASSERT_EQ('6', b.getDig(0));
#if (SZ>=4)
	char str[6] = { '-', '1', '5', 'A', 'B', '\0' }; //WRONG?!
	Hex c(str);
	ASSERT_EQ(1, c.getSign());
	ASSERT_EQ(4, c.getLen());
	for (int i = 0; i < 4; i++)
		ASSERT_EQ(str[c.getLen() - i], c.getDig(i));
	Hex f("ABCD");
	Hex g(f);
	ASSERT_EQ(f.getLen(), g.getLen());
	ASSERT_EQ(f.getSign(), g.getSign());
	for (int i = 0; i < 4; i++)
		ASSERT_EQ(f.getDig(i), g.getDig(i));
#endif
	Hex toor("-0");
	ASSERT_EQ(0, toor.getSign());
}

TEST(HConstructor, Exceptions)
{
#if (SZ<6)
	ASSERT_ANY_THROW(Hex(0xF000000));
#endif
	ASSERT_ANY_THROW(Hex("K"));
	ASSERT_ANY_THROW(Hex("!"));
#if (SZ<9)
	ASSERT_ANY_THROW(Hex("123456789"));
#endif
}

TEST(HMethods, All)
{
#if (SZ>=4)
	//
	ASSERT_EQ(1, Hex("AD23")==(Hex("8C01")+(Hex("2122"))));
	ASSERT_EQ(1, Hex("-5FA1")==(Hex("202E")-("7FCF")));
	ASSERT_EQ(1, Hex("-1")==(Hex("-1")-("0")));
	//
	ASSERT_EQ(1, Hex("5F00")==(Hex("5F")<<=(2)));
	ASSERT_EQ(1, Hex("1")==(Hex("1BCA")>>=(3)));
	ASSERT_EQ(1, Hex("0")==(Hex("1BCA")>>=(10 * SZ)));
	//
	ASSERT_EQ(1, Hex("ABB")>("A0"));
	ASSERT_EQ(1, Hex("ACA")>("-B"));
	ASSERT_EQ(0, Hex("A00")>("DABB"));
	ASSERT_EQ(0, Hex("AB")<("AB"));
	ASSERT_EQ(1, Hex("ABB")<("DABB"));
	ASSERT_EQ(1, Hex("-0")==("0"));
	ASSERT_EQ(0, Hex("A")==("BA"));
	//
	ASSERT_EQ(1, Hex("A").isEven());
	ASSERT_EQ(0, Hex("-F").isEven());
	ASSERT_EQ(1, Hex(0).isEven());
#endif
}

TEST(HMethods, Exceptions)
{
#if (SZ == 4)
	ASSERT_ANY_THROW(Hex(0xFFFF)+(1));
	ASSERT_ANY_THROW(Hex(-65535)-(1));
	ASSERT_ANY_THROW(Hex(1)<<=(5));
#endif
}

//SET SZ TO 4

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

