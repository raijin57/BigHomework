//
// Created by arsen on 25.05.25.
//
#include "counting_logics.h"
#include "big_integer.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numbers>

std::string DeleteFirstZero(std::string number) {
  if (number.empty()) {
    return "0";
  }
  size_t index = 0;
  while (index < number.size() && number[index] == '0') {
    index++;
  }
  return index == number.size() ? "0" : number.substr(index);
}
std::string Addition(std::string first_number, std::string second_number) {
  std::string str1 = first_number;
  std::string str2 = second_number;
  std::string output;
  std::reverse(str1.begin(), str1.end());
  std::reverse(str2.begin(), str2.end());
  size_t index = 0;
  int value = 0;
  for (size_t i = 0; i < std::min(str1.size(), str2.size()); i++) {
    output += static_cast<char>((value + (str1[i] - '0') + (str2[i] - '0')) % 10 + '0');
    value = (value + str1[i] - '0' + str2[i] - '0') / 10;
    index = i;
  }
  index++;
  if (str1.size() > str2.size()) {
    for (; index < str1.size(); index++) {
      int sum = value + str1[index] - '0';
      output += static_cast<char>(sum % 10 + '0');
      value = sum / 10;
    }
  } else if (str1.size() < str2.size()) {
    for (; index < str2.size(); index++) {
      int sum = value + str2[index] - '0';
      output += static_cast<char>((sum % 10) + '0');
      value = sum / 10;
    }
  }
  if (value > 0) {
    output += static_cast<char>(value + '0');
  }
  std::reverse(output.begin(), output.end());
  output = DeleteFirstZero(output);
  if (output.length() > 30000) {
    throw BigIntegerOverflow();
  }
  return output;
}
std::string Subtraction(std::string first_number, std::string second_number) {
  int64_t difference = std::abs(static_cast<int>(first_number.size() - second_number.size()));
  std::string zero_string(difference, '0');
  if (first_number.size() > second_number.size()) {
    second_number = zero_string + second_number;
  } else {
    first_number = zero_string + first_number;
  }
  std::string max_value;
  std::string min_value;
  for (size_t i = 0; i < first_number.size(); i++) {
    if (first_number[i] > second_number[i]) {
      max_value = first_number;
      min_value = second_number;
      break;
    }
    if (first_number[i] < second_number[i]) {
      max_value = second_number;
      min_value = first_number;
      break;
    }
  }
  std::reverse(max_value.begin(), max_value.end());
  std::reverse(min_value.begin(), min_value.end());
  std::string output;
  int value = 0;
  for (size_t i = 0; i < max_value.size(); ++i) {
    int a = max_value[i] - '0';
    int b = min_value[i] - '0';
    a += value;
    if (a < b) {
      a += 10;
      value = -1;
    } else {
      value = 0;
    }
    output += static_cast<char>((a - b) + '0');
  }
  std::reverse(output.begin(), output.end());
  output = DeleteFirstZero(output);
  return output;
}
std::vector<int> ToVector(const std::string& s) {
  std::vector<int> result;
  for (size_t i = s.size() - 1; static_cast<int>(i) >= 0; i--) {
    result.push_back(s[i] - '0');
  }
  return result;
}
std::string Multiplication(const std::string& num1, const std::string& num2) {
  if (num1 == "0" || num2 == "0") {
    return "0";
  }
  int n = static_cast<int>(num1.size());
  int m = static_cast<int>(num2.size());
  std::vector<int> result(n + m, 0);
  std::string reversed_num1(num1.rbegin(), num1.rend());
  std::string reversed_num2(num2.rbegin(), num2.rend());
  for (int i = 0; i < n; ++i) {
    int digit_1 = reversed_num1[i] - '0';
    for (int j = 0; j < m; ++j) {
      int digit_2 = reversed_num2[j] - '0';
      result[i + j] += digit_1 * digit_2;
      result[i + j + 1] += result[i + j] / 10;
      result[i + j] %= 10;
    }
  }
  std::string output;
  for (int i = n + m - 1; i >= 0; --i) {
    output.push_back(static_cast<char>(result[i] + '0'));
  }
  output = DeleteFirstZero(output);
  return output;
}