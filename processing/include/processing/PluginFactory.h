
#ifndef PROCESSING_PLUGINFACTORY_H
#define PROCESSING_PLUGINFACTORY_H

#include "processing/Processor.h"

//---< C++ >---//
#include <map>
#include <set>
#include <vector>

namespace processing {

/**
 * @class PluginFactory
 * @brief Singleton module factory that creates Processor objects.
 */
class PluginFactory {
public:
  /**
   * Get the factory instance.
   * @return The factory.
   */
  static PluginFactory &getInstance() { return theFactory_; }

  /**
   * Register the event processor.
   * @param classname The name of the class associated with processor.
   * @param classtype The type of class associated with processor.
   * @param maker TODO.
   */
  void registerProcessor(const std::string &classname, int classtype,
                         ProcessorMaker *maker);

  /**
   * Get the classes associated with the processor.
   * @return a vector of strings corresponding to processor classes.
   */
  std::vector<std::string> getProcessorClasses() const;

  /**
   * Get the class type for the event processor.
   * @param TODO.
   */
  int getProcessorClasstype(const std::string &) const;

  /**
   * Make an event processor.
   * @param classname Class name of event processor.
   * @param moduleInstanceName TODO.
   * @param process The process handle
   */
  Processor *createProcessor(const std::string &classname,
                             const std::string &moduleInstanceName,
                             Process &process);

  /**
   * Load a library.
   * @param libname The library to load.
   */
  void loadLibrary(const std::string &libname);

private:
  /**
   * Constructor
   */
  PluginFactory() = default;

  /**
   * @struct PluginInfo
   * @brief info container to hold classname, class type and maker.
   */
  struct PluginInfo {
    std::string classname;
    int classtype;
    ProcessorMaker *ep_maker;
  };

  /** A map of names to processor containers. */
  std::map<std::string, PluginInfo> moduleInfo_;

  /** A set of names of loaded libraries. */
  std::set<std::string> librariesLoaded_;

  /** Factory for creating the plugin objects. */
  static PluginFactory theFactory_;
};

} // namespace processing

#endif
