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

void mbcfit_charged_all(){
  /*******************Fit Variables***********************************/
  RooRealVar mbc_charged("mbc","M_{bc} (GeV)",5.23,5.29);
  /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
  RooDataSet* data_charged=new RooDataSet("data","data",RooArgSet(mbc_charged));
  /*******************Input root file**********************************/
  TChain* chain_charged=new TChain();
  chain_charged->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/charged.root/tree");

  Double_t  de3_charged, md03_charged, mbc3_charged, r23_charged, kid3_charged,pid3_charged,sig_charged,cont_prob_charged;
  Int_t run_charged;
  Int_t nevt3_charged=(int)chain_charged->GetEntries();

  // chain->SetBranchAddress("isSignal",&sig);
  chain_charged->SetBranchAddress("deltaE",&de3_charged);
  chain_charged->SetBranchAddress("Mbc",&mbc3_charged);
  chain_charged->SetBranchAddress("D0_bar_InvM",&md03_charged);
  chain_charged->SetBranchAddress("ContProb",&cont_prob_charged);
  chain_charged->SetBranchAddress("Kp_PID_bin_kaon",&kid3_charged);
  // chain->SetBranchAddress("R2",&r23);
  // chain->SetBranchAddress("pi_PID_bin_pion",&pid3);
  // chain->SetBranchAddress("__run__",&run);
  

  //Loading data 
  Double_t counter_charged =0;
  for(int l=0;l<nevt3_charged;l++) {
    chain_charged->GetEntry(l);
    mbc_charged.setVal(mbc3_charged);
    if(md03_charged>1.85 && md03_charged<1.88 && mbc3_charged>5.23 && mbc3_charged < 5.29 && de3_charged < 0.1 && de3_charged > -0.1 && kid3_charged > 0.6 && cont_prob_charged < 0.6){
      data_charged->add(RooArgSet(mbc_charged));
      counter_charged++;
    }
  }

  /*****************************Mbc Fit***********************/
  // --- Build Signal PDF ---
  RooRealVar mean1_charged("mean1","mean of Gaussian-1",5.279145,5.27,5.29);
  RooRealVar mean2_charged("mean2","mean of Gaussian-2",5.279260,5.27,5.29);
  RooRealVar sigma1_charged("sigma1","sigma of Gaussian-1",0., 0., 1);	
  RooRealVar sigma2_charged("sigma2","sigma of Gaussian-2",0.01191,0.00000001,1);

  RooGaussian sig1_charged("sig1","Gaussian-1",mbc_charged,mean1_charged,sigma1_charged);  
  RooGaussian sig2_charged("sig2","Gaussian-2",mbc_charged,mean2_charged,sigma2_charged);

  RooRealVar fsig_1_charged("frac_gaussians", "signal fraction", 0.5,0.,1.);
  RooAddPdf twoGaussians_charged("twoGaussians", "sum of two Gaussians ",RooArgList(sig1_charged, sig2_charged), RooArgList(fsig_1_charged));

  // --- Build Argus background PDF ---
  RooRealVar argpar_charged("argpar","argus shape parameter",-34.70,-100.,-1.) ;
  RooArgusBG bkg_charged("argus","Argus PDF",mbc_charged,RooConst(5.29),argpar_charged) ; //mbc background

  //Initialization of parameter before adding two pdf
  // cout<<"Total number of events which are used to fitting are : "<<counter<<endl;
  Double_t event_count_charged = counter_charged; 
  Double_t signal_count_charged = counter_charged*0.2;
  Double_t back_count_charged = counter_charged*0.8;
  RooRealVar n_sig_charged("n_sig", "n_sig", signal_count_charged, 0., event_count_charged);//52000
  RooRealVar n_bkg_charged("n_bkg", "n_bkg", back_count_charged, 0., event_count_charged);//95000
  RooAddPdf sum_charged("sum","sum",RooArgList(twoGaussians_charged,bkg_charged),RooArgList(n_sig_charged, n_bkg_charged));//adding two pdf
  sum_charged.fitTo(*data_charged);
  /****************************FIT COMPLETE*************************************/
  //Plotting fitted result
  RooPlot* deframe_charged = mbc_charged.frame(Title("Fitting M_{bc} of B^{#pm}"), Bins(300)) ;                          
  data_charged->plotOn(deframe_charged, Binning(300), DataError(RooAbsData::SumW2)) ;
  // sum.plotOn(deframe, LineColor(kBlue)	, LineStyle(kSolid)) ;
  // sum.paramOn(deframe,data,"", 2, Format("NEU"),AutoPrecision(1)), 0.7, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas
  sum_charged.plotOn(deframe_charged,Components(sig1_charged),LineColor(kGreen),LineStyle(kDashed)) ;
  sum_charged.plotOn(deframe_charged,Components(sig2_charged),LineColor(kBlack),LineStyle(kDashed)) ;
  sum_charged.plotOn(deframe_charged,Components(twoGaussians_charged),LineColor(kRed),LineStyle(kDashed));
  sum_charged.plotOn(deframe_charged,Components(bkg_charged),LineColor(kMagenta),LineStyle(kDashed)) ;
  sum_charged.plotOn(deframe_charged, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
  sum_charged.paramOn(deframe_charged,data_charged,"", 2, "NEU", 0.1, 0.35, 0.9); //"NELU",  Prints the fitted parameter on the canvas

  //Extract info. from fitting frame and showing
  RooHist* hpull_charged = deframe_charged->pullHist() ;
  RooPlot* frame3_charged = mbc_charged.frame(Title("Pull Distribution")) ;
  hpull_charged->SetFillColor(1);
  frame3_charged->addPlotable(hpull_charged,"X0B"); // "X0" is for errorbar; and "B" is for histograms
  // frame3->addPlotable(hpull,"P");


  TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();  
  deframe_charged->Draw() ;
  // Adding legend
  TLegend *legend1 = new TLegend(0.35,0.7,0.5,0.9);
  TLegendEntry *entry = legend1->AddEntry("sig1_charged","1st Gaussian pdf","l");
  entry->SetLineColor(kGreen);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sig2_charged","2nd Gaussian pdf","l");
  entry->SetLineColor(kBlack);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("twoGaussians_charged","Combined signal pdf","l");
  entry->SetLineColor(kRed);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("bkg_charged","ARGUS","l");
  entry->SetLineColor(kMagenta);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sum_charged","Fitted pdf","l");
  entry->SetLineColor(kBlue);
  entry->SetLineStyle(kSolid);
  legend1->Draw();

  // // Adding arrow at (+-)3*sigma of signal pdf
  // double weightedMean_charged = mean1_charged.getVal()*fsig_1.getVal() + mean2_charged.getVal()*(1.0-fsig_1.getVal());
  // double weightedSigma_charged = std::sqrt( pow(sigma1_charged.getVal(),2)*pow(fsig_1_charged.getVal(),2)  +  pow(sigma2_charged.getVal(),2)*pow((1-fsig_1_charged.getVal()),2));
  // double mbc_min_fit_charged = weightedMean_charged -3*weightedSigma_charged; 
  // double mbc_max_fit_charged = weightedMean_charged +3*weightedSigma_charged;
  // double arrowHeight_charged = n_sig_charged.getVal()*0.03;
  // cout<<"Mbc min :"<< mbc_min_fit_charged <<endl;
  // cout<<"Mbc max :"<< mbc_max_fit_charged <<endl;
  // cout<<"Arrow Height :"<< arrowHeight_charged <<endl;

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
  frame3_charged->Draw() ;
  frame3_charged->SetLineStyle(9);
  frame3_charged->GetYaxis()->SetNdivisions(505);
  frame3_charged->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
  frame3_charged->GetXaxis()->SetTitle("M_{bc} (GeV)"); 
  frame3_charged->GetXaxis()->SetTitleSize(0.13);
  frame3_charged->GetYaxis()->SetTitleSize(0.15);
  frame3_charged->GetXaxis()->SetLabelSize(0.120);
  frame3_charged->GetYaxis()->SetLabelSize(0.120); 
  frame3_charged->GetXaxis()->SetTitleOffset(0.90);      
  frame3_charged->GetYaxis()->SetTitleOffset(0.22);       
  frame3_charged->GetYaxis()->SetRangeUser(-10.0, 10.0);       
  frame3_charged->GetYaxis()->SetLimits(-10.0, 10.0);       
  frame3_charged->GetXaxis()->SetNdivisions(505);
  frame3_charged->GetYaxis()->CenterTitle(true);
  frame3_charged->GetXaxis()->CenterTitle(true);
  frame3_charged->Draw("AXISSAME");

  cout<<"Total number of events which are used to fitting are : "<<counter_charged<<endl;
  cout<<"chisq of the fit is :"<<deframe_charged->chiSquare()<<endl;//chi-square of the fit
  cout<<"chi-square/ndof :"<<deframe_charged->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)

  c1->Print("mbc_plot/mbc_fit_charged_all.png");
}