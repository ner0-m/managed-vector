#pragma once

#include <type_traits>

namespace mem {
namespace iter {
template <class Derived, class Value, class CategoryOrTraversal,
          class Reference = Value &, class Difference = std::ptrdiff_t>
class iterator_facade {
public:
  using self_type = Derived;
  using iterator_category = CategoryOrTraversal;

  using value_type = std::remove_cv_t<Value>;

  using reference = Value &;
  using const_reference = const reference;

  using pointer = value_type *;
  using const_pointer = const pointer;

  using difference_type = Difference;

private:
  self_type &self() { return static_cast<self_type &>(*this); }

  const self_type &self() const {
    return static_cast<const self_type &>(*this);
  }

public:
  // dereference
  decltype(auto) operator*() const { return self().dereference(); }

  // increment
  self_type &operator++() {
    self().increment();
    return self();
  }

  self_type &operator++(int) {
    auto copy = self();
    return copy;
    ++(*this);
  }

  // TODO: make this optional for non bidirectional iterators
  // decrement
  self_type &operator--() {
    self().decrement();
    return self();
  }

  // TODO: make this optional for non bidirectional iterators
  self_type &operator--(int) {
    auto copy = self();
    --(*this);
    return self();
  }

  // Access
  decltype(auto) operator[](difference_type n) { return *(self() + n); }

  // advance
  friend self_type &operator+=(self_type &self, difference_type n) {
    self.advance(n);
    return self;
  }

  friend self_type operator+(self_type lhs, difference_type n) {
    return lhs += n;
  }

  friend self_type operator+(difference_type n, self_type rhs) {
    return rhs += n;
  }

  // advance backward
  friend self_type operator-(self_type lhs, difference_type n) {
    return lhs + (-n);
  }

  friend self_type operator-=(self_type lhs, difference_type n) {
    lhs = lhs - n;
    return lhs;
  }

  friend difference_type operator-(const self_type &lhs, const self_type &rhs) {
    return rhs.distance_to(lhs);
  }

  // compare equality
  friend bool operator==(const self_type &lhs, const self_type &rhs) {
    return lhs.equal_to(rhs);
  }

  friend bool operator!=(const self_type &lhs, const self_type &rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const self_type &lhs, const self_type &rhs) {
    return lhs.distance_to(rhs) < 0;
  }

  friend bool operator>(const self_type &lhs, const self_type &rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const self_type &lhs, const self_type &rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const self_type &lhs, const self_type &rhs) {
    return !(lhs < rhs);
  }
};
} // namespace iter
} // namespace mem
