#pragma once

#include "normal_iterator.hpp"
#include <algorithm>
#include <exception>
#include <memory>

namespace mem {
namespace detail {

class allocator_mismatch_on_swap : public std::runtime_error {
public:
  allocator_mismatch_on_swap()
      : std::runtime_error("swap called on containers with allocators that "
                           "propagate on swap, but compare non-equal") {}
};

struct copy_allocator_t {};

template <class T, class Alloc> class contiguous_storage {
private:
  using alloc_traits = std::allocator_traits<Alloc>;

public:
  using allocator_type = Alloc;

  using value_type = T;

  using reference = value_type&;
  using const_reference = const reference;

  using pointer = typename alloc_traits::pointer;
  using const_pointer = typename alloc_traits::const_pointer;

  using size_type = typename alloc_traits::size_type;

  using difference_type = typename alloc_traits::difference_type;

  using iterator = typename iter::normal_iterator<value_type>;
  using const_iterator = typename iter::normal_iterator<value_type>;

private:
  allocator_type allocator_;

  iterator begin_;

  size_type size_;

public:
  explicit contiguous_storage(const allocator_type &alloc = allocator_type{});

  explicit contiguous_storage(size_type n,
                              const allocator_type &alloc = allocator_type{});

  explicit contiguous_storage(copy_allocator_t,
                              const contiguous_storage &alloc);

  explicit contiguous_storage(copy_allocator_t, const contiguous_storage &alloc,
                              size_type n);

  ~contiguous_storage();

  /// return the current number of elements
  size_type size() const;

  /// Maximum possible value of base_vector
  size_type max_size() const;

  pointer data();

  const_pointer data() const;

  iterator begin();

  const_iterator begin() const;

  const_iterator cbegin() const;

  iterator end();

  const_iterator end() const;

  const_iterator cend() const;

  reference operator[](size_type idx);

  const_reference operator[](size_type idx) const;

  allocator_type get_allocator() const;

  void allocate(size_type n);

  void deallocate();

  void swap(contiguous_storage &x);

  void default_construct_n(iterator first, size_type n);

  void uninitialized_fill_n(iterator first, size_type n,
                            const value_type &value);

  template <typename InputIterator>
  iterator uninitialized_copy(InputIterator first, InputIterator last,
                              iterator result);

  template <typename InputIterator, typename Size>
  iterator uninitialized_copy_n(InputIterator first, Size n, iterator result);

  void destroy(iterator first, iterator last);

  void deallocate_on_allocator_mismatch(const contiguous_storage &other);

  void destroy_on_allocator_mismatch(const contiguous_storage &other,
                                     iterator first, iterator last);

  void propagate_allocator(const contiguous_storage &other);

  void propagate_allocator(contiguous_storage &other);

  bool is_allocator_not_equal(const allocator_type &alloc) const;

  bool is_allocator_not_equal(const contiguous_storage &other) const;

  contiguous_storage &operator=(contiguous_storage &&other);

  // disallow assignment
  contiguous_storage &operator=(const contiguous_storage &x) = delete;

private:
  void swap_allocators(std::true_type, const allocator_type &);

  void swap_allocators(std::false_type, allocator_type &);

  bool is_allocator_not_equal_dispatch(std::true_type,
                                       const allocator_type &) const;

  bool is_allocator_not_equal_dispatch(std::false_type,
                                       const allocator_type &alloc) const;

  void
  deallocate_on_allocator_mismatch_dispatch(std::true_type,
                                            const contiguous_storage &other);

  void
  deallocate_on_allocator_mismatch_dispatch(std::false_type,
                                            const contiguous_storage &other);

  void destroy_on_allocator_mismatch_dispatch(std::true_type,
                                              const contiguous_storage &other,
                                              iterator first, iterator last);

  void destroy_on_allocator_mismatch_dispatch(std::false_type,
                                              const contiguous_storage &other,
                                              iterator first, iterator last);

  void propagate_allocator_dispatch(std::true_type,
                                    const contiguous_storage &other);

  void propagate_allocator_dispatch(std::false_type,
                                    const contiguous_storage &other);

  void propagate_allocator_dispatch(std::true_type, contiguous_storage &other);

  void propagate_allocator_dispatch(std::false_type, contiguous_storage &other);
};

template <class T, class Alloc>
contiguous_storage<T, Alloc>::contiguous_storage(const allocator_type &alloc)
    : allocator_(alloc), begin_(pointer(static_cast<T *>(0))), size_(0) {}

template <class T, class Alloc>
contiguous_storage<T, Alloc>::contiguous_storage(size_type n,
                                                 const allocator_type &alloc)
    : allocator_(alloc), begin_(pointer(static_cast<T *>(0))), size_(0) {
  allocate(n);
}

template <class T, class Alloc>
contiguous_storage<T, Alloc>::contiguous_storage(
    copy_allocator_t, const contiguous_storage &other)
    : allocator_(other.allocator_), begin_(pointer(static_cast<T *>(0))),
      size_(0) {}

template <class T, class Alloc>
contiguous_storage<T, Alloc>::contiguous_storage(
    copy_allocator_t, const contiguous_storage &other, size_type n)
    : allocator_(other.allocator_), begin_(pointer(static_cast<T *>(0))),
      size_(0) {
  allocate(n);
}

template <class T, class Alloc>
contiguous_storage<T, Alloc>::~contiguous_storage() {
  deallocate();
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::size_type
contiguous_storage<T, Alloc>::size() const {
  return size_;
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::size_type
contiguous_storage<T, Alloc>::max_size() const {
  return alloc_traits::max_size(allocator_);
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::pointer
contiguous_storage<T, Alloc>::data() {
  return &(*begin_);
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::const_pointer
contiguous_storage<T, Alloc>::data() const {
  return &(*begin_);
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::iterator
contiguous_storage<T, Alloc>::begin() {
  return begin_;
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::const_iterator
contiguous_storage<T, Alloc>::begin() const {
  return begin_;
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::const_iterator
contiguous_storage<T, Alloc>::cbegin() const {
  return begin_;
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::iterator
contiguous_storage<T, Alloc>::end() {
  return begin_ + size();
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::const_iterator
contiguous_storage<T, Alloc>::end() const {
  return begin_ + size();
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::const_iterator
contiguous_storage<T, Alloc>::cend() const {
  return begin_ + size();
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::reference
contiguous_storage<T, Alloc>::operator[](size_type idx) {
  return begin_[idx];
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::const_reference
contiguous_storage<T, Alloc>::operator[](size_type idx) const {
  return begin_[idx];
}

template <class T, class Alloc>
typename contiguous_storage<T, Alloc>::allocator_type
contiguous_storage<T, Alloc>::get_allocator() const {
  return allocator_;
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::allocate(size_type n) {
  if (n > 0) {
    begin_ = iterator(alloc_traits::allocate(allocator_, n));
    size_ = n;
  } else {
    begin_ = pointer(static_cast<T *>(0));
    size_ = 0;
  }
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::deallocate() {
  if (size() > 0) {
    alloc_traits::deallocate(allocator_, begin_.base(), size());
    begin_ = pointer(static_cast<T *>(0));
    size_ = 0;
  }
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::swap(contiguous_storage &x) {
  using std::swap;
  swap(begin_, x.begin_);
  swap(size_, x.size_);

  swap_allocators(
      std::integral_constant<
          bool,
          std::allocator_traits<Alloc>::propagate_on_container_swap::value>(),
      x.m_allocator);

  swap(allocator_, x.allocator_);
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::default_construct_n(
    contiguous_storage<T, Alloc>::iterator first,
    contiguous_storage<T, Alloc>::size_type n) {
  std::uninitialized_default_construct_n(first.base(), n);
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::uninitialized_fill_n(
    contiguous_storage<T, Alloc>::iterator first,
    contiguous_storage<T, Alloc>::size_type n,
    const contiguous_storage<T, Alloc>::value_type &value) {
  std::uninitialized_fill_n(first.base(), n, value);
}

template <class T, class Alloc>
template <typename InputIterator>
typename contiguous_storage<T, Alloc>::iterator
contiguous_storage<T, Alloc>::uninitialized_copy(
    InputIterator first, InputIterator last,
    contiguous_storage<T, Alloc>::iterator result) {
  return iterator(std::uninitialized_copy(first, last, result.base()));
}

template <class T, class Alloc>
template <typename InputIterator, typename Size>
typename contiguous_storage<T, Alloc>::iterator
contiguous_storage<T, Alloc>::uninitialized_copy_n(
    InputIterator first, Size n,
    contiguous_storage<T, Alloc>::iterator result) {
  return iterator(std::uninitialized_copy_n(first, n, result.base()));
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::destroy(
    contiguous_storage<T, Alloc>::iterator first,
    contiguous_storage<T, Alloc>::iterator last) {
  std::destroy(first.base(), last.base());
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::deallocate_on_allocator_mismatch(
    const contiguous_storage &other) {
  std::integral_constant<
      bool, std::allocator_traits<
                Alloc>::propagate_on_container_copy_assignment::value>
      c;
  deallocate_on_allocator_mismatch_dispatch(c, other);
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::destroy_on_allocator_mismatch(
    const contiguous_storage &other,
    contiguous_storage<T, Alloc>::iterator first,
    contiguous_storage<T, Alloc>::iterator last) {

  std::integral_constant<
      bool, std::allocator_traits<
                Alloc>::propagate_on_container_copy_assignment::value>
      c;
  destroy_on_allocator_mismatch_dispatch(c, other, first, last);
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::propagate_allocator(
    const contiguous_storage &other) {

  std::integral_constant<
      bool, std::allocator_traits<
                Alloc>::propagate_on_container_copy_assignment::value>
      c;

  propagate_allocator_dispatch(c, other);
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::propagate_allocator(
    contiguous_storage &other) {
  std::integral_constant<
      bool, std::allocator_traits<
                Alloc>::propagate_on_container_move_assignment::value>
      c;

  propagate_allocator_dispatch(c, other);
}

template <class T, class Alloc>
contiguous_storage<T, Alloc> &
contiguous_storage<T, Alloc>::operator=(contiguous_storage &&other) {
  if (size() > 0) {
    deallocate();
  }
  propagate_allocator(other);
  begin_ = std::move(other.m_begin);
  size_ = std::move(other.m_size);

  other.begin_ = pointer(static_cast<T *>(0));
  other.size_ = 0;

  return *this;
}

template <class T, class Alloc>
bool contiguous_storage<T, Alloc>::is_allocator_not_equal(
    const allocator_type &alloc) const {
  return is_allocator_not_equal_dispatch(
      std::integral_constant<
          bool, std::allocator_traits<Alloc>::is_always_equal::value>(),
      alloc);
}

template <class T, class Alloc>
bool contiguous_storage<T, Alloc>::is_allocator_not_equal(
    const contiguous_storage &other) const {
  return is_allocator_not_equal(allocator_, other.allocator_);
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::swap_allocators(std::true_type,
                                                   const allocator_type &) {}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::swap_allocators(std::false_type,
                                                   allocator_type &other) {
  using std::swap;
  if (is_allocator_not_equal(other)) {
    throw allocator_mismatch_on_swap();
  }

  swap(allocator_, other);
}

template <class T, class Alloc>
bool contiguous_storage<T, Alloc>::is_allocator_not_equal_dispatch(
    std::true_type, const allocator_type &) const {
  return false;
}

template <class T, class Alloc>
bool contiguous_storage<T, Alloc>::is_allocator_not_equal_dispatch(
    std::false_type, const allocator_type &other) const {
  return allocator_ != other;
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::deallocate_on_allocator_mismatch_dispatch(
    std::true_type, const contiguous_storage &other) {
  if (allocator_ != other.allocator_) {
    deallocate();
  }
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::deallocate_on_allocator_mismatch_dispatch(
    std::false_type, const contiguous_storage &other) {}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::destroy_on_allocator_mismatch_dispatch(
    std::true_type, const contiguous_storage &other, iterator first,
    iterator last) {
  if (allocator_ != other.allocator_) {
    destroy(first, last);
  }
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::destroy_on_allocator_mismatch_dispatch(
    std::false_type, const contiguous_storage &other, iterator first,
    iterator last) {}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::propagate_allocator_dispatch(
    std::true_type, const contiguous_storage &other) {
  allocator_ = other.allocator_;
}

template <class T, class Alloc>
void contiguous_storage<T, Alloc>::propagate_allocator_dispatch(
    std::false_type, const contiguous_storage &other) {}

} // namespace detail
} // namespace mem
