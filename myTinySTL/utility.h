#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_
namespace MyTinySTL {
	//交换模板的类
	template<class T>
	void swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}

	//pair的模板
	template<class T1, class T2>
	struct pair {
	public:
		typedef T1 first_type;
		typedef T2 second_type;
	public:
		T1 first;
		T2 second;
	public:
		pair(){}
		template<class u, class v>
		pair(const pair<u, v>& pr);//借助引用的赋值
		pair(const first_type& a, const second_type& b);//直接初始化
		pair& operator=(const pair& pr);//与pair(const pair<u,v>& pr) 相同，本质是调用前者
		void swap(pair& pr);
	public:
		template<class T1,class T2>
		friend bool operator==(const pair<T1，T2>&lhs, const pair<T1, T2>& rhs);
		template<class T1,class T2>
		friend bool operator!=(const pair<T1, T2>&lhs, const pair<T1, T2>& rhs);
		template<class T1,class T2>
		friend bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template<class T1, class T2>
		friend bool operator<=(const pair<T1, T2>&lhs, const pair<T1, T2>&rhs);
		template<class T1, class T2>
		friend bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
		template<class T1, class T2>
		friend bool operator >= (const pair<T1, T2>&lhs, const pair<T1, T2>& rhs);
		template<class T1, class T2>
		friend void swap(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs);
	};

	template<class T1,class T2>
	template<class u,class v>
	pair<T1, T2>::pair(const pair < u, v>& pr):first(pr.first), second(pr.second){}

	template<class T1, class T2>
	pair<T1,T2>::pair(const first_type&a,const second_type&b):first(a),second(b){}

	template<class T1, class T2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair<T1,T2>& pr) {
		if (this != pr) {
			first = pr.first;
			second = pr.second;
		}
		return *this;//返回引用
	}

	template<class T1, class T2>
	void pair<T1, T2>::swap( pair<T1, T2>& pr) {
		MyTinySTL::swap(first, pr.first);
		MyTinySTL::swap(second, pr.second);
	}

	template<class T1, class T2>
	bool operator==(const pair<T1, T2>&lhs, const pair<T1, T2>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<class T1, class T2>
	bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
		return !(lhs == rhs);
	}

	template<class T1, class T2>
	bool operator <(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	}

	template<class T1, class T2>
	bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
		return !(rhs < lhs);
	}

	template<class T1, class T2>
	bool operator > (const pair<T1, T2>&lhs, const pair<T1, T2>& rhs) {
		return rhs < lhs;
	}

	template<class T1, class T2>
	bool operator >=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
		return !(lhs < rhs);
	}

	template<class T1, class T2>
	void swap(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
		lhs.swap(rhs);//相当于 MyTinySTL::swap(lhs, rhs);
	}

	template<class u, class v>
	pair<u, v> make_pair(const u& uu, const v& vv) {
		return pair<u, v>(uu, vv);
	}
}
#endif // !_UTILITY_H_


