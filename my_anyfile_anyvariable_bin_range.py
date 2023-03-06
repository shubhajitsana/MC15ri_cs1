#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys


file_name = sys.argv[1]
variable_name = sys.argv[2]
bin_number = int(sys.argv[3])
lower_range = float(sys.argv[4])
upper_range = float(sys.argv[5])
histogram_title = variable_name

# if sys.argv[6] == "": histogram_title = variable_name     #showing ""IndexError: list index out of range""
# else:
#     histogram_title = ""
#     input_from_6th_string = sys.argv[6].split("-")
#     for i in input_from_6th_string:
#         histogram_title += i
#         histogram_title += " "

print(f"My steering file name is {file_name}")
print(f"{variable_name} has been plotted.")
print(f"Number of bins are {bin_number}")
print(f"Lower range of histogram is {lower_range}")
print(f"Upper range of histogram is {upper_range}")
print(f"Title of histogram is {histogram_title}")

input_file = ROOT.TFile.Open(f"{file_name}")
input_tree = input_file.Get('tree')
total_event_number = input_tree.GetEntries()

histogram = ROOT.TH1F(f"{histogram_title}", f"Distribution of {histogram_title}", bin_number, lower_range, upper_range)
for iEvent in range(total_event_number):
    input_tree.GetEntry(iEvent)
    variable_value = getattr(input_tree, f"{variable_name}")
    histogram.Fill(variable_value)

c1 = ROOT.TCanvas(f"{histogram_title}", f"{histogram_title}", 900, 700)
histogram.SetLineColor(9)
histogram.GetXaxis().SetTitle(f"{histogram_title}")
histogram.GetXaxis().SetTitleSize(0.05)
histogram.GetYaxis().SetTitle("Number of events")
histogram.GetYaxis().SetTitleSize(0.05)
# histogram.SetStats(ROOT.kFALSE)
histogram.Draw()
c1.Update()
c1.SaveAs(f"plot_from_my_anyfile_anyvariable_bin_range/{variable_name}.png")