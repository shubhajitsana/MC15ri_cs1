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

bbbar_Event = np.zeros(4)
for i in range(4): bbbar_Event[i] = input_tree[0].GetEntries()

qqbar_event_number = []
for i in range(4): qqbar_event_number.append(input_tree[(i +1)].GetEntries())

NEvent = np.array(qqbar_event_number)
NEvent_total = NEvent.sum()
scale = bbbar_Event[0]/NEvent_total    # Setting Scale

print(f"The event number of u,d,s,c(bar) are : {qqbar_event_number}")
print(f"The total number events of u,d,s,c(bar) are : {NEvent_total}")
print(f"The total number of TM events is : {bbbar_Event[0]}")
print(f"So scaling factor is {scale}")

for i in range(1,5):
    # # Loading input root file and creating new root file
    # inFile = ROOT.TFile.Open(f"{input_filename[i]}")
    # inTree = inFile.Get('tree')

    outFile = ROOT.TFile.Open(f"{output_filename[i]}","RECREATE"); 
    outTree = input_tree[i].CloneTree(0)
    print(f"Scaling from {input_filename[i]} and saving to {output_filename[i]} \
    with scale factor {scale}")

    # Scaling the file
    Number_of_selected_events = 0       # To count and print the number of selected events per file
    # j = i - 1       #as dimension of NEvent is 1 less than input_file
    for iEvent in range(qqbar_event_number[(i - 1)]):     #as dimension of NEvent is 1 less than input_file
        rand = ROOT.gRandom.Rndm()
        if rand > scale: continue    #shouldn't be "<" symbol bcz "continue" BREAKS one iteration
        input_tree[i].GetEntry(iEvent)
        outTree.Fill()
        Number_of_selected_events += 1

    print(f"Afer scaling {Number_of_selected_events} events has been stored in {output_filename[i]}")
    outTree.AutoSave()
    outFile.Close()