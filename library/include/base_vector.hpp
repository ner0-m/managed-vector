#pragma once

#include <cstddef>
#include <iterator>
#include <vector>

#include "contiguous_storage.hpp"

namespace mem {
template <class T, class Alloc> class base_vector {
private:
  using storage_type = typename detail::contiguous_storage<T, Alloc>;

public:
  using value_type = typename storage_type::value_type;

  using reference = typename storage_type::reference;
  using const_reference = typename storage_type::const_reference;

  using pointer = typename storage_type::pointer;
  using const_pointer = typename storage_type::const_pointer;

  using size_type = typename storage_type::size_type;

  using difference_size = typename storage_type::difference_type;

  using allocator_type = typename storage_type::allocator_type;

  using iterator = typename storage_type::iterator;
  using const_iterator = typename storage_type::const_iterator;

protected:
  storage_type storage_;

  size_type size_;

public:
  /// create empty vector
  base_vector();

  /// create empty vector with given allocator
  explicit base_vector(const Alloc &alloc);

  /// create vector of size n, with default initialized values
  explicit base_vector(size_type n);

  /// create vector of size n, with default initialized values
  explicit base_vector(size_type n, const Alloc &alloc);

  /// create vector of size n, with copies of value
  explicit base_vector(size_type n, const value_type &value);

  /// create vector of size n, with copies of value
  explicit base_vector(size_type n, const value_type &value,
                       const Alloc &alloc);

  /// Copy constructor copies from a exemplar base_vector
  base_vector(const base_vector &v);

  /// Copy constructor copies from a exemplar base_vector
  base_vector(const base_vector &v, const Alloc &alloc);

  /// Move constructor copies from a exemplar base_vector
  base_vector(base_vector &&v);

  /// Copy assignment copies from a exemplar base_vector
  base_vector &operator=(const base_vector &v);

  /// Move assignment copies from a exemplar base_vector
  base_vector &operator=(base_vector &&v);

  /// Copy constructor from exemplar base_vector with different type
  template <class OtherT, class OtherAlloc>
  base_vector(const base_vector<OtherT, OtherAlloc> &v);

  /// Copy assignment from exemplar base_vector with different type
  template <class OtherT, class OtherAlloc>
  base_vector &operator=(const base_vector<OtherT, OtherAlloc> &v);

  /// Copy constructor from given std::vector with different type
  template <class OtherT, class OtherAlloc>
  base_vector(const std::vector<OtherT, OtherAlloc> &v);

  /// Copy assignment from given std::vector with different type
  template <class OtherT, class OtherAlloc>
  base_vector &operator=(const std::vector<OtherT, OtherAlloc> &v);

  /// Construct base_vector from iterator range
  template <typename InputIter> base_vector(InputIter first, InputIter last);

  /// Construct base_vector from iterator range
  template <typename InputIter>
  base_vector(InputIter first, InputIter last, const Alloc &alloc);

  /// Destructor
  ~base_vector();

  /// Shrink or grow current size of the buffer
  void resize(size_type new_size);

  /// Shrink or grow current size of the buffer, new elements will have the
  /// given value
  void resize(size_type new_size, const value_type &value);

  /// return the current number of elements
  size_type size() const;

  /// Maximum possible value of base_vector
  size_type max_size() const;

  /// request for larger buffer, if it's smaller than current capacity, this
  /// function has no effect
  void reserve(size_type new_capacity);

  /// Current capacity (i.e. allocated memory buffer)
  size_type capacity() const;

  /// subscript operator to access a mutable reference
  reference operator[](size_type idx);

  /// subscript operator to access a immutable reference
  const_reference operator[](size_type idx) const;

  iterator begin();

  const_iterator begin() const;

  const_iterator cbegin() const;

  iterator end();

  const_iterator end() const;

  const_iterator cend() const;

  pointer data();

  const_pointer data() const;

  /// returns true iff size() == 0
  bool empty() const;

  /// Swap elements of this vector with given vector
  void swap(base_vector &v);

  /// TODO: What does this do exactly?
  void assign(size_type n, const value_type &value);

  /// Make base_vector a copy of given range
  template <class InputIter> void assign(InputIter first, InputIter last);

  /// TODO: Doc
  allocator_type get_allocator() const;

  iterator erase(iterator pos);

  iterator erase(iterator first, iterator last);

private:
  void default_init(size_type n);

  void fill_init(size_type n, const value_type &value);

  template <typename InputIterator>
  void range_init(InputIterator first, InputIterator last);

  template <typename InputIterator>
  void range_init(InputIterator first, InputIterator last,
                  std::forward_iterator_tag);

  template <typename ForwardIterator>
  void range_init(ForwardIterator first, ForwardIterator last,
                  std::random_access_iterator_tag);

  template <typename IteratorOrIntegralType>
  void init_dispatch(IteratorOrIntegralType begin, IteratorOrIntegralType end,
                     std::false_type);

  template <typename IteratorOrIntegralType>
  void init_dispatch(IteratorOrIntegralType n, IteratorOrIntegralType value,
                     std::true_type);

  void fill_assign(size_type n, const T &x);

  template <typename InputIterator>
  void assign_dispatch(InputIterator first, InputIterator last,
                       std::false_type);

  template <typename Integral>
  void assign_dispatch(Integral n, Integral x, std::true_type);
};

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector() : storage_(), size_(0) {}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(const Alloc &alloc)
    : storage_(alloc), size_(0) {}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(size_type n) : storage_(), size_(0) {
  default_init(n);
}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(size_type n, const Alloc &alloc)
    : storage_(alloc), size_(0) {
  default_init(n);
}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(size_type n, const value_type &value)
    : storage_(), size_(0) {
  fill_init(n, value);
}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(size_type n, const value_type &value,
                                   const Alloc &alloc)
    : storage_(alloc), size_(0) {
  fill_init(n, value);
}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(const base_vector &v)
    : storage_(detail::copy_allocator_t{}, v.storage_), size_(0) {
  range_init(v.cbegin(), v.cend());
}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(const base_vector &v, const Alloc &alloc)
    : storage_(alloc), size_(0) {
  range_init(v.cbegin(), v.cend());
}

template <class T, class Alloc>
base_vector<T, Alloc>::base_vector(base_vector &&v)
    : storage_(detail::copy_allocator_t{}, v.storage_), size_(0) {
  *this = std::move(v);
}

template <class T, class Alloc>
base_vector<T, Alloc> &base_vector<T, Alloc>::operator=(const base_vector &v) {
  if (this != &v) {
    storage_.destroy_on_allocator_mismatch(v.storage_, begin(), end());
    storage_.deallocate_on_allocator_mismatch(v.storage_);

    storage_.propagate_allocator(v.storage_);

    assign(v.begin(), v.end());
  } // end if

  return *this;
}

template <class T, class Alloc>
base_vector<T, Alloc> &base_vector<T, Alloc>::operator=(base_vector &&v) {
  storage_.destroy(begin(), end());
  storage_ = std::move(v.storage_);
  size_ = std::move(v.size_);

  v.storage_ = detail::contiguous_storage<T, Alloc>(detail::copy_allocator_t(),
                                                    storage_);
  v.size_ = 0;

  return *this;
}

template <class T, class Alloc>
template <class OtherT, class OtherAlloc>
base_vector<T, Alloc>::base_vector(const base_vector<OtherT, OtherAlloc> &v) {
  range_init(v.cbegin(), v.cend());
}

template <class T, class Alloc>
template <class OtherT, class OtherAlloc>
base_vector<T, Alloc> &
base_vector<T, Alloc>::operator=(const base_vector<OtherT, OtherAlloc> &v) {
  assign(v.begin(), v.end());
  return *this;
}

template <class T, class Alloc>
template <class OtherT, class OtherAlloc>
base_vector<T, Alloc>::base_vector(const std::vector<OtherT, OtherAlloc> &v)
    : storage_(), size_(0) {
  range_fill(v.begin(), v.end());
}

template <class T, class Alloc>
template <class OtherT, class OtherAlloc>
base_vector<T, Alloc> &
base_vector<T, Alloc>::operator=(const std::vector<OtherT, OtherAlloc> &v) {
  assign(v.begin(), v.end());
  return *this;
}

template <class T, class Alloc>
template <typename InputIter>
base_vector<T, Alloc>::base_vector(InputIter first, InputIter last)
    : storage_(), size_(0) {
  // check the type of InputIterator: if it's an integral type,
  // we need to interpret this call as (size_type, value_type)
  constexpr bool IsInteger = std::is_integral_v<InputIter>;

  init_dispatch(first, last, IsInteger);
}

template <class T, class Alloc>
template <typename InputIter>
base_vector<T, Alloc>::base_vector(InputIter first, InputIter last,
                                   const Alloc &alloc)
    : storage_(alloc), size_(0) {
  // check the type of InputIterator: if it's an integral type,
  // we need to interpret this call as (size_type, value_type)
  constexpr bool IsInteger = std::is_integral_v<InputIter>;

  init_dispatch(first, last, IsInteger);
}

template <class T, class Alloc> base_vector<T, Alloc>::~base_vector() {
  if (!empty()) {
    storage_.destroy(begin(), end());
  }
}

template <class T, class Alloc>
void base_vector<T, Alloc>::resize(size_type new_size) {}

template <class T, class Alloc>
void base_vector<T, Alloc>::resize(size_type new_size,
                                   const value_type &value) {}

template <class T, class Alloc>
typename base_vector<T, Alloc>::size_type base_vector<T, Alloc>::size() const {
  return size_;
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::size_type
base_vector<T, Alloc>::max_size() const {
  return storage_.max_size();
}

template <class T, class Alloc>
void base_vector<T, Alloc>::reserve(size_type new_capacity) {}

template <class T, class Alloc>
typename base_vector<T, Alloc>::size_type
base_vector<T, Alloc>::capacity() const {
  return storage_.size();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::reference
base_vector<T, Alloc>::operator[](size_type idx) {
  return storage_[idx];
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::const_reference
base_vector<T, Alloc>::operator[](size_type idx) const {
  return storage_[idx];
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::iterator base_vector<T, Alloc>::begin() {
  return storage_.begin();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::const_iterator
base_vector<T, Alloc>::begin() const {
  return storage_.begin();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::const_iterator
base_vector<T, Alloc>::cbegin() const {
  return storage_.cbegin();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::iterator base_vector<T, Alloc>::end() {
  return storage_.end();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::const_iterator
base_vector<T, Alloc>::end() const {
  return storage_.end();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::const_iterator
base_vector<T, Alloc>::cend() const {
  return storage_.cend();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::pointer base_vector<T, Alloc>::data() {
  return storage_.data();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::const_pointer
base_vector<T, Alloc>::data() const {
  return storage_.data();
}

template <class T, class Alloc> bool base_vector<T, Alloc>::empty() const {
  return size() == 0;
}

template <class T, class Alloc>
void base_vector<T, Alloc>::swap(base_vector &v) {
  using std::swap;
  swap(storage_, v.storage_);
  swap(size_, v.size_);
}

template <class T, class Alloc>
void base_vector<T, Alloc>::assign(size_type n, const value_type &value) {
  fill_assign(n, value);
}

template <class T, class Alloc>
template <class InputIter>
void base_vector<T, Alloc>::assign(InputIter first, InputIter last) {
  // check the type of InputIterator: if it's an integral type,
  // we need to interpret this call as (size_type, value_type)
  constexpr bool IsInteger = std::is_integral_v<InputIter>;

  assign_dispatch(first, last, IsInteger);
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::allocator_type
base_vector<T, Alloc>::get_allocator() const {
  return storage_.get_allocator();
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::iterator
base_vector<T, Alloc>::erase(iterator pos) {
  auto end = pos;
  ++end;
  return erase(pos, end);
}

template <class T, class Alloc>
typename base_vector<T, Alloc>::iterator
base_vector<T, Alloc>::erase(iterator first, iterator last) {
  iterator i = overlapped_copy(last, end(), first);
  storage_.destroy(i, end());
  size_ -= (last - first);
  return first;
}

template <class T, class Alloc>
template <typename IteratorOrIntegralType>
void base_vector<T, Alloc>::init_dispatch(IteratorOrIntegralType n,
                                          IteratorOrIntegralType value,
                                          std::true_type) {
  fill_init(n, value);
}

template <class T, class Alloc>
template <typename IteratorOrIntegralType>
void base_vector<T, Alloc>::init_dispatch(IteratorOrIntegralType begin,
                                          IteratorOrIntegralType end,
                                          std::false_type) {
  range_init(begin, end);
}

template <class T, class Alloc>
void base_vector<T, Alloc>::fill_assign(size_type n, const T &x) {
  if (n > capacity()) {
    // If the wanted size is larger than our current capacity: allocate a new
    // vector
    auto tmp = base_vector<T, Alloc>(n, x, get_allocator());
    tmp.swap(*this);
  } else if (n > size()) {
    // We have enough allocated space
    std::fill(begin(), end(), x);
    storage_.uninitialized_fill_n(end(), n - size(), x);
    size_ += n - size();
  } else {
    // size() > n, so fill till n, and delete the rest
    iterator new_end = std::fill_n(begin(), n, x);
    erase(new_end, end());
  }
}

template <class T, class Alloc>
template <typename InputIterator>
void base_vector<T, Alloc>::assign_dispatch(InputIterator first,
                                            InputIterator last,
                                            std::false_type) {
  range_assign(first, last);
}

template <class T, class Alloc>
template <typename Integral>
void base_vector<T, Alloc>::assign_dispatch(Integral n, Integral x,
                                            std::true_type) {
  fill_assign(n, x);
}

} // namespace mem
