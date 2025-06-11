//
// Created by arsen on 25.05.25.
//

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <iostream>
#include <numbers>
#include <string>
#include <vector>

class BigIntegerOverflow : public std::runtime_error {
public:
  BigIntegerOverflow() : std::runtime_error("BigInteger overflow") {
  }
};
class BigInteger {
public:
  BigInteger();
  explicit BigInteger(const char *value);
  explicit BigInteger(int);                  // NOLINT
  BigInteger(int64_t);                       // NOLINT
  BigInteger(const BigInteger &) = default;  // NOLINT
  bool IsNegative()const;
  BigInteger operator+() const;
  BigInteger operator+(const int64_t &) const;
  BigInteger operator+(const BigInteger &) const;
  BigInteger &operator+=(const int64_t &);
  BigInteger &operator+=(const BigInteger &);
  BigInteger operator-() const;
  BigInteger operator-(const int64_t &) const;
  BigInteger operator-(const BigInteger &) const;
  BigInteger &operator-=(const int64_t &);
  BigInteger &operator-=(const BigInteger &);
  BigInteger operator*(const BigInteger &) const;
  BigInteger operator*(const int64_t &) const;
  BigInteger &operator*=(const int64_t &);
  BigInteger &operator*=(const BigInteger &);
  BigInteger &operator=(const int &);
  BigInteger &operator=(const int64_t &);
  BigInteger &operator=(const BigInteger &) = default;
  BigInteger &operator++();
  BigInteger operator++(int);
  BigInteger &operator--();
  BigInteger operator--(int);
  bool operator==(const BigInteger &) const;
  bool operator!=(const BigInteger &) const;
  bool operator>(const BigInteger &) const;
  bool operator<(const BigInteger &) const;
  bool operator>=(const BigInteger &) const;
  bool operator<=(const BigInteger &) const;
  explicit operator bool() const;
  friend std::ostream &operator<<(std::ostream &, const BigInteger &);
  friend std::istream &operator>>(std::istream &, BigInteger &);
private:
  std::string value_;
  bool is_positive_;
  int CompareAbs(const BigInteger &other) const;
};
#endif //BIG_INTEGER_H
