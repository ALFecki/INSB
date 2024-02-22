#pragma once
#include <bitset>
#include <stdexcept>
#include <vector>

namespace des {

std::bitset<32> S_boxes(const std::bitset<48> &s);

void P(std::bitset<32> &s);

void initial_permutation(std::bitset<64> &bs);

void final_permutation(std::bitset<64> &bs);

template <size_t N>
void split_in_half(const std::bitset<N * 2> &full, std::bitset<N> &left, std::bitset<N> &right);

template <size_t N>
std::bitset<2 * N> merge_halves(const std::bitset<N> &left, const std::bitset<N> &right);

std::bitset<56> PC_1(const std::bitset<64> &s);

std::bitset<48> PC_2(const std::bitset<56> &s);

void cyclic_shift(std::bitset<28> &key, int step);

std::bitset<48> expand(const std::bitset<32> &Ri);

std::bitset<64> encrypt(std::bitset<64> data, std::bitset<64> key);

void get_keys(std::vector<std::bitset<48>> &keys, const std::bitset<64> &key);

std::bitset<64> decrypt(std::bitset<64> data, std::bitset<64> key);

std::bitset<64> str_to_bits(const std::string &s);

std::string bits_to_str(const std::bitset<64> &bits);
}  // namespace des