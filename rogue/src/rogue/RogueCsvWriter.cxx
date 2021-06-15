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

  // Extract the sample blocks from the file
  std::vector<SampleBlock> blocks = parser->parse(frame);

  // If there aren't any sample blocks, skip the rest of processing
  if (blocks.empty())
    return;

  // Write out the blocks to csv
  for (auto &block : blocks) {
    int pchannel =
        (5 * 128 - 1) - (block.apv * 128 + (128 - 1) - block.channel);
    csv << block.rce << "," << block.feb << "," << block.hybrid << ","
        << block.apv << "," << block.channel << "," << pchannel << ","
        << block.samples[0] << "," << block.samples[1] << ","
        << block.samples[2] << "," << block.samples[3] << ","
        << block.samples[4] << "," << block.samples[5] << "," << block.error
        << "," << block.head << "," << block.tail << "," << block.filter
        << std::endl;
  }
}

void RogueCsvWriter::open(const std::string &file_path) {
  csv.open(file_path);
  csv << "RCE,FEB,Hybrid,APV,channel,pchannel,sample0,sample1,sample2,"
         "sample3,sample4,sample5,error,head,tail,filter"
      << std::endl;
}

void RogueCsvWriter::close() { csv.close(); }
} // namespace rogue
