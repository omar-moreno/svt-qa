//---< C++ >---//
#include <cstdlib>
#include <exception>
#include <iostream>
#include <signal.h>
#include <string>

//---< boost >---//
#include <boost/program_options.hpp>

//---< processing >---//
#include "processing/Exception.h"
#include "processing/Process.h"
#include "processing/config/ConfigurePython.h"

using namespace std;

int main(int argc, char **argv) {

  boost::program_options::variables_map var_map;
  try {
    // NOTE: The same options needs to be declared here and in positionals
    boost::program_options::options_description desc("Available options");
    desc.add_options()("help", "Print usage")(
        "config, c", boost::program_options::value<vector<string>>(),
        "Configuration");

    // Configure the positional arguments.
    // The config option is configured to take any number of arguments.
    // This is needed to allow passing of the python script and any
    // commands passed to it.
    boost::program_options::positional_options_description positionals;
    positionals.add("config", -1);

    // Parse command line arguments
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .positional(positionals)
            .options(desc)
            .run(),
        var_map);
    boost::program_options::notify(var_map);

    if (var_map.count("help") || !var_map.count("config")) {
      cerr << desc << endl;
      return EXIT_FAILURE;
    }
  } catch (exception &e) {
    cerr << "Error!: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  vector<string> config{var_map["config"].as<vector<string>>()};
  cout << "-----< svt-qa > Loading configuration from " << config[0] << " -----"
       << endl;

  processing::ProcessHandle p;
  try {
    processing::config::ConfigurePython cfg(config);
    p = cfg.makeProcess();
  } catch (processing::Exception &e) {
    cerr << "Configuration Error [" << e.name() << "] : " << e.message()
         << endl;
    cerr << "  at " << e.module() << ":" << e.line() << " in " << e.function()
         << endl;
    cerr << "Stack trace: " << endl << e.stackTrace();
    return EXIT_FAILURE;
  }

  cout << "-----< svt-qa > Configuration load complete -----" << endl;

  // If Ctrl-c is used, immediately exit the application.
  struct sigaction act;
  memset(&act, '\0', sizeof(act));
  if (sigaction(SIGINT, &act, NULL) < 0) {
    perror("sigaction");
    return 1;
  }

  cout << "-----< svt-qa > Starting event processing -----" << endl;

  try {
    p->run();
  } catch (processing::Exception &e) {
    cerr << "-----< svt-qa > [" << e.name() << "] : " << e.message() << "\n"
         << "  at " << e.module() << ":" << e.line() << " in " << e.function()
         << "\nStack trace: " << std::endl
         << e.stackTrace();
  }
  cout << "-----< svt-qa > Event processing complete  -------" << endl;

  return EXIT_SUCCESS;
}
