// P2v5_Testing.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include "..\P2v5_Library\ConchoNic.h"
#include "include\gtest\gtest.h"

using namespace P2_v5;

TEST(CNConstructor, DefaultConstructor)
{
	ConchoNic a1;
	ASSERT_EQ(a1.getP().x, 0);
	ASSERT_EQ(a1.getP().y, 0);
	ASSERT_EQ(a1.getA(), 1);
	ASSERT_EQ(a1.getL(), 2);
}

TEST(CNConstructor, InitConstructors)
{
	ConchoNic a2(3, 5);
	ASSERT_EQ(a2.getP().x, 0);
	ASSERT_EQ(a2.getP().y, 0);
	ASSERT_EQ(a2.getA(), 3);
	ASSERT_EQ(a2.getL(), 5);

	Point p(5, 6);
	ConchoNic a3(p, 10, 11);
	ASSERT_EQ(a3.getP().x, 5);
	ASSERT_EQ(a3.getP().y, 6);
	ASSERT_EQ(a3.getA(), 10);
	ASSERT_EQ(a3.getL(), 11);

	ConchoNic a4(-20, 30, 15, 17);
	ASSERT_EQ(a4.getP().x, -20);
	ASSERT_EQ(a4.getP().y, 30);
	ASSERT_EQ(a4.getA(), 15);
	ASSERT_EQ(a4.getL(), 17);
}

TEST(CNConstructor, TestException)
{
	Point p;
	ASSERT_ANY_THROW(ConchoNic(2, -3));
	ASSERT_ANY_THROW(ConchoNic(p, -1));
}

TEST(CNMethods, Setters)
{
	ConchoNic a;
	Point p1(2, 1);
	a.setP(p1);
	ASSERT_EQ(a.getP().x, 2);
	ASSERT_EQ(a.getP().y, 1);
	Point p2(-22, -16);
	a.setP(p2);
	ASSERT_EQ(a.getP().x, -22);
	ASSERT_EQ(a.getP().y, -16);
	a.setA(4);
	ASSERT_EQ(a.getA(), 4);
	a.setL(2);
	ASSERT_EQ(a.getL(), 2);
	ASSERT_ANY_THROW(a.setA(0));
	ASSERT_ANY_THROW(a.setL(-3));
}

TEST(CNMEthods, Others)
{
	const double err = 0.0001;
	ConchoNic a;
	//fDec
	ASSERT_EQ(a.fDec(-1).x, 0);
	ASSERT_EQ(a.fDec(-1).y, 0);
	ASSERT_EQ(a.fDec(0).x, 0);
	ASSERT_EQ(a.fDec(0).y, 0);
	ASSERT_EQ(a.fDec(3).x, 0);
	ASSERT_EQ(a.fDec(3).y, 0);
	ASSERT_ANY_THROW(a.fDec(-5));
	ASSERT_ANY_THROW(a.fDec(20));
	ASSERT_ANY_THROW(a.fDec(1));
	//fPol
	ASSERT_EQ(a.fPol(0).x, 3);
	ASSERT_EQ(a.fPol(0).y, 3);
	ASSERT_EQ(a.fPol(M_PI).x, 1);
	ASSERT_EQ(a.fPol(M_PI).y, 1);
	ASSERT_ANY_THROW(a.fPol(M_PI_2));
	//radCurv
	ASSERT_NEAR(a.radCurv().Ra, 4.5, err);
	ASSERT_NEAR(a.radCurv().Rc, 0.5, err);
	ASSERT_NEAR(a.radCurv().Ro, sqrt(3), err);
	//area
	ConchoNic b(2, 1);
	ASSERT_EQ(b.area(), 0);
	//INFPOINTS
}

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	//system("pause");
	//return 0;
}

