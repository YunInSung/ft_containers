#ifndef LEXICOGRAPHICAL_CAMPARE_HPP
# define LEXICOGRAPHICAL_CAMPARE_HPP

# include "type_traits.hpp"
# include "iterator.hpp"

namespace ft
{
	template <class _T1, class _T2 = _T1>
	struct __less
	{
		bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}

		bool operator()(const _T1& __x, const _T2& __y) const {return __x < __y;}

		bool operator()(const _T2& __x, const _T1& __y) const {return __x < __y;}

		bool operator()(const _T2& __x, const _T2& __y) const {return __x < __y;}
	};

	template <class _T1>
	struct __less<_T1, _T1>
	{
		bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}
	};

	template <class _T1>
	struct __less<const _T1, _T1>
	{
		bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}
	};

	template <class _T1>
	struct __less<_T1, const _T1>
	{
		bool operator()(const _T1& __x, const _T1& __y) const {return __x < __y;}
	};

	template <typename _Tp, bool = ft::__is_referenceable<_Tp>::value> struct __add_lvalue_reference_impl	{ typedef _Tp	type; };
	template <typename _Tp> struct __add_lvalue_reference_impl<_Tp, true>	{ typedef _Tp&	type; };

	template <typename _Tp> struct add_lvalue_reference
	{
		typedef typename  __add_lvalue_reference_impl<_Tp>::type type;
	};

	template <typename _Comp>
	struct __comp_ref_type 
	{
		typedef typename add_lvalue_reference<_Comp>::type	type;
	};

	template <typename _Compare, typename _InputIterator1, typename _InputIterator2>
	bool __lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
							_InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
	{
		for (; __first2 != __last2; ++__first1, (void) ++__first2)
		{
			if (__first1 == __last1 || __comp(*__first1, *__first2))
				return true;
			if (__comp(*__first2, *__first1))
				return false;
		}
		return false;
	}

	template <typename _InputIterator1, typename _InputIterator2, typename _Compare>
	bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
							_InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
	{
		typedef typename __comp_ref_type<_Compare>::type _Comp_ref;
		return ft::__lexicographical_compare<_Comp_ref>(__first1, __last1, __first2, __last2, __comp);
	}

	template <typename _InputIterator1, typename _InputIterator2>
	bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
							_InputIterator2 __first2, _InputIterator2 __last2)
	{
		return ft::lexicographical_compare(__first1, __last1, __first2, __last2,
											__less<typename iterator_traits<_InputIterator1>::value_type,
													typename iterator_traits<_InputIterator2>::value_type>());
	}
};

#endif