#ifndef PROCESSING_PROCESS_H
#define PROCESSING_PROCESS_H

#include "processing/config/Parameters.h"
#include "processing/Exception.h"

// STL
#include <map>
#include <memory>
#include <vector>

//---< Rogue >---//
#include "rogue/Helpers.h" 
#include "rogue/utilities/fileio/StreamReader.h"

//---< rogue >---//
#include "rogue/RogueCsvWriter.h"

namespace processing {

class Processor;

/**
 * @class Process
 * @brief Class which represents the process under execution.
 */
class Process {
 public:
  /**
   * Class constructor.
   * @param configuration Parameters to configure process with
   */
  Process(const processing::config::Parameters &configuration);

  /**
   * Class Destructor
   *
   * Cleans up sequence of EventProcessors.
   * These processors were created by ConfigurePython and should be deleted.
   */
  ~Process();

  /**
   * Get the frequency with which the event information is printed.
   * @return integer log frequency (negative if turned off)
   */
  int getLogFrequency() const { return logFrequency_; }

  /**
   * Run the process.
   */
  void run();

 private:

  /// rogue reader
  std::shared_ptr<rogue::utilities::fileio::StreamReader> rogue_reader_;

  /// CSV writer for rogue data
  std::shared_ptr<rogue::RogueCsvWriter> receiver_;

  /// The parameters used to configure this class.
  processing::config::Parameters config_; 

  /// The run mode.  This is used to determine which data stream is used.
  std::string mode_{"rogue"}; 

  /** Limit on events to process. */
  int eventLimit_;

  /** The frequency with which event info is printed. */
  int logFrequency_;

  /** Integer form of logging level to print to terminal */
  int termLevelInt_;

  /** Integer form of logging level to print to file */
  int fileLevelInt_;

  /** Ordered list of EventProcessors to execute. */
  std::vector<Processor *> sequence_;

  /** List of input files to process.  May be empty if this Process will
   * generate new events. */
  std::vector<std::string> input_files_;

  /** List of output file names.  If empty, no output file will be created. */
  std::vector<std::string> output_files_;

  /** Compression setting to pass to output files
   *
   * Look at the documentation for the TFile constructor if you
   * want to learn more details. Essentially,
   * setting = 100*algo + level
   * with algo = 0 being the global default.
   */
  int compressionSetting_;
};

/**
 * A handle to the current process
 * Used to pass a process from ConfigurePython
 * to ldmx-app.
 */
typedef std::unique_ptr<Process> ProcessHandle;
}  // namespace processing

#endif
