#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys
import numpy as np
# import math

print(f"Number of input files are {len(sys.argv)-1}")
print(f"{sys.argv[0]}")
for i in sys.argv[1:]:
    input_path = i.split("/test.root")
    output_path = input_path[0]
    output_filename = "fom.txt"
    fom_file = open(f"{output_path}/{output_filename}", 'w')    # defining exporting file
    input_file = ROOT.TFile.Open(f"{i}")    # Loading root file
    input_tree = input_file.Get('tree')
    total_event_in_charged = input_tree.GetEntries()

    # calculating FOM
    for cs_prob_cut in np.arange(0.01, 1, 0.01):
        sig = 0
        bkg = 0
        for iEvent in range(total_event_in_charged):
            input_tree.GetEntry(iEvent)
            cs_prob = getattr(input_tree, 'ContProb')
            issig = getattr(input_tree, 'isSignal')
            iscontinuum = getattr(input_tree, 'isContinuumEvent')
            if cs_prob < cs_prob_cut:
                # bkg += 1 if iscontinuum == 1 else sig += 1  # can't use arithmetic operator
                if iscontinuum == 1: bkg += 1
                else: sig +=1
        cs_prob_fom = sig/np.sqrt(sig + bkg)
        fom_file.write(repr(cs_prob_cut) + '\t' + repr(cs_prob_fom) + '\n')
    fom_file.close()