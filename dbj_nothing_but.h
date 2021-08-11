#pragma once

#ifdef __clang__
#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma clang system_header
#endif // __clang__
/*
Value Handle to Avoid Implicit Conversions in standard C++

Copyright 2019 - 2021 by Dušan B. Jovanović (dbj@dbj.org)

https://dbj.org/license_dbj

*/

#include <type_traits>
#include <utility>

namespace dbj {

	namespace util {
		using namespace std;

#define DBJ_UTIL_NOTHING_BUT_VERSION "0.6.1"

		/*
		On C++ types, start from here:
		https://en.cppreference.com/w/cpp/types/is_fundamental
		Bellow one can see what types we currently allow.
		Basicaly only Arythmetic types
		(https://en.cppreference.com/w/cpp/types/is_arithmetic)

		ROADMAP: will be using requirements with C++20
		*/
		template< class T >
		struct is_handled_by_nothing_but
			: std::integral_constant<
			bool,
			std::is_arithmetic<T>::value&&
			std::is_trivially_constructible<T>::value
			> {};
		/*
		Arithmetic literals

		in general and in case you use them your code will be less of a surprise to you
		in particular you will have much less surprises when mixing literals and
		instances of nothing_but<T>

		integer literal ( https://en.cppreference.com/w/cpp/language/integer_literal )

		floating point literal (https://en.cppreference.com/w/cpp/language/floating_literal)
		The hexadecimal floating-point literals were not part of C++ until C++17,
		although they can be parsed and printed by the I/O functions since C++11

		character literal (https://en.cppreference.com/w/cpp/language/character_literal)

		string literal (https://en.cppreference.com/w/cpp/language/string_literal)

		avoid implicit conversions to/ from type T
		by handling it through this class

		terminology: type X is any other type but T

		NOTE! static_assert() kick's in *not* before
		isntance of a type is made.
		Using T1 = T2 will not use the static_assert's inside T2

		If not for SFINAE, the wrong type will be made and users
		will not be stopped untill they try to make the instance.
		*/
		template<typename T,
			std::enable_if_t< is_handled_by_nothing_but<T>::value, int> = 0
		>
			struct nothing_but final
		{
			using type = nothing_but;
			// as is the std custom
			using value_type = T;

			// default ctor makes default T 
			// must exist, default is fine
			nothing_but() = default;

			// copying --  moving 
			// just leave it all to compiler

			// to convert or assign from T is allowed
			// to move from T is allowed
			nothing_but(T&& t_) noexcept : val_(std::move(t_)) { }
			type& operator = (T&& new_val_) & noexcept { val_ = std::move(new_val_); return *this; }

			// If you suspect that a type of yours is susceptible to accidental 
			// assignment-to-temporary (like our vec is), consider deleting the 
			// rvalue ref-qualified assignments
			type& operator = (T&& new_val_) && noexcept = delete;
			type& operator = (T const& new_val_) && noexcept = delete;

			//to convert or assign from T is allowed
			// by copying
			nothing_but(T const& t_) noexcept : val_(t_) { }
			type& operator = (T const& new_val_) & noexcept { val_ = new_val_; return *this; }

			/* to construct from X is banned */
			// template< typename X, std::enable_if_t<false == std::is_same_v<T, X>, int> = 0>
			template< typename X> nothing_but(X const& x_) = delete;

			/* to assign from X is banned */
			// template<typename X, std::enable_if_t<false == std::is_same_v<T, X>, int> = 0>
			template< typename X> type& operator = (X const& new_val_) = delete;

			// conversion to T& -- but only if not const
			operator T& ()& { return val_; }
			// conversion to T& -- banned on temporaryes
			operator T& () && = delete;

			/* conversion to X is banned */
			// template<typename X, std::enable_if_t<false == std::is_same_v<T, X>, int> = 0 >
			template< typename X>
			operator X& () = delete;

			// as elsewhere in std 
			// the convention is to provide 'data()' method
			T const& data() const& { return val_; }
			// can not get data from temporary 
			T const& data() const&& = delete;
			// for users to reach to non const data handled
			// ref. qualified since 0.6.0
			T& data()& { return (T&)val_; }
			// can not get data from temporary 
			T& data() && = delete;


		private:
			T val_{};

			// compatibility with std::
			// to act as element type in some of std:: containers
			// class has to provide less than operator
			// type T has to be comparable in the same way 
			// that is 'operator <' for type T has ot be available
			friend bool operator < (type const& left_, type const& right_)
			{
				return ((left_.val_) < (right_.val_));
			}

			// type T has to be comparable in the same way 
			// that is 'operator ==' for type T has to be available
			friend bool operator == (type const& left_, type const& right_)
			{
				return ((left_.val_) == (right_.val_));
			}


#ifdef DBJ_NOTHING_USING_IO_STREAMS
			// bellow means type T has to be "printable" too 
			// that is std::ostream & << ( std::ostream, T const & );
			// must be defined and in the scope, when required
			friend std::ostream& operator << (std::ostream& os_, type const& right_)
			{
				return os_ << right_.val_;
			}
#endif
		};

	} // util
} // dbj

// EOF