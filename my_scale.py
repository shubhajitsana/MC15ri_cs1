#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys
import numpy as np

# get input/output path/file from the command line
# 1st 4 are for input and next 4 are for output
input_number = [1, 2, 3, 4]
input_filename = []
for i in input_number:
    input_filename.append(sys.argv[i])
print(f"4 Input files are {input_filename}")

output_number = [5, 6, 7, 8]
output_filename = []
for j in output_number:
    output_filename.append(sys.argv[j])
print(f"4 Output files are {output_filename}")


# Getting scales
infile_uubar = ROOT.TFile.Open(f"{input_filename[0]}")
infile_ddbar = ROOT.TFile.Open(f"{input_filename[1]}")
infile_ssbar = ROOT.TFile.Open(f"{input_filename[2]}")
infile_ccbar = ROOT.TFile.Open(f"{input_filename[3]}")

intree_uubar = infile_uubar.Get('tree')
intree_ddbar = infile_ddbar.Get('tree')
intree_ssbar = infile_ssbar.Get('tree')
intree_ccbar = infile_ccbar.Get('tree')

# Counting number of events of different (qqbar) folder before scaling
NEvent = np.zeros(4)
NEvent[0] = intree_uubar.GetEntries()
NEvent[1] = intree_ddbar.GetEntries()
NEvent[2] = intree_ssbar.GetEntries()
NEvent[3] = intree_ccbar.GetEntries()

NEvent_total = NEvent.sum()
cross_section = np.array([0.4363, 0.1084, 0.10298, 0.3523])

expect_event_total = NEvent/cross_section
lowest_expect_event_total = expect_event_total.min()
print(f"The total events in the given input files are {NEvent_total} . But \
    to include maximum continuum event from this data \
    \n(following relative cross section ratio between u:d:s:c)\
     \n HIGHEST number of total events should be {lowest_expect_event_total}") # this is the number
                            # using which we can include maximum continuum event following their
                            # (relative) cross section ratio - u:d:s:c = 1.61:0.4:0.38:1.3
if lowest_expect_event_total <= NEvent_total:
    New_NEvent_total = np.copy(NEvent)
    for i in range(4):
        New_NEvent_total[i] = lowest_expect_event_total
else:
    print("There is some problem during calculation of scaling factor")
# print(f"New total event is {New_NEvent_total}")
New_NEvent = New_NEvent_total * cross_section
scale = NEvent/New_NEvent                                       # Setting Scale
print(f"Previous total events in u:d:s:c(bar) folder were {NEvent}")
print(f"New total event according to cross section should be {New_NEvent}")
print(f"So scaling factor is {scale}")

for i in range(4):
    # Loading input root file and creating new root file
    inFile = ROOT.TFile.Open(f"{input_filename[i]}")
    inTree = inFile.Get('tree')

    outFile = ROOT.TFile.Open(f"{output_filename[i]}","RECREATE"); 
    outTree = inTree.CloneTree(0)
    print(f"Scaling from {input_filename[i]} and saving to {output_filename[i]} \
    with scale factor {scale[i]}")

    # Scaling the file
    k = 0       # To count and print the number of selected events per file
    for iEvent in range(inTree.GetEntries()):
        rand = ROOT.gRandom.Rndm()
        num = 1/scale[i]
        if rand>num:    #shouldn't be "<" symbol bcz "continue" BREAKS one iteration
            continue     
        inTree.GetEntry(iEvent)
        outTree.Fill()
        k += 1

    print(f"Afer scaling {k} events has been stored in {output_filename[i]}")
    outTree.AutoSave()
    outFile.Close()