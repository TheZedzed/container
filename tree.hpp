#pragma once
# ifndef TREE_HPP
# define TREE_HPP

# include "utility.hpp"
# include "algorithm.hpp"
# include "iterator.hpp"
# include "Rb_iterator.hpp"

namespace ft {

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc = std::allocator<Val> >
	class	Rb_tree {

		public:
			//rebind allocator for alloc nodes
			typedef typename Alloc::template rebind<Rb_Node<Val> >::other	Node_allocator;

			typedef Key										key_type;
			typedef Val										value_type;
			typedef value_type*								pointer;
			typedef const value_type*						const_pointer;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
			typedef Rb_Node<Val>*							Node_ptr;
			typedef const Rb_Node<Val>*						Const_Node_ptr;
			typedef size_t									size_type;
			typedef ptrdiff_t								difference_type;
			typedef Alloc									allocator_type;
			typedef Rb_iterator<value_type>					iterator;
			typedef Rb_Const_iterator<value_type>			const_iterator;
			typedef ft::reverse_iterator<iterator>			reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

			allocator_type	get_allocator() const { return this->_alloc; }

			Rb_tree(const Compare& comp, const allocator_type& a) : _alloc(a), _node_count(0), _key_compare(comp) {
				_root.color = _red;
				_root.parent = 0;
				_root.left = &this->_root;
				_root.right = &this->_root;
			}

			Rb_tree(const Rb_tree& x) : _alloc(x._alloc), _key_compare(x._key_compare) {
				if (x._root.parent != 0) {
					_root.parent = _copy(x._root.parent, &this->_root);
					_root.left = Rb_Node<Val>::minimum(_root.parent);
					_root.right = Rb_Node<Val>::maximum(_root.parent);
					_node_count = x._node_count;
				}
			}

			~Rb_tree() { _erase(_root.parent); }

			Rb_tree&	operator=(const Rb_tree& x) {
				if (this != &x) {
					clear();
					_key_compare = x._key_compare;
					if (x._root.parent != 0) {
						_root.parent = _copy(x._root.parent, &_root);
						_root.left = Rb_Node<Val>::minimum(_root.parent);
						_root.right = Rb_Node<Val>::maximum(_root.parent);
						_node_count = x._node_count;
					}
				}
				return *this;
			}

			void	swap(Rb_tree& t) {
				std::swap(_root.parent, t._root.parent);
				std::swap(_root.left, t._root.left);
				std::swap(_root.right, t._root.right);
				_root.parent->parent = &this->_root;
				t._root.parent->parent = &(t._root);
				std::swap(_node_count, t._node_count);
				std::swap(_key_compare, t._key_compare);
			}

			Compare	key_comp() const { return this->_key_compare; }

			iterator				begin() { return iterator(this->_root.left); }
			const_iterator			begin() const { return const_iterator(this->_root.left); }
			iterator				end() { return iterator(&this->_root); }
			const_iterator			end() const { return const_iterator(&this->_root); }
			reverse_iterator		rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
			reverse_iterator		rend() { return reverse_iterator(begin()); }
			const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

			bool		empty() const { return _node_count == 0; }
			size_type	size() const { return _node_count; }
			size_type	max_size() const { return Node_allocator().max_size(); }

			pair<iterator, bool>	insert(const value_type& val) {
				Node_ptr	x = _root.parent;
				Node_ptr	y = &this->_root;
				iterator	j(0);
				bool 		comp = true;

				while (x != 0) {
					y = x;
					comp = _key_compare(KeyOfValue()(val), KeyOfValue()(x->value));
					x = comp ? x->left : x->right;
				}
				j = iterator(y);
				if (comp) {
					if (j == begin())
						return pair<iterator, bool>(_insert(x, y, val), true);
					--j;
				}
				if (_key_compare(KeyOfValue()(j._node->value), KeyOfValue()(val)))
					return pair<iterator, bool>(_insert(x, y, val), true);
				return pair<iterator, bool>(j, false);
			}

			iterator	insert(const_iterator position, const value_type& val) {
				if (position._node == &this->_root) {
					if (size() > 0 && _key_compare(KeyOfValue()(_root.right->value), KeyOfValue()(val)))
						return _insert(0, _root.right, val);
					return insert(val).first;
				}
				else if (_key_compare(KeyOfValue()(val), KeyOfValue()(position._node->value))) {
					const_iterator	before = position;

					if (position._node == _root.left)
						return _insert(_root.left, _root.left, val);
					else if (_key_compare(KeyOfValue()((--before)._node->value), KeyOfValue()(val))) {
						if (before._node->right == 0)
							return _insert(0, before._node, val);
						return _insert(position._node, position._node, val);
					}
					return insert(val).first;
				}
				else if (_key_compare(KeyOfValue()(position._node->value), KeyOfValue()(val))) {
					const_iterator	after = position;

					if (position._node == _root.right)
						return _insert(0, _root.right, val);
					else if (_key_compare(KeyOfValue()(val), KeyOfValue()((++after)._node->value))) {
						if (position._node->right == 0)
							return _insert(0, position._node, val);
						return _insert(after._node, after._node, val);
					}
					return insert(val).first;
				}
				return iterator(const_cast<Node_ptr>(position._node));
			}

			template <typename InpuIterator>
			void	insert(InpuIterator first, InpuIterator last) {
				while (first != last) {
					insert(end(), *first);
					++first;
				}
			}

			void		erase(iterator position) { _erase_aux(position); }
			void		erase(const_iterator position) { _erase_aux(position); }
			void		erase(iterator first, iterator last) {
				while (first != last) {
					_erase_aux(first++);
				}
			}
			void		erase(const_iterator first, const_iterator last) {
				while (first != last) {
					_erase_aux(first++);
				}
			}
			size_type	erase(const key_type& x) {
				pair<iterator, iterator>	p = equal_range(x);
				const size_type				old_size = size();

				_erase_aux(p.first, p.second);
				return old_size - size();
			}

			void	clear() {
				_erase(_root.parent);
				_root.left = &this->_root;
				_root.parent = 0;
				_root.right = &this->_root;
				_node_count = 0;
			}

			iterator	find(const key_type& k) {
				iterator	j = lower_bound(k);
				return (j == end() || _key_compare(k, KeyOfValue()(j._node->value))) ? end() : j;
			}

			const_iterator	find(const key_type& k) const {
				const_iterator	j = lower_bound(k);
				return (j == end() || _key_compare(k, KeyOfValue()(j._node->value))) ? end() : j;
			}

			size_type	count(const key_type& k) const {
				pair<const_iterator, const_iterator>	p = equal_range(k);
				const	size_type	n = std::distance(p.first, p.second);
				return n;
			}

			iterator		lower_bound(const key_type& k) { return _lower_bound(_root.parent, &this->_root, k); }
			const_iterator	lower_bound(const key_type& k) const { return _lower_bound(_root.parent, &this->_root, k); }
			iterator		upper_bound(const key_type& k) { return _upper_bound(_root.parent, &this->_root, k); }
			const_iterator	upper_bound(const key_type& k) const { return _upper_bound(_root.parent, &this->_root, k); }

			pair<iterator, iterator>	equal_range(const key_type& k) {
				Node_ptr	x = _root.parent;
				Node_ptr	y = &this->_root;

				while (x != 0) {
					if (_key_compare(KeyOfValue()(x->value), k))
						x = x->right;
					else if (_key_compare(k, KeyOfValue()(x->value))) {
						y = x;
						x = x->left;
					}
					else {
						Node_ptr	xu(x), yu(y);
						y = x;
						x = x->left;
						xu = xu->right;
						return pair<iterator, iterator>(_lower_bound(x, y, k), _upper_bound(xu, yu, k));
					}
				}
				return pair<iterator, iterator>(iterator(y), iterator(y));
			}

			pair<const_iterator, const_iterator>	equal_range(const key_type& k) const {
				Const_Node_ptr	x = _root.parent;
				Const_Node_ptr	y = &this->_root;

				while (x != 0) {
					if (_key_compare(KeyOfValue()(x->value), k))
						x = x->right;
					else if (_key_compare(k, KeyOfValue()(x->value))) {
						y = x;
						x = x->left;
					}
					else {
						Const_Node_ptr	xu(x), yu(y);
						y = x;
						x = x->left;
						xu = xu->right;
						return pair<const_iterator, const_iterator>(_lower_bound(x, y, k), _upper_bound(xu, yu, k));
					}
				}
				return pair<const_iterator, const_iterator>(const_iterator(y), const_iterator(y));
			}

		private:
			Node_ptr	_create_node(const value_type& val) {
				//use node allocator
				Node_ptr	node = Node_allocator().allocate(1);

				//use pair allocator
				_alloc.construct(&node->value, val);
				return node;
			}

			void		_destroy_node(Node_ptr node) {
				_alloc.destroy(&node->value);
				Node_allocator().deallocate(node, 1);
			}

			Node_ptr	_clone_node(Const_Node_ptr node) {
				Node_ptr	tmp = _create_node(node->value);
				tmp->color = node->color;
				tmp->left = 0;
				tmp->right = 0;
				return tmp;
			}

			Node_ptr	_copy(Const_Node_ptr x, Node_ptr p) {
				Node_ptr	top = _clone_node(x);

				top->parent = p;
				if (x->right)
					top->right = _copy(x->right, top);
				p = top;
				x = x->left;
				while (x != 0) {
					Node_ptr	y = _clone_node(x);
					p->left = y;
					y->parent = p;
					if (x->right)
						y->right = _copy(x->right, y);
					p = y;
					x = x->left;
				}
				return top;
			}

			iterator	_insert(Const_Node_ptr x, Const_Node_ptr y, const value_type& val) {
				bool		insert_left = (x != 0 || y == &this->_root || _key_compare(KeyOfValue()(val), KeyOfValue()(y->value)));
				Node_ptr	z = _create_node(val);

				insert_and_rebalance(insert_left, z, const_cast<Node_ptr>(y), this->_root);
				++_node_count;
				return iterator(z);
			}

			iterator	_lower_bound(Node_ptr x, Node_ptr y, const Key& k) {
				while (x != 0) {
					if (!_key_compare(KeyOfValue()(x->value), k)) {
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return iterator(y);
			}

			const_iterator	_lower_bound(Const_Node_ptr x, Const_Node_ptr y, const Key& k) const {
				while (x != 0) {
					if (!_key_compare(KeyOfValue()(x->value), k)) {
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return const_iterator(y);
			}

			iterator		_upper_bound(Node_ptr x, Node_ptr y, const Key& k) {
				while (x != 0) {
					if (_key_compare(k, KeyOfValue()(x->value))) {
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return iterator(y);
			}

			const_iterator	_upper_bound(Const_Node_ptr x, Const_Node_ptr y, const Key& k) const {
				while (x != 0) {
					if (_key_compare(k, KeyOfValue()(x->value))) {
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return const_iterator(y);
			}

			void	_erase(Node_ptr x) {
				Node_ptr	y(0);

				while (x != 0) {
					_erase(x->right);
					y = x->left;
					_destroy_node(x);
					x = y;
				}
			}

			void	_erase_aux(const_iterator position) {
				Node_ptr	y = rebalance_for_erase(const_cast<Node_ptr>(position._node), this->_root);

				_destroy_node(y);
				--_node_count;
			}

			void	_erase_aux(const_iterator first, const_iterator last) {
				if (first == begin() && last == end())
					clear();
				else {
					while (first != last) {
						_erase_aux(first++);
					}
				}
			}

			allocator_type	_alloc;
			size_type		_node_count;
			Compare			_key_compare;
			Rb_Node<Val>	_root;

	};

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline bool	operator==(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x, const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) { return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()); }

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline bool	operator<(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x, const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) { return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline bool	operator!=(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x, const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) { return !(x == y); }

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline bool	operator>(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x, const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) { return y < x; }

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline bool	operator<=(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x, const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) { return !(y < x); }

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline bool	operator>=(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x, const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) { return !(x < y); }

}//namespace

#endif