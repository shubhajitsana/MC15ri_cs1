#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TFile.h"
#include "TF1.h"
#include "TROOT.h"
#include "TNtuple.h"
#include <algorithm>
#include "TLegend.h"
#include "TLorentzVector.h"
#include "TH1D.h"
using namespace RooFit ;

void dbkskpi_kp()
{

/*************** The code is orignally written for d2kpi tree. **************/
/* The names for brenches are same, Just change tree entry while linking*/

/*	pid : D0 = 421 	k- = -321		pi+ = 211		pi0 = 111		gaama = 22	psi(3770) = 30443	
	e- = 11	nu_e = 12		mu- = 13	nu_mu = 14 	*/	


  TFile* file_QCMC1  = new TFile(	"DRoot_files/psippData_combine.root");	
  TTree* trEvent_QCMC1;
  trEvent_QCMC1 = (TTree*)file_QCMC1->Get("dbkskpi");

	int indexmc;		//No of Partiles
	trEvent_QCMC1->SetBranchAddress("indexmc",&indexmc);	
	int n=30;
	int pdgid[n];			//This no is the no of particle produced in a event
	trEvent_QCMC1->SetBranchAddress("pdgid",&pdgid);	
	double xmbc_d2kpi;
	trEvent_QCMC1->SetBranchAddress("xmbc_dbkskpi",&xmbc_d2kpi);	
	double xmde_d2kpi;
	trEvent_QCMC1->SetBranchAddress("xmde_dbkskpi",&xmde_d2kpi);	
	double ckan_dbkskpi;	
	trEvent_QCMC1->SetBranchAddress("ckan_dbkskpi",&ckan_dbkskpi);	
	double xmks_dbkskpi;
	trEvent_QCMC1->SetBranchAddress("xmks_dbkskpi",&xmks_dbkskpi);
	double eraw_dbkskpi;
	trEvent_QCMC1->SetBranchAddress("eraw_dbkskpi",&eraw_dbkskpi);

  RooRealVar deltae("deltae", "#Delta E", -0.1, 0.1);	
  RooDataSet dataIMC("dataIMC", "dataIMC", RooArgSet(deltae));


	double event_count=0;
	int kaon_charge = -1;

	int entries_QCMC1 = trEvent_QCMC1->GetEntries();
	cout<< " entries_QCMC1 : " <<entries_QCMC1 <<endl;
	
	//Event Loop
	for(int i=0 ; i< entries_QCMC1; i++){
		trEvent_QCMC1 -> GetEntry(i);
		
			if(ckan_dbkskpi != kaon_charge) continue;
			if(eraw_dbkskpi > 0.0 ) continue;
			if(xmks_dbkskpi < 0.485 && xmks_dbkskpi > 0.510) continue;
			event_count++;
			
			deltae = xmde_d2kpi ;
			dataIMC.add(RooArgSet(deltae));
	
	}
  /**************************************************************************/
	int backg = event_count*0.2;
	int total_entries = event_count;
	cout << "Total Entries : "<< total_entries << endl;

  RooDataHist* binDataSet = new RooDataHist("binDataSet", "binDataSet", deltae, dataIMC);

  // Drawing the histogram
  TCanvas *c1 = new TCanvas("c1", "", 500, 500);  
  // dataIMC.plotOn(	xframe1	, Binning(200)		, DataError(RooAbsData::SumW2));
  // RooPlot* xframe1 = deltae.frame(	Title(" ")	, Bins(200));

  RooPlot* xframe1 = deltae.frame(	Title(" ")	, Bins(200));
  binDataSet->plotOn(	xframe1	, Binning(200)		, DataError(RooAbsData::SumW2));
  xframe1->Draw();
  c1 -> Print("dbkskpi_dE1.png");
  /********************************************fit start**********************************/
////////////////Signal/////////////////
  RooRealVar mean1("mean1","mean of Gaussian-1",-0.001,-0.1,0.1);
  RooRealVar mean2("mean2","mean of Gaussian-2",0.002,-0.1,0.1);
  
  RooRealVar sigma1("sigma1","sigma of Gaussian-1",0.0004	,0.00000001,0.1);	
  RooRealVar sigma2("sigma2","sigma of Gaussian-2",0.001	,0.00000001,0.1);
  RooRealVar fsig1("fsig1", "signal fraction", 0.4,0.,1.);

  RooGaussian sig1("sig1","Gaussian-1",deltae,mean1,sigma1);  
  RooGaussian sig2("sig2","Gaussian-2",deltae,mean2,sigma2);

  RooAddPdf twoGaussians("twoGaussians", "sum of two Gaussians ",RooArgList(sig1, sig2), RooArgList(fsig1));
//////////////////////////////////////////


///////////////Background///////////////// 
  RooRealVar p1("p1","coeff #1", 0., -100., 100.);
  RooRealVar p2("p2","coeff #2", 0., -10000., 10000.);
  
  RooPolynomial bkg("bkg","bkgd pdf", deltae, RooArgList(p1,p2));
//////////////////////////////////////////

  RooRealVar nsig("nsig","number of sig events",total_entries - backg	 ,0.0 ,total_entries);
  RooRealVar nbkg("nbkg","number of bkg events",backg ,0.0 ,total_entries);

	RooAddPdf model2("model"," two-Gaussians + background polynomial PDF's",RooArgList(bkg,twoGaussians), RooArgList(nbkg,nsig));

  RooFitResult *result = model2.fitTo(dataIMC	, Range(-0.1,0.1));		//,RooFit::Save(kTRUE));


  /***********************************Drawing start****************************/
  RooPlot* xframe = deltae.frame(	Title(" ")	, Bins(200));
  dataIMC.plotOn(	xframe	, Binning(200)		, DataError(RooAbsData::SumW2));
  model2.plotOn(	xframe	, LineColor(kRed)	, LineStyle(1));

  RooHist* hpull = xframe->pullHist();
  RooPlot* frame2 = deltae.frame(Title(" "));
  frame2->addPlotable(hpull,"P");

  double weightedMean = mean1.getVal()*fsig1.getVal() + mean2.getVal()*(1.0-fsig1.getVal());
  double weightedSigma = std::sqrt( pow(sigma1.getVal(),2)*fsig1.getVal()  +  pow(sigma2.getVal(),2)*(1-fsig1.getVal())      );
  
  double Emin_fit = weightedMean -3*weightedSigma; 
  double Emax_fit = weightedMean +3*weightedSigma;
  double arrowHeight = nsig.getVal()*0.03;

  cout<<"Emin :"<< Emin_fit <<endl;
  cout<<"Emax :"<< Emax_fit <<endl;
  cout<<"Arrow Height :"<< arrowHeight <<endl;

  cout<<"chisq of the fit is :"<<xframe->chiSquare()<<endl;
  cout<<"Fit chi square/dof :"<<xframe->chiSquare(7)<<endl;
  double chisq_PerDOF = xframe->chiSquare(7);     // You have to get the chi-square of the fit from MassFrame - the 7 tells RooFit to take into account the 7 fit parameters when calculating the number of degrees of freedom
  
  model2.plotOn(xframe,Components(bkg),LineColor(kBlue),LineStyle(9));	//blue
  xframe->Draw();

  model2.plotOn(xframe,Components("sig1,sig2"),LineColor(6),LineStyle(9)) ;	//pink
  xframe->Draw() ;

//  model2.plotOn(xframe,Components(sig2),LineColor(7),LineStyle(9)) ;	//cyan
//  xframe->Draw() ;

      TCanvas *c2 = new TCanvas("c2", "", 500, 500);  
   
   
      TPad *pad1 = new TPad("pad1", "This is pad1",0,0.3065693,1,1);      
      TPad *pad2 = new TPad("pad2", "This is pad2",0,0,1,0.30);		//x1,y1,x2,y2
      pad1->Draw();
      pad2->Draw();
      pad1->cd();
      pad1->SetTopMargin(0.05);
      pad1->SetBottomMargin(0.02);	//(0.003105263);   		//Sets bottom margin of pad1
      pad1->SetLeftMargin(0.15);
      xframe->Draw();      

      //pt->Draw(); 
      //pt->SetBorderSize(0);
      //pt->SetFillColor(10);
      //xframe->SetMinimum(0.0001);
      xframe->GetYaxis()->SetTitle("Events/(1.00 MeV)");
      xframe->GetYaxis()->SetTitleSize(0.06);
      xframe->GetYaxis()->SetTitleOffset(1.3);
      xframe->GetYaxis()->SetLabelSize(0.05);
      xframe->GetYaxis()->CenterTitle(true);
      xframe->GetYaxis()->SetNdivisions(505);   
      xframe->GetXaxis()->SetNdivisions(400);   	//Wanted to remove x axis, it's working. 
      xframe->Draw("AXISSAME");
      pad1->Modified();
      
   TLegend *legend = new TLegend(0.70,0.78,0.90,0.95);		// l: horizental line, e: vertical line, p: point
//   legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
	legend->AddEntry(binDataSet,"Data","ep");	

	TLegendEntry *entry = legend->AddEntry("model2","fit","l");
	   entry->SetLineColor(kRed);
	   entry->SetLineStyle(1);
   
	entry = legend->AddEntry("sig1,sig2","Signal","l");
	   entry->SetLineColor(6);
	   entry->SetLineStyle(9);
//	   entry->SetLineWidth(1);
//	   entry->SetMarkerColor(1);
//	   entry->SetMarkerStyle(21);
//	   entry->SetMarkerSize(1);
//	   entry->SetTextFont(62);

	entry = legend->AddEntry("bkg","Combinatorial bkg","l");
	   entry->SetLineColor(kBlue);
	   entry->SetLineStyle(9);

	legend->Draw("same");

//	pad1->BuildLegend(0.60,0.75,0.90,0.95);
      
      
//      
  TLegend *leg1 = new TLegend(0.75,0.56,0.88,0.65);
  leg1->SetHeader("Data");
  leg1->SetFillColor(10);
  leg1->SetLineColor(10);
	leg1->Draw("same");
//
//      
  TLegend *leg2 = new TLegend(0.20,0.80,0.45,0.89);
  leg2->SetHeader("#bar{D}^{0} #rightarrow K^{0}_{S}K^{-}#pi^{-}");
  //leg2->SetHeader("#bar{D}^{0} 9999");
  leg2->SetFillColor(10);
  leg2->SetLineColor(10);
	leg2->Draw("same");


//
/*      
    TLegend *leg3 = new TLegend(0.65,0.65,0.88,0.75);
    leg3->SetHeader("#chi^{2}/ndof = ");
    leg3->SetFillColor(10);
    leg3->SetLineColor(10);
	leg3->Draw("same");
*/
	// TLatex *t = new TLatex(0.025,360,Form("#chi^{2}/ndof = %g",chisq_PerDOF));
	// t->Draw("same");
	
	TArrow *arr1 = new TArrow(-0.01, 100, -0.01, 400, 0.01,"|>");
	arr1->SetLineWidth(4);
  arr1->SetLineColor(2);
  // arr1->SetFillColor(4);
  arr1->SetFillStyle(3008);
  // arr->SetArrowSize(5);
  // arr->SetAngle(40);
	// arr->DrawArrow(mean1.getVal(), 0, mean1.getVal(), 400, 0.02,"<"); 
  arr1->DrawArrow(Emin_fit, arrowHeight*0.1, Emin_fit, arrowHeight, 0.02,"<"); 
  arr1->DrawArrow(Emax_fit, arrowHeight*0.1, Emax_fit, arrowHeight, 0.02,"<"); 
  // arr->Draw("same");

  // std::cout <<  "Mean :" <<mean1.getVal();


      gPad->Update();
      gPad->RedrawAxis();
      gPad->Update();
      TLine *line = new TLine();
      line->SetLineWidth(3);
      line->DrawLine(gPad->GetUxmin(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymin());
      line->DrawLine(gPad->GetUxmin(),gPad->GetUymin(),gPad->GetUxmin(),gPad->GetUymax());
      line->DrawLine(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax());
      

      pad2->cd();      
      pad2->Range(0.6860972,-8.426074,0.8030151,3.916962);      
      pad2->SetTopMargin(0.05);
      pad2->SetBottomMargin(0.3505571);
      pad2->SetLeftMargin(0.15);

      frame2->Draw();
      frame2->SetLineStyle(9);
      frame2->GetYaxis()->SetNdivisions(505);
      frame2->GetYaxis()->SetTitle("#sigma_{p}"); 
      frame2->GetXaxis()->SetTitle("#Delta E (GeV)"); 
      frame2->GetXaxis()->SetTitleSize(0.13);
      frame2->GetYaxis()->SetTitleSize(0.15);
      frame2->GetXaxis()->SetLabelSize(0.120);
      frame2->GetYaxis()->SetLabelSize(0.120); 
      frame2->GetXaxis()->SetTitleOffset(0.90);      
      frame2->GetYaxis()->SetTitleOffset(0.22);       
      frame2->GetYaxis()->SetRangeUser(-10.0, 10.0);       
      frame2->GetYaxis()->SetLimits(-10.0, 10.0);       
      frame2->GetXaxis()->SetNdivisions(505);
      frame2->GetYaxis()->CenterTitle(true);
      frame2->GetXaxis()->CenterTitle(true);
      frame2->Draw("AXISSAME");
      
//      TLine *gridline = new TLine(1.83,0.00,1.885,0.00);
//      gridline->Draw();
      
      gPad->SetGridy(1);     
      pad2->Modified();

	c2 -> Print("dbkskpi_dE_c2.tex");
	c2 -> Print("dbkskpi_dE.png");
}
