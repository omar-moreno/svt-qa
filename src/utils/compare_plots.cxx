

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <getopt.h>
#include <iostream>
#include <cstdlib>
#include <list>
#include <fstream>
#include <map>

#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TCanvas.h>

using namespace std;

int main(int argc, char **argv) {
    
    string root_file_list_name;

    // Parse all the command line arguments. If there are no valid command line
    // arguments passed, print the usage and exit.
    static struct option long_options[] = {
        {"root_list", required_argument, 0, 'l' },
        { 0, 0, 0, 0 }
    };

    int option_index = 0;
    int option_char; 
    while ((option_char = getopt_long(argc, argv, "l:", long_options, &option_index)) != -1) {
        switch (option_char) {
            case 'l': 
                root_file_list_name = optarg;
                break;
        }
    }

    if (root_file_list_name.empty()) { 
        cerr << "Please specify a list of ROOT files to process." << endl;
        return EXIT_FAILURE;
    }

    // Create the list of files to process
    //list<string> root_files; 
    list< TFile* > root_files;
    string root_file; 
    ifstream root_file_list(root_file_list_name.c_str(), ifstream::in);
    if (!root_file_list.is_open()) { 
        cerr << "Failed to open file " << root_file_list_name << endl;
        return EXIT_FAILURE;
    }

    while (root_file_list >> root_file) { 
        root_files.push_back(new TFile(root_file.c_str())); 
        if (root_files.back()->IsZombie()) { 
            cout << "Failed to open root file " << root_file << endl;
            return EXIT_FAILURE;
        }
    } 
    root_file_list.close();

    map <string, vector<TH1*>> histogram_map; 

    // Loop over all of the ROOT files and create the plot maps
    //list<string>::iterator root_file_it = root_files.begin();
    list < TFile* >::iterator root_file_it = root_files.begin();
    for (root_file_it; root_file_it != root_files.end(); ++root_file_it) { 
        cout << "[ ]: Processing file: " << (*root_file_it)->GetName() << endl;
            
        TIter next((*root_file_it)->GetListOfKeys());
        while (TKey *key = (TKey*) next()) { 
            cout << "[ ]: Key: " << key->GetName() << endl;
        
            if (key->IsFolder()) { 
                (*root_file_it)->cd(key->GetName());
                TIter next_sub(((TDirectory*) key->ReadObj())->GetListOfKeys());
                while (TKey *sub_key = (TKey*) next_sub()) { 
                    cout << "[ ]: Sub key: " << sub_key->GetName() << endl;
                        histogram_map[sub_key->GetName()].push_back(((TH1*) sub_key->ReadObj()));
                    cout << "Map size: " << histogram_map.size() << endl;
                    cout << "Vector size: " << histogram_map[sub_key->GetName()].size() << endl;
                }
            }
        }
    }
       
    TCanvas* canvas = new TCanvas("canvas", "canvas", 500, 500); 
    map <string, vector<TH1*>>::iterator histogram_it = histogram_map.begin();
    canvas->Print("test.pdf[");
    for (histogram_it; histogram_it != histogram_map.end(); histogram_it++) { 
        int color_index = 1;
        histogram_it->second[0]->SetLineColor(color_index);
	histogram_it->second[0]->GetYaxis()->SetRangeUser(0, 500);
        ++color_index;
	histogram_it->second[0]->Draw();

        for (int hist_n = 1; hist_n < histogram_it->second.size(); hist_n++) { 
           histogram_it->second[hist_n]->SetLineColor(color_index);
	   ++color_index;
	   histogram_it->second[hist_n]->GetYaxis()->SetRangeUser(0, 500);
           histogram_it->second[hist_n]->Draw("same");  
            cout << "Draw : " << hist_n << endl;
       } 
        canvas->Print("test.pdf(");
    }
    canvas->Print("test.pdf]");
    return EXIT_SUCCESS;
}


