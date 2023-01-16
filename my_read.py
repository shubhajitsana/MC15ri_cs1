#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
myFile = ROOT.TFile.Open("mdst_000001_prod00016662_task10020000001.root")
myTree = myFile.tree

# j=0
# for entry in myTree:
#     if entry.isSignal==1:
#         j+=1
# print(j)

a = myTree.GetListOfLeaves()
# datatype = a.GetTypeName    # is not working
# print(datatype)
d = ""
for item in a:
    b = str(item)
    c = b.split("Title: ")
    d += c[1]
    d += " : "
print(d)
# print(a)