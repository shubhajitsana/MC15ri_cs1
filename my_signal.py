#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys

# get input/output path/file from the command line
input_filename = sys.argv[1]
output_filename = sys.argv[2]
print(f"The input file is {input_filename}")
print(f"The output file is {output_filename}")


# Loading input root file and creating new root file
inFile = ROOT.TFile.Open(f"{input_filename}")
inTree = inFile.Get('tree')

outFile = ROOT.TFile.Open(f"{output_filename}","RECREATE"); 
outTree = inTree.CloneTree(0)

# Filtering only Signal Events
k = 0       # To count and print the number of selected signal
j = 0       # To count and print the number of reconstructed event
for iEvent in range(inTree.GetEntries()):
    inTree.GetEntry(iEvent)
    signal = getattr(inTree, 'isSignal')
    j += 1
    if signal==1:
        outTree.Fill()
        k += 1
print(f"There were {j} reconstructed events in {input_filename}")
print(f"There were {k} signal events which have been stored in {output_filename}")
outTree.AutoSave()
outFile.Close()