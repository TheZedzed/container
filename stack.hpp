#pragma once
# ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

# define _HEADER_STACK \
	template <typename T, typename Container>

namespace ft {

	template <typename T, typename Container = ft::vector<T> >
	class	stack {

		private:
			Container	_c;

		public:
			typedef T			value_type;
			typedef size_t		size_type;
			typedef Container	container_type;

			explicit	stack(const container_type& c = container_type()) : _c(c) {}
			~stack() {}

			bool				empty() const { return _c.empty(); }
			void				push(const value_type& val) { _c.push_back(val); }
			void				pop() { _c.pop_back(); }
			size_type			size() const { return _c.size(); }
			value_type&			top() { return _c.back(); }
			const value_type&	top() const { return _c.back(); }

		template <typename T1, typename A1>
		friend bool	operator==(const stack<T1,A1>&, const stack<T1,A1>&);

		template <typename T1, typename A1>
		friend bool	operator<(const stack<T1,A1>&, const stack<T1,A1>&);
	};

	_HEADER_STACK
	inline bool	operator==(const stack<T,Container>& x, const stack<T,Container>& y)
	{ return x._c == y._c; }

	_HEADER_STACK
	inline bool	operator<(const stack<T,Container>& x, const stack<T,Container>& y)
	{ return x._c < y._c; }

	_HEADER_STACK
	inline bool	operator!=(const stack<T,Container>& x, const stack<T,Container>& y)
	{ return !(x == y); }

	_HEADER_STACK
	inline bool	operator<=(const stack<T,Container>& x, const stack<T,Container>& y)
	{ return !(y < x); }

	_HEADER_STACK
	inline bool	operator>(const stack<T,Container>& x, const stack<T,Container>& y)
	{ return y < x; }

	_HEADER_STACK
	inline bool	operator>=(const stack<T,Container>& x, const stack<T,Container>& y)
	{ return !(x < y); }

}//namespace

#endif