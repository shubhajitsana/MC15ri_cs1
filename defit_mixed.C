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

void defit_mixed(){
    /*******************Fit Variables***********************************/
    RooRealVar deltae_mixed("deltae","#DeltaE (GeV)", -0.1, 0.1);
    /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
    RooDataSet* data_mixed=new RooDataSet("data","data",RooArgSet(deltae_mixed));
    /*******************Input root file**********************************/
    TChain* chain_mixed=new TChain();
    chain_mixed->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/mixed.root/tree");

    Double_t  de3_mixed, md03_mixed, mbc3_mixed, r23_mixed, kid3_mixed,pid3_mixed,sig_mixed,cont_prob_mixed;
    Int_t run_mixed;
    Int_t nevt3_mixed=(int)chain_mixed->GetEntries();

    // chain_mixed->SetBranchAddress("isSignal",&sig);
    chain_mixed->SetBranchAddress("deltaE",&de3_mixed);
    chain_mixed->SetBranchAddress("Mbc",&mbc3_mixed);
    chain_mixed->SetBranchAddress("D0_bar_InvM",&md03_mixed);
    chain_mixed->SetBranchAddress("Kp_PID_bin_kaon",&kid3_mixed);
    chain_mixed->SetBranchAddress("ContProb",&cont_prob_mixed);
    // chain_mixed->SetBranchAddress("R2",&r23);
    // chain_mixed->SetBranchAddress("pi_PID_bin_pion",&pid3);
    // chain_mixed->SetBranchAddress("__run__",&run);
    
    // D0_bar_InvM >1.84 && D0_bar_InvM <1.89 && Mbc>5.27 && Mbc < 5.29 && deltaE < 0.1 && deltaE > -0.1 && Kp_PID_bin_kaon > 0.6 && ContProb < 0.86
    //Loading data 
    Double_t counter_mixed =0;
    for(int l=0;l<nevt3_mixed;l++) {
        chain_mixed->GetEntry(l);
        deltae_mixed.setVal(de3_mixed);
        if(md03_mixed>1.85 && md03_mixed<1.88 && mbc3_mixed>5.23 && mbc3_mixed < 5.29 && de3_mixed < 0.1 && de3_mixed > -0.1 && kid3_mixed > 0.6 && cont_prob_mixed < 0.6){
            data_mixed->add(RooArgSet(deltae_mixed));
            counter_mixed++;
        }
    }

    /*****************************Delta E Fit***********************/
    // --- Build Argus background PDF ---
    RooRealVar b1_mixed("Chbyshv-prm", "Chbyshv-prm", -0.062, -10., 10.);
    RooChebychev sum_mixed("bkg","Background",deltae_mixed,RooArgSet(b1_mixed)) ;
    sum_mixed.fitTo(*data_mixed);
    /****************************FIT COMPLETE*************************************/

    /*********************Start Plotting and showing outpouts*****************/
    //Plotting fitted result
    RooPlot* deframe_mixed = deltae_mixed.frame(Title("Fitting #DeltaE for B^{0}#bar{B}^{0} background"), Bins(300)) ;                          
    data_mixed->plotOn(deframe_mixed, Binning(300), DataError(RooAbsData::SumW2)) ;
    sum_mixed.plotOn(deframe_mixed, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    sum_mixed.paramOn(deframe_mixed,data_mixed,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas

    //Extract info. from fitting frame and showing
    RooHist* hpull_mixed = deframe_mixed->pullHist() ;
    RooPlot* frame3_mixed = deltae_mixed.frame(Title("Pull Distribution")) ;
    hpull_mixed->SetFillColor(1);
    frame3_mixed->addPlotable(hpull_mixed,"X0B"); // "X0" is for errorbar; and "B" is for histograms


    TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();  
    deframe_mixed->Draw() ;

    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd(); 
    frame3_mixed->Draw() ;
    frame3_mixed->SetLineStyle(9);
    frame3_mixed->GetYaxis()->SetNdivisions(505);
    frame3_mixed->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    frame3_mixed->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
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

    c1->Print("de_plot/de_fit_mixed.png");
}