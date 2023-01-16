void scale()
{
    /*
   TFile *f1 = new TFile("train_u.root");
   TTree *t1 = (TTree*)f1->Get("tree");
   
   TFile *F = new TFile("train_uubar.root","RECREATE"); 
   TTree *T = t1->CloneTree(0);
   
   //Long64_t nentries = t1->GetEntries();
   TRandom *r0 = new TRandom();
   
   for (Int_t iEvent = 0; iEvent < t1->GetEntries(); iEvent++) {
		Double_t ran = r0->Uniform(1);//(0,1)
		Double_t num = 1./4.2389;
		//cout<<ran<<endl;

		// Picking the event randomly with probablity 1/scale
		if(ran>num)continue; // should be "<" symbol. No bcz "continue" BREAKS one iteration
		t1->GetEntry(iEvent);      
		T->Fill();
	}
     
   T->Write();
   F->Close();
   f1->Close();
    
}
*/
	vector<TString> in_filename;        	
	in_filename.push_back("train_u.root");
	in_filename.push_back("train_d.root");
	in_filename.push_back("train_s.root");
	in_filename.push_back("train_c.root");

	vector<TString> o_filename;        	
	o_filename.push_back("train_uubar.root");
	o_filename.push_back("train_ddbar.root");
	o_filename.push_back("train_ssbar.root");
	o_filename.push_back("train_ccbar.root");

	vector<TString> treename;
	treename.push_back("tree");
	treename.push_back("tree");
	treename.push_back("tree");
	treename.push_back("tree");

	vector<double> scale;
	scale.push_back(4.2389);
	scale.push_back(3.8558);
	scale.push_back(3.3369);
	scale.push_back(6.4844);

    for(Int_t i=0; i<in_filename.size(); i++){
        cout<<" Scaling from file: "<<in_filename[i]<<"....." <<endl;
        
        TFile *FILE_I = new TFile(in_filename[i]);
        TTree *TREE_I = (TTree*)FILE_I->Get(treename[i]);

        TFile *FILE_O = new TFile(o_filename[i], "recreate");
        TTree *TREE_O = TREE_I->CloneTree(0);

        TRandom *r0 = new TRandom();
        for (Int_t iEvent = 0; iEvent < TREE_I->GetEntries(); iEvent++) {
            Double_t ran = r0->Uniform(1);//(0,1)
            Double_t num = 1./scale[i];
            // Picking the event randomly with probablity 1/scale
            if(ran>num)continue; // shouldn't be "<" symbol. No bcz "continue" BREAKS one iteration
            TREE_I->GetEntry(iEvent);      
            TREE_O->Fill();
        }
            
        TREE_O->Write();
        delete FILE_I;
        delete FILE_O;
    }
}