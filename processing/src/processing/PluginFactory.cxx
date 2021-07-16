#include "processing/PluginFactory.h"
#include <dlfcn.h>
#include "processing/Processor.h"

processing::PluginFactory processing::PluginFactory::theFactory_
    __attribute((init_priority(500)));

namespace processing {

void PluginFactory::registerProcessor(const std::string& classname,
                                           int classtype,
                                           ProcessorMaker* maker) {
  auto ptr = moduleInfo_.find(classname);
  if (ptr != moduleInfo_.end()) {
    EXCEPTION_RAISE("ExistingProcessorDefinition",
                    "Already have a module registered with the classname '" +
                        classname + "'");
  }
  PluginInfo mi;
  mi.classname = classname;
  mi.classtype = classtype;
  mi.ep_maker = maker;
  moduleInfo_[classname] = mi;
}

std::vector<std::string> PluginFactory::getProcessorClasses() const {
  std::vector<std::string> classes;
  for (auto ptr : moduleInfo_) {
    if (ptr.second.ep_maker != 0) classes.push_back(ptr.first);
  }
  return classes;
}

int PluginFactory::getProcessorClasstype(const std::string& ct) const {
  auto ptr = moduleInfo_.find(ct);
  if (ptr == moduleInfo_.end() || ptr->second.ep_maker == 0) {
    return 0;

  } else {
    return ptr->second.classtype;
  }
}

Processor* PluginFactory::createProcessor(
    const std::string& classname, const std::string& moduleInstanceName,
    Process& process) {
  auto ptr = moduleInfo_.find(classname);
  if (ptr == moduleInfo_.end() || ptr->second.ep_maker == 0) {
    return 0;
  }
  return ptr->second.ep_maker(moduleInstanceName, process);
}

void PluginFactory::loadLibrary(const std::string& libname) {
  if (librariesLoaded_.find(libname) != librariesLoaded_.end()) {
    return;  // already loaded
  }

  void* handle = dlopen(libname.c_str(), RTLD_NOW);
  if (handle == nullptr) {
    EXCEPTION_RAISE("LibraryLoadFailure",
                    "Error loading library '" + libname + "':" + dlerror());
  }

  librariesLoaded_.insert(libname);
}

}  // namespace processing
