//
// Created by arsen on 25.05.25.
//
#include "range.h"
RangeIterator::RangeIterator(int begin, int step) : begin_(begin), step_(step) {
}
int RangeIterator::operator*() const {
  return begin_;
}
RangeIterator& RangeIterator::operator++() {
  begin_ += step_;
  return *this;
}
bool RangeIterator::operator!=(const RangeIterator& other) const {
  return (step_ < 0 && begin_ > other.begin_) || (step_ > 0 && begin_ < other.begin_);
}
Range::Range(int end) : begin_(0), end_(std::max(0, end)), step_(1) {
}
Range::Range(int begin, int end) : begin_(begin), end_(std::max(begin, end)), step_(1) {
}
Range::Range(int begin, int end, int step) : begin_(begin), end_(0), step_(step) {
  if ((step < 0 && end < begin) || (step > 0 && end > begin)) {
    end_ = end;
  } else {
    end_ = begin;
  }
}
RangeIterator Range::begin() const {
  return {begin_, step_};
}
RangeIterator Range::end() const {
  return {end_, step_};
}
RangeIterator Range::rbegin() const {
  if (begin_ == end_) {
    return {begin_ + (step_ > 0 ? -1 : 1), -step_};
  }
  return {begin_ + step_ * ((end_ - begin_ + (step_ > 0 ? -1 : 1)) / step_), -step_};
}
RangeIterator Range::rend() const {
  return {begin_ + (step_ > 0 ? -1 : 1), -step_};
}