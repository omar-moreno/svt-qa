#include "rogue/RogueDataReceiver.h"

//---< C++ >---//
//#include <algorithm>
//#include <bitset>
//#include <cstdlib>
#include <iostream>

//---< rogue >---//
#include "rogue/interfaces/stream/Frame.h"
#include "rogue/interfaces/stream/FrameIterator.h"

namespace ris = rogue::interfaces::stream;

typedef unsigned __int128 uint128_t;

namespace rogue {

RogueDataReceiver::RogueDataReceiver() {
  
  
  for (int index{0}; index < 16; ++index) channels.push_back(0x10 + index);
  for (const auto& value : channels) std::cout <<unsigned(value) << std::endl;
  //csv.open("test.csv");
  //csv << "Event, RCE, FEB, Hybrid, APV, channel, sample0, sample1, sample2, "
  //       "sample3, sample4, sample5\n";
}

void RogueDataReceiver::acceptFrame(std::shared_ptr<ris::Frame> frame) {

  // Check for a frame error
  if (frame->getError()) {
    //std::cerr << "[ RogueDataReceiver ]: Frame error!" << std::endl;
    return;
  }

  if (frame->getChannel() == 0) {
    // std::cout << "[ RogueDataReceiver ]: Config channel found." << std::endl;
    return;
  } /*else if (std::find(channels.begin(), channels.end(), frame->getChannel()) !=
             channels.end()) {

    event_index += 1;
    // std::cout << "Channel: " <<  unsigned(frame->getChannel()) << std::endl;
    // std::cout << "[ RogueDataReceiver ]: Payload size: " << frame->getPayload()
    // << std::endl; std::cout << "[ RogueDataReceiver ]: Parsing data " <<
    // std::endl;

    auto it = frame->beginRead();

    for (int i{0}; i < frame->getPayload(); ++i) {
        printf("Location %i = 0x%x\n", i, *it);
        it += 1;
    }

    it = frame->beginRead();
    uint32_t event_count;
    ris::fromFrame(it, 4, &event_count);
    // std::cout << "[ RogueDataReceiver ]: Events: " << event_count << std::endl;

    uint8_t data_dpm;
    ris::fromFrame(it, 1, &data_dpm);
    // std::cout << "[ RogueDataReceiver ]: Data DPM: " << unsigned(data_dpm) <<
    // std::endl;

    it += 3;

    uint64_t time_stamp;
    ris::fromFrame(it, 8, &time_stamp);
    // std::cout << "[ RogueDataReceiver ]: Time stamp: " << time_stamp <<
    // std::endl;

    it = frame->endRead() - 16;
    uint16_t sample_count;
    ris::fromFrame(it, 2, &sample_count);
    // std::cout << "[ RogueDataReceiver ]: Sample Count: " << sample_count <<
    // std::endl;

    int n_samples = (frame->getPayload() - 32) / 16;
    // std::cout << "[ RogueDataReceiver ]: N samples: " << n_samples <<
    // std::endl;

    it = frame->beginRead() + 16;
    std::vector<uint16_t> samples = {0, 0, 0, 0, 0, 0};
    uint8_t rce;
    uint8_t feb;
    uint16_t tail;

    for (int isample{0}; isample < n_samples; ++isample) {
      // std::cout << "[ RogueDataReceiver ]: Sample set: " << isample <<
      // std::endl;
      for (int nsample{0}; nsample < 6; ++nsample) {
        ris::fromFrame(it, 2, &samples[nsample]);
        // std::cout << "[ RogueDataReceiver ]: Sample " << nsample << ": " <<
        // samples[nsample] << std::endl;
      }
      ris::fromFrame(it, 1, &rce);
      // std::cout << "[ RogueDataReceiver ]: RCE: " << unsigned(rce) <<
      // std::endl;
      ris::fromFrame(it, 1, &feb);
      // std::cout << "[ RogueDataReceiver ]: FEB: " << unsigned(feb) <<
      // std::endl;
      ris::fromFrame(it, 2, &tail);
      if (getField(tail, 14, 14) == 1)
        continue;
      csv << event_count << "," << unsigned(rce) << ", " << unsigned(feb)
          << ", " << getField(tail, 11, 10) << ", ";
      csv << getField(tail, 9, 7) << ", " << getField(tail, 6, 0) << ", ";
      for (int nsample{0}; nsample < 6; ++nsample) {
        csv << samples[nsample] << ", ";
      }
      csv << "\n";*/
      /*std::cout << "[ RogueDataReceiver ]: Tail: " << tail << std::endl;
      std::cout << "[ RogueDataReceiver ]: APV ch: " << getField(tail, 6, 0) <<
      std::endl; std::cout << "[ RogueDataReceiver ]: APV add: " << getField(tail,
      9, 7) << std::endl; std::cout << "[ RogueDataReceiver ]: Hybrid: " <<
      getField(tail, 11, 10) << std::endl; std::cout << "[ RogueDataReceiver ]:
      RCE Error: " << getField(tail, 12, 12) << std::endl; std::cout << "[
      RogueDataReceiver ]: Tail: " << getField(tail, 13, 13) << std::endl;
      std::cout << "[ RogueDataReceiver ]: Head: " << getField(tail, 14, 14) <<
      std::endl;
      std::cout << "[ RogueDataReceiver ]: Filter: " << getField(tail, 15, 15) <<
      std::endl;
    }
  }*/
}
} // namespace rogue
/*



}*/

/*
uint8_t data[frame->getPayload()];
std::copy(frame->beginRead()+16, frame->endRead()-16, data);
std::cout << "Lenght: " << sizeof(data)/sizeof(uint8_t) << std::endl;
for (int i{0}; i < 24; i+=2) {
    uint16_t sample = (data[i+1] << 8) | data[i];
    std::cout << "sample: " << std::bitset<16>(sample) << std::endl;
    //std::cout << "index ( " << i << " ) " << std::bitset<8>(data[i]) <<
std::endl;
}*/
// uint32_t event_count = uint32_t((data[3] << 24) | (data[2] << 16) | (data[1]
// << 8) | data[0]); uint8_t dpm = uint8_t(data[4]);

// std::cout << "events: " << event_count << std::endl;
// std::cout << "dpm: " << std::bitset<8>(data[4]) <<  std::endl;
// std::cout << "Data DPM: " << getField(*header, 39, 32) << std::endl;
// std::cout << "Event count: " << getField(*header, 31, 0) << std::endl;

/*for (uint32_t x{0}; x < frame->getPayload(); ++x) {
    std::cout << "Location: " << *it << std::endl;
    it++;
}*/
