//
// Created by arsen on 25.05.25.
//
#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstring>
#include <sstream>
#include "counting_logics.h"
#include "big_integer.h"

BigInteger::BigInteger() {
  is_positive_ = true;
  value_ = "0";
}
BigInteger::BigInteger(const int value) {
  if (value == 0) {
    value_ = "0";
    is_positive_ = true;
    return;
  }
  is_positive_ = value >= 0;
  if (value == INT_MIN) {
    value_ = "2147483648";
  } else {
    value_ = std::to_string(std::abs(value));
  }
}
BigInteger::BigInteger(const int64_t value) {
  if (value == 0) {
    value_ = "0";
    is_positive_ = true;
    return;
  }
  is_positive_ = value >= 0;
  if (value < 0) {
    if (value == INT64_MIN) {
      value_ = "9223372036854775808";
    } else {
      value_ = std::to_string(-value);
    }
  } else {
    value_ = std::to_string(value);
  }
}
BigInteger::BigInteger(const char* value) {
  if (value == nullptr || strlen(value) == 0) {
    value_ = "0";
    is_positive_ = true;
    return;
  }
  const std::string string = value;
  if (string.length() > 30000) {
    throw BigIntegerOverflow();
  }
  size_t start = 0;
  is_positive_ = true;
  if (string[0] == '-') {
    is_positive_ = false;
    start = 1;
  } else if (string[0] == '+') {
    start = 1;
  }
  if (start >= string.length()) {
    throw std::invalid_argument("Invalid number format");
  }
  for (size_t i = start; i < string.length(); ++i) {
    if (string[i] < '0' || string[i] > '9') {
      throw std::invalid_argument("Invalid number format");
    }
  }
  value_ = string.substr(start);
  DeleteFirstZero(value_);
  if (value_ == "0") {
    is_positive_ = true;
  }
}
bool BigInteger::IsNegative() const {
  return !is_positive_;
}
BigInteger BigInteger::operator+() const {
  return *this;
}
BigInteger BigInteger::operator-() const {
  if (this->is_positive_) {
    return BigInteger(('-' + this->value_).c_str());
  }
  return BigInteger((this->value_).c_str());
}
BigInteger& BigInteger::operator++() {
  if (is_positive_) {
    value_ = Addition(value_, "1");
  } else {
    value_ = Subtraction(value_, "1");
  }
  if (value_ == "0") {
    is_positive_ = true;
  }
  if (static_cast<int64_t>(this->value_.size()) > 30000) {
    throw BigIntegerOverflow();
  }
  return *this;
}
BigInteger BigInteger::operator++(int) {
  BigInteger old = *this;
  ++(*this);
  if (static_cast<int64_t>(this->value_.size()) > 30000) {
    throw BigIntegerOverflow();
  }
  return old;
}
BigInteger BigInteger::operator--(int) {
  BigInteger old = *this;
  --(*this);
  if (static_cast<int64_t>(this->value_.size()) > 30000) {
    throw BigIntegerOverflow();
  }
  return old;
}
BigInteger& BigInteger::operator--() {
  if (is_positive_) {
    if (value_ == "0") {
      is_positive_ = false;
      value_ = "1";
    } else {
      value_ = Subtraction(value_, "1");
      if (value_ == "0") {
        is_positive_ = true;
      }
    }
  } else {
    value_ = Addition(value_, "1");
    if (value_ == "0") {
      is_positive_ = true;
    }
  }
  if (static_cast<int64_t>(this->value_.size()) > 30000) {
    throw BigIntegerOverflow();
  }
  return *this;
}
BigInteger BigInteger::operator+(const BigInteger& other) const {
  if (this->is_positive_ && other.is_positive_) {
    return BigInteger(Addition(this->value_, other.value_).c_str());
  }
  if (!this->is_positive_ && !other.is_positive_) {
    return BigInteger(
        (std::string("-") + Addition(this->value_, other.value_)).c_str());
  }
  if (this->is_positive_ && !other.is_positive_) {
    if (BigInteger((this->value_).c_str()) <
        BigInteger((other.value_).c_str())) {
      return BigInteger(
          (std::string("-") + Subtraction(this->value_, other.value_)).c_str());
    }
    return BigInteger(Subtraction(this->value_, other.value_).c_str());
  }
  if (BigInteger(this->value_.c_str()) > BigInteger(other.value_.c_str())) {
    return BigInteger(
        (std::string("-") + Subtraction(this->value_, other.value_)).c_str());
  }
  return BigInteger(Subtraction(this->value_, other.value_).c_str());
}
BigInteger BigInteger::operator+(const int64_t& other) const {
  return *this + BigInteger(other);
}
BigInteger& BigInteger::operator+=(const BigInteger& other) {
  return *this = *this + other;
}
BigInteger& BigInteger::operator+=(const int64_t& other) {
  *this += BigInteger(other);
  if (static_cast<int64_t>(this->value_.size()) > 30000) {
    throw BigIntegerOverflow();
  }
  return *this;
}
BigInteger BigInteger::operator-(const BigInteger& other) const {
  return *this + -other;
}
BigInteger BigInteger::operator-(const int64_t& other) const {
  return *this - BigInteger(other);
}
BigInteger& BigInteger::operator-=(const BigInteger& other) {
  return *this = *this - other;
}
BigInteger& BigInteger::operator-=(const int64_t& other) {
  *this -= BigInteger(other);
  if (static_cast<int64_t>(this->value_.size()) > 30000) {
    throw BigIntegerOverflow();
  }
  return *this;
}
BigInteger BigInteger::operator*(const BigInteger& other) const {
  if (this->is_positive_ == other.is_positive_) {
    return BigInteger(Multiplication(this->value_, other.value_).c_str());
  }
  return BigInteger((std::string("-") + Multiplication(this->value_, other.value_)).c_str());
}
BigInteger BigInteger::operator*(const int64_t& other) const {
  return *this * BigInteger(other);
}
BigInteger& BigInteger::operator*=(const BigInteger& other) {
  return *this = *this * other;
}
BigInteger& BigInteger::operator*=(const int64_t& other) {
  return *this *= BigInteger(other);
}
bool BigInteger::operator==(const BigInteger& other) const {
  return is_positive_ == other.is_positive_ && value_ == other.value_;
}
bool BigInteger::operator!=(const BigInteger& other) const {
  return !(*this == other);
}
bool BigInteger::operator<(const BigInteger& other) const {
  if (is_positive_ != other.is_positive_) {
    return !is_positive_;
  }
  const int difference = CompareAbs(other);
  return is_positive_ ? difference < 0 : difference > 0;
}
bool BigInteger::operator>(const BigInteger& other) const {
  return !(*this <= other);
}
BigInteger& BigInteger::operator=(const int64_t& other) {
  *this = BigInteger(other);
  return *this;
}
BigInteger& BigInteger::operator=(const int& value) {
  if (value >= 0) {
    this->value_ = std::to_string(value);
    this->is_positive_ = true;
    return *this;
  }
  this->value_ = std::to_string(std::abs(value));
  this->is_positive_ = false;
  return *this;
}

bool BigInteger::operator<=(const BigInteger& other) const {
  return *this < other || *this == other;
}
bool BigInteger::operator>=(const BigInteger& other) const {
  return !(*this < other);
}
BigInteger::operator bool() const {
  return this->value_ != "0";
}
std::ostream& operator<<(std::ostream& os, const BigInteger& other) {
  if (!other.is_positive_) {
    os << '-';
  }
  os << other.value_;
  return os;
}
std::istream& operator>>(std::istream& in, BigInteger& bi) {
  std::string str;
  in >> str;
  bi = BigInteger(str.c_str());
  return in;
}
int BigInteger::CompareAbs(const BigInteger& other) const {
  if (value_.length() != other.value_.length()) {
    return value_.length() < other.value_.length() ? -1 : 1;
  }
  return value_.compare(other.value_);
}










