/*
 * @file TupleMaker.h
 * @brief Utility used to create ROOT ntuples. 
 * @author Omar Moreno, SLAC National Accelerator Laboratory
 */

#include "TupleMaker.h"

TupleMaker::TupleMaker(std::string file_name, std::string tree_name) { 

    file = new TFile(file_name.c_str(), "RECREATE");

    tree = new TTree(tree_name.c_str(), tree_name.c_str());   
    
}

TupleMaker::~TupleMaker() { 
    delete file; 
    delete tree; 
}

void TupleMaker::addVariable(std::string variable_name) { 
    
    // Set the default value of the variable to something unrealistic 
    variables[variable_name] = -9999;
    
    // Add a leaf to the ROOT tree and set its address to the address of the 
    // newly created variable. 
    tree->Branch(variable_name.c_str(), &variables[variable_name], (variable_name + "/D").c_str()); 
}

void TupleMaker::addVector(std::string variable_name) { 
    vectors[variable_name] = {}; 
    tree->Branch(variable_name.c_str(), &vectors[variable_name]); 
}

void TupleMaker::addToVector(std::string variable_name, double value) {
    vectors[variable_name].push_back(value); 
}

bool TupleMaker::hasVariable(std::string variable_name) { 
    
    auto search = variables.find(variable_name); 
    if (search != variables.end()) return true; 

    return false; 
}

void TupleMaker::close() { 
    file->Write();
    file->Close(); 
}


void TupleMaker::fill() { 
    
    // Fill the event with the current variables.
    tree->Fill();
}
