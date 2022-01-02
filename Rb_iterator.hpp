#pragma once
# ifndef RB_ITERATOR_HPP
# define RB_ITERATOR_HPP

# include "Rb_algo.hpp"

namespace ft {

	template <typename T>
	struct	Rb_iterator {
		typedef T							value_type;
		typedef T&							reference;
		typedef T*							pointer;
		typedef ptrdiff_t					difference_type;
		typedef Rb_iterator<T>				Self;
		typedef Rb_Node<T>*					Node_ptr;
		typedef bidirectional_iterator_tag	iterator_category;
		typedef size_t						size_type;

		Rb_iterator() : _node() {}
		
		explicit Rb_iterator(Node_ptr x) : _node(x) {}
		
		reference	operator*() const
		{ return _node->value; }

		pointer	operator->() const
		{ return &_node->value; }

		Self&	operator++() {
			Node_ptr	y(0);

			if (_node->right != 0) {
				_node = _node->right;
				while (_node->left != 0)
					_node = _node->left;
			}
			else {
				y = _node->parent;
				while (_node == y->right) {
					_node = y;
					y = y->parent;
				}
				if (_node->right != y)
					_node = y;
			}
			return *this;
		}

		Self operator++(int) {
			Self	tmp = *this;
			this->operator++();
			return tmp;
		}

		Self& operator--() {
			Node_ptr	y(0);

			if (_node->color == _red && _node->parent->parent == _node)
				_node = _node->right;
			else if (_node->left != 0) {
				y = _node->left;
				while (y->right != 0)
					y = y->right;
				_node = y;
			}
			else {
				y = _node->parent;
				while (_node == y->left) {
					_node = y;
					y = y->parent;
				}
				_node = y;
			}
			return *this;
		}

		Self operator--(int) {
			Self	tmp = *this;
			this->operator--();
			return tmp;
		}

		bool	operator==(const Self& x) const
		{ return _node == x._node; }

		bool	operator!=(const Self& x) const
		{ return _node != x._node; }

		Node_ptr	_node;
	};

	template <typename T>
	struct	Rb_Const_iterator {
		typedef T							value_type;
		typedef const T&					reference;
		typedef const T*					pointer;
		typedef Rb_iterator<T>				iterator;
		typedef Rb_Const_iterator<T>		Self;
		typedef const Rb_Node<T>*			Node_ptr;
		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;
		typedef size_t						size_type;

		Rb_Const_iterator() : _node() {}

		explicit Rb_Const_iterator(Node_ptr x) : _node(x) {}

		Rb_Const_iterator(const iterator& it) : _node(it._node) {}

		reference	operator*() const
		{ return _node->value; }

		pointer	operator->() const
		{ return &_node->value; }

		Self&	operator++() {
			Node_ptr	y(0);

			if (_node->right != 0) {
				_node = _node->right;
				while (_node->left != 0)
					_node = _node->left;
			}
			else {
				y = _node->parent;
				while (_node == y->right) {
					_node = y;
					y = y->parent;
				}
				if (_node->right != y)
					_node = y;
			}
			return *this;
		}

		Self operator++(int) {
			Self	tmp = *this;
			this->operator++();
			return tmp;
		}

		Self& operator--() {
			Node_ptr	y(0);

			if (_node->color == _red && _node->parent->parent == _node)
				_node = _node->right;
			else if (_node->left != 0) {
				y = _node->left;
				while (y->right != 0)
					y = y->right;
				_node = y;
			}
			else {
				y = _node->parent;
				while (_node == y->left) {
					_node = y;
					y = y->parent;
				}
				_node = y;
			}
			return *this;
		}

		Self operator--(int) {
			Self	tmp = *this;
			this->operator--();
			return tmp;
		}

		bool	operator==(const Self& x) const
		{ return _node == x._node; }

		bool	operator!=(const Self& x) const
		{ return _node != x._node; }

		Node_ptr	_node;
	};

	template <typename Val>
	inline bool	operator==(const Rb_iterator<Val>& x, const Rb_Const_iterator<Val>& y)
	{ return x._node == y._node; }

	template <typename Val>
	inline bool	operator!=(const Rb_iterator<Val>& x, const Rb_Const_iterator<Val>& y)
	{ return x._node != y._node; }

}//namespace

#endif