#ifndef EQUAL_HPP
# define EQUAL_HPP

namespace ft 
{
	template <class _InputIter1, class _InputIter2>
	bool equal(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2)
	{
		for ( ; __first1 != __last1; ++__first1, (void) ++__first2)
			if (!(*__first1 == *__first2))
			return false;
		return true;
	}
}; 


#endif