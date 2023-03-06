#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys

# get input/output path/file from the command line
input_filename = sys.argv[1]
signal_filename = sys.argv[2]
bkg_filename = sys.argv[3]
print(f"The input file is {input_filename}")
print(f"The signal file is {signal_filename}")
print(f"The bkg file is {bkg_filename}")


# Loading input root file and creating new root file
inFile = ROOT.TFile.Open(f"{input_filename}")
inTree = inFile.Get('tree')
print(f"There are {inTree.GetEntries()} reconstructed events in {input_filename}")

# Filling Signal event
outFile_signal = ROOT.TFile.Open(f"{signal_filename}","RECREATE")
outTree_signal = inTree.CloneTree(0)
signal_count = 0       # To count and print the number of selected signal
for iEvent in range(inTree.GetEntries()):
    inTree.GetEntry(iEvent)
    signal = getattr(inTree, 'isSignal')
    if signal==1:
        outTree_signal.Fill()
        signal_count += 1
print(f"There were {signal_count} signal events which have been stored in {signal_filename}")
outTree_signal.AutoSave()
outFile_signal.Close()

# Filling bkg events
outFile_bkg = ROOT.TFile.Open(f"{bkg_filename}","RECREATE")
outTree_bkg = inTree.CloneTree(0)
bkg_count = 0       # To count and print the number of selected signal
for iEvent in range(inTree.GetEntries()):
    inTree.GetEntry(iEvent)
    signal = getattr(inTree, 'isSignal')
    if signal==1: continue
    else:
        outTree_bkg.Fill()
        bkg_count += 1
print(f"There were {bkg_count} background events which have been stored in {bkg_filename}")
outTree_bkg.AutoSave()
outFile_bkg.Close()