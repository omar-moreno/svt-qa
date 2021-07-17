
#include "processing/Process.h"
#include "processing/Exception.h"
#include "processing/Processor.h"
#include "processing/PluginFactory.h"
//#include "TFile.h"
//#include "TROOT.h"
#include <iostream>
#include <algorithm> 

//---< rogue >---//
#include "rogue/RogueCsvWriter.h"

namespace processing {

Process::Process(const processing::config::Parameters &configuration) {

  config_ = configuration;

  mode_ = configuration.getParameter<std::string>("mode", "rogue");

  eventLimit_ = configuration.getParameter<int>("maxEvents", -1);
  logFrequency_ = configuration.getParameter<int>("logFrequency", -1);
  compressionSetting_ =
      configuration.getParameter<int>("compressionSetting", 9);

  input_files_ =
      configuration.getParameter<std::vector<std::string>>("input_files", {});
  output_files_ =
      configuration.getParameter<std::vector<std::string>>("output_files", {});

  auto libs{
      configuration.getParameter<std::vector<std::string>>("libraries", {})};
  std::for_each(libs.begin(), libs.end(), [](auto &lib) {
    PluginFactory::getInstance().loadLibrary(lib);
  });

  auto sequence{
      configuration.getParameter<std::vector<processing::config::Parameters>>(
          "sequence", {})};
  if (sequence.empty() &&
      configuration.getParameter<bool>("testingMode", false)) {
    EXCEPTION_RAISE(
        "NoSeq", "No sequence has been defined. What should I be doing?\nUse "
                 "p.sequence to tell me what processors to run.");
  }
  
  for (auto proc : sequence) {
    auto className{proc.getParameter<std::string>("className")};
    auto instanceName{proc.getParameter<std::string>("instanceName")};
    Processor *ep = PluginFactory::getInstance().createProcessor(
        className, instanceName, *this);
    if (ep == 0) {
      EXCEPTION_RAISE(
          "UnableToCreate",
          "Unable to create instance '" + instanceName + "' of class '" +
              className +
              "'. Did you load the library that this class is apart of?");
    }
    ep->configure(proc);
    sequence_.push_back(ep);
  }
}

Process::~Process() {
  for (Processor *ep : sequence_) {
    delete ep;
  }
}

void Process::run() {

  // Counter to keep track of the number of events that have been
  // procesed
  auto n_events_processed{0};

  /// TODO: Switch to using Exception class
  if (input_files_.empty())
    throw std::runtime_error("Please specify files to process.");
 
  if (mode_.compare("rogue") == 0) {
    // Instantiate the reader used to stream frames from a file created 
    // by rogue
    rogue_reader_ = std::make_shared<rogue::utilities::fileio::StreamReader>(); 
    receiver_ = std::make_shared<rogue::RogueCsvWriter>();
    // Now connect the two streams
    rogueStreamConnect(rogue_reader_, receiver_);
  }
  // data_reader = new Phys2019DataReadEvio();
  // Phys2019SvtEvent event;

  int cfile = 0;

  for (auto& ifile : input_files_) {

     if (mode_.compare("rogue") == 0) {
      // Open the file for reading
      receiver_->open(ifile);  
  
      // Open the CSV file to write the data to
      rogue_reader_->open(output_files_[cfile]); 
    
      rogue_reader_->closeWait(); 

      receiver_->close(); 

      ++cfile;

      continue;
     }


    // Open the input file.  If the input file can't be opened, exit
    // the application.
    // if (!data_reader->open(ifile))
    //  throw std::runtime_error("Error! File " + ifile + " cannot be opened.");

    std::cout << "-----< svt-qa >< Process > Processing file " << ifile
              << std::endl;

    // Open the output file where all histograms will be stored.
    // TFile *ofile = new TFile(output_files_[cfile].c_str(), "RECREATE");

    // Create the tree
    // TTree *tree = new TTree("SVT_QA", "Tree containing calibration data.");

    // first, notify everyone that we are starting
    //for (auto module : sequence_) {
      // module->initialize(tree);
    //}

    //while (data_reader->next(&event)) {
    //   for (auto module : sequence_) {
        // module->process(&event);
    //  }
    //}
    //++cfile;

    //for (auto module : sequence_) {
      // module->finalize();
    //}

    // if (ofile) {
    //  ofile->Write();
    //  delete ofile;
    //  ofile = nullptr;
    //}
  }
}
//catch (exception &e) {
//  std::cerr << "---- [ svt-qa ][ Process ]: Error! " << e.what() << std::endl;
//}
} // namespace processing
