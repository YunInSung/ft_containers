#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

# include <cstddef>
# include "debug.hpp"

namespace ft
{

	//addressof
	template <typename _Tp>
	_Tp*	addressof(_Tp& __x) throw()
	{
		return reinterpret_cast<_Tp *>(
			const_cast<char *>(&reinterpret_cast<const volatile char &>(__x)));
	}
	
	//integral_constant : false / true
	template <typename _Tp, _Tp __v>
	struct integral_constant
	{
		static const _Tp		value = __v;
		typedef _Tp				value_type;
	};

	template <bool __Val>
	struct _BoolConstant
	{
		typedef integral_constant<bool, __Val>	type;
	};

	typedef integral_constant<bool, true>				true_type;
	typedef integral_constant<bool, false>				false_type;
	
	//_Is_integer
	template <typename _Tp> struct _Is_integer 
	{
		typedef false_type _Integral;
	};

	template<> struct _Is_integer<bool> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<char> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<signed char>
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<unsigned char> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<wchar_t> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<short> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<unsigned short> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<int>
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<unsigned int>
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<long> 
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<unsigned long>
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<long long>
	{
		typedef true_type _Integral;
	};

	template<> struct _Is_integer<unsigned long long>
	{
		typedef true_type _Integral;
	};

	// enable_if
	template <bool, typename _Tp = void> struct enable_if {};
	template <typename _Tp> struct enable_if<true, _Tp> {typedef _Tp type;};

	// is_const
	template <typename _Tp> struct is_const				: public false_type {};
	template <typename _Tp> struct is_const<_Tp const>	: public true_type {};

	//__is_referenceable
	template <typename>
	struct __void_t { typedef void type; };

	template <typename _Tp, typename basis = void> 
	struct __is_referenceable_sub : public false_type
	{};

	template <typename _Tp> 
	struct __is_referenceable_sub<_Tp, typename __void_t<_Tp&>::type> : public true_type
	{};

	template <typename _Tp>
	struct __is_referenceable : integral_constant<bool, __is_referenceable_sub<_Tp>::value> {};

	// is_reference
	template <typename _Tp> struct is_reference				: public false_type {};
	template <typename _Tp> struct is_reference<_Tp&>		: public true_type {};
	// template <typename _Tp> struct is_reference<const _Tp&>	: public true_type {};
	
	// is_function
	template <typename _Tp> struct is_function
		: public _BoolConstant<!(is_reference<_Tp>::value || is_const<const _Tp>::value)>::type {};

	// is_array
	template <typename _Tp> struct is_array : public false_type {};
	template <typename _Tp> struct is_array<_Tp[]> : public true_type {};
	template <typename _Tp, size_t _Np> struct is_array<_Tp[_Np]> : public true_type {};

	// remove_const
	template <typename _Tp> struct remove_const				{typedef _Tp type;};
	template <typename _Tp> struct remove_const<const _Tp>	{typedef _Tp type;};

	// remove_volatile
	template <typename _Tp> struct remove_volatile					{typedef _Tp type;};
	template <typename _Tp> struct remove_volatile<volatile _Tp>	{typedef _Tp type;};

	// remove_cv
	template <typename _Tp> struct remove_cv
	{typedef typename remove_volatile<typename remove_const<_Tp>::type>::type type;};

	//is_integral

	template <class _Tp> struct __libcpp_is_integral                     : public false_type {};
	template <>          struct __libcpp_is_integral<bool>               : public true_type {};
	template <>          struct __libcpp_is_integral<char>               : public true_type {};
	template <>          struct __libcpp_is_integral<signed char>        : public true_type {};
	template <>          struct __libcpp_is_integral<unsigned char>      : public true_type {};
	template <>          struct __libcpp_is_integral<wchar_t>            : public true_type {};
	#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
	template <>          struct __libcpp_is_integral<char16_t>           : public true_type {};
	template <>          struct __libcpp_is_integral<char32_t>           : public true_type {};
	#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
	template <>          struct __libcpp_is_integral<short>              : public true_type {};
	template <>          struct __libcpp_is_integral<unsigned short>     : public true_type {};
	template <>          struct __libcpp_is_integral<int>                : public true_type {};
	template <>          struct __libcpp_is_integral<unsigned int>       : public true_type {};
	template <>          struct __libcpp_is_integral<long>               : public true_type {};
	template <>          struct __libcpp_is_integral<unsigned long>      : public true_type {};
	template <>          struct __libcpp_is_integral<long long>          : public true_type {};
	template <>          struct __libcpp_is_integral<unsigned long long> : public true_type {};
	#ifndef _LIBCPP_HAS_NO_INT128
	template <>          struct __libcpp_is_integral<__int128_t>         : public true_type {};
	template <>          struct __libcpp_is_integral<__uint128_t>        : public true_type {};
	#endif

	template <typename _Tp> struct is_integral
		: public ft::__libcpp_is_integral<typename remove_cv<_Tp>::type> {};

	// is_void
	template <typename _Tp>	struct __libcpp_is_void       : public false_type {};
	template <>				struct __libcpp_is_void<void> : public true_type {};

	template <typename _Tp> struct is_void
		: public __libcpp_is_void<typename remove_cv<_Tp>::type> {};

	// remove_reference
	template <typename _Tp>
	struct remove_reference
	{
		typedef _Tp type;
	};

	template <typename _Tp>
	struct remove_reference<_Tp&>
	{
		typedef _Tp type;
	};

	//__to_address
	template <typename _Tp>
	_Tp* __to_address(_Tp* __p) throw()
	{
		// ft_assert(!is_function<_Tp>::value, "_Tp is a function type");
		return __p;
	}

	template <typename _Tp>
	struct remove_reference<const _Tp&>
	{
		typedef _Tp type;
	};

	//is_convertible

	template <typename _Tp> _Tp	declval() throw();
	
	template <typename From, typename To>
	struct ___test_convert
	{
		private:
			template <typename _Tp> static char  __test_convert(...);
			template <typename _Tp> static int  __test_convert(_Tp);

		public:
			static const bool value = (sizeof(__test_convert<To>(declval<From>())) == 4);
	};

	template <typename From, typename To, bool = ___test_convert<From, To>::value>
	struct _test_convert : public true_type
	{};
	template <typename From, typename To> 
	struct _test_convert<From, To, false> : public false_type
	{};

	template <typename _From, typename _To, bool basis = _test_convert<_From, _To>::value>
	struct __is_convertible_test : public false_type {};

	template <typename _From, typename _To>
	struct __is_convertible_test<_From, _To, true> : public true_type {};
	
	template <typename _Tp, bool _IsArray =    is_array<_Tp>::value,
					bool _IsFunction = is_function<_Tp>::value,
					bool _IsVoid =     is_void<_Tp>::value>
					struct __is_array_function_or_void                          {enum {value = 0};};
	template <typename _Tp> struct __is_array_function_or_void<_Tp, true, false, false> {enum {value = 1};};
	template <typename _Tp> struct __is_array_function_or_void<_Tp, false, true, false> {enum {value = 2};};
	template <typename _Tp> struct __is_array_function_or_void<_Tp, false, false, true> {enum {value = 3};};

	template <typename _Tp,
    unsigned = __is_array_function_or_void<typename remove_reference<_Tp>::type>::value>
	struct __is_convertible_check
	{
		static const size_t __v = 0;
	};

	template <typename _Tp>
	struct __is_convertible_check<_Tp, 0>
	{
		static const size_t __v = sizeof(_Tp);
	};

	template <typename _T1, typename _T2,
		unsigned _T1_is_array_function_or_void = __is_array_function_or_void<_T1>::value,
		unsigned _T2_is_array_function_or_void = __is_array_function_or_void<_T2>::value>
	struct ___is_convertible
		: public integral_constant<bool,
			__is_convertible_test<_T1, _T2>::value
		>
	{};

	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 0, 1> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 1, 1> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 2, 1> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 3, 1> : public false_type {};

	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 0, 2> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 1, 2> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 2, 2> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 3, 2> : public false_type {};

	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 0, 3> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 1, 3> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 2, 3> : public false_type {};
	template <typename _T1, typename _T2> struct ___is_convertible<_T1, _T2, 3, 3> : public true_type {};

	template <typename _T1, typename _T2> struct is_convertible
		: public ___is_convertible<_T1, _T2>
	{
		static const size_t __complete_check1 = __is_convertible_check<_T1>::__v;
		static const size_t __complete_check2 = __is_convertible_check<_T2>::__v;
	};

	// uses_allocator

	template <typename _Tp>
	struct __has_allocator_type
	{
	private:
		struct __two {char __lx; char __lxx;};
		template <typename _Up> static __two __test(...);
		template <typename _Up> static char __test(typename _Up::allocator_type* = 0);
	public:
		static const bool value = sizeof(__test<_Tp>(0)) == 1;
	};

	template <typename _Tp, typename _Alloc, bool = __has_allocator_type<_Tp>::value>
	struct __uses_allocator
		: public integral_constant<bool, ft::is_convertible<_Alloc, typename _Tp::allocator_type>::value>
	{
	};

	template <typename _Tp, typename _Alloc>
	struct __uses_allocator<_Tp, _Alloc, false>
		: public false_type
	{
	};

	template <typename _Tp, typename _Alloc>
	struct _LIBCPP_TEMPLATE_VIS uses_allocator
		: public __uses_allocator<_Tp, _Alloc>
	{
	};

}

#endif
