#ifndef ROGUE_ROGUEDATARECEIVER_H
#define ROGUE_ROGUEDATARECEIVER_H

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

//---< C++ >---//
//#include <fstream>

//---< rogue >---//
#include "rogue/interfaces/stream/Slave.h"

// Forward declarations
namespace rogue::interfaces::stream {
class Frame;
} // namespace rogue::interfaces::stream

namespace rogue {

class RogueDataReceiver : public rogue::interfaces::stream::Slave {

public:
  /// Constructor
  RogueDataReceiver();

  /// Default destructor
  ~RogueDataReceiver() = default;

  void acceptFrame(std::shared_ptr<rogue::interfaces::stream::Frame> frame);

  // void closeCSV() { csv.close(); }

private:
  /// Counter to keep track of the event number
  int event_index_{0};

  ///
  std::vector<uint8_t> channels;
  // void parseFrame(byte data);

  /*
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

  std::ofstream csv;*/

}; // RogueDataReceiver
} // namespace rogue

#endif // 
