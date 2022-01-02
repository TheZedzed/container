#pragma once
# ifndef UTILITY_HPP
# define UTILITY_HPP

namespace ft {

	template <typename T1, typename T2>
	struct pair {
		typedef T1	first_type;
		typedef T2	second_type;

		T1	first;
		T2	second;
		pair() : first(T1()), second(T2()) {};
		pair(const T1& x, const T2& y) : first(x), second(y) {};
		template<typename U, typename V>
		pair(const pair<U, V>& p) : first(p.first), second(p.second) {};
		pair	&operator=(const pair &pr) {
			if (this != &pr) {
				first = pr.first;
				second = pr.second;
			}
			return *this;
		}
	};

	template <typename T1, typename T2>
	bool	operator==(const pair<T1, T2>& x, const pair<T1, T2>& y)
	{ return x.first == y.first && x.second == y.second; }

	template <typename T1, typename T2>
	bool	operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
	{ return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }

	template <typename T1, typename T2>
	bool	operator!=(const pair<T1,T2>& x, const pair<T1,T2>& y)
	{ return !(x == y); }

	template <typename T1, typename T2>
	bool	operator<=(const pair<T1,T2>& x, const pair<T1,T2>& y)
	{ return !(y < x); }

	template <typename T1, typename T2>
	bool	operator>(const pair<T1,T2>& x, const pair<T1,T2>& y)
	{ return y < x; }

	template <typename T1, typename T2>
	bool	operator>=(const pair<T1,T2>& x, const pair<T1,T2>& y)
	{ return !(x < y); }

	template <typename T1, typename T2>
	pair<T1, T2>	make_pair(T1 x, T2 y)
	{ return pair<T1, T2>(x, y); }

}//namespace

#endif