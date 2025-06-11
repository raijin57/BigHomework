//
// Created by arsen on 19.05.25.
//
#include <cstdint>
#include <iostream>
#include <set>
#include <vector>
int64_t HeapUpdate(std::multiset<int64_t>& heap) {
  const auto first = heap.begin();
  const int64_t first_el = *first;
  heap.erase(first);
  const auto second = heap.begin();
  const int64_t second_el = *second;
  heap.erase(second);
  return first_el + second_el;
}

int64_t CountPassports(const std::vector<int>& provinces) {
  if (provinces.empty()) {
    return 0;
  }
  std::multiset<int64_t> heap(provinces.begin(), provinces.end());
  int64_t total_passports = 0;
  while (heap.size() > 1) {
    int64_t new_province = HeapUpdate(heap);
    total_passports += new_province;
    heap.insert(new_province);
  }
  return total_passports;
}


int main() {
  int n = 0;
  std::cin >> n;
  std::vector<int> provinces(n);
  for (int i = 0; i < n; i++) {
    std::cin >> provinces[i];
  }
  std::cout << CountPassports(provinces) << std::endl;
  return 0;
}