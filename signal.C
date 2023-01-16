void signal()
{
   TFile *f1 = new TFile("train_charged.root");
   TTree *t1 = (TTree*)f1->Get("tree");
   
   TFile *F = new TFile("train_charged_onlysignal.root","RECREATE"); 
   TTree *T = t1->CloneTree(0);
   
   Double_t signal;
   t1->SetBranchAddress("isSignal",&signal);

   for (Int_t iEvent = 0; iEvent < t1->GetEntries(); iEvent++){
	t1->GetEntry(iEvent);
    if(signal==1){
        T->Fill();
    }
   }

   T->Write();
   F->Close();
   f1->Close(); 	
} 