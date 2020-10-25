#pragma once
#ifndef _FUNCTIONAL_H_
#define _FUNCTIONNAL_H_
#include<cstddef>
namespace myTinySTL {
	//����һԪ�����Ĳ����ͱ�ͷ���ֵ����
	template<class arg,class result>
	struct unarg_function {
		typedef arg argument_type;
		typedef result result_type;
	};

	//�����Ԫ�����Ĳ����ͱ�ͷ���ֵ����
	template<class arg1,class arg2,class result>
	struct binary_function {
		typedef arg1 first_argument_type;
		typedef arg2 second_argument_type;
		typedef result result_type;
	};

	//�������󣺼ӷ�
	template<class T>
	struct plus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x + y; }
	};

	//����
	template<class T>
	struct minus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x - y; }
	};

	//�˷�
	template<class T>
	struct multiple :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x * y; }
	};

	//����
	template<class T>
	struct divide :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x / y;
		}
	};

	//ȡģ
	template<class T>
	struct modulus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x%y;
		}
	};

	//��
	template<class T>
	struct negate :public unarg_function<T, T> {
		T operator()(const T& x) const { return -x; }
	};

	//�ӷ���֤ͬԪ��
	template<class T>
	T identity_element(plus<T>) { return T(0); }

	//�˷�֤ͬ
	template<class T>
	T identity_element(multiple<T>) { return T(1); }

	//����
	template<class T>
	struct equal_to :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T&y) const{ return x == y; }
	};

	//������
	template<class T>
	struct not_euqal_to:public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x != y; }
	};

	//����
	template<class T>
	struct greater:public binary_function<T,T,bool>
	{
		bool operator() (const T& x, const T&y) const { return x > y; }
	};

	//С��
	template<class T>
	struct less:public binary_function<T,T,bool>
	{
		bool operator() (const T& x, const T& y) const { return x < y; }
	};

	//���ڵ���
	template<class T>
	struct greater_equal:public binary_function<T,T,bool>
	{
		bool operator() (const T& x, const T& y) const { return x >= y; }
	};

	//С�ڵ���
	template<class T>
	struct less_equal :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T& y) const { return x <= y; }
	};

	//�߼���
	template<class T>
	struct logical_and :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T& y) const { return x && y; }
	};

	//�߼���
	template<class T>
	struct logical_or :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T& y) const { return x || y; }
	};

	//�߼���
	template<class T>
	struct logical_not :public unarg_function<T,bool> {
		bool operator() (const T& x) const { return !x; }
	};

	//֤ͬ����������ı�Ԫ�أ����ر���
	template<class T>
	struct identity:public unarg_function<T,bool>
	{
		const T& operator() (const T& x) const { return x; }
	};

	// ѡ����������һ�� pair�����ص�һ��Ԫ��
	template<class Pair>
	struct selectfirst :public unarg_function<Pair, typename Pair::first_type> {
		const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
	};

	// ѡ����������һ�� pair�����صڶ���Ԫ��
	template <class Pair>
	struct selectsecond :public unarg_function<Pair, typename Pair::second_type> {
		const typename Pair::second_type& operator() (const Pair& x) const { return x.second; }
	};

	//Ͷ����������ص�һ����
	template <class arg1, class arg2>
	struct projectfirst :public binary_function<arg1, arg2, arg1> {
		arg1 operator()(const arg1& x, const arg2& y) const { return x; }
	};

	//Ͷ����������صڶ�����
	template<class arg1,class arg2>
	struct projectsecond :public binary_function<arg1, arg2, arg2> {
		arg2 operator()(const arg1& x, const arg2& y) const { return y; }
	};
}
#endif // !_FUNCTIONAL_H_

