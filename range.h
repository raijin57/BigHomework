//
// Created by arsen on 25.05.25.
//
#include <algorithm>
#ifndef RANGE_H
#define RANGE_H
#define REVERSE_RANGE_IMPLEMENTED

class RangeIterator {
public:
  RangeIterator(int, int);
  int operator*() const;
  RangeIterator& operator++();
  bool operator!=(const RangeIterator& other) const;
private:
  int begin_;
  int step_;
};

class Range {
public:
  explicit Range(int);
  Range(int, int);
  Range(int, int, int);
  RangeIterator begin() const;  // NOLINT
  RangeIterator end() const;    // NOLINT
  RangeIterator rbegin() const;  // NOLINT
  RangeIterator rend() const;    // NOLINT
private:
  int begin_;
  int end_;
  int step_;
};
#endif //RANGE_H
