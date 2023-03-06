void root_plot_mbc_before_fitting(){
    TFile *fa = new TFile("cs_fom_data/combined/scaled_signal.root");
    TTree *ta = (TTree*)fa->Get("tree");
    TFile *fch = new TFile("cs_fom_data/combined/charged.root");
    TTree *tch = (TTree*)fch->Get("tree");
    TFile *fch_signal = new TFile("cs_fom_data/combined/charged_signal.root");
    TTree *tch_signal = (TTree*)fch_signal->Get("tree");
    TFile *fch_bkg = new TFile("cs_fom_data/combined/charged_bkg.root");
    TTree *tch_bkg = (TTree*)fch_bkg->Get("tree");
    TFile *fm = new TFile("cs_fom_data/combined/mixed.root");
    TTree *tm = (TTree*)fm->Get("tree");
    TFile *fu = new TFile("cs_fom_data/combined/qqbar.root");
    TTree *tu = (TTree*)fu->Get("tree");


    Double_t mbc_a, mbc_ch, mbc_ch_signal, mbc_ch_bkg, mbc_m, mbc_u;   // mbc_d, mbc_s, mbc_c;
    Double_t de_a, de_ch, de_ch_signal, de_ch_bkg, de_m, de_u;
    Double_t md_a, md_ch, md_ch_signal, md_ch_bkg, md_m, md_u;
    Double_t kid_a, kid_ch, kid_ch_signal, kid_ch_bkg, kid_m, kid_u;
    Double_t cont_prob_a, cont_prob_ch, cont_prob_ch_signal, cont_prob_ch_bkg, cont_prob_m, cont_prob_u;


    ta->SetBranchAddress("Mbc",&mbc_a);
    tch->SetBranchAddress("Mbc",&mbc_ch);
    tch_signal->SetBranchAddress("Mbc",&mbc_ch_signal);
    tch_bkg->SetBranchAddress("Mbc",&mbc_ch_bkg);
    tm->SetBranchAddress("Mbc",&mbc_m);
    tu->SetBranchAddress("Mbc",&mbc_u);
 
    ta->SetBranchAddress("deltaE",&de_a);
    tch->SetBranchAddress("deltaE",&de_ch);
    tch_signal->SetBranchAddress("deltaE",&de_ch_signal);
    tch_bkg->SetBranchAddress("deltaE",&de_ch_bkg);
    tm->SetBranchAddress("deltaE",&de_m);
    tu->SetBranchAddress("deltaE",&de_u);
 
    ta->SetBranchAddress("D0_bar_InvM",&md_a);
    tch->SetBranchAddress("D0_bar_InvM",&md_ch);
    tch_signal->SetBranchAddress("D0_bar_InvM",&md_ch_signal);
    tch_bkg->SetBranchAddress("D0_bar_InvM",&md_ch_bkg);
    tm->SetBranchAddress("D0_bar_InvM",&md_m);
    tu->SetBranchAddress("D0_bar_InvM",&md_u);
 
    ta->SetBranchAddress("Kp_PID_bin_kaon",&kid_a);
    tch->SetBranchAddress("Kp_PID_bin_kaon",&kid_ch);
    tch_signal->SetBranchAddress("Kp_PID_bin_kaon",&kid_ch_signal);
    tch_bkg->SetBranchAddress("Kp_PID_bin_kaon",&kid_ch_bkg);
    tm->SetBranchAddress("Kp_PID_bin_kaon",&kid_m);
    tu->SetBranchAddress("Kp_PID_bin_kaon",&kid_u);
 
    ta->SetBranchAddress("ContProb",&cont_prob_a);
    tch->SetBranchAddress("ContProb",&cont_prob_ch);
    tch_signal->SetBranchAddress("ContProb",&cont_prob_ch_signal);
    tch_bkg->SetBranchAddress("ContProb",&cont_prob_ch_bkg);
    tm->SetBranchAddress("ContProb",&cont_prob_m);
    tu->SetBranchAddress("ContProb",&cont_prob_u);
    

    TH1F *mbca = new TH1F("mbca","mbca", 100, 5.23, 5.29);
    TH1F *mbcch = new TH1F("mbcch","mbcch", 100, 5.23, 5.29);
    TH1F *mbcch_signal = new TH1F("mbcch_signal","mbcch_signal", 100, 5.23, 5.29);
    TH1F *mbcch_bkg = new TH1F("mbcch_bkg","mbcch_bkg", 100, 5.23, 5.29);
    TH1F *mbcm = new TH1F("mbcm","mbcm", 100, 5.23, 5.29);
    TH1F *mbcu = new TH1F("mbcu","mbcu", 100, 5.23, 5.29);

    // Filling the histogram
    for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
        ta->GetEntry(iEvent);
        if(md_a>1.85 && md_a<1.88 && mbc_a>5.23 && mbc_a < 5.29 && de_a < 0.1 && de_a > -0.1 && kid_a > 0.6 && cont_prob_a < 0.6) mbca->Fill(mbc_a);
    }
    for(Int_t iEvent=0; iEvent < tch->GetEntries(); iEvent++){
        tch->GetEntry(iEvent);
        if(md_ch>1.85 && md_ch<1.88 && mbc_ch>5.23 && mbc_ch < 5.29 && de_ch < 0.1 && de_ch > -0.1 && kid_ch > 0.6 && cont_prob_ch < 0.6) mbcch->Fill(mbc_ch);
    }
    for(Int_t iEvent=0; iEvent < tch_signal->GetEntries(); iEvent++){
        tch_signal->GetEntry(iEvent);
        if(md_ch_signal>1.85 && md_ch_signal<1.88 && mbc_ch_signal>5.23 && mbc_ch_signal < 5.29 && de_ch_signal < 0.1 && de_ch_signal > -0.1 && kid_ch_signal > 0.6 && cont_prob_ch_signal < 0.6) mbcch_signal->Fill(mbc_ch_signal);
    }
    for(Int_t iEvent=0; iEvent < tch_bkg->GetEntries(); iEvent++){
        tch_bkg->GetEntry(iEvent);
        if(md_ch_bkg>1.85 && md_ch_bkg<1.88 && mbc_ch_bkg>5.23 && mbc_ch_bkg < 5.29 && de_ch_bkg < 0.1 && de_ch_bkg > -0.1 && kid_ch_bkg > 0.6 && cont_prob_ch_bkg < 0.6) mbcch_bkg->Fill(mbc_ch_bkg);
    }
    for(Int_t iEvent=0; iEvent < tm->GetEntries(); iEvent++){
        tm->GetEntry(iEvent);
        if(md_m>1.85 && md_m<1.88 && mbc_m>5.23 && mbc_m < 5.29 && de_m < 0.1 && de_m > -0.1 && kid_m > 0.6 && cont_prob_m < 0.6) mbcm->Fill(mbc_m);
    }
    for(Int_t iEvent=0; iEvent < tu->GetEntries(); iEvent++){
        tu->GetEntry(iEvent);
        if(md_u>1.85 && md_u<1.88 && mbc_u>5.23 && mbc_u < 5.29 && de_u < 0.1 && de_u > -0.1 && kid_u > 0.6 && cont_prob_u < 0.6) mbcu->Fill(mbc_u);
    }

    /////////////////////Drawing///////////////////
    TCanvas *c = new TCanvas();

    mbca->SetTitle("Plotting M_{bc}");
    mbca->GetYaxis()->SetTitle("Number of Events");
    mbca->GetXaxis()->SetTitle("M_{bc} (GeV/c^{2})");
    mbca->GetYaxis()->SetRangeUser(0,10000);
    mbca->GetYaxis()->SetTitleOffset(0.8);
    mbca->GetYaxis()->SetTitleSize(0.035);
    // mbca->GetYaxis()->SetLabelOffset(0.8);
    mbca->GetYaxis()->SetLabelSize(0.02);


    mbca->SetStats(kFALSE);             // Don't print Statistics
    mbcch->SetStats(kFALSE);
    mbcch_signal->SetStats(kFALSE);
    mbcch_bkg->SetStats(kFALSE);
    mbcm->SetStats(kFALSE);
    mbcu->SetStats(kFALSE);
    // mbcd->SetStats(kFALSE);
    // mbcs->SetStats(kFALSE);
    // mbcc->SetStats(kFALSE);

    mbca->SetLineColor(kRed);
    mbcch->SetLineColor(kViolet+10);
    mbcch_signal->SetLineColor(kMagenta);
    mbcch_bkg->SetLineColor(kCyan);
    mbcm->SetLineColor(kGreen);
    mbcu->SetLineColor(kBlack);
    // mbcd->SetLineColor(kMagenta);
    // mbcs->SetLineColor(kCyan);
    // mbcc->SetLineColor(kYellow);

    mbca->Draw();
    mbcch->Draw("SAME");
    mbcch_signal->Draw("SAME");
    mbcch_bkg->Draw("SAME");
    mbcm->Draw("SAME");
    mbcu->Draw("SAME");
    // mbcd->Draw("SAME");
    // mbcs->Draw("SAME");
    // mbcc->Draw("SAME");

    TLegend *legend1 = new TLegend(0.1,0.7,0.35,0.9);
    legend1->AddEntry(mbca,"Signal","l");
    legend1->AddEntry(mbcch,"charged","l");
    legend1->AddEntry(mbcch_signal,"charged_signal","l");
    legend1->AddEntry(mbcch_bkg,"charged_bkg","l");
    legend1->AddEntry(mbcm,"mixed","l");
    legend1->AddEntry(mbcu,"qqbar","l");
    // legend1->AddEntry(mbcd,"Only for ddbar","l");
    // legend1->AddEntry(mbcs,"Only for ssbar","l");
    // legend1->AddEntry(mbcc,"Only for ccbar","l");
    legend1->Draw();

    c->Update();                 // Save changes in the pad
    c->Print("mbc_plot/histo_mbc_scaled_signal_with_cut_ultrazoom.png");
}