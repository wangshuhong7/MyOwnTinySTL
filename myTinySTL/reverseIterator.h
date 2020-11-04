#pragma once
#ifndef _REVERSE_ITERATOR_H_
#define _REVERSE_ITERATOR_H_
#include "iterator.h"

namespace myTinySTL {
	template<class Iterator>
	class reverse_iterator_t {
	public:
		typedef Iterator iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef const pointer const_pointer;
		typedef typename iterator_traits<Iterator>::reference reference;
		typedef const reference const_reference;
	private:
		Iterator base_;
		Iterator cur_;
	public :
		//默认构造函数
		reverse_iterator_t():base_(0),cur_(0){}
		//explicit阻止隐式的类型转换，此函数只能被显示调用
		explicit reverse_iterator_t(const iterator_type& it) :base(it) {
			auto temp = it;
			cur = --temp;//由于是逆向遍历，所以地址向前移动
		}
		//默认构造函数
		template<class Iter>
		reverse_iterator_t(const reverse_iterator_t<Iter>& rev_it) {
			base_ = (iterator_type)rev_it.base_();
			auto temp = base_;
			cur_ = --temp;
		}
		iterator_type base() { return base_; }//返回反转后的最前端
		reference operator*() { return(*cur_); }//返回当前指针
		const_reference operator*() const { return (*cur_); }//对常迭代器返回当前指针
		pointer operator->() { return &(operator*()); }//返回当前指针的引用
		const_pointer operator->() { return &(operator*()); }//返回当前指针的常引用
		reverse_iterator_t& operator++() {
			--base_;
			--cur_;//由于是反向迭代器，所以向前反而是--
		}
		reverse_iterator_t& operator++(int) {
			reverse_iterator_t temp = *this;
			++(*this);//对指针本身调用++（其实a++本质是先保存a为temp，然后调用++a。最后返回temp）
			return temp;//返回之前的指针
		}
		reverse_iterator_t& operator--() {
			++base_;
			++cur_;
			return *this;
		}

		reverse_iterator_t& operator--(int) {
			reverse_iterator_t temp = *this;
			++(*this);
			return temp;
		}

		reference operator[](difference_type n) {
			return base()[-n - 1];
		}


	private:
		//对于random_access_iterator 和 bidirectional_iterator进行额外处理
		//random_access_iterator 可以直接跳转
		Iterator advanceNStep(Iterator it, difference_type n, bool right, random_access_iterator_tag) {
			if (right) {
				it += n;//true -> \ false <-
			}
			else {
				it -= n;
			}
			return it;
		}
		//对于 bidirectional_iterator必须一步一步移动
		Iterator advanceNStep(Iterator it, difference_type n, bool right, bidirectional_iterator_tag) {
			difference_type i;
			difference_type absN = n >= 0 ? n : -n;
			if ((right&&n > 0) || (!right&&n < 0)) {
				for (i = 0; i !=absN; i++) {
					it = it + 1;
				}
			}
			else if ((!right && n > 0) || (right && n < 0)) {
				for (i = 0; i != absN; i++) {
					it = it - 1;
				}
			}
			return it;
		}
	public:
		template<class Iterator>
		friend bool operator==(const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		template<class Iterator>
		friend bool operator!=(const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		template<class Iterator>
		friend bool operator <(const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		template<class Iterator>
		friend bool operator <=(const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		template<class Iterator>
		friend bool operator >(const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		template<class Iterator>
		friend bool operator >=(const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		template<class Iterator>
		friend reverse_iterator_t<Iterator> operator+(
			typename reverse_iterator_t<Iterator>::difference_type n,
			const reverse_iterator_t<Iterator>& rev_it);
		template<class Iterator>
		friend typename reverse_iterator_t<Iterator>::difference_type operator-(
			const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs
			);
		reverse_iterator_t operator +(difference_type n)const;
		reverse_iterator_t operator -(difference_type n)const;
		reverse_iterator_t& operator +=(difference_type n);
		reverse_iterator_t& operator -=(difference_type n);
	};//end of reverse_iterator_it

	template<class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator +(difference_type n) const{
		reverse_iterator_t<Iterator> res = *this;
		res += n;
		return res;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator -(difference_type n) const {
		reverse_iterator_t<Iterator> res = *this;
		res -= n;
		return res;
	}
	template<class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator+=(difference_type n) {
		base_ = advanceNStep(base_,n,false,iterator_category());//调用之前的泛型函数实现
		cur_ = advanceNStep(base_, n, false, iterator_category());
		return *this;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator-=(difference_type n) {
		base_ = advanceNStep(base_, n, true, iterator_category());
		cur_ = advanceNStep(base_, n, true, iterator_category());
		return *this;
	}

	template<class Iterator>
	bool operator==(const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return lhs.cur_ == rhs.cur_;
	}

	template<class Iterator>
	bool operator!=(const reverse_iterator_t<Iterator>&lhs, const reverse_iterator_t<Iterator>& rhs) {
		return !(lhs == rhs);
	}

	template<class Iterator>
	bool operator<(const reverse_iterator_t<Iterator>&lhs, const reverse_iterator_t<Iterator>&rhs) {
		return lhs.cur_ < rhs.cur_;
	}

	template<class Iterator>
	bool operator>(const reverse_iterator_t<Iterator>&lhs, const reverse_iterator_t<Iterator>& rhs) {
		return lhs.cur_ > rhs.cur_;
	}

	template<class Iterator>
	bool operator >=(const reverse_iterator_t<Iterator>&lhs, const reverse_iterator_t<Iterator>& rhs) {
		return !(lhs < rhs);
	}

	template<class Iterator>
	bool operator <=(const reverse_iterator_t<Iterator>&lhs, const reverse_iterator_t<Iterator>&rhs) {
		return !(lhs > rhs);
	}

	template<class Iterator>
	reverse_iterator_t<Iterator> operator+(
		typename reverse_iterator_t<Iterator>::difference_type n,
		const reverse_iterator_t<Iterator>& rev_it) {
		return rev_it + n;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator>operator-(
		const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs
		) {
		return lhs.cur_ - rhs.cur_;
	}
}
#endif // _REVERSE_ITERATOR_H_


