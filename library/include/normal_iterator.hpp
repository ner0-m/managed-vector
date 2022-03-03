#pragma once

#include "iterator_facade.hpp"

#include <iterator>

namespace mem {
namespace iter {

template <class T>
struct normal_iterator
    : iterator_facade<normal_iterator<T>, T, std::random_access_iterator_tag> {
private:
  using super_t =
      iterator_facade<normal_iterator<T>, T, std::random_access_iterator_tag>;

public:
  using value_type = typename super_t::value_type;
  using iterator_category = typename super_t::iterator_category;

  using reference = typename super_t::reference;
  using const_reference = typename super_t::const_reference;

  using pointer = typename super_t::pointer;
  using const_pointer = typename super_t::const_pointer;

  using difference_type = typename super_t::difference_type;

private:
  pointer cur_;

public:
  normal_iterator(pointer cur) : cur_(cur) {}

  pointer base() { return cur_; }

  const_pointer base() const { return cur_; }

  const_reference dereference() const { return *cur_; }

  reference dereference() { return *cur_; }

  void increment() { ++cur_; }

  void decrement() { --cur_; }

  void advance(difference_type n) { cur_ += n; }

  difference_type distance_to(const normal_iterator &other) const {
    return cur_ - other.cur_;
  }

  bool equal_to(const normal_iterator &other) const {
    return cur_ == other.cur_;
  }
};
} // namespace iter
} // namespace mem

template <typename T>
struct std::iterator_traits<::mem::iter::normal_iterator<T>> {
  using self = ::mem::iter::normal_iterator<T>;
  using pointer = typename self::pointer;
  using iterator_category = typename self::iterator_category;
  using value_type = typename self::value_type;
  using difference_type = typename self::difference_type;
  using reference = typename self::reference;
};
