//
// Created by arsen on 21.05.25.
//
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

int main() {
  std::unordered_map<std::string, int> words;
  std::string word;

  while (std::cin >> word) {
    std::cout << words[word] << " ";
    words[word]++;
  }
}
