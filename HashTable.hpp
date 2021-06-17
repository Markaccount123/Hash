#pragma once
#include<iostream>
#include<vector>
#include<string>
using namespace std;
//闭散列
namespace Close
{

	enum State
	{
		EMPTY,
		EXIST,
		DELETE
	};

	template<class T>
	struct HashNode
	{
		State _state = EMPTY;
		T _t;
	};

	template<class K>
	struct Hash
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};

	//模板特化
	template<>
	struct Hash < string >
	{
		size_t operator()(const string& s)
		{
			size_t hash = 0;
			for (auto ch : s)
			{
				//hash += ch;
				hash = hash * 131 + ch; //
			}
			return hash;
		}
	};
	//这里和使用红黑树来封装map和set是一样的，都是由第二个模板参数来控制存储的类型是K,还是pair<const K,V>
	template<class K, class T, class HashFunc = Hash<K>>//需要把这里的K转换为整形
	class HashTable
	{
	public:
		bool Insert(const T& t)
		{
			//载荷因子= 填入表中的元素个数 / 散列表的长度
			//负载因子>0.7(严格控制在0.7以下)
			//只有在这种情况下：效率相对较高，并且空间利用率也较好
			if (_tables.size() == 0 || _size * 10 / _tables.size() == 7)
			{
				size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				//但是这里是不对的，因为有可能原来冲突的，由于空间的扩容，而不再是冲突的了，
				//所以我们应该开辟好一段空间，然后重新对上面的数据进行计算，然后放入这段空间内，在释放原来的空间


				//vector<HashNode<T>> newtables;
				//newtables.resize(newsize);

				////此时就是把原空间上的数据，拿来重新计算放到相对应的新空间上
				//for (size_t i = 0; i < _tables.size(); ++i)
				//{
				//	if (_tables[i]._state == EXIST)
				//	{
				//		//线性探测找在新表中的位置
				//	}
				//}
				//newtables.swap(_tables);

				HashTable<K, T, HashFunc> newht;
				newht._tables.resize(newsize);
				for (auto& e : _tables)
				{
					if (e._state == EXIST)
					{
						//重新计算位置，然后放在这个新的表中
						newht.Insert(e._t); //拿旧空间的数据重新计算我嫌弃太麻烦，我还不如直接重新来一遍插入呢
					}
				}
				_tables.swap(newht._tables);
			}
			//不允许数据冗余
			HashNode<T>*  ret = Find(t);
			if (ret)
				return false;

			HashFunc hf;
			//这里在一开始的时候，你的哈希表里面是没有元素的，所以会出现模0，出错情况
			size_t start = hf(t) % _tables.size();
			size_t index = start; //这样写的好处就是，能够在二次探测的时候方便修改
			//线性探测,找一个空位置
			size_t i = 1;
			while (_tables[index]._state == EXIST) //应该去找下一个空位置
			{
				index = start + i;
				index %= _tables.size();
				++i;
			}
			//跳出循环就是两种情况1.EMPTY  2. DELETE这两种情况都可以把值放进去
			_tables[index]._t = t;
			_tables[index]._state = EXIST;
			_size++;

			return true;
		}

		HashNode<T>* Find(const K& key)
		{
			HashFunc hf;
			size_t start = hf(key) % _tables.size();
			size_t index = start;
			size_t i = 1;
			while (_tables[index]._state != EMPTY)
			{
				if (_tables[index]._t == key && _tables[index]._state == EXIST) //因为可能找到的这个值被删除掉了
				{
					return &_tables[index];
				}
				index = start + i;
				index %= _tables.size();
				++i;
			}
			return nullptr;
		}

		bool Erase(const K& key)
		{
			HashNode<T>* ret = Find(key);
			if (ret == nullptr)
			{
				return false;
			}
			else
			{
				//伪删除
				ret->_state = DELETE;
				return true;
			}
		}
	private:
		vector<HashNode<T>> _tables;
		size_t _size = 0; //有效数据的个数
	};

	void testHashTable()
	{
		HashTable<int, int> ht;
		ht.Insert(5);
		ht.Insert(15);
		ht.Insert(16);
		ht.Insert(17);
		ht.Insert(25);
		ht.Insert(35);
		ht.Insert(45);
		ht.Insert(55);

		////只要K的类型不支持取模，就要写仿函数
		//Hash<string> strht;
		//cout << strht("abcd") << endl;
		//cout << strht("acdb") << endl;
		//cout << strht("abdc") << endl;
		//cout << strht("adbc") << endl;
	}
}


// 闭散列方法 -- 线性探测
// set<K>->HashTable<K, K>
// map<K, V>->HashTable<K, pair<K, V>>





//开散列
//哈希桶


namespace Open
{
	size_t GetNextPrime(size_t prime)
	{
		static const int PRIMECOUNT = 28;
		static const size_t primeList[PRIMECOUNT] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		size_t i = 0;
		for (; i < PRIMECOUNT; ++i)
		{
			if (primeList[i] > prime)
				return primeList[i];
		}

		return primeList[i];
	}

	template<class T>
	struct HashLinkNode
	{
		T _t;
		HashLinkNode<T>* _next;

		HashLinkNode(const T& t)
			: _t(t)
			, _next(nullptr)
		{}
	};

	template<class K>
	struct Hash
	{
		size_t operator()(const K& key)
		{
			return key;
		}
	};

	// 特化
	template<>
	struct Hash < string >
	{
		size_t operator()(const string& s)
		{
			size_t hash = 0;
			for (auto ch : s)
			{
				//hash += ch;
				hash = hash * 131 + ch;
			}

			return hash;
		}
	};

	//前置声明，因为在构造迭代器的时候，需要HashTable，但是这里模板还没有进行特化，找不到
	template<class K, class T, class KeyOfT, class hash>
	class HashTable;

	//对于封装迭代器来说，++那么就会跳到桶里面的下一个结点的位置，但是当这个桶不再有结点的时候，就需要直接的跳到下一个有hash映射的位置
	template<class K, class T, class Ref,class Ptr ,class KeyOfT, class hash> 
	struct HashIterator
	{
		typedef HashIterator<K, T, Ref,Ptr,KeyOfT, hash> Self;
		typedef HashLinkNode<T> Node;
		Node* _node;
		HashTable<K, T, KeyOfT, hash>* _pht; //还需要一个指向哈希表的指针，因为需要遍历哈希表，如果此时这个通遍历完了
		//能够帮助找到下一个需要到跳跃的位置

		HashIterator(Node* node, HashTable<K, T, KeyOfT, hash>* pht)
			:_node(node)
			, _pht(pht)
		{}

		Ref operator*()
		{
			return _node->_t;
		}

		Ptr operator->()
		{
			return &(_node->_t);
		}

		bool operator!=(const Self& s) const 
		{
			return _node != s._node;
		}
		//哈希表的迭代器是没有--的，因为这是一个单项迭代器
		//对于operator++（前置++）来说，加完之后依旧返回的是一个迭代器的指针
		//1. 当前桶还有数据，继续走
		//2. 当前桶没有数据，跳到下一个桶，从第一个开始
		Self operator++()
		{
			if (_node->_next)
			{
				_node = _node->_next;
			}
			else
			{
				KeyOfT kot;
				//一个桶已经走完了，找到下一个桶
				//因为我在当前桶是可以算出我的位置的，但是还需要一个hash表
				//走到这里说明我算出来的index的原来位置，已经全部都走完了，那么应该从他的下一个位置开始计算
				size_t index = _pht->HashFunc(kot(_node->_t),_pht->_tables.size());
				++index;
				while (index < _pht->_tables.size())
				{
					if (_pht->_tables[index])
					{
						//就开始遍历这个桶
						_node = _pht->_tables[index];
						break;
					}
					else
					{
						++index;
					}
				}

				//但是while结束有可能是两种可能1.break跳出来的   2.循环走结束了
				//所以这两种情况最好能够区分一下
				if (index == _pht->_tables.size())
				{
					_node = nullptr;
				}
			}
			return *this;
		}
	};


	template<class K,class T,class KeyOfT,class hash = Hash<K>>
	class HashTable
	{
		typedef HashLinkNode<T> Node;
		friend struct HashIterator < K, T, T&, T*, KeyOfT, hash > ;
	public:
		typedef HashIterator<K, T, T&,T*,KeyOfT, hash> Iterator;
		typedef HashIterator<K, T, const T&, const T*, KeyOfT, hash> Const_Iterator;
		Iterator Begin()
		{
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				if (_tables[i])
				{
					return Iterator(_tables[i],this); //this是指向HashTable的指针
				}
			}

			//如果哈希表中一个数据也没有，那么返回nullptr也是正确的
			return End();
		}
		Iterator End()
		{
			return Iterator(nullptr,this);
		}

		size_t HashFunc(const K& key, size_t n)
		{
			hash hf;
			size_t ki = hf(key);
			return ki % n;
		}
	
		pair<Iterator, bool> Insert(const T& t)
		{
			//对于开散列/哈希桶依旧要考虑负载因子的问题,但是一般控制在1，也就是满了在加，空间利用率更高
			//自己冲突的多，只会影响我自己，不会影响别人
			KeyOfT kot;
			//控制负载因子 == 1的时候增容
			if (_size == _tables.size())
			{
				size_t newsize = GetNextPrime(_tables.size());//得到素数,有效验证能够提高效率
				vector<Node*> newtables;
				newtables.resize(newsize,nullptr);

				//循环的把每个哈希桶拿过来，重新挂接
				for (size_t i = 0; i < _tables.size(); ++i)
				{
					//旧表中结点直接取下来挂在新表中
					Node* cur = _tables[i];
					while (cur)
					{
						Node* next = cur->_next;
						size_t index = HashFunc(kot(cur->_t),newtables.size());//重新计算在所开辟的新表中的位置
						//头插
						cur->_next = newtables[index];
						newtables[index] = cur;

						cur = next;
					}
					_tables[i] = nullptr;//因为你算来的表   指针可能还指向着这个位置
				}
				newtables.swap(_tables);
			}

			//需要得到下标的位置
			size_t index = HashFunc(kot(t),_tables.size());//首先把T里面的key提取出来，但是还是不能够确定是否可以取模，所以还需要在套用一层
			
			//不允许键值冗余，所以需要查找t在不在
			//先找到那个位置，然后在遍历整个哈希桶
			Node* cur = _tables[index];
			while (cur)
			{
				if (kot(cur->_t) == kot(t))
				{
					return make_pair(Iterator(cur,this), false);
				}
				cur = cur->_next;
			}

			//对于哈希表中一开始存储的是NULL，当底下有挂接的时候，存储的将会变为第一个结点的地址
			//头插到链表桶里面
			Node* newnode = new Node(t);
			newnode->_next = _tables[index];
			_tables[index] = newnode;

			return make_pair(Iterator(newnode,this), true);
		}

		//Find就是通过Key来寻找的
		Iterator Find(const K& key)
		{
			KeyOfT kot;
			size_t index = HashFunc(key,_tables.size());
			Node* cur = _tables[index];
			//现在hash表中找到位置，然后在遍历该位置的整个hash桶
			while (cur)
			{
				if (kot(cur->_t) == key)
				{
					return Iterator(cur,this);
				}
				cur = cur->_next;
			}
			return End();
		}
		bool Earse(const K& key)
		{
			KeyOfT kot;
			size_t index = HashFunc(key, _tables.size());
			//这里就是一个单链表的删除
			Node* prev = nullptr;
			Node* cur = _tables[index];
			//现在hash表中找到位置，然后在遍历该位置的整个hash桶
			while (cur)
			{
				if (kot(cur->_t) == key)
				{
					//就进行删除
					if (prev == nullptr)
					{
						//说明删除的是头
						_tables[index] = cur->_next; //链表挂接的第一个就直接找到了
					}
					else
					{
						prev->_next = cur->_next;
					}
					delete cur;
					return true;
				}
				else
				{
					prev = cur;
					cur = cur->_next;
				}
			}
			return false;
		}
	private:
		vector<Node*> _tables;
		size_t _size = 0;//有效数据的个数
	};

}