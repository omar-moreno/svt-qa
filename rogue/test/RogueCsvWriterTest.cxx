#include "catch.hpp"
#include <iostream>

//---< Rogue >---//
#include "rogue/Helpers.h"
#include "rogue/utilities/fileio/StreamReader.h"

//---< rogue >---//
#include "rogue/RogueCsvWriter.h"

namespace rogue {

TEST_CASE("CSV Writer", "[rogue]") {

  // Instantiate the reader used to stream frames from a file created
  // by Rogue
  std::shared_ptr<rogue::utilities::fileio::StreamReader> reader
	  = std::make_shared<rogue::utilities::fileio::StreamReader>();

  // Instantiate the receiver used to process the frames
  std::shared_ptr<RogueCsvWriter> receiver 
  	  = std::make_shared<RogueCsvWriter>();
  
  // Now connect the two streams
  rogueStreamConnect(reader, receiver);

  // Open the CSV file to write the data to
  receiver->open("test.csv"); 

  // Open the file for reading
  reader->open("");  

  // Close the streams
  reader->closeWait(); 
  
  receiver->close(); 
}
}
