#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys
import numpy as np

print(f"Number of input files are {len(sys.argv)-1}")
print(f"{sys.argv[0]}")
for i in sys.argv[1:]:
    multiplicity_output_path = "swap_data_new/combined/multiplicity"

    filename_option = i.split("swap_data_new/combined/")
    output_filename_option_with_extension = filename_option[1]
    output_filename_option_with_extension_split = output_filename_option_with_extension.split(".root")
    output_filename_option = output_filename_option_with_extension_split[0]

    multiplicity_output_filename = f"multiplicity_{output_filename_option}.png"
    multiplicity_output_txt_filename = f"multiplicity_{output_filename_option}.txt"
    multiplicity_txt = open(f"{multiplicity_output_path}/{multiplicity_output_txt_filename}", 'w')
    
    input_file = ROOT.TFile.Open(f"{i}")
    input_tree = input_file.Get('tree')
    total_event_number = input_tree.GetEntries()
    event_number_array = []
    candidate_number_array = []

    histogram = ROOT.TH1I("Multiplicity", "Multiplicity", 21, 0, 20)
    iEvent = 0
    separate_event_number = 0
    while iEvent < total_event_number:
        input_tree.GetEntry(iEvent)
        event_number = getattr(input_tree, '__event__')
        candidate_number = getattr(input_tree, '__ncandidates__')
        event_number_array.append(event_number)
        candidate_number_array.append(candidate_number)
        iEvent += candidate_number
        separate_event_number += 1
        histogram.Fill(candidate_number)

    event_number_np = np.array(event_number_array)
    candidate_number_np = np.array(candidate_number_array)

    print(f"Final value of iteration is {iEvent}. And total event is {total_event_number}")
    print(f"Total number of candidates is {candidate_number_np.sum()}")
    print(f"Total number of separate event is {separate_event_number}")
    multiplicity_txt.write("Final value of iteration is " + repr(iEvent) + ". And total event is " + repr(total_event_number) + "\n")
    multiplicity_txt.write("Total number of candidates are " + repr(candidate_number_np.sum()) + "\n")
    multiplicity_txt.write("Total number of separate event is " + repr(separate_event_number) + "\n")

    for i in range(event_number_np.size):
        # print(f"event number {event_number_np[i]}; and candidates are {candidate_number_np[i]}")
        multiplicity_txt.write("event number " + repr(event_number_np[i]) + "; and candidates are " + repr(candidate_number_np[i]) + "\n")
    multiplicity_txt.close()


    c1 = ROOT.TCanvas("Multiplicity", "Multiplicity", 900, 700)

    histogram.SetFillColor(9)
    histogram.SetTitle(f"Distribution of multiplicity in {output_filename_option} folder")
    histogram.GetXaxis().SetTitle("Number of candidates")
    histogram.GetXaxis().SetTitleSize(0.05)
    histogram.GetYaxis().SetTitle("Number of events")
    histogram.GetYaxis().SetTitleSize(0.05)
    histogram.Draw("B")
    c1.Update()   

    c1.SaveAs(f"{multiplicity_output_path}/{multiplicity_output_filename}")