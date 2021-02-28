#pragma once
#ifndef _STACK_H_
#define _STACK_H_

#include"vector.h"
namespace myTinySTL {
	//class of stack
	template<class T,class Container=myTinySTL::vector<T>>
	class stack {
	public:
		typedef typename Container::value_type  value_type;
		typedef typename Container::reference_type reference;
		typedef typename Container::size_type size_type;
		typedef Container container_type;
	private:
		container_type container_;
	public:
		explicit stack(container_type& ctnr = container_type()) :container_(ctnr) {}
		bool empty()const { return container_.empty(); }
	};
}
#endif // _STACK_H_
