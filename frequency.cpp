//
// Created by arsen on 22.05.25.
//
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::string text;
  std::string line;
  while (std::getline(std::cin, line)) {
    text += line + " ";
  }
  std::istringstream iss(text);
  std::vector<std::pair<int, std::string>> words;
  std::map<std::string, int> number_of_words;
  std::string word;
  while (iss >> word) {
    number_of_words[word]++;
  }
  for (auto& [w, count] : number_of_words) {
    words.emplace_back(-count, w);
  }
  std::sort(words.begin(), words.end());
  for (auto& item : words) {
    std::cout << item.second << std::endl;
  }

  return 0;
}
