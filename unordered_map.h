#pragma once 
#include"HashTable.hpp"

namespace wzy
{
	template<class K,class V,class hash = Open::Hash<K>>
	class unordered_map
	{
		struct MapKOfT
		{
			const K& operator()(const pair<const K, V>& kv)
			{
				return kv.first;
			}
		};

	public:
		typedef typename Open::HashTable<K, pair<const K, V>, MapKOfT, hash>::Iterator iterator;
		pair<iterator, bool> insert(const pair<const K, V>& kv)
		{
			return _ht.Insert(kv);
		}

		iterator begin()
		{
			return _ht.Begin();
		}

		iterator end()
		{
			return _ht.End();
		}

		iterator find(const K& key)
		{
			return _ht.Find(key);
		}

		bool earse(const K& key)
		{
			return _ht.Earse(key);
		}

		V& operator[](const K& key)
		{
			pair<iterator,bool> ret = insert(make_pair(key, V()));
			return ret.first->second;
		}
	private:
		Open::HashTable<K, pair<const K, V>, MapKOfT,hash> _ht;
	};

	void test_unordered_map()
	{
		wzy::unordered_map<int, int> um;
		um.insert(make_pair(1, 1));
		um.insert(make_pair(2, 1));
		um.insert(make_pair(3, 1));

		unordered_map<int,int>::iterator it = um.begin();
		while (it != um.end())
		{
			cout << it->first << ":"<<it->second<<endl;
			++it;
		}

		wzy::unordered_map<string,string> dict;
		dict["hash"] = "¹şÏ£";
		dict["sort"] = "ÅÅĞò";
		dict["insert"] = "²åÈë";
		dict["find"] = "²éÕÒ";

		for (auto& e : dict)
		{
			cout << e.first << ":" << e.second << endl;
		}

		cout <<endl;

		dict.earse("hash");
		dict.earse("find");
		for (auto& e : dict)
		{
			cout << e.first << ":" << e.second << endl;
		}
	}
}