#include "rogue/RogueFileParser.h"

//---< C++ >---//
#include <iostream>

//---< rogue >---//
#include "rogue/RogueUtils.h"
#include "rogue/interfaces/stream/Frame.h"
#include "rogue/interfaces/stream/FrameIterator.h"

namespace rogue {

std::vector<SampleBlock> RogueFileParser::parse(
    std::shared_ptr<rogue::interfaces::stream::Frame> frame) {

  // Container for all samples
  std::vector<SampleBlock> samples;

  // Get an iterator to the data in the frame.
  auto it{frame->begin()};

  // The event count is stored in the first 32 bits of the frame.
  uint32_t event_count;
  rogue::interfaces::stream::fromFrame(it, 4, &event_count);
  // std::cout << "[ RogueFileParser ]: Event counts: "
  //          << rogue::toBinary<uint32_t>(event_count) << std::endl;

  // The last 128 bits of the frame contain the Event Builder tail.
  // The information in the tail is packed as follows:
  // 11:00 - Number of multisamples
  // 23:12 - Skip count, the number of multisamples that got dumped
  //         due to FIFO backup
  // 25:24 - Unused
  // 26 - ApvBufferAddressSyncError - APV frames didn't have the same addresses
  // 27 - FIFO backup error (skip count will only show up if this is 1)
  // 128:27 - Unused
  it = frame->end() - 16;
  uint16_t eb_tail;
  rogue::interfaces::stream::fromFrame(it, 2, &eb_tail);

  // Get the sample block count from the tail. Each block is composed of 6
  // samples.
  int block_count{eb_tail & ((1 << 11) - 1)};
  // std::cout << "[ RogueFileParser ]: Sample Count: " << block_count <<
  // std::endl;

  // If the sample count is 0, skip processing the rest of the frame.
  if (block_count == 0)
    return samples;

  // Reset the iterator so it points to the beginning of the sample blocks.
  it = frame->begin() + 16;

  // Loop through the sample blocks and extract the values for each block.
  for (auto iblock{0}; iblock < block_count; ++iblock) {

    // Container to hold the sample block information
    SampleBlock block;

    // Each sample block contains 6 samples
    for (auto isample{0}; isample < 6; ++isample) {
      rogue::interfaces::stream::fromFrame(it, 2, &block.samples[isample]);
      // std::cout << "[ RogueFileParser ]: Sample " << isample << ": " <<
      // rogue::toBinary<int>(block.samples[isample]) << std::endl;
    }
    uint16_t tail;
    // Extract information of the sample block origin from the tail
    rogue::interfaces::stream::fromFrame(it, 1, &block.rce);
    rogue::interfaces::stream::fromFrame(it, 1, &block.feb);
    rogue::interfaces::stream::fromFrame(it, 2, &tail);
    block.channel = rogue::getField(tail, 6, 0);
    block.apv = rogue::getField(tail, 9, 7);
    block.hybrid = rogue::getField(tail, 11, 10);
    block.error = rogue::getField(tail, 12, 12);
    block.tail = rogue::getField(tail, 13, 13);
    block.head = rogue::getField(tail, 14, 14);
    block.filter = rogue::getField(tail, 15, 15);

    samples.push_back(block);
  }

  return samples;
}
} // namespace rogue
