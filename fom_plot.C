void fom_plot(){
    TCanvas *c1 = new TCanvas("fom","fom",200,10,900,700);
    c1->SetFillColor(0);
    c1->SetGrid();

    TGraph *GR2 = new TGraph("fom.txt");
    GR2->SetLineColor(2);
    GR2->SetLineWidth(3);
    GR2->SetMarkerColor(2);
    GR2->SetMarkerStyle(21);
    GR2->GetXaxis()->SetTitle("Cut");
    GR2->GetXaxis()->SetTitleSize(0.05);
    GR2->GetYaxis()->SetTitle("S/ #sqrt{S+B}");
    GR2->GetYaxis()->SetTitleSize(0.05);
    GR2->Draw("ACP");

    c1->Print("fom_plot.pdf")
}