#pragma once
#ifndef _FUNCTIONAL_H_
#define _FUNCTIONNAL_H_
#include<cstddef>
namespace myTinySTL {
	//定义一元函数的参数型别和返回值类型
	template<class arg,class result>
	struct unarg_function {
		typedef arg argument_type;
		typedef result result_type;
	};

	//定义二元函数的参数型别和返回值类型
	template<class arg1,class arg2,class result>
	struct binary_function {
		typedef arg1 first_argument_type;
		typedef arg2 second_argument_type;
		typedef result result_type;
	};

	//函数对象：加法
	template<class T>
	struct plus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x + y; }
	};

	//减法
	template<class T>
	struct minus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x - y; }
	};

	//乘法
	template<class T>
	struct multiple :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const { return x * y; }
	};

	//除法
	template<class T>
	struct divide :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x / y;
		}
	};

	//取模
	template<class T>
	struct modulus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x%y;
		}
	};

	//否定
	template<class T>
	struct negate :public unarg_function<T, T> {
		T operator()(const T& x) const { return -x; }
	};

	//加法的证同元素
	template<class T>
	T identity_element(plus<T>) { return T(0); }

	//乘法证同
	template<class T>
	T identity_element(multiple<T>) { return T(1); }

	//等于
	template<class T>
	struct equal_to :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T&y) const{ return x == y; }
	};

	//不等于
	template<class T>
	struct not_euqal_to:public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const { return x != y; }
	};

	//大于
	template<class T>
	struct greater:public binary_function<T,T,bool>
	{
		bool operator() (const T& x, const T&y) const { return x > y; }
	};

	//小于
	template<class T>
	struct less:public binary_function<T,T,bool>
	{
		bool operator() (const T& x, const T& y) const { return x < y; }
	};

	//大于等于
	template<class T>
	struct greater_equal:public binary_function<T,T,bool>
	{
		bool operator() (const T& x, const T& y) const { return x >= y; }
	};

	//小于等于
	template<class T>
	struct less_equal :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T& y) const { return x <= y; }
	};

	//逻辑与
	template<class T>
	struct logical_and :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T& y) const { return x && y; }
	};

	//逻辑或
	template<class T>
	struct logical_or :public binary_function<T, T, bool> {
		bool operator() (const T& x, const T& y) const { return x || y; }
	};

	//逻辑非
	template<class T>
	struct logical_not :public unarg_function<T,bool> {
		bool operator() (const T& x) const { return !x; }
	};

	//证同函数：不会改变元素，返回本身
	template<class T>
	struct identity:public unarg_function<T,bool>
	{
		const T& operator() (const T& x) const { return x; }
	};

	// 选择函数：接受一个 pair，返回第一个元素
	template<class Pair>
	struct selectfirst :public unarg_function<Pair, typename Pair::first_type> {
		const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
	};

	// 选择函数：接受一个 pair，返回第二个元素
	template <class Pair>
	struct selectsecond :public unarg_function<Pair, typename Pair::second_type> {
		const typename Pair::second_type& operator() (const Pair& x) const { return x.second; }
	};

	//投射参数，返回第一参数
	template <class arg1, class arg2>
	struct projectfirst :public binary_function<arg1, arg2, arg1> {
		arg1 operator()(const arg1& x, const arg2& y) const { return x; }
	};

	//投射参数，返回第二参数
	template<class arg1,class arg2>
	struct projectsecond :public binary_function<arg1, arg2, arg2> {
		arg2 operator()(const arg1& x, const arg2& y) const { return y; }
	};
}
#endif // !_FUNCTIONAL_H_

