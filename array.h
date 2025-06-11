//
// Created by arsen on 22.05.25.
//

#ifndef ARRAY_H
#define ARRAY_H
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
struct ArrayOutOfRange : public std::out_of_range {
  ArrayOutOfRange() : std::out_of_range("Array index out of range") {}
};

template <typename T, size_t N>
class Array {
public:
  T data[N];
  T& operator[](size_t index) {
    return data[index];
  }
  const T& operator[](size_t idx) const {
    return data[idx];
  }
  T& At(size_t idx) {
    if (idx >= N) {
      throw ArrayOutOfRange{};
    }
    return data[idx];
  }
  const T& At(size_t idx) const {
    if (idx >= N) {
      throw ArrayOutOfRange{};
    }
    return data[idx];
  }
  T& Front() {
    return data[0];
  }
  const T& Front() const {
    return data[0];
  }
  T& Back() {
    return data[N - 1];
  }
  const T& Back() const {
    return data[N - 1];
  }
  T* Data() {
    return data;
  }
  const T* Data() const {
    return data;
  }
  size_t Size() const {
    return N;
  }
  bool Empty() const {
    return false;
  }
  bool Empty() {
    return false;
  }
  void Fill(const T& value) {
    for (size_t i = 0; i < N; ++i) {
      data[i] = value;
    }
  }
  void Swap(Array<T, N>& other) {
    for (size_t i = 0; i < N; ++i) {
      T temp = data[i];
      data[i] = other.data[i];
      other.data[i] = temp;
    }
  }
};
#endif //ARRAY_H
