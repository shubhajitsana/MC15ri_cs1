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

void defit_qqbar(){
    /*******************Fit Variables***********************************/
    RooRealVar deltae_qqbar("deltae","#DeltaE (GeV)", -0.1, 0.1);
    /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
    RooDataSet* data_qqbar=new RooDataSet("data","data",RooArgSet(deltae_qqbar));
    /*******************Input root file**********************************/
    TChain* chain_qqbar=new TChain();
    chain_qqbar->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/qqbar.root/tree");

    Double_t  de3_qqbar, md03_qqbar, mbc3_qqbar, r23_qqbar, kid3_qqbar,pid3_qqbar,sig_qqbar,cont_prob_qqbar;
    Int_t run_qqbar;
    Int_t nevt3_qqbar=(int)chain_qqbar->GetEntries();

    // chain_qqbar->SetBranchAddress("isSignal",&sig);
    chain_qqbar->SetBranchAddress("deltaE",&de3_qqbar);
    chain_qqbar->SetBranchAddress("Mbc",&mbc3_qqbar);
    chain_qqbar->SetBranchAddress("D0_bar_InvM",&md03_qqbar);
    chain_qqbar->SetBranchAddress("Kp_PID_bin_kaon",&kid3_qqbar);
    chain_qqbar->SetBranchAddress("ContProb",&cont_prob_qqbar);
    // chain_qqbar->SetBranchAddress("R2",&r23);
    // chain_qqbar->SetBranchAddress("pi_PID_bin_pion",&pid3);
    // chain_qqbar->SetBranchAddress("__run__",&run);
    
    // D0_bar_InvM >1.84 && D0_bar_InvM <1.89 && Mbc>5.27 && Mbc < 5.29 && deltaE < 0.1 && deltaE > -0.1 && Kp_PID_bin_kaon > 0.6 && ContProb < 0.86
    //Loading data 
    Double_t counter_qqbar =0;
    for(int l=0;l<nevt3_qqbar;l++) {
        chain_qqbar->GetEntry(l);
        deltae_qqbar.setVal(de3_qqbar);
        if(md03_qqbar>1.85 && md03_qqbar<1.88 && mbc3_qqbar>5.23 && mbc3_qqbar < 5.29 && de3_qqbar < 0.1 && de3_qqbar > -0.1 && kid3_qqbar > 0.6 && cont_prob_qqbar < 0.6){
            data_qqbar->add(RooArgSet(deltae_qqbar));
            counter_qqbar++;
        }
    }

    /*****************************Delta E Fit***********************/
    // --- Build Argus background PDF ---
    RooRealVar b1_qqbar("Chbyshv-prm", "Chbyshv-prm", -0.062, -10., 10.);
    RooChebychev sum_qqbar("bkg","Background",deltae_qqbar,RooArgSet(b1_qqbar)) ;
    sum_qqbar.fitTo(*data_qqbar);
    /****************************FIT COMPLETE*************************************/

    /*********************Start Plotting and showing outpouts*****************/
    //Plotting fitted result
    RooPlot* deframe_qqbar = deltae_qqbar.frame(Title("Fitting #DeltaE for qqbar background"), Bins(300)) ;                          
    data_qqbar->plotOn(deframe_qqbar, Binning(300), DataError(RooAbsData::SumW2)) ;
    sum_qqbar.plotOn(deframe_qqbar, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    sum_qqbar.paramOn(deframe_qqbar,data_qqbar,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas

    //Extract info. from fitting frame and showing
    RooHist* hpull_qqbar = deframe_qqbar->pullHist() ;
    RooPlot* frame3_qqbar = deltae_qqbar.frame(Title("Pull Distribution")) ;
    hpull_qqbar->SetFillColor(1);
    frame3_qqbar->addPlotable(hpull_qqbar,"X0B"); // "X0" is for errorbar; and "B" is for histograms


    TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();  
    deframe_qqbar->Draw() ;

    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd(); 
    frame3_qqbar->Draw() ;
    frame3_qqbar->SetLineStyle(9);
    frame3_qqbar->GetYaxis()->SetNdivisions(505);
    frame3_qqbar->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    frame3_qqbar->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
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

    c1->Print("de_plot/de_fit_qqbar.png");
}