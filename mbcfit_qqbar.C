//#ifndef __CINT__
#include "RooGlobalFunc.h"
//#endif
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
#include<cmath>
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooCategory.h"
#include "RooSuperCategory.h"
#include "RooSimultaneous.h"
#include "RooNLLVar.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <string>
using namespace RooFit ;
using namespace std;
//int main(){

void mbcfit_qqbar(){
  /*******************Fit Variables***********************************/
  RooRealVar mbc_qqbar("mbc","M_{bc} (GeV)",5.23,5.29);
  /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
  RooDataSet* data_qqbar=new RooDataSet("data","data",RooArgSet(mbc_qqbar));
  /*******************Input root file**********************************/
  TChain* chain_qqbar=new TChain();
  chain_qqbar->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/qqbar.root/tree");

  Double_t  de3_qqbar, md03_qqbar, mbc3_qqbar, r23_qqbar, kid3_qqbar,pid3_qqbar,sig_qqbar,cont_prob_qqbar;
  Int_t run_qqbar;
  Int_t nevt3_qqbar=(int)chain_qqbar->GetEntries();

  // chain->SetBranchAddress("isSignal",&sig);
  chain_qqbar->SetBranchAddress("deltaE",&de3_qqbar);
  chain_qqbar->SetBranchAddress("Mbc",&mbc3_qqbar);
  chain_qqbar->SetBranchAddress("D0_bar_InvM",&md03_qqbar);
  chain_qqbar->SetBranchAddress("ContProb",&cont_prob_qqbar);
  chain_qqbar->SetBranchAddress("Kp_PID_bin_kaon",&kid3_qqbar);
  // chain->SetBranchAddress("R2",&r23);
  // chain->SetBranchAddress("pi_PID_bin_pion",&pid3);
  // chain->SetBranchAddress("__run__",&run);
  

  //Loading data 
  Double_t counter_qqbar =0;
  for(int l=0;l<nevt3_qqbar;l++) {
    chain_qqbar->GetEntry(l);
    mbc_qqbar.setVal(mbc3_qqbar);
    if(md03_qqbar>1.85 && md03_qqbar<1.88 && mbc3_qqbar>5.23 && mbc3_qqbar < 5.29 && de3_qqbar < 0.1 && de3_qqbar > -0.1 && kid3_qqbar > 0.6 && cont_prob_qqbar < 0.6){
      data_qqbar->add(RooArgSet(mbc_qqbar));
      counter_qqbar++;
    }
  }

  /*****************************Mbc Fit***********************/
  // --- Build Signal PDF ---
  RooRealVar mean1_qqbar("mean1","mean of Gaussian-1",5.279145,5.27,5.29);
  RooRealVar sigma1_qqbar("sigma1","sigma of Gaussian-1",0., 0., 1);	
  RooGaussian sig1_qqbar("sig1","Gaussian-1",mbc_qqbar,mean1_qqbar,sigma1_qqbar);  

  // --- Build Argus background PDF ---
  RooRealVar argpar_qqbar("argpar","argus shape parameter",-34.70,-100.,-1.) ;
  RooArgusBG bkg_qqbar("argus","Argus PDF",mbc_qqbar,RooConst(5.29),argpar_qqbar) ; //mbc background

  RooRealVar fbkg_1_qqbar("frac_argus", "argus fraction", 0.13,0.,1.);
  RooAddPdf sum_qqbar("sum","sum",RooArgList(sig1_qqbar,bkg_qqbar),RooArgList(fbkg_1_qqbar));//adding two pdf
  sum_qqbar.fitTo(*data_qqbar);
  /****************************FIT COMPLETE*************************************/
  //Plotting fitted result
  RooPlot* deframe_qqbar = mbc_qqbar.frame(Title("Fitting M_{bc} for qqbar background"), Bins(300)) ;                          
  data_qqbar->plotOn(deframe_qqbar, Binning(300), DataError(RooAbsData::SumW2)) ;
  sum_qqbar.plotOn(deframe_qqbar,Components(sig1_qqbar),LineColor(kGreen),LineStyle(kDashed)) ;
  sum_qqbar.plotOn(deframe_qqbar,Components(bkg_qqbar),LineColor(kMagenta),LineStyle(kDashed)) ;
  sum_qqbar.plotOn(deframe_qqbar, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
  sum_qqbar.paramOn(deframe_qqbar,data_qqbar,"", 2, "NEU", 0.1, 0.35, 0.9); //"NELU",  Prints the fitted parameter on the canvas

  //Extract info. from fitting frame and showing
  RooHist* hpull_qqbar = deframe_qqbar->pullHist() ;
  RooPlot* frame3_qqbar = mbc_qqbar.frame(Title("Pull Distribution")) ;
  hpull_qqbar->SetFillColor(1);
  frame3_qqbar->addPlotable(hpull_qqbar,"X0B"); // "X0" is for errorbar; and "B" is for histograms


  TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();  
  deframe_qqbar->Draw() ;
  // Adding legend
  TLegend *legend1 = new TLegend(0.35,0.7,0.5,0.9);
  TLegendEntry *entry = legend1->AddEntry("sig1_qqbar","1st Gaussian pdf","l");
  entry->SetLineColor(kGreen);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("bkg_qqbar","ARGUS","l");
  entry->SetLineColor(kMagenta);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sum_qqbar","Fitted pdf","l");
  entry->SetLineColor(kBlue);
  entry->SetLineStyle(kSolid);
  legend1->Draw();

  // // Adding arrow at (+-)3*sigma of signal pdf
  // double weightedMean_qqbar = mean1_qqbar.getVal()*fsig_1.getVal() + mean2_qqbar.getVal()*(1.0-fsig_1.getVal());
  // double weightedSigma_qqbar = std::sqrt( pow(sigma1_qqbar.getVal(),2)*pow(fsig_1_qqbar.getVal(),2)  +  pow(sigma2_qqbar.getVal(),2)*pow((1-fsig_1_qqbar.getVal()),2));
  // double mbc_min_fit_qqbar = weightedMean_qqbar -3*weightedSigma_qqbar; 
  // double mbc_max_fit_qqbar = weightedMean_qqbar +3*weightedSigma_qqbar;
  // double arrowHeight_qqbar = n_sig_qqbar.getVal()*0.03;
  // cout<<"Mbc min :"<< mbc_min_fit_qqbar <<endl;
  // cout<<"Mbc max :"<< mbc_max_fit_qqbar <<endl;
  // cout<<"Arrow Height :"<< arrowHeight_qqbar <<endl;

	// TArrow *arr1 = new TArrow(-0.01, 100, -0.01, 400, 0.01,"|>");
	// arr1->SetLineWidth(4);
  // arr1->SetLineColor(2);
  // arr1->SetFillStyle(3008);
  // arr1->DrawArrow(mbc_min_fit, arrowHeight*0.1, mbc_min_fit, arrowHeight, 0.02,"<"); 
  // arr1->DrawArrow(mbc_max_fit, arrowHeight*0.1, mbc_max_fit, arrowHeight, 0.02,"<");

  c1->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->Draw();
  pad2->cd(); 
  frame3_qqbar->Draw() ;
  frame3_qqbar->SetLineStyle(9);
  frame3_qqbar->GetYaxis()->SetNdivisions(505);
  frame3_qqbar->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
  frame3_qqbar->GetXaxis()->SetTitle("M_{bc} (GeV)"); 
  frame3_qqbar->GetXaxis()->SetTitleSize(0.13);
  frame3_qqbar->GetYaxis()->SetTitleSize(0.15);
  frame3_qqbar->GetXaxis()->SetLabelSize(0.120);
  frame3_qqbar->GetYaxis()->SetLabelSize(0.120); 
  frame3_qqbar->GetXaxis()->SetTitleOffset(0.90);      
  frame3_qqbar->GetYaxis()->SetTitleOffset(0.22);       
  frame3_qqbar->GetYaxis()->SetRangeUser(-10.0, 10.0);       
  frame3_qqbar->GetYaxis()->SetLimits(-10.0, 10.0);       
  frame3_qqbar->GetXaxis()->SetNdivisions(505);
  frame3_qqbar->GetYaxis()->CenterTitle(true);
  frame3_qqbar->GetXaxis()->CenterTitle(true);
  frame3_qqbar->Draw("AXISSAME");

  cout<<"Total number of events which are used to fitting are : "<<counter_qqbar<<endl;
  cout<<"chisq of the fit is :"<<deframe_qqbar->chiSquare()<<endl;//chi-square of the fit
  cout<<"chi-square/ndof :"<<deframe_qqbar->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
  
  c1->Print("mbc_plot/mbc_fit_qqbar.png");
}