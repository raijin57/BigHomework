//
// Created by arsen on 31.05.25.
//

#ifndef SHAREDPTR_H
#define SHAREDPTR_H
#define WEAK_PTR_IMPLEMENTED
#include <stdexcept>

class BadWeakPtr final : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("") {}
};

struct Counter {
  Counter() : count_strong_ptr_(0), count_weak_ptr_(0) {};
  void AddWeakPtr() {
    ++count_weak_ptr_;
  }
  void AddStrongPtr() {
    ++count_strong_ptr_;
  }
  void DeleteStrongPtr() {
    --count_strong_ptr_;
  }
  void DeleteWeakPtr() {
    --count_weak_ptr_;
  }
  bool ShouldDeleteCounter() const {
    return count_weak_ptr_ == 0 && count_strong_ptr_ == 0;
  }
  ~Counter() = default;
  size_t GetCountStrongPtr() const {
    return count_strong_ptr_;
  }
  size_t GetCountWeakPtr() const {
    return count_weak_ptr_;
  }

private:
  size_t count_strong_ptr_;
  size_t count_weak_ptr_;
};

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
  void Delete() {
    if (counter_) {
      size_t prev_strong = counter_->GetCountStrongPtr();
      counter_->DeleteStrongPtr();
      if (prev_strong == 1) {
        delete ptr_;
        ptr_ = nullptr;
        if (counter_->GetCountWeakPtr() == 0) {
          delete counter_;
          counter_ = nullptr;
        }
      }
    }
  }
  SharedPtr(T* ptr, Counter* counter, const bool is_weak_ptr) : ptr_(ptr), counter_(counter) {
    if (is_weak_ptr && (!counter_ || counter_->GetCountStrongPtr() == 0)) {
      throw BadWeakPtr();
    }
    if (counter_ && counter_->GetCountStrongPtr() > 0) {
      counter_->AddStrongPtr();
    }
  }
  friend class WeakPtr<T>;

 public:
  T* ptr_ = nullptr;
  Counter* counter_ = nullptr;
  SharedPtr() noexcept : ptr_(nullptr), counter_(nullptr) {}
  explicit SharedPtr(T* ptr) {
    ptr_ = ptr;
    if (ptr) {
      counter_ = new Counter();
      counter_->AddStrongPtr();
    }
  }
  SharedPtr(const SharedPtr& other) {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    if (counter_) {
      counter_->AddStrongPtr();
    }
  }
  SharedPtr(SharedPtr&& other) noexcept {
    ptr_ = std::move(other.ptr_);
    counter_ = std::move(other.counter_);
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }
  explicit SharedPtr(const WeakPtr<T>& weak_ptr) {
    ptr_ = weak_ptr.ptr_;
    counter_ = weak_ptr.counter_;
    if (!counter_) {
      throw BadWeakPtr();
    }
    if (counter_->GetCountStrongPtr() == 0) {
      throw BadWeakPtr();
    }
    counter_->AddStrongPtr();
  }
  ~SharedPtr() { Delete(); }
  T* Get() const { return ptr_; }
  size_t UseCount() const {
    if (counter_ == nullptr) {
      return 0;
    }
    return counter_->GetCountStrongPtr();
  }
  void Reset(T* ptr = nullptr) {
    Delete();
    ptr_ = ptr;
    if (ptr_) {
      counter_ = new Counter();
      counter_->AddStrongPtr();
    } else {
      counter_ = nullptr;
    }
  }
  void Swap(SharedPtr& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
  }
  T& operator*() const {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }
  explicit operator bool() const {
    return ptr_ != nullptr;
  }
  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      Delete();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      if (counter_) {
        counter_->AddStrongPtr();
      }
    }
    return *this;
  }
  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      Delete();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      other.ptr_ = nullptr;
      other.counter_ = nullptr;
    }
    return *this;
  }
};

template <typename T>
class WeakPtr {
  friend class WeakPtr;
  void Delete() {
    ptr_ = nullptr;
    if (counter_) {
      counter_->DeleteWeakPtr();
      if (counter_->ShouldDeleteCounter()) {
        delete counter_;
      }
      counter_ = nullptr;
    }
  }

 public:
  T* ptr_ = nullptr;
  Counter* counter_ = nullptr;
  WeakPtr() noexcept : ptr_(nullptr) {}
  WeakPtr(const WeakPtr& other) {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    if (counter_) {
      counter_->AddWeakPtr();
    }
  }
  WeakPtr(WeakPtr&& other) noexcept {
    Delete();
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }
  WeakPtr(const SharedPtr<T>& shared_ptr) : ptr_(shared_ptr.ptr_), counter_(shared_ptr.counter_) { // NOLINT
    if (counter_) {
      counter_->AddWeakPtr();
    }
  }
  ~WeakPtr() {
    Delete();
  }
  size_t UseCount() const {
    return counter_ == nullptr ? 0 : counter_->GetCountStrongPtr();
  }
  bool Expired() const {
    return counter_ == nullptr || UseCount() == 0;
  }
  void Reset() {
    Delete();
  }
  void Swap(WeakPtr& other) {
    std::swap(counter_, other.counter_);
    std::swap(ptr_, other.ptr_);
  }
  WeakPtr& operator=(const WeakPtr& other) {
    if (this != &other) {
      Delete();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      if (counter_) {
        counter_->AddWeakPtr();
      }
    }
    return *this;
  }
  WeakPtr& operator=(WeakPtr&& other) noexcept {
    if (this != &other) {
      Delete();
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      other.ptr_ = nullptr;
      other.counter_ = nullptr;
    }
    return *this;
  }
  WeakPtr& operator=(const SharedPtr<T>& shared_ptr) {
    Delete();
    ptr_ = shared_ptr.ptr_;
    counter_ = shared_ptr.counter_;
    if (counter_) {
      counter_->AddWeakPtr();
    }
    return *this;
  }
  SharedPtr<T> Lock() const {
    if (Expired()) {
      return SharedPtr<T>();
    }
    return SharedPtr<T>(ptr_, counter_, false);
  }
};
template <typename T, typename... ArgType>
SharedPtr<T> MakeShared(ArgType&&... args) {
  return SharedPtr<T>(new T(std::forward<ArgType>(args)...));
}
#endif //SHAREDPTR_H
