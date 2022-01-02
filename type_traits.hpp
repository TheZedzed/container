#pragma once
# ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

namespace ft {

#define _DEFINE_SPEC(_Order, _Trait, _Type, _Value) \
	template <> struct _Trait<_Type> : public integral_constant<bool, _Value> {};

	template <class T, T v>
	struct integral_constant {
		static const T	value = v;
		typedef T	value_type;
		typedef integral_constant<T, v>	type;
		operator	T() { return v; }
	};
	typedef integral_constant<bool, false>	false_type;
	typedef integral_constant<bool, true>	true_type;

	template <class T> struct	remove_cv					{ typedef T	type; };
	template <class T> struct	remove_cv<const T>			{ typedef T	type; };
	template <class T> struct	remove_cv<volatile T>		{ typedef T	type; };
	template <class T> struct	remove_cv<const volatile T>	{ typedef T	type; };

	template <class T> struct	remove_const				{ typedef T	type; };
	template <class T> struct	remove_const<const T>		{ typedef T	type; };

	template <class T> struct	remove_volatile				{ typedef T	type; };
	template <class T> struct	remove_volatile<volatile T>	{ typedef T	type; };

	template <typename>
	struct __is_integral_helper : public false_type {};
	_DEFINE_SPEC(0, __is_integral_helper, bool, true)
	_DEFINE_SPEC(0, __is_integral_helper, char, true)
	_DEFINE_SPEC(0, __is_integral_helper, signed char, true)
	_DEFINE_SPEC(0, __is_integral_helper, unsigned char, true)
	_DEFINE_SPEC(0, __is_integral_helper, wchar_t, true)
	_DEFINE_SPEC(0, __is_integral_helper, short, true)
	_DEFINE_SPEC(0, __is_integral_helper, unsigned short, true)
	_DEFINE_SPEC(0, __is_integral_helper, int, true)
	_DEFINE_SPEC(0, __is_integral_helper, unsigned int, true)
	_DEFINE_SPEC(0, __is_integral_helper, long, true)
	_DEFINE_SPEC(0, __is_integral_helper, unsigned long, true)
	_DEFINE_SPEC(0, __is_integral_helper, long long, true)
	_DEFINE_SPEC(0, __is_integral_helper, unsigned long long, true)

	template <typename T>
	struct	is_integral
	: public integral_constant<bool, (__is_integral_helper<typename remove_cv<T>::type>::value)> {};

	template <typename, typename>
	struct	are_same {
		enum { value = 0 };
		typedef false_type	type;
	};

	template <typename T>
	struct	are_same<T, T> {
		enum { value = 1 };
		typedef true_type	type;
	};

	template <bool, typename T = void> struct enable_if {};
	template <typename T> struct enable_if<true, T> { typedef T type; };

}//namespace

#endif