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
// #include "fitstyle_header/prefit_hist_draw.h"
// #include "fitstyle_header/mbc_fit_style.h"
using namespace RooFit ;
using namespace std;

void prefit_hist_draw(RooRealVar var_name, RooDataSet dataset_name){
  RooDataHist* binDataSet = new RooDataHist("binDataSet", "binDataSet", var_name, dataset_name);
  TCanvas *c1 = new TCanvas("c1", "", 1500, 1500);  
  RooPlot* xframe1 = var_name.frame(	Title(" ")	, Bins(200));
  binDataSet->plotOn(	xframe1	, Binning(200)		, DataError(RooAbsData::SumW2));
  xframe1->Draw();
  c1 -> Print("prefit_histo.png");
}

void mbc_fit_style(RooRealVar var_name, RooDataSet dataset_name,
 RooGaussian signal_pdf, RooArgusBG backround_pdf, RooAddPdf added_pdf)
{
  RooPlot* frame = var_name.frame(20) ;
  dataset_name.plotOn(frame) ;
  added_pdf.plotOn(frame) ;
  // added_pdf.paramOn(frame,dataset_name,"Fit parameter", 3, "NELU", 0.1, 0.35, 0.9); //Prints the fitted parameter on the canvas
  added_pdf.plotOn(frame,Components(signal_pdf),LineColor(kRed),LineStyle(kDashed));
  added_pdf.plotOn(frame,Components(backround_pdf),LineColor(kMagenta),LineStyle(kDashed));

  //Extract info. from fitting frame and showing
  Double_t chisq = frame->chiSquare();//extract chi2 value
  RooHist* hpull = frame->pullHist() ;//extract pull distribution
  cout << "chi2 of mbc fitting =" << chisq << endl;//Printing chi2 value
  RooPlot* frame3 = var_name.frame(Title("Pull Distribution")) ;//Frame for plotting pull distribution
  frame3->addPlotable(hpull,"P") ;

  //Start drawing fitting results
  TCanvas* c2 = new TCanvas("c2", "", 1500, 1500) ;
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();  
  frame->Draw() ;

  c2->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->Draw();
  pad2->cd(); 
  frame3->Draw() ;
  c2 -> Print("mbcfit.pdf");
}

void mbcfit(){ 
  /*******************Input root file**********************************/
  TChain* chain=new TChain();
  chain->Add("/home/sana/ssana/MC15ri/data/charged/mdst_000001_prod00024816_task10020000001.root/tree");

  Double_t  o_de, o_md0, o_mbc, o_r2, o_kid,o_pid,sig;
  Int_t run;
  Int_t nevt=(int)chain->GetEntries();

  chain->SetBranchAddress("deltaE",&o_de);
  chain->SetBranchAddress("Mbc",&o_mbc);
  chain->SetBranchAddress("D0_bar_InvM",&o_md0);
  chain->SetBranchAddress("R2",&o_r2);
  chain->SetBranchAddress("isSignal",&sig);
  chain->SetBranchAddress("Kp_PID_bin_kaon",&o_kid);
  // chain->SetBranchAddress("pi_PID_bin_pion",&o_pid);
  //  chain->SetBranchAddress("__run__",&run);



  /*******************Fit Variables***********************************/
  RooRealVar mbc("mbc","M_{bc} (GeV)",5.23,5.29);
  /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
  RooDataSet data("data","data",RooArgSet(mbc));


  //Loading data 
  Double_t counter =0;
  for(int i=0;i<nevt;i++) {
    chain->GetEntry(i);
    mbc.setVal(o_mbc);
    if(o_md0>1.84 && o_md0<1.89 && o_mbc>5.23 && o_mbc < 5.29 && o_r2 < 0.3 && o_de < 0.05 && o_de > -0.05 && o_kid > 0.6){
      data.add(RooArgSet(mbc));
      counter++;
    }
  }

  //Drawing casual Histogram(BINNED) before fitting
  prefit_hist_draw(mbc, &data);
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
  
  sum.fitTo(data);//fitting

  /*********************Start Plotting and showing outpouts*****************/
  //Plotting fitted result
  mbc_fit_style(mbc, &data, gauss1, argus,sum);
  // RooPlot* frame = mbc.frame(20) ;
  // data->plotOn(frame) ;
  // sum.plotOn(frame) ;
  // sum.paramOn(frame,data,"Fit parameter", 3, "NELU", 0.1, 0.35, 0.9); //Prints the fitted parameter on the canvas
  // sum.plotOn(frame,Components(gauss1),LineColor(kRed),LineStyle(kDashed));
  // sum.plotOn(frame,Components(argus),LineColor(kMagenta),LineStyle(kDashed));

  // //Extract info. from fitting frame and showing
  // Double_t chisq = frame->chiSquare();//extract chi2 value
  // RooHist* hpull = frame->pullHist() ;//extract pull distribution
  // cout << "chi2 of mbc fitting =" << chisq << endl;//Printing chi2 value
  // RooPlot* frame3 = mbc.frame(Title("Pull Distribution")) ;//Frame for plotting pull distribution
  // frame3->addPlotable(hpull,"P") ;

  // //Start drawing fitting results
  // TCanvas* c2 = new TCanvas("c2", "", 1500, 1500) ;
  // TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  // pad1->Draw();             // Draw the upper pad: pad1
  // pad1->cd();  
  // frame->Draw() ;

  // c2->cd();          // Go back to the main canvas before defining pad2
  // TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  // pad2->Draw();
  // pad2->cd(); 
  // frame3->Draw() ;
  // c2 -> Print("mbcfit.pdf");
}