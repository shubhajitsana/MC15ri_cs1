#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys

print(f"Number of input files are {len(sys.argv)-1}")
print(f"{sys.argv[0]}")
for i in sys.argv[1:]:
    c1 = ROOT.TCanvas("fom", "fom", 900, 700)
    c1.SetFillColor(0)
    c1.SetGrid()

    GR2 = ROOT.TGraph(f"{i}")
    GR2.SetLineColor(2)
    GR2.SetLineWidth(3)
    GR2.SetMarkerColor(2)
    GR2.SetMarkerStyle(21)
    GR2.GetXaxis().SetTitle("Cut")
    GR2.GetXaxis().SetTitleSize(0.05)
    GR2.GetYaxis().SetTitle("S/ #sqrt{S+B}")
    GR2.GetYaxis().SetTitleSize(0.05)
    GR2.Draw("ACP")

    input_path = i.split("/fom.txt")
    output_path = input_path[0]
    output_filename = "fom_plot.pdf"
    c1.SaveAs(f"{output_path}/{output_filename}")