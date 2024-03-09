#ifndef PAIR_HPP
# define PAIR_HPP

namespace ft
{
	template <typename _T1, typename _T2>
	struct pair 
	{
		typedef _T1 first_type;    ///<  @c first_type is the first bound type
		typedef _T2 second_type;   ///<  @c second_type is the second bound type

		_T1 first;                 ///< @c first is a copy of the first object
		_T2 second;                ///< @c second is a copy of the second object
		
		pair() : first(_T1()), second(_T2()) {}
		pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}

		template <typename _U1, typename _U2>
		pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}

		pair& operator=(const pair& __p)
		{
			first = __p.first;
			second = __p.second;
			return *this;
		}
	};

	template <typename _T1, typename _T2>
	bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
	{ 
		return __x.first == __y.first && __x.second == __y.second; 
	}

	template <typename _T1, typename _T2>
	bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
	{ 
		return __x.first < __y.first || 
				(!(__y.first < __x.first) && __x.second < __y.second); 
	}

	template <typename _T1, typename _T2>
	bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) 
	{
		return !(__x == __y);
	}

	template <typename _T1, typename _T2>
	bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) 
	{
		return __y < __x;
	}

	template <typename _T1, typename _T2>
	bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) 
	{
		return !(__y < __x);
	}

	template <typename _T1, typename _T2>
	bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) 
	{
		return !(__x < __y);
	}

	// template <typename _T1, typename _T2>
	// pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
	// {
	// 	return pair<_T1, _T2>(__x, __y);
	// }

	template <typename _T1, typename _T2>
	pair<_T1, _T2> make_pair(_T1 __x, _T2 __y)
	{
		return pair<_T1, _T2>(__x, __y);
	}
};

#endif