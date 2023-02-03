#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT

######################Fit Variables#################
mbc = ROOT.RooRealVar("mbc","M_{bc} (GeV)",5.23,5.29)
####defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT
data = ROOT.RooDataSet("data", "data", ROOT.RooArgSet(mbc))

# Loading input root file and creating new root file
inFile = ROOT.TFile.Open("/home/sana/ssana/15_data/combined/charged.root")
inTree = inFile.Get('tree')
NEvent = inTree.GetEntries()

# Loading data 
counter =0
for iEvent in range(inTree.GetEntries()):
    inTree.GetEntry(iEvent)
    signal = getattr(inTree, 'isSignal')
    o_de = getattr(inTree, 'deltaE')
    o_mbc = getattr(inTree, 'Mbc')
    o_md0 = getattr(inTree, 'D0_bar_InvM')
    o_r2 = getattr(inTree, 'R2')
    o_kid = getattr(inTree, 'Kp_PID_bin_kaon')
    # o_ = getattr(inTree, '')
    if((o_md0>1.84) and (o_md0<1.89) and (o_mbc>5.27) and (o_mbc < 5.29) and (o_de < 0.15) and (o_de > -0.15) and (o_kid > 0.6)): #(o_r2 < 0.3) and 
        data.add({mbc})
        counter += 1


# //Drawing casual Histogram(BINNED) before fitting
binDataSet = ROOT.RooDataHist("binDataSet", "binDataSet", mbc, data)
c1 = ROOT.TCanvas("c1", "", 1500, 1500)  
xframe1 = mbc.frame(Title="prefit histooo", Bins=200)
binDataSet.plotOn(xframe1, Binning=200, DataError="SumW2")
xframe1.Draw()
c1.SaveAs("py_prefit_histo.png")


#***********************************Mbc fit****************************
#--- Build Signal PDF ---
sigmean = ROOT.RooRealVar("mean_{M_{bc}}","B^{#pm} mass",5.279145,5.27,5.29)
sigwidth = ROOT.RooRealVar("sig_{M_{bc}}","B^{#pm} width",0.002708,0., 0.01)
gauss1 = ROOT.RooGaussian("gauss1","gaussian PDF",mbc,sigmean,sigwidth) #Gauss1 signal for mbc

# // --- Build Argus background PDF ---
argpar = ROOT.RooRealVar("argpar","argus shape parameter",-34.70)#,-100.,-1.)
argus = ROOT.RooArgusBG("argus","Argus PDF",mbc, 5.29, argpar,0.5) #mbc background		 
    
# //Initialization of parameter before adding two pdf
event_count = counter 
signal_count = counter*0.4
back_count = counter*0.6
n_sig = ROOT.RooRealVar("n_sig", "n_sig", signal_count, 0., event_count)#52000
n_bkg = ROOT.RooRealVar("n_bkg", "n_bkg", back_count, 0., event_count)#95000
sum = ROOT.RooAddPdf("sum","sum", [gauss1,argus], [n_sig, n_bkg])#adding two pdf

sum.fitTo(data)#fitting

#   /*********************Start Plotting and showing outpouts*****************/
#   //Plotting fitted result
frame = mbc.frame()#20)
data.plotOn(frame)
sum.plotOn(frame)
sum.plotOn(frame) 
#Prints the fitted parameter on the canvas
sum.paramOn(frame, FillColor="kRed", Label="Fit parameters", Layout=(0.1, 0.35, 0.9),Format="NELU", AutoPrecision=1, ShowConstants=True)
sum.plotOn(frame, Components="gauss1",LineColor="kRed", LineStyle="--")
sum.plotOn(frame, Components="argus",LineColor="kMagenta", LineStyle="--")

# //Extract info. from fitting frame and showing
chisq = frame.chiSquare()#extract chi2 value
hpull = frame.pullHist() #extract pull distribution
print(f"chi2 of mbc fitting = {chisq}") #Printing chi2 value
frame3 = mbc.frame() # ROOT.Title("Pull Distribution") #Frame for plotting pull distribution
frame3.addPlotable(hpull,"P") 

# //Start drawing fitting results
c2 = ROOT.TCanvas("c2", "", 1500, 1500) 
pad1 = ROOT.TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
pad1.Draw()             # Draw the upper pad: pad1
pad1.cd()  
frame.Draw() 

c2.cd()          # Go back to the main canvas before defining pad2
pad2 = ROOT.TPad("pad2", "pad2", 0, 0.05, 1, 0.3)
pad2.Draw()
pad2.cd() 
frame3.Draw() 
c2.SaveAs("py_mbcfit.pdf")