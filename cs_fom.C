#include <iostream>
#include <math.h>
#include <fstream>

void cs_fom(){
    TFile *fa = new TFile("test.root");
    TTree *ta = (TTree*)fa->Get("tree");

    Double_t cs_prob, issig, iscontinuum;

    ta->SetBranchAddress("ContProb",&cs_prob);
    ta->SetBranchAddress("isSignal",&issig);
    ta->SetBranchAddress("isContinuumEvent",&iscontinuum);

    ////////////////////// Setting limit  interval 0.5
    // Double_t u_cs_prob, l_cs_prob, interval;
    // int dimension=99;
    // u_cs_prob = 1;
    // l_cs_prob = 0.01;
    // interval = (u_cs_prob-l_cs_prob)/dimension;
    // cout << "Continuum probablities are: [";
    // Double_t cs_prob_cut[dimension+1];
    // for( int i=0; i < (dimension+1); i++){
    //     cs_prob_cut[i] = (l_cs_prob + interval * i);
    //     cout << cs_prob_cut[i] << " ";
    // }
    // cout << "]" << endl;


    // // Calculating FOM for Continuum probablities
    // Int_t sig, bkg;
    // Int_t sig_event[dimension+1], bkg_event[dimension+1];
    // Double_t cs_prob_fom[dimension+1];
    // for(int i=0; i < (dimension+1); i++){
    //     sig = 0;
    //     bkg = 0;
    //     for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
    //         ta->GetEntry(iEvent);
    //         if(cs_prob<cs_prob_cut[i]){
    //             if(iscontinuum == 1) bkg++;
    //             else sig++;
    //         }
    //     }
    //     sig_event[i] = sig;
    //     bkg_event[i] = bkg;
    //     cs_prob_fom[i] = sig/sqrt(sig+bkg);
    // }

    // // Saving FOM data in output files
    // ofstream out_ptr1;
    // out_ptr1.open("cs_prob_fom.txt"); //define output file 
    // for( int i=0; i < (dimension+1); i++){
    // out_ptr1 << cs_prob_cut[i] << "\t" << cs_prob_fom[i] << endl; 
    // }

    // // Printing diffrent values
    // cout << "cs_prob_cut" << "\t" << "Sig" << "\t" << "Bkg" << "\t" << "FOM" << endl;
    // for( int i=0; i < (dimension+1); i++){
    //     cout << cs_prob_cut[i] << "\t" << sig_event[i] << "\t" << bkg_event[i] << "\t" << cs_prob_fom[i] << endl;
    // }


    // In short
    ofstream out_ptr1;
    out_ptr1.open("fom.txt"); //define output file 
    Int_t sig, bkg;
    for(Double_t cs_prob_cut = 0.01; cs_prob_cut < 1; cs_prob_cut += 0.01){
        sig = 0;
        bkg = 0;
        for(Int_t iEvent=0; iEvent < ta->GetEntries(); iEvent++){
            ta->GetEntry(iEvent);
            if(cs_prob<cs_prob_cut){
                if(iscontinuum == 1) bkg++;
                else sig++;
            }
        }
        out_ptr1 << cs_prob_cut << "\t" << sig/sqrt(sig+bkg) << endl;      
    }
}