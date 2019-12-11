/*
  Type Handle to Avoid Implicit Conversions in standard C++

  Copyright 2019/2020 Dušan B. Jovanović (dbj@dbj.org)

  Please see the LICENSE file
*/
#include <exception>
#include <cstdio>


	void test_comparators();
	void test_vector_walk();
	void test_different_types();
	void test_try_to_trick();
	void test_assignments();
	void test_compatibility();

int main()
{
	using namespace std;
	try {
		test_comparators();
		test_vector_walk();
		test_different_types();
		test_try_to_trick();
		test_assignments();
		test_compatibility();
	}
	catch (exception & x)
	{
		fprintf( stderr, "\n\nstd::exception\n\t%s\n", x.what());

	}
	catch ( ... )
	{
		fprintf(stderr, "\n\nunknown exception\n");
	}

	return 0;
}
