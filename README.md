# nothing_but&lt;T&gt;

> The C++ standard allows the implicit conversion between any two integer types, regardless of their values.
>
> Compiler warnings are unrelated to the code being legal; the compiler just warns you when your code probably does not do what you wanted it to.
>
> If you (or your paymaster) think that is (very) bad, this header might be your saviour.

- For the prose please see the article on the blog
  - https://dbj.org/cpp-how-to-avoid-implicit-conversions/
- Please read through the testing code in here
  - then compile it and follow through Visual Studio Debugger
- This is build with clang-cl aka "clang 10.0.0"
  - thus compatibility issues are not expected.
- Update 2021 APR 23 : epyphany.h added, please look into it


### Other work, libraries and efforts

I know of no other simple C++ code achieveing the same goal.
Of course there are other very clever people that have done similliar things like,I have attempted here.

- Boost [Safe Numerics](https://github.com/boostorg/safe_numerics/tree/develop/include/boost/safe_numerics) (Robert Ramey, 2012)
    - Part of Boost suite, thus (very) intertwined with the rest of Boost.
- Dani Le Blanc (Microsoft) [SafeInt](https://github.com/dcleblanc/SafeInt)
    - Used by MSFT teams, for Office, Windows etc.
    - This is decades old, but still has only 8 forks on the GitHub?
    - One can safely assume, there has to be internal C version for Windows code itself.

Above are two probably most mature C++ lib's on this same subject.

They primarily exist to asure safe arithemtics on "numbers", not to provide type safe handler like NothingBut<T> does.
None of them is built to stop accidental implicit conversion in C++.

### Contact
Please report issues or [questions here](https://github.com/dbj-systems/dbj-laboratorium/issues).
 
Best way to contact me is the "Issues" tab.
### Contributing
Any feedback from users and stakeholders will be used to improve the library.
### License
#### https://dbj.org/license_dbj
#### &copy; 2019-2021 by dbj@dbj.org
---------
[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  
