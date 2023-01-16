void thrustom_vxpos_fom(){
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

    Double_t thrstom_a, thrstom_ch, thrstom_m, thrstom_u, thrstom_d, thrstom_s, thrstom_c;
    Double_t drr_a, dzz_a, mcx_a, mcy_a, mcz_a, xx_a, yy_a, zz_a;

    ta->SetBranchAddress("thrustOm",&thrstom_a);
    tch->SetBranchAddress("thrustOm",&thrstom_ch);
    tm->SetBranchAddress("thrustOm",&thrstom_m);
    tu->SetBranchAddress("thrustOm",&thrstom_u);
    td->SetBranchAddress("thrustOm",&thrstom_d);
    ts->SetBranchAddress("thrustOm",&thrstom_s);
    tc->SetBranchAddress("thrustOm",&thrstom_c);

    ta->SetBranchAddress("D0_bar_dr",&drr_a);
    ta->SetBranchAddress("D0_bar_dz",&dzz_a);
    ta->SetBranchAddress("D0_bar_mcDecayVertexX",&mcx_a);
    ta->SetBranchAddress("D0_bar_mcDecayVertexY",&mcy_a);
    ta->SetBranchAddress("D0_bar_mcDecayVertexZ",&mcz_a);
    ta->SetBranchAddress("D0_bar_x",&xx_a);
    ta->SetBranchAddress("D0_bar_y",&yy_a);
    ta->SetBranchAddress("D0_bar_z",&zz_a);


    TH1F *drra = new TH1F("drra","drra", 100, -0.13, 0.13);
    TH1F *dzza = new TH1F("dzza","dzza", 100, -0.13, 0.13);
    TH1F *mcxa = new TH1F("mcxa","mcxa", 100, -0.13, 0.13);
    TH1F *mcya = new TH1F("mcya","mcya", 100, -0.13, 0.13);
    TH1F *mcza = new TH1F("mcza","mcza", 100, -0.13, 0.13);
    TH1F *xxa = new TH1F("xxa","xxa", 100, -0.13, 0.13);
    TH1F *yya = new TH1F("yya","yya", 100, -0.13, 0.13);
    TH1F *zza = new TH1F("zza","zza", 100, -0.13, 0.13);



    TH1F *thrstoma = new TH1F("thrstoma","thrstoma", 100, 0, 1);
    TH1F *thrstomch = new TH1F("thrstomch","thrstomch", 100, 0, 1);
    TH1F *thrstomm = new TH1F("thrstomm","thrstomm", 100, 0, 1);
    TH1F *thrstomu = new TH1F("thrstomu","thrstomu", 100, 0, 1);
    TH1F *thrstomd = new TH1F("thrstomd","thrstomd", 100, 0, 1);
    TH1F *thrstoms = new TH1F("thrstoms","thrstoms", 100, 0, 1);
    TH1F *thrstomc = new TH1F("thrstomc","thrstomc", 100, 0, 1);


    // Filling the histogram
    for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
        ta->GetEntry(iEvent);
        thrstoma->Fill(thrstom_a);

        drra->Fill(drr_a);
        dzza->Fill(dzz_a);
        mcxa->Fill(mcx_a);
        mcya->Fill(mcy_a);
        mcza->Fill(mcz_a);
        xxa->Fill(xx_a);
        yya->Fill(yy_a);
        zza->Fill(zz_a);
    }
    for(Int_t iEvent=0; iEvent < tch->GetEntries(); iEvent++){
        tch->GetEntry(iEvent);
        thrstomch->Fill(thrstom_ch);
    }
    for(Int_t iEvent=0; iEvent < tm->GetEntries(); iEvent++){
        tm->GetEntry(iEvent);
        thrstomm->Fill(thrstom_m);
    }
    for(Int_t iEvent=0; iEvent < tu->GetEntries(); iEvent++){
        tu->GetEntry(iEvent);
        thrstomu->Fill(thrstom_u);
    }
    for(Int_t iEvent=0; iEvent < td->GetEntries(); iEvent++){
        td->GetEntry(iEvent);
        thrstomd->Fill(thrstom_d);
    }
    for(Int_t iEvent=0; iEvent < ts->GetEntries(); iEvent++){
        ts->GetEntry(iEvent);
        thrstoms->Fill(thrstom_s);
    }
    for(Int_t iEvent=0; iEvent < tc->GetEntries(); iEvent++){
        tc->GetEntry(iEvent);
        thrstomc->Fill(thrstom_c);
    }    

    //////////////////////////////////////FOM/////////////////////////////
    Double_t kaonid,issig;
    ta->SetBranchAddress("Kp_PID_bin_kaon",&kaonid);
    ta->SetBranchAddress("isSignal",&issig);

    ////////////////////// Setting limit  interval 0.5
    Double_t u_kid, l_kid, interval;
    int dimension=10;
    u_kid = 0.9;
    l_kid = 0.4;
    interval = (u_kid-l_kid)/dimension;
    cout << "Binary Kaon id are: [";
    Double_t kid_cut[dimension+1];
    for( int i=0; i < (dimension+1); i++){
        kid_cut[i] = (l_kid + interval * i);
        cout << kid_cut[i] << "  ";
    }
    cout << "]" << endl;

    Int_t sig, bkg;

    // Calculating FOM for different binary kid
    Int_t sig_event[dimension+1], bkg_event[dimension+1];
    Double_t fom[dimension+1];
    for(int i=0; i < (dimension+1); i++){
        sig = 0;
        bkg = 0;
        for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
            ta->GetEntry(iEvent);
            if(kaonid>kid_cut[i]){
                if(issig == 1) sig++;
                else bkg++;
            }
        }
        sig_event[i] = sig;
        bkg_event[i] = bkg;
        fom[i] = sig/sqrt(sig+bkg);
    }

    // Printing diffrent values
    cout << "Signal events are: [";
    for( int i=0; i < (dimension+1); i++){
        cout << sig_event[i] << " ";
    }
    cout << "]" << endl;
    cout << "Background events are: [";
    for( int i=0; i < (dimension+1); i++){
        cout << bkg_event[i] << " ";
    }
    cout << "]" << endl;
    cout << "Calculated FOM are: [";
    for( int i=0; i < (dimension+1); i++){
        cout << fom[i] << " ";
    }
    cout << "]" << endl;

    //////////////////////////// Setting limit with interval 0.25/////////////////////
    Double_t u_kid2, l_kid2, interval2;
    int dimension2=20;
    u_kid2 = 0.9;
    l_kid2 = 0.4;
    interval2 = (u_kid2-l_kid2)/dimension2;
    cout << "Binary Kaon id are: [";
    Double_t kid_cut2[dimension2+1];
    for( int i=0; i < (dimension2+1); i++){
        kid_cut2[i] = (l_kid2 + interval2 * i);
        cout << kid_cut2[i] << "  ";
    }
    cout << "]" << endl;

    Int_t sig2, bkg2;

    // Calculating FOM for different binary kid
    Int_t sig_event2[dimension2+1], bkg_event2[dimension2+1];
    Double_t fom2[dimension2+1];
    for(int i=0; i < (dimension2+1); i++){
        sig2 = 0;
        bkg2 = 0;
        for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
            ta->GetEntry(iEvent);
            if(kaonid>kid_cut2[i]){
                if(issig == 1) sig2++;
                else bkg2++;
            }
        }
        sig_event2[i] = sig2;
        bkg_event2[i] = bkg2;
        fom2[i] = sig2/sqrt(sig2+bkg2);
    }

    // Printing diffrent values
    cout << "Signal events are: [";
    for( int i=0; i < (dimension2+1); i++){
        cout << sig_event2[i] << " ";
    }
    cout << "]" << endl;
    cout << "Background events are: [";
    for( int i=0; i < (dimension2+1); i++){
        cout << bkg_event2[i] << " ";
    }
    cout << "]" << endl;
    cout << "Calculated FOM are: [";
    for( int i=0; i < (dimension2+1); i++){
        cout << fom2[i] << " ";
    }
    cout << "]" << endl;

    ////////////////////////FOM Complete/////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // Drawing Plots
    TCanvas *c = new TCanvas();


    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.5, 0.5, 1.0);
    pad1->Draw();
    pad1->cd();
    thrstoma->SetTitle("Plotting Thrust of Rest of the Event");
    thrstoma->GetYaxis()->SetTitle("Number of Events");
    thrstoma->GetXaxis()->SetTitle("Thrust_{ROE}");
    thrstoma->GetYaxis()->SetTitleOffset(0.8);
    thrstoma->GetYaxis()->SetTitleSize(0.035);
    // thrstoma->GetYaxis()->SetLabelOffset(0.8);
    thrstoma->GetYaxis()->SetLabelSize(0.02);


    thrstoma->SetStats(kFALSE);             // Don't print Statistics
    thrstomch->SetStats(kFALSE);
    thrstomm->SetStats(kFALSE);
    thrstomu->SetStats(kFALSE);
    thrstomd->SetStats(kFALSE);
    thrstoms->SetStats(kFALSE);
    thrstomc->SetStats(kFALSE);

    thrstoma->SetLineColor(kRed);
    thrstomch->SetLineColor(kViolet+10);
    thrstomm->SetLineColor(kGreen);
    thrstomu->SetLineColor(kYellow);
    thrstomd->SetLineColor(kMagenta);
    thrstoms->SetLineColor(kCyan);
    thrstomc->SetLineColor(kBlack);

    thrstoma->Draw();
    thrstomch->Draw("SAME");
    thrstomm->Draw("SAME");
    thrstomu->Draw("SAME");
    thrstomd->Draw("SAME");
    thrstoms->Draw("SAME");
    thrstomc->Draw("SAME");

    TLegend *legend1 = new TLegend(0.1,0.7,0.35,0.9);
    legend1->AddEntry(thrstoma,"For all data","l");
    legend1->AddEntry(thrstomch,"Only for charged","l");
    legend1->AddEntry(thrstomm,"Only for mixed","l");
    legend1->AddEntry(thrstomu,"Only for uubar","l");
    legend1->AddEntry(thrstomd,"Only for ddbar","l");
    legend1->AddEntry(thrstoms,"Only for ssbar","l");
    legend1->AddEntry(thrstomc,"Only for ccbar","l");
    legend1->Draw();


    pad1->Update();                 // Save changes in the pad

    c->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0.5, 1.0, 1.0);
    pad2->Draw();
    pad2->cd();
    drra->SetTitle("Plotting Vertex Position of #bar{D}^{0}");
    drra->GetYaxis()->SetTitle("Number of Events per 2.6 #times 10^{-3} cm");
    drra->GetXaxis()->SetTitle("Distance (cm)");
    drra->GetYaxis()->SetTitleOffset(0.8);
    drra->GetYaxis()->SetTitleSize(0.035);
    // drra->GetYaxis()->SetLabelOffset(0.8);
    drra->GetYaxis()->SetLabelSize(0.02);


    drra->SetStats(kFALSE);             // Don't print Statistics
    dzza->SetStats(kFALSE);
    mcxa->SetStats(kFALSE);
    mcya->SetStats(kFALSE);
    mcza->SetStats(kFALSE);
    xxa->SetStats(kFALSE);
    yya->SetStats(kFALSE);
    zza->SetStats(kFALSE);

    drra->SetLineColor(kRed);
    dzza->SetLineColor(kViolet+10);
    mcxa->SetLineColor(kYellow);
    mcya->SetLineColor(kGreen);
    mcza->SetLineColor(kMagenta);
    xxa->SetLineColor(kCyan);
    yya->SetLineColor(kBlack);
    zza->SetLineColor(kBlack);

    drra->Draw();
    dzza->Draw("SAME");
    mcxa->Draw("SAME");
    mcya->Draw("SAME");
    mcza->Draw("SAME");
    xxa->Draw("SAME");
    yya->Draw("SAME");
    zza->Draw("SAME");

    TLegend *legend2 = new TLegend(0.7,0.7,0.9,0.9);
    legend2->AddEntry(drra,"For all data","l");
    legend2->AddEntry(dzza,"Only for charged","l");
    legend2->AddEntry(mcxa,"mcDecayVertexX: x coordinate of MC matched #bar{D}^{0} vertex","l");
    legend2->AddEntry(mcya,"mcDecayVertexY: y coordinate of MC matched #bar{D}^{0} vertex","l");
    legend2->AddEntry(mcza,"mcDecayVertexZ: z coordinate of MC matched #bar{D}^{0} vertex","l");
    legend2->AddEntry(xxa,"x : x coordinate of #bar{D}^{0} vertex","l");
    legend2->AddEntry(yya,"y : y coordinate of #bar{D}^{0} vertex","l");
    legend2->AddEntry(zza,"z : z coordinate of #bar{D}^{0} vertex","l");
    legend2->Draw();


    pad2->Update();

    ////////////////next pad//////////////////

    c->cd();
    TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 0.5, 0.5);
    pad3->Draw();
    pad3->cd();
    TGraph *gr1 = new TGraph(dimension+1,kid_cut,fom);
    gr1->SetFillColor(9);
    gr1->SetTitle("Figure of Merit");
    gr1->GetYaxis()->SetTitle("Figure of Merit");
    gr1->GetXaxis()->SetTitle("Binary Kaon ID");
    gr1->Draw("AB");


    c->cd();
    TPad *pad4 = new TPad("pad4", "pad4", 0.5, 0, 1.0, 0.5);
    pad4->Draw();
    pad4->cd();
    TGraph *gr4 = new TGraph(dimension2+1,kid_cut2,fom2);
    gr4->SetFillColor(9);
    gr4->SetTitle("Figure of Merit");
    gr4->GetYaxis()->SetTitle("Figure of Merit");
    gr4->GetXaxis()->SetTitle("Binary Kaon ID");
    gr4->Draw("AB");

    c->Print("root_plot/thrustom_vxpos_fom.pdf");
}