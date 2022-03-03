#include <doctest/doctest.h>

#include "normal_iterator.hpp"

#include <algorithm>
#include <array>

// Needs to be after #include <doctest/doctest>
#include "stringmaker.h"

TEST_CASE("normal iterator forward") {
  std::array a = {1, 2, 3, 4, 5};
  auto begin = mem::iter::normal_iterator<int>(a.data());
  auto end = mem::iter::normal_iterator<int>(a.data() + a.size());

  CHECK_EQ(begin - end, 5);

  CHECK_LT(a.begin(), a.end());
  CHECK_GT(a.end(), a.begin());
  CHECK_LT(begin, end);
  CHECK_GT(end, begin);

  auto counter = 0;
  for (; begin != end; ++begin) {
    CHECK_EQ(++counter, *begin);
  }
  CHECK_EQ(begin, end);
}

TEST_CASE("normal iterator backward") {
  std::array a = {1, 2, 3, 4, 5};
  auto begin = mem::iter::normal_iterator<int>(a.data());
  auto end = mem::iter::normal_iterator<int>(a.data() + a.size() - 1);

  CHECK_EQ(begin - end, 4);

  auto counter = 5;
  for (; begin != end; --end) {
    CHECK_EQ(counter--, *end);
  }
  CHECK_EQ(begin, end);
}
