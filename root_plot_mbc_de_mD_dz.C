void mbc_de_mD_dz(){
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

    Double_t mbc_a, mbc_ch, mbc_m, mbc_u, mbc_d, mbc_s, mbc_c;
    Double_t de_a, de_ch, de_m, de_u, de_d, de_s, de_c;
    Double_t mD_a, mD_ch, mD_m, mD_u, mD_d, mD_s, mD_c;
    Double_t dz_a, dz_ch, dz_m, dz_u, dz_d, dz_s, dz_c;


    ta->SetBranchAddress("Mbc",&mbc_a);
    tch->SetBranchAddress("Mbc",&mbc_ch);
    tm->SetBranchAddress("Mbc",&mbc_m);
    tu->SetBranchAddress("Mbc",&mbc_u);
    td->SetBranchAddress("Mbc",&mbc_d);
    ts->SetBranchAddress("Mbc",&mbc_s);
    tc->SetBranchAddress("Mbc",&mbc_c);

    ta->SetBranchAddress("deltaE",&de_a);
    tch->SetBranchAddress("deltaE",&de_ch);
    tm->SetBranchAddress("deltaE",&de_m);
    tu->SetBranchAddress("deltaE",&de_u);
    td->SetBranchAddress("deltaE",&de_d);
    ts->SetBranchAddress("deltaE",&de_s);
    tc->SetBranchAddress("deltaE",&de_c);

    ta->SetBranchAddress("D0_bar_InvM",&mD_a);
    tch->SetBranchAddress("D0_bar_InvM",&mD_ch);
    tm->SetBranchAddress("D0_bar_InvM",&mD_m);
    tu->SetBranchAddress("D0_bar_InvM",&mD_u);
    td->SetBranchAddress("D0_bar_InvM",&mD_d);
    ts->SetBranchAddress("D0_bar_InvM",&mD_s);
    tc->SetBranchAddress("D0_bar_InvM",&mD_c);

    ta->SetBranchAddress("DeltaZ",&dz_a);
    tch->SetBranchAddress("DeltaZ",&dz_ch);
    tm->SetBranchAddress("DeltaZ",&dz_m);
    tu->SetBranchAddress("DeltaZ",&dz_u);
    td->SetBranchAddress("DeltaZ",&dz_d);
    ts->SetBranchAddress("DeltaZ",&dz_s);
    tc->SetBranchAddress("DeltaZ",&dz_c);
    

    TH1F *mbca = new TH1F("mbca","mbca", 80, 5.25, 5.29);
    TH1F *mbcch = new TH1F("mbcch","mbcch", 80, 5.25, 5.29);
    TH1F *mbcm = new TH1F("mbcm","mbcm", 80, 5.25, 5.29);
    TH1F *mbcu = new TH1F("mbcu","mbcu", 80, 5.25, 5.29);
    TH1F *mbcd = new TH1F("mbcd","mbcd", 80, 5.25, 5.29);
    TH1F *mbcs = new TH1F("mbcs","mbcs", 80, 5.25, 5.29);
    TH1F *mbcc = new TH1F("mbcc","mbcc", 80, 5.25, 5.29);

    TH1F *dea = new TH1F("dea","dea", 100, -0.15, 0.15);
    TH1F *dech = new TH1F("dech","dech", 100, -0.15, 0.15);
    TH1F *dem = new TH1F("dem","dem", 100, -0.15, 0.15);
    TH1F *deu = new TH1F("deu","deu", 100, -0.15, 0.15);
    TH1F *ded = new TH1F("ded","ded", 100, -0.15, 0.15);
    TH1F *des = new TH1F("des","des", 100, -0.15, 0.15);
    TH1F *dec = new TH1F("dec","dec", 100, -0.15, 0.15);

    TH1F *mDa = new TH1F("mDa","mDa", 100, 1.84, 1.89);
    TH1F *mDch = new TH1F("mDch","mDch", 100, 1.84, 1.89);
    TH1F *mDm = new TH1F("mDm","mDm", 100, 1.84, 1.89);
    TH1F *mDu = new TH1F("mDu","mDu", 100, 1.84, 1.89);
    TH1F *mDd = new TH1F("mDd","mDd", 100, 1.84, 1.89);
    TH1F *mDs = new TH1F("mDs","mDs", 100, 1.84, 1.89);
    TH1F *mDc = new TH1F("mDc","mDc", 100, 1.84, 1.89);

    TH1F *dza = new TH1F("dza","dza", 100, -0.08, 0.08);
    TH1F *dzch = new TH1F("dzch","dzch", 100, -0.08, 0.08);
    TH1F *dzm = new TH1F("dzm","dzm", 100, -0.08, 0.08);
    TH1F *dzu = new TH1F("dzu","dzu", 100, -0.08, 0.08);
    TH1F *dzd = new TH1F("dzd","dzd", 100, -0.08, 0.08);
    TH1F *dzs = new TH1F("dzs","dzs", 100, -0.08, 0.08);
    TH1F *dzc = new TH1F("dzc","dzc", 100, -0.08, 0.08);
    





    // Filling the histogram
    for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
        ta->GetEntry(iEvent);
        mbca->Fill(mbc_a);
        dea->Fill(de_a);
        mDa->Fill(mD_a);
        dza->Fill(dz_a);
    }
    for(Int_t iEvent=0; iEvent < tch->GetEntries(); iEvent++){
        tch->GetEntry(iEvent);
        mbcch->Fill(mbc_ch);
        dech->Fill(de_ch);
        mDch->Fill(mD_ch);
        dzch->Fill(dz_ch);
    }
    for(Int_t iEvent=0; iEvent < tm->GetEntries(); iEvent++){
        tm->GetEntry(iEvent);
        mbcm->Fill(mbc_m);
        dem->Fill(de_m);
        mDm->Fill(mD_m);
        dzm->Fill(dz_m);
    }
    for(Int_t iEvent=0; iEvent < tu->GetEntries(); iEvent++){
        tu->GetEntry(iEvent);
        mbcu->Fill(mbc_u);
        deu->Fill(de_u);
        mDu->Fill(mD_u);
        dzu->Fill(dz_u);
    }
    for(Int_t iEvent=0; iEvent < td->GetEntries(); iEvent++){
        td->GetEntry(iEvent);
        mbcd->Fill(mbc_d);
        ded->Fill(de_d);
        mDd->Fill(mD_d);
        dzd->Fill(dz_d);
    }
    for(Int_t iEvent=0; iEvent < ts->GetEntries(); iEvent++){
        ts->GetEntry(iEvent);
        mbcs->Fill(mbc_s);
        des->Fill(de_s);
        mDs->Fill(mD_s);
        dzs->Fill(dz_s);
    }
    for(Int_t iEvent=0; iEvent < tc->GetEntries(); iEvent++){
        tc->GetEntry(iEvent);
        mbcc->Fill(mbc_c);
        dec->Fill(de_c);
        mDc->Fill(mD_c);
        dzc->Fill(dz_c);
    }



    /////////////////////Drawing///////////////////
    TCanvas *c = new TCanvas();


    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.51, 0.49, 1.0);
    pad1->Draw();                     //  xmin, ymin, xmax, ymax
    pad1->cd();
    mbca->SetTitle("Plotting M_{bc} of B^{+}");
    mbca->GetYaxis()->SetTitle("Number of Events per 5 #times 10^{-4} GeV/c^{2}");
    mbca->GetXaxis()->SetTitle("M_{bc} (GeV/c^{2})");
    mbca->GetYaxis()->SetTitleOffset(0.8);
    mbca->GetYaxis()->SetTitleSize(0.035);
    // mbca->GetYaxis()->SetLabelOffset(0.8);
    mbca->GetYaxis()->SetLabelSize(0.02);


    mbca->SetStats(kFALSE);             // Don't print Statistics
    mbcch->SetStats(kFALSE);
    mbcm->SetStats(kFALSE);
    mbcu->SetStats(kFALSE);
    mbcd->SetStats(kFALSE);
    mbcs->SetStats(kFALSE);
    mbcc->SetStats(kFALSE);

    mbca->SetLineColor(kRed);
    mbcch->SetLineColor(kViolet+10);
    mbcm->SetLineColor(kGreen);
    mbcu->SetLineColor(kYellow);
    mbcd->SetLineColor(kMagenta);
    mbcs->SetLineColor(kCyan);
    mbcc->SetLineColor(kBlack);

    mbca->Draw();
    mbcch->Draw("SAME");
    mbcm->Draw("SAME");
    mbcu->Draw("SAME");
    mbcd->Draw("SAME");
    mbcs->Draw("SAME");
    mbcc->Draw("SAME");

    TLegend *legend1 = new TLegend(0.1,0.7,0.35,0.9);
    legend1->AddEntry(mbca,"For all data","l");
    legend1->AddEntry(mbcch,"Only for charged","l");
    legend1->AddEntry(mbcm,"Only for mixed","l");
    legend1->AddEntry(mbcu,"Only for uubar","l");
    legend1->AddEntry(mbcd,"Only for ddbar","l");
    legend1->AddEntry(mbcs,"Only for ssbar","l");
    legend1->AddEntry(mbcc,"Only for ccbar","l");
    legend1->Draw();


    pad1->Update();                 // Save changes in the pad

    // next pad
    c->cd();            // Go back to the canvas
    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0.5, 1.0, 1.0);
    pad2->Draw();
    pad2->cd();
    dea->SetTitle("Plotting #Delta E");
    dea->GetYaxis()->SetTitle("Number of Events per 3 #times 10^{-3} GeV/c^{2}");
    dea->GetXaxis()->SetTitle("#Delta E (GeV/c^{2})");
    dea->GetYaxis()->SetRangeUser(0,22750);
    dea->GetYaxis()->SetTitleOffset(0.8);
    dea->GetYaxis()->SetTitleSize(0.035);
    // dea->GetYaxis()->SetLabelOffset(0.8);
    dea->GetYaxis()->SetLabelSize(0.02);

    dea->SetStats(kFALSE);
    dech->SetStats(kFALSE);
    dem->SetStats(kFALSE);
    deu->SetStats(kFALSE);
    ded->SetStats(kFALSE);
    des->SetStats(kFALSE);
    dec->SetStats(kFALSE);

    dea->SetLineColor(kRed);
    dech->SetLineColor(kViolet+10);
    dem->SetLineColor(kGreen);
    deu->SetLineColor(kYellow);
    ded->SetLineColor(kMagenta);
    des->SetLineColor(kCyan);
    dec->SetLineColor(kBlack);

    dea->Draw();
    dech->Draw("SAME");
    dem->Draw("SAME");
    deu->Draw("SAME");
    ded->Draw("SAME");
    des->Draw("SAME");
    dec->Draw("SAME");

    TLegend *legend2 = new TLegend(0.7,0.7,0.9,0.9);
    legend2->AddEntry(dea,"For all data","l");
    legend2->AddEntry(dech,"Only for charged","l");
    legend2->AddEntry(dem,"Only for mixed","l");
    legend2->AddEntry(deu,"Only for uubar","l");
    legend2->AddEntry(ded,"Only for ddbar","l");
    legend2->AddEntry(des,"Only for ssbar","l");
    legend2->AddEntry(dec,"Only for ccbar","l");
    legend2->Draw();

    pad2->Update();

    ////////////////next pad//////////////////
    c->cd();
    TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 0.49, 0.49);
    pad3->Draw();
    pad3->cd();
    mDa->SetTitle("Plotting Invariant mass of #bar{D}^{0}");
    mDa->GetYaxis()->SetTitle("Number of Events per 5 #times 10^{-4} GeV/c^{2}");
    mDa->GetXaxis()->SetTitle("Invariant mass (GeV/c^{2})");
    mDa->GetYaxis()->SetRangeUser(0,28000);
    mDa->GetYaxis()->SetTitleOffset(0.8);
    mDa->GetYaxis()->SetTitleSize(0.035);
    // mDa->GetYaxis()->SetLabelOffset(0.8);
    mDa->GetYaxis()->SetLabelSize(0.02);


    mDa->SetStats(kFALSE);
    mDch->SetStats(kFALSE);
    mDm->SetStats(kFALSE);
    mDu->SetStats(kFALSE);
    mDd->SetStats(kFALSE);
    mDs->SetStats(kFALSE);
    mDc->SetStats(kFALSE);

    mDa->SetLineColor(kRed);
    mDch->SetLineColor(kViolet+10);
    mDm->SetLineColor(kGreen);
    mDu->SetLineColor(kYellow);
    mDd->SetLineColor(kMagenta);
    mDs->SetLineColor(kCyan);
    mDc->SetLineColor(kBlack);

    mDa->Draw();
    mDch->Draw("SAME");
    mDm->Draw("SAME");
    mDu->Draw("SAME");
    mDd->Draw("SAME");
    mDs->Draw("SAME");
    mDc->Draw("SAME");

    TLegend *legend3 = new TLegend(0.1,0.7,0.35,0.9);
    legend3->AddEntry(mDa,"For all data","l");
    legend3->AddEntry(mDch,"Only for charged","l");
    legend3->AddEntry(mDm,"Only for mixed","l");
    legend3->AddEntry(mDu,"Only for uubar","l");
    legend3->AddEntry(mDd,"Only for ddbar","l");
    legend3->AddEntry(mDs,"Only for ssbar","l");
    legend3->AddEntry(mDc,"Only for ccbar","l");
    legend3->Draw();

    pad3->Update();


    ///////////////// next pad///////////////////////
    c->cd();
    TPad *pad4 = new TPad("pad4", "pad4", 0.5, 0, 1.0, 0.49);
    pad4->Draw();
    pad4->cd();
    dza->SetTitle("Plotting Distance of two vertex (#Delta Z)");
    dza->GetYaxis()->SetTitle("Number of Events per 1.6 #times 10^{-3} cm");
    dza->GetXaxis()->SetTitle("#Delta Z (cm)");
    dza->GetYaxis()->SetRangeUser(0,220000);
    dza->GetYaxis()->SetTitleOffset(0.8);
    dza->GetYaxis()->SetTitleSize(0.035);
    // dza->GetYaxis()->SetLabelOffset(0.8);
    dza->GetYaxis()->SetLabelSize(0.02);


    dza->SetStats(kFALSE);
    dzch->SetStats(kFALSE);
    dzm->SetStats(kFALSE);
    dzu->SetStats(kFALSE);
    dzd->SetStats(kFALSE);
    dzs->SetStats(kFALSE);
    dzc->SetStats(kFALSE);

    dza->SetLineColor(kRed);
    dzch->SetLineColor(kViolet+10);
    dzm->SetLineColor(kGreen);
    dzu->SetLineColor(kYellow);
    dzd->SetLineColor(kMagenta);
    dzs->SetLineColor(kCyan);
    dzc->SetLineColor(kBlack);

    dza->Draw();
    dzch->Draw("SAME");
    dzm->Draw("SAME");
    dzu->Draw("SAME");
    dzd->Draw("SAME");
    dzs->Draw("SAME");
    dzc->Draw("SAME");

    TLegend *legend4 = new TLegend(0.7,0.7,0.9,0.9);
    legend4->AddEntry(dza,"For all data","l");
    legend4->AddEntry(dzch,"Only for charged","l");
    legend4->AddEntry(dzm,"Only for mixed","l");
    legend4->AddEntry(dzu,"Only for uubar","l");
    legend4->AddEntry(dzd,"Only for ddbar","l");
    legend4->AddEntry(dzs,"Only for ssbar","l");
    legend4->AddEntry(dzc,"Only for ccbar","l");
    legend4->Draw();

    pad4->Update();

    c->Print("root_plot/mbc_de_mD_dz.pdf");

}