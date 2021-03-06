/**
 *  @file   svt_qa.cxx
 *  @brief  App used to process SVT EVIO events and Data events
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   May 5, 2014
 */

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

//-------------//
//   SVT QA    //
//-------------//
#include "ConfigurePython.h"
#include "Process.h"

using namespace std; 

void displayUsage(); 

int main(int argc, char **argv) {
   
    if (argc < 2) {
        displayUsage(); 
        return EXIT_FAILURE;
    }

    int ptrpy = 1;
    for (ptrpy = 1; ptrpy < argc; ptrpy++) {
        if (strstr(argv[ptrpy], ".py"))
            break;
    }
    
    if (ptrpy == argc) {
        displayUsage(); 
        printf("  ** No python script provided. **\n");
        return EXIT_FAILURE;
    }

    try {
        std::cout << "---- [ svt-qa ]: Loading configuration --------" << std::endl;
        
        ConfigurePython cfg(argv[ptrpy], argv + ptrpy + 1, argc - ptrpy);

        std::cout << "---- [ svt-qa ]: Configuration load complete  --------" << std::endl;

        Process* p = cfg.makeProcess();
        
        std::cout << "---- [ svt-qa ]: Process initialized.  --------" << std::endl;

        // If Ctrl-c is used, immediately exit the application.
        struct sigaction act;
        memset (&act, '\0', sizeof(act));
        if (sigaction(SIGINT, &act, NULL) < 0) {
            perror ("sigaction");
            return 1;
        }

        // See comment above for reason why this code is commented out.
        /* Use the sa_sigaction field because the handles has two additional parameters */
        //act.sa_sigaction = &softFinish;

        /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
        //act.sa_flags = SA_SIGINFO;

        std::cout << "---- [ svt-qa ]: Starting event processing --------" << std::endl;
        
        p->run();
        
        std::cout << "---- [ svt-qa ]: Event processing complete  --------" << std::endl;

    } catch (exception& e) { 
        //std::cerr << "Error! [" << e.name() << "] : " << e.message() << std::endl;
        //std::cerr << "  at " << e.module() << ":" << e.line() << " in " << e.function() << std::endl;
    
    } 

    return EXIT_SUCCESS;
}

void displayUsage() {
    printf("Usage: svt-qa [application arguments] {configuration_script.py} [arguments to configuration script]\n");
}
