#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import sys

print(f"Number of input files are {len(sys.argv)-1}")
print(f"{sys.argv[0]}")
for i in sys.argv[1:]:
    input_file = ROOT.TFile.Open(f"{i}")
    input_tree = input_file.Get('tree')
    print(f"Total number of events in tree(name:\"tree\") of {i} are {input_tree.GetEntries()}")
# inputArgs = sys.argv
# print(f"Number of input files are {len(inputArgs)-1}")
# for i in inputArgs[1:]:
#     input_file = ROOT.TFile.Open(f"{i}")
#     input_tree = input_file.Get('tree')
#     print(f"Total number of events in tree(name:\"tree\") of {i} are {input_tree.GetEntries()}")