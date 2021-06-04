#ifndef ROGUE_ROGUEFILEREADER_H
#define ROGUE_ROGUEFILEREADER_H

//---< C++ >---//
#include <memory>
#include <vector>

// Forward declarations
namespace rogue::interfaces::stream {
class Frame;
} // namespace rogue::interfaces::stream

namespace rogue {

struct SampleBlock {
  int rce{-9999};
  int feb{-9999};
  int hybrid{-9999};
  int apv{-9999};
  int channel{-9999};
  int error{-9999};
  int head{-9999};
  int tail{-9999};
  int filter{-9999};
  std::vector<int> samples{0, 0, 0, 0, 0, 0};
};

class RogueFileParser {

public:
  /// Constructor
  RogueFileParser() = default;

  /// Destructor
  ~RogueFileParser() = default;

  /**
   */
  std::vector<SampleBlock>
  parse(std::shared_ptr<rogue::interfaces::stream::Frame> frame);
};
} // namespace rogue

#endif // ROGUE_ROGUEFILEREADER_H
