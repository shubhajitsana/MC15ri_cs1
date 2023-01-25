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
void mbc_fit_style(RooRealVar var_name, RooDataSet* dataset_name,
 RooGaussian signal_pdf, RooArgusBG backround_pdf, RooAddPdf added_pdf)
{
  RooPlot* frame = var_name.frame(20) ;
  dataset_name->plotOn(frame) ;
  added_pdf.plotOn(frame) ;
  added_pdf.paramOn(frame,dataset_name,"Fit parameter", 3, "NELU", 0.1, 0.35, 0.9); //Prints the fitted parameter on the canvas
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