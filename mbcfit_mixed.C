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

void mbcfit_mixed(){
  /*******************Fit Variables***********************************/
  RooRealVar mbc_mixed("mbc","M_{bc} (GeV)",5.23,5.29);
  /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
  RooDataSet* data_mixed=new RooDataSet("data","data",RooArgSet(mbc_mixed));
  /*******************Input root file**********************************/
  TChain* chain_mixed=new TChain();
  chain_mixed->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/mixed.root/tree");

  Double_t  de3_mixed, md03_mixed, mbc3_mixed, r23_mixed, kid3_mixed,pid3_mixed,sig_mixed,cont_prob_mixed;
  Int_t run_mixed;
  Int_t nevt3_mixed=(int)chain_mixed->GetEntries();

  // chain->SetBranchAddress("isSignal",&sig);
  chain_mixed->SetBranchAddress("deltaE",&de3_mixed);
  chain_mixed->SetBranchAddress("Mbc",&mbc3_mixed);
  chain_mixed->SetBranchAddress("D0_bar_InvM",&md03_mixed);
  chain_mixed->SetBranchAddress("ContProb",&cont_prob_mixed);
  chain_mixed->SetBranchAddress("Kp_PID_bin_kaon",&kid3_mixed);
  // chain->SetBranchAddress("R2",&r23);
  // chain->SetBranchAddress("pi_PID_bin_pion",&pid3);
  // chain->SetBranchAddress("__run__",&run);
  

  //Loading data 
  Double_t counter_mixed =0;
  for(int l=0;l<nevt3_mixed;l++) {
    chain_mixed->GetEntry(l);
    mbc_mixed.setVal(mbc3_mixed);
    if(md03_mixed>1.85 && md03_mixed<1.88 && mbc3_mixed>5.23 && mbc3_mixed < 5.29 && de3_mixed < 0.1 && de3_mixed > -0.1 && kid3_mixed > 0.6 && cont_prob_mixed < 0.6){
      data_mixed->add(RooArgSet(mbc_mixed));
      counter_mixed++;
    }
  }

  /*****************************Mbc Fit***********************/
  // --- Build Signal PDF ---
  RooRealVar mean1_mixed("mean1","mean of Gaussian-1",5.279145,5.27,5.29);
  RooRealVar mean2_mixed("mean2","mean of Gaussian-2",5.279260,5.27,5.29);
  RooRealVar sigma1_mixed("sigma1","sigma of Gaussian-1",0., 0., 1);	
  RooRealVar sigma2_mixed("sigma2","sigma of Gaussian-2",0.01191,0.00000001,1);

  RooGaussian sig1_mixed("sig1","Gaussian-1",mbc_mixed,mean1_mixed,sigma1_mixed);  
  RooGaussian sig2_mixed("sig2","Gaussian-2",mbc_mixed,mean2_mixed,sigma2_mixed);

  RooRealVar fsig_1_mixed("frac_gaussians", "signal fraction", 0.5,0.,1.);
  RooAddPdf twoGaussians_mixed("twoGaussians", "sum of two Gaussians ",RooArgList(sig1_mixed, sig2_mixed), RooArgList(fsig_1_mixed));

  // --- Build Argus background PDF ---
  RooRealVar argpar_mixed("argpar","argus shape parameter",-34.70,-100.,-1.) ;
  RooArgusBG bkg_mixed("argus","Argus PDF",mbc_mixed,RooConst(5.29),argpar_mixed) ; //mbc background

  //Initialization of parameter before adding two pdf
  // cout<<"Total number of events which are used to fitting are : "<<counter<<endl;
  Double_t event_count_mixed = counter_mixed; 
  Double_t signal_count_mixed = counter_mixed*0.2;
  Double_t back_count_mixed = counter_mixed*0.8;
  RooRealVar n_sig_mixed("n_sig", "n_sig", signal_count_mixed, 0., event_count_mixed);//52000
  RooRealVar n_bkg_mixed("n_bkg", "n_bkg", back_count_mixed, 0., event_count_mixed);//95000
  RooAddPdf sum_mixed("sum","sum",RooArgList(twoGaussians_mixed,bkg_mixed),RooArgList(n_sig_mixed, n_bkg_mixed));//adding two pdf
  sum_mixed.fitTo(*data_mixed);
  /****************************FIT COMPLETE*************************************/
  //Plotting fitted result
  RooPlot* deframe_mixed = mbc_mixed.frame(Title("Fitting M_{bc} for B^{0}#bar{B}^{0} background"), Bins(300)) ;                          
  data_mixed->plotOn(deframe_mixed, Binning(300), DataError(RooAbsData::SumW2)) ;
  sum_mixed.plotOn(deframe_mixed,Components(sig1_mixed),LineColor(kGreen),LineStyle(kDashed)) ;
  sum_mixed.plotOn(deframe_mixed,Components(sig2_mixed),LineColor(kBlack),LineStyle(kDashed)) ;
  sum_mixed.plotOn(deframe_mixed,Components(twoGaussians_mixed),LineColor(kRed),LineStyle(kDashed));
  sum_mixed.plotOn(deframe_mixed,Components(bkg_mixed),LineColor(kMagenta),LineStyle(kDashed)) ;
  sum_mixed.plotOn(deframe_mixed, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
  sum_mixed.paramOn(deframe_mixed,data_mixed,"", 2, "NEU", 0.1, 0.35, 0.9); //"NELU",  Prints the fitted parameter on the canvas

  //Extract info. from fitting frame and showing
  RooHist* hpull_mixed = deframe_mixed->pullHist() ;
  RooPlot* frame3_mixed = mbc_mixed.frame(Title("Pull Distribution")) ;
  hpull_mixed->SetFillColor(1);
  frame3_mixed->addPlotable(hpull_mixed,"X0B"); // "X0" is for errorbar; and "B" is for histograms
  // frame3->addPlotable(hpull,"P");


  TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();  
  deframe_mixed->Draw() ;
  // Adding legend
  TLegend *legend1 = new TLegend(0.35,0.7,0.5,0.9);
  TLegendEntry *entry = legend1->AddEntry("sig1_mixed","1st Gaussian pdf","l");
  entry->SetLineColor(kGreen);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sig2_mixed","2nd Gaussian pdf","l");
  entry->SetLineColor(kBlack);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("twoGaussians_mixed","Combined signal pdf","l");
  entry->SetLineColor(kRed);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("bkg_mixed","ARGUS","l");
  entry->SetLineColor(kMagenta);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sum_mixed","Fitted pdf","l");
  entry->SetLineColor(kBlue);
  entry->SetLineStyle(kSolid);
  legend1->Draw();

  // // Adding arrow at (+-)3*sigma of signal pdf
  // double weightedMean_mixed = mean1_mixed.getVal()*fsig_1.getVal() + mean2_mixed.getVal()*(1.0-fsig_1.getVal());
  // double weightedSigma_mixed = std::sqrt( pow(sigma1_mixed.getVal(),2)*pow(fsig_1_mixed.getVal(),2)  +  pow(sigma2_mixed.getVal(),2)*pow((1-fsig_1_mixed.getVal()),2));
  // double mbc_min_fit_mixed = weightedMean_mixed -3*weightedSigma_mixed; 
  // double mbc_max_fit_mixed = weightedMean_mixed +3*weightedSigma_mixed;
  // double arrowHeight_mixed = n_sig_mixed.getVal()*0.03;
  // cout<<"Mbc min :"<< mbc_min_fit_mixed <<endl;
  // cout<<"Mbc max :"<< mbc_max_fit_mixed <<endl;
  // cout<<"Arrow Height :"<< arrowHeight_mixed <<endl;

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
  frame3_mixed->Draw() ;
  frame3_mixed->SetLineStyle(9);
  frame3_mixed->GetYaxis()->SetNdivisions(505);
  frame3_mixed->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
  frame3_mixed->GetXaxis()->SetTitle("M_{bc} (GeV)"); 
  frame3_mixed->GetXaxis()->SetTitleSize(0.13);
  frame3_mixed->GetYaxis()->SetTitleSize(0.15);
  frame3_mixed->GetXaxis()->SetLabelSize(0.120);
  frame3_mixed->GetYaxis()->SetLabelSize(0.120); 
  frame3_mixed->GetXaxis()->SetTitleOffset(0.90);      
  frame3_mixed->GetYaxis()->SetTitleOffset(0.22);       
  frame3_mixed->GetYaxis()->SetRangeUser(-10.0, 10.0);       
  frame3_mixed->GetYaxis()->SetLimits(-10.0, 10.0);       
  frame3_mixed->GetXaxis()->SetNdivisions(505);
  frame3_mixed->GetYaxis()->CenterTitle(true);
  frame3_mixed->GetXaxis()->CenterTitle(true);
  frame3_mixed->Draw("AXISSAME");

  cout<<"Total number of events which are used to fitting are : "<<counter_mixed<<endl;
  cout<<"chisq of the fit is :"<<deframe_mixed->chiSquare()<<endl;//chi-square of the fit
  cout<<"chi-square/ndof :"<<deframe_mixed->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
  
  c1->Print("mbc_plot/mbc_fit_mixed.png");
}