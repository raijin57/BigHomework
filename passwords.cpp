//
// Created by arsen on 22.05.25.
//
#include <iostream>
#include <string>
#include <vector>

int main() {
  int n = 0;
  int count = 0;
  std::cin >> n;
  std::vector<std::string> password;
  for (int i = 0; i < n; i++) {
    std::string some_password;
    std::cin >> some_password;
    password.push_back(some_password);
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      if (password[i] == password[j] || password[i].find(password[j]) != std::string::npos) {
        count++;
      }
    }
  }
  std::cout << count;
}
