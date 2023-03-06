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

void mbcfit_signalfolder(){
  /*******************Fit Variables***********************************/
  RooRealVar mbc_signalfolder("mbc","M_{bc} (GeV)",5.23,5.29);
  /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
  RooDataSet* data_signalfolder=new RooDataSet("data","data",RooArgSet(mbc_signalfolder));
  /*******************Input root file**********************************/
  TChain* chain_signalfolder=new TChain();
  chain_signalfolder->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/scaled_signal.root/tree");

  Double_t  de3_signalfolder, md03_signalfolder, mbc3_signalfolder, r23_signalfolder, kid3_signalfolder,pid3_signalfolder,sig_signalfolder,cont_prob_signalfolder;
  Int_t run_signalfolder;
  Int_t nevt3_signalfolder=(int)chain_signalfolder->GetEntries();

  // chain->SetBranchAddress("isSignal",&sig);
  chain_signalfolder->SetBranchAddress("deltaE",&de3_signalfolder);
  chain_signalfolder->SetBranchAddress("Mbc",&mbc3_signalfolder);
  chain_signalfolder->SetBranchAddress("D0_bar_InvM",&md03_signalfolder);
  chain_signalfolder->SetBranchAddress("ContProb",&cont_prob_signalfolder);
  chain_signalfolder->SetBranchAddress("Kp_PID_bin_kaon",&kid3_signalfolder);
  // chain->SetBranchAddress("R2",&r23);
  // chain->SetBranchAddress("pi_PID_bin_pion",&pid3);
  // chain->SetBranchAddress("__run__",&run);
  

  //Loading data 
  Double_t counter_signalfolder =0;
  for(int l=0;l<nevt3_signalfolder;l++) {
    chain_signalfolder->GetEntry(l);
    mbc_signalfolder.setVal(mbc3_signalfolder);
    if(md03_signalfolder>1.85 && md03_signalfolder<1.88 && mbc3_signalfolder>5.23 && mbc3_signalfolder < 5.29 && de3_signalfolder < 0.1 && de3_signalfolder > -0.1 && kid3_signalfolder > 0.6 && cont_prob_signalfolder < 0.6){
      data_signalfolder->add(RooArgSet(mbc_signalfolder));
      counter_signalfolder++;
    }
  }

  /*****************************Mbc Fit***********************/
  // --- Build Signal PDF ---
  RooRealVar mean1_signalfolder("mean1","mean of Gaussian-1",5.279145,5.27,5.29);
  RooRealVar mean2_signalfolder("mean2","mean of Gaussian-2",5.279260,5.27,5.29);
  RooRealVar sigma1_signalfolder("sigma1","sigma of Gaussian-1",0., 0., 1);	
  RooRealVar sigma2_signalfolder("sigma2","sigma of Gaussian-2",0.01191,0.00000001,1);

  RooGaussian sig1_signalfolder("sig1","Gaussian-1",mbc_signalfolder,mean1_signalfolder,sigma1_signalfolder);  
  RooGaussian sig2_signalfolder("sig2","Gaussian-2",mbc_signalfolder,mean2_signalfolder,sigma2_signalfolder);

  RooRealVar fsig_1_signalfolder("frac_gaussians", "signal fraction", 0.5,0.,1.);
  RooAddPdf sum_signalfolder("twoGaussians", "sum of two Gaussians ",RooArgList(sig1_signalfolder, sig2_signalfolder), RooArgList(fsig_1_signalfolder));
  sum_signalfolder.fitTo(*data_signalfolder);
  /****************************FIT COMPLETE*************************************/
  //Plotting fitted result
  RooPlot* deframe_signalfolder = mbc_signalfolder.frame(Title("Fitting M_{bc} for signal MC"), Bins(300)) ;                          
  data_signalfolder->plotOn(deframe_signalfolder, Binning(300), DataError(RooAbsData::SumW2)) ;
  sum_signalfolder.plotOn(deframe_signalfolder,Components(sig1_signalfolder),LineColor(kGreen),LineStyle(kDashed)) ;
  sum_signalfolder.plotOn(deframe_signalfolder,Components(sig2_signalfolder),LineColor(kBlack),LineStyle(kDashed)) ;
  sum_signalfolder.plotOn(deframe_signalfolder, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
  sum_signalfolder.paramOn(deframe_signalfolder,data_signalfolder,"", 2, "NEU", 0.1, 0.35, 0.9); //"NELU",  Prints the fitted parameter on the canvas

  //Extract info. from fitting frame and showing
  RooHist* hpull_signalfolder = deframe_signalfolder->pullHist() ;
  RooPlot* frame3_signalfolder = mbc_signalfolder.frame(Title("Pull Distribution")) ;
  hpull_signalfolder->SetFillColor(1);
  frame3_signalfolder->addPlotable(hpull_signalfolder,"X0B"); // "X0" is for errorbar; and "B" is for histograms

  TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();  
  deframe_signalfolder->Draw() ;
  // Adding legend
  TLegend *legend1 = new TLegend(0.35,0.7,0.5,0.9);
  TLegendEntry *entry = legend1->AddEntry("sig1_signalfolder","1st Gaussian pdf","l");
  entry->SetLineColor(kGreen);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sig2_signalfolder","2nd Gaussian pdf","l");
  entry->SetLineColor(kBlack);
  entry->SetLineStyle(kDashed);
  entry = legend1->AddEntry("sum_signalfolder","Fitted pdf","l");
  entry->SetLineColor(kBlue);
  entry->SetLineStyle(kSolid);
  legend1->Draw();

  // // Adding arrow at (+-)3*sigma of signal pdf
  // double weightedMean_signalfolder = mean1_signalfolder.getVal()*fsig_1.getVal() + mean2_signalfolder.getVal()*(1.0-fsig_1.getVal());
  // double weightedSigma_signalfolder = std::sqrt( pow(sigma1_signalfolder.getVal(),2)*pow(fsig_1_signalfolder.getVal(),2)  +  pow(sigma2_signalfolder.getVal(),2)*pow((1-fsig_1_signalfolder.getVal()),2));
  // double mbc_min_fit_signalfolder = weightedMean_signalfolder -3*weightedSigma_signalfolder; 
  // double mbc_max_fit_signalfolder = weightedMean_signalfolder +3*weightedSigma_signalfolder;
  // double arrowHeight_signalfolder = n_sig_signalfolder.getVal()*0.03;
  // cout<<"Mbc min :"<< mbc_min_fit_signalfolder <<endl;
  // cout<<"Mbc max :"<< mbc_max_fit_signalfolder <<endl;
  // cout<<"Arrow Height :"<< arrowHeight_signalfolder <<endl;

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
  frame3_signalfolder->Draw() ;
  frame3_signalfolder->SetLineStyle(9);
  frame3_signalfolder->GetYaxis()->SetNdivisions(505);
  frame3_signalfolder->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
  frame3_signalfolder->GetXaxis()->SetTitle("M_{bc} (GeV)"); 
  frame3_signalfolder->GetXaxis()->SetTitleSize(0.13);
  frame3_signalfolder->GetYaxis()->SetTitleSize(0.15);
  frame3_signalfolder->GetXaxis()->SetLabelSize(0.120);
  frame3_signalfolder->GetYaxis()->SetLabelSize(0.120); 
  frame3_signalfolder->GetXaxis()->SetTitleOffset(0.90);      
  frame3_signalfolder->GetYaxis()->SetTitleOffset(0.22);       
  frame3_signalfolder->GetYaxis()->SetRangeUser(-10.0, 10.0);       
  frame3_signalfolder->GetYaxis()->SetLimits(-10.0, 10.0);       
  frame3_signalfolder->GetXaxis()->SetNdivisions(505);
  frame3_signalfolder->GetYaxis()->CenterTitle(true);
  frame3_signalfolder->GetXaxis()->CenterTitle(true);
  frame3_signalfolder->Draw("AXISSAME");

  cout<<"Total number of events which are used to fitting are : "<<counter_signalfolder<<endl;
  cout<<"chisq of the fit is :"<<deframe_signalfolder->chiSquare()<<endl;//chi-square of the fit
  cout<<"chi-square/ndof :"<<deframe_signalfolder->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
  
  c1->Print("mbc_plot/mbc_fit_signalfolder.png");
}