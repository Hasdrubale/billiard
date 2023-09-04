void graphs() {
  delete gROOT->FindObject("Y iniziale");
  delete gROOT->FindObject("Y finale");
  delete gROOT->FindObject("Angolo iniziale");
  delete gROOT->FindObject("Angolo finale");
  delete gROOT->FindObject("c");
  ifstream input{"input.txt"};
  Double_t r1;
  Double_t r2;
  input >> r1;
  input >> r2;
  TCanvas *c = new TCanvas("c", "Distribuzione");
  TH1D *h_yi = new TH1D("Y iniziale", "Y iniziale", 50, -r1, r1);
  TH1D *h_angi =
      new TH1D("Angolo iniziale", "Angolo iniziale", 50, -M_PI / 2, M_PI / 2);
  TH1D *h_yf = new TH1D("Y finale", "Y finale", 50, -r2, r2);
  TH1D *h_angf =
      new TH1D("Angolo finale", "Angolo finale", 50, -M_PI / 2, M_PI / 2);

  h_yi->SetFillColor(kBlue);
  h_yf->SetFillColor(kGreen);
  h_angi->SetFillColor(kBlue);
  h_angf->SetFillColor(kGreen);
  h_yi->SetLineColor(kBlue);
  h_yf->SetLineColor(kGreen);
  h_angi->SetLineColor(kBlue);
  h_angf->SetLineColor(kGreen);

  ifstream outinit{"outinit.txt"};
  while (true) {
    Double_t x, y, ang;
    outinit >> x >> y >> ang;
    if (!outinit.good()) {
      break;
    }
    h_yi->Fill(y);
    h_angi->Fill(ang);
    break;
  }

  ifstream outfin{"outfin.txt"};
  while (true) {
    Double_t x, y, ang;
    outfin >> x >> y >> ang;
    if (!outfin.good()) {
      break;
    }
    h_yf->Fill(y);
    h_angf->Fill(ang);
    break;
  }

  c->Divide(2, 2);
  c->cd(1);
  h_yi->Fit("gaus");
  c->cd(2);
  h_angi->Fit("gaus");
  c->cd(3);
  h_yf->Draw();
  c->cd(4);
  h_angf->Draw();
}