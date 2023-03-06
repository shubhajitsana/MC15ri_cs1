#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys


print(f"Number of input files are {len(sys.argv)-1}")
print(f"Steering file name is : {sys.argv[0]}")
whole_signal_event_number = 0 
whole_total_event_in_charged = 0 
for i in sys.argv[1:]:
    input_file = ROOT.TFile.Open(f"{i}")
    input_tree = input_file.Get('tree')
    signal_event_number = 0 
    total_event_in_charged = input_tree.GetEntries()
    for iEvent in range(total_event_in_charged):
        input_tree.GetEntry(iEvent)
        signal = getattr(input_tree, 'isSignal')
        bcs_rank = getattr(input_tree, 'BCS3_rank')
        if ((signal==1) and ((bcs_rank==1) or (bcs_rank==2) or (bcs_rank==3))):
            signal_event_number += 1
    print(f"There are {total_event_in_charged} reconstructed events in {i}")
    print(f"There are {signal_event_number} signal events in {i}")
    whole_signal_event_number += signal_event_number 
    whole_total_event_in_charged += total_event_in_charged
print(f"There are total {whole_signal_event_number} signal events out of \
{whole_total_event_in_charged} reconstructed events in given list of files.")