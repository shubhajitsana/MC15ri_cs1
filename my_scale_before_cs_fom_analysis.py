#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys
import numpy as np

# get input/output path/file from the command line
# 1st 4 are for input and next 4 are for output
input_filename = sys.argv[1]
output_filename = sys.argv[2]
print(f"Input file is {input_filename}")
print(f"Output file is {output_filename}")

scale = 8779.6/200

# Loading input root file and creating new root file
inFile = ROOT.TFile.Open(f"{input_filename}")
inTree = inFile.Get('tree')

outFile = ROOT.TFile.Open(f"{output_filename}","RECREATE"); 
outTree = inTree.CloneTree(0)
print(f"Scaling from {input_filename} and saving to {output_filename} \
with scale factor {scale}")

# Scaling the file
number_of_selected_event = 0       # To count and print the number of selected events per file
for iEvent in range(inTree.GetEntries()):
    rand = ROOT.gRandom.Rndm()
    num = 1/scale
    if rand>num:    #shouldn't be "<" symbol bcz "continue" BREAKS one iteration
        continue     
    inTree.GetEntry(iEvent)
    outTree.Fill()
    number_of_selected_event += 1

print(f"Afer scaling {number_of_selected_event} events has been stored in {output_filename}")
outTree.AutoSave()
outFile.Close()