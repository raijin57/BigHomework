//
// Created by arsen on 30.05.25.
//

#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H
#include <functional>
#include <iterator>
#include <list>
#include <vector>

template <typename KeyT>
class UnorderedSet {
  std::vector<std::list<KeyT>> buckets_;
  size_t size_ = 0;
  void Resize() {
    if (buckets_.empty() || size_ >= buckets_.size()) {
      size_t new_size = 0;
      if (buckets_.empty()) {
        new_size = 1;
      } else {
        new_size = buckets_.size() * 2;
      }
      Rehash(new_size);
    }
  }

 public:
  UnorderedSet() = default;
  explicit UnorderedSet(size_t count) {
    buckets_ = std::vector<std::list<KeyT>>(count);
    size_ = 0;
  }
  template <typename ForwardIterator>
  UnorderedSet(ForwardIterator start, ForwardIterator end) {
    size_ = 0;
    if (size_t length = std::distance(start, end); length > 0) {
      buckets_.resize(length);
      for (auto it = start; it != end; ++it) {
        Insert(*it);
      }
    } else {
      buckets_.resize(1);
    }
  }
  UnorderedSet(const UnorderedSet &other) {
    buckets_ = other.buckets_;
    size_ = other.size_;
  }
  UnorderedSet(UnorderedSet &&other) noexcept {
    buckets_ = std::move(other.buckets_);
    size_ = other.size_;
    other.size_ = 0;
  }
  ~UnorderedSet() = default;
  bool Empty() const {
    return size_ == 0;
  }
  size_t Size() const {
    return size_;
  }
  bool Find(const KeyT &key) const {
    if (buckets_.empty()) {
      return false;
    }
    for (auto &value : buckets_[Bucket(key)]) {
      if (value == key) {
        return true;
      }
    }
    return false;
  }
  bool Insert(const KeyT &key) {
    if (Find(key)) {
      return false;
    }
    Resize();
    buckets_[Bucket(key)].push_back(key);
    ++size_;
    return true;
  }
  UnorderedSet &operator=(const UnorderedSet &other) {
    if (this != &other) {
      buckets_ = other.buckets_;
      size_ = other.size_;
    }
    return *this;
  }
  UnorderedSet &operator=(UnorderedSet &&other) noexcept {
    if (this != &other) {
      buckets_ = std::move(other.buckets_);
      size_ = other.size_;
      other.size_ = 0;
      other.buckets_.clear();
    }
    return *this;
  }
  bool Erase(const KeyT &key) {
    if (buckets_.empty()) {
      return false;
    }
    auto &bucket = buckets_[Bucket(key)];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
      if (key == *it) {
        bucket.erase(it);
        --size_;
        return true;
      }
    }
    return false;
  }
  void Clear() {
    for (auto &bucket : buckets_) {
      bucket.clear();
    }
    size_ = 0;
  }
  void Rehash(size_t new_buckets_count) {
    if (new_buckets_count == buckets_.size() || new_buckets_count < size_) {
      return;
    }
    std::vector<std::list<KeyT>> new_buckets(new_buckets_count);
    for (auto &bucket : buckets_) {
      while (!bucket.empty()) {
        auto it = bucket.begin();
        KeyT key = std::move(*it);
        bucket.erase(it);
        std::hash<KeyT> hasher;
        size_t new_index = hasher(key) % new_buckets_count;
        new_buckets[new_index].push_back(std::move(key));
      }
    }
    buckets_ = std::move(new_buckets);
  }
  void Reserve(size_t new_size) {
    if (new_size > buckets_.size()) {
      Rehash(new_size);
    }
  }
  size_t BucketCount() const {
    return buckets_.size();
  }
  size_t BucketSize(size_t index) const {
    if (index >= buckets_.size()) {
      return 0;
    }
    return buckets_[index].size();
  }
  size_t Bucket(const KeyT &key) const {
    if (buckets_.empty()) {
      return 0;
    }
    std::hash<KeyT> hash;
    return hash(key) % buckets_.size();
  }
  double LoadFactor() const {
    if (buckets_.empty()) {
      return 0.0;
    }
    return static_cast<double>(size_) / static_cast<double>(buckets_.size());
  }
  bool Insert(KeyT &&key) {
    if (Find(key)) {
      return false;
    }
    Resize();
    buckets_[Bucket(key)].emplace_back(std::move(key));
    ++size_;
    return true;
  }
};
#endif  // UNORDERED_SET_H