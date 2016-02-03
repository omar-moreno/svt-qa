#! /usr/bin/python

import argparse
import os
import sys

from ROOT import TFile

from root_plotting_utils import RootPlottingUtils

# Parse command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input", required=True, help="Input ROOT file")
parser.add_argument("-n", "--name", default="",  help="Name to add to output ROOT file")
parser.add_argument("-r", "--regexp",  help="Regular expression to select histograms")
parser.add_argument("-c", "--cmpfile",  help="Input ROOT file to serve as reference for histograms")
parser.add_argument("-l", "--legend",  help="Space delimited legend list")
args = parser.parse_args()
print args

#if not args.input:
#    print "[ ]: Please specify an input ROOT file"
#    sys.exit(2)

# open input ROOT file
root_file = TFile(str(args.input))
if not root_file.IsOpen():
    print "[ ]: File " + str(args.input) + " couldn't be opened."

# open reference ROOT file
ref_root_file = None
if args.cmpfile != None:
    ref_root_file = TFile(str(args.cmpfile))
    if not ref_root_file.IsOpen():
        print "[ ]: File " + str(args.cmpfile) + " couldn't be opened."

legend = None
if args.legend != None:
    legend = args.legend.split()

root_plotting_utils = RootPlottingUtils()
root_plotting_utils.name = args.name
root_plotting_utils.save_to_pdf(root_file,ref_root_file,legend,args.regexp)
