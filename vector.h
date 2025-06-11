//
// Created by arsen on 26.05.25.
//

#ifndef VECTOR_H
#define VECTOR_H
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <memory>
#include <utility>

template <class T>
class Vector {
public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() : data_(nullptr), size_(0), capacity_(0) {};
  explicit Vector(const SizeType size) : data_(size ? new ValueType[size] : nullptr), size_(size), capacity_(size) {};
  Vector(const SizeType size, ValueType value) : data_(nullptr), size_(size), capacity_(size) {
    if (size == 0) {
      return;
    }
    std::unique_ptr<ValueType[]> guard(new ValueType[size]);
    SizeType i = 0;
    try {
      for (; i < size; ++i) {
        guard[i] = value;
      }
    } catch(...) {
      throw;
    }
    data_ = guard.release();
  }
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    SizeType cnt = std::distance(first, last);
    std::unique_ptr<ValueType[]> guard(cnt ? new ValueType[cnt] : nullptr);
    SizeType i = 0;
    try {
      for (; first != last; ++first, ++i) {
        guard[i] = *first;
      }
    } catch(...) {
      throw;
    }
    data_ = guard.release();
    size_ = capacity_ = cnt;
  }
  Vector(std::initializer_list<ValueType> init) : data_(nullptr), size_(init.size()), capacity_(init.size())
  {
    if (size_ == 0) {
      return;
    }
    std::unique_ptr<ValueType[]> guard(new ValueType[size_]);
    try {
      SizeType i = 0;
      for (auto& elem : init) {
        guard[i++] = elem;
      }
    } catch (...) {
      throw;
    }
    data_ = guard.release();
  }

  Vector(const Vector& other) {
    const SizeType other_size = other.size_;
    const SizeType other_cap = other.capacity_;
    if (other_size > 0) {
      std::unique_ptr<ValueType[]> guard(new ValueType[other_cap]);
      for (SizeType i = 0; i < other_size; i++) {
        guard[i] = other.data_[i];
      }
      data_ = guard.release();
    } else {
      data_ = nullptr;
    }
    size_ = other_size;
    capacity_ = other_cap;
  }
  Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_),  capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  Vector& operator=(const Vector& other) {
    if (this == &other) {
      return *this;
    }
    Vector temp(other);
    std::swap(*this, temp);
    return *this;
  }
  Vector& operator=(Vector&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }
  ~Vector() {
    delete[] data_;
  }
  SizeType Size() const {
    return size_;
  }
  SizeType Capacity() const {
    return capacity_;
  }
  bool Empty() const {
    return size_ == 0;
  }
  Reference operator[](const SizeType index) {
    return data_[index];
  }
  ConstReference operator[](const SizeType index) const {
    return data_[index];
  }
  Reference At(SizeType index) {
    if (index >= size_) {
      throw std::out_of_range("At was out of range"); {}
    }
    return data_[index];
  }
  ConstReference At(SizeType index) const {
    if (index >= size_) {
      throw std::out_of_range("At was out of range"); {}
    }
    return data_[index];
  }
  Reference Front() {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty"); {}
    }
    return data_[0];
  }
  ConstReference Front() const {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty"); {}
    }
    return data_[0];
  }
  Reference Back() {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty"); {}
    }
    return data_[size_ - 1];
  }
  ConstReference Back() const {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty"); {}
    }
    return data_[size_ - 1];
  }
  Pointer Data() noexcept {
    return data_;
  }
  ConstPointer Data() const noexcept {
    return data_;
  }
  void Swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }
  void Resize(const SizeType new_size) {
    if (new_size <= size_) {
      size_ = new_size;
      return;
    }
    if (size_ < new_size && new_size <= capacity_) {
      for (SizeType i = size_; i < new_size; i++) {
        data_[i] = ValueType();
      }
      size_ = new_size;
      return;
    }
    std::unique_ptr<ValueType[]> guard(new ValueType[new_size]);
    SizeType i = 0;
    try {
      for (; i < size_; i++) {
        guard[i] = std::move(data_[i]);
      }
      for (; i < new_size; i++) {
        guard[i] = ValueType();
      }
    } catch (...) {
      throw;
    }
    delete[] data_;
    data_ = guard.release();
    size_ = new_size;
    capacity_ = new_size;
  }
  void Resize(const SizeType new_size, ConstReference value) {
    if (new_size <= size_) {
      size_ = new_size;
      return;
    }
    if (size_ < new_size && new_size <= capacity_) {
      for (SizeType i = size_; i < new_size; i++) {
        data_[i] = value;
      }
      size_ = new_size;
      return;
    }
    std::unique_ptr<ValueType[]> guard(new ValueType[new_size]);
    SizeType i = 0;
    try {
      for (; i < size_; i++) {
        guard[i] = std::move(data_[i]);
      }
      for (; i < new_size; i++) {
        guard[i] = value;
      }
    } catch (...) {
      throw;
    }
    delete[] data_;
    data_ = guard.release();
    size_ = new_size;
    capacity_ = new_size;
  }
  void Reserve(const SizeType new_cap) {
    if (new_cap <= capacity_) {
      return;
    }
    auto temp = new ValueType[std::max(new_cap, capacity_)];
    std::move(data_, data_ + size_, temp);
    delete[] data_;
    data_ = temp;
    capacity_ = new_cap;
  }
  void ShrinkToFit() {
    Pointer new_data = size_ ? new ValueType[size_] : nullptr;
    std::move(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
    capacity_ = size_;
  }
  void Clear() {
    size_ = 0;
  }
  void PushBack(ConstReference value) {
    if (size_ < capacity_) {
      data_[size_] = value;
      ++size_;
    } else {
      DoubleCap(value);
    }
  }
  void PushBack(ValueType&& value) {
    if (size_ < capacity_) {
      data_[size_] = std::move(value);
      ++size_;
    } else {
      DoubleCap(std::move(value));
    }
  }
  void PopBack() {
    if (size_ == 0) {
      throw std::out_of_range("Vector is empty"); {}
    }
    size_--;
  }
  bool operator==(const Vector& other) const {
    if (size_ != other.size_) {
      return false;
    }
    for (SizeType i = 0; i < size_; i++) {
      if (data_[i] != other.data_[i]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const Vector& other) const {
    return !(*this == other);
  }
  bool operator<(const Vector& other) const {
    return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
  }
  bool operator>(const Vector& other) const {
    return other < *this;
  }
  bool operator<=(const Vector& other) const {
    return !(other < *this);
  }
  bool operator>=(const Vector& other) const {
    return !(*this < other);
  }
  Iterator begin() noexcept { // NOLINT
    return data_;
  }
  Iterator end() noexcept { // NOLINT
    return data_ + size_;
  }
  ConstIterator begin() const noexcept { // NOLINT
    return data_;
  }
  ConstIterator end() const noexcept { // NOLINT
    return data_ + size_;
  }
  ConstIterator cbegin() const noexcept { // NOLINT
    return data_;
  }
  ConstIterator cend() const noexcept { // NOLINT
    return data_ + size_;
  }
  ReverseIterator rbegin() noexcept { // NOLINT
    return ReverseIterator(end());
  }
  ConstReverseIterator rbegin() const noexcept { // NOLINT
    return ConstReverseIterator(end());
  }
  ReverseIterator rend() noexcept { // NOLINT
    return ReverseIterator(begin());
  }
  ConstReverseIterator rend() const noexcept { // NOLINT
    return ConstReverseIterator(begin());
  }
  ConstReverseIterator crbegin() noexcept { // NOLINT
    return ConstReverseIterator(end());
  }
  ConstReverseIterator crend() noexcept { // NOLINT
    return ConstReverseIterator(begin());
  }


private:
  Pointer data_;
  SizeType size_ ;
  SizeType capacity_;
  template <typename U>
  void DoubleCap(U&& v) {
    const SizeType new_cap = capacity_ ? capacity_ * 2 : 1;
    std::unique_ptr<ValueType[]> guard(new ValueType[new_cap]);
    SizeType i = 0;
    try {
      for (; i < size_; ++i) {
        guard[i] = std::move(data_[i]);
      }
      guard[size_] = std::forward<U>(v);
    } catch(...) {
      throw;
    }
    delete[] data_;
    data_ = guard.release();
    capacity_ = new_cap;
    ++size_;
  }
};
#endif //VECTOR_H
