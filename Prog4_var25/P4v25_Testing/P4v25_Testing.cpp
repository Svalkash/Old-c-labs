// P4v25_Testing.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <include\gtest\gtest.h>
#include "../P4v25_Library_Templates/myMultimap.h"
#include "../P4v25_Library_Templates/mySet.h"
#include "../P4v25_Library_Templates/myVector.h"

using namespace P4v25_T;
using namespace std;

TEST(myVector, defaultConstructor)
{
	myVector<int> m;
	ASSERT_EQ(0, m.size());
}

TEST(myVector, pushBack)
{
	myVector<int> m;
	int a = 12;
	m.push_back(a);
	ASSERT_EQ(1, m.size());
	ASSERT_EQ(a, m[0]);
}

TEST(myVector, iterators)
{
	myVector<int> m;
	int a = 12, b = 14;
	m.push_back(a);
	m.push_back(b);
	ASSERT_EQ(2, m.size());
	myVector<int>::iterator it = m.begin();
	ASSERT_EQ(a, (*it));
	++it;
	ASSERT_EQ(b, (*it));
}

TEST(mySet, defaultConstructor)
{
	mySet<int> m;
	ASSERT_EQ(0, m.size());
}

TEST(mySet, insert)
{
	mySet<int> m;
	int a = 4, b = a;
	m.insert(a);
	ASSERT_EQ(1, m.size());
	m.insert(b);
	ASSERT_EQ(1, m.size());
}

TEST(mySet, erase)
{
	mySet<int> m;
	int a = 4, b = 6;
	m.insert(a);
	ASSERT_EQ(1, m.size());
	m.insert(b);
	ASSERT_EQ(2, m.size());
	m.erase(b);
	ASSERT_EQ(1, m.size());
	m.erase(b);
	ASSERT_EQ(1, m.size());
}

TEST(mySet, iterators)
{
	mySet<int> m;
	int a = 12, b = 14;
	m.insert(a);
	m.insert(b);
	ASSERT_EQ(2, m.size());
	mySet<int>::iterator it = m.begin();
	ASSERT_EQ(a, (*it));
	++it;
	ASSERT_EQ(b, (*it));
}

TEST(myMultimap, defaultConstructor)
{
	myMultimap<int, char> m;
	ASSERT_EQ(0, m.size());
}

TEST(myMultimap, insert)
{
	myMultimap<int, char> m;
	myPair<const int, char> a(4, 'a'), b(4, 'b');
	m.insert(a);
	ASSERT_EQ(1, m.size());
	m.insert(b);
	ASSERT_EQ(2, m.size());
}

TEST(myMultimap, iterators)
{
	myMultimap<int, char> m;
	myPair<const int, char> a(4, 'a'), b(6, 'b');
	m.insert(a);
	m.insert(b);
	ASSERT_EQ(2, m.size());
	myMultimap<int, char>::iterator it = m.begin();
	ASSERT_EQ(a, (*it));
	++it;
	ASSERT_EQ(b, (*it));
}

TEST(myMultimap, erase)
{
	myMultimap<int, char> m;
	myPair<const int, char> a(4, 'a'), b(6, 'b'), c(4, 'c');
	m.insert(a);
	ASSERT_EQ(1, m.size());
	m.insert(b);
	ASSERT_EQ(2, m.size());
	m.insert(c);
	ASSERT_EQ(3, m.size());
	myMultimap<int, char>::iterator it = m.begin();
	++it;
	m.erase(it);
	it = m.begin();
	ASSERT_EQ(a, (*it));
	++it;
	ASSERT_EQ(c, (*it));
}


int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	return 0;
}

