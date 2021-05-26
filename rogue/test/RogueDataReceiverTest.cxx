#include "catch.hpp"
#include <iostream>

//---< Rogue >---//
#include "rogue/utilities/fileio/StreamReader.h"

//---< rogue >---//
#include "rogue/RogueDataReceiver.h"

namespace rogue {

TEST_CASE("Rogue reader", "[rogue]") {

  // Instantiate the reader used to stream frames from a file created
  // by Rogue
  std::shared_ptr<rogue::utilities::fileio::StreamReader> reader
	  = std::make_shared<rogue::utilities::fileio::StreamReader>();

  // Instantiate the receiver used to process the frames
  std::shared_ptr<RogueDataReceiver> receiver 
	  = std::make_shared<RogueDataReceiver>();
}
}
