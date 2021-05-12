#ifndef READERS_ROGUEDATAREADER_H
#define READERS_ROGUEDATAREADER_H

//---< C++ >---//
#include <fstream>

//---< rogue >---//
#include "rogue/interfaces/stream/Slave.h"

// Forward declarations
namespace rogue::interfaces::stream {
class Frame;
} // namespace rogue::interfaces::stream

namespace readers {

class RogueDataReader : public rogue::interfaces::stream::Slave {

public:
  /// Constructor
  RogueDataReader();

  /// Default destructor
  ~RogueDataReader() = default;

  void
  acceptFrame(std::shared_ptr<rogue::interfaces::stream::Frame> frame);

  void closeCSV() { csv.close(); }

private:
  int event_index{0};
  // void parseFrame(byte data);

  int getField(int value, int high_bit, int low_bit) {
    int mask = pow(2, (high_bit - low_bit + 1)) - 1;
    return (value >> low_bit) & mask;
  }

  void print_uint8_t(uint8_t n) {
    int i;
    for (i = 8; i >= 0; i--)
      printf("%d", (n & (1 << i)) >> i);
    putchar('\n');
  }
  std::vector<uint8_t> channels;

  std::ofstream csv;

}; // RogueDataReader
} // namespace reader

#endif // __SVT_DATA_READER_H__
