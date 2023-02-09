#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys


print(f"Number of input files are {len(sys.argv)-1}")
print(f"{sys.argv[0]}")
whole_signal_event_number = 0 
whole_total_event_in_charged = 0 
for i in sys.argv[1:]:
    input_file = ROOT.TFile.Open(f"{i}")
    input_tree = input_file.Get('tree')
    signal_event_number = 0 
    total_event_in_charged = input_tree.GetEntries()
    for iEvent in range(total_event_in_charged):
        input_tree.GetEntry(iEvent)
        run_1 = getattr(input_tree, '__run__')
        event_1 = getattr(input_tree, '__event__')
        mbc_1 = getattr(input_tree, 'Mbc')
        deltae_1 = getattr(input_tree, 'deltaE')
        signal_1 = getattr(input_tree, 'isSignal')
        print(f"{iEvent} : run number : {run_1} ; event number: {event_1} ; issignal {signal_1}; Mbc : {mbc_1} ; deltaE : {deltae_1}")
        # input_tree.GetEntry(iEvent+1)
        # run_2 = getattr(input_tree, '__run__')
        # event_2 = getattr(input_tree, '__event__')
        # print(f"{iEvent} : run number is {run_1} and event number is {event_1}")
        # print(f"{iEvent+1} : run number is {run_2} and event number is {event_2}")
        # print("************************************")