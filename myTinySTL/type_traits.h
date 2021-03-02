#pragma once
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
// 这个头文件用于提取类型信息
//POD适用于需要明确的数据底层操作的系统中。POD通常被用在系统的边界处，即指
//不同系统之间只能以底层数据的形式进行交互，系统的高层逻辑不能互相兼容。
//比如当对象的字段值是从外部数据中构建时，系统还没有办法对对象进行语义检查和解释，于是就需要POD类型

namespace myTinySTL {
	/*namespace {
		template<bool,class Ta,class Tb>
		struct IfThenElse;
		template<class Ta, class Tb>
		struct IfThenElse <true,Ta,Tb>{
			using result = Ta;
		};
		template<class  Ta,class Tb>
		struct IfThenElse<false,Ta,Tb> {
			using result = Tb;
		};
	}
}*/
	struct _true_type {};
	struct _false_type {};
	//萃取传入的T类型的类型特性
	template<class T>
	struct _type_traits {
		typedef _false_type has_trivial_default_constructor;
		typedef _false_type has_trivial_copy_constructor;
		typedef _false_type has_trivial_assignment_operator;
		typedef _false_type has_trivial_destructor;
		typedef _false_type is_POD_type;
	};
	template<>
	struct _type_traits<bool>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;

	};
	template<>
	struct _type_traits<char>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<wchar_t>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<short>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned short>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<int>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned int>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<long>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<long long>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long long>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<double>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<long double>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<class T>
	struct _type_traits<T*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<class T>
	struct _type_traits<const T*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<char*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<const char*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<const unsigned char*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	template<>
	struct _type_traits<const signed char*> {
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	// helper struct

	template <class T, T v>
	struct m_integral_constant
	{
		static constexpr T value = v;
	};

	template <bool b>
	using m_bool_constant = m_integral_constant<bool, b>;

	typedef m_bool_constant<true>  m_true_type;
	typedef m_bool_constant<false> m_false_type;

	/*****************************************************************************************/
	// type traits

	// is_pair

	// --- forward declaration begin
	template <class T1, class T2>
	struct pair;
	// --- forward declaration end

	template <class T>
	struct is_pair : myTinySTL::m_false_type {};

	template <class T1, class T2>
	struct is_pair<myTinySTL::pair<T1, T2>> : myTinySTL::m_true_type {};

}
#endif // !_TYPE_TRAITS_H_
