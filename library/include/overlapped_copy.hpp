#pragma once

#include "fmt/core.h"
#include <algorithm>

namespace mem {

/**
 * @brief Copy range [first, last) to the range [result, result + (last -
 * first)), if the range is overlapping, i.e. result lies in [first, last), this
 * will still yield expected results
 *
 * @param first start of source range
 * @param last end of source range
 * @param result start of result range
 */
template <typename RandomAccessIterator1, typename RandomAccessIterator2>
RandomAccessIterator2 overlapped_copy(RandomAccessIterator1 first,
                                      RandomAccessIterator1 last,
                                      RandomAccessIterator2 result) {
  if (first < last && first <= result && result < last) {
    // result lies in [first, last)
    // it's safe to use std::copy_backward here
    std::copy_backward(first, last, result + (last - first));
    result += (last - first);
  } // end if
  else {
    // result + (last - first) lies in [first, last)
    // it's safe to use sequential_copy here
    result = std::copy(first, last, result);
  } // end else

  return result;
}

} // namespace mem
