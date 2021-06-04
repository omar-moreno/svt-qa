#ifndef ROGUE_ROGUECSVWRITER_H
#define ROGUE_ROGUECSVWRITER_H

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

//---< C++ >---//
#include <memory>

//---< rogue >---//
#include "rogue/RogueFileParser.h"
#include "rogue/interfaces/stream/Slave.h"

// Forward declarations
namespace rogue::interfaces::stream {
class Frame;
} // namespace rogue::interfaces::stream

namespace rogue {

class RogueCsvWriter : public rogue::interfaces::stream::Slave {

public:
  /// Constructor
  RogueCsvWriter();

  /// Default destructor
  ~RogueCsvWriter() = default;

  void acceptFrame(std::shared_ptr<rogue::interfaces::stream::Frame> frame);

private:
  // Parser used to extract samples
  std::unique_ptr<RogueFileParser> parser;
}; // RogueCsvWriter
} // namespace rogue

#endif // ROGUE_ROGUECSVWRITER_H
