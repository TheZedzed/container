#pragma once
# ifndef VECTOR_HPP
# define VECTOR_HPP

# include "iterator.hpp"
# include "algorithm.hpp"

namespace ft {

	template <typename T, typename Alloc = std::allocator<T> >
	class	vector {

		public:

			typedef T											value_type;
			typedef typename Alloc::reference					reference;
			typedef typename Alloc::const_reference				const_reference;
			typedef typename Alloc::pointer						pointer;
			typedef typename Alloc::const_pointer				const_pointer;
			typedef ft::normal_iterator<pointer, vector>		iterator;
			typedef ft::normal_iterator<const_pointer, vector>	const_iterator;
			typedef ptrdiff_t									difference_type;
			typedef size_t										size_type;
			typedef Alloc										allocator_type;
			typedef ft::reverse_iterator<iterator>				reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

		private:

			size_type	_check_len(size_type n, const char* s) const {
				if (max_size() - size() < n)
					throw std::length_error(s);
				const size_type	len = size() + std::max(size(), n);
				return (len < size() || len > max_size() ? max_size() : len);
			}

			void	_check_range(size_type n) const
			{ if (n >= size()) throw std::out_of_range("vector::_range_check"); }

			void	_destroy(pointer first, pointer last) {
				for (; first != last; ++first) 
					this->_impl.destroy(first);
			}

			pointer	_move_backward(pointer first, pointer last, pointer result) {
				while (last != first)
					*(--result) = *(--last);
				return result;
			}

			template <typename InputIterator, typename ForwardIterator>
			ForwardIterator	_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator d_first) {
				while (first != last) {
					this->_impl.construct(d_first, *first);
					++first; ++d_first;
				}
				return d_first;
			}

			template <typename InputIt, typename OutputIt>
			OutputIt	_copy(InputIt first, InputIt last, OutputIt d_first) {
				while (first != last) {
					*d_first++ = *first++;
				}
				return d_first;
			}

			void	_fill(pointer first, pointer last, const T& value) {
				for (; first != last; ++first) {
					*first = value;
				}
			}

			template <typename ForwardIterator, class Size>
			ForwardIterator	_uninitialized_fill(ForwardIterator first, Size count, const value_type& val) {
				while (count > 0) {
					this->_impl.construct(first, val);
					++first;
					--count;
				}
				return first;
			}

			//for ambiguity when 2 same template type, SFINAE-friendly
			template<typename Integer>
			void	_initialize_dispatch(Integer n, Integer val, true_type) {
				this->_start = this->_impl.allocate(static_cast<size_type>(n));
				this->_end_of_storage = this->_start + static_cast<size_type>(n);
				this->_finish = this->_uninitialized_fill(this->_start, static_cast<size_type>(n), static_cast<value_type>(val));
			}

			template <typename InputIterator>
			void	_initialize_dispatch(InputIterator first, InputIterator last, false_type) {
				if (typeid(typename iterator_traits<InputIterator>::iterator_category) == typeid(input_iterator_tag)) {
					for (; first != last; ++first) push_back(*first);
				}
				else {
					size_type	n = ft::distance(first, last);

					this->_start = this->_impl.allocate(n);
					this->_end_of_storage = this->_start + n;
					this->_finish = this->_uninitialized_copy(first, last, this->_start);
				}
			}

			template <typename Integer>
			void	_assign_dispatch(Integer n, Integer val, true_type) {
				const size_type	_n(n);

				if (_n > capacity()) {
					vector	tmp(_n, val);
					tmp.swap(*this);
				}
				else if (_n > size()) {
					this->_fill(this->_start, this->_finish, val);
					const size_type	add = _n - size();
					this->_finish = this->_uninitialized_fill(this->_finish, add, val);
				}
				else {
					this->_fill(this->_start, this->_start + _n, val);
					this->_destroy(this->_start + _n, this->_finish);
					this->_finish = this->_start + _n;
				}
			}

			template <typename InputIterator>
			void	_assign_dispatch(InputIterator first,InputIterator last, false_type) {
				pointer	cur(this->_start);

				for (;first != last && cur != this->_finish; ++cur, ++first)
					*cur = *first;
				if (first == last) {
					if (size_type n = this->_finish - cur) {
						this->_destroy(cur, this->_finish);
						this->_finish = cur;
					}
				}
				else {
					for (; first != last; ++first)
						insert(end(), *first);
				}
			}

			template <typename Integer>
			void	_insert_dispatch(iterator position, Integer n, Integer val, true_type)
			{ this->_fill_insert(position, n, val); }

			void	_fill_insert(iterator position, size_type n, const value_type& val) {
				if (n != 0) {
					const size_type	_n(n);

					if (size_type(this->_end_of_storage - this->_finish) >= _n) {
						const size_type	elems_after = end() - position;
						pointer			old_finish(this->_finish);

						if (elems_after > _n) {
							this->_uninitialized_copy(this->_finish - _n, this->_finish, this->_finish);
							this->_finish += _n;
							this->_move_backward(position.base(), old_finish - _n, old_finish);
							this->_fill(position.base(), position.base() + _n, val);
						}
						else {
							this->_finish = this->_uninitialized_fill(this->_finish, _n - elems_after, val);
							this->_uninitialized_copy(position.base(), old_finish, this->_finish);
							this->_finish += elems_after;
							this->_fill(position.base(), old_finish, val);
						}
					}
					else {
						const size_type	len = this->_check_len(n, "vector::insert_fill");
						const size_type	elems_before = position - begin();
						pointer			new_start(this->_impl.allocate(len));
						pointer			new_finish(new_start);

						this->_uninitialized_fill(new_start + elems_before, _n, val);
						new_finish = this->_uninitialized_copy(this->_start, position.base(), new_start);
						new_finish += _n;
						new_finish = this->_uninitialized_copy(position.base(), this->_finish, new_finish);
						this->_destroy(this->_start, this->_finish);
						this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
						this->_start = new_start;
						this->_finish = new_finish;
						this->_end_of_storage = new_start + len;
					}
				}
			}

			template <typename ForwardIterator>
			void	_insert_dispatch(iterator position, ForwardIterator first, ForwardIterator last, false_type)
			{ this->_range_insert(position, first, last); }

			template <typename ForwardIterator>
			void	_range_insert(iterator position, ForwardIterator first, ForwardIterator last) {
				if (first != last) {
					const size_type	n = ft::distance(first, last);

					if (size_type(this->_end_of_storage - this->_finish) >= n) {
						const size_type	elems_after = end() - position;
						pointer			old_finish = this->_finish;

						if (elems_after > n) {
							this->_uninitialized_copy(this->_finish - n, this->_finish, this->_finish);
							this->_finish += n;
							this->_move_backward(position.base(), old_finish - n, old_finish);
							this->_copy(first, last, position);
						}
						else {
							ForwardIterator	mid = first;

							ft::advance(mid, elems_after);
							this->_uninitialized_copy(mid, last, this->_finish);
							this->_finish += n - elems_after;
							this->_uninitialized_copy(position.base(), old_finish, this->_finish);
							this->_finish += elems_after;
							this->_copy(first, mid, position);
						}
					}
					else {
						const size_type	len = this->_check_len(n, "vector::insert_range");
						pointer			new_start(this->_impl.allocate(len));
						pointer			new_finish = 0;

						new_finish = this->_uninitialized_copy(this->_start, position.base(), new_start);
						new_finish = this->_uninitialized_copy(first, last, new_finish);
						new_finish = this->_uninitialized_copy(position.base(), this->_finish, new_finish);
						this->_destroy(this->_start, this->_finish);
						this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
						this->_start = new_start;
						this->_finish = new_finish;
						this->_end_of_storage = new_start + len;
					}
				}
			}

			void	_realloc_insert(iterator position, const value_type& val) {
				const size_type	len = this->_check_len(size_type(1), "vector::insert");
				const size_type	elems_before = position - begin();
				pointer			new_start(this->_impl.allocate(len));
				pointer			new_finish(new_start);

				this->_impl.construct(new_start + elems_before, val);
				new_finish = this->_uninitialized_copy(this->_start, position.base(), new_start);
				++new_finish;
				new_finish = this->_uninitialized_copy(position.base(), this->_finish, new_finish);
				this->_destroy(this->_start, this->_finish);
				this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
				this->_start = new_start;
				this->_finish = new_finish;
				this->_end_of_storage = this->_start + len;
			}

		protected:
			allocator_type	_impl;
			pointer			_start;
			pointer			_finish;
			pointer			_end_of_storage;

		public:
			//Constructors
			vector(const Alloc& alloc = allocator_type()) : _impl(alloc), _start(0), _finish(0), _end_of_storage(0) {}

			vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : _impl(alloc) {
				this->_start = this->_impl.allocate(n);
				this->_end_of_storage = this->_start + n;
				this->_finish = this->_uninitialized_fill(this->_start, n, val);
			}

			vector(const vector& x) : _impl(x._impl) {
				size_type	n = x.size();

				this->_start = this->_impl.allocate(n);
				this->_end_of_storage = this->_start + n;
				this->_finish = this->_uninitialized_copy(x._start, x._finish, this->_start);
			}

			template <typename InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : _impl(alloc) {
				typedef typename	ft::is_integral<InputIterator>::type Integral;

				this->_initialize_dispatch(first, last, Integral());
			}

			//Destructor
			~vector() {
				clear();
				this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
			}

			//Assign operator
			vector&	operator=(const vector& x) {
				if (this != &x) {
					clear();
					insert(end(), x.begin(), x.end());
				}
				return *this;
			}

			//Iterators
			iterator	begin()
			{ return iterator(this->_start); }

			const_iterator	begin() const
			{ return const_iterator(this->_start); }

			iterator	end()
			{ return iterator(this->_finish); }

			const_iterator	end() const
			{ return const_iterator(this->_finish); }

			reverse_iterator	rbegin()
			{ return reverse_iterator(end()); }

			const_reverse_iterator	rbegin() const
			{ return const_reverse_iterator(end()); }

			reverse_iterator	rend()
			{ return reverse_iterator(begin()); }

			const_reverse_iterator	rend() const
			{ return const_reverse_iterator(begin()); }

			//Capacity
			size_type	size() const
			{ return size_type(this->_finish - this->_start); }

			size_type	max_size() const
			{ return this->_impl.max_size(); }

			bool	empty() const
			{ return this->_start == this->_finish; }

			void	resize(size_type n, value_type val = value_type()) {
				if (n > size())
					insert(end(), n - size(), val);
				else if (n < size()) {
					this->_destroy(this->_start + n, this->_finish);
					this->_finish = this->_start + n;
				}
			}

			size_type	capacity() const
			{ return size_type(this->_end_of_storage - this->_start); }

			void	reserve(size_type n) {
				if (n >= max_size())
					throw std::length_error("vector::reserve");
				if (capacity() < n) {
					size_type	old = size_type(this->_finish - this->_start);
					pointer		tmp = this->_impl.allocate(n);
					pointer		ptr = this->_start;
					pointer		cpy = tmp;

					while (ptr != this->_finish) {
						this->_impl.construct(cpy, *ptr);
						this->_impl.destroy(ptr);
						++cpy; ++ptr;
					}
					this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
					this->_start = tmp;
					this->_finish = this->_start + old;
					this->_end_of_storage = this->_start + n;
				}
			}

			//Element access
			reference	at(size_type n) {
				this->_check_range(n);
				return (*this)[n];
			}

			const_reference	at(size_type n) const {
				this->_check_range(n);
				return (*this)[n];
			}

			reference	operator[] (size_type n)
			{ return *(begin() + n); }

			const_reference	operator[] (size_type n) const
			{ return *(begin() + n); }

			reference	front()
			{ return *(begin()); }

			const_reference	front() const
			{ return *(begin()); }

			reference	back()
			{ return *(end() - 1); }

			const_reference	back() const
			{ return *(end() - 1); }

			//Modifiers
			iterator	insert(iterator position, const value_type& val) {
				const size_type	n = position - begin();

				if (this->_finish != this->_end_of_storage && position == end()) {
					this->_impl.construct(this->_finish, val);
					++this->_finish;
				}
				else {
					if (this->_finish != this->_end_of_storage) {
						this->_impl.construct(this->_finish, *(this->_finish - 1));
						++this->_finish;
						this->_move_backward(position.base(), this->_finish - 2, this->_finish - 1);
						*position = val;
					}
					else
						this->_realloc_insert(position, val);
				}
				return iterator(this->_start + n);
			}

			void	insert(iterator position, size_type n, const value_type& val)
			{ this->_fill_insert(position, n, val); }

			template <typename InputIterator>
			void	insert(iterator position, InputIterator first, InputIterator last) {
				typedef typename	ft::is_integral<InputIterator>::type	Integral;

				this->_insert_dispatch(position, first, last, Integral());
			}

			template <typename InputIterator>
			void	assign(InputIterator first, InputIterator last) {
				typedef typename	ft::is_integral<InputIterator>::type	Integral;

				this->_assign_dispatch(first, last, Integral());
			}

			void	assign(size_type n, const value_type& val) {
				erase(begin(), end());
				insert(begin(), n, val);
			}

			void	push_back(const value_type& val) {
				if (this->_finish != this->_end_of_storage) {
					this->_impl.construct(this->_finish, val);
					++this->_finish;
				}
				else
					this->_realloc_insert(this->end(), val);
			}

			void	pop_back() {
				--this->_finish;
				this->_impl.destroy(this->_finish);
			}

			iterator	erase(iterator position) {
				iterator	cpy(position);

				if (position + 1 != end()) {
					while ((position + 1) != end()) {
						*position++ = *(position + 1);
					}
				}
				--this->_finish;
				this->_impl.destroy(this->_finish);
				return cpy;
			}

			iterator	erase(iterator first, iterator last) {
				iterator	cpy(first);

				if (first != last) {
					if (last != end()) {
						while (last != end())
							*first++ = *last++;
					}
					this->_destroy(first.base() + (end() - last), this->_finish);
					this->_finish = first.base() + (end() - last);
				}
				return cpy;
			}

			void	swap(vector& x) {
				pointer			tmp(0);
				allocator_type	tp;

				tmp = this->_start;
				this->_start = x._start;
				x._start = tmp;
				tmp = this->_finish;
				this->_finish = x._finish;
				x._finish = tmp;
				tmp = this->_end_of_storage;
				this->_end_of_storage = x._end_of_storage;
				x._end_of_storage = tmp;
				tp = this->_impl;
				this->_impl = x._impl;
				x._impl = tp;
			}

			void	clear()
			{ erase(begin(), end()); }

			//Allocator
			allocator_type	get_allocator() const
			{ return this->_impl; }
	};

	template <typename T, typename Alloc>
	bool	operator==(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
	{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); }

	template <typename T, typename Alloc>
	bool	operator<(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
	{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

	template <typename T, typename Alloc>
	bool	operator!=(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
	{ return !(x == y); }

	template <typename T, typename Alloc>
	bool	operator<=(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
	{ return !(y < x); }

	template <typename T, typename Alloc>
	bool	operator>(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
	{ return (y < x); }

	template <typename T, typename Alloc>
	bool	operator>=(const vector<T,Alloc>& x, const vector<T,Alloc>& y)
	{ return !(x < y); }

	template <typename T, typename Alloc>
	void	swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
	{ x.swap(y); }

}

#endif