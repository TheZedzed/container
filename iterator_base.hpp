#pragma once
# ifndef ITERATOR_BASE_HPP
# define ITERATOR_BASE_HPP

# include <cstddef>
# include <typeinfo>
# include <iostream>

namespace ft {

	struct	input_iterator_tag { };

	struct	output_iterator_tag { };

	struct	forward_iterator_tag : public input_iterator_tag { };

	struct	bidirectional_iterator_tag : public forward_iterator_tag { };

	struct	random_access_iterator_tag : public bidirectional_iterator_tag { };

	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct	iterator {
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	template <typename Iterator>
	struct	iterator_traits {
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
	};

	template <typename T>
	struct	iterator_traits<T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template <typename T>
	struct	iterator_traits<const T*> {
		typedef	random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef const T*					pointer;
		typedef const T&					reference;
	};

	template <typename InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type	distance(InputIterator first, InputIterator last) {
		typename iterator_traits<InputIterator>::difference_type 	n = 0;

		while (first != last) {
			++first;
			++n;
		}
		return n;
	}

	template <typename InputIterator, typename Distance>
	inline void	advance(InputIterator& i, Distance n) {
		while (n--)
			++i;
	}
}//namespace

#endif