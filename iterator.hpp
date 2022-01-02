#pragma once
# ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include "iterator_base.hpp"
# include "type_traits.hpp"

namespace ft {

	template <class Iterator>
	class	reverse_iterator
	:	public iterator<typename iterator_traits<Iterator>::iterator_category,
						typename iterator_traits<Iterator>::value_type,
						typename iterator_traits<Iterator>::difference_type,
						typename iterator_traits<Iterator>::pointer,
						typename iterator_traits<Iterator>::reference> {
		protected:
			Iterator	_current;
		public:
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::reference			reference;

			reverse_iterator() : _current() {}
			explicit reverse_iterator(iterator_type x) : _current(x) {}
			reverse_iterator(const reverse_iterator& x) : _current(x._current) {}
			template <typename Iter>
			reverse_iterator(const reverse_iterator<Iter>& x) : _current(x.base()) {}

			template <typename Iter>
			reverse_iterator&	operator=(const reverse_iterator<Iter>& x) {
				_current = x.base();
				return *this;
			}

			iterator_type	base() const
			{ return _current; }

			reference	operator*() const {
				iterator_type	tmp = _current;
				return *--tmp;
			}

			pointer	operator->() const
			{ return &(operator*()); }

			reverse_iterator&	operator++() {
				--_current;
				return *this;
			}

			reverse_iterator	operator++(int) {
				reverse_iterator	tmp = *this;
				--_current;
				return tmp;
			}

			reverse_iterator&	operator--() {
				++_current;
				return *this;
			}

			reverse_iterator	operator--(int) {
				reverse_iterator	tmp = *this;
				++_current;
				return tmp;
			}

			reverse_iterator	operator+(difference_type n) const
			{ return reverse_iterator(_current - n); }

			reverse_iterator&	operator+=(difference_type n) {
				_current -= n;
				return *this;
			}

			reverse_iterator	operator-(difference_type n) const
			{ return reverse_iterator(_current + n); }

			reverse_iterator&	operator-=(difference_type n) {
				_current += n;
				return *this;
			}

			reference	operator[](difference_type n) const
			{ return _current[-n-1]; }
	};

	template <class Iterator>
	inline bool
	operator==(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return x.base() == y.base(); }

	template <class IteratorL, class IteratorR>
	inline bool
	operator==(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return x.base() == y.base(); }

	template <class Iterator>
	inline bool
	operator!=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return x.base() != y.base(); }

	template <class IteratorL, class IteratorR>
	inline bool
	operator!=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return x.base() != y.base(); }

	template <class Iterator>
	inline bool
	operator<(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return x.base() > y.base(); }

	template <class IteratorL, class IteratorR>
	inline bool
	operator<(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return x.base() > y.base(); }

	template <class Iterator>
	inline bool
	operator>(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return x.base() < y.base(); }

	template <class IteratorL, class IteratorR>
	inline bool
	operator>(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return x.base() < y.base(); }

	template <class Iterator>
	inline bool
	operator>=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return x.base() <= y.base(); }

	template <class IteratorL, class IteratorR>
	inline bool
	operator>=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return x.base() <= y.base(); }

	template <class Iterator>
	inline bool
	operator<=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return x.base() >= y.base(); }

	template <class IteratorL, class IteratorR>
	inline bool
	operator<=(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return x.base() >= y.base(); }

	template <class Iterator>
	inline reverse_iterator<Iterator>
	operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x)
	{ return reverse_iterator<Iterator>(x.base() - n); }

	template <class IteratorL, class IteratorR>
	inline typename reverse_iterator<IteratorL>::difference_type
	operator-(const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y)
	{ return y.base() - x.base(); }

	template <class Iterator>
	inline typename reverse_iterator<Iterator>::difference_type
	operator-(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y)
	{ return y.base() - x.base(); }

	// Primary purpose is to convert an iterator that is
	// not a class, e.g. a pointer, into an iterator that is a class.
	template <typename Iterator, typename Container>
	class	normal_iterator {
		protected:
			Iterator	_current;
		public:
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::reference			reference;
			typedef typename iterator_traits<Iterator>::pointer				pointer;

			normal_iterator() : _current(Iterator()) {}
			explicit normal_iterator(const Iterator& i) : _current(i) {}

			//const to normal
			template <typename Iter>
			normal_iterator(const normal_iterator<Iter, typename enable_if<
			(are_same<Iter, typename Container::pointer>::value),
			Container>::type>& i) : _current(i.base()) {}

			normal_iterator&	operator=(const normal_iterator& x) {
				_current = x._current;
				return *this;
			}

			reference	operator*() const
			{ return *_current; }

			pointer	operator->() const
			{ return _current; }

			normal_iterator&	operator++() {
				++_current;
				return *this;
			}

			normal_iterator	operator++(int)
			{ return normal_iterator(_current++); }

			normal_iterator&	operator--() {
				--_current;
				return *this;
			}

			normal_iterator	operator--(int)
			{ return normal_iterator(_current--); }

			reference	operator[](difference_type n) const
			{ return _current[n]; }

			normal_iterator&	operator+=(difference_type n) {
				_current += n;
				return *this;
			}

			normal_iterator	operator+(difference_type n) const
			{ return normal_iterator(_current + n); }

			normal_iterator&	operator-=(difference_type n) {
				_current -= n;
				return *this;
			}

			normal_iterator	operator-(difference_type n) const
			{ return normal_iterator(_current - n); }

			const Iterator&	base() const
			{ return _current; }
	};

	//overloads normal iterators
	template <typename IteratorL, typename IteratorR, typename Container>
	inline bool
	operator==(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() == rhs.base(); }

	template <typename Iterator, typename Container>
	inline bool
	operator==(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() == rhs.base(); }

	template <typename IteratorL, typename IteratorR, typename Container>
	inline bool
	operator!=(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() != rhs.base(); }

	template <typename Iterator, typename Container>
	inline bool
	operator!=(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() != rhs.base(); }

	template <typename IteratorL, typename IteratorR, typename Container>
	inline bool
	operator<(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() < rhs.base(); }

	template <typename Iterator, typename Container>
	inline bool
	operator<(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() < rhs.base(); }

	template <typename IteratorL, typename IteratorR, typename Container>
	inline bool
	operator>(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() > rhs.base(); }

	template <typename Iterator, typename Container>
	inline bool
	operator>(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() > rhs.base(); }

	template <typename IteratorL, typename IteratorR, typename Container>
	inline bool
	operator<=(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() <= rhs.base(); }

	template <typename Iterator, typename Container>
	inline bool
	operator<=(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() <= rhs.base(); }

	template <typename IteratorL, typename IteratorR, typename Container>
	inline bool
	operator>=(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() >= rhs.base(); }

	template <typename Iterator, typename Container>
	inline bool
	operator>=(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() >= rhs.base(); }

	template <typename IteratorL, typename IteratorR, typename Container>
	inline typename normal_iterator<IteratorL, Container>::difference_type
	operator-(const normal_iterator<IteratorL, Container>& lhs, const normal_iterator<IteratorR, Container>& rhs)
	{ return lhs.base() - rhs.base(); }

	template <typename Iterator, typename Container>
	inline typename normal_iterator<Iterator, Container>::difference_type
	operator-(const normal_iterator<Iterator, Container>& lhs, const normal_iterator<Iterator, Container>& rhs)
	{ return lhs.base() - rhs.base(); }

	template <typename Iterator, typename Container>
	inline normal_iterator<Iterator, Container>
	operator+(typename normal_iterator<Iterator, Container>::difference_type n, const normal_iterator<Iterator, Container>& i)
	{ return normal_iterator<Iterator, Container>(i.base() + n); }

}//namespace

#endif