#pragma once
#include<iostream>
#include<vector>
#include<string>
using namespace std;
//��ɢ��
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

	//ģ���ػ�
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
	//�����ʹ�ú��������װmap��set��һ���ģ������ɵڶ���ģ����������ƴ洢��������K,����pair<const K,V>
	template<class K, class T, class HashFunc = Hash<K>>//��Ҫ�������Kת��Ϊ����
	class HashTable
	{
	public:
		bool Insert(const T& t)
		{
			//�غ�����= ������е�Ԫ�ظ��� / ɢ�б�ĳ���
			//��������>0.7(�ϸ������0.7����)
			//ֻ������������£�Ч����Խϸߣ����ҿռ�������Ҳ�Ϻ�
			if (_tables.size() == 0 || _size * 10 / _tables.size() == 7)
			{
				size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				//���������ǲ��Եģ���Ϊ�п���ԭ����ͻ�ģ����ڿռ�����ݣ��������ǳ�ͻ���ˣ�
				//��������Ӧ�ÿ��ٺ�һ�οռ䣬Ȼ�����¶���������ݽ��м��㣬Ȼ�������οռ��ڣ����ͷ�ԭ���Ŀռ�


				//vector<HashNode<T>> newtables;
				//newtables.resize(newsize);

				////��ʱ���ǰ�ԭ�ռ��ϵ����ݣ��������¼���ŵ����Ӧ���¿ռ���
				//for (size_t i = 0; i < _tables.size(); ++i)
				//{
				//	if (_tables[i]._state == EXIST)
				//	{
				//		//����̽�������±��е�λ��
				//	}
				//}
				//newtables.swap(_tables);

				HashTable<K, T, HashFunc> newht;
				newht._tables.resize(newsize);
				for (auto& e : _tables)
				{
					if (e._state == EXIST)
					{
						//���¼���λ�ã�Ȼ���������µı���
						newht.Insert(e._t); //�þɿռ���������¼���������̫�鷳���һ�����ֱ��������һ�������
					}
				}
				_tables.swap(newht._tables);
			}
			//��������������
			HashNode<T>*  ret = Find(t);
			if (ret)
				return false;

			HashFunc hf;
			//������һ��ʼ��ʱ����Ĺ�ϣ��������û��Ԫ�صģ����Ի����ģ0���������
			size_t start = hf(t) % _tables.size();
			size_t index = start; //����д�ĺô����ǣ��ܹ��ڶ���̽���ʱ�򷽱��޸�
			//����̽��,��һ����λ��
			size_t i = 1;
			while (_tables[index]._state == EXIST) //Ӧ��ȥ����һ����λ��
			{
				index = start + i;
				index %= _tables.size();
				++i;
			}
			//����ѭ�������������1.EMPTY  2. DELETE��������������԰�ֵ�Ž�ȥ
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
				if (_tables[index]._t == key && _tables[index]._state == EXIST) //��Ϊ�����ҵ������ֵ��ɾ������
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
				//αɾ��
				ret->_state = DELETE;
				return true;
			}
		}
	private:
		vector<HashNode<T>> _tables;
		size_t _size = 0; //��Ч���ݵĸ���
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

		////ֻҪK�����Ͳ�֧��ȡģ����Ҫд�º���
		//Hash<string> strht;
		//cout << strht("abcd") << endl;
		//cout << strht("acdb") << endl;
		//cout << strht("abdc") << endl;
		//cout << strht("adbc") << endl;
	}
}


// ��ɢ�з��� -- ����̽��
// set<K>->HashTable<K, K>
// map<K, V>->HashTable<K, pair<K, V>>





//��ɢ��
//��ϣͰ


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

	// �ػ�
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

	//ǰ����������Ϊ�ڹ����������ʱ����ҪHashTable����������ģ�廹û�н����ػ����Ҳ���
	template<class K, class T, class KeyOfT, class hash>
	class HashTable;

	//���ڷ�װ��������˵��++��ô�ͻ�����Ͱ�������һ������λ�ã����ǵ����Ͱ�����н���ʱ�򣬾���Ҫֱ�ӵ�������һ����hashӳ���λ��
	template<class K, class T, class Ref,class Ptr ,class KeyOfT, class hash> 
	struct HashIterator
	{
		typedef HashIterator<K, T, Ref,Ptr,KeyOfT, hash> Self;
		typedef HashLinkNode<T> Node;
		Node* _node;
		HashTable<K, T, KeyOfT, hash>* _pht; //����Ҫһ��ָ���ϣ���ָ�룬��Ϊ��Ҫ������ϣ�������ʱ���ͨ��������
		//�ܹ������ҵ���һ����Ҫ����Ծ��λ��

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
		//��ϣ��ĵ�������û��--�ģ���Ϊ����һ�����������
		//����operator++��ǰ��++����˵������֮�����ɷ��ص���һ����������ָ��
		//1. ��ǰͰ�������ݣ�������
		//2. ��ǰͰû�����ݣ�������һ��Ͱ���ӵ�һ����ʼ
		Self operator++()
		{
			if (_node->_next)
			{
				_node = _node->_next;
			}
			else
			{
				KeyOfT kot;
				//һ��Ͱ�Ѿ������ˣ��ҵ���һ��Ͱ
				//��Ϊ���ڵ�ǰͰ�ǿ�������ҵ�λ�õģ����ǻ���Ҫһ��hash��
				//�ߵ�����˵�����������index��ԭ��λ�ã��Ѿ�ȫ���������ˣ���ôӦ�ô�������һ��λ�ÿ�ʼ����
				size_t index = _pht->HashFunc(kot(_node->_t),_pht->_tables.size());
				++index;
				while (index < _pht->_tables.size())
				{
					if (_pht->_tables[index])
					{
						//�Ϳ�ʼ�������Ͱ
						_node = _pht->_tables[index];
						break;
					}
					else
					{
						++index;
					}
				}

				//����while�����п��������ֿ���1.break��������   2.ѭ���߽�����
				//�����������������ܹ�����һ��
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
					return Iterator(_tables[i],this); //this��ָ��HashTable��ָ��
				}
			}

			//�����ϣ����һ������Ҳû�У���ô����nullptrҲ����ȷ��
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
			//���ڿ�ɢ��/��ϣͰ����Ҫ���Ǹ������ӵ�����,����һ�������1��Ҳ���������ڼӣ��ռ������ʸ���
			//�Լ���ͻ�Ķֻ࣬��Ӱ�����Լ�������Ӱ�����
			KeyOfT kot;
			//���Ƹ������� == 1��ʱ������
			if (_size == _tables.size())
			{
				size_t newsize = GetNextPrime(_tables.size());//�õ�����,��Ч��֤�ܹ����Ч��
				vector<Node*> newtables;
				newtables.resize(newsize,nullptr);

				//ѭ���İ�ÿ����ϣͰ�ù��������¹ҽ�
				for (size_t i = 0; i < _tables.size(); ++i)
				{
					//�ɱ��н��ֱ��ȡ���������±���
					Node* cur = _tables[i];
					while (cur)
					{
						Node* next = cur->_next;
						size_t index = HashFunc(kot(cur->_t),newtables.size());//���¼����������ٵ��±��е�λ��
						//ͷ��
						cur->_next = newtables[index];
						newtables[index] = cur;

						cur = next;
					}
					_tables[i] = nullptr;//��Ϊ�������ı�   ָ����ܻ�ָ�������λ��
				}
				newtables.swap(_tables);
			}

			//��Ҫ�õ��±��λ��
			size_t index = HashFunc(kot(t),_tables.size());//���Ȱ�T�����key��ȡ���������ǻ��ǲ��ܹ�ȷ���Ƿ����ȡģ�����Ի���Ҫ������һ��
			
			//�������ֵ���࣬������Ҫ����t�ڲ���
			//���ҵ��Ǹ�λ�ã�Ȼ���ڱ���������ϣͰ
			Node* cur = _tables[index];
			while (cur)
			{
				if (kot(cur->_t) == kot(t))
				{
					return make_pair(Iterator(cur,this), false);
				}
				cur = cur->_next;
			}

			//���ڹ�ϣ����һ��ʼ�洢����NULL���������йҽӵ�ʱ�򣬴洢�Ľ����Ϊ��һ�����ĵ�ַ
			//ͷ�嵽����Ͱ����
			Node* newnode = new Node(t);
			newnode->_next = _tables[index];
			_tables[index] = newnode;

			return make_pair(Iterator(newnode,this), true);
		}

		//Find����ͨ��Key��Ѱ�ҵ�
		Iterator Find(const K& key)
		{
			KeyOfT kot;
			size_t index = HashFunc(key,_tables.size());
			Node* cur = _tables[index];
			//����hash�����ҵ�λ�ã�Ȼ���ڱ�����λ�õ�����hashͰ
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
			//�������һ���������ɾ��
			Node* prev = nullptr;
			Node* cur = _tables[index];
			//����hash�����ҵ�λ�ã�Ȼ���ڱ�����λ�õ�����hashͰ
			while (cur)
			{
				if (kot(cur->_t) == key)
				{
					//�ͽ���ɾ��
					if (prev == nullptr)
					{
						//˵��ɾ������ͷ
						_tables[index] = cur->_next; //����ҽӵĵ�һ����ֱ���ҵ���
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
		size_t _size = 0;//��Ч���ݵĸ���
	};

}