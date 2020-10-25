#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_
namespace MyTinySTL {
	//����ģ�����
	template<class T>
	void swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}

	//pair��ģ��
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
		pair(const pair<u, v>& pr);//�������õĸ�ֵ
		pair(const first_type& a, const second_type& b);//ֱ�ӳ�ʼ��
		pair& operator=(const pair& pr);//��pair(const pair<u,v>& pr) ��ͬ�������ǵ���ǰ��
		void swap(pair& pr);
	public:
		template<class T1,class T2>
		friend bool operator==(const pair<T1��T2>&lhs, const pair<T1, T2>& rhs);
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
		return *this;//��������
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
		lhs.swap(rhs);//�൱�� MyTinySTL::swap(lhs, rhs);
	}

	template<class u, class v>
	pair<u, v> make_pair(const u& uu, const v& vv) {
		return pair<u, v>(uu, vv);
	}
}
#endif // !_UTILITY_H_


