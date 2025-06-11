#ifndef VECTOR_H
#define VECTOR_H
#define VECTOR_MEMORY_IMPLEMENTED

#include <memory>
#include <stdexcept>
#include <iterator>
#include <algorithm>

template<typename T>
class Vector {
public:
  using ValueType = T;
  using SizeType = std::size_t;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  Vector() : data_(nullptr), size_(0), capacity_(0) {}
  explicit Vector(const SizeType size) : data_(nullptr), size_(0), capacity_(size) {
    if (size == 0) {
      return;
    }
    Pointer new_data = Allocate(size);
    SizeType i = 0;
    try {
      for (; i < size; ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, new_data + i);
      }
    } catch (...) {
      std::destroy_n(new_data, i);
      Deallocate(new_data, size);
      throw;
    }
    data_ = new_data;
    size_ = size;
  }
  Vector(const SizeType size, ValueType value) : data_(nullptr), size_(0), capacity_(size) {
    if (size == 0) {
      return;
    }
    Pointer new_data = Allocate(size);
    SizeType i = 0;
    try {
      for (; i < size; ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, new_data + i, value);
      }
    } catch (...) {
      std::destroy_n(new_data, i);
      Deallocate(new_data, size);
      throw;
    }
    data_ = new_data;
    size_ = size;
  }
  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) : data_(nullptr), size_(0), capacity_(0) {
    const SizeType cnt = std::distance(first, last);
    capacity_ = cnt;
    if (cnt == 0) {
      return;
    }
    Pointer new_data = Allocate(cnt);
    SizeType i = 0;
    try {
      for (; first != last; ++first, ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, new_data + i, *first);
      }
    } catch (...) {
      std::destroy_n(new_data, i);
      Deallocate(new_data, cnt);
      throw;
    }
    data_ = new_data;
    size_ = cnt;
  }
    Vector(std::initializer_list<T> init) : data_(nullptr), size_(0), capacity_(init.size()) {
    if (init.size() == 0) {
      return;
    }
    Pointer new_data = Allocate(init.size());
    SizeType i = 0;
    try {
      for (auto& elem : init) {
        std::allocator_traits<Allocator>::construct(alloc_, new_data + i, elem);
        ++i;
      }
    } catch (...) {
      std::destroy_n(new_data, i);
      Deallocate(new_data, init.size());
      throw;
    }
    data_ = new_data;
    size_ = init.size();
  }
  Vector(const Vector& other) : data_(nullptr), size_(0), capacity_(other.capacity_) {
    if (other.size_ == 0) {
      return;
    }
    Pointer new_data = Allocate(other.capacity_);
    SizeType i = 0;
    try {
      for (; i < other.size_; ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, new_data + i, other.data_[i]);
      }
    } catch (...) {
      std::destroy_n(new_data, i);
      Deallocate(new_data, other.capacity_);
      throw;
    }
    data_ = new_data;
    size_ = other.size_;
  }
  Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = other.capacity_ = 0;
  }
  Vector& operator=(const Vector& other) {
    if (this != &other) {
      Vector tmp(other);
      Swap(tmp);
    }
    return *this;
  }
  Vector &operator=(Vector&& other) noexcept {
    if (this != &other) {
      Clear();
      Deallocate(data_, capacity_);
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = other.capacity_ = 0;
    }
    return *this;
  }
  ~Vector() {
    Clear();
    Deallocate(data_, capacity_);
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
  Reference operator[](SizeType pos) {
    return data_[pos];
  }
  ConstReference operator[](SizeType pos) const {
    return data_[pos];
  }
  Reference At(SizeType index) {
    if (index >= size_) {
      throw std::out_of_range("At was out of range");
    }
    return data_[index];
  }
  ConstReference At(SizeType pos) const {
    if (pos >= size_) {
      throw std::out_of_range("At was out of range");
    }
    return data_[pos];
  }
  Reference Front() {
    if (Empty()) {
      throw std::out_of_range("Vector is empty");
    }
    return data_[0];
  }
  ConstReference Front() const {
    if (Empty()) {
      throw std::out_of_range("Vector is empty");
    }
    return data_[0];
  }
  Reference Back() {
    if (Empty()) {
      throw std::out_of_range("Vector is empty");
    }
    return data_[size_ - 1];
  }
  ConstReference Back() const {
    if (Empty()) {
      throw std::out_of_range("Vector is empty");
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
  void Resize(SizeType new_size) {
    if (new_size < size_) {
      std::destroy_n(data_ + new_size, size_ - new_size);
      size_ = new_size;
    } else if (new_size <= capacity_) {
      std::uninitialized_value_construct_n(data_ + size_, new_size - size_);
      size_ = new_size;
    } else {
      SizeType new_cap = new_size;
      T* new_data = Allocate(new_cap);
      SizeType i = 0;
      try {
        for (; i < size_; ++i) {
          std::allocator_traits<Allocator>::construct(alloc_, new_data + i,
              std::move_if_noexcept(data_[i]));
        }
        std::uninitialized_value_construct_n(new_data + size_, new_size - size_);
      }
      catch (...) {
        std::destroy_n(new_data, i);
        Deallocate(new_data, new_cap);
        throw;
      }
      std::destroy_n(data_, size_);
      Deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_cap;
      size_ = new_size;
    }
  }
  void Resize(const SizeType new_size, ConstReference value) {
    if (new_size < size_) {
      std::destroy_n(data_ + new_size, size_ - new_size);
      size_ = new_size;
    } else if (new_size <= capacity_) {
      std::uninitialized_fill_n(data_ + size_, new_size - size_, value);
      size_ = new_size;
    } else {
      SizeType new_cap = new_size;
      T* new_data = Allocate(new_cap);
      SizeType i = 0;
      try {
        for (; i < size_; ++i)
        {
          std::allocator_traits<Allocator>::construct(alloc_, new_data + i,
              std::move_if_noexcept(data_[i]));
        }
        std::uninitialized_fill_n(new_data + size_, new_size - size_, value);
      }
      catch (...) {
        std::destroy_n(new_data, i);
        Deallocate(new_data, new_cap);
        throw;
      }
      std::destroy_n(data_, size_);
      Deallocate(data_, capacity_);
      data_ = new_data;
      capacity_ = new_cap;
      size_ = new_size;
    }
  }
  void Reserve(const SizeType new_cap) {
    if (new_cap <= capacity_) {
      return;
    }
    Reallocate(new_cap);
  }
  void ShrinkToFit() {
    if (size_ < capacity_) {
      Reallocate(size_);
    }
  }
  void Clear() {
    std::destroy_n(data_, size_);
    size_ = 0;
  }
  void PushBack(ConstReference value) {
    if (size_ < capacity_) {
      std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, value);
      ++size_;
    } else {
      DoubleCap(value);
    }
  }
  void PushBack(ValueType&& value) {
    if (size_ < capacity_) {
      std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::move(value));
      ++size_;
    } else {
      DoubleCap(std::move(value));
    }
  }
  template <typename... Args>
  Reference EmplaceBack(Args&&... args) {
    if (size_ == capacity_) {
      Reallocate(capacity_ ? capacity_ * 2 : 1);
    }
    std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
    ++size_;
    return data_[size_ - 1];
  }
  void PopBack() {
    if (Empty()) {
      throw std::out_of_range("Vector::pop_back on empty");
    }
    --size_;
    std::allocator_traits<Allocator>::destroy(alloc_, data_ + size_);
  }
  bool operator==(const Vector& other) const {
    if (size_ != other.size_) {
      return false;
    }
    return std::equal(data_, data_ + size_, other.data_);
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
  using Allocator = std::allocator<T>;
  Allocator alloc_;
  Pointer data_;
  SizeType size_;
  SizeType capacity_;
  template <typename U>
  void DoubleCap(U&& value)
  {
    const SizeType new_cap = capacity_ ? capacity_ * 2 : 1;
    Pointer new_data = Allocate(new_cap);
    SizeType i = 0;
    try
    {
      for (; i < size_; ++i)
      {
        std::allocator_traits<Allocator>::construct(alloc_, new_data + i,
            std::move_if_noexcept(data_[i]));
      }
      std::allocator_traits<Allocator>::construct(alloc_, new_data + size_,
          std::forward<U>(value));
    }
    catch (...)
    {
      std::destroy_n(new_data, i);
      Deallocate(new_data, new_cap);
      throw;
    }
    std::destroy_n(data_, size_);
    Deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_cap;
    ++size_;
  }
  static Pointer Allocate(SizeType size) {
    if (size == 0) {
      return nullptr;
    }
    Allocator a;
    return std::allocator_traits<Allocator>::allocate(a, size);
  }
  static void Deallocate(Pointer pointer, SizeType size) {
    if (!pointer) {
      return;
    }
    Allocator a;
    std::allocator_traits<Allocator>::deallocate(a, pointer, size);
  }
  void Reallocate(const SizeType new_cap) {
    Pointer new_data = Allocate(new_cap);
    if constexpr (std::is_nothrow_move_constructible_v<ValueType>) {
      std::uninitialized_move_n(data_, size_, new_data);
    } else {
      std::uninitialized_copy_n(data_, size_, new_data);
    }
    std::destroy_n(data_, size_);
    Deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_cap;
  }
};

#endif
