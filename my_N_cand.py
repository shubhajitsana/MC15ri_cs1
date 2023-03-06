#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys
import numpy as np

print(f"Number of input files are {len(sys.argv)-1}")
print(f"Steering file is {sys.argv[0]}")
for i in sys.argv[1:]:
    # N_cand_output_path = "swap_data/combined/N_cand"
    N_cand_output_path = "data/combined/N_cand"

    # filename_option = i.split("swap_data/combined/")
    filename_option = i.split("data/combined/")
    output_filename_option_with_extension = filename_option[1]
    output_filename_option_with_extension_split = output_filename_option_with_extension.split(".root")
    output_filename_option = output_filename_option_with_extension_split[0]

    N_cand_output_filename = f"N_cand_{output_filename_option}.png"
    
    input_file = ROOT.TFile.Open(f"{i}")
    input_tree = input_file.Get('tree')
    total_event_number = input_tree.GetEntries()

    histogram = ROOT.TH1I("N_cand", "N_cand", 6, 0, 5)

    TM_in_rank_1 = 0
    TM_in_rank_2 = 0
    TM_in_rank_3 = 0
    TM_in_rank_4 = 0
    TM_in_rank_5 = 0
    Total_TM_event = 0
    for iEvent in range(total_event_number):
        input_tree.GetEntry(iEvent)
        signal = getattr(input_tree, 'isSignal')
        bcs_rank = getattr(input_tree, 'BCS3_rank')

        if signal ==1:
            Total_TM_event += 1
            histogram.Fill(bcs_rank)
            if bcs_rank == 1:
                TM_in_rank_1 += 1
            elif bcs_rank ==2:
                TM_in_rank_2 += 1
            elif bcs_rank ==3:
                TM_in_rank_3 += 1
            elif bcs_rank ==4:
                TM_in_rank_4 += 1
            elif bcs_rank ==5:
                TM_in_rank_5 += 1

    print(f"Total number of Truth Matched event in the given file is {Total_TM_event}")
    print(f"Number of Truth Matched events in BCS rank 1 is {TM_in_rank_1} with percentage {100 * (TM_in_rank_1/Total_TM_event)}")
    print(f"Number of Truth Matched events in BCS rank 2 is {TM_in_rank_2} with percentage {100 * (TM_in_rank_2/Total_TM_event)}")
    print(f"Number of Truth Matched events in BCS rank 3 is {TM_in_rank_3} with percentage {100 * (TM_in_rank_3/Total_TM_event)}")
    print(f"Number of Truth Matched events in BCS rank 4 is {TM_in_rank_4} with percentage {100 * (TM_in_rank_4/Total_TM_event)}")
    print(f"Number of Truth Matched events in BCS rank 5 is {TM_in_rank_5} with percentage {100 * (TM_in_rank_5/Total_TM_event)}")

    c1 = ROOT.TCanvas("N_cand", "N_cand", 900, 700)

    histogram.SetFillColor(9)
    histogram.SetTitle(f"Distribution of N_cand in {output_filename_option} folder")
    histogram.GetXaxis().SetTitle("N_{cand}")
    histogram.GetXaxis().SetTitleSize(0.05)
    histogram.GetYaxis().SetTitle("Number of TM events")
    histogram.GetYaxis().SetTitleSize(0.05)
    histogram.Draw("B")
    c1.Update()   

    c1.SaveAs(f"{N_cand_output_path}/{N_cand_output_filename}")