#include <doctest/doctest.h>

#include "contiguous_storage.hpp"

TEST_CASE_TEMPLATE("contiguous_storage: default construction", T, int, float) {
  using namespace mem::detail;

  contiguous_storage<T, std::allocator<T>> storage;

  CHECK_EQ(storage.size(), 0);
}
