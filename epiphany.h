#ifndef DBJ_EPIPHANY_INC_
#define DBJ_EPIPHANY_INC_
/*
Copyright 2019 - 2021 by Dušan B. Jovanović (dbj@dbj.org)

https://dbj.org/license_dbj

This is bastardized small portion of
https://github.com/google/s2geometry/blob/master/src/s2/base/port.h


TO use this header include it. This is just a nucleus and inspiration for a
full solution tuned for YOUR project.

But what is this and why does it exist? This is for that moment when it hits you
what can happen with implicit or silent casting sometimes called coercion or implicit conversion.

When that happens one does hope for the remedy. And that means imposing this kind of header on the project.
This is for MSVC. For GNUC or LLVM please devise your own type coercion epiphany after support.

In here we basically make some type coercion warnings into errors.

For less drastic measures please see here:
https://docs.microsoft.com/en-us/cpp/preprocessor/compiler-warnings-that-are-off-by-default?view=msvc-160

This header is for when using cl.exe and clang-cl.exe.
*/

#ifdef _MSC_VER 

// This compiler flag can be easily overlooked on MSVC.
// _CHAR_UNSIGNED gets set with the /J flag.
#ifndef _CHAR_UNSIGNED
#error chars must be unsigned!  Use the /J flag on the compiler command line.
#endif

// DO NOT allow comparisons between signed and unsigned values. Lots of YOUR code uses this pattern:
// 
//   for (int i = 0; i < container.size(); ++i)
// 
// Since size() returns an unsigned value, this warning would trigger
// frequently.  
// Unfortunately, there are bugs related to signed-unsigned comparisons that have been missed because YOU disabled this
// warning.  Thus we shall turn these two into the errors
#pragma warning(error : 4018)
#pragma warning(error : 4267)

// Do warn about unused local variables.
//
// This sometimes catches real bugs.
#pragma warning(disable : 4101)  // level 3

// DO NOT allow initialization and assignment to a smaller type without warnings about
// possible loss of data.
//
// There is a distinct warning, 4267, that warns about size_t conversions to
// smaller types.
//
// This warning triggers at levels 2, 3, and 4 depending on the specific
// type of conversion.  By disabling it, YOU not only silence minor narrowing
// conversions but also serious ones. 
// thus we shall turn it into the error
#pragma warning(error : 4244)  

// DO NOT allow silent truncation of double to float.
// Silencing this warning causes some subtle bugs.
// thus we shall turn it into the error
#pragma warning(error : 4305)  // level 1

// DO NOT allow a constant to be assigned to a type that is too small.
//
// Why do YOU allow this at all?  I can't think of a case where this
// wouldn't be a bug, making this warning into error breaks many builds today.
#pragma warning(error : 4307)  // level 2

// DO NOT allow implicit coercion from an integral type to a bool.
// Af course this also means C99 or better where _Bool is a keyword
#pragma warning(error : 4800)  // level 3

// here add your own remedies after your little epiphany moments

#endif  // _MSC_VER

#endif // !DBJ_EPIPHANY_INC_

