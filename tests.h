#ifndef TESTING_INC_
#define TESTING_INC_

#if __clang__
#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif // clang



/*
Testing the Type Handle to Avoid Implicit Conversions in standard C++

 By "testing" in this context we basically mean: "does it or does it not compile"
 So we do not use any "testing framework"

Firmware, drivers, avionics, medical machines, and simillar projects
very likely do not use C++ streams, RTTI, exceptions and such
This is tested in such a environment

	Copyright 2019-2021 dbj@dbj.org
*/

#include "dbj_nothing_but.h"

#include <array>
#include <vector>
#include <stdio.h>
#include <inttypes.h>
#include <cassert>
#include <type_traits>

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4101 )
// warning C4101 : unused local variable
#endif


/*
-----------------------------------------------------------------------------
utils
*/

#undef PRINT
// Show eXpression 
#define PRINT(F_, x_) do { \
printf("\n%4d | %s | %16s : ", __LINE__, __FILE__, (#x_));\
printf( F_, (x_));\
} while(0)

// boolean expression
#undef TF
#define TF(a) (a ? "true" : "false")
#undef PRINB
#define PRINB(x_) PRINT("%s", (TF(x_)) )


namespace dbj {

	template<typename T>
	using remove_cvr_t = typename std::remove_reference_t< std::remove_cv_t<T> >;
} // dbj
/**/
inline auto mover = [](auto arg_) noexcept
{
	return arg_;
};

// safe computing in action
// Safe SiZE
using safe_size = dbj::util::nothing_but< size_t >;
// Safe u char
using unsigned_char_type = dbj::util::nothing_but < unsigned char >;
// safe u char buffer
using uc_buffer_type = std::vector<unsigned_char_type>;

/* safe buffer does not allow mixing singed and unsigned char's */
static uc_buffer_type uc_buffer(safe_size  sz_) noexcept
{
	// create vector of sz length filled with '?'
	return uc_buffer_type(
		safe_size::value_type(sz_),
		unsigned_char_type::value_type('?')
	);
};

inline void test_comparators() noexcept
{
	// Safe SiZE
	using safe_size = dbj::util::nothing_but < size_t >;

	safe_size s1{ size_t(1) };
	safe_size s2{ size_t(2) };

	PRINB(s1 < s2);

	// this should provoke a build warning
	// this is getting the data out 
	safe_size::value_type sze = s1;

	/*
	bellow produces a warning, as we are not using safe_size services

	clang does -Wliteral-conversion here, but only if -W3 or above is used

	message is

	warning : implicit conversion from 'double' to 'safe_size::value_type'
	(aka 'unsigned long long') changes value from 23.4 to 23 [-Wliteral-conversion]
	*/
	sze = 23.4;

	float  f{ float(1.2) };
	double d{ double(3.4) };

	PRINB(f < d);
}

inline void test_vector_walk() noexcept {

	// Safe SiZE
	using safe_size = dbj::util::nothing_but < size_t >;

	// creates vector of 0xF length
	// filled with '?'
	auto buffy_ = uc_buffer(safe_size::value_type(0xF));

	// fill the vector
	safe_size walker = safe_size(safe_size::value_type(0));
	safe_size counter_ = safe_size::value_type(0);
	while (
		walker < safe_size(buffy_.size())
		) {
		// since this is safe buffer of u chars 
		// we need to take care so that we
		// pass u char and notthing but u char
		buffy_[walker] = unsigned_char_type::value_type(
			safe_size::value_type(65 + ((counter_++) % 25))
		);
		walker++;
	}

	printf("\n{");
	// print the vector
	for (auto&& elem_ : buffy_)
	{
		printf(" %c ", elem_.data());
		walker++;
	}
	printf("}\n");
}

/*
basic tests on type T
*/
template<typename T >
inline
dbj::util::nothing_but<T> test_basic(const char* const FMT_)
noexcept
{
	/*
	Attention: T and dbj::remove_cvr_t<T>
	might be two different types
	we will use them latter
	*/
	using TT = typename dbj::remove_cvr_t<T>;
	using NBT = dbj::util::nothing_but<TT>;

	TT value_{};

	// default ctor
	NBT nbt_a;
	NBT nbt_b(value_);
	// assignment
	nbt_a = mover(nbt_b);

	assert(!(nbt_a < nbt_b));

	// compare wrapped up to native T
	PRINB(nbt_a < nbt_b);
	PRINB(nbt_a == nbt_b);

	// notice the use of TT here
	const TT& peeping_tom = nbt_b.data();

	PRINT(FMT_, peeping_tom);

	return nbt_b;
}

inline void test_different_types() noexcept
{
	// fundamental types
	// no can do -- test_basic<void>();
	// no can do -- test_basic<std::nullptr_t>();
	// floating point types
	test_basic<float>("%f");
	test_basic<double>("%d");
	test_basic<long double>("%ld");
	//// integral types
	test_basic<bool>("%d");
	test_basic<char>("%c");
	test_basic<signed char>("%c");
	test_basic<unsigned char>("%c");
	test_basic<char16_t>("%c");
	test_basic<char32_t>("%c");
	test_basic<wchar_t>("%C");
	//// signed integer types
	test_basic<short int>("%d");
	test_basic<int>("%d");
	test_basic<long int>("%dl");
	test_basic<long long int>("%ll");
	//// unsigned integer types
	test_basic<unsigned short int>("%d");
	test_basic<unsigned int>("%du");
	test_basic<unsigned long int>("%zu");
	test_basic<unsigned long long int>("%zu");

}


/*
-----------------------------------------------------------------------------
try to trick with using two similar types
*/
inline void test_try_to_trick() noexcept
{
	/*
	NOTE: in here (MSVC  19.16.27027.1 for x86 )
	type of 2.3 is double
	type of float(2.3) is float
	type of 2.3f is float
	so to convince dbj::util::nothing_but<float>
	to play, please use float() c++ style cast
	or appropriate float literal operators
	as bellow

	(no suffix) defines double
	f F defines float
	l L defines long double

	*/
	using just_float = dbj::util::nothing_but<float>;
	using just_double = dbj::util::nothing_but<double>;

	auto just_float_fun = [](just_float  jf_arg)
		-> just_float
	{ return { 2.3f };  };

	auto just_double_fun = [](just_double jd_arg)
		-> just_double
	{ return { (2.3) }; };

	// if no float() cast
	// 'argument': truncation from 'double' to 'T'
	just_float(2.3f);

	// if no float() cast 
	// 'argument': truncation from 'double' to 'T'
	just_float jf = 2.3f;
	just_float jf_aggregate = { 2.3f };

	just_double(2.3);
	just_double jd = 2.3;

	// no can do -- jf = jd;

	PRINB(jf < jf);
	// no can do -- jf < jd;

	just_float_fun(jf);
	just_double_fun(jd);

}
/*
-----------------------------------------------------------------------------
*/
inline void test_assignments() noexcept
{
	using just_signed = dbj::util::nothing_but<signed char>;
	using just_unsigned = dbj::util::nothing_but<unsigned char>;

	just_signed s;
	just_unsigned u;

	// CLANG/GNUC/G++/MSVC(UCRT) default behaviour is wrong
	// no warnings whatsoever here
	uint8_t uc = int8_t('s');
	int8_t sc = uc;

	s = int8_t('s');
	u = uint8_t('u');

	/*
	no can do:
	s = 's';
	u = 'u';
	s = u;
	s == u;
	*/
}
/*
-----------------------------------------------------------------------------
receive and return ref to array of 3 NBT's made of singed or unsigned chars
*/
template< typename NBT >
inline auto native_array_filler(NBT(&sarr_arg)[3])
noexcept ->NBT(&)[3]
{
	using VT = typename NBT::value_type;

	static_assert(
		std::is_same_v< VT, unsigned char > ||
		std::is_same_v< VT, signed char >
	);

	sarr_arg[0] = NBT(VT('9'));
	sarr_arg[1] = NBT(VT('8'));
	sarr_arg[2] = NBT(VT('7'));

	return sarr_arg;
};

inline void test_compatibility() noexcept
{
	using namespace std;

	auto display = [](auto arg_)
	{
		printf("\n{ %c %c %c }", arg_[0].data(), arg_[1].data(), arg_[2].data());
	};

	using just_signed = dbj::util::nothing_but<signed char>;
	using just_unsigned = dbj::util::nothing_but<unsigned char>;

	// nothing happens implicitly in the world of nothing_but<T>
	just_signed	sarr[3];
	just_unsigned	uarr[3];

	display(native_array_filler(sarr));
	display(native_array_filler(uarr));

	// fill arrays and vectors of size 3
	// made up of signed or unsigned chars
	auto filler = [](auto sarr_arg) noexcept
	{
		using container = decltype(sarr_arg);
		using nothing_but = typename container::value_type;
		using v_type = typename nothing_but::value_type;

		static_assert(
			std::is_same_v< v_type, unsigned char > ||
			std::is_same_v< v_type, signed char >
			);

		assert(sarr_arg.size() == 3);

		sarr_arg[0] = nothing_but(v_type('A'));
		sarr_arg[1] = nothing_but(v_type('B'));
		sarr_arg[2] = nothing_but(v_type('C'));

		return sarr_arg;
	};

	{
		std::vector<just_signed>	std_vec(3);
		std::array<just_signed, 3>	std_arr;

		display(filler(std_arr));
		display(filler(std_vec));
	}
}

#ifdef _WIN32
#pragma warning( pop )
#endif

#endif // TESTING_INC_
