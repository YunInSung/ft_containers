#ifndef STL_FUNCTION_HPP
# define STL_FUNCTION_HPP

namespace ft
{

	template <typename _Arg1, typename _Arg2, typename _Result>
	struct binary_function
	{
		typedef _Arg1	first_argument_type;
		typedef _Arg2	second_argument_type;
		typedef _Result	result_type;
	};

	template <typename _Arg, typename _Result>
	struct unary_function 
	{
		typedef _Arg	argument_type;
		typedef _Result	result_type;
	};

	template <class _Tp>
	struct _Identity : public unary_function<_Tp,_Tp> 
	{
		_Tp& operator()(_Tp& __x) const { return __x; }
		const _Tp& operator()(const _Tp& __x) const { return __x; }
	};


	template <class _Pair>
	struct _Select1st : public unary_function<_Pair, typename _Pair::first_type> 
	{
		typename _Pair::first_type& operator()(_Pair& __x) const 
		{
			return __x.first;
		}
		const typename _Pair::first_type& operator()(const _Pair& __x) const 
		{
			return __x.first;
		}
	};
};

#endif