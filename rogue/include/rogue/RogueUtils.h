#ifndef ROGUE_ROGUEUTILS_H
#define ROGUE_ROGUEUTILS_H

//---< C++ >---//
#include <bitset>
#include <string>
#include <math.h>

namespace rogue {
int getField(int value, int high_bit, int low_bit) {
  int mask{static_cast<int>(pow(2, (high_bit - low_bit + 1)) - 1)};
  return (value >> low_bit) & mask;
}

template <typename T> std::string toBinary(T value) {
  return std::bitset<32>(value)
      .to_string<char, std::string::traits_type, std::string::allocator_type>();
}

} // namespace rogue

#endif // ROGUE_ROGUEUTILS_H
