#ifndef MAP_HPP
# define MAP_HPP

# include "rbtree.hpp"
# include "lexicographical_compare.hpp"
# include "stl_function.hpp"
# include "pair.hpp"
# include <memory>

namespace ft
{	
	template <typename _Key, typename _Tp, typename _Compare = __less<_Key>,
          typename _Alloc = std::allocator<pair<_Key, _Tp> > >
	class map
	{
		public:
		// typedefs:
			typedef _Key					key_type;
			typedef _Tp						data_type;
			typedef _Tp 					mapped_type;
			typedef pair<_Key, _Tp>	value_type;
			typedef _Compare				key_compare;
			
		class value_compare : public binary_function<value_type, value_type, bool>
		{
			friend class map<_Key,_Tp,_Compare,_Alloc>;
			protected :
				_Compare comp;
				value_compare(_Compare __c) : comp(__c) {}
			public:
				bool operator()(const value_type& __x, const value_type& __y) const 
				{
					return comp(__x.first, __y.first);
				}
		};

		private:
			typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
			_Rep_type _M_t;  // red-black tree representing map

			void __throw_out_of_range(const char*__msg) const
			{
				throw std::out_of_range(__msg);
			}
		public:
			typedef typename _Rep_type::pointer					pointer;
			typedef typename _Rep_type::const_pointer			const_pointer;
			typedef typename _Rep_type::reference				reference;
			typedef typename _Rep_type::const_reference			const_reference;
			typedef typename _Rep_type::iterator				iterator;
			typedef typename _Rep_type::const_iterator			const_iterator;
			typedef typename _Rep_type::reverse_iterator		reverse_iterator;
			typedef typename _Rep_type::const_reverse_iterator	const_reverse_iterator;
			typedef typename _Rep_type::size_type				size_type;
			typedef typename _Rep_type::difference_type			difference_type;
			typedef typename _Rep_type::allocator_type			allocator_type;

		// allocation/deallocation

		map() : _M_t(_Compare(), allocator_type()) {}
		explicit map(const _Compare& __comp, const allocator_type& __a = allocator_type()) : _M_t(__comp, __a) {}

		template <typename _InputIterator>
		map(_InputIterator __first, _InputIterator __last) : _M_t(_Compare(), allocator_type())
		{ _M_t.insert_unique(__first, __last); }

		template <typename _InputIterator>
		map(_InputIterator __first, _InputIterator __last, const _Compare& __comp, const allocator_type& __a = allocator_type()) : _M_t(__comp, __a)
		{ _M_t.insert_unique(__first, __last); }
		map(const map<_Key,_Tp,_Compare,_Alloc>& __x) : _M_t(__x._M_t) {}

		map<_Key,_Tp,_Compare,_Alloc>&
		operator=(const map<_Key, _Tp, _Compare, _Alloc>& __x)
		{
			_M_t = __x._M_t;
			return *this; 
		}

		// accessors:

		key_compare key_comp() const { return _M_t.key_comp(); }
		value_compare value_comp() const { return value_compare(_M_t.key_comp()); }
		allocator_type get_allocator() const { return _M_t.get_allocator(); }

		iterator begin() { return _M_t.begin(); }
		const_iterator begin() const { return _M_t.begin(); }
		iterator end() { return _M_t.end(); }
		const_iterator end() const { return _M_t.end(); }
		reverse_iterator rbegin() { return _M_t.rbegin(); }
		const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
		reverse_iterator rend() { return _M_t.rend(); }
		const_reverse_iterator rend() const { return _M_t.rend(); }
		bool empty() const { return _M_t.empty(); }
		size_type size() const { return _M_t.size(); }
		size_type max_size() const { return _M_t.max_size(); }
		_Tp& operator[](const key_type& __k) 
		{
			iterator __i = lower_bound(__k);
			// __i->first is greater than or equivalent to __k.
			if (__i == end() || key_comp()(__k, (*__i).first))
				__i = insert(__i, value_type(__k, _Tp()));
			return (*__i).second;
		}

		_Tp& at(const key_type& __k)
		{
			iterator __i = lower_bound(__k);
			if (__i == end() || key_comp()(__k, (*__i).first))
	 			__throw_out_of_range("map::at:  key not found");
			return (*__i).second;
		}

		const _Tp& at(const key_type& __k) const
		{
			const_iterator __i = lower_bound(__k);
			if (__i == end() || key_comp()(__k, (*__i).first))
	 			__throw_out_of_range("map::at:  key not found");
			return (*__i).second;
		}
		void swap(map<_Key,_Tp,_Compare,_Alloc>& __x) { _M_t.swap(__x._M_t); }

		// insert/erase

		pair<iterator,bool> insert(const value_type& __x) 
			{ return _M_t.insert_unique(__x); }
		iterator insert(iterator position, const value_type& __x)
			{ return _M_t.insert_unique(position, __x); }
		template <class _InputIterator>
		void insert(_InputIterator __first, _InputIterator __last) 
		{
			_M_t.insert_unique(__first, __last);
		}

		void erase(iterator __position) { _M_t.erase(__position); }
		size_type erase(const key_type& __x) { return _M_t.erase(__x); }
		void erase(iterator __first, iterator __last)
			{ _M_t.erase(__first, __last); }
		void clear() { _M_t.clear(); }

		// map operations:

		iterator find(const key_type& __x) { return _M_t.find(__x); }
		const_iterator find(const key_type& __x) const { return _M_t.find(__x); }
		size_type count(const key_type& __x) const {
			return _M_t.find(__x) == _M_t.end() ? 0 : 1; 
		}
		iterator lower_bound(const key_type& __x) {return _M_t.lower_bound(__x); }
		const_iterator lower_bound(const key_type& __x) const {
			return _M_t.lower_bound(__x); 
		}
		iterator upper_bound(const key_type& __x) {return _M_t.upper_bound(__x); }
		const_iterator upper_bound(const key_type& __x) const {
			return _M_t.upper_bound(__x); 
		}
		
		pair<iterator,iterator> equal_range(const key_type& __x) {
			return _M_t.equal_range(__x);
		}
		pair<const_iterator,const_iterator> equal_range(const key_type& __x) const {
			return _M_t.equal_range(__x);
		}

		template <typename _K1, typename _T1, typename _C1, typename _A1>
		friend bool operator== (const map<_K1, _T1, _C1, _A1>&,
								const map<_K1, _T1, _C1, _A1>&);
		template <typename _K1, typename _T1, typename _C1, typename _A1>
		friend bool operator< (const map<_K1, _T1, _C1, _A1>&,
								const map<_K1, _T1, _C1, _A1>&);
	};

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		bool operator==(const map<_Key,_Tp,_Compare,_Alloc>& __x, const map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			return __x._M_t == __y._M_t;
		}

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		bool operator<(const map<_Key,_Tp,_Compare,_Alloc>& __x, const map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			return __x._M_t < __y._M_t;
		}

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		bool operator!=(const map<_Key,_Tp,_Compare,_Alloc>& __x, const map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			return !(__x == __y);
		}

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		bool operator>(const map<_Key,_Tp,_Compare,_Alloc>& __x, const map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			return __y < __x;
		}

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		bool operator<=(const map<_Key,_Tp,_Compare,_Alloc>& __x, const map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			return !(__y < __x);
		}

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		bool operator>=(const map<_Key,_Tp,_Compare,_Alloc>& __x, const map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			return !(__x < __y);
		}

		template <typename _Key, typename _Tp, typename _Compare, typename _Alloc>
		void swap(map<_Key,_Tp,_Compare,_Alloc>& __x, map<_Key,_Tp,_Compare,_Alloc>& __y) 
		{
			__x.swap(__y);
		}
};

#endif
