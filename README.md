# nothing_but&lt;T&gt;

**Value Handle to Avoid Implicit Conversions in standard C++**

## Motivation

Probably you indulge into recreational C++ and "implicit type conversions" are just "normal thing to do". Or you take them so seriously you actually do not use C++ on mission critical projects, just because of "implicit type conversions" ? Perhaps your company delivers  firmware to be used inside some medical device. Flight control software, core component? 

You might take the C++ [implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) so seriously that "even" the following is very serious matter for you.

```cpp
   // implicit conversion of int to char
   char C = 64 ;
   // implicit conversion of double to float
   float F = 3.7;
```
Not because you happen to be "unreasonable", but because yuo need to deliver code where implicit conversions are simply not allowed. 
Just like for example exceptions, in many real-time projects are not allowed. Them exceptions simply do not exist over there. Simply switched off.

And very likely, you have turned to the "[official sources](https://docs.microsoft.com/en-us/cpp/cpp/type-conversions-and-type-safety-modern-cpp?view=vs-2017)" just to come back dissapointed.

One options is to plan for usual but extended: test/debug/test/debug, cycles. Ad infinitum, of course.

### The suggestion

Before you discard (with the heavy heart) C++ for mission critical projects completely, we might suggest you look into this ridiculously tiny single header? Here is some code to tickle your fancy.

```cpp
// let's assume your code must not accidentaly mix
// signed and unsigned chars
    using just_signed   = dbj::util::nothing_but<signed char>;
    using just_unsigned = dbj::util::nothing_but<unsigned char>;
```
Just declarations first. As ever, make default initialized content, but with a twist: of exactly the types required.
```cpp
    just_signed s;
    just_unsigned u;
```

Now comes the interesting part. To actually assign anything to these types you must very conciously make those types first.
```cpp
    s =  int8_t('s');
    u = uint8_t('u');
```
You, or your team simply can not introduce a bug there. The following will simply not compile.
```cpp
s = 's'; // does not compile
u = 'u'; // does not compile
s = u;   // does not compile
s == u;  // does not compile
```
Just a perfect API to avoid those nasty little pests growing into bugs very difficult to find.

Now, this might seem like a "not a lot of code" to you, but we are just showing an API new to you. Above code looks almost too simple. It is easy to forget the safety service this API provides.

Now the really worn out phrase: *Your imagination is the limit*. Very true here.

### Type's handled

This API does handle all the arithmetic types.

- integral types	
    - bool
    - char
    - char16_t
    - char32_t
    - wchar_t
    - signed char
    - short int
    - int
    - long int
    - long long int
    - unsigned char
    - unsigned short int
    - unsigned int
    - unsigned long int
    - unsigned long long int
- floating point types
     - float
     - double
     - long double

These are the types where implicit conversions do happen, by design.

But what about compound types?  For example what about:

- References
- Pointers
- Arrays

Why not handling them too?  Simply because, in case you need them 
you will naturaly use them as ever before, but combined with this API.

```cpp
     // 0 of a signed char type
     just_signed signed_char ; 
     // pointer to it
     just_signed * signed_char_pointer ; // null_ptr as ever_
     // reference to it
     just_signed & signed_char_ref = signed_char ;
     //  array of three elements of it
     just_signed signed_char_arr[3]_;
```

Please do note, how above, all the standard C++ 
default value initialization rules are respected.

In case of some serious bugs, singularities discovered, or edge case, we will reconsider the currently handled types. 

Going beyond arithmetics it is very inlikely the implicit conversion might be the problem in your code.

## Dependencies

This API depends on C++ std lib only. We are developing using the Visual Studio 2017 15.9.X, 
But we are always checking it is equaly usable with both CLANG and GCC.

## Installation

This API is header-only: it consists entirely of one header file:

       dbj_nothing_but.h

*No compilation necessary. No installation required.* 

Just drop it in and include. Make it part of your project.


Every attempt has been made to make this into a cross-platform, 
header only, standard C++ library.

At time of this writting (2019Q1) standard C++ is C++17. 

This project builds and tests are passing with C++14 (/std:c++14)

This project builds and tests are passing with C++17 (/std:c++17)

**streams**

To include std::ostram support please 
hash define `DBJ_NOTHING_BUT_STREAMS`, before including this API

```cpp
// std::ostream support
#define DBJ_NOTHING_BUT_STREAMS
#include <dbj_nothing_but.h>

```

### Other work, libraries and efforts

I know of no other so simple C++ code achieveing the same goal.
Of course there are other very clever people that have done similliar things like,I have attempted here.

- Boost [Safe Numerics](https://github.com/boostorg/safe_numerics/tree/develop/include/boost/safe_numerics) (Robert Ramey, 2012)
    - Part of Boost suite, thus (very) intertwined with the rest of Boost.
- Dani Le Blanc (Microsoft) [SafeInt](https://github.com/dcleblanc/SafeInt)
    - Used by MSFT teams, for Office, Windows etc.
    - This is decades old, but still has only 8 forks on the GitHub?
    - One can safely assume, there has to be internal C version for Windows code itself.

Above are two probably most mature C++ lib's. 

They primarily exist to asure safe arythemtics on "numbers", not to provide type safe handler like NothingBut<T> does.
None of them is built to stop accidental implicit conversion in C++.

### Contact
Please report issues or [questions here](https://github.com/dbj-systems/dbj-laboratorium/issues).
 
Best way to contact me is the "Issues" tab.
### Contributing
Any feedback from users and stakeholders will be used to improve the library.
### License
![CC BY-SA 4.0](https://mirrors.creativecommons.org/presskit/buttons/88x31/png/by-sa.png)
#### [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)
#### &copy; 2019 by dbj@dbj.org
---------
[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  
