/*
  Type Handle to Avoid Implicit Conversions in standard C++

  Copyright 2019/2021 Dušan B. Jovanović (dbj@dbj.org)

  Please see the LICENSE file
*/
#include "tests.h"

int main(void)
{
	test_comparators();
	test_vector_walk();
	test_different_types();
	test_try_to_trick();
	test_assignments();
	test_compatibility();
	return 42;
}
