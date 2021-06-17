#include<iostream>
#include<vector>
#include<unordered_set>
#include<set>
#include<time.h>

using namespace std;

void test_time()
{
	int n = 1000000;
	vector<int> v;
	srand(time(0)); //初始化随机数发生器
	for (int i = 0; i < n; ++i)
	{
		v.push_back(rand()); //随机数发生器
	}

	set<int> s;
	size_t begin1 = clock();

	for (auto e : v)
	{
		s.insert(e);
	}
	size_t end1 = clock();

	cout << "set : " << end1 - begin1 << endl;


	unordered_set<int> us;
	size_t begin2 = clock();

	for (auto e : v)
	{
		us.insert(e);
	}
	size_t end2 = clock();

	cout << "unordered_set : " << end2 - begin2 << endl;
}

int main()
{
	test_time();
}

//#include"HashTable.hpp"
//#include"unordered_set.h"
//#include"unordered_map.h"
//int main()
//{
//	//wzy::test_unordered_set();
//	wzy::test_unordered_map();
//	return 0;
//}

