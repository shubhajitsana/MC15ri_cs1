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

void defit_charged_mixed_qqbar_signal(){
    /*******************Fit Variables***********************************/
    RooRealVar deltae("deltae","#DeltaE (GeV)", -0.1, 0.1);
    /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
    RooDataSet* data=new RooDataSet("data","data",RooArgSet(deltae));
    /*******************Input root file**********************************/
    TChain* chain=new TChain();
    chain->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/all_without_sig.root/tree");

    Double_t  de3, md03, mbc3, r23, kid3,pid3,sig,cont_prob;
    Int_t run;
    Int_t nevt3=(int)chain->GetEntries();

    // chain->SetBranchAddress("isSignal",&sig);
    chain->SetBranchAddress("deltaE",&de3);
    chain->SetBranchAddress("Mbc",&mbc3);
    chain->SetBranchAddress("D0_bar_InvM",&md03);
    chain->SetBranchAddress("Kp_PID_bin_kaon",&kid3);
    chain->SetBranchAddress("ContProb",&cont_prob);
    // chain->SetBranchAddress("R2",&r23);
    // chain->SetBranchAddress("pi_PID_bin_pion",&pid3);
    // chain->SetBranchAddress("__run__",&run);
    
    // D0_bar_InvM >1.84 && D0_bar_InvM <1.89 && Mbc>5.27 && Mbc < 5.29 && deltaE < 0.1 && deltaE > -0.1 && Kp_PID_bin_kaon > 0.6 && ContProb < 0.86
    //Loading data 
    Double_t counter =0;
    for(int l=0;l<nevt3;l++) {
        chain->GetEntry(l);
        deltae.setVal(de3);
        if(md03>1.85 && md03<1.88 && mbc3>5.23 && mbc3 < 5.29 && de3 < 0.1 && de3 > -0.1 && kid3 > 0.6 && cont_prob < 0.6){
            data->add(RooArgSet(deltae));
            counter++;
        }
    }

    /*****************************Delta E Fit***********************/
    // --- Build Signal PDF ---
    RooRealVar mean1("mean1","mean of Gaussian-1",-0.001,-0.02,0.02);
    RooRealVar mean2("mean2","mean of Gaussian-2",0.002,-0.02,0.02);
    RooRealVar sigma1("sigma1","sigma of Gaussian-1",0., 0., 1);	
    RooRealVar sigma2("sigma2","sigma of Gaussian-2",0.01191,0.00000001,1);

    RooGaussian sig1("sig1","Gaussian-1",deltae,mean1,sigma1);  
    RooGaussian sig2("sig2","Gaussian-2",deltae,mean2,sigma2);

    RooRealVar fsig_1("frac_gaussians", "signal fraction", 0.5,0.,1.);
    RooAddPdf twoGaussians("twoGaussians", "sum of two Gaussians ",RooArgList(sig1, sig2), RooArgList(fsig_1));

    // --- Build Argus background PDF ---
    RooRealVar b1("Chbyshv-prm", "Chbyshv-prm", -0.062, -10., 10.);
    RooChebychev bkg("bkg","Background",deltae,RooArgSet(b1)) ;

    //Initialization of parameter before adding two pdf
    // cout<<"Total number of events which are used to fitting are : "<<counter<<endl;
    Double_t event_count = counter; 
    Double_t signal_count = counter*0.2;
    Double_t back_count = counter*0.8;
    RooRealVar n_sig("n_sig", "n_sig", signal_count, 0., event_count);//52000
    RooRealVar n_bkg("n_bkg", "n_bkg", back_count, 0., event_count);//95000
    RooAddPdf sum("sum","sum",RooArgList(twoGaussians,bkg),RooArgList(n_sig, n_bkg));//adding two pdf
    sum.fitTo(*data);
    /****************************FIT COMPLETE*************************************/

    /*********************Start Plotting and showing outpouts*****************/
    //Plotting fitted result
    RooPlot* deframe = deltae.frame(Title("Fitting #DeltaE of B^{#pm}"), Bins(200)) ;                          
    data->plotOn(deframe, Binning(200), DataError(RooAbsData::SumW2)) ;
    // sum.plotOn(deframe, LineColor(kBlue)	, LineStyle(kSolid)) ;
    // sum.paramOn(deframe,data,"", 2, Format("NEU"),AutoPrecision(1)), 0.7, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas
    // sum.plotOn(deframe,Components(sig1),LineColor(kGreen),LineStyle(kDashed)) ;
    // sum.plotOn(deframe,Components(sig2),LineColor(kBlack),LineStyle(kDashed)) ;
    // sum.plotOn(deframe,Components(twoGaussians),LineColor(kRed),LineStyle(kDashed));
    // sum.plotOn(deframe,Components(bkg),LineColor(kMagenta),LineStyle(kDashed)) ;
    sum.plotOn(deframe, LineColor(kBlue), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    // sum.paramOn(deframe,data,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas

    //Extract info. from fitting frame and showing
    cout<<"chisq of the fit is : "<<deframe->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof : "<<deframe->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
    RooHist* hpull = deframe->pullHist() ;
    RooPlot* frame3 = deltae.frame(Title("Pull Distribution")) ;
    hpull->SetFillColor(kBlue);
    frame3->addPlotable(hpull,"X0B"); // "X0" is for errorbar; and "B" is for histograms


    TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();  
    deframe->Draw() ;
    // Adding legend
    TLegend *legend1 = new TLegend(0.1,0.7,0.3,0.9);
    // TLegendEntry *entry = legend1->AddEntry("sig1","1st Gaussian pdf","l");
    // entry->SetLineColor(kGreen);
    // entry->SetLineStyle(kDashed);
    // entry = legend1->AddEntry("sig2","2nd Gaussian pdf","l");
    // entry->SetLineColor(kBlack);
    // entry->SetLineStyle(kDashed);
    // entry = legend1->AddEntry("twoGaussians","Combined signal pdf","l");
    // entry->SetLineColor(kRed);
    // entry->SetLineStyle(kDashed);
    // entry = legend1->AddEntry("bkg","bkg-Chebyshev","l");
    // entry->SetLineColor(kMagenta);
    // entry->SetLineStyle(kDashed);
    TLegendEntry *entry = legend1->AddEntry("sum","Fitted pdf for all events(Chsv+G+G)","l");
    entry->SetLineColor(kBlue);
    entry->SetLineStyle(kSolid);
    // legend1->Draw();

    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->Draw();
    pad2->cd(); 
    frame3->Draw() ;
    frame3->SetLineStyle(9);
    frame3->GetYaxis()->SetNdivisions(505);
    frame3->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    frame3->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
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

    cout<<"Total number of events which are used to fit for all data are : "<<counter<<endl;
    cout<<"chisq of the fit is :"<<deframe->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof :"<<deframe->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)

    //////////////////////////end for all files///////////////////////////////
    ////////////////////////////start of charged events////////////////////////
    /*******************Fit Variables***********************************/
    RooRealVar deltae_charged("deltae","#DeltaE (GeV)", -0.1, 0.1);
    /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
    RooDataSet* data_charged=new RooDataSet("data","data",RooArgSet(deltae_charged));
    /*******************Input root file**********************************/
    TChain* chain_charged=new TChain();
    chain_charged->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/charged.root/tree");

    Double_t  de3_charged, md03_charged, mbc3_charged, r23_charged, kid3_charged,pid3_charged,sig_charged,cont_prob_charged;
    Int_t run_charged;
    Int_t nevt3_charged=(int)chain_charged->GetEntries();

    // chain_charged->SetBranchAddress("isSignal",&sig);
    chain_charged->SetBranchAddress("deltaE",&de3_charged);
    chain_charged->SetBranchAddress("Mbc",&mbc3_charged);
    chain_charged->SetBranchAddress("D0_bar_InvM",&md03_charged);
    chain_charged->SetBranchAddress("Kp_PID_bin_kaon",&kid3_charged);
    chain_charged->SetBranchAddress("ContProb",&cont_prob_charged);
    // chain_charged->SetBranchAddress("R2",&r23);
    // chain_charged->SetBranchAddress("pi_PID_bin_pion",&pid3);
    // chain_charged->SetBranchAddress("__run__",&run);
    
    // D0_bar_InvM >1.84 && D0_bar_InvM <1.89 && Mbc>5.27 && Mbc < 5.29 && deltaE < 0.1 && deltaE > -0.1 && Kp_PID_bin_kaon > 0.6 && ContProb < 0.86
    //Loading data 
    Double_t counter_charged =0;
    for(int l=0;l<nevt3_charged;l++) {
        chain_charged->GetEntry(l);
        deltae_charged.setVal(de3_charged);
        if(md03_charged>1.85 && md03_charged<1.88 && mbc3_charged>5.23 && mbc3_charged < 5.29 && de3_charged < 0.1 && de3_charged > -0.1 && kid3_charged > 0.6 && cont_prob_charged < 0.6){
            data_charged->add(RooArgSet(deltae_charged));
            counter_charged++;
        }
    }

    /*****************************Delta E Fit***********************/
    // --- Build Signal PDF ---
    RooRealVar mean1_charged("mean1","mean of Gaussian-1",-0.001,-0.02,0.02);
    RooRealVar mean2_charged("mean2","mean of Gaussian-2",0.002,-0.02,0.02);
    RooRealVar sigma1_charged("sigma1","sigma of Gaussian-1",0., 0., 1);	
    RooRealVar sigma2_charged("sigma2","sigma of Gaussian-2",0.01191,0.00000001,1);

    RooGaussian sig1_charged("sig1","Gaussian-1",deltae_charged,mean1_charged,sigma1_charged);  
    RooGaussian sig2_charged("sig2","Gaussian-2",deltae_charged,mean2_charged,sigma2_charged);

    RooRealVar fsig_1_charged("frac_gaussians", "signal fraction", 0.5,0.,1.);
    RooAddPdf twoGaussians_charged("twoGaussians", "sum of two Gaussians ",RooArgList(sig1_charged, sig2_charged), RooArgList(fsig_1_charged));

    // --- Build Argus background PDF ---
    RooRealVar b1_charged("Chbyshv-prm", "Chbyshv-prm", -0.062, -10., 10.);
    RooChebychev bkg_charged("bkg","Background",deltae_charged,RooArgSet(b1_charged)) ;

    //Initialization of parameter before adding two pdf
    // cout<<"Total number of events which are used to fitting are : "<<counter_charged<<endl;
    Double_t event_count_charged = counter_charged; 
    Double_t signal_count_charged = counter_charged*0.2;
    Double_t back_count_charged = counter_charged*0.8;
    RooRealVar n_sig_charged("n_sig", "n_sig", signal_count_charged, 0., event_count_charged);//52000
    RooRealVar n_bkg_charged("n_bkg", "n_bkg", back_count_charged, 0., event_count_charged);//95000
    RooAddPdf sum_charged("sum","sum",RooArgList(twoGaussians_charged,bkg_charged),RooArgList(n_sig_charged, n_bkg_charged));//adding two pdf
    sum_charged.fitTo(*data_charged);
    /****************************FIT COMPLETE*************************************/

    /*********************Start Plotting and showing outpouts*****************/
    //Plotting fitted result
    RooPlot* deframe_charged = deltae_charged.frame(Title("Fitting #DeltaE of B^{#plus}B^{#minus}"), Bins(200)) ;                          
    data_charged->plotOn(deframe_charged, Binning(200), DataError(RooAbsData::SumW2)) ;
    // sum_charged.plotOn(deframe_charged,Components(sig1_charged),LineColor(kGreen),LineStyle(kDashed)) ;
    // sum_charged.plotOn(deframe_charged,Components(sig2_charged),LineColor(kBlack),LineStyle(kDashed)) ;
    // sum_charged.plotOn(deframe_charged,Components(twoGaussians_charged),LineColor(kRed),LineStyle(kDashed));
    // sum_charged.plotOn(deframe_charged,Components(bkg_charged),LineColor(kMagenta),LineStyle(kDashed)) ;
    sum_charged.plotOn(deframe_charged, LineColor(kGreen), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    // sum_charged.paramOn(deframe_charged,data_charged,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas

    //Extract info. from fitting frame and showing
    RooHist* hpull_charged = deframe_charged->pullHist() ;
    RooPlot* frame3_charged = deltae_charged.frame(Title("Pull Distribution")) ;
    hpull_charged->SetFillColor(kGreen);
    frame3_charged->addPlotable(hpull_charged,"X0B"); // "X0" is for errorbar; and "B" is for histograms


    c1->cd();
    pad1->cd();  
    deframe_charged->Draw("SAME") ;
    // Adding legend
    // TLegend *legend1_charged = new TLegend(0.1,0.7,0.25,0.9);
    // TLegendEntry *entry_charged = legend1_charged->AddEntry("sig1_charged","1st Gaussian pdf","l");
    // entry_charged->SetLineColor(kGreen);
    // entry_charged->SetLineStyle(kDashed);
    // entry_charged = legend1_charged->AddEntry("sig2_charged","2nd Gaussian pdf","l");
    // entry_charged->SetLineColor(kBlack);
    // entry_charged->SetLineStyle(kDashed);
    // entry_charged = legend1_charged->AddEntry("twoGaussians_charged","Combined signal pdf","l");
    // entry_charged->SetLineColor(kRed);
    // entry_charged->SetLineStyle(kDashed);
    // entry_charged = legend1_charged->AddEntry("bkg_charged","bkg-Chebyshev","l");
    // entry_charged->SetLineColor(kMagenta);
    // entry_charged->SetLineStyle(kDashed);
    entry = legend1->AddEntry("sum_charged","Fitted pdf for B^{#plus}B^{#minus}(Chsv+G+G)","l");
    entry->SetLineColor(kGreen);
    entry->SetLineStyle(kSolid);
    // legend1->Modified();

    c1->cd();          // Go back to the main canvas before defining pad2
    pad2->cd(); 
    frame3_charged->Draw("SAME") ;
    // frame3_charged->SetLineStyle(9);
    // frame3_charged->GetYaxis()->SetNdivisions(505);
    // frame3_charged->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    // frame3_charged->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
    // frame3_charged->GetXaxis()->SetTitleSize(0.13);
    // frame3_charged->GetYaxis()->SetTitleSize(0.15);
    // frame3_charged->GetXaxis()->SetLabelSize(0.120);
    // frame3_charged->GetYaxis()->SetLabelSize(0.120); 
    // frame3_charged->GetXaxis()->SetTitleOffset(0.90);      
    // frame3_charged->GetYaxis()->SetTitleOffset(0.22);       
    // frame3_charged->GetYaxis()->SetRangeUser(-10.0, 10.0);       
    // frame3_charged->GetYaxis()->SetLimits(-10.0, 10.0);       
    // frame3_charged->GetXaxis()->SetNdivisions(505);
    // frame3_charged->GetYaxis()->CenterTitle(true);
    // frame3_charged->GetXaxis()->CenterTitle(true);
    // frame3_charged->Draw("AXISSAME");

    cout<<"Total number of events which are used to fit B+- events are : "<<counter_charged<<endl;
    cout<<"chisq of the fit is :"<<deframe_charged->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof :"<<deframe_charged->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)

    ///////////////////////////////// end of charged files////////////////////////////
    ///////////////////////////////// start of mixed files////////////////////////////
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
    //Plotting fitted result
    RooPlot* deframe_mixed = deltae_mixed.frame(Title("Fitting #DeltaE of B^{0}#bar{B}^{0} background"), Bins(200)) ;                          
    data_mixed->plotOn(deframe_mixed, Binning(200), DataError(RooAbsData::SumW2)) ;
    sum_mixed.plotOn(deframe_mixed, LineColor(kRed), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    // sum_mixed.paramOn(deframe_mixed,data_mixed,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas





    //Extract info. from fitting frame and showing
    RooHist* hpull_mixed = deframe_mixed->pullHist() ;
    RooPlot* frame3_mixed = deltae_mixed.frame(Title("Pull Distribution")) ;
    hpull_mixed->SetFillColor(kRed);
    frame3_mixed->addPlotable(hpull_mixed,"X0B"); // "X0" is for errorbar; and "B" is for histograms


    c1->cd();
    pad1->cd();  
    deframe_mixed->Draw("SAME") ;
    entry = legend1->AddEntry("sum_mixed","Fitted pdf for B^{0}#bar{B}^{0}(Chsv)","l");
    entry->SetLineColor(kRed);
    entry->SetLineStyle(kSolid);
    // legend1->Modified();

    c1->cd();          // Go back to the main canvas before defining pad2
    pad2->cd(); 
    frame3_mixed->Draw("SAME") ;
    // frame3_mixed->SetLineStyle(9);
    // frame3_mixed->GetYaxis()->SetNdivisions(505);
    // frame3_mixed->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    // frame3_mixed->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
    // frame3_mixed->GetXaxis()->SetTitleSize(0.13);
    // frame3_mixed->GetYaxis()->SetTitleSize(0.15);
    // frame3_mixed->GetXaxis()->SetLabelSize(0.120);
    // frame3_mixed->GetYaxis()->SetLabelSize(0.120); 
    // frame3_mixed->GetXaxis()->SetTitleOffset(0.90);      
    // frame3_mixed->GetYaxis()->SetTitleOffset(0.22);       
    // frame3_mixed->GetYaxis()->SetRangeUser(-10.0, 10.0);       
    // frame3_mixed->GetYaxis()->SetLimits(-10.0, 10.0);       
    // frame3_mixed->GetXaxis()->SetNdivisions(505);
    // frame3_mixed->GetYaxis()->CenterTitle(true);
    // frame3_mixed->GetXaxis()->CenterTitle(true);
    // frame3_mixed->Draw("AXISSAME");

    cout<<"Total number of events which are used to fit B0B0bar background are : "<<counter_mixed<<endl;
    cout<<"chisq of the fit is :"<<deframe_mixed->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof :"<<deframe_mixed->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)

    ///////////////////////////////end of mixed files/////////////////////////
    //////////////////////////////start of qqbar backgrounds////////////////////
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
    //Plotting fitted result
    RooPlot* deframe_qqbar = deltae_qqbar.frame(Title("Fitting #DeltaE for qqbar background"), Bins(200)) ;                          
    data_qqbar->plotOn(deframe_qqbar, Binning(200), DataError(RooAbsData::SumW2)) ;
    sum_qqbar.plotOn(deframe_qqbar, LineColor(kMagenta), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    // sum_qqbar.paramOn(deframe_qqbar,data_qqbar,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas



    //Extract info. from fitting frame and showing
    RooHist* hpull_qqbar = deframe_qqbar->pullHist() ;
    RooPlot* frame3_qqbar = deltae_qqbar.frame(Title("Pull Distribution")) ;
    hpull_qqbar->SetFillColor(kMagenta);
    frame3_qqbar->addPlotable(hpull_qqbar,"X0B"); // "X0" is for errorbar; and "B" is for histograms


    c1->cd();
    pad1->cd();   
    deframe_qqbar->Draw("SAME") ;
    entry = legend1->AddEntry("sum_qqbar","Fitted pdf for qqbar(Chsv)","l");
    entry->SetLineColor(kMagenta);
    entry->SetLineStyle(kSolid);
    // legend1->Modified();

    c1->cd();          // Go back to the main canvas before defining pad2
    pad2->cd(); 
    frame3_qqbar->Draw("SAME") ;
    // frame3_qqbar->SetLineStyle(9);
    // frame3_qqbar->GetYaxis()->SetNdivisions(505);
    // frame3_qqbar->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    // frame3_qqbar->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
    // frame3_qqbar->GetXaxis()->SetTitleSize(0.13);
    // frame3_qqbar->GetYaxis()->SetTitleSize(0.15);
    // frame3_qqbar->GetXaxis()->SetLabelSize(0.120);
    // frame3_qqbar->GetYaxis()->SetLabelSize(0.120); 
    // frame3_qqbar->GetXaxis()->SetTitleOffset(0.90);      
    // frame3_qqbar->GetYaxis()->SetTitleOffset(0.22);       
    // frame3_qqbar->GetYaxis()->SetRangeUser(-10.0, 10.0);       
    // frame3_qqbar->GetYaxis()->SetLimits(-10.0, 10.0);       
    // frame3_qqbar->GetXaxis()->SetNdivisions(505);
    // frame3_qqbar->GetYaxis()->CenterTitle(true);
    // frame3_qqbar->GetXaxis()->CenterTitle(true);
    // frame3_qqbar->Draw("AXISSAME");

    cout<<"Total number of events which are used to fit qqbar backgrounds are : "<<counter_qqbar<<endl;
    cout<<"chisq of the fit is :"<<deframe_qqbar->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof :"<<deframe_qqbar->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)

    ///////////////////////////////////end of qqbar background//////////////////////
    /////////////////////////////////starting of signal events //////////////// 
    /*******************Fit Variables***********************************/
    RooRealVar deltae_signalfolder("deltae","#DeltaE (GeV)", -0.1, 0.1);
    /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
    RooDataSet* data_signalfolder=new RooDataSet("data","data",RooArgSet(deltae_signalfolder));
    /*******************Input root file**********************************/
    TChain* chain_signalfolder=new TChain();
    chain_signalfolder->Add("/home/belle2/ssana/MC15ri/cs_fom_data/combined/scaled_signal.root/tree");

    Double_t  de3_signalfolder, md03_signalfolder, mbc3_signalfolder, r23_signalfolder, kid3_signalfolder,pid3_signalfolder,sig_signalfolder,cont_prob_signalfolder;
    Int_t run_signalfolder;
    Int_t nevt3_signalfolder=(int)chain_signalfolder->GetEntries();

    // chain_signalfolder->SetBranchAddress("isSignal",&sig);
    chain_signalfolder->SetBranchAddress("deltaE",&de3_signalfolder);
    chain_signalfolder->SetBranchAddress("Mbc",&mbc3_signalfolder);
    chain_signalfolder->SetBranchAddress("D0_bar_InvM",&md03_signalfolder);
    chain_signalfolder->SetBranchAddress("Kp_PID_bin_kaon",&kid3_signalfolder);
    chain_signalfolder->SetBranchAddress("ContProb",&cont_prob_signalfolder);
    // chain_signalfolder->SetBranchAddress("R2",&r23);
    // chain_signalfolder->SetBranchAddress("pi_PID_bin_pion",&pid3);
    // chain_signalfolder->SetBranchAddress("__run__",&run);
    
    // D0_bar_InvM >1.84 && D0_bar_InvM <1.89 && Mbc>5.27 && Mbc < 5.29 && deltaE < 0.1 && deltaE > -0.1 && Kp_PID_bin_kaon > 0.6 && ContProb < 0.86
    //Loading data 
    Double_t counter_signalfolder =0;
    for(int l=0;l<nevt3_signalfolder;l++) {
        chain_signalfolder->GetEntry(l);
        deltae_signalfolder.setVal(de3_signalfolder);
        if(md03_signalfolder>1.85 && md03_signalfolder<1.88 && mbc3_signalfolder>5.23 && mbc3_signalfolder < 5.29 && de3_signalfolder < 0.1 && de3_signalfolder > -0.1 && kid3_signalfolder > 0.6 && cont_prob_signalfolder < 0.6){
            data_signalfolder->add(RooArgSet(deltae_signalfolder));
            counter_signalfolder++;
        }
    }

    /*****************************Delta E Fit***********************/
    // --- Build Signal PDF ---
    RooRealVar mean1_signalfolder("mean1","mean of Gaussian-1",-0.001,-0.02,0.02);
    RooRealVar mean2_signalfolder("mean2","mean of Gaussian-2",0.002,-0.02,0.02);
    RooRealVar sigma1_signalfolder("sigma1","sigma of Gaussian-1",0., 0., 1);	
    RooRealVar sigma2_signalfolder("sigma2","sigma of Gaussian-2",0.01191,0.00000001,1);

    RooGaussian sig1_signalfolder("sig1","Gaussian-1",deltae_signalfolder,mean1_signalfolder,sigma1_signalfolder);  
    RooGaussian sig2_signalfolder("sig2","Gaussian-2",deltae_signalfolder,mean2_signalfolder,sigma2_signalfolder);

    RooRealVar fsig_1_signalfolder("frac_gaussians", "signal fraction", 0.9,0.,1.);
    RooAddPdf sum_signalfolder("twoGaussians", "sum of two Gaussians ",RooArgList(sig1_signalfolder, sig2_signalfolder), RooArgList(fsig_1_signalfolder));
    sum_signalfolder.fitTo(*data_signalfolder);
    /****************************FIT COMPLETE*************************************/
    //Plotting fitted result
    RooPlot* deframe_signalfolder = deltae_signalfolder.frame(Title("Fitting #DeltaE signal MC"), Bins(200)) ;                          
    data_signalfolder->plotOn(deframe_signalfolder, Binning(200), DataError(RooAbsData::SumW2)) ;
    // sum_signalfolder.plotOn(deframe_signalfolder,Components(sig1_signalfolder),LineColor(kGreen),LineStyle(kDashed)) ;
    // sum_signalfolder.plotOn(deframe_signalfolder,Components(sig2_signalfolder),LineColor(kBlack),LineStyle(kDashed)) ;
    sum_signalfolder.plotOn(deframe_signalfolder, LineColor(kCyan), LineStyle(kSolid)) ; // we need to write it last.
                        // otherwise pull distribution will calculate error wrt last mentioned pdf inside plotOn function
    // sum_signalfolder.paramOn(deframe_signalfolder,data_signalfolder,"", 2, "NEU", 0.63, 0.9, 0.9); //"NELU",  Prints the fitted parameter on the canvas

    //Extract info. from fitting frame and showing
    RooHist* hpull_signalfolder = deframe_signalfolder->pullHist() ;
    RooPlot* frame3_signalfolder = deltae_signalfolder.frame(Title("Pull Distribution")) ;
    hpull_signalfolder->SetFillColor(kCyan);
    frame3_signalfolder->addPlotable(hpull_signalfolder,"X0B"); // "X0" is for errorbar; and "B" is for histograms

    c1->cd();
    pad1->cd();  
    deframe_signalfolder->Draw("SAME") ;
    // Adding legend
    // TLegend *legend1_signalfolder = new TLegend(0.1,0.7,0.25,0.9);
    // TLegendEntry *entry_signalfolder = legend1_signalfolder->AddEntry("sig1_signalfolder","1st Gaussian pdf","l");
    // entry_signalfolder->SetLineColor(kGreen);
    // entry_signalfolder->SetLineStyle(kDashed);
    // entry_signalfolder = legend1_signalfolder->AddEntry("sig2_signalfolder","2nd Gaussian pdf","l");
    // entry_signalfolder->SetLineColor(kBlack);
    // entry_signalfolder->SetLineStyle(kDashed);
    entry = legend1->AddEntry("sum_signalfolder","Fitted pdf for signal MC(G+G)","l");
    entry->SetLineColor(kCyan);
    entry->SetLineStyle(kSolid);
    legend1->Draw();

    c1->cd();          // Go back to the main canvas before defining pad2
    pad2->cd(); 
    frame3_signalfolder->Draw("SAME") ;
    // frame3_signalfolder->SetLineStyle(9);
    // frame3_signalfolder->GetYaxis()->SetNdivisions(505);
    // frame3_signalfolder->GetYaxis()->SetTitle("#sqrt{#chi^{2}}"); 
    // frame3_signalfolder->GetXaxis()->SetTitle("#DeltaE (GeV)"); 
    // frame3_signalfolder->GetXaxis()->SetTitleSize(0.13);
    // frame3_signalfolder->GetYaxis()->SetTitleSize(0.15);
    // frame3_signalfolder->GetXaxis()->SetLabelSize(0.120);
    // frame3_signalfolder->GetYaxis()->SetLabelSize(0.120); 
    // frame3_signalfolder->GetXaxis()->SetTitleOffset(0.90);      
    // frame3_signalfolder->GetYaxis()->SetTitleOffset(0.22);       
    // frame3_signalfolder->GetYaxis()->SetRangeUser(-10.0, 10.0);       
    // frame3_signalfolder->GetYaxis()->SetLimits(-10.0, 10.0);       
    // frame3_signalfolder->GetXaxis()->SetNdivisions(505);
    // frame3_signalfolder->GetYaxis()->CenterTitle(true);
    // frame3_signalfolder->GetXaxis()->CenterTitle(true);
    // frame3_signalfolder->Draw("AXISSAME");

    cout<<"Total number of events which are used to fit signal events are : "<<counter_signalfolder<<endl;
    cout<<"chisq of the fit is :"<<deframe_signalfolder->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof :"<<deframe_signalfolder->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)

    //////////////////////////////end of signal events//////////////////////////////////
    c1->Print("de_plot/de_fit_charged_mixed_qqbar_signal.png");
}