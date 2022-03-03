
#include <doctest/doctest.h>

#include "overlapped_copy.hpp"

#include <array>

// Needs to be after #include <doctest/doctest>
#include "stringmaker.h"

TEST_CASE("overlapped_copy: Having an actual overlapping copy") {
  std::array a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  mem::overlapped_copy(a.begin(), a.begin() + 5, a.begin() + 3);

  THEN("The range is copied correctly") {
    CHECK_EQ(a, std::array{1, 2, 3, 1, 2, 3, 4, 5, 9, 10});
  }
}

TEST_CASE("overlapped_copy: Having no overlapping copy") {
  std::array a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  mem::overlapped_copy(a.begin(), a.begin() + 3, a.begin() + 5);

  THEN("overlapped_copy works like std::copy") {
    CHECK_EQ(a, std::array{1, 2, 3, 4, 5, 1, 2, 3, 9, 10});
  }
}

TEST_CASE("overlapped_copy: try to copy an empty range") {
  std::array a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  mem::overlapped_copy(a.begin(), a.begin(), a.begin() + 5);

  THEN("No changes are done") {
    CHECK_EQ(a, std::array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  }
}

TEST_CASE("overlapped_copy: Copy whole range") {
  std::array a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  mem::overlapped_copy(a.begin(), a.end(), a.begin());

  THEN("The range is preserved") {
    CHECK_EQ(a, std::array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  }
}
