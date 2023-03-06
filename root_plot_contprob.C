void root_plot_contprob(){
    TFile *fa = new TFile("cs_fom_data/combined/signal.root");
    TTree *ta = (TTree*)fa->Get("tree");
    TFile *fch = new TFile("cs_fom_data/combined/charged.root");
    TTree *tch = (TTree*)fch->Get("tree");
    TFile *fm = new TFile("cs_fom_data/combined/mixed.root");
    TTree *tm = (TTree*)fm->Get("tree");
    TFile *fu = new TFile("cs_fom_data/combined/uubar.root");
    TTree *tu = (TTree*)fu->Get("tree");
    TFile *fd = new TFile("cs_fom_data/combined/ddbar.root");
    TTree *td = (TTree*)fd->Get("tree");
    TFile *fs = new TFile("cs_fom_data/combined/ssbar.root");
    TTree *ts = (TTree*)fs->Get("tree");
    TFile *fc = new TFile("cs_fom_data/combined/ccbar.root");
    TTree *tc = (TTree*)fc->Get("tree");

    Double_t mbc_a, mbc_ch, mbc_m, mbc_u, mbc_d, mbc_s, mbc_c;


    ta->SetBranchAddress("ContProb",&mbc_a);
    tch->SetBranchAddress("ContProb",&mbc_ch);
    tm->SetBranchAddress("ContProb",&mbc_m);
    tu->SetBranchAddress("ContProb",&mbc_u);
    td->SetBranchAddress("ContProb",&mbc_d);
    ts->SetBranchAddress("ContProb",&mbc_s);
    tc->SetBranchAddress("ContProb",&mbc_c);
    

    TH1F *mbca = new TH1F("mbca","mbca", 200, 0, 1);
    TH1F *mbcch = new TH1F("mbcch","mbcch", 200, 0, 1);
    TH1F *mbcm = new TH1F("mbcm","mbcm", 200, 0, 1);
    TH1F *mbcu = new TH1F("mbcu","mbcu", 200, 0, 1);
    TH1F *mbcd = new TH1F("mbcd","mbcd", 200, 0, 1);
    TH1F *mbcs = new TH1F("mbcs","mbcs", 200, 0, 1);
    TH1F *mbcc = new TH1F("mbcc","mbcc", 200, 0, 1);

    // Filling the histogram
    for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
        ta->GetEntry(iEvent);
        mbca->Fill(mbc_a);
    }
    for(Int_t iEvent=0; iEvent < tch->GetEntries(); iEvent++){
        tch->GetEntry(iEvent);
        mbcch->Fill(mbc_ch);
    }
    for(Int_t iEvent=0; iEvent < tm->GetEntries(); iEvent++){
        tm->GetEntry(iEvent);
        mbcm->Fill(mbc_m);
    }
    for(Int_t iEvent=0; iEvent < tu->GetEntries(); iEvent++){
        tu->GetEntry(iEvent);
        mbcu->Fill(mbc_u);
    }
    for(Int_t iEvent=0; iEvent < td->GetEntries(); iEvent++){
        td->GetEntry(iEvent);
        mbcd->Fill(mbc_d);
    }
    for(Int_t iEvent=0; iEvent < ts->GetEntries(); iEvent++){
        ts->GetEntry(iEvent);
        mbcs->Fill(mbc_s);
    }
    for(Int_t iEvent=0; iEvent < tc->GetEntries(); iEvent++){
        tc->GetEntry(iEvent);
        mbcc->Fill(mbc_c);
    }

    /////////////////////Drawing///////////////////
    TCanvas *c = new TCanvas();

    mbca->SetTitle("Continuum Probability Distribution");
    mbca->GetYaxis()->SetTitle("Number of Events");
    mbca->GetXaxis()->SetTitle("Continuum Probability");
    mbca->GetYaxis()->SetRangeUser(0,750000);
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
    mbcu->SetLineColor(kBlack);
    mbcd->SetLineColor(kMagenta);
    mbcs->SetLineColor(kCyan);
    mbcc->SetLineColor(20);

    mbca->Draw();
    mbcch->Draw("SAME");
    mbcm->Draw("SAME");
    mbcu->Draw("SAME");
    mbcd->Draw("SAME");
    mbcs->Draw("SAME");
    mbcc->Draw("SAME");

    TLegend *legend1 = new TLegend(0.3,0.7,0.65,0.9);
    legend1->AddEntry(mbca,"Signal","l");
    legend1->AddEntry(mbcch,"charged","l");
    legend1->AddEntry(mbcm,"mixed","l");
    legend1->AddEntry(mbcu,"uubar","l");
    legend1->AddEntry(mbcd,"ddbar","l");
    legend1->AddEntry(mbcs,"ssbar","l");
    legend1->AddEntry(mbcc,"ccbar","l");
    legend1->Draw();

    c->Update();                 // Save changes in the pad
    c->Print("cs_fom_data/combined/continuum_probability_plot/continuum_probability_plot_separately_200.png");
}