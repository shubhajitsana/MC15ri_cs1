#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Doesn't work
import ROOT
import sys
# import numpy as np

c1 = ROOT.TCanvas("Continuum Probability", "Continuum Probability", 900, 700)
legend = ROOT.TLegend(0.3,0.7,0.65,0.9)
continuum_probability_plot_output_path = "cs_fom_data/combined/continuum_probability_plot"
continuum_probability_plot_output_file = "continuum_probability_plot.png"

print(f"Number of input files are {len(sys.argv)-1}")
print(f"My steering file name is {sys.argv[0]}")
color_code = 1
for i in sys.argv[1:]:
    filename_option = i.split("cs_fom_data/combined/")
    output_filename_option_with_extension = filename_option[1]
    output_filename_option_with_extension_split = output_filename_option_with_extension.split(".root")
    output_filename_option = output_filename_option_with_extension_split[0]
    
    input_file = ROOT.TFile.Open(f"{i}")
    input_tree = input_file.Get('tree')
    total_event_number = input_tree.GetEntries()

    # Defining and filling histogram
    if color_code == 1:
        histogram_1 = ROOT.TH1F("Continuum Probability", "Continuum Probability", 100, 0, 1)
        for iEvent in range(total_event_number):
            input_tree.GetEntry(iEvent)
            Continuum_Probability = getattr(input_tree, 'ContProb')
            histogram_1.Fill(Continuum_Probability)
        # Decorate histogram
        histogram_1.SetLineColor(color_code)
        histogram_1.GetXaxis().SetTitle("Continuum Probability")
        histogram_1.GetXaxis().SetTitleSize(0.05)
        histogram_1.GetYaxis().SetTitle("Number of events")
        histogram_1.GetYaxis().SetTitleSize(0.05)
        histogram_1.SetStats(ROOT.kFALSE)
        histogram_1.Draw()
        legend.AddEntry(histogram_1, f"{output_filename_option}")
        c1.Update()
    elif color_code == 2:
        histogram_2 = ROOT.TH1F("Continuum Probability", "Continuum Probability", 100, 0, 1)
        for iEvent in range(total_event_number):
            input_tree.GetEntry(iEvent)
            Continuum_Probability = getattr(input_tree, 'ContProb')
            histogram_2.Fill(Continuum_Probability)
        # Decorate histogram
        histogram_2.SetLineColor(color_code)
        histogram_2.GetXaxis().SetTitle("Continuum Probability")
        histogram_2.GetXaxis().SetTitleSize(0.05)
        histogram_2.GetYaxis().SetTitle("Number of events")
        histogram_2.GetYaxis().SetTitleSize(0.05)
        histogram_2.SetStats(ROOT.kFALSE)
        histogram_2.Draw("SAME")
        legend.AddEntry(histogram_2, f"{output_filename_option}")
        c1.Update()
    color_code += 1
legend.Draw()
c1.Update()
c1.SaveAs(f"{continuum_probability_plot_output_path}/{continuum_probability_plot_output_file}")