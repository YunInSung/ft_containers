#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "split_buffer.hpp"
# include "iter.hpp"
# include "nullptr.hpp"
# include "lexicographical_compare.hpp"
# include "equal.hpp"

namespace ft
{
	template <typename _Tp, typename _Allocator>
	class __vector_base
	{
		public:
			typedef _Allocator									allocator_type;
			typedef std::allocator_traits<allocator_type>		__alloc_traits;
			typedef typename __alloc_traits::size_type			size_type;

			void	__throw_length_error(void) const;
			void	__throw_out_of_range(void) const;
		protected:
			typedef _Tp											value_type;
			typedef	value_type&									reference;
			typedef const value_type&							const_reference;
			typedef typename __alloc_traits::difference_type	difference_type;
			typedef typename __alloc_traits::pointer			pointer;
			typedef typename __alloc_traits::const_pointer		const_pointer;
			typedef pointer										iterator;
			typedef const_pointer								const_iterator;

			pointer											__begin_;
			pointer											__end_;
			pointer											__end_of_storage;
			allocator_type									__data_allocator;

			allocator_type& __alloc() throw()
			{
				return __data_allocator;
			}

			const allocator_type& __alloc() const throw()
			{
				return static_cast<const allocator_type&>(__data_allocator);
			}

			pointer& __end_cap() throw()
			{
				return __end_of_storage;
			}

			const pointer& __end_cap() const throw()
			{
				return static_cast<const pointer&>(__end_of_storage);
			}

			__vector_base()
				: __begin_(0), __end_(0), __end_of_storage(0) {}

			__vector_base(const allocator_type& __a)
				: __begin_(0), __end_(0), __end_of_storage(0), __data_allocator(__a) {}
			
			~__vector_base()
			{
				if (__begin_ != 0)
				{
					clear();
					__alloc_traits::deallocate(__alloc(), __begin_, capacity());
				}
			}

			void clear() throw()
			{
				__destruct_at_end(__begin_);
			}

			size_type capacity() const throw()
			{
				return static_cast<size_type>(__end_cap() - __begin_);
			}

			void __destruct_at_end(pointer __new_last) throw()
			{
				pointer __soon_to_be_end = __end_;
				while (__new_last != __soon_to_be_end)
					__alloc_traits::destroy(__alloc(), ft::__to_address(--__soon_to_be_end));
				__end_ = __new_last;
			}

			void __copy_assign_alloc(const __vector_base& __c)
			{
				__copy_assign_alloc(__c, integral_constant<bool,
						__alloc_traits::propagate_on_container_copy_assignment::value>());
			}

		private:
			void __copy_assign_alloc(const __vector_base& __c, true_type)
			{
				if (__alloc() != __c.__alloc())
				{
					clear();
					__alloc_traits::deallocate(__alloc(), __begin_, capacity());
					__begin_ = __end_ = __end_cap() = 0;
				}
				__alloc() = __c.__alloc();
			}
			void __copy_assign_alloc(const __vector_base&, false_type)
        	{}

	};

	template <typename _Tp, typename _Allocator>
	void	__vector_base<_Tp, _Allocator>::__throw_length_error(void) const
	{
		throw std::length_error("vector");
	}

	template <typename _Tp, typename _Allocator>
	void	__vector_base<_Tp, _Allocator>::__throw_out_of_range(void) const
	{
		throw std::out_of_range("vector");
	}

	template <typename _Tp, typename _Allocator = std::allocator<_Tp> >
	class vector : private __vector_base<_Tp, _Allocator>
	{
		private:
			typedef	__vector_base<_Tp, _Allocator>				__base;
			typedef std::allocator<_Tp>							__default_allocator_type;
		public:
			typedef vector										__self;
			typedef _Tp											value_type;
			typedef _Allocator									allocator_type;
			typedef typename __base::__alloc_traits				__alloc_traits;
			typedef typename __base::reference					reference;
			typedef typename __base::const_reference			const_reference;
			typedef typename __base::size_type					size_type;
			typedef typename __base::difference_type			difference_type;
			typedef typename __base::pointer					pointer;
			typedef typename __base::const_pointer				const_pointer;
			typedef __wrap_iter<pointer>						iterator;
			typedef __wrap_iter<const_pointer>					const_iterator;
			typedef ft::reverse_iterator<iterator>				reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

			vector() {}

			explicit vector(const allocator_type& __a) throw() : __base(__a) {}

			explicit vector(size_type __n)
			{
				if (__n > 0)
				{
					__vallocate(__n);
					__construct_at_end(__n);
				}
			}

			vector(size_type __n, const value_type& __x)
			{
				if (__n > 0)
				{
					__vallocate(__n);
					__construct_at_end(__n, __x);
				}
			}

			vector(size_type __n, const value_type& __x, const allocator_type& __a) : __base(__a)
			{
				if (__n > 0)
				{
					__vallocate(__n);
					__construct_at_end(__n, __x);
				}
			}

			template <class _InputIterator>
			vector(_InputIterator __first,
			typename enable_if<	__is_cpp17_input_iterator  <_InputIterator>::value &&
								!__is_cpp17_forward_iterator<_InputIterator>::value,
								_InputIterator>::type __last)
			{
				for (; __first != __last; ++__first)
					__emplace_back(*__first);
			}

			template <class _ForwardIterator>
			vector(_ForwardIterator __first,
			typename enable_if<	__is_cpp17_forward_iterator<_ForwardIterator>::value,
								_ForwardIterator>::type __last)
			{
				size_type __n = static_cast<size_type>(ft::distance(__first, __last));
				if (__n > 0)
				{
					__vallocate(__n);
					__construct_at_end(__first, __last, __n);
				}
			}

			~vector() {}

			vector(const vector& __x)
			{
				size_type __n = __x.size();
				if (__n > 0)
				{
					__vallocate(__n);
					__construct_at_end(__x.__begin_, __x.__end_, __n);
				}
			}

			vector(const vector& __x, const allocator_type& __a)
				: __base(__a)
			{
				size_type __n = __x.size();
				if (__n > 0)
				{
					__vallocate(__n);
					__construct_at_end(__x.__begin_, __x.__end_, __n);
				}
			}
			vector& operator=(const vector& __x)
			{
				if (this != &__x)
				{
					__base::__copy_assign_alloc(__x);
					assign(__x.__begin_, __x.__end_);
				}
				return *this;
			}

			void __emplace_back(const value_type& __x) 
			{ push_back(__x); }

			template <class _ForwardIterator>
			typename enable_if
			<
				__is_cpp17_input_iterator<_ForwardIterator>::value &&
				!__is_cpp17_forward_iterator<_ForwardIterator>::value,
				void
			>::type	assign(_ForwardIterator __first, _ForwardIterator __last)
			{
				clear();
				for (; __first != __last; ++__first)
					__emplace_back(*__first);
			}

			template <class _ForwardIterator>
			typename enable_if
			<
				__is_cpp17_forward_iterator<_ForwardIterator>::value,
				void
			>::type	assign(_ForwardIterator __first, _ForwardIterator __last)
			{
				size_type __new_size = static_cast<size_type>(ft::distance(__first, __last));
				if (__new_size <= capacity())
				{
					_ForwardIterator __mid = __last;
					bool __growing = false;
					if (__new_size > size())
					{
						__growing = true;
						__mid =  __first;
						ft::advance(__mid, size());
					}
					pointer __m = ft::copy(__first, __mid, this->__begin_);
					if (__growing)
						__construct_at_end(__mid, __last, __new_size - size());
					else
						this->__destruct_at_end(__m);
				}
				else
				{
					__vdeallocate();
					__vallocate(__recommend(__new_size));
					__construct_at_end(__first, __last, __new_size);
				}
			}

			void assign(size_type __n, const_reference __u)
			{
				if (__n <= capacity())
				{
					size_type __s = size();
					ft::fill_n(this->__begin_, ft::min(__n, __s), __u);
					if (__n > __s)
						__construct_at_end(__n - __s, __u);
					else
						this->__destruct_at_end(this->__begin_ + __n);
				}
				else
				{
					__vdeallocate();
					__vallocate(__recommend(static_cast<size_type>(__n)));
					__construct_at_end(__n, __u);
				}
			}

			allocator_type	get_allocator() const throw()
        	{
				return this->__alloc();
			}

			iterator				begin() throw()
			{
				return __make_iter(this->__begin_);
			}

			const_iterator			begin() const throw()
			{
				return __make_iter(this->__begin_);
			}

			iterator				end() throw()
			{
				return __make_iter(this->__end_);
			}

			const_iterator			end() const throw()
			{
				return __make_iter(this->__end_);
			}

			reverse_iterator		rbegin() throw()
			{
				return reverse_iterator(end());
			}

			const_reverse_iterator	rbegin() const throw()
			{
				return const_reverse_iterator(end());
			}

			reverse_iterator		rend() throw()
			{
				return reverse_iterator(begin());
			}

			const_reverse_iterator	rend() const throw()
			{
				return const_reverse_iterator(begin());
			}

			const_iterator			cbegin() const throw()
			{
				return begin();
			}

			const_iterator			cend() const throw()
			{
				return end();
			}

			const_reverse_iterator	crbegin() const throw()
			{
				return rbegin();
			}

			const_reverse_iterator	crend() const throw()
			{
				return rend();
			}

			size_type size() const throw()
			{
				return static_cast<size_type>(this->__end_ - this->__begin_);
			}

			size_type capacity() const throw()
			{
				return __base::capacity();
			}

			bool empty() const throw()
			{
				return this->__begin_ == this->__end_;
			}

			size_type max_size() const throw()
			{
				return ft::min<size_type>(__alloc_traits::max_size(this->__alloc()),
											std::numeric_limits<difference_type>::max());
			}

			void reserve(size_type __n)
			{
				if (__n > capacity())
				{
					allocator_type& __a = this->__alloc();
					split_buffer<value_type, allocator_type&> __v(__n, size(), __a);
					__swap_out_circular_buffer(__v);
				}
			}

			reference		operator[](size_type __n) throw()
			{
				return this->__begin_[__n];
			}

			const_reference	operator[](size_type __n) const throw()
			{
				return this->__begin_[__n];
			}

			reference		at(size_type __n)
			{
				if (__n >= size())
					this->__throw_out_of_range();
				return this->__begin_[__n];
			}
			
			const_reference	at(size_type __n) const
			{
				if (__n >= size())
					this->__throw_out_of_range();
				return this->__begin_[__n];
			}

			reference		front() throw()
			{
				return *this->__begin_;
			}

			const_reference	front() const throw()
			{
				return *this->__begin_;
			}

			reference		back() throw()
			{
				return *(this->__end_ - 1);
			}
			
			const_reference	back()  const throw()
			{
				return *(this->__end_ - 1);
			}

			value_type*			data() throw()
			{
				return ft::__to_address(this->__begin_);
			}

			const value_type*	data() const throw()
			{
				return ft::__to_address(this->__begin_);
			}

			void	push_back(const_reference __x)
			{
				if (this->__end_ != this->__end_cap())
				{
					__construct_one_at_end(__x);
				}
				else
					__push_back_slow_path(__x);
			}

			void	pop_back()
			{
				this->__destruct_at_end(this->__end_ - 1);
			}

			iterator	insert(iterator __position, const_reference __x)
			{
				pointer __p = this->__begin_ + (__position - begin());
				if (this->__end_ < this->__end_cap())
				{
					if (__p == this->__end_)
					{
						__construct_one_at_end(__x);
					}
					else
					{
						__move_range(__p, this->__end_, __p + 1);
						const_pointer __xr = ft::addressof(__x);
						if (__p <= __xr && __xr < this->__end_)
							++__xr;
						*__p = *__xr;
					}
				}
				else
				{
					allocator_type& __a = this->__alloc();
					split_buffer<value_type, allocator_type&> __v(__recommend(size() + 1), __p - this->__begin_, __a);
					__v.push_back(__x);
					__p = __swap_out_circular_buffer(__v, __p);
				}
				return __make_iter(__p);
			}

			iterator	insert(iterator __position, size_type __n, const_reference __x)
			{
				pointer __p = this->__begin_ + (__position - begin());
				if (__n > 0)
				{
					if (__n <= static_cast<size_type>(this->__end_cap() - this->__end_))
					{
						size_type __old_n = __n;
						pointer __old_last = this->__end_;
						if (__n > static_cast<size_type>(this->__end_ - __p))
						{
							size_type __cx = __n - (this->__end_ - __p);
							__construct_at_end(__cx, __x);
							__n -= __cx;
						}
						if (__n > 0)
						{
							__move_range(__p, __old_last, __p + __old_n);
							const_pointer __xr = ft::addressof(__x);
							if (__p <= __xr && __xr < this->__end_)
								__xr += __old_n;
							ft::fill_n(__p, __n, *__xr);
						}
					}
					else
					{
						allocator_type& __a = this->__alloc();
						split_buffer<value_type, allocator_type&> __v(__recommend(size() + __n), __p - this->__begin_, __a);
						__v.__construct_at_end(__n, __x);
						__p = __swap_out_circular_buffer(__v, __p);
					}
				}
				return __make_iter(__p);
			}

			template <typename _InputIterator>
			typename enable_if
			<
				__is_cpp17_input_iterator  <_InputIterator>::value &&
				!__is_cpp17_forward_iterator<_InputIterator>::value,
				void
			>::type
			insert(iterator __pos, _InputIterator __first, _InputIterator __last) 
			{
				for ( ; __first != __last; ++__first) 
				{
					__pos = insert(__pos, *__first);
					++__pos;
				}
			}

			void __move_range(pointer __from_s, pointer __from_e, pointer __to)
			{
				pointer			__old_last = this->__end_;
				difference_type	__n = __old_last - __to;
				{
					pointer __i = __from_s + __n;
					_ConstructTransaction __tx(*this, __from_e - __i);
					for (; __i < __from_e; ++__i, (void) ++__tx.__pos_) 
					{
						__alloc_traits::construct(this->__alloc(), ft::__to_address(__tx.__pos_), *__i);
					}
				}
				clone_backward(__from_s, __from_s + __n, __old_last);
			}


			template <class _ForwardIterator>
			typename enable_if
			<
				__is_cpp17_forward_iterator<_ForwardIterator>::value,
				iterator
			>::type
			insert(const_iterator __position, _ForwardIterator __first, _ForwardIterator __last)
			{
				pointer __p = this->__begin_ + (__position - begin());
				difference_type __n = ft::distance(__first, __last);
				if (__n > 0)
				{
					if (__n <= this->__end_cap() - this->__end_)
					{
						size_type __old_n = __n;
						pointer __old_last = this->__end_;
						_ForwardIterator __m = __last;
						difference_type __dx = this->__end_ - __p;
						if (__n > __dx)
						{
							__m = __first;
							difference_type __diff = this->__end_ - __p;
							ft::advance(__m, __diff);
							__construct_at_end(__m, __last, __n - __diff);
							__n = __dx;
						}
						if (__n > 0)
						{
							__move_range(__p, __old_last, __p + __old_n);
							ft::copy(__first, __m, __p);
						}
					}
					else
					{
						allocator_type& __a = this->__alloc();
						ft::split_buffer<value_type, allocator_type&> __v(__recommend(size() + __n), __p - this->__begin_, __a);
						__v.__construct_at_end(__first, __last);
						__p = __swap_out_circular_buffer(__v, __p);
					}
				}
				return __make_iter(__p);
			}

			iterator	erase(const_iterator __position)
			{
				difference_type __ps = __position - cbegin();
				pointer __p = this->__begin_ + __ps;
				this->__destruct_at_end(clone(__p + 1, this->__end_, __p));
				iterator __r = __make_iter(__p);
				return __r;
			}

    		iterator	erase(const_iterator __first, const_iterator __last)
			{
				pointer __p = this->__begin_ + (__first - begin());
				if (__first != __last) {
					this->__destruct_at_end(clone(__p + (__last - __first), this->__end_, __p));
				}
				iterator __r = __make_iter(__p);
				return __r;
			}

			void clear() throw()
			{
				// size_type __old_size = size();
				__base::clear();
			}

			void resize(size_type __sz)
			{
				size_type __cs = size();
				if (__cs < __sz)
					this->__append(__sz - __cs);
				else if (__cs > __sz)
					this->__destruct_at_end(this->__begin_ + __sz);
			}

			void resize(size_type __sz, const_reference __x)
			{
				size_type __cs = size();
				if (__cs < __sz)
					this->__append(__sz - __cs, __x);
				else if (__cs > __sz)
					this->__destruct_at_end(this->__begin_ + __sz);
			}

			void swap(vector& __x)
			{
				ft::swap(this->__begin_, __x.__begin_);
				ft::swap(this->__end_, __x.__end_);
				ft::swap(this->__end_cap(), __x.__end_cap());
				ft::swap(this->__alloc(), __x.__alloc());
			}

			bool __invariants() const
			{
				if (this->__begin_ == 0)
				{
					if (this->__end_ != 0 || this->__end_cap() != 0)
						return false;
				}
				else
				{
					if (this->__begin_ > this->__end_)
						return false;
					if (this->__begin_ == this->__end_cap())
						return false;
					if (this->__end_ > this->__end_cap())
						return false;
				}
				return true;
			}
		private:
			void __vallocate(size_type __n)
			{
				if (__n > max_size())
					this->__throw_length_error();
				this->__begin_ = this->__end_ = __alloc_traits::allocate(this->__alloc(), __n);
				// this->__begin_ = this->__end_ = ::operator new(__n);
				this->__end_cap() = this->__begin_ + __n;
			}

			void __vdeallocate() throw()
			{
				if (this->__begin_ != 0)
				{
					clear();
					__alloc_traits::deallocate(this->__alloc(), this->__begin_, capacity());
					this->__begin_ = this->__end_ = this->__end_cap() = 0;
				}
			}

			size_type __recommend(size_type __new_size) const
			{
				const size_type __ms = max_size();
				if (__new_size > __ms)
					this->__throw_length_error();
				const size_type __cap = capacity();
				if (__cap >= __ms / 2)
					return __ms;
				return ft::max<size_type>(2*__cap, __new_size);
			}

			void __construct_at_end(size_type __n)
			{
				_ConstructTransaction __tx(*this, __n);
				for (; __tx.__pos_ != __tx.__new_end_; ++__tx.__pos_) 
				{
					__alloc_traits::construct(this->__alloc(), ft::__to_address(__tx.__pos_));
				}
			}
			void __construct_at_end(size_type __n, const_reference __x)
			{
				_ConstructTransaction __tx(*this, __n);
				for (; __tx.__pos_ != __tx.__new_end_; ++__tx.__pos_) 
				{
					__alloc_traits::construct(this->__alloc(), ft::__to_address(__tx.__pos_), __x);
				}
			}
			template <class _ForwardIterator>
			void __construct_at_end(_ForwardIterator __first, _ForwardIterator __last, size_type __n)
			{
				_ConstructTransaction __tx(*this, __n);
				__alloc_traits::__construct_range_forward(this->__alloc(), __first, __last, __tx.__pos_);
			}

			void __append(size_type __n)
			{
				if (static_cast<size_type>(this->__end_cap() - this->__end_) >= __n)
					this->__construct_at_end(__n);
				else
				{
					allocator_type& __a = this->__alloc();
					split_buffer<value_type, allocator_type&> __v(__recommend(size() + __n), size(), __a);
					__v.__construct_at_end(__n);
					__swap_out_circular_buffer(__v);
				}
			}

			void __append(size_type __n, const_reference __x)
			{
				if (static_cast<size_type>(this->__end_cap() - this->__end_) >= __n)
					this->__construct_at_end(__n, __x);
				else
				{
					allocator_type& __a = this->__alloc();
					split_buffer<value_type, allocator_type&> __v(__recommend(size() + __n), size(), __a);
					__v.__construct_at_end(__n, __x);
					__swap_out_circular_buffer(__v);
				}
			}

			iterator		__make_iter(pointer __p) throw()
			{
				return iterator(__p);
			}

			const_iterator	__make_iter(const_pointer __p) const throw()
			{
				return const_iterator(__p);
			}

			template <typename _Ptr>
			void __construct_backward_with_exception_guarantees(allocator_type& __a, _Ptr __begin1, _Ptr __end1, _Ptr& __end2)
			{
				while (__end1 != __begin1)
				{
					__alloc_traits::construct(__a, ft::__to_address(__end2 - 1), *--__end1);
					--__end2;
				}
			}

			template <typename _Ptr>
			void __construct_forward_with_exception_guarantees(allocator_type& __a, _Ptr __begin1, _Ptr __end1, _Ptr& __begin2)
			{
				for (; __begin1 != __end1; ++__begin1, (void) ++__begin2)
				__alloc_traits::construct(__a, ft::__to_address(__begin2), *__begin1);
			}

			void __swap_out_circular_buffer(split_buffer<value_type, allocator_type&>& __v)
			{
				__construct_backward_with_exception_guarantees(this->__alloc(), this->__begin_, this->__end_, __v.__begin_);
				ft::swap(this->__begin_, __v.__begin_);
				ft::swap(this->__end_, __v.__end_);
				ft::swap(this->__end_cap(), __v.__end_cap());
				__v.__first_ = __v.__begin_;
			}

			pointer __swap_out_circular_buffer(split_buffer<value_type, allocator_type&>& __v, pointer __p)
			{
				pointer __r = __v.__begin_;
				__construct_backward_with_exception_guarantees(this->__alloc(), this->__begin_, __p, __v.__begin_);
				__construct_forward_with_exception_guarantees(this->__alloc(), __p, this->__end_, __v.__end_);
				ft::swap(this->__begin_, __v.__begin_);
				ft::swap(this->__end_, __v.__end_);
				ft::swap(this->__end_cap(), __v.__end_cap());
				__v.__first_ = __v.__begin_;
				return __r;
			}

			void __destruct_at_end(pointer __new_last) throw()
			{
				__base::__destruct_at_end(__new_last);
			}

			struct _ConstructTransaction
			{
				explicit _ConstructTransaction(vector &__v, size_type __n)
				: __v_(__v),  __pos_(__v.__end_), __new_end_(__v.__end_ + __n) {}
				~_ConstructTransaction() 
				{
					__v_.__end_ = __pos_;
				}

				vector& 			__v_;
				pointer				__pos_;
				const_pointer const	__new_end_;

				private:
					_ConstructTransaction(_ConstructTransaction const&);
					_ConstructTransaction& operator=(_ConstructTransaction const&);
			};

			template <typename _Up>
			void __construct_one_at_end(const _Up& __value)
			{
				_ConstructTransaction __tx(*this, 1);
				__alloc_traits::construct(this->__alloc(), ft::__to_address(__tx.__pos_), __value);
				++__tx.__pos_;
			}

			template <typename _Up>
			void __push_back_slow_path(_Up& __x)
			{
				allocator_type& __a = this->__alloc();
				split_buffer<value_type, allocator_type&> __v(__recommend(size() + 1), size(), __a);
				__alloc_traits::construct(__a, ft::__to_address(__v.__end_), __x);
				__v.__end_++;
				__swap_out_circular_buffer(__v);
			}
	};

	template <class _Tp, class _Allocator>
	bool operator==(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y)
	{
		const typename vector<_Tp, _Allocator>::size_type __sz = __x.size();
		return __sz == __y.size() && ft::equal(__x.begin(), __x.end(), __y.begin());
	}

	template <class _Tp, class _Allocator>
	bool operator!=(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y)
	{
		return !(__x == __y);
	}

	template <class _Tp, class _Allocator>
	bool operator< (const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y)
	{
		return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
	}

	template <class _Tp, class _Allocator>
	bool operator> (const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y)
	{
		return __y < __x;
	}

	template <class _Tp, class _Allocator>
	bool operator>=(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y)
	{
		return !(__x < __y);
	}

	template <class _Tp, class _Allocator>
	bool operator<=(const vector<_Tp, _Allocator>& __x, const vector<_Tp, _Allocator>& __y)
	{
		return !(__y < __x);
	}

	template <class _Tp, class _Alloc>
	void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
	{
		__x.swap(__y);
	}

};

#endif