#include <doctest/doctest.h>
#include <iterator>
#include <sstream>

namespace doctest {
template <typename T, std::size_t N> struct StringMaker<std::array<T, N>> {
  static String convert(const std::array<T, N> &vec) {
    std::ostringstream oss;
    oss << "[ ";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(oss, " "));
    oss << "]";
    return oss.str().c_str();
  }
};
} // namespace doctest
