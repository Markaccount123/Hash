#pragma once
#include "HashTable.hpp"

//��ʵ��map��setһ������������������û��ʲô�����ģ����ǵ����ֲ��������ں�����Ļ�������ʵ�ֵ�
//���ڵ�unordered_set��unordered_mapҲ�ǣ������ڿ�ɢ��HashTable��������ʵ�ֵ�
namespace wzy
{
	template<class K,class hash = Open::Hash<K>> //����orderedset_set��˵��������ʹ�ã������ô�������ģ�����
	class unordered_set
	{
		struct SetKOfT
		{
			const K& operator()(const K& k)
			{
				return k;
			}
		};
	public:
		typedef typename Open::HashTable<K, K, SetKOfT, hash>::Iterator iterator;

		iterator begin()
		{
			return _t.Begin();
		}

		iterator end()
		{
			return _t.End();
		}

		iterator find(const K& key)
		{
			return _t.Find(key);
		}

		bool earse(const K& key)
		{
			return _t.Earse(key);
		}
		pair<iterator, bool> insert(const K& k)
		{
			return _t.Insert(k);
		}

	private:
		Open::HashTable<K, K, SetKOfT, hash> _t;
	};

	void test_unordered_set()
	{
		wzy::unordered_set<int> us;
		us.insert(1);
		us.insert(54);
		us.insert(58);
		us.insert(59);
		us.insert(21);
		us.insert(22); 
		us.insert(23);
		us.insert(24);
		for (auto& e : us)
		{
			cout << e << " ";
		}
		cout << endl;

		unordered_set<int>::iterator it = us.find(22);
		cout << *it << endl;

		us.earse(24);
		us.earse(21);
		us.earse(22);
		for (auto& e : us)
		{
			cout << e << " ";
		}
		cout << endl;
	}
}




