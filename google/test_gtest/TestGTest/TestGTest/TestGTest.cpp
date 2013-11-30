// TestGTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "gtest/gtest.h"

// http://www.cnblogs.com/coderzh/archive/2009/03/31/1426758.html

class TestEnvironment : public testing::Environment
{
public:
	virtual void SetUp() {puts("======================================>>");}
	virtual void TearDown() {puts("<<======================================");}
};

class TestCaseEnvironment : public testing::Test
{
public:
	virtual void SetUp() {puts("-------------------------------------->>");}
	virtual void TearDown() {puts("<<--------------------------------------");}
};


int add(int a, int b)
{
	return a+b;
}

float div(float a, float b)
{
	//if (fabs(b) <= 0.000001f)
	//{
	//	throw "error!";
	//}
	return a/b;
}

TEST(addTest, ExpectTest)
{
	EXPECT_GE(2, add(1, 1));
	EXPECT_LT(4, add(1, 3));
	EXPECT_EQ(6, add(4, 2));
	EXPECT_EQ(444, add(404, 40));
	EXPECT_EQ(3, add(1, 1));
}

TEST(addTest, AssertTest)
{
	ASSERT_GE(2, add(1, 1));
	ASSERT_LT(4, add(1, 3));
	ASSERT_EQ(6, add(4, 2));
	ASSERT_EQ(444, add(404, 40));
	ASSERT_EQ(3, add(1, 1));
}

TEST(divTest, FloatAndThrowTest)
{
	EXPECT_FLOAT_EQ(5.f, div(100.f, 20.f));
	EXPECT_FLOAT_EQ(3.333333f, div(10.f, 3.f));
	EXPECT_ANY_THROW(div(3.f, 0.f));
	EXPECT_NE(0, div(10.f, 0.f));
}

TEST_F(TestCaseEnvironment, CaseEnvironmentTest)
{
	EXPECT_FLOAT_EQ(5.f, div(100.f, 20.f));
	EXPECT_FLOAT_EQ(3.333333f, div(10.f, 3.f));
	EXPECT_ANY_THROW(div(3.f, 0.f));
	EXPECT_NE(0, div(10.f, 0.f));
}

int _tmain(int argc, _TCHAR* argv[])
{
	testing::AddGlobalTestEnvironment(new TestEnvironment);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

