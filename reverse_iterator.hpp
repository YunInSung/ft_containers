#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator.hpp"

namespace ft
{
	template <typename _Iter>
	class reverse_iterator
		: public ft::iterator<typename iterator_traits<_Iter>::iterator_category,
							typename iterator_traits<_Iter>::value_type,
							typename iterator_traits<_Iter>::difference_type,
							typename iterator_traits<_Iter>::pointer,
							typename iterator_traits<_Iter>::reference>
	{
		private:
			_Iter	__t; // no longer used as of LWG #2360, not removed due to ABI break
		protected:
			_Iter														current;
		public:
			typedef _Iter												iterator_type;
			typedef typename iterator_traits<_Iter>::difference_type	difference_type;
			typedef typename iterator_traits<_Iter>::reference			reference;
			typedef typename iterator_traits<_Iter>::pointer			pointer;

			reverse_iterator() : __t(), current() {}
			reverse_iterator(_Iter __x) : __t(__x), current(__x) {}
			template <typename _Up>
			reverse_iterator(const reverse_iterator<_Up>& __u) : __t(__u.base()), current(__u.base()) {}
			template <typename _Up>
			reverse_iterator& operator=(const reverse_iterator<_Up>& __u)
			{
				__t = current = __u.base();
				return (*this);
			}
			_Iter	base() const
			{
				return (current);
			}
			reference			operator*() const
			{
				_Iter	__tmp = current;
				return (*--__tmp);
			}
			pointer				operator->() const
			{
				return (ft::addressof(operator*()));
			}
			reverse_iterator&	operator++()
			{
				--current;
				return (*this);
			}
			reverse_iterator	operator++(int)
			{
				reverse_iterator	__tmp(*this);
				--current;
				return (__tmp);
			}
			reverse_iterator&	operator--()
			{
				++current;
				return (*this);
			}
			reverse_iterator	operator--(int)
			{
				reverse_iterator	__tmp(*this);
				++current;
				return (__tmp);
			}
			reverse_iterator	operator+ (difference_type __n) const
			{
				return (reverse_iterator(current - __n));
			}
			reverse_iterator&	operator+=(difference_type __n)
			{
				current -= __n;
				return (*this);
			}
			reverse_iterator	operator- (difference_type __n) const
			{
				return (reverse_iterator(current + __n));
			}
			reverse_iterator&	operator-=(difference_type __n)
			{
				current += __n;
				return (*this);
			}
			reference			operator[](difference_type __n) const
			{
				return *(*this + __n);
			}
	};
	template <class _Iter1, class _Iter2>
	bool operator==(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() == __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool operator<(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() > __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool operator!=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() != __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool operator>(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() < __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool operator>=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() <= __y.base();
	}

	template <class _Iter1, class _Iter2>
	bool operator<=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __x.base() >= __y.base();
	}

	template <class _Iter1, class _Iter2>
	typename reverse_iterator<_Iter1>::difference_type operator-(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
	{
		return __y.base() - __x.base();
	}

	template <class _Iter>
	reverse_iterator<_Iter> operator+(typename reverse_iterator<_Iter>::difference_type __n, const reverse_iterator<_Iter>& __x)
	{
		return reverse_iterator<_Iter>(__x.base() - __n);
	}
};

#endif