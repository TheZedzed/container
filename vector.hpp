#pragma once
# ifndef VECTOR_HPP
# define VECTOR_HPP

# include "iterator.hpp"
# include "algorithm.hpp"

# define _HEADER_VECT \
	template <typename T, typename Alloc>

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
			//utils
			void				_check_range(size_type n) const;
			size_type			_check_len(size_type n, const char* s) const;
			void				_destroy(pointer first, pointer last);
			pointer				_move_backward(pointer first, pointer last, pointer result);
			template <typename InputIt, typename OutputIt>
			OutputIt			_copy(InputIt first, InputIt last, OutputIt d_first);
			template <typename InputIterator, typename ForwardIterator>
			ForwardIterator		_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator d_first);
			void				_fill(pointer first, pointer last, const T& value);
			template <typename ForwardIterator, class Size>
			ForwardIterator		_uninitialized_fill(ForwardIterator first, Size count, const value_type& value);

		//for ambiguity when 2 same template type, SFINAE-friendly
			//range constructor
			template <typename Integer>
			void	_initialize_dispatch(Integer n, Integer val, true_type);
			template <typename InputIterator>
			void	_initialize_dispatch(InputIterator first, InputIterator last, false_type);

			//assign
			template <typename Integer>
			void	_assign_dispatch(Integer n, Integer val, true_type);
			template <typename InputIterator>
			void	_assign_dispatch(InputIterator first,InputIterator last, false_type);

			//insert
			template <typename Integer>
			void	_insert_dispatch(iterator position, Integer n, Integer val, true_type);
			void	_fill_insert(iterator position, size_type n, const value_type& val);
			template <typename ForwardIterator>
			void	_range_insert(iterator position, ForwardIterator first, ForwardIterator last);
			void	_realloc_insert(iterator position, const value_type& val);
			template <typename ForwardIterator>
			void	_insert_dispatch(iterator position, ForwardIterator n, ForwardIterator val, false_type);

		protected:
			allocator_type	_impl;
			pointer			_start;
			pointer			_finish;
			pointer			_end_of_storage;

		public:
			explicit	vector(const Alloc& alloc = Alloc());
			explicit	vector(size_type n, const T& val = T(), const Alloc& alloc = Alloc());
			template <typename InputIterator>
			vector(InputIterator first, InputIterator last, const Alloc& alloc = Alloc());
			vector(const vector<T, Alloc>& x);
			~vector();
			vector<T, Alloc>&	operator=(const vector<T, Alloc>& x);

			//iterators
			iterator				begin();
			const_iterator			begin() const;
			iterator				end();
			const_iterator			end() const;
			reverse_iterator		rbegin();
			const_reverse_iterator	rbegin() const;
			reverse_iterator		rend();
			const_reverse_iterator	rend() const;

			//capacity
			size_type	size() const;
			size_type	max_size() const;
			bool		empty() const;
			void		resize(size_type n, T val = T());
			size_type	capacity() const;
			void		reserve(size_type n);

			//element access
			reference		operator[] (size_type n);
			const_reference	operator[] (size_type n) const;
			reference		at(size_type n);
			const_reference	at(size_type n) const;
			reference		front();
			const_reference	front() const;
			reference		back();
			const_reference	back() const;

			//modifiers
			template <typename InputIterator>
			void		assign(InputIterator first, InputIterator last);
			void		assign(size_type n, const T& t);
			iterator	insert(iterator position, const T& val);
			void		insert(iterator position, size_type n, const T& val);
			template <typename InputIterator>
			void		insert(iterator position, InputIterator first, InputIterator last);
			void		push_back(const T& t);
			void		pop_back();
			iterator	erase(iterator position);
			iterator	erase(iterator first, iterator last);
			void		swap(vector<T, Alloc>& x);
			void		clear();

			//Allocator
			allocator_type	get_allocator() const;
	};

	_HEADER_VECT bool	operator==(const vector<T,Alloc>& x, const vector<T,Alloc>& y);
	_HEADER_VECT bool	operator<(const vector<T,Alloc>& x, const vector<T,Alloc>& y);
	_HEADER_VECT bool	operator!=(const vector<T,Alloc>& x, const vector<T,Alloc>& y);
	_HEADER_VECT bool	operator<=(const vector<T,Alloc>& x, const vector<T,Alloc>& y);
	_HEADER_VECT bool	operator>(const vector<T,Alloc>& x, const vector<T,Alloc>& y);
	_HEADER_VECT bool	operator>=(const vector<T,Alloc>& x, const vector<T,Alloc>& y);
	_HEADER_VECT void	swap(vector<T, Alloc>& x, vector<T, Alloc>& y);
}

/********************\
** PRIVATE FUNCTIONS *
\********************/

_HEADER_VECT
typename ft::vector<T, Alloc>::size_type	ft::vector<T, Alloc>::_check_len(size_type n, const char* s) const {
	if (this->max_size() - this->size() < n)
		throw std::length_error(s);
	const size_type	len = this->size() + std::max(this->size(), n);
	return (len < this->size() || len > this->max_size() ? this->max_size() : len);
}

_HEADER_VECT
void	ft::vector<T, Alloc>::_check_range(size_type n) const {
	if (n >= this->size())
		throw std::out_of_range("vector::_range_check");
}

_HEADER_VECT
void	ft::vector<T, Alloc>::_destroy(pointer first, pointer last) {
	for (; first != last; ++first) 
		this->_impl.destroy(first);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::pointer	ft::vector<T, Alloc>::_move_backward(pointer first, pointer last, pointer result) {
	while (last != first)
		*(--result) = *(--last);
	return result;
}

_HEADER_VECT
template <typename InputIterator, typename ForwardIterator>
ForwardIterator	ft::vector<T, Alloc>::_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator d_first) {
	while (first != last) {
		this->_impl.construct(d_first, *first);
		++first; ++d_first;
	}
	return d_first;
}

_HEADER_VECT
template <typename InputIt, typename OutputIt>
OutputIt	ft::vector<T, Alloc>::_copy(InputIt first, InputIt last, OutputIt d_first) {
	while (first != last) {
		*d_first++ = *first++;
	}
	return d_first;
}

_HEADER_VECT
void	ft::vector<T, Alloc>::_fill(pointer first, pointer last, const T& value) {
	for (; first != last; ++first) {
		*first = value;
	}
}

_HEADER_VECT
template <typename ForwardIterator, class Size>
ForwardIterator	ft::vector<T, Alloc>::_uninitialized_fill(ForwardIterator first, Size count, const value_type& val) {
	while (count > 0) {
		this->_impl.construct(first, val);
		++first;
		--count;
	}
	return first;
}

_HEADER_VECT
template<typename Integer>
void	ft::vector<T, Alloc>::_initialize_dispatch(Integer n, Integer val, true_type) {
	this->_start = this->_impl.allocate(static_cast<size_type>(n));
	this->_end_of_storage = this->_start + static_cast<size_type>(n);
	this->_finish = this->_uninitialized_fill(this->_start, static_cast<size_type>(n), static_cast<value_type>(val));
}

_HEADER_VECT
template <typename InputIterator>
void	ft::vector<T, Alloc>::_initialize_dispatch(InputIterator first, InputIterator last, false_type) {
	if (typeid(typename iterator_traits<InputIterator>::iterator_category) == typeid(input_iterator_tag)) {
		for (; first != last; ++first) this->push_back(*first);
	}
	else {
		size_type	n = ft::distance(first, last);

		this->_start = this->_impl.allocate(n);
		this->_end_of_storage = this->_start + n;
		this->_finish = this->_uninitialized_copy(first, last, this->_start);
	}
}

_HEADER_VECT
template <typename Integer>
void	ft::vector<T, Alloc>::_assign_dispatch(Integer n, Integer val, true_type) {
	const size_type	_n(n);

	if (_n > this->capacity()) {
		ft::vector<T, Alloc>	tmp(_n, val);
		tmp.swap(*this);
	}
	else if (_n > this->size()) {
		this->_fill(this->_start, this->_finish, val);
		const size_type	add = _n - this->size();
		this->_finish = this->_uninitialized_fill(this->_finish, add, val);
	}
	else {
		this->_fill(this->_start, this->_start + _n, val);
		this->_destroy(this->_start + _n, this->_finish);
		this->_finish = this->_start + _n;
	}
}

_HEADER_VECT
template <typename InputIterator>
void	ft::vector<T, Alloc>::_assign_dispatch(InputIterator first,InputIterator last, false_type) {
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
			this->insert(this->end(), *first);
	}
}

_HEADER_VECT
template <typename Integer>
void	ft::vector<T, Alloc>::_insert_dispatch(iterator position, Integer n, Integer val, true_type) {
	this->_fill_insert(position, n, val);
}

_HEADER_VECT
void	ft::vector<T, Alloc>::_fill_insert(iterator position, size_type n, const value_type& val) {
	if (n != 0) {
		const size_type	_n(n);

		if (size_type(this->_end_of_storage - this->_finish) >= _n) {
			const size_type	elems_after = this->end() - position;
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
			const size_type	elems_before = position - this->begin();
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

_HEADER_VECT
void	ft::vector<T, Alloc>::_realloc_insert(iterator position, const value_type& val) {
	const size_type	len = this->_check_len(size_type(1), "vector::insert");
	const size_type	elems_before = position - this->begin();
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

_HEADER_VECT
template <typename ForwardIterator>
void	ft::vector<T, Alloc>::_range_insert(iterator position, ForwardIterator first, ForwardIterator last) {
	if (first != last) {
		const size_type	n = ft::distance(first, last);

		if (size_type(this->_end_of_storage - this->_finish) >= n) {
			const size_type	elems_after = this->end() - position;
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

_HEADER_VECT
template <typename ForwardIterator>
void	ft::vector<T, Alloc>::_insert_dispatch(iterator position, ForwardIterator first, ForwardIterator last, false_type) {
	this->_range_insert(position, first, last);	
}

/*****************************\
** MEMBER FUNCTION DEFINITION *
\*****************************/

//Constructors
_HEADER_VECT
ft::vector<T, Alloc>::vector(const Alloc& alloc) : _impl(alloc), _start(0), _finish(0), _end_of_storage(0) {
}

_HEADER_VECT
ft::vector<T, Alloc>::vector(size_type n, const value_type& val, const Alloc& alloc) : _impl(alloc) {
	this->_start = this->_impl.allocate(n);
	this->_end_of_storage = this->_start + n;
	this->_finish = this->_uninitialized_fill(this->_start, n, val);
}

_HEADER_VECT
ft::vector<T, Alloc>::vector(const vector<T, Alloc>& x) : _impl(x._impl) {
	size_type	n = x.size();

	this->_start = this->_impl.allocate(n);
	this->_end_of_storage = this->_start + n;
	this->_finish = this->_uninitialized_copy(x._start, x._finish, this->_start);
}

_HEADER_VECT
template <typename InputIterator>
ft::vector<T, Alloc>::vector(InputIterator first, InputIterator last, const Alloc& alloc) : _impl(alloc) {
	typedef typename	ft::is_integral<InputIterator>::type Integral;

	this->_initialize_dispatch(first, last, Integral());
}

//Destructor
_HEADER_VECT
ft::vector<T, Alloc>::~vector() {
	this->clear();
	this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
}

//Assign operator
_HEADER_VECT
typename ft::vector<T, Alloc>&	ft::vector<T, Alloc>::operator=(const vector<T, Alloc>& x) {
	if (this != &x) {
		size_type	xlen = x.size();
		pointer		xfirst = x._start;
		pointer		start = 0;
		pointer		tmp = 0;

		if (xlen > this->capacity()) {
			tmp = this->_impl.allocate(xlen);
			start = tmp;
			while (xfirst != x._finish)
				*start++ = *xfirst++;
			start = this->_start;
			while (start != this->_finish) {
				this->_impl.destroy(start);
				++start;
			}
			this->_impl.deallocate(this->_start, this->_end_of_storage - this->_start);
			this->_start = tmp;
			this->_end_of_storage = this->_start + xlen;
		}
		else if (this->size() >= xlen) {
			start = this->_start;
			while (xfirst != x._finish)
				*start++ = *xfirst++;
			while (start != this->_finish) {
				this->_impl.destroy(start);
				++start;
			}
		}
		else {
			start = this->_start;
			while (xfirst != x._start + xlen)
				*start++ = *xfirst++;
			start = this->_finish;
			while (xfirst != x._finish)
				*start++ = *xfirst++;
		}
		this->_finish = this->_start + xlen;
	}
	return *this;
}

//Iterators
_HEADER_VECT
typename ft::vector<T, Alloc>::iterator		ft::vector<T, Alloc>::begin() {
	return iterator(this->_start);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_iterator	ft::vector<T, Alloc>::begin() const {
	return const_iterator(this->_start);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::iterator			ft::vector<T, Alloc>::end() {
	return iterator(this->_finish);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_iterator	ft::vector<T, Alloc>::end() const {
	return const_iterator(this->_finish);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::reverse_iterator		ft::vector<T, Alloc>::rbegin() {
	return reverse_iterator(end());
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_reverse_iterator	ft::vector<T, Alloc>::rbegin() const {
	return const_reverse_iterator(end());
}

_HEADER_VECT
typename ft::vector<T, Alloc>::reverse_iterator		ft::vector<T, Alloc>::rend() {
	return reverse_iterator(begin());
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_reverse_iterator	ft::vector<T, Alloc>::rend() const {
	return const_reverse_iterator(begin());
}

//Capacity
_HEADER_VECT
typename ft::vector<T, Alloc>::size_type	ft::vector<T, Alloc>::size() const {
	return size_type(this->_finish - this->_start);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::size_type	ft::vector<T, Alloc>::max_size() const {
	return this->_impl.max_size();
}

_HEADER_VECT
bool	ft::vector<T, Alloc>::empty() const {
	return this->_start == this->_finish;
}

_HEADER_VECT
void	ft::vector<T, Alloc>::resize(size_type n, T val) {
	if (n > this->size())
		this->insert(this->end(), n - this->size(), val);
	else if (n < this->size()) {
		this->_destroy(this->_start + n, this->_finish);
		this->_finish = this->_start + n;
	}
}

_HEADER_VECT
typename ft::vector<T, Alloc>::size_type	ft::vector<T, Alloc>::capacity() const {
	return size_type(this->_end_of_storage - this->_start);
}

_HEADER_VECT
void	ft::vector<T, Alloc>::reserve(size_type n) {
	if (n >= this->max_size())
		throw std::length_error("vector::reserve");
	if (this->capacity() < n) {
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
_HEADER_VECT
typename ft::vector<T, Alloc>::reference	ft::vector<T, Alloc>::at(size_type n) {
	_check_range(n);
	return (*this)[n];
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_reference	ft::vector<T, Alloc>::at(size_type n) const {
	_check_range(n);
	return (*this)[n];
}

_HEADER_VECT
typename ft::vector<T, Alloc>::reference	ft::vector<T, Alloc>::operator[] (size_type n) {
	return *(this->begin() + n);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_reference	ft::vector<T, Alloc>::operator[] (size_type n) const {
	return *(this->begin() + n);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::reference	ft::vector<T, Alloc>::front() {
	return *(this->begin());
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_reference	ft::vector<T, Alloc>::front() const {
	return *(this->begin());
}

_HEADER_VECT
typename ft::vector<T, Alloc>::reference	ft::vector<T, Alloc>::back() {
	return *(this->end() - 1);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::const_reference	ft::vector<T, Alloc>::back() const {
	return *(this->end() - 1);
}

//Modifiers
_HEADER_VECT
typename ft::vector<T, Alloc>::iterator	ft::vector<T, Alloc>::insert(iterator position, const T& val) {
	const size_type	n = position - this->begin();

	if (this->_finish != this->_end_of_storage && position == this->end()) {
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

_HEADER_VECT
void	ft::vector<T, Alloc>::insert(iterator position, size_type n, const value_type& val) {
	this->_fill_insert(position, n, val);
}

_HEADER_VECT
template <typename InputIterator>
void	ft::vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
	typedef typename	ft::is_integral<InputIterator>::type	Integral;

	this->_insert_dispatch(position, first, last, Integral());
}

_HEADER_VECT
template <typename InputIterator>
void	ft::vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	typedef typename	ft::is_integral<InputIterator>::type	Integral;

	this->_assign_dispatch(first, last, Integral());
}

_HEADER_VECT
void	ft::vector<T, Alloc>::assign(size_type n, const T& t) {
	this->erase(this->begin(), this->end());
	this->insert(this->begin(), n, t);
}

_HEADER_VECT
void	ft::vector<T, Alloc>::push_back(const T& t) {
	if (this->_finish != this->_end_of_storage) {
		this->_impl.construct(this->_finish, t);
		++this->_finish;
	}
	else
		this->_realloc_insert(this->end(), t);
}

_HEADER_VECT
void	ft::vector<T, Alloc>::pop_back() {
	--this->_finish;
	this->_impl.destroy(this->_finish);
}

_HEADER_VECT
typename ft::vector<T, Alloc>::iterator		ft::vector<T, Alloc>::erase(iterator position) {
	iterator	cpy(position);

	if (position + 1 != this->end()) {
		while ((position + 1) != this->end()) {
			*position++ = *(position + 1);
		}
	}
	--this->_finish;
	this->_impl.destroy(this->_finish);
	return cpy;
}

_HEADER_VECT
typename ft::vector<T, Alloc>::iterator		ft::vector<T, Alloc>::erase(iterator first, iterator last) {
	iterator	cpy(first);

	if (first != last) {
		if (last != this->end()) {
			while (last != this->end()) {
				*first++ = *last++;
			}
		}
		this->_destroy(first.base() + (this->end() - last), this->_finish);
		this->_finish = first.base() + (this->end() - last);
	}
	return cpy;
}

_HEADER_VECT
void	ft::vector<T, Alloc>::swap(vector<T, Alloc>& x) {
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

_HEADER_VECT
void	ft::vector<T, Alloc>::clear() {
	this->erase(this->begin(), this->end());
}

//Allocator
_HEADER_VECT
typename ft::vector<T, Alloc>::allocator_type	ft::vector<T, Alloc>::get_allocator() const {
	return this->_impl;
}

/*********************************\
** NON MEMBER FUNCTION DEFINITION *
\*********************************/

_HEADER_VECT
bool	ft::operator==(const vector<T,Alloc>& x, const vector<T,Alloc>& y) {
	return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()));
}

_HEADER_VECT
bool	ft::operator<(const vector<T,Alloc>& x, const vector<T,Alloc>& y) {
	return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

_HEADER_VECT
bool	ft::operator!=(const vector<T,Alloc>& x, const vector<T,Alloc>& y) {
	return !(x == y);
}

_HEADER_VECT
bool	ft::operator<=(const vector<T,Alloc>& x, const vector<T,Alloc>& y) {
	return !(y < x);
}

_HEADER_VECT
bool	ft::operator>(const vector<T,Alloc>& x, const vector<T,Alloc>& y) {
	return (y < x);
}

_HEADER_VECT
bool	ft::operator>=(const vector<T,Alloc>& x, const vector<T,Alloc>& y) {
	return !(x < y);
}

_HEADER_VECT
void	ft::swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
	x.swap(y);
}
#endif