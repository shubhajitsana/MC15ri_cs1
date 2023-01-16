void r2_costbto_costbz_thurstbm(){
    TFile *fa = new TFile("data/combined/all.root");
    TTree *ta = (TTree*)fa->Get("tree");
    TFile *fch = new TFile("data/combined/charged.root");
    TTree *tch = (TTree*)fch->Get("tree");
    TFile *fm = new TFile("data/combined/mixed.root");
    TTree *tm = (TTree*)fm->Get("tree");
    TFile *fu = new TFile("data/combined/uubar.root");
    TTree *tu = (TTree*)fu->Get("tree");
    TFile *fd = new TFile("data/combined/ddbar.root");
    TTree *td = (TTree*)fd->Get("tree");
    TFile *fs = new TFile("data/combined/ssbar.root");
    TTree *ts = (TTree*)fs->Get("tree");
    TFile *fc = new TFile("data/combined/ccbar.root");
    TTree *tc = (TTree*)fc->Get("tree");

    Double_t r2_a, r2_ch, r2_m, r2_u, r2_d, r2_s, r2_c;
    Double_t costbto_a, costbto_ch, costbto_m, costbto_u, costbto_d, costbto_s, costbto_c;
    Double_t costbz_a, costbz_ch, costbz_m, costbz_u, costbz_d, costbz_s, costbz_c;
    Double_t thrstbm_a, thrstbm_ch, thrstbm_m, thrstbm_u, thrstbm_d, thrstbm_s, thrstbm_c;


    ta->SetBranchAddress("R2",&r2_a);
    tch->SetBranchAddress("R2",&r2_ch);
    tm->SetBranchAddress("R2",&r2_m);
    tu->SetBranchAddress("R2",&r2_u);
    td->SetBranchAddress("R2",&r2_d);
    ts->SetBranchAddress("R2",&r2_s);
    tc->SetBranchAddress("R2",&r2_c);

    ta->SetBranchAddress("cosTBTO",&costbto_a);
    tch->SetBranchAddress("cosTBTO",&costbto_ch);
    tm->SetBranchAddress("cosTBTO",&costbto_m);
    tu->SetBranchAddress("cosTBTO",&costbto_u);
    td->SetBranchAddress("cosTBTO",&costbto_d);
    ts->SetBranchAddress("cosTBTO",&costbto_s);
    tc->SetBranchAddress("cosTBTO",&costbto_c);

    ta->SetBranchAddress("cosTBz",&costbz_a);
    tch->SetBranchAddress("cosTBz",&costbz_ch);
    tm->SetBranchAddress("cosTBz",&costbz_m);
    tu->SetBranchAddress("cosTBz",&costbz_u);
    td->SetBranchAddress("cosTBz",&costbz_d);
    ts->SetBranchAddress("cosTBz",&costbz_s);
    tc->SetBranchAddress("cosTBz",&costbz_c);

    ta->SetBranchAddress("thrustBm",&thrstbm_a);
    tch->SetBranchAddress("thrustBm",&thrstbm_ch);
    tm->SetBranchAddress("thrustBm",&thrstbm_m);
    tu->SetBranchAddress("thrustBm",&thrstbm_u);
    td->SetBranchAddress("thrustBm",&thrstbm_d);
    ts->SetBranchAddress("thrustBm",&thrstbm_s);
    tc->SetBranchAddress("thrustBm",&thrstbm_c);


    

    TH1F *r2a = new TH1F("r2a","r2a", 100, 0, 1);
    TH1F *r2ch = new TH1F("r2ch","r2ch", 100, 0, 1);
    TH1F *r2m = new TH1F("r2m","r2m", 100, 0, 1);
    TH1F *r2u = new TH1F("r2u","r2u", 100, 0, 1);
    TH1F *r2d = new TH1F("r2d","r2d", 100, 0, 1);
    TH1F *r2s = new TH1F("r2s","r2s", 100, 0, 1);
    TH1F *r2c = new TH1F("r2c","r2c", 100, 0, 1);
    

    TH1F *costbtoa = new TH1F("costbtoa","costbtoa", 100, 0, 1);
    TH1F *costbtoch = new TH1F("costbtoch","costbtoch", 100, 0, 1);
    TH1F *costbtom = new TH1F("costbtom","costbtom", 100, 0, 1);
    TH1F *costbtou = new TH1F("costbtou","costbtou", 100, 0, 1);
    TH1F *costbtod = new TH1F("costbtod","costbtod", 100, 0, 1);
    TH1F *costbtos = new TH1F("costbtos","costbtos", 100, 0, 1);
    TH1F *costbtoc = new TH1F("costbtoc","costbtoc", 100, 0, 1);
    

    TH1F *costbza = new TH1F("costbza","costbza", 100, 0, 1);
    TH1F *costbzch = new TH1F("costbzch","costbzch", 100, 0, 1);
    TH1F *costbzm = new TH1F("costbzm","costbzm", 100, 0, 1);
    TH1F *costbzu = new TH1F("costbzu","costbzu", 100, 0, 1);
    TH1F *costbzd = new TH1F("costbzd","costbzd", 100, 0, 1);
    TH1F *costbzs = new TH1F("costbzs","costbzs", 100, 0, 1);
    TH1F *costbzc = new TH1F("costbzc","costbzc", 100, 0, 1);
    

    TH1F *thrstbma = new TH1F("thrstbma","thrstbma", 100, 0, 1);
    TH1F *thrstbmch = new TH1F("thrstbmch","thrstbmch", 100, 0, 1);
    TH1F *thrstbmm = new TH1F("thrstbmm","thrstbmm", 100, 0, 1);
    TH1F *thrstbmu = new TH1F("thrstbmu","thrstbmu", 100, 0, 1);
    TH1F *thrstbmd = new TH1F("thrstbmd","thrstbmd", 100, 0, 1);
    TH1F *thrstbms = new TH1F("thrstbms","thrstbms", 100, 0, 1);
    TH1F *thrstbmc = new TH1F("thrstbmc","thrstbmc", 100, 0, 1);



    // Filling the histogram
    for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
        ta->GetEntry(iEvent);
        r2a->Fill(r2_a);
        costbtoa->Fill(costbto_a);
        costbza->Fill(costbz_a);
        thrstbma->Fill(thrstbm_a);
    }
    for(Int_t iEvent=0; iEvent < tch->GetEntries(); iEvent++){
        tch->GetEntry(iEvent);
        r2ch->Fill(r2_ch);
        costbtoch->Fill(costbto_ch);
        costbzch->Fill(costbz_ch);
        thrstbmch->Fill(thrstbm_ch);
    }
    for(Int_t iEvent=0; iEvent < tm->GetEntries(); iEvent++){
        tm->GetEntry(iEvent);
        r2m->Fill(r2_m);
        costbtom->Fill(costbto_m);
        costbzm->Fill(costbz_m);
        thrstbmm->Fill(thrstbm_m);
    }
    for(Int_t iEvent=0; iEvent < tu->GetEntries(); iEvent++){
        tu->GetEntry(iEvent);
        r2u->Fill(r2_u);
        costbtou->Fill(costbto_u);
        costbzu->Fill(costbz_u);
        thrstbmu->Fill(thrstbm_u);
    }
    for(Int_t iEvent=0; iEvent < td->GetEntries(); iEvent++){
        td->GetEntry(iEvent);
        r2d->Fill(r2_d);
        costbtod->Fill(costbto_d);
        costbzd->Fill(costbz_d);
        thrstbmd->Fill(thrstbm_d);
    }
    for(Int_t iEvent=0; iEvent < ts->GetEntries(); iEvent++){
        ts->GetEntry(iEvent);
        r2s->Fill(r2_s);
        costbtos->Fill(costbto_s);
        costbzs->Fill(costbz_s);
        thrstbms->Fill(thrstbm_s);
    }
    for(Int_t iEvent=0; iEvent < tc->GetEntries(); iEvent++){
        tc->GetEntry(iEvent);
        r2c->Fill(r2_c);
        costbtoc->Fill(costbto_c);
        costbzc->Fill(costbz_c);
        thrstbmc->Fill(thrstbm_c);
    }

    /////////////////////Drawing///////////////////
    TCanvas *c = new TCanvas();


    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.51, 0.49, 1.0);
    pad1->Draw();                     //  xmin, ymin, xmax, ymax
    pad1->cd();
    r2a->SetTitle("Plotting R2");
    r2a->GetYaxis()->SetTitle("Number of Events");
    r2a->GetXaxis()->SetTitle("Value of R2 variable");
    r2a->GetYaxis()->SetTitleOffset(0.8);
    r2a->GetYaxis()->SetTitleSize(0.035);
    // r2a->GetYaxis()->SetLabelOffset(0.8);
    r2a->GetYaxis()->SetLabelSize(0.02);


    r2a->SetStats(kFALSE);             // Don't print Statistics
    r2ch->SetStats(kFALSE);
    r2m->SetStats(kFALSE);
    r2u->SetStats(kFALSE);
    r2d->SetStats(kFALSE);
    r2s->SetStats(kFALSE);
    r2c->SetStats(kFALSE);

    r2a->SetLineColor(kRed);
    r2ch->SetLineColor(kViolet+10);
    r2m->SetLineColor(kGreen);
    r2u->SetLineColor(kYellow);
    r2d->SetLineColor(kMagenta);
    r2s->SetLineColor(kCyan);
    r2c->SetLineColor(kBlack);

    r2a->Draw();
    r2ch->Draw("SAME");
    r2m->Draw("SAME");
    r2u->Draw("SAME");
    r2d->Draw("SAME");
    r2s->Draw("SAME");
    r2c->Draw("SAME");

    TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
    legend1->AddEntry(r2a,"For all data","l");
    legend1->AddEntry(r2ch,"Only for charged","l");
    legend1->AddEntry(r2m,"Only for mixed","l");
    legend1->AddEntry(r2u,"Only for uubar","l");
    legend1->AddEntry(r2d,"Only for ddbar","l");
    legend1->AddEntry(r2s,"Only for ssbar","l");
    legend1->AddEntry(r2c,"Only for ccbar","l");
    legend1->Draw();


    pad1->Update();                 // Save changes in the pad

    // next pad
    c->cd();            // Go back to the canvas
    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0.51, 1.0, 1.0);
    pad2->Draw();
    pad2->cd();
    costbtoa->SetTitle("Plotting CosTBTO");
    costbtoa->GetYaxis()->SetTitle("Number of Events");
    costbtoa->GetXaxis()->SetTitle("Value of CosTBTO variable");
    costbtoa->GetYaxis()->SetTitleOffset(0.8);
    costbtoa->GetYaxis()->SetTitleSize(0.035);
    // costbtoa->GetYaxis()->SetLabelOffset(0.8);
    costbtoa->GetYaxis()->SetLabelSize(0.02);


    costbtoa->SetStats(kFALSE);             // Don't print Statistics
    costbtoch->SetStats(kFALSE);
    costbtom->SetStats(kFALSE);
    costbtou->SetStats(kFALSE);
    costbtod->SetStats(kFALSE);
    costbtos->SetStats(kFALSE);
    costbtoc->SetStats(kFALSE);

    costbtoa->SetLineColor(kRed);
    costbtoch->SetLineColor(kViolet+10);
    costbtom->SetLineColor(kGreen);
    costbtou->SetLineColor(kYellow);
    costbtod->SetLineColor(kMagenta);
    costbtos->SetLineColor(kCyan);
    costbtoc->SetLineColor(kBlack);

    costbtoa->Draw();
    costbtoch->Draw("SAME");
    costbtom->Draw("SAME");
    costbtou->Draw("SAME");
    costbtod->Draw("SAME");
    costbtos->Draw("SAME");
    costbtoc->Draw("SAME");

    TLegend *legend2 = new TLegend(0.1,0.7,0.35,0.9);
    legend2->AddEntry(costbtoa,"For all data","l");
    legend2->AddEntry(costbtoch,"Only for charged","l");
    legend2->AddEntry(costbtom,"Only for mixed","l");
    legend2->AddEntry(costbtou,"Only for uubar","l");
    legend2->AddEntry(costbtod,"Only for ddbar","l");
    legend2->AddEntry(costbtos,"Only for ssbar","l");
    legend2->AddEntry(costbtoc,"Only for ccbar","l");
    legend2->Draw();

    pad2->Update();


    ////////////////next pad//////////////////
    c->cd();
    TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 0.49, 0.49);
    pad3->Draw();
    pad3->cd();
    costbza->SetTitle("Plotting CosTBz");
    costbza->GetYaxis()->SetTitle("Number of Events");
    costbza->GetXaxis()->SetTitle("Value of CosTBz variable");
    costbza->GetYaxis()->SetTitleOffset(0.8);
    costbza->GetYaxis()->SetTitleSize(0.035);
    // costbza->GetYaxis()->SetLabelOffset(0.8);
    costbza->GetYaxis()->SetLabelSize(0.02);


    costbza->SetStats(kFALSE);             // Don't print Statistics
    costbzch->SetStats(kFALSE);
    costbzm->SetStats(kFALSE);
    costbzu->SetStats(kFALSE);
    costbzd->SetStats(kFALSE);
    costbzs->SetStats(kFALSE);
    costbzc->SetStats(kFALSE);

    costbza->SetLineColor(kRed);
    costbzch->SetLineColor(kViolet+10);
    costbzm->SetLineColor(kGreen);
    costbzu->SetLineColor(kYellow);
    costbzd->SetLineColor(kMagenta);
    costbzs->SetLineColor(kCyan);
    costbzc->SetLineColor(kBlack);

    costbza->Draw();
    costbzch->Draw("SAME");
    costbzm->Draw("SAME");
    costbzu->Draw("SAME");
    costbzd->Draw("SAME");
    costbzs->Draw("SAME");
    costbzc->Draw("SAME");

    TLegend *legend3 = new TLegend(0.7,0.7,0.9,0.9);
    legend3->AddEntry(costbza,"For all data","l");
    legend3->AddEntry(costbzch,"Only for charged","l");
    legend3->AddEntry(costbzm,"Only for mixed","l");
    legend3->AddEntry(costbzu,"Only for uubar","l");
    legend3->AddEntry(costbzd,"Only for ddbar","l");
    legend3->AddEntry(costbzs,"Only for ssbar","l");
    legend3->AddEntry(costbzc,"Only for ccbar","l");
    legend3->Draw();

    pad3->Update();


    ///////////////// next pad///////////////////////
    c->cd();
    TPad *pad4 = new TPad("pad4", "pad4", 0.5, 0, 1.0, 0.49);
    pad4->Draw();
    pad4->cd();
    thrstbma->SetTitle("Plotting Thrust of B meson");
    thrstbma->GetYaxis()->SetTitle("Number of Events");
    thrstbma->GetXaxis()->SetTitle("Value of Thrust");
    thrstbma->GetYaxis()->SetTitleOffset(0.8);
    thrstbma->GetYaxis()->SetTitleSize(0.035);
    // thrstbma->GetYaxis()->SetLabelOffset(0.8);
    thrstbma->GetYaxis()->SetLabelSize(0.02);


    thrstbma->SetStats(kFALSE);             // Don't print Statistics
    thrstbmch->SetStats(kFALSE);
    thrstbmm->SetStats(kFALSE);
    thrstbmu->SetStats(kFALSE);
    thrstbmd->SetStats(kFALSE);
    thrstbms->SetStats(kFALSE);
    thrstbmc->SetStats(kFALSE);

    thrstbma->SetLineColor(kRed);
    thrstbmch->SetLineColor(kViolet+10);
    thrstbmm->SetLineColor(kGreen);
    thrstbmu->SetLineColor(kYellow);
    thrstbmd->SetLineColor(kMagenta);
    thrstbms->SetLineColor(kCyan);
    thrstbmc->SetLineColor(kBlack);

    thrstbma->Draw();
    thrstbmch->Draw("SAME");
    thrstbmm->Draw("SAME");
    thrstbmu->Draw("SAME");
    thrstbmd->Draw("SAME");
    thrstbms->Draw("SAME");
    thrstbmc->Draw("SAME");

    TLegend *legend4 = new TLegend(0.1,0.7,0.35,0.9);
    legend4->AddEntry(thrstbma,"For all data","l");
    legend4->AddEntry(thrstbmch,"Only for charged","l");
    legend4->AddEntry(thrstbmm,"Only for mixed","l");
    legend4->AddEntry(thrstbmu,"Only for uubar","l");
    legend4->AddEntry(thrstbmd,"Only for ddbar","l");
    legend4->AddEntry(thrstbms,"Only for ssbar","l");
    legend4->AddEntry(thrstbmc,"Only for ccbar","l");
    legend4->Draw();

    pad4->Update();

    c->Print("root_plot/r2_costbto_costbz_thurstbm.pdf");
}