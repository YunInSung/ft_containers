#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft
{
	template <typename _Tp, typename _Container = vector<_Tp> > class stack;

	template <typename _Tp, typename _Container>
	bool operator==(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y);

	template <typename _Tp, typename _Container>
	bool operator< (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y);

	template <typename _Tp, typename _Container>
	class stack
	{
		public:
			typedef _Container									container_type;
			typedef typename container_type::value_type			value_type;
			typedef typename container_type::reference			reference;
			typedef typename container_type::const_reference	const_reference;
			typedef typename container_type::size_type			size_type;

		protected:
			container_type c;

		public:
			stack() : c() {}

			stack(const stack& __q) : c(__q.c) {}

			stack& operator=(const stack& __q)
			{
				c = __q.c;
				return *this;
			}

			explicit stack(const container_type& __c) : c(__c) {}

			template <typename _Alloc>
			explicit stack(const _Alloc& __a, typename enable_if<uses_allocator<container_type, _Alloc>::value>::type* = 0) : c(__a) {}
			template <typename _Alloc>
			stack(const container_type& __c, const _Alloc& __a, typename enable_if<uses_allocator<container_type, _Alloc>::value>::type* = 0) : c(__c, __a) {}
			template <typename _Alloc>
			stack(const stack& __s, const _Alloc& __a, typename enable_if<uses_allocator<container_type, _Alloc>::value>::type* = 0) : c(__s.c, __a) {}

			bool empty()     const      
			{
				return c.empty();
			}

			size_type size() const      
			{
				return c.size();
			}

			reference top()             
			{
				return c.back();
			}

			const_reference top() const 
			{
				return c.back();
			}

			void push(const value_type& __v) 
			{
				c.push_back(__v);
			}

			void pop()
			{
				c.pop_back();
			}

			const container_type& getC(void) const
			{
				return (this->c);
			}
	};


	template <typename _Tp, typename _Container>
	bool operator==(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
	{
		return __x.getC() == __y.getC();
	}

	template <typename _Tp, typename _Container>
	bool operator< (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
	{
		return __x.getC() < __y.getC();
	}

	template <typename _Tp, typename _Container>
	bool operator!=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
	{
		return !(__x == __y);
	}

	template <typename _Tp, typename _Container>
	bool operator> (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
	{
		return __y < __x;
	}

	template <typename _Tp, typename _Container>
	bool operator>=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
	{
		return !(__x < __y);
	}

	template <typename _Tp, typename _Container>
	bool operator<=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
	{
		return !(__y < __x);
	}

	template <typename _Tp, typename _Container, typename _Alloc>
	struct uses_allocator<stack<_Tp, _Container>, _Alloc>
		: public uses_allocator<_Container, _Alloc>
	{
	};
};

#endif