#ifndef SPLIT_BUFFER_HPP
# define SPLIT_BUFFER_HPP

# include <memory>
# include "type_traits.hpp"
# include "algorithm.hpp"
# include "nullptr.hpp"

namespace ft
{
	template <typename _Tp, typename _Allocator>
	class split_buffer
	{
		public:
			typedef _Tp												value_type;
			typedef _Allocator										allocator_type;
			typedef typename remove_reference<allocator_type>::type	__alloc_rr;
			typedef std::allocator_traits<__alloc_rr>				__alloc_traits;
			typedef value_type&										reference;
			typedef const value_type&								const_reference;
			typedef typename __alloc_traits::difference_type		difference_type;
			typedef typename __alloc_traits::size_type				size_type;
			typedef typename __alloc_traits::pointer				pointer;
			typedef typename __alloc_traits::const_pointer			const_pointer;
			typedef pointer											iterator;
			typedef const_pointer									const_iterator;

			pointer													__first_;
			pointer													__begin_;
			pointer													__end_;
			pointer													__end_of_storage;
			allocator_type											__data_allocator;

			__alloc_rr& __alloc() throw()
			{
				return __data_allocator;
			}

			const __alloc_rr& __alloc() const throw()
			{
				return static_cast<const __alloc_rr&>(__data_allocator);
			}

			pointer& __end_cap() throw()
			{
				return __end_of_storage;
			}

			const pointer& __end_cap() const throw()
			{
				return static_cast<const pointer&>(__end_of_storage);
			}

			size_type	capacity() const 
			{
				return static_cast<size_type>(__end_cap() - __first_);
			}

			void clear()
			{
				__destruct_at_end(__begin_);
			}
			
			void __destruct_at_end(pointer __new_last) throw()
			{
				__destruct_at_end(__new_last, false_type());
			}

			void __destruct_at_end(pointer __new_last, false_type) throw()
			{
				while (__new_last != __end_)
					__alloc_traits::destroy(__alloc(), ft::__to_address(--__end_));
			}

			void __destruct_at_end(pointer __new_last, true_type) throw()
			{
				__end_ = __new_last;
			}

			split_buffer(size_type __cap, size_type __start, __alloc_rr& __a) : __end_of_storage(0), __data_allocator(__a)
			{
				__first_ = __cap != 0 ? __alloc_traits::allocate(__alloc(), __cap) : 0;
				__begin_ = __end_ = __first_ + __start;
				__end_cap() = __first_ + __cap;
			}

			~split_buffer()
			{
				clear();
				if (__first_)
					__alloc_traits::deallocate(__alloc(), __first_, capacity());
			}

			struct _ConstructTransaction 
			{
				explicit _ConstructTransaction(pointer* __p, size_type __n) throw() : __pos_(*__p), __end_(*__p + __n), __dest_(__p) {}
				~_ConstructTransaction() 
				{
					*__dest_ = __pos_;
				}
				pointer			__pos_;
				const pointer	__end_;
				
				private:
					pointer *__dest_;
			};

			void __construct_at_end(size_type __n)
			{
				_ConstructTransaction __tx(&this->__end_, __n);
				for (; __tx.__pos_ != __tx.__end_; ++__tx.__pos_) {
					__alloc_traits::construct(this->__alloc(), ft::__to_address(__tx.__pos_));
				}
			}

			void __construct_at_end(size_type __n, const_reference __x)
			{
				_ConstructTransaction __tx(&this->__end_, __n);
				for (; __tx.__pos_ != __tx.__end_; ++__tx.__pos_) 
				{
					__alloc_traits::construct(this->__alloc(), ft::__to_address(__tx.__pos_), __x);
				}
			}

			template <typename _InputIter>
			void __construct_at_end(_InputIter __first, _InputIter __last)
			{
				__alloc_rr& __a = this->__alloc();
				for (; __first != __last; ++__first)
				{
					if (__end_ == __end_cap())
					{
						size_type __old_cap = __end_cap() - __first_;
						size_type __new_cap = ft::max<size_type>(2 * __old_cap, 8);
						split_buffer __buf(__new_cap, 0, __a);
						for (pointer __p = __begin_; __p != __end_; ++__p, (void) ++__buf.__end_)
							__alloc_traits::construct(__buf.__alloc(), ft::__to_address(__buf.__end_), *__p);
						swap(__buf);
					}
					__alloc_traits::construct(__a, ft::__to_address(this->__end_), *__first);
					++this->__end_;
				}
			}

			void push_back(const_reference __x)
			{
				pointer	temp_begin = __begin_;
				pointer	temp_end = __end_;

				if (__end_ == __end_cap())
				{
					if (__begin_ > __first_)
					{
						difference_type __d = __begin_ - __first_;
						__d = (__d + 1) / 2;
						__end_ = clone(__begin_, __end_, __begin_ - __d);
						__begin_ -= __d;
					}
					else
					{
						size_type __c = ft::max<size_type>(2 * static_cast<size_t>(__end_cap() - __first_), 1);
						split_buffer<value_type, __alloc_rr&> __t(__c, __c / 4, __alloc());
						__t.__construct_at_end(temp_begin, temp_end);
						ft::swap(__first_, __t.__first_);
						ft::swap(__begin_, __t.__begin_);
						ft::swap(__end_, __t.__end_);
						ft::swap(__end_cap(), __t.__end_cap());
					}
				}
				__alloc_traits::construct(__alloc(), ft::__to_address(__end_), __x);
				++__end_;
			}
			
			void swap(split_buffer& __x)
			{
				ft::swap(__first_, __x.__first_);
				ft::swap(__begin_, __x.__begin_);
				ft::swap(__end_, __x.__end_);
				ft::swap(__end_cap(), __x.__end_cap());
				ft::swap(__alloc(), __x.__alloc());
			}

	};
};

#endif