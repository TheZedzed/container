#pragma once
# ifndef MAP_HPP
# define MAP_HPP

# include "tree.hpp"
# include "functional.hpp"

namespace ft {

# define _HEADER_HELP_DFT \
		template <typename Key, typename T, typename Compare, typename Alloc>

	template <class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<pair<const Key, T> > >
	class	map {

		public:
			typedef Key					key_type;
			typedef T					mapped_type;
			typedef pair<const Key, T>	value_type;
			typedef Compare				key_compare;
			typedef Alloc				allocator_type;

			class	value_compare : public binary_function<value_type, value_type, bool> {
				friend class map;
				protected:
					Compare	comp;
					value_compare(Compare c) : comp(c) {}
				public:
					bool	operator()(const value_type& x, const value_type& y) const {
						return comp(x.first, y.first);
					}
			};

		private:
			typedef Rb_tree<key_type, value_type, select1st<value_type>, key_compare, allocator_type>	Tree;

			Tree	_t;

		public:
			typedef typename Alloc::reference				reference;
			typedef typename Alloc::const_reference			const_reference;
			typedef typename Alloc::pointer					pointer;
			typedef typename Alloc::const_pointer			const_pointer;
			typedef typename Tree::iterator					iterator;
			typedef typename Tree::const_iterator			const_iterator;
			typedef typename Tree::reverse_iterator			reverse_iterator;
			typedef typename Tree::const_reverse_iterator	const_reverse_iterator;
			typedef typename Tree::difference_type			difference_type;
			typedef typename Tree::size_type				size_type;

			explicit map(const Compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _t(comp, alloc) {}
			template <class InputIterator>
			map(InputIterator first, InputIterator last, const Compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _t(comp, alloc) { _t.insert(first, last); }
			map(const map& x) : _t(x._t) {}
			~map() {}

			map&	operator=(const map& x) {
				_t = x._t;
				return *this;
			}

			allocator_type	get_allocator() const { return _t.get_allocator(); }

			//iterators
			iterator				begin() { return _t.begin(); }
			const_iterator			begin() const { return _t.begin(); }
			iterator				end() { return _t.end(); }
			const_iterator			end() const { return _t.end(); }
			reverse_iterator		rbegin() { return _t.rbegin(); }
			const_reverse_iterator	rbegin() const { return _t.rbegin(); }
			reverse_iterator		rend() { return _t.rend(); }
			const_reverse_iterator	rend() const { return _t.rend(); }

			//capacity
			bool		empty() const { return _t.empty(); }
			size_type	size() const { return _t.size(); }
			size_type	max_size() const { return _t.max_size(); }

			//element access
			mapped_type&	operator[](const key_type k) {
				iterator	i = lower_bound(k);

				if (i == end() || key_comp()(k, (*i).first))
					i = insert(i, value_type(k, mapped_type()));
				return (*i).second;
			}

			//modifiers
			pair<iterator, bool>	insert(const value_type& val) { return _t.insert(val); }
			iterator				insert(iterator position, const value_type& val) { return _t.insert(position, val); }
			template <class InputIterator>
			void					insert(InputIterator first, InputIterator last) { _t.insert(first, last); }

			void		erase(iterator position) { _t.erase(position); }
			size_type	erase(const key_type& k) { return _t.erase(k); }
			void		erase(iterator first, iterator last) { _t.erase(first, last); }
			void		swap(map& x) { _t.swap(x._t); }
			void		clear() { _t.clear(); }

			//observers
			key_compare		key_comp() const { return _t.key_comp(); }
			value_compare	value_comp() const { return value_compare(_t.key_comp()); }

			//operations
			iterator		find(const key_type& k) { return _t.find(k); }
			const_iterator	find(const key_type& k) const { return _t.find(k); }
			size_type		count(const key_type& k) const { return _t.find(k) == _t.end() ? 0 : 1; }
			iterator		lower_bound(const key_type& k) { return _t.lower_bound(k); }
			const_iterator	lower_bound(const key_type& k) const { return _t.lower_bound(k); }
			iterator		upper_bound(const key_type& k) { return _t.upper_bound(k); }
			const_iterator	upper_bound(const key_type& k) const { return _t.upper_bound(k); }

			pair<iterator, iterator>				equal_range(const key_type& k) { return _t.equal_range(k); }
			pair<const_iterator, const_iterator>	equal_range(const key_type& k) const { return _t.equal_range(k); }

			template <typename K1, typename T1, typename C1, typename A1>
			friend bool	operator==(const map<K1, T1, C1, A1>&, const map<K1, T1, C1, A1>&);

			template <typename K1, typename T1, typename C1, typename A1>
			friend bool	operator<(const map<K1, T1, C1, A1>&, const map<K1, T1, C1, A1>&);
	};

	_HEADER_HELP_DFT 
	inline bool	operator==(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return x._t == y._t; }

	_HEADER_HELP_DFT
	inline bool	operator<(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return x._t < y._t; }

	_HEADER_HELP_DFT
	inline bool	operator!=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return !(x == y); }

	_HEADER_HELP_DFT
	inline bool	operator>(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return y < x; }

	_HEADER_HELP_DFT
	inline bool	operator>=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return !(x < y); }

	_HEADER_HELP_DFT
	inline bool	operator<=(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y)
	{ return !(y < x); }

	_HEADER_HELP_DFT
	void	swap(map<Key, T, Compare, Alloc>& x, map<Key, T, Compare, Alloc>& y)
	{ x.swap(y); }

}//namespace

#endif