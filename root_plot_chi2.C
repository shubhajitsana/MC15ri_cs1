void chi2(){
    TFile *fa = new TFile("data/combined/all.root");
    TTree *ta = (TTree*)fa->Get("tree");

    Double_t chisq, chi2_prob;
    
    ta->SetBranchAddress("D0_bar_chiSqrd",&chisq);
    ta->SetBranchAddress("D0_bar_chiProb",&chi2_prob);

    TH1F *chi2a = new TH1F("#chi^{2}","#chi^{2} value in absolute scale", 1000, 0, 2000);
    TH1F *chi2la = new TH1F("#chi^{2}","#chi^{2} value in log scale", 1000, 0, 2000);
    TH1F *chi2proba = new TH1F("#chi^{2} Probability","#chi^{2} Probability in absolute scale", 100, 0, 1);
    TH1F *chi2probla = new TH1F("#chi^{2} Probability","#chi^{2} Probability in log scale", 100, 0, 1);

    // Filling the histogram
    for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
        ta->GetEntry(iEvent);
        chi2a->Fill(chisq);
        chi2la->Fill(chisq);
        chi2proba->Fill(chi2_prob);
        chi2probla->Fill(chi2_prob);
    }

    /////////////////////Drawing///////////////////
    TCanvas *c = new TCanvas();


    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.51, 0.49, 1.0);
    pad1->Draw();                     //  xmin, ymin, xmax, ymax
    pad1->cd();
    chi2a->GetYaxis()->SetTitle("Number of Events");
    chi2a->GetXaxis()->SetTitle("#chi^{2} Value");
    chi2a->Draw();
    pad1->Update();                 // Save changes in the pad

    // next pad
    c->cd();            // Go back to the canvas
    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0.5, 1.0, 1.0);
    pad2->Draw();
    pad2->cd();
    chi2proba->GetYaxis()->SetTitle("Number of Events");
    chi2proba->GetXaxis()->SetTitle("#chi^{2} Probability");
    chi2proba->Draw();


    pad2->Update();

    ////////////////next pad//////////////////
    c->cd();
    TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 0.49, 0.49);
    pad3->Draw();
    pad3->cd();
    pad3->SetLogy();
    chi2la->GetYaxis()->SetTitle("Log(Number of Events)");
    chi2la->GetXaxis()->SetTitle("#chi^{2} Value");
    chi2la->Draw();


    pad3->Update();

    ///////////////// next pad///////////////////////
    c->cd();
    TPad *pad4 = new TPad("pad4", "pad4", 0.5, 0, 1.0, 0.49);
    pad4->Draw();
    pad4->cd();
    pad4->SetLogy();
    chi2probla->GetYaxis()->SetTitle("Log(Number of Events)");
    chi2probla->GetXaxis()->SetTitle("#chi^{2} Probability");
    chi2probla->Draw();

    pad4->Update();

    c->Print("root_plot/chi2.pdf");

}