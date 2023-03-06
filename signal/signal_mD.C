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

using namespace RooFit ;
using namespace std;

void signal_mD(){
    /*******************Fit Variables***********************************/
    RooRealVar mD("mD","M (GeV)",1.84, 1.89);
    /**defining DATAFRAME{unbinned histogram}(FROM FIT VARIABLE) TO FIT AND PLOT**/
    RooDataSet* data=new RooDataSet("data","data",RooArgSet(mD));
    /*******************Input root file**********************************/
    TChain* chain=new TChain();
    chain->Add("/home/belle2/ssana/MC15ri/signal/signal.root/tree");
    // chain->Add("/home/sana/ssana/fit/signal/signal.root/tree");

    Double_t  de3, md03, mbc3, r23, kid3,pid3,sig,cont_prob;
    Int_t run;
    Int_t nevt3=(int)chain->GetEntries();

    chain->SetBranchAddress("isSignal",&sig);
    // chain->SetBranchAddress("deltaE",&de3);
    // chain->SetBranchAddress("Mbc",&mbc3);
    chain->SetBranchAddress("D0_bar_InvM",&md03);
    // chain->SetBranchAddress("ContProb",&cont_prob);
    // chain->SetBranchAddress("Kp_PID_bin_kaon",&kid3);    

    //Loading data 
    Double_t counter =0;
    for(int l=0;l<nevt3;l++) {
      chain->GetEntry(l);
      mD.setVal(md03);
      if(sig==1){// && md03>1.84 && md03<1.89 && mbc3>5.23 && mbc3 < 5.30 && de3 < 0.3 && de3 > -0.3 && kid3 > 0.6 && cont_prob < 0.86&& r23 < 0.3  )// && (run <= 1702 || run >= 1835))
          data->add(RooArgSet(mD));
          counter++;
      }
    }

    /*****************************Delta E Fit***********************/
    // --- Build Signal PDF ---
    RooRealVar mean1("mean1","mean of Gaussian-1",1.86466,1.84,1.89);
    RooRealVar mean2("mean2","mean of Gaussian-2",1.86484,1.84,1.89);
    RooRealVar sigma1("sigma1","sigma of Gaussian-1",0.00324467, 0., 10);	
    RooRealVar sigma2("sigma2","sigma of Gaussian-2",0.0132255,0.,10);

    RooGaussian sig1("sig1","Gaussian-1",mD,mean1,sigma1);  
    RooGaussian sig2("sig2","Gaussian-2",mD,mean2,sigma2);

    RooRealVar fsig_1("frac_gaussians", "signal fraction", 0.759039,0.,1.);
    RooAddPdf twoGaussians("twoGaussians", "sum of two Gaussians ",RooArgList(sig1, sig2), RooArgList(fsig_1));

    twoGaussians.fitTo(*data);
    /****************************FIT COMPLETE*************************************/

    /*********************Start Plotting and showing outpouts*****************/
    //Plotting fitted result
    RooPlot* deframe = mD.frame(Title("Fitting M of #bar{D}^{0} of signal events"), Bins(588)) ;                          
    data->plotOn(deframe, Binning(588), DataError(RooAbsData::SumW2)) ;
    twoGaussians.plotOn(deframe, LineColor(kBlue)	, LineStyle(kSolid)) ;
    twoGaussians.paramOn(deframe,data,"", 2, "NU", 0.1, 0.3, 0.9); //"NELU",  Prints the fitted parameter on the canvas
    twoGaussians.plotOn(deframe,Components(sig1),LineColor(kGreen),LineStyle(kDashed)) ;
    twoGaussians.plotOn(deframe,Components(sig2),LineColor(kBlack),LineStyle(kDashed)) ;
    twoGaussians.plotOn(deframe,Components(twoGaussians),LineColor(kRed),LineStyle(kDashed));

    //Extract info. from fitting frame and showing
    cout<<"chisq of the fit is : "<<deframe->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof : "<<deframe->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
    RooHist* hpull = deframe->pullHist() ;
    RooPlot* frame3 = mD.frame(Title("Pull Distribution")) ;
    hpull->SetFillColor(1);
    frame3->addPlotable(hpull,"X0B"); // "X0" is for errorbar; and "B" is for histograms



    TCanvas* c1 = new TCanvas("c1", "c1", 2550, 1500) ;
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();  
    deframe->Draw() ;
    // Adding legend
    TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
    TLegendEntry *entry = legend1->AddEntry("sig1","1st Gaussian pdf","l");
    entry->SetLineColor(kGreen);
    entry->SetLineStyle(kDashed);
    entry = legend1->AddEntry("sig2","2nd Gaussian pdf","l");
    entry->SetLineColor(kBlack);
    entry->SetLineStyle(kDashed);
    entry = legend1->AddEntry("twoGaussians","Combined signal pdf","l");
    entry->SetLineColor(kRed);
    entry->SetLineStyle(kDashed);
    legend1->Draw();

    c1->cd();          // Go back to the main canvas before defining pad2
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

    c1->Print("signal_mD/mD_fit_5.png");
    cout<<"Total number of events which are used to fitting are : "<<counter<<endl;
    cout<<"chisq of the fit is :"<<deframe->chiSquare()<<endl;//chi-square of the fit
    cout<<"chi-square/ndof :"<<deframe->chiSquare(7)<<endl;// Chi^2/(the number of degrees of freedom)
}