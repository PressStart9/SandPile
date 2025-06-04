#pragma once

#include <cstdint>
#include <utility>

template<typename Elem>
class GrowingDeque {
 public:
  using value_type = Elem;
  using reference = Elem&;
  using const_reference = const Elem&;
  using pointer = Elem*;
  using const_pointer = const Elem*;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  GrowingDeque() = default;
  GrowingDeque(size_t size) : capacity_(size), size_(size), bytes_(new value_type[size]) {}
  GrowingDeque(const GrowingDeque& other) : capacity_(other.capacity_), size_(other.size_), bytes_(new value_type[other.size_]) {
    for (size_t i = 0; i < size_; ++i) {
      bytes_[i] = other.bytes_[i];
    }
  }
  GrowingDeque(GrowingDeque&& other) noexcept : capacity_(std::exchange(other.capacity_, 0)), size_(std::exchange(other.size_, 0)), bytes_(std::exchange(other.bytes_, nullptr)) {}

  GrowingDeque& operator=(const GrowingDeque& other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    bytes_ = new value_type[other.size_];
    for (size_t i = 0; i < size_; ++i) {
      bytes_[i] = other.bytes_[i];
    }

    return *this;
  }
  GrowingDeque& operator=(GrowingDeque&& other) noexcept {
    capacity_ = std::exchange(other.capacity_, 0);
    size_ = std::exchange(other.size_, 0);
    bytes_ = std::exchange(other.bytes_, nullptr);

    return *this;
  }

  ~GrowingDeque() {
    delete[] bytes_;
  }

  size_t size() const { return size_; }

  reference operator[](size_t i) {
    return bytes_[i < shift_ ? capacity_ - shift_ + i : i - shift_];
  }

  const_reference operator[](size_t i) const {
    return bytes_[i < shift_ ? capacity_ - shift_ + i : i - shift_];
  }

  void push_back(const value_type& elem) {
    if (size_ == capacity_) {
        realloc();
    }

    bytes_[size_ - shift_] = elem;
    ++size_;
  }

  void push_back(value_type&& elem) {
    if (size_ == capacity_) {
        realloc();
    }

    bytes_[size_ - shift_] = std::move(elem);
    ++size_;
  }

  void push_front(const value_type& elem) {
    if (size_ == capacity_) {
        realloc();
    }

    ++shift_;
    bytes_[capacity_ - shift_] = elem;
    ++size_;
  }

  void push_front(value_type&& elem) {
    if (size_ == capacity_) {
        realloc();
    }

    ++shift_;
    bytes_[capacity_ - shift_] = std::move(elem);
    ++size_;
  }
  
 private:
  value_type* bytes_ = nullptr;
  size_t capacity_ = 0;

  size_t size_ = 0;
  size_t shift_ = 0;

  static const size_t DefaultDequeSize_ = 16;
  static const size_t ReallocFactor_ = 2;

  void realloc() {
    size_t old_capacity = capacity_;
    capacity_ = capacity_ == 0 ? DefaultDequeSize_ : capacity_ * ReallocFactor_;
    value_type* new_bytes = new value_type[capacity_];

    for (size_t i = 0; i < shift_; ++i) {
        new_bytes[i] = std::move(bytes_[old_capacity - shift_ + i]);
    }

    for (size_t i = 0; i < size_ - shift_; ++i) {
        new(&new_bytes[shift_ + i]) value_type(std::move(bytes_[i]));
        bytes_[i].~value_type();
    }

    shift_ = 0;

    delete[] bytes_;
    bytes_ = new_bytes;
  }
};
