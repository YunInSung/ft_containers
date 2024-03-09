#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "lexicographical_compare.hpp"

namespace ft
{
	template <typename _Tp, typename _Compare>
	const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp)
	{
		return __comp(__b, __a) ? __b : __a;
	}

	template <typename _Tp>
	const _Tp& min(const _Tp& __a, const _Tp& __b)
	{
		return ft::min(__a, __b, __less<_Tp>());
	}

	template <typename _Tp, typename _Compare>
	const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp)
	{
		return __comp(__a, __b) ? __b : __a;
	}

	template <typename _Tp>
	const _Tp& max(const _Tp& __a, const _Tp& __b)
	{
		return ft::max(__a, __b, __less<_Tp>());
	}

	template<typename InputIterator, typename OutputIterator>
	OutputIterator	copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		while (first != last) 
		{
			*result = *first;
			++result;
			++first;
		}
		return result;
	}

	template <typename OutputIterator, typename Size, typename T>
	OutputIterator	fill_n(OutputIterator first, Size n, const T& val)
	{
		while (n > 0) 
		{
			*first = val;
			++first;
			--n;
		}
		return first;
	}

	template <typename _Tp>
	void	swap(_Tp& __a, _Tp& __b)
	{
		_Tp __tmp = __a;
		__a = __b;
		__b = __tmp;
	}

	template <typename _InputIterator, typename _OutputIterator>
	_OutputIterator clone(_InputIterator __first, _InputIterator __last, _OutputIterator __result)
	{
		for (; __first != __last; ++__first, (void) ++__result)
			*__result = *__first;
		return __result;
	}

	template <typename _InputIterator, typename _OutputIterator>
	_OutputIterator clone_backward(_InputIterator __first, _InputIterator __last, _OutputIterator __result)
	{
		while (__first != __last)
			*--__result = *--__last;
		return __result;
	}

};

#endif