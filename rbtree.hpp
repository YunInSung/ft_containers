#ifndef RBTREE_HPP
# define RBTREE_HPP

# include "pair.hpp"
# include "iterator.hpp"
# include "reverse_iterator.hpp"
# include "nullptr.hpp"
# include "algorithm.hpp"
# include "equal.hpp"
# include <iostream>

namespace ft
{
	enum _RbtreeColorType
	{
		RED = 0,
		BLACK = 1,
		DOUBLE_BLACK = 2
	};

	class _Rb_tree_node_base
	{
		public:
			typedef unsigned char		_ColorType;
			typedef _Rb_tree_node_base*	_Base_ptr;

			_ColorType	_color;
			_Base_ptr	_parent;
			_Base_ptr	_left;
			_ColorType	_leftColor;
			_Base_ptr	_right;
			_ColorType	_rightColor;

			static _Base_ptr _S_minimum(_Base_ptr __x)
			{
				while (__x->_left != 0)
					__x = __x->_left;
				return __x;
			}

			static _Base_ptr _S_maximum(_Base_ptr __x)
			{
				while (__x->_right != 0)
					__x = __x->_right;
				return __x;
			}

	};

	template <typename _Value>
	class _Rb_tree_node : public _Rb_tree_node_base
	{
		public:
			typedef _Rb_tree_node<_Value>*	_Link_type;
			_Value _value;
	};

	class _Rb_tree_base_iterator
	{
		public:
			typedef _Rb_tree_node_base::_Base_ptr		_Base_ptr;
			typedef std::bidirectional_iterator_tag		iterator_category;
			typedef ptrdiff_t							difference_type;
			_Base_ptr								_M_node;

			void _M_increment()
			{
				if (_M_node->_right != 0) // 하강 - 노드의 오른쪽 서브트리가 존재할 경우
				{
					_M_node = _M_node->_right; // 노드의 오른쪽 서브트리에서
					while (_M_node->_left != 0) // 가장 작은 값을 찾는다.
						_M_node = _M_node->_left;
				} // 노드의 successor를 찾는다.
				else // 상승 - 노드의 오른쪽 트리가 존재하지 않는 경우
				{
					_Base_ptr __y = _M_node->_parent;
					while (_M_node == __y->_right) // 노드가 부모의 왼쪽 자녀가 될때까지 계속 올라간다. 
					{
						_M_node = __y;
						__y = __y->_parent;
					}
					if (_M_node->_right != __y) // 노드가 루트노드가 아니라면(즉, 부모노드와 노드 값이 다를때)
						_M_node = __y; // 노드를 부모노드와 동치시킨다.
				}
			}

			void _M_decrement()
			{
				if (_M_node->_color == RED && _M_node->_parent->_parent == _M_node)
					_M_node = _M_node->_right; // ?
				else if (_M_node->_left != 0) // 하강 - 노드의 왼쪽 서브트리가 존재할 경우
				{
					_Base_ptr __y = _M_node->_left;
					while (__y->_right != 0) // 노드의 왼쪽 서브 트리중 가장 큰 값을 찾는다.
						__y = __y->_right;
					_M_node = __y;
				}
				else // 상승 - 노드의 왼쪽 서브트리가 없을 경우
				{
					_Base_ptr __y = _M_node->_parent;
					while (_M_node == __y->_left) // 노드가 부모의 오른쪽 자녀가 될때까지 계속 올라간다.
					{
						_M_node = __y;
						__y = __y->_parent;
					}
					_M_node = __y; // 노드의 부모와 노드를 동치시킨다.
				}
			}

	};

	template <typename _Value, typename _Ref, typename _Ptr>
	struct _Rb_tree_iterator : public _Rb_tree_base_iterator
	{
		typedef _Value													value_type;
		typedef _Ref													reference;
		typedef _Ptr													pointer;
		typedef _Rb_tree_iterator<_Value, _Value&, _Value*>				iterator;
		typedef _Rb_tree_iterator<_Value, const _Value&, const _Value*>	const_iterator;
		typedef _Rb_tree_iterator<_Value, _Ref, _Ptr>					_Self;
		typedef _Rb_tree_node<_Value>*									_Link_type;
		typedef _Rb_tree_base_iterator::iterator_category				iterator_category;
		typedef _Rb_tree_base_iterator::difference_type					difference_type;

		_Rb_tree_iterator() {}
		_Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
		_Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }

		reference operator*() const { return _Link_type(_M_node)->_value; }
		pointer operator->() const { return &(operator*()); }

		_Self& operator++() { _M_increment(); return *this; }
		_Self operator++(int) 
		{
			_Self __tmp = *this;
			_M_increment();
			return __tmp;
		}
			
		_Self& operator--() { _M_decrement(); return *this; }
		_Self operator--(int) 
		{
			_Self __tmp = *this;
			_M_decrement();
			return __tmp;
		}

	};

	template <typename _Value, typename _Ref, typename _Ptr>
	bool operator==(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
				const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) 
	{
		return __x._M_node == __y._M_node;
	}

	template <typename _Value>
	bool operator==(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
				const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) 
	{
		return __x._M_node == __y._M_node;
	}

	template <typename _Value>
	bool operator==(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
				const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y)
	{
		return __x._M_node == __y._M_node;
	}

	template <typename _Value, typename _Ref, typename _Ptr>
	bool operator!=(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
				const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) 
	{
		return __x._M_node != __y._M_node;
	}

	template <typename _Value>
	bool operator!=(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
				const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y)
	{
		return __x._M_node != __y._M_node;
	}

	template <typename _Value>
	bool operator!=(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
				const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y)
	{
		return __x._M_node != __y._M_node;
	}

	// void	PlusBlack(_Rb_tree_node_base* __x)
	// {
	// 	__x->_color += BLACK;
	// 	if (__x->_parent->_parent != __x)
	// 	{
	// 		if (__x->_parent->_left == __x)
	// 			__x->_parent->_leftColor += BLACK;
	// 		else
	// 			__x->_parent->_rightColor += BLACK;
	// 	}
	// 	if (__x->_left != 0)
	// 		__x->_left->_color -= BLACK;
	// 	__x->_leftColor -= BLACK;
	// 	if (__x->_right != 0)
	// 		__x->_right->_color -= BLACK;
	// 	__x->_rightColor -= BLACK;
	// }

	// void	MinusBlack(_Rb_tree_node_base* __x)
	// {
	// 	__x->_color -= BLACK;
	// 	if (__x->_parent->_parent != __x)
	// 	{
	// 		if (__x->_parent->_left == __x)
	// 			__x->_parent->_leftColor -= BLACK;
	// 		else
	// 			__x->_parent->_rightColor -= BLACK;
	// 	}
	// 	if (__x->_left != 0)
	// 		__x->_left->_color += BLACK;
	// 	__x->_leftColor += BLACK;
	// 	if (__x->_right != 0)
	// 		__x->_right->_color += BLACK;
	// 	__x->_rightColor += BLACK;
	// }

	// void	ClockWiseRotate(_Rb_tree_node_base* __x)
	// {
	// 	_Rb_tree_node_base*	_parent = __x->_parent;
	// 	_Rb_tree_node_base*	_GrandParent = _parent->_parent;
	// 	_Rb_tree_node_base*	__xRight = __x->_right;
	// 	unsigned char		__xColor = __x->_color;

	// 	__x->_color = _parent->_color;
	// 	_parent->_color = __xColor;

	// 	_parent->_left = __xRight;
	// 	_parent->_leftColor = __x->_rightColor;
	// 	if (__xRight != 0)
	// 		__xRight->_parent = _parent;

	// 	if (_parent->_parent->_parent != _parent)
	// 	{
	// 		if (_GrandParent->_left == _parent)
	// 		{
	// 			_GrandParent->_left = __x;
	// 			_GrandParent->_leftColor = __x->_color;
	// 		}
	// 		else
	// 		{
	// 			_GrandParent->_right = __x;
	// 			_GrandParent->_rightColor = __x->_color;
	// 		}
	// 	}
	// 	else
	// 		_GrandParent->_parent = __x;
	// 	__x->_parent = _GrandParent;

	// 	__x->_right = _parent;
	// 	__x->_rightColor = _parent->_color;
	// 	_parent->_parent = __x;
	// }

	// void	CounterClockWiseRotate(_Rb_tree_node_base* __x)
	// {
	// 	_Rb_tree_node_base*	_parent = __x->_parent;
	// 	_Rb_tree_node_base*	_GrandParent = _parent->_parent;
	// 	_Rb_tree_node_base*	__xLeft = __x->_left;
	// 	unsigned char		__xColor = __x->_color;

	// 	__x->_color = _parent->_color;
	// 	_parent->_color = __xColor;

	// 	_parent->_right = __xLeft;
	// 	_parent->_rightColor = __x->_leftColor;
	// 	if (__xLeft != 0)
	// 		__xLeft->_parent = _parent;

	// 	if (_parent->_parent->_parent != _parent)
	// 	{
	// 		if (_GrandParent->_left == _parent)
	// 		{
	// 			_GrandParent->_left = __x;
	// 			_GrandParent->_leftColor = __x->_color;
	// 		}
	// 		else
	// 		{
	// 			_GrandParent->_right = __x;
	// 			_GrandParent->_rightColor = __x->_color;
	// 		}
	// 	}
	// 	else
	// 		_GrandParent->_parent = __x;
	// 	__x->_parent = _GrandParent;

	// 	__x->_left = _parent;
	// 	__x->_leftColor = _parent->_color;
	// 	_parent->_parent = __x;
	// }

	// void	RootBlack(_Rb_tree_node_base* __x)
	// {
	// 	if (__x->_color == RED || __x->_color == DOUBLE_BLACK)
	// 		__x->_color = BLACK;
	// }

	// void	insert_rebalance(_Rb_tree_node_base* __x)
	// {
	// 	_Rb_tree_node_base* _parent;
	// 	_Rb_tree_node_base* _grandParent;
	// 	while (1)
	// 	{
	// 		_parent = __x->_parent;
	// 		_grandParent = _parent->_parent;
	// 		if (_grandParent == __x)
	// 		{
	// 			__x->_color = BLACK;
	// 			return ;
	// 		}
	// 		else if (_grandParent->_parent == _parent)
	// 		{
	// 			return ;
	// 		}
	// 		else if (_grandParent->_rightColor == RED && _grandParent->_leftColor == RED)
	// 		{
	// 			MinusBlack(__x->_parent->_parent);
	// 			__x = _grandParent;
	// 			continue ;
	// 		}
	// 		else if ((_parent->_color == RED \
	// 				&& _grandParent->_left == _parent) \
	// 				&& _grandParent->_rightColor == BLACK)
	// 		{
	// 			if (_parent->_left == __x)
	// 				ClockWiseRotate(__x->_parent);
	// 			else if (_parent->_right == __x)
	// 			{
	// 				CounterClockWiseRotate(__x);
	// 				ClockWiseRotate(__x);
	// 			}
	// 			break ;
	// 		}
	// 		else if ((_parent->_color == RED \
	// 				&& _grandParent->_right == _parent) \
	// 				&& _grandParent->_leftColor == BLACK)
	// 		{
	// 			if (_parent->_right == __x)
	// 				CounterClockWiseRotate(__x->_parent);
	// 			else if (_parent->_left == __x)
	// 			{
	// 				ClockWiseRotate(__x);
	// 				CounterClockWiseRotate(__x);
	// 			}
	// 			break ;
	// 		}
	// 		else
	// 			break ;
	// 	}
	// }
 	// void	erase_rebalance(_Rb_tree_node_base* parent)
	// {
	// 	_Rb_tree_node_base* _leftSon;
	// 	_Rb_tree_node_base* _rightSon;
	// 	_Rb_tree_node_base* _rightGrandSon;
	// 	_Rb_tree_node_base* _leftGrandSon;

	// 	while (1)
	// 	{	
	// 		if (parent->_leftColor != DOUBLE_BLACK && parent->_rightColor != DOUBLE_BLACK)
	// 			return ;
	// 		else
	// 		{
	// 			_leftSon = parent->_left;
	// 			_rightSon = parent->_right;

	// 			if (parent->_leftColor == DOUBLE_BLACK)
	// 			{
	// 				_leftGrandSon = _rightSon->_left;
	// 				if (_rightSon->_color == BLACK)
	// 				{
	// 					if (_rightSon->_rightColor == RED)
	// 					{
	// 						MinusBlack(_rightSon);
	// 						CounterClockWiseRotate(_rightSon);
	// 						PlusBlack(parent);
	// 						return ;
	// 					}
	// 					else if (_rightSon->_leftColor == RED)
	// 					{
	// 						ClockWiseRotate(_leftGrandSon);
	// 						MinusBlack(_leftGrandSon);
	// 						CounterClockWiseRotate(_leftGrandSon);
	// 						PlusBlack(parent);
	// 						return ;
	// 					}
	// 					else
	// 					{
	// 						PlusBlack(parent);
	// 						if (parent->_parent->_parent == parent)
	// 							RootBlack(parent);
	// 						else if (parent->_color == DOUBLE_BLACK)
	// 						{
	// 							parent = parent->_parent;
	// 							continue ;
	// 						}
	// 						return ;
	// 					}
	// 				}
	// 				else if (_rightSon->_color == RED)
	// 				{
	// 					CounterClockWiseRotate(_rightSon);
	// 					if (parent->_right->_leftColor == BLACK && parent->_right->_rightColor == BLACK)
	// 					{
	// 						PlusBlack(parent);
	// 						return ;
	// 					}
	// 					continue ;
	// 				}
	// 				else
	// 				{
	// 					std::cerr << "error" << std::endl;
	// 					return ;
	// 				}
	// 			}
	// 			else if (parent->_rightColor == DOUBLE_BLACK)
	// 			{
	// 				_rightGrandSon = _leftSon->_right;
	// 				if (_leftSon->_color == BLACK)
	// 				{
	// 					if (_leftSon->_leftColor == RED)
	// 					{
	// 						MinusBlack(_leftSon);
	// 						ClockWiseRotate(_leftSon);
	// 						PlusBlack(parent);
	// 						return ;
	// 					}
	// 					else if (_leftSon->_rightColor == RED)
	// 					{
	// 						CounterClockWiseRotate(_rightGrandSon);
	// 						MinusBlack(_rightGrandSon);
	// 						ClockWiseRotate(_rightGrandSon);
	// 						PlusBlack(parent);
	// 						return ;
	// 					}
	// 					else
	// 					{
	// 						PlusBlack(parent);
	// 						if (parent->_parent->_parent == parent)
	// 							RootBlack(parent);
	// 						else if (parent->_color == DOUBLE_BLACK)
	// 						{
	// 							parent = parent->_parent;
	// 							continue ;
	// 						}
	// 						return ;
	// 					}
	// 				}
	// 				else if (_leftSon->_color == RED)
	// 				{
	// 					ClockWiseRotate(_leftSon);
	// 					if (parent->_left->_leftColor == BLACK && parent->_left->_rightColor == BLACK)
	// 					{
	// 						PlusBlack(parent);
	// 						return ;
	// 					}
	// 					continue ;
	// 				}
	// 				else
	// 				{
	// 					std::cerr << "error" << std::endl;
	// 					return ;
	// 				}
	// 			}
	// 			else
	// 				return ;
	// 		}
	// 	}
	// }


	// void	erase_rebalance(_Rb_tree_node_base* parent)
	// {
	// 	_Rb_tree_node_base* _leftSon;
	// 	_Rb_tree_node_base* _rightSon;
	// 	_Rb_tree_node_base* _rightGrandSon;
	// 	_Rb_tree_node_base* _leftGrandSon;
		
	// 	while (1)
	// 	{
	// 		_leftSon = parent->_left;
	// 		_rightSon = parent->_right;
	// 		if (parent->_leftColor == DOUBLE_BLACK)
	// 		{
	// 			_leftGrandSon = _rightSon->_left;
	// 			if (_rightSon->_color == BLACK)
	// 			{
	// 				if (_rightSon->_rightColor == RED)
	// 				{
	// 					MinusBlack(_rightSon);
	// 					CounterClockWiseRotate(_rightSon);
	// 					PlusBlack(parent);
	// 					return ;
	// 				}
	// 				else if (_rightSon->_leftColor == RED && _rightSon->_rightColor == BLACK)
	// 				{
	// 					ClockWiseRotate(_leftGrandSon);
	// 					continue ;
	// 				}
	// 				else if (_rightSon->_leftColor == BLACK && _rightSon->_rightColor == BLACK)
	// 				{
	// 					PlusBlack(parent);
	// 					if (parent->_parent->_parent == parent)
	// 						RootBlack(parent);
	// 					else if (parent->_color == DOUBLE_BLACK)
	// 					{
	// 						parent = parent->_parent;
	// 						continue ;
	// 					}
	// 					return ;
	// 				}
	// 				else
	// 				{
	// 					std::cerr << "error" << std::endl;
	// 					return ;
	// 				}
	// 			}
	// 			else if (parent->_color == BLACK && parent->_rightColor == RED)
	// 			{
	// 				CounterClockWiseRotate(_rightSon);
	// 				continue ;
	// 			}
	// 			else
	// 			{
	// 				std::cerr << "error" << std::endl;
	// 				return ;
	// 			}
	// 		}
	// 		else if (parent->_rightColor == DOUBLE_BLACK)
	// 		{
	// 			_rightGrandSon = _leftSon->_right;
	// 			if (_leftSon->_color == BLACK)
	// 			{
	// 				if (_leftSon->_leftColor == RED)
	// 				{
	// 					MinusBlack(_leftSon);
	// 					ClockWiseRotate(_leftSon);
	// 					PlusBlack(parent);
	// 					return ;
	// 				}
	// 				else if (_leftSon->_rightColor == RED && _leftSon->_leftColor == BLACK)
	// 				{
	// 					CounterClockWiseRotate(_rightGrandSon);
	// 					continue ;
	// 				}
	// 				else if (_leftSon->_rightColor == BLACK && _leftSon->_leftColor == BLACK)
	// 				{
	// 					PlusBlack(parent);
	// 					if (parent->_parent->_parent == parent)
	// 						RootBlack(parent);
	// 					else if (parent->_color == DOUBLE_BLACK)
	// 					{
	// 						parent = parent->_parent;
	// 						continue ;
	// 					}
	// 					return ;
	// 				}
	// 				else
	// 				{
	// 					std::cerr << "error" << std::endl;
	// 					return ;
	// 				}
	// 			}
	// 			else if (parent->_color == BLACK && parent->_leftColor == RED)
	// 			{
	// 				ClockWiseRotate(_leftSon);
	// 				continue ;
	// 			}
	// 			else
	// 			{
	// 				std::cerr << "error" << std::endl;
	// 				return ;
	// 			}
	// 		}
	// 		else
	// 			return ;
	// 	}
	// }

	// int __black_count(_Rb_tree_node_base* __node, _Rb_tree_node_base* __root)
	// {
	// 	if (__node == 0)
	// 		return 0;
	// 	int __sum = 0;
	// 	do {
	// 		if (__node->_color == BLACK) 
	// 			++__sum;
	// 		if (__node == __root) 
	// 			break ;
	// 		__node = __node->_parent;
	// 	} while (1);
	// 	return __sum;
	// }

	template <typename _T1, typename _T2>
	void _Construct(_T1* __p, const _T2& __value) 
	{
		new ((void*) __p) _T1(__value);
	}
	
	template <typename _T1>
	void _Construct(_T1* __p) 
	{
		new ((void*) __p) _T1();
	}

	template <typename _T1, typename _T2>
	void construct(_T1* __p, const _T2& __value) 
	{
		_Construct(__p, __value);
	}

	template <typename _T1>
	void construct(_T1* __p) 
	{
		_Construct(__p);
	}
	template <typename _Tp>
	void _Destroy(_Tp* __pointer)
	{
		__pointer->~_Tp();
	}

	template <typename _Tp>
	void destroy(_Tp* __pointer) 
	{
		_Destroy(__pointer);
	}

	template <typename _Tp, typename _Allocator>
	struct _Alloc_traits
	{
		typedef typename _Allocator::template rebind<_Tp>::other	allocator_type;
	};

	template <typename _Tp, typename _Alloc>
	class _Rb_tree_alloc_base
	{
		public:
			typedef typename _Alloc_traits<_Tp, _Alloc>::allocator_type allocator_type;
			allocator_type get_allocator() const { return _M_node_allocator; }

			_Rb_tree_alloc_base(const allocator_type& __a)
				: _M_node_allocator(__a), _M_header(0) {}

		// protected:
			typename _Alloc_traits<_Rb_tree_node<_Tp>, _Alloc>::allocator_type
					_M_node_allocator;
			_Rb_tree_node<_Tp>* _M_header;

			_Rb_tree_node<_Tp>* _M_get_node() 
				{ return _M_node_allocator.allocate(1); }
			void _M_put_node(_Rb_tree_node<_Tp>* __p) 
				{ _M_node_allocator.deallocate(__p, 1); }
	};

	template <class _Tp, class _Alloc>
	struct _Rb_tree_base : public _Rb_tree_alloc_base<_Tp, _Alloc>
	{
		typedef _Rb_tree_alloc_base<_Tp, _Alloc>	_Base;
		typedef typename _Base::allocator_type		allocator_type;

		_Rb_tree_base(const allocator_type& __a) 
			: _Base(__a) { this->_M_header = this->_M_get_node(); }
		~_Rb_tree_base() { this->_M_put_node(this->_M_header); }

	};

	template <class _Key, class _Value, class _KeyOfValue, class _Compare,
          class _Alloc = std::allocator<_Value> >
	class _Rb_tree : protected _Rb_tree_base<_Value, _Alloc> 
	{
			typedef _Rb_tree_base<_Value, _Alloc>	_Base;
		protected:
			typedef _Rb_tree_node_base*				_Base_ptr;
			typedef _Rb_tree_node<_Value>			_Rb_tree_node;
			typedef unsigned char		 			_Color_type;
		public:
			typedef _Key							key_type;
			typedef _Value							value_type;
			typedef value_type*						pointer;
			typedef const value_type*				const_pointer;
			typedef value_type&						reference;
			typedef const value_type&				const_reference;
			typedef _Rb_tree_node*					_Link_type;
			typedef size_t							size_type;
			typedef ptrdiff_t						difference_type;

			typedef typename _Base::allocator_type allocator_type;
			allocator_type get_allocator() const { return _Base::get_allocator(); }

		protected:

			_Link_type _M_create_node(const value_type& __x)
			{
				_Link_type __tmp = this->_M_get_node();
				try 
				{
					construct(&__tmp->_value, __x);
				}
				catch(...)
				{
					this->_M_put_node(__tmp);
				}
				return __tmp;
			}

			_Link_type _M_clone_node(_Link_type __x)
			{
				_Link_type __tmp = _M_create_node(__x->_value);
				__tmp->_color = __x->_color;
				__tmp->_left = 0;
				__tmp->_right = 0;
				__tmp->_leftColor = __x->_leftColor;
				__tmp->_rightColor = __x->_rightColor;
				return __tmp;
			}

			void destroy_node(_Link_type __p)
			{
				destroy(&__p->_value);
				this->_M_put_node(__p);
			}

		protected:
			size_type	_M_node_count; // keeps track of size of tree
			_Compare	_M_key_compare;

			_Link_type& _M_root() const 
			{ return (_Link_type&) this->_M_header->_parent; }
			_Link_type& _M_leftmost() const 
			{ return (_Link_type&) this->_M_header->_left; }
			_Link_type& _M_rightmost() const 
			{ return (_Link_type&) this->_M_header->_right; }

			static _Link_type& _S_left(_Link_type __x)
			{ return (_Link_type&)(__x->_left); }
			static _Link_type& _S_right(_Link_type __x)
			{ return (_Link_type&)(__x->_right); }
			static _Link_type& _S_parent(_Link_type __x)
			{ return (_Link_type&)(__x->_parent); }
			static reference _S_value(_Link_type __x)
			{ return __x->_value; }
			static const _Key& _S_key(_Link_type __x)
			{ return _KeyOfValue()(_S_value(__x)); }
			static _Color_type& _S_color(_Link_type __x)
			{ return (_Color_type&)(__x->_color); }

			static _Link_type& _S_left(_Base_ptr __x)
			{ return (_Link_type&)(__x->_left); }
			static _Link_type& _S_right(_Base_ptr __x)
			{ return (_Link_type&)(__x->_right); }
			static _Link_type& _S_parent(_Base_ptr __x)
			{ return (_Link_type&)(__x->_parent); }
			static reference _S_value(_Base_ptr __x)
			{ return ((_Link_type)__x)->_value; }
			static const _Key& _S_key(_Base_ptr __x)
			{ return _KeyOfValue()(_S_value(_Link_type(__x)));} 
			static _Color_type& _S_color(_Base_ptr __x)
			{ return (_Color_type&)(_Link_type(__x)->_color); }

			static _Link_type _S_minimum(_Link_type __x) 
			{ return (_Link_type)  _Rb_tree_node_base::_S_minimum(__x); }

			static _Link_type _S_maximum(_Link_type __x)
			{ return (_Link_type) _Rb_tree_node_base::_S_maximum(__x); }
		public:
			typedef _Rb_tree_iterator<value_type, reference, pointer>				iterator;
			typedef _Rb_tree_iterator<value_type, const_reference, const_pointer>	const_iterator;

			typedef reverse_iterator<const_iterator> const_reverse_iterator;
			typedef reverse_iterator<iterator> reverse_iterator;


////////////////////////////////////////////////////////////////////////////////////////////////
			std::string color(iterator _x)
			{
				_Link_type __x = (_Link_type)(_x._M_node);

				if (__x->_color == RED)
					return "\x1b[31m" + std::to_string(__x->_value.first) + "\x1b[0m";
				return "\x1b[37m" + std::to_string(__x->_value.first) + "\x1b[0m";
			}
			std::string LeftChildcolor(iterator _x)
			{
				_Link_type ___x = (_Link_type)(_x._M_node);
				_Link_type __x = (_Link_type)(___x->_left);

				if (__x != 0 && __x->_color == RED)
					return "\x1b[31m" + std::to_string(__x->_value.first) + "\x1b[0m";
				else if (__x != 0 && __x->_color == BLACK)
					return "\x1b[37m" + std::to_string(__x->_value.first) + "\x1b[0m";
				return "";
			}
			std::string RightChildcolor(iterator _x)
			{
				_Link_type ___x = (_Link_type)(_x._M_node);
				_Link_type __x = (_Link_type)(___x->_right);

				if (__x != 0 && __x->_color == RED)
					return "\x1b[31m" + std::to_string(__x->_value.first) + "\x1b[0m";
				else if (__x != 0 && __x->_color == BLACK)
					return "\x1b[37m" + std::to_string(__x->_value.first) + "\x1b[0m";
				return "";
			}

			std::string Leftcolor(iterator _x)
			{
				_Link_type ___x = (_Link_type)(_x._M_node);

				if (___x->_leftColor == RED)
					return "\x1b[31mRED\x1b[0m";
				return "\x1b[37mBLACK\x1b[0m";
			}
			std::string Rightcolor(iterator _x)
			{
				_Link_type ___x = (_Link_type)(_x._M_node);

				if (___x->_rightColor == RED)
					return "\x1b[31mRED\x1b[0m";
				return "\x1b[37mBLACK\x1b[0m";
			}
			std::string	root_value()
			{
				_Link_type __x = (_Link_type)(_M_root());
				if (__x != 0 && __x->_color == RED)
					return "\x1b[31m" + std::to_string(__x->_value.first) + "\x1b[0m";
				else if (__x != 0 && __x->_color == BLACK)
					return "\x1b[37m" + std::to_string(__x->_value.first) + "\x1b[0m";
				return "empty";
			}
////////////////////////////////////////////////////////////////////////////////////////////////

		private:
			iterator insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v)
			{
				_Link_type __x = (_Link_type) __x_;
				_Link_type __y = (_Link_type) __y_;
				_Link_type __z;

				if (__y == this->_M_header || __x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))) 
				{
					__z = _M_create_node(__v);
					__z->_color = RED;
					_S_left(__y) = __z;               // also makes _M_leftmost() = __z
					__y->_leftColor = __z->_color;
													//    when __y == this->_M_header
					if (__y == this->_M_header) 
					{
						_M_root() = __z;
						_M_rightmost() = __z;
						this->_M_header->_rightColor = __z->_color;
					}
					else if (__y == _M_leftmost())
					{
						_M_leftmost() = __z;   // maintain _M_leftmost() pointing to min node
						this->_M_header->_leftColor = __z->_color;
					}
				}
				else 
				{
					__z = _M_create_node(__v);
					__z->_color = RED;
					_S_right(__y) = __z;
					__y->_rightColor = __z->_color;
					if (__y == _M_rightmost())
					{
						_M_rightmost() = __z;  // maintain _M_rightmost() pointing to max node
						this->_M_header->_rightColor = __z->_color;
					}
				}
				_S_parent(__z) = __y;
				_S_left(__z) = 0;
				__z->_leftColor = BLACK;
				_S_right(__z) = 0;
				__z->_rightColor = BLACK;
				insert_rebalance(__z);
				++_M_node_count;
				return iterator(__z);
			}

			_Link_type _M_copy(_Link_type __x, _Link_type __p) // __p는 header자리가 합리적
			{
                        // structural copy.  __x and __p must be non-null.
				_Link_type __top = _M_clone_node(__x);
				__top->_parent = __p;
				try 
				{
					if (__x->_right)
						__top->_right = _M_copy(_S_right(__x), __top);
					__p = __top;
					__x = _S_left(__x);

					while (__x != 0) 
					{
						_Link_type __y = _M_clone_node(__x);
						__p->_left = __y;
						__y->_parent = __p;
						if (__x->_right)
							__y->_right = _M_copy(_S_right(__x), __y);
						__p = __y;
						__x = _S_left(__x);
					}
				}
				catch(...)
				{
					_M_erase(__top);
				};
				return __top;
			}
			
			void _M_erase(_Link_type __x)
			{
											// erase without rebalancing
				while (__x != 0) 
				{
					_M_erase(_S_right(__x));
					_Link_type __y = _S_left(__x);
					destroy_node(__x);
					__x = __y;
				}
			}

			void	PlusBlack(_Rb_tree_node_base* __x)
			{
				__x->_color += BLACK;
				if (__x->_parent->_parent != __x)
				{
					if (__x->_parent->_left == __x)
						__x->_parent->_leftColor += BLACK;
					else
						__x->_parent->_rightColor += BLACK;
				}
				if (__x->_left != 0)
					__x->_left->_color -= BLACK;
				__x->_leftColor -= BLACK;
				if (__x->_right != 0)
					__x->_right->_color -= BLACK;
				__x->_rightColor -= BLACK;
			}

			void	MinusBlack(_Rb_tree_node_base* __x)
			{
				__x->_color -= BLACK;
				if (__x->_parent->_parent != __x)
				{
					if (__x->_parent->_left == __x)
						__x->_parent->_leftColor -= BLACK;
					else
						__x->_parent->_rightColor -= BLACK;
				}
				if (__x->_left != 0)
					__x->_left->_color += BLACK;
				__x->_leftColor += BLACK;
				if (__x->_right != 0)
					__x->_right->_color += BLACK;
				__x->_rightColor += BLACK;
			}

			void	ClockWiseRotate(_Rb_tree_node_base* __x)
			{
				_Rb_tree_node_base*	_parent = __x->_parent;
				_Rb_tree_node_base*	_GrandParent = _parent->_parent;
				_Rb_tree_node_base*	__xRight = __x->_right;
				unsigned char		__xColor = __x->_color;

				__x->_color = _parent->_color;
				_parent->_color = __xColor;

				_parent->_left = __xRight;
				_parent->_leftColor = __x->_rightColor;
				if (__xRight != 0)
					__xRight->_parent = _parent;

				if (_parent->_parent->_parent != _parent)
				{
					if (_GrandParent->_left == _parent)
					{
						_GrandParent->_left = __x;
						_GrandParent->_leftColor = __x->_color;
					}
					else
					{
						_GrandParent->_right = __x;
						_GrandParent->_rightColor = __x->_color;
					}
				}
				else
					_GrandParent->_parent = __x;
				__x->_parent = _GrandParent;

				__x->_right = _parent;
				__x->_rightColor = _parent->_color;
				_parent->_parent = __x;
			}

			void	CounterClockWiseRotate(_Rb_tree_node_base* __x)
			{
				_Rb_tree_node_base*	_parent = __x->_parent;
				_Rb_tree_node_base*	_GrandParent = _parent->_parent;
				_Rb_tree_node_base*	__xLeft = __x->_left;
				unsigned char		__xColor = __x->_color;

				__x->_color = _parent->_color;
				_parent->_color = __xColor;

				_parent->_right = __xLeft;
				_parent->_rightColor = __x->_leftColor;
				if (__xLeft != 0)
					__xLeft->_parent = _parent;

				if (_parent->_parent->_parent != _parent)
				{
					if (_GrandParent->_left == _parent)
					{
						_GrandParent->_left = __x;
						_GrandParent->_leftColor = __x->_color;
					}
					else
					{
						_GrandParent->_right = __x;
						_GrandParent->_rightColor = __x->_color;
					}
				}
				else
					_GrandParent->_parent = __x;
				__x->_parent = _GrandParent;

				__x->_left = _parent;
				__x->_leftColor = _parent->_color;
				_parent->_parent = __x;
			}

			void	RootBlack(_Rb_tree_node_base* __x)
			{
				if (__x->_color == RED || __x->_color == DOUBLE_BLACK)
					__x->_color = BLACK;
			}

			void	insert_rebalance(_Rb_tree_node_base* __x)
			{
				_Rb_tree_node_base* _parent;
				_Rb_tree_node_base* _grandParent;
				while (1)
				{
					_parent = __x->_parent;
					_grandParent = _parent->_parent;
					if (_grandParent == __x)
					{
						__x->_color = BLACK;
						return ;
					}
					else if (_grandParent->_parent == _parent)
					{
						return ;
					}
					else if (_grandParent->_rightColor == RED && _grandParent->_leftColor == RED)
					{
						MinusBlack(__x->_parent->_parent);
						__x = _grandParent;
						continue ;
					}
					else if ((_parent->_color == RED \
							&& _grandParent->_left == _parent) \
							&& _grandParent->_rightColor == BLACK)
					{
						if (_parent->_left == __x)
							ClockWiseRotate(__x->_parent);
						else if (_parent->_right == __x)
						{
							CounterClockWiseRotate(__x);
							ClockWiseRotate(__x);
						}
						break ;
					}
					else if ((_parent->_color == RED \
							&& _grandParent->_right == _parent) \
							&& _grandParent->_leftColor == BLACK)
					{
						if (_parent->_right == __x)
							CounterClockWiseRotate(__x->_parent);
						else if (_parent->_left == __x)
						{
							ClockWiseRotate(__x);
							CounterClockWiseRotate(__x);
						}
						break ;
					}
					else
						break ;
				}
			}
			void	erase_rebalance(_Rb_tree_node_base* parent)
			{
				_Rb_tree_node_base* _leftSon;
				_Rb_tree_node_base* _rightSon;
				_Rb_tree_node_base* _rightGrandSon;
				_Rb_tree_node_base* _leftGrandSon;

				while (1)
				{	
					if (parent->_leftColor != DOUBLE_BLACK && parent->_rightColor != DOUBLE_BLACK)
						return ;
					else
					{
						_leftSon = parent->_left;
						_rightSon = parent->_right;

						if (parent->_leftColor == DOUBLE_BLACK)
						{
							_leftGrandSon = _rightSon->_left;
							if (_rightSon->_color == BLACK)
							{
								if (_rightSon->_rightColor == RED)
								{
									MinusBlack(_rightSon);
									CounterClockWiseRotate(_rightSon);
									PlusBlack(parent);
									return ;
								}
								else if (_rightSon->_leftColor == RED)
								{
									ClockWiseRotate(_leftGrandSon);
									MinusBlack(_leftGrandSon);
									CounterClockWiseRotate(_leftGrandSon);
									PlusBlack(parent);
									return ;
								}
								else
								{
									PlusBlack(parent);
									if (parent->_parent->_parent == parent)
										RootBlack(parent);
									else if (parent->_color == DOUBLE_BLACK)
									{
										parent = parent->_parent;
										continue ;
									}
									return ;
								}
							}
							else if (_rightSon->_color == RED)
							{
								CounterClockWiseRotate(_rightSon);
								if (parent->_right->_leftColor == BLACK && parent->_right->_rightColor == BLACK)
								{
									PlusBlack(parent);
									return ;
								}
								continue ;
							}
							else
							{
								std::cerr << "error" << std::endl;
								return ;
							}
						}
						else if (parent->_rightColor == DOUBLE_BLACK)
						{
							_rightGrandSon = _leftSon->_right;
							if (_leftSon->_color == BLACK)
							{
								if (_leftSon->_leftColor == RED)
								{
									MinusBlack(_leftSon);
									ClockWiseRotate(_leftSon);
									PlusBlack(parent);
									return ;
								}
								else if (_leftSon->_rightColor == RED)
								{
									CounterClockWiseRotate(_rightGrandSon);
									MinusBlack(_rightGrandSon);
									ClockWiseRotate(_rightGrandSon);
									PlusBlack(parent);
									return ;
								}
								else
								{
									PlusBlack(parent);
									if (parent->_parent->_parent == parent)
										RootBlack(parent);
									else if (parent->_color == DOUBLE_BLACK)
									{
										parent = parent->_parent;
										continue ;
									}
									return ;
								}
							}
							else if (_leftSon->_color == RED)
							{
								ClockWiseRotate(_leftSon);
								if (parent->_left->_leftColor == BLACK && parent->_left->_rightColor == BLACK)
								{
									PlusBlack(parent);
									return ;
								}
								continue ;
							}
							else
							{
								std::cerr << "error" << std::endl;
								return ;
							}
						}
						else
							return ;
					}
				}
			}

		public:
			_Rb_tree() : _Base(allocator_type()), _M_node_count(0), _M_key_compare()
				{ _M_empty_initialize(); }

			_Rb_tree(const _Compare& __comp) : _Base(allocator_type()), _M_node_count(0), _M_key_compare(__comp) 
				{ _M_empty_initialize(); }

			_Rb_tree(const _Compare& __comp, const allocator_type& __a) : _Base(__a), _M_node_count(0), _M_key_compare(__comp) 
				{ _M_empty_initialize(); }

			_Rb_tree(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x) : _Base(__x.get_allocator()), _M_node_count(0), _M_key_compare(__x._M_key_compare)
			{ 
				if (__x._M_root() == 0)
					_M_empty_initialize();
				else 
				{
					_S_color(this->_M_header) = RED;
					_M_root() = _M_copy(__x._M_root(), this->_M_header);
					_M_leftmost() = _S_minimum(_M_root());
					this->_M_header->_leftColor = _S_minimum(_M_root())->_color;
					_M_rightmost() = _S_maximum(_M_root());
					this->_M_header->_rightColor = _S_maximum(_M_root())->_color;
				}
				_M_node_count = __x._M_node_count;
			}
			~_Rb_tree() { clear(); }
			_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& 
 			operator=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x)
			{
				if (this != &__x) 
				{
												// Note that _Key may be a constant type.
					clear();
					_M_node_count = 0;
					_M_key_compare = __x._M_key_compare;        
					if (__x._M_root() == 0) 
					{
						_M_root() = 0;
						_M_leftmost() = this->_M_header;
						this->_M_header->_leftColor = RED;
						_M_rightmost() = this->_M_header;
						this->_M_header->_rightColor = RED;
					}
					else 
					{
						_M_root() = _M_copy(__x._M_root(), this->_M_header);
						_M_leftmost() = _S_minimum(_M_root());
						this->_M_header->_leftColor = _S_minimum(_M_root())->_color;
						_M_rightmost() = _S_maximum(_M_root());
						this->_M_header->_rightColor = _S_maximum(_M_root())->_color;
						_M_node_count = __x._M_node_count;
					}
				}
				return *this;
			}
		private:
			void _M_empty_initialize() 
			{
				_S_color(this->_M_header) = RED; // used to distinguish header from 
													// __root, in iterator.operator++
				_M_root() = 0;
				_M_leftmost() = this->_M_header;
				this->_M_header->_leftColor = RED;
				_M_rightmost() = this->_M_header;
				this->_M_header->_rightColor = RED;
			}

		public:    
			_Compare key_comp() const { return _M_key_compare; }
			iterator begin() { return _M_leftmost(); }
			const_iterator begin() const { return _M_leftmost(); }
			iterator end() { return this->_M_header; }
			const_iterator end() const { return this->_M_header; }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator rbegin() const { 
				return const_reverse_iterator(end()); 
			}
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator rend() const { 
				return const_reverse_iterator(begin());
			} 
			bool empty() const { return _M_node_count == 0; }
			size_type size() const { return _M_node_count; }
			size_type max_size() const { return size_type(-1); }

			void swap(_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __t) 
			{
				ft::swap(this->_M_header, __t._M_header);
				ft::swap(_M_node_count, __t._M_node_count);
				ft::swap(_M_key_compare, __t._M_key_compare);
			}
		
		public:
			ft::pair<iterator,bool> insert_unique(const value_type& __v)
			{
				_Link_type __y = this->_M_header;
				_Link_type __x = _M_root();
				bool __comp = true;
				while (__x != 0) 
				{
					__y = __x;
					__comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
					__x = __comp ? _S_left(__x) : _S_right(__x);
				}
				iterator __j = iterator(__y);   
				if (__comp)
				{
					if (__j == begin())     
						return ft::pair<iterator,bool>(insert(__x, __y, __v), true);
					else
						--__j;
				}
				if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
					return ft::pair<iterator,bool>(insert(__x, __y, __v), true);
				return ft::pair<iterator,bool>(__j, false);
			}

			iterator insert_unique(iterator __position, const value_type& __v)
			{
				if (__position._M_node == this->_M_header->_left) 
				{ // begin()
					if (size() > 0 && _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
						return insert(__position._M_node, __position._M_node, __v);
					// first argument just needs to be non-null 
					else
						return insert_unique(__v).first;
				} 
				else if (__position._M_node == this->_M_header) 
				{ // end()
					if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
						return insert(0, _M_rightmost(), __v);
					else
						return insert_unique(__v).first;
				}
				else 
				{
					iterator __before = __position;
					--__before;
					if (_M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v)) && _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node))) 
					{
						if (_S_right(__before._M_node) == 0)
							return insert(0, __before._M_node, __v); 
						else
							return insert(__position._M_node, __position._M_node, __v);
						// first argument just needs to be non-null 
					}
					else
						return insert_unique(__v).first;
				}
			}

			template <class _InputIterator>
			void insert_unique(_InputIterator __first, _InputIterator __last)
			{
				for ( ; __first != __last; ++__first)
					insert_unique(*__first);
			}

			void erase(iterator __position)
			{
				_Link_type	_pos = (_Link_type)__position._M_node;
				_Link_type	_parent;

				if (_pos == this->_M_header || _pos == 0)
					return ;
				
				while (1)
				{
					_parent = (_Link_type)_pos->_parent;
					if (_pos->_left == 0 && _pos->_right == 0)
					{
						if (_pos == _M_root())
						{
							_M_empty_initialize();
						}
						else
						{
							if ( _pos == _M_leftmost())
								_M_leftmost() = (_Link_type&)_pos->_parent;
							else if (_pos == _M_rightmost())
								_M_rightmost() = (_Link_type&)_pos->_parent;
							if (_parent->_left == _pos)
							{
								if (_pos->_color == BLACK)
									_parent->_leftColor += BLACK;
								else if (_pos->_color == RED)
									_parent->_leftColor = BLACK;
								_parent->_left = 0;
							}
							else
							{
								if (_pos->_color == BLACK)
									_parent->_rightColor += BLACK;
								else if (_pos->_color == RED)
									_parent->_rightColor = BLACK;
								_parent->_right = 0;
							}
						}
						break ;
					}
					else if(_pos->_left != 0 && _pos->_right != 0)
					{
						_Link_type	_successor = _pos;
						successor((_Base_ptr&)_successor);

						swap_successor(_pos, _successor);
						continue ;
					}
					else if (_pos->_left == 0)
					{
						_Link_type	_rightSon = (_Link_type)_pos->_right;

						_rightSon->_color += _pos->_color;
						if (_pos == _M_root())
						{
							_M_root() = _rightSon;
							_M_leftmost() = _rightSon;
							this->_M_header->_leftColor = _rightSon->_color;
							_M_rightmost() = _rightSon;
							this->_M_header->_rightColor = _rightSon->_color;
							_rightSon->_parent = this->_M_header;
						}
						else
						{
							if (_pos == _M_leftmost())
								_M_leftmost() = (_Link_type&)_pos->_right;
							if (_parent->_left == _pos)
							{
								_parent->_leftColor = _rightSon->_color;
								_parent->_left = _rightSon;
							}
							else
							{
								_parent->_rightColor = _rightSon->_color;
								_parent->_right = _rightSon;
							}
						}
						_rightSon->_parent = _parent;
						break ;
					}
					else if (_pos->_right == 0)
					{
						_Link_type	_leftSon = (_Link_type)_pos->_left;

						_leftSon->_color += _pos->_color;
						if (_pos == _M_root())
						{
							_M_root() = _leftSon;
							_M_leftmost() = _leftSon;
							this->_M_header->_leftColor = _leftSon->_color;
							_M_rightmost() = _leftSon;
							this->_M_header->_rightColor = _leftSon->_color;
							_leftSon->_parent = this->_M_header;
						}
						else
						{
							if (_pos == _M_rightmost())
								_M_rightmost() = (_Link_type&)_pos->_left;
							if (_parent->_left == _pos)
							{
								_parent->_leftColor = _leftSon->_color;
								_parent->_left = _leftSon;
							}
							else
							{
								_parent->_rightColor = _leftSon->_color;
								_parent->_right = _leftSon;
							}
						}
						_leftSon->_parent = _parent;
						break ;
					}
				}
				erase_rebalance(_pos->_parent);
				destroy_node(_pos);
				--_M_node_count;
			}

			void	print_pos_succ(_Link_type _pos, _Link_type _successor)
			{
				_Link_type _pos_parent = (_Link_type)_pos->_parent;
				_Link_type _successor_parent = (_Link_type)_successor->_parent;
				std::cout << "pos : " << color((iterator)_pos) << std::endl;
				std::cout << "pos_parent : " << color((iterator)(_pos_parent)) << std::endl;
				std::cout << "pos_right : " << Rightcolor((iterator)_pos) << " : " << RightChildcolor((iterator)_pos) << std::endl;
				std::cout << "pos_left : " << Leftcolor((iterator)_pos) << " : " << LeftChildcolor((iterator)_pos) << std::endl;
				std::cout << "successor : " << color((iterator)_successor) << std::endl;
				std::cout << "successor_parent : " << color((iterator)(_successor_parent)) << std::endl;
				std::cout << "successor_right : " << Rightcolor((iterator)_successor) << " : " << RightChildcolor((iterator)_successor) << std::endl;
				std::cout << "successor_left : " << Leftcolor((iterator)_successor) << " : " << LeftChildcolor((iterator)_successor) << std::endl;
			}

			void	swap_successor(_Link_type _pos, _Link_type _successor)
			{
				_Link_type	pos_parent = (_Link_type)_pos->_parent;
				_Link_type	succ_parent = (_Link_type)_successor->_parent;
				_Link_type	pos_leftson = (_Link_type)_pos->_left;
				_Link_type	pos_rightson = (_Link_type)_pos->_right;
				_Link_type	succ_rightson = (_Link_type)_successor->_right;
				_Color_type	succ_rightC = _successor->_rightColor;
				_Color_type	succ_leftC = _successor->_leftColor;
				
				ft::swap(_pos->_color, _successor->_color);
				if (pos_parent->_parent == _pos)
				{
					pos_parent->_parent = _successor;
					_successor->_parent = pos_parent;
				}
				else if (pos_parent->_left == _pos)
				{
					pos_parent->_left = _successor;
					pos_parent->_leftColor = _successor->_color;
					_successor->_parent = pos_parent;
				}
				else if (pos_parent->_right == _pos)
				{
					pos_parent->_right = _successor;
					pos_parent->_rightColor = _successor->_color;
					_successor->_parent = pos_parent;
				}

				if (succ_parent->_left == _successor)
				{
					succ_parent->_left = _pos;
					succ_parent->_leftColor = _pos->_color;
					_pos->_parent = succ_parent;

					_successor->_left = pos_leftson;
					_successor->_leftColor = pos_leftson->_color;
					pos_leftson->_parent = _successor;

					_successor->_right = pos_rightson;
					_successor->_rightColor = pos_rightson->_color;
					pos_rightson->_parent = _successor;
				}
				else if (succ_parent->_right == _successor)
				{
					_successor->_left = pos_leftson;
					_successor->_leftColor = pos_leftson->_color;
					pos_leftson->_parent = _successor;

					_successor->_right = _pos;
					_successor->_rightColor = _pos->_color;
					_pos->_parent = _successor;
				}

				if (succ_rightson != 0)
				{
					succ_rightson->_parent = _pos;
					_pos->_right = succ_rightson;
				}
				else
					_pos->_right = 0;
				_pos->_rightColor = succ_rightC;
				_pos->_left = 0;
				_pos->_leftColor = succ_leftC;
			}

			void successor(_Base_ptr& __X)
			{
				if (__X->_right != 0)
				{
					__X = __X->_right;
					while (__X->_left != 0)
						__X = __X->_left;
				}
				else
				{
					_Base_ptr __y = __X->_parent;
					while (__X == __y->_right)
					{
						__X = __y;
						__y = __y->_parent;
					}
					if (__X->_right != __y)
						__X = __y;
				}
			}

			size_type erase(const key_type& __x)
			{
				pair<iterator,iterator> __p = equal_range(__x);
				size_type __n = ft::distance(__p.first, __p.second);
				erase(__p.first, __p.second);
				return __n;
			}

			void erase(iterator __first, iterator __last)
			{
				if (__first == begin() && __last == end())
					clear();
				else
					while (__first != __last)
						erase(__first++);
			}

			void erase(const key_type* __first, const key_type* __last)
			{
				while (__first != __last)
					erase(*__first++);
			}

			void clear() 
			{
				if (_M_node_count != 0) 
				{
					_M_erase(_M_root());
					_M_leftmost() = this->_M_header;
					this->_M_header->_leftColor = BLACK;
					_M_root() = 0;
					_M_rightmost() = this->_M_header;
					this->_M_header->_rightColor = BLACK;
					_M_node_count = 0;
				}
			}
		public:
			iterator find(const key_type& __k)
			{
				_Link_type __y = this->_M_header;      // Last node which is not less than __k. 
				_Link_type __x = _M_root();      // Current node. 

				while (__x != 0)
				{
					if (!_M_key_compare(_S_key(__x), __k))
					{
						__y = __x;
						__x = _S_left(__x);
					}
					else
						__x = _S_right(__x);
				}

				iterator __j = iterator(__y);   
				return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end() : __j;
			}

			const_iterator find(const key_type& __k) const
			{
				_Link_type __y = this->_M_header;      // Last node which is not less than __k. 
				_Link_type __x = _M_root();      // Current node. 

				while (__x != 0)
				{
					if (!_M_key_compare(_S_key(__x), __k))
					{
						__y = __x;
						__x = _S_left(__x);
					}
					else
						__x = _S_right(__x);
				}

				const_iterator __j = const_iterator(__y);   
				return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end() : __j;
			}

			size_type count(const key_type& __k) const
			{
				pair<const_iterator, const_iterator> __p = equal_range(__k);
				size_type __n = ft::distance(__p.first, __p.second);
				return __n;
			}

			iterator lower_bound(const key_type& __k)
			{
				_Link_type __y = this->_M_header; /* Last node which is not less than __k. */
				_Link_type __x = _M_root(); /* Current node. */

				while (__x != 0)
				{
					if (!_M_key_compare(_S_key(__x), __k))
					{
						__y = __x;
						__x = _S_left(__x);
					}
					else
						__x = _S_right(__x);
				}

				return iterator(__y);
			}
			const_iterator lower_bound(const key_type& __k) const
			{
				_Link_type __y = this->_M_header; /* Last node which is not less than __k. */
				_Link_type __x = _M_root(); /* Current node. */

				while (__x != 0)
				{
					if (!_M_key_compare(_S_key(__x), __k))
					{
						__y = __x;
						__x = _S_left(__x);
					}
					else
						__x = _S_right(__x);
				}

				return const_iterator(__y);
			}
			iterator upper_bound(const key_type& __k)
			{
				_Link_type __y = this->_M_header; /* Last node which is greater than __k. */
				_Link_type __x = _M_root(); /* Current node. */

				while (__x != 0)
				{
					if (_M_key_compare(__k, _S_key(__x)))
					{
						__y = __x;
						__x = _S_left(__x);
					}
					else
						__x = _S_right(__x);
				}

				return iterator(__y);
			}
			const_iterator upper_bound(const key_type& __k) const
			{
				_Link_type __y = this->_M_header; /* Last node which is greater than __k. */
				_Link_type __x = _M_root(); /* Current node. */

				while (__x != 0)
				{
					if (_M_key_compare(__k, _S_key(__x)))
					{
						__y = __x;
						__x = _S_left(__x);
					}
					else
						__x = _S_right(__x);
				}

				return const_iterator(__y);
			}
			pair<iterator,iterator> equal_range(const key_type& __k)
			{
				return pair<iterator, iterator>(lower_bound(__k), upper_bound(__k));
			}
			pair<const_iterator, const_iterator> equal_range(const key_type& __k) const
			{
				return pair<const_iterator,const_iterator>(lower_bound(__k), upper_bound(__k));
			}
		private:
			int __black_count(_Rb_tree_node_base* __node, _Rb_tree_node_base* __root)
			{
				if (__node == 0)
					return 0;
				int __sum = 0;
				do {
					if (__node->_color == BLACK) 
						++__sum;
					if (__node == __root) 
						break ;
					__node = __node->_parent;
				} while (1);
				return __sum;
			}

		public:
			bool __rb_verify() const
			{
				if (_M_node_count == 0 || begin() == end())
					return _M_node_count == 0 && begin() == end() && this->_M_header->_left == this->_M_header && this->_M_header->_right == this->_M_header;
				
				int __len = __black_count(_M_leftmost(), _M_root());
				for (const_iterator __it = begin(); __it != end(); ++__it) {
					_Link_type __x = (_Link_type) __it._M_node;
					_Link_type __L = _S_left(__x);
					_Link_type __R = _S_right(__x);

					if (__x->_color == RED)
					if ((__L && __L->_color == RED) ||
						(__R && __R->_color == RED))
						return false;

					if (__L && _M_key_compare(_S_key(__x), _S_key(__L)))
						return false;
					if (__R && _M_key_compare(_S_key(__R), _S_key(__x)))
						return false;

					if (!__L && !__R && __black_count(__x, _M_root()) != __len)
						return false;
				}

				if (_M_leftmost() != _Rb_tree_node_base::_S_minimum(_M_root()))
					return false;
				if (_M_rightmost() != _Rb_tree_node_base::_S_maximum(_M_root()))
					return false;

				return true;
			}
	};

	// template <typename _InputIter1, typename _InputIter2>
	// bool equal(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2)
	// {
	// 	for ( ; __first1 != __last1; ++__first1, ++__first2)
	// 		if (!(*__first1 == *__first2))
	// 		return false;
	// 	return true;
	// }

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	bool operator==(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
			const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y)
	{
		return (__x.size() == __y.size() && ft::equal(__x.begin(), __x.end(), __y.begin())) ;
	}

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	bool operator<(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
			const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y)
	{
		return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
	}

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	bool operator!=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
			const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) 
	{
		return !(__x == __y);
	}

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	bool operator>(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
			const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) 
	{
		return __y < __x;
	}

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	bool operator<=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
			const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) 
	{
		return !(__y < __x);
	}

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	bool operator>=(const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
			const _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y) 
	{
		return !(__x < __y);
	}


	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
	void swap(_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __x, 
		_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>& __y)
	{
		__x.swap(__y);
	}
	
	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc = std::allocator<_Value> >
	struct rb_tree : public _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>
	{
		typedef _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> _Base;
		typedef typename _Base::allocator_type allocator_type;

		rb_tree(const _Compare& __comp = _Compare(),
				const allocator_type& __a = allocator_type())
			: _Base(__comp, __a) {}
		
		~rb_tree() {}
	};
};


#endif