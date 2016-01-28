#
#    @file root_plotting_utils.py
#    @brief Class containing a set of ROOT plotting utilites 
#    @author Omar Moreno <omoreno1@ucsc.edu>
#            Santa Cruz Institute for Particle Physics
#            University of California, Santa Cruz
#    @date December 14, 2014
#
import re
import os
import subprocess

from fnmatch import fnmatch

from ROOT import TFile
from ROOT import TCanvas
from ROOT import gROOT
from ROOT import TDatime

class RootPlottingUtils:
    
    def __init__(self):
        self.name = ''

    def save_str(self):
        if self.name == "": return ""
        else: return "_" + self.name
    
    def save_to_pdf(self, file, ref_file=None, regexp=None):
        
        gROOT.SetBatch()
        
        # Check that the file is still opened.  If not, return
        if not file.IsOpen():
             print "[ RootPlottingUtils ]: File is not opened."
             return


        # Check that the ref file is still opened if needed.  If not, return
        if ref_file != None:
            if not ref_file.IsOpen():
                print "[ RootPlottingUtils ]: Reference file is not opened."
                return
        
         # create canvas
        canvas = TCanvas("canvas", "canvas", 200, 200)
         
         # Iterate over all objects in the ROOT file
        for file_key in file.GetListOfKeys():
            if file_key.IsFolder():
                file.cd(file_key.GetName())
                output_file_name = str(file_key.GetDatime().GetDate()) + "_" + file_key.GetName() + self.save_str() + ".pdf"
                canvas.Print(output_file_name + "[")
                count = 0
                for file_sub_key in file_key.ReadObj().GetListOfKeys():
                    if regexp != None:
                        m = re.match(regexp,file_sub_key.ReadObj().GetName())
                        if m == None:
                            continue
                    ref_obj = None
                    if ref_file != None:
                        ref_path = os.path.join(file_key.ReadObj().GetName(),file_sub_key.ReadObj().GetName())
                        print "[ RootPlottingUtils ]: Find reference plot " + ref_path
                        ref_obj = ref_file.Get(ref_path)
                        if ref_obj == None:
                            print "[ RootPlottingUtils ]: No reference plot found for " + file_sub_key.ReadObj().GetName()
                            print "[ RootPlottingUtils ]: Exit."
                            return
                        else:
                            print "[ RootPlottingUtils ]: Found reference plot for " + file_sub_key.ReadObj().GetName()
                            
                    print "[ RootPlottingUtils ]: Saving plot " + file_sub_key.ReadObj().GetName()
                    if fnmatch(file_sub_key.GetClassName(), "TH2*"): 
                        file_sub_key.ReadObj().Draw("colz")
                    elif fnmatch(file_sub_key.GetClassName(), "*Graph*"):
                        if ref_obj != None:
                            ref_obj.SetLineColor(1)
                            ref_obj.SetMarkerColor(1)
                            ref_obj.Draw("Ap")
                            file_sub_key.ReadObj().SetLineColor(2)
                            file_sub_key.ReadObj().SetMarkerColor(2)
                            file_sub_key.ReadObj().Draw("p,same")
                        else:
                            file_sub_key.ReadObj().Draw("Ap")
                    else:
                        if ref_obj != None:
                            ref_obj.SetLineColor(1)
                            ref_obj.SetFillColor(1)
                            ref_obj.SetFillStyle(3001)
                            ref_obj.Draw("")
                            file_sub_key.ReadObj().SetLineColor(2)
                            file_sub_key.ReadObj().Draw("same")
                        else:
                            file_sub_key.ReadObj().Draw("")
                    canvas.Print(output_file_name + "(")
                    count += 1
                    canvas.Clear()
                
                canvas.Print(output_file_name + "]")
                # this is a hack
                if count == 0:
                    print "[ RootPlottingUtils ]: Delete empty file " + output_file_name
                    subprocess.call("rm " + output_file_name, shell=True)
                file.cd()
                
                
