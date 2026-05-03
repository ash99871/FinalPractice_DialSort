#pragma once
#include <vector>
#include <cstdint>
using namespace std;

bool dialsort(std::vector<int>& a);

vector<int> gen_uniform(size_t n, int U, uint64_t seed);
vector<int> gen_skewed(size_t n, int U, uint64_t seed);
vector<int> gen_sorted(size_t n, int U, uint64_t seed);
vector<int> gen_reverse(size_t n, int U, uint64_t seed);
