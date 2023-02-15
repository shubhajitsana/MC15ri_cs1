#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys
import numpy as np

# get input/output path/file from the command line
# 1st 5 are for input and next 5 are for output
input_filename = []
output_filename = []
for i in range(1, 6):
    input_filename.append(sys.argv[i])
print(f"5 Input files are {input_filename}")
for j in range(6,11):
    output_filename.append(sys.argv[j])
print(f"5 Output files are {output_filename}")

# Getting scales
infile_uubar = ROOT.TFile.Open(f"{input_filename[0]}")
infile_ddbar = ROOT.TFile.Open(f"{input_filename[1]}")
infile_ssbar = ROOT.TFile.Open(f"{input_filename[2]}")
infile_ccbar = ROOT.TFile.Open(f"{input_filename[3]}")
infile_bbbar = ROOT.TFile.Open(f"{input_filename[4]}")

intree_uubar = infile_uubar.Get('tree')
intree_ddbar = infile_ddbar.Get('tree')
intree_ssbar = infile_ssbar.Get('tree')
intree_ccbar = infile_ccbar.Get('tree')
intree_bbbar = infile_bbbar.Get('tree')

# Counting number of events of different folder before scaling
NEvent = np.zeros(5)
NEvent[0] = intree_uubar.GetEntries()
NEvent[1] = intree_ddbar.GetEntries()
NEvent[2] = intree_ssbar.GetEntries()
NEvent[3] = intree_ccbar.GetEntries()
NEvent[4] = intree_bbbar.GetEntries()
print(f"The event number of u,d,s,c,b(bar) are : {NEvent}")

NEvent_total = NEvent.sum()
cross_section = np.array([1.11, 1.61, 0.4, 0.38, 1.3])
print(f"The cross sections of u,d,s,c,b(bar) are : {cross_section} \
* 10^(-6) fb^-1)/(0.8 or 0.2 depending on train or test)")

luminocity = NEvent/cross_section   # as events = Luminocity * cross_section
print(f"The luminocity of u,d,s,c,b(bar) are : {luminocity}")
lowest_luminocity = luminocity.min()
print(f"The total events in the given input files are {NEvent_total} . But \
to include maximum events from this data \
\n(keeping effective luminocity same)\
lowest luminocity is {lowest_luminocity}") # this is the number
    #  \n HIGHEST number of total events should be {lowest_luminocity}") # this is the number
                            # using which we can include maximum continuum event following their
                            # (relative) cross section ratio - u:d:s:c = 1.61:0.4:0.38:1.3
if lowest_luminocity <= NEvent_total:
    New_NEvent_total = np.zeros(5)
    for i in range(5):
        New_NEvent_total[i] = lowest_luminocity
else:
    print("There is some problem during calculation of scaling factor")
# print(f"New total event is {New_NEvent_total}")
New_NEvent = New_NEvent_total * cross_section

scale = NEvent/New_NEvent                                       # Setting Scale
print(f"Previous total events in u,d,s,c,b(bar) folder were {NEvent}")
print(f"New total event according to cross section should be {New_NEvent}")
print(f"So scaling factor is {scale}")

for i in range(5):
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

    print(f"Afer scaling {k} events has been stored in {output_filename[i]} with luminocity \
    ({lowest_luminocity} * 10^(-6) fb^-1)/(0.8 or 0.2 depending on train or test)")
    outTree.AutoSave()
    outFile.Close()