#!/usr/bin/env python
# -*- coding: utf-8 -*-
# This file scale such a way that 50% will be signal and 50% will be continnum background
import ROOT
import sys
import numpy as np

# get input/output path/file from the command line
# 1st 5 are for input and next 5 are for output
input_filename = []
output_filename = []
for i in range(1, 6): input_filename.append(sys.argv[i])
print(f"5 Input files are {input_filename}")
for j in range(6,11): output_filename.append(sys.argv[j])
print(f"5 Output files are {output_filename}")

######################reading data###############
input_root_file = []
for i in range(len(input_filename)): input_root_file.append(ROOT.TFile.Open(f"{input_filename[i]}"))

input_tree = []
for i in range(len(input_root_file)): input_tree.append(input_root_file[i].Get('tree'))

total_event_number = []
for i in range(len(input_tree)): total_event_number.append(input_tree[i].GetEntries())
NEvent = np.array(total_event_number)
cross_section_ratio_in_percentage = np.array([1, 0.43168, 0.10785, 0.10301, 0.35745])

Projected_NEvent = NEvent/cross_section_ratio_in_percentage
TM_NEvent = np.zeros(5)
for i in range(len(TM_NEvent)): TM_NEvent[i] = Projected_NEvent.min()

Final_NEvent = TM_NEvent * cross_section_ratio_in_percentage
scale = Final_NEvent/NEvent                                       # Setting Scale

print(f"The event number of TM,u,d,s,c(bar) are : {NEvent}")
print(f"The cross sections ratio of u,d,s,c(bar) are : {cross_section_ratio_in_percentage}")
print(f"The Projected event number of TM,u,d,s,c(bar) are : {Projected_NEvent}")
print(f"The number of TM events should be : {TM_NEvent}")
print(f"New total event according to cross section should be {Final_NEvent}")
print(f"So scaling factor is {scale}")

for i in range(5):
    # # Loading input root file and creating new root file
    # inFile = ROOT.TFile.Open(f"{input_filename[i]}")
    # inTree = inFile.Get('tree')

    outFile = ROOT.TFile.Open(f"{output_filename[i]}","RECREATE"); 
    outTree = input_tree[i].CloneTree(0)
    print(f"Scaling from {input_filename[i]} and saving to {output_filename[i]} \
    with scale factor {scale[i]}")

    # Scaling the file
    Number_of_selected_events = 0       # To count and print the number of selected events per file
    for iEvent in range(NEvent[i]):
        rand = ROOT.gRandom.Rndm()
        if rand > scale[i]:    #shouldn't be "<" symbol bcz "continue" BREAKS one iteration
            continue     
        input_tree[i].GetEntry(iEvent)
        outTree.Fill()
        Number_of_selected_events += 1

    print(f"Afer scaling {Number_of_selected_events} events has been stored in {output_filename[i]}")
    outTree.AutoSave()
    outFile.Close()