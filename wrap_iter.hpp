#ifndef WRAP_ITER_HPP
# define WRAP_ITER_HPP

# include "iterator.hpp"

namespace ft
{


	template <typename _Iter>
	class __wrap_iter
	{
		public:
			typedef _Iter														iterator_type;
			typedef typename iterator_traits<iterator_type>::value_type			value_type;
			typedef typename iterator_traits<iterator_type>::difference_type	difference_type;
			typedef typename iterator_traits<iterator_type>::pointer			pointer;
			typedef typename iterator_traits<iterator_type>::reference			reference;
			typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;
		
		private:
			iterator_type	__i;

		public:
			__wrap_iter() throw() {}
			template <typename _Up>
			__wrap_iter(const __wrap_iter<_Up>& __u,
				typename ft::enable_if<ft::is_convertible<_Up, iterator_type>::value>::type* = 0) throw()
				: __i(__u.base()) {}
			reference	operator*() const throw()
			{
				return (*__i);
			}
			pointer	operator->() const throw()
			{
				return ((pointer)ft::addressof(*__i));
			}
			__wrap_iter&	operator++() throw()
			{
				++__i;
				return (*this);
			}
			__wrap_iter	operator++(int) throw()
        	{
				__wrap_iter __tmp(*this);
				++(*this);
				return (__tmp);
			}
			__wrap_iter&	operator--() throw()
			{
				--__i;
				return (*this);
			}
			__wrap_iter	operator--(int) throw()
			{
				__wrap_iter __tmp(*this);
				--(*this);
				return (__tmp);
			}
			__wrap_iter	operator+ (difference_type __n) const throw()
			{
				__wrap_iter __w(*this);
				__w += __n;
				return (__w);
			}
			__wrap_iter&	operator+=(difference_type __n) throw()
			{
				__i += __n;
				return (*this);
			}
			__wrap_iter	operator- (difference_type __n) const throw()
			{
				return (*this + (-__n));
			}
			__wrap_iter&	operator-=(difference_type __n) throw()
			{
				*this += -__n;
				return (*this);
			}
			reference	operator[](difference_type __n) const throw()
			{
				return (__i[__n]);
			}
			iterator_type	base() const throw()
			{
				return (__i);
			}
			__wrap_iter(iterator_type __x) throw() : __i(__x) {}
	};

	template <class _Iter1>
	bool	operator==(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) throw()
	{
		return (__x.base() == __y.base());
	}

	template <class _Iter1, class _Iter2>
	bool	operator==(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return (__x.base() == __y.base());
	}

	template <class _Iter1>
	bool	operator<(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) throw()
	{
		return (__x.base() < __y.base());
	}

	template <class _Iter1, class _Iter2>
	bool	operator<(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return (__x.base() < __y.base());
	}

	template <class _Iter1>
	bool	operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) throw()
	{
		return !(__x == __y);
	}

	template <class _Iter1, class _Iter2>
	bool	operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return !(__x == __y);
	}

	template <class _Iter1>
	bool	operator>(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) throw()
	{
		return __y < __x;
	}

	template <class _Iter1, class _Iter2>
	bool	operator>(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return __y < __x;
	}

	template <class _Iter1>
	bool	operator>=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) throw()
	{
		return !(__x < __y);
	}

	template <class _Iter1, class _Iter2>
	bool	operator>=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return !(__x < __y);
	}

	template <class _Iter1>
	bool	operator<=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) throw()
	{
		return !(__y < __x);
	}

	template <class _Iter1, class _Iter2>
	bool	operator<=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return !(__y < __x);
	}
	
	template <class _Iter1, class _Iter2>
	typename __wrap_iter<_Iter1>::difference_type
	operator-(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) throw()
	{
		return (__x.base() - __y.base());
	}

	template <class _Iter1>
	__wrap_iter<_Iter1> operator+(typename __wrap_iter<_Iter1>::difference_type __n, __wrap_iter<_Iter1> __x) throw()
	{
		__x += __n;
		return (__x);
	}
}

#endif