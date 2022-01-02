#pragma once
# ifndef SET_HPP
# define SET_HPP

# include "tree.hpp"
# include "functional.hpp"

namespace ft {

# define _HEADER_SET \
	template <typename T, typename Compare, typename Alloc>

	template <typename Key, typename Compare = less<Key>, typename Alloc = std::allocator<Key> >
	class	set {

		public:
			typedef Key								key_type;
			typedef Key								value_type;
			typedef Compare							key_compare;
			typedef Compare							value_compare;
			typedef Alloc							allocator_type;
			typedef typename Alloc::reference		reference;
			typedef typename Alloc::const_reference	const_reference;
			typedef typename Alloc::pointer			pointer;
			typedef typename Alloc::const_pointer	const_pointer;

		private:
			typedef Rb_tree<key_type, value_type, Identity<value_type>, key_compare, Alloc>	Tree;

			Tree	_t;

		public:
			typedef typename Tree::const_iterator			iterator;
			typedef typename Tree::const_iterator			const_iterator;
			typedef typename Tree::const_reverse_iterator	reverse_iterator;
			typedef typename Tree::const_reverse_iterator	const_reverse_iterator;
			typedef size_t									size_type;
			typedef ptrdiff_t								difference_type;

			explicit	set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _t(comp, alloc) {}
			template <typename InputIterator>
			set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(),  const allocator_type& alloc = allocator_type()) : _t(comp, alloc) { _t.insert(first, last); }
			set(const set& x) : _t(x._t) {}
			~set() {}

			set&	operator=(const set& x) {
				_t = x._t;
				return *this;
			}

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

			//modifiers
			pair<iterator,bool>	insert(const value_type& val) {
				pair<typename Tree::iterator, bool>	p = _t.insert(val);
				return pair<iterator, bool>(p.first, p.second);
			}

			iterator	insert(iterator position, const value_type& val) { return _t.insert(position, val); }
			template <typename InputIterator>
			void	insert(InputIterator first, InputIterator last) { _t.insert(first, last); }
			void	erase(iterator position) { _t.erase(position); }
			size_type	erase(const value_type& val) { return _t.erase(val); }
			void	erase(iterator first, iterator last) { _t.erase(first, last); }
			void	swap(set& x) { _t.swap(x._t); }
			void	clear() { _t.clear(); }

			//observers
			key_compare		key_comp() const { return _t.key_comp(); }
			value_compare	value_comp() const { return _t.key_comp(); }

			//operations
			iterator	find(const value_type& val) const { return _t.find(val); }
			size_type	count(const value_type& val) const { return _t.find(val) == _t.end() ? 0 : 1; }
			iterator	lower_bound(const value_type& val) const { return _t.lower_bound(val); }
			iterator	upper_bound(const value_type& val) const { return _t.upper_bound(val); }
			pair<iterator, iterator>
			equal_range(const value_type& val) const { return _t.equal_range(val); }
			allocator_type	get_allocator() const { return _t.get_allocator(); }

		template <typename K1, typename C1, typename A1>
		friend bool	operator==(const set<K1, C1, A1>&, const set<K1, C1, A1>&);

		template <typename K1, typename C1, typename A1>
		friend bool	operator<(const set<K1, C1, A1>&, const set<K1, C1, A1>&);

	};

	_HEADER_SET
	inline bool	operator==(const set<T,Compare,Alloc>& x, const set<T,Compare,Alloc>& y) { return x._t == y._t; }

	_HEADER_SET
	inline bool	operator<(const set<T,Compare,Alloc>& x, const set<T,Compare,Alloc>& y) { return  x._t < y._t; }

	_HEADER_SET
	inline bool	operator!=(const set<T,Compare,Alloc>& x, const set<T,Compare,Alloc>& y) { return !(x == y); }

	_HEADER_SET
	inline bool	operator<=(const set<T,Compare,Alloc>& x, const set<T,Compare,Alloc>& y) { return !(y < x); }

	_HEADER_SET
	inline bool	operator>(const set<T,Compare,Alloc>& x, const set<T,Compare,Alloc>& y) { return y < x; }

	_HEADER_SET
	inline bool	operator>=(const set<T,Compare,Alloc>& x, const set<T,Compare,Alloc>& y) { return !(x < y); }

	_HEADER_SET
	inline void	swap(set<T, Compare, Alloc>& x, set<T, Compare, Alloc>& y) { x.swap(y); }

}//namespace

#endif