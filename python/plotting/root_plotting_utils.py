#
#    @file root_plotting_utils.py
#    @brief Class containing a set of ROOT plotting utilites 
#    @author Omar Moreno <omoreno1@ucsc.edu>
#            Santa Cruz Institute for Particle Physics
#            University of California, Santa Cruz
#    @date December 14, 2014
#

from fnmatch import fnmatch

from ROOT import TFile
from ROOT import TCanvas
from ROOT import gROOT
from ROOT import TDatime

class RootPlottingUtils:
    
    def save_to_pdf(self, file):
        
        gROOT.SetBatch()
        
        # Check that the file is still opened.  If not, return
        if not file.IsOpen():
             print "[ RootPlottingUtils ]: File is not opened."
             return
         
         # 
        canvas = TCanvas("canvas", "canvas", 200, 200)
         
         # Iterate over all objects in the ROOT file
        for file_key in file.GetListOfKeys():
            if file_key.IsFolder():
                file.cd(file_key.GetName())
                output_file_name = str(file_key.GetDatime().GetDate()) + "_" + file_key.GetName() + ".pdf"
                canvas.Print(output_file_name + "[")
                for file_sub_key in file_key.ReadObj().GetListOfKeys():
                    print "[ RootPlottingUtils ]: Saving plot " + file_sub_key.ReadObj().GetName()
                    if fnmatch(file_sub_key.GetClassName(), "TH2*"): 
                        file_sub_key.ReadObj().Draw("colz")
                    elif fnmatch(file_sub_key.GetClassName(), "*Graph*"):
                        file_sub_key.ReadObj().Draw("Ap")
                    else:
                        file_sub_key.ReadObj().Draw("")
                    canvas.Print(output_file_name + "(")
                    canvas.Clear()
                
                canvas.Print(output_file_name + "]")
                file.cd()
                
                