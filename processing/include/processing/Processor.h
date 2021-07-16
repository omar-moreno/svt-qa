
#ifndef PROCESSING_PROCESSOR_H
#define PROCESSING_PROCESSOR_H

//---< C++ StdLib >---//
#include <string>

//---< processing >---//
#include "processing/config/Parameters.h"

namespace processing {

// Forward declarations
class Process;
class Processor;
// class TrackerEvent;
// class Phys2019SvtEvent;
// class TTree;

/** Typedef for ProcessorFactory use. */
typedef Processor *ProcessorMaker(const std::string &name, Process &process);

/**
 * @class Processor
 * @brief Base class for all event processing components
 */
class Processor {

public:
  /**
   * Class constructor.
   * @param name Name for this instance of the class.
   * @param process The Process class associated with Processor, provided by the
   * framework.
   *
   * @note The name provided to this function should not be
   * the class name, but rather a logical label for this instance of
   * the class, as more than one copy of a given class can be loaded
   * into a Process with different parameters.  Names should not include
   * whitespace or special characters.
   */
  Processor(const std::string &name, Process &process);

  /// Destructor
  virtual ~Processor() = default;

  /**
   * Callback for the Processor to configure itself from the
   * given set of parameters.
   *
   * The parameters a processor has access to are the member variables
   * of the python class in the sequence that has className equal to
   * the Processor class name.
   *
   * @param parameters Parameters for configuration.
   */
  virtual void configure(processing::config::Parameters &parameters) {}

  /**
   * @param event The Event to process.
   * Process the event and put new data products into it.
   */
  // virtual void process(TrackerEvent* event) = 0;
  // virtual void process(Phys2019SvtEvent *event) = 0;

  /**
   * Callback for the Processor to take any necessary
   * action when the processing of events starts, such as
   * creating histograms.
   */
  // virtual void initialize(TTree *tree) = 0;

  /**
   * Callback for the Processor to take any necessary
   * action when the processing of events finishes, such as
   * calculating job-summary quantities.
   */
  virtual void finalize(){};

  /**
   * Callback for the Processor to configure itself from the given set of
   * parameters.
   * @param parameters ParameterSet for configuration.
   */
  // virtual void configure(const ParameterSet& parameters) {
  //}

  /**
   * Internal function which is part of the ProcessorFactory machinery.
   * @param classname The class name of the processor.
   * @param classtype The class type of the processor (1 for Producer, 2 for
   * Analyzer).
   */
  static void declare(const std::string &classname, ProcessorMaker *);

protected:
  /** Handle to the Process. */
  Process &process_;

private:
  /** The name of the Processor. */
  std::string name_;
};

} // namespace processing

/**
 * @def DECLARE_PROCESSOR(CLASS)
 * @param CLASS The name of the class to register, which must not be in a
 * namespace.
 * @brief Macro which allows the framework to construct a producer given its
 * name during configuration.
 * @attention Every Producer class must call this macro in the associated
 * implementation (.cxx) file.
 */
#define DECLARE_PROCESSOR(CLASS)                                               \
  processing::Processor *CLASS##_make(const std::string &name,                 \
                                      processing::Process &process) {          \
    return new CLASS(name, process);                                           \
  }                                                                            \
  __attribute__((constructor(1000))) static void CLASS##_declare() {           \
    processing::Processor::declare(#CLASS, &CLASS##_make);                     \
  }

#endif
