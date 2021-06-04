#include "rogue/RogueCsvWriter.h"

#include <iostream>

//---< rogue >---//
#include "rogue/interfaces/stream/Frame.h"
#include "rogue/interfaces/stream/FrameIterator.h"

namespace rogue {

RogueCsvWriter::RogueCsvWriter() {
  parser = std::make_unique<RogueFileParser>();
}

void RogueCsvWriter::acceptFrame(
    std::shared_ptr<rogue::interfaces::stream::Frame> frame) {

  // Skip the frame if an error is found.
  if (frame->getError())
    return;

  //
  // Channels
  // 0: Configuration i.e. dump of the yaml config
  // > 0x10 + 1: The channels open to the DPMs for receiving data.
  //
  // For now, skip the configuration
  if (frame->getChannel() == 0)
    return;

  // A maximum of 16 channels are opened to the DPMs
  // TODO: This should throw an exception instead.
  if (frame->getChannel() >= 0x10 + 16)
    return;

  parser->parse(frame);
}
} // namespace rogue
