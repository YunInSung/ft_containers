#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include "type_traits.hpp"
# include <algorithm>
# include <iterator>

namespace ft
{
	// struct input_iterator_tag {};
	// struct output_iterator_tag {};
	// struct forward_iterator_tag			: public input_iterator_tag {};
	// struct bidirectional_iterator_tag	: public forward_iterator_tag {};
	// struct random_access_iterator_tag	: public bidirectional_iterator_tag {};

	template <typename _Iter, bool>
	struct __iterator_traits_impl {};

	template <typename _Iter, bool>
	struct __iterator_traits {};

	template <typename _Iter>
	struct __iterator_traits_impl<_Iter, true>
	{
		typedef typename _Iter::difference_type		difference_type;
		typedef typename _Iter::value_type			value_type;
		typedef typename _Iter::pointer				pointer;
		typedef typename _Iter::reference			reference;
		typedef typename _Iter::iterator_category	iterator_category;
	};

	template <typename _Iter>
	struct __iterator_traits<_Iter, true>
		: __iterator_traits_impl
		<
			_Iter,
			ft::is_convertible<typename _Iter::iterator_category, std::input_iterator_tag>::value ||
			ft::is_convertible<typename _Iter::iterator_category, std::output_iterator_tag>::value
		>
	{};

	template <typename _Tp>
	struct __has_iterator_typedefs
	{
		private:
			struct __two {char __lx; char __lxx;};
			template <typename _Up> static __two __test(...);
			template <typename _Up> static char __test(typename __void_t<typename _Up::iterator_category>::type* = 0,
													typename __void_t<typename _Up::difference_type>::type* = 0,
													typename __void_t<typename _Up::value_type>::type* = 0,
													typename __void_t<typename _Up::reference>::type* = 0,
													typename __void_t<typename _Up::pointer>::type* = 0
													);
		public:
			static const bool value = (sizeof(__test<_Tp>(0,0,0,0,0)) == 1);
	};

	template <typename _Iter>
	struct iterator_traits : __iterator_traits<_Iter, __has_iterator_typedefs<_Iter>::value>
	{
		typedef iterator_traits	__primary_template;
	};

	template<typename _Tp>
	struct iterator_traits<_Tp*>
	{
		typedef ptrdiff_t							difference_type;
		typedef typename remove_cv<_Tp>::type		value_type;
		typedef _Tp*								pointer;
		typedef _Tp&								reference;
		typedef std::random_access_iterator_tag		iterator_category;
	};

	template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
			typename _Pointer = _Tp*, typename _Reference = _Tp&>
	struct iterator
	{
		typedef _Tp			value_type;
		typedef _Distance	difference_type;
		typedef _Pointer	pointer;
		typedef _Reference	reference;
		typedef _Category	iterator_category;
	};

	template <typename _InputIter>
	typename iterator_traits<_InputIter>::difference_type _distance(_InputIter __first, _InputIter __last, std::input_iterator_tag)
	{
		typename iterator_traits<_InputIter>::difference_type __r(0);
		for (; __first != __last; ++__first)
			++__r;
		return __r;
	}

	template <typename _RandIter>
	typename iterator_traits<_RandIter>::difference_type _distance(_RandIter __first, _RandIter __last, std::random_access_iterator_tag)
	{
		return __last - __first;
	}

	template <typename _InputIter>
	typename iterator_traits<_InputIter>::difference_type distance(_InputIter __first, _InputIter __last)
	{
		return ft::_distance(__first, __last, typename iterator_traits<_InputIter>::iterator_category());
	}

	template <class _InputIter>
	void _advance(_InputIter& __i, typename iterator_traits<_InputIter>::difference_type __n, std::input_iterator_tag)
	{
		for (; __n > 0; --__n)
			++__i;
	}

	template <class _BiDirIter>
	void _advance(_BiDirIter& __i, typename iterator_traits<_BiDirIter>::difference_type __n, std::bidirectional_iterator_tag)
	{
		if (__n >= 0)
			for (; __n > 0; --__n)
				++__i;
		else
			for (; __n < 0; ++__n)
				--__i;
	}

	template <class _RandIter>
	void _advance(_RandIter& __i, typename iterator_traits<_RandIter>::difference_type __n, std::random_access_iterator_tag)
	{
		__i += __n;
	}

	template <class _InputIter>
	void advance(_InputIter& __i, typename iterator_traits<_InputIter>::difference_type __n)
	{
		ft::_advance(__i, __n, typename iterator_traits<_InputIter>::iterator_category());
	}

	template <typename _Iter>
	typename iterator_traits<_Iter>::iterator_category
	__iterator_category(const _Iter&)
	{
		typedef typename iterator_traits<_Iter>::iterator_category _Category;
		return _Category();
	}

	template <typename _Tp>
	struct __has_iterator_category
	{
		private:
			struct __two {char __lx; char __lxx;};
			template <typename _Up> static __two __test(...);
			template <typename _Up> static char __test(typename _Up::iterator_category* = 0);
		public:
			static const bool value = sizeof(__test<_Tp>(0)) == 1;
	};

	template <typename _Tp, typename _Up, bool = __has_iterator_category<iterator_traits<_Tp> >::value>
	struct __has_iterator_category_convertible_to
		: public integral_constant<bool, ft::is_convertible<typename iterator_traits<_Tp>::iterator_category, _Up>::value>
	{};

	template <typename _Tp, typename _Up>
	struct __has_iterator_category_convertible_to<_Tp, _Up, false> : public false_type {};

	template <typename _Tp>
	struct __is_cpp17_input_iterator : public __has_iterator_category_convertible_to<_Tp, std::input_iterator_tag> {};

	template <typename _Tp>
	struct __is_cpp17_forward_iterator : public __has_iterator_category_convertible_to<_Tp, std::forward_iterator_tag> {};
}

#endif
