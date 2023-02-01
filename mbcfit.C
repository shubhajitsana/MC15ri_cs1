#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooExponential.h"
#include "RooBifurGauss.h"
#include "RooAddModel.h"
#include "RooProdPdf.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooHist.h"
#include "RooCBShape.h"
#include "RooPolynomial.h"
#include "RooBinning.h"
#include "TH1.h"
#include "TH2.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooLandau.h"
#include "TChain.h"
#include <cmath>
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooCategory.h"
#include "RooSuperCategory.h"
#include "RooSimultaneous.h"
#include "RooNLLVar.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "RooArgusBG.h"
using namespace RooFit ;
using namespace std;

void mbcfit(){ 
  /*******************Fit Variables***********************************/
  RooRealVar mbc("mbc","M_{bc} (GeV)",5.23,5.32);
  /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
  RooDataSet* data=new RooDataSet("data","data",RooArgSet(mbc));


  /*******************Input root file**********************************/
  TChain* chain=new TChain();
  chain->Add("/home/sana/ssana/15_data/combined/all.root/tree");

  Double_t  o_de, o_md0, o_mbc, o_r2, o_kid,o_pid,sig;
  Int_t run;
  Int_t nevt=(int)chain->GetEntries();

  chain->SetBranchAddress("isSignal",&sig);
  chain->SetBranchAddress("deltaE",&o_de);
  chain->SetBranchAddress("Mbc",&o_mbc);
  chain->SetBranchAddress("D0_bar_InvM",&o_md0);
  chain->SetBranchAddress("R2",&o_r2);
  chain->SetBranchAddress("Kp_PID_bin_kaon",&o_kid);
  // chain->SetBranchAddress("pi_PID_bin_pion",&o_pid);
  //  chain->SetBranchAddress("__run__",&run);

  //Loading data 
  Double_t counter =0;
  for(int i=0;i<nevt;i++) {
    chain->GetEntry(i);
    mbc.setVal(o_mbc);
    if(o_md0>1.84 && o_md0<1.89 && o_mbc>5.27 && o_mbc < 5.29 && o_r2 < 0.3 && o_de < 0.15 && o_de > -0.15 && o_kid > 0.6){
      data->add(RooArgSet(mbc));
      counter++;
    }
  }
  /*****************************************************************/

  //Drawing casual Histogram(BINNED) before fitting
  RooDataHist* binDataSet = new RooDataHist("binDataSet", "binDataSet", mbc, *data);
  TCanvas *c1 = new TCanvas("c1", "", 1500, 1500);  
  RooPlot* xframe1 = mbc.frame(	Title(" ")	, Bins(200));
  binDataSet->plotOn(	xframe1	, Binning(200)		, DataError(RooAbsData::SumW2));
  xframe1->Draw();
  c1 -> Print("prefit_histo.png");


  /***********************************Mbc fit****************************/
  // --- Build Signal PDF ---
  RooRealVar sigmean("mean_{M_{bc}}","B^{#pm} mass",5.279145,5.27,5.29) ;
  RooRealVar sigwidth("sig_{M_{bc}}","B^{#pm} width",0.002708,0., 0.01) ;
  RooGaussian gauss1("gauss1","gaussian PDF",mbc,sigmean,sigwidth) ; //Gauss1 signal for mbc

  // --- Build Argus background PDF ---
  RooRealVar argpar("argpar","argus shape parameter",-34.70);//,-100.,-1.) ;
  RooArgusBG argus("argus","Argus PDF",mbc,RooConst(5.29),argpar) ; //mbc background		 
      
  //Initialization of parameter before adding two pdf
  Double_t event_count = counter; 
  Double_t signal_count = counter*0.4;
  Double_t back_count = counter*0.6;
  RooRealVar n_sig("n_sig", "n_sig", signal_count, 0., event_count);//52000
  RooRealVar n_bkg("n_bkg", "n_bkg", back_count, 0., event_count);//95000
  RooAddPdf sum("sum","sum",RooArgList(gauss1,argus),RooArgList(n_sig, n_bkg));//adding two pdf
  
  RooFitResult *result = sum.fitTo(*data, Range(5.23,5.32));//fitting
  /****************************FIT COMPLETE*************************************/

  /*********************Start Plotting and showing outpouts*****************/
  //Plotting fitted result
  RooPlot* frame = mbc.frame(Title("Fitting Beam constrained mass of B^{#pm}"), Bins(200)) ;
  data->plotOn(frame, Binning(200), DataError(RooAbsData::SumW2)) ;
  sum.plotOn(frame, LineColor(kBlue)	, LineStyle(kSolid)) ;
  // sum.paramOn(frame,data,"Fit parameter", 3, "NELU", 0.1, 0.35, 0.9); //Prints the fitted parameter on the canvas
  sum.plotOn(frame,Components(gauss1),LineColor(kRed),LineStyle(kDashed));
  sum.plotOn(frame,Components(argus),LineColor(kMagenta),LineStyle(kDashed)); 

  //Extract info. from fitting frame and showing
  cout<<"chisq of the fit is :"<<frame->chiSquare()<<endl;// You have to get the chi-square of the fit from MassFrame --
  cout<<"chi-square/ndof :"<<frame->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
  RooHist* hpull = frame->pullHist() ;//extract pull distribution
  RooPlot* frame3 = mbc.frame(Title("Pull Distribution")) ;//Frame for plotting pull distribution
  // frame3->addPlotable(hpull,"P");
  hpull->SetFillColor(1);
  frame3->addPlotable(hpull,"X0B") ; // "X0" is for errorbar; and "B" is for histograms

  //Start drawing fitting results
  TCanvas* c2 = new TCanvas("c2", "", 1500, 1500) ;
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();  
  frame->Draw();
  // Drawing arrow in 3 sigma point of each side
  // double weightedMean = mean1.getVal()*fsig1.getVal() + mean2.getVal()*(1.0-fsig1.getVal());
  // double weightedSigma = std::sqrt(pow(sigma1.getVal(),2)*fsig1.getVal() + pow(sigma2.getVal(),2)*(1-fsig1.getVal()));
  double weightedMean = sigmean.getVal();
  double weightedSigma = std::sqrt(pow(sigwidth.getVal(),2));
  double Emin_fit = weightedMean - 3*weightedSigma; 
  double Emax_fit = weightedMean + 3*weightedSigma;
  double arrowHeight = n_sig.getVal()*0.03;
	TArrow *arr1 = new TArrow(-0.01, 100, -0.01, 400, 0.01,"|>");
	arr1->SetLineWidth(4);
  arr1->SetLineColor(2);
  arr1->SetFillStyle(3008);
  // arr->SetArrowSize(5);
  arr1->DrawArrow(Emin_fit, arrowHeight*0.1, Emin_fit, arrowHeight, 0.02,"<"); 
  arr1->DrawArrow(Emax_fit, arrowHeight*0.1, Emax_fit, arrowHeight, 0.02,"<");

  // Adding legend
  TLegend *legend1 = new TLegend(0.1,0.7,0.3,0.9);
  TLegendEntry *entry = legend1->AddEntry("gauss1","Gaussian pdf","l");
  entry->SetLineColor(kRed);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("argus","Argus pdf","l");
  entry->SetLineColor(kMagenta);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sum","Fitted pdf","l");
  entry->SetLineColor(kBlue);
  entry->SetLineStyle(kSolid);
  legend1->Draw();

  c2->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->Draw();
  pad2->cd(); 
  frame3->Draw() ;
  frame3->SetLineStyle(9);
  frame3->GetYaxis()->SetNdivisions(505);
  frame3->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
  frame3->GetXaxis()->SetTitle("M_{bc} (GeV)"); 
  frame3->GetXaxis()->SetTitleSize(0.13);
  frame3->GetYaxis()->SetTitleSize(0.15);
  frame3->GetXaxis()->SetLabelSize(0.120);
  frame3->GetYaxis()->SetLabelSize(0.120); 
  frame3->GetXaxis()->SetTitleOffset(0.90);      
  frame3->GetYaxis()->SetTitleOffset(0.22);       
  frame3->GetYaxis()->SetRangeUser(-10.0, 10.0);       
  frame3->GetYaxis()->SetLimits(-10.0, 10.0);       
  frame3->GetXaxis()->SetNdivisions(505);
  frame3->GetYaxis()->CenterTitle(true);
  frame3->GetXaxis()->CenterTitle(true);
  frame3->Draw("AXISSAME");

  c2 -> Print("mbcfit1.pdf");
}