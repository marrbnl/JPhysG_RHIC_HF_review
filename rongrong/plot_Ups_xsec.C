#include "style.C"


void plot_Ups_xsec(int savePlot = 0)
{
  gROOT->ProcessLine("set_style()");

  // Reference: W.R. Innes et al., PRL 39 1240 (1977)
  // BR x d\sigma/dy | y=0 = 0.18 \pm 0.01 + 0.065 \pm 0.007 + 0.011 \pm 0.007
  // Adding it up: 0.256 \pm 0.014
  // lab momentum: 400 GeV protons
  // fixed targets Pt, Cu
  // sqrt(s) = sqrt(400.938^2 - 400^2) = 27.4 GeV

  TCanvas* c = new TCanvas("UpsWorldData","Upsilon World Data",800, 600);
  //UpsWorldData->SetLeftMargin(0.15);
  // UpsWorldData->SetRightMargin(0.01);
  // UpsWorldData->SetTopMargin(0.01);
  // Upsilon world data
  //
  TH1D* hplot = new TH1D("hplot","",100,15,8e3);
  hplot->SetMinimum(1);
  hplot->SetMaximum(8e3);
  hplot->GetYaxis()->SetTitle("#font[32]{B} #upoint d#sigma_{#varUpsilon}/dy|_{y=0} (pb)");
  hplot->GetXaxis()->SetTitle("#sqrt{s} (GeV)");
  //hplot->GetYaxis()->SetTitleOffset(1.4);
  gPad->SetLogx();
  gPad->SetLogy();
  hplot->Draw();

    // Theory curve
  // MRST HO, mu = m = 4.75 GeV
  //
  const Int_t n2 = 17;
  Float_t x2[] = {
    19.4, 23.7, 27.4, 38.8, 44, 53, 63, 100, 200, 500, 630, 1000, 1800, 2000, 5500, 9000, 14000
  };
  Float_t y2[] = {
    23.62, 158.7, 424.6, 2271, 3630, 6580, 10670, 28172, 84520, 236200, 276200, 437200, 750000, 802800, 1770400, 2485200, 3454400
  };
  // Scale factor to get from fb to pb
  for (size_t i=0;i<17;++i) {
    y2[i]*=1e-3;
  }
  TGraph* g2 = new TGraph(n2, x2, y2);
  //g2->GetXaxis()->SetRangeUser(10, 1e4);
  //g2->GetYaxis()->SetRangeUser(0.1, 1e4);
  g2->SetLineWidth(2);
  g2->SetLineColor(kGreen+3);
  g2->Draw("L");

  const double gMarkerSize = 2.0;

  const Int_t nppCCOR = 2;
  Float_t xppCCOR[] = { 44, 62};
  Float_t yppCCOR[] = {  6, 9};
  Float_t dyppCCOR[] = { 3, 3 };
  TGraphErrors* gppCCOR = new TGraphErrors(nppCCOR, xppCCOR, yppCCOR, 0, dyppCCOR);
  gppCCOR->SetMarkerStyle(22);
  gppCCOR->SetMarkerSize(gMarkerSize);
  gppCCOR->SetTitle("");
  gppCCOR->Draw("P");

  const Int_t nppR209 = 1;
  Float_t xppR209[] = { 62.4};
  Float_t yppR209[] = { 10};
  Float_t dyppR209[] = { 4};
  TGraphErrors* gppR209 = new TGraphErrors(nppR209, xppR209, yppR209, 0, dyppR209);
  gppR209->SetMarkerStyle(23);
  gppR209->SetMarkerSize(gMarkerSize);
  gppR209->SetTitle("");
  gppR209->Draw("P");
  
  const Int_t nppR806 = 2;
  Float_t xppR806[] = { 53, 62};
  Float_t yppR806[] = { 13.5, 15.2};
  Float_t dyppR806[] = { 7.4, 5.5};
  TGraphErrors* gppR806 = new TGraphErrors(nppR806, xppR806, yppR806, 0, dyppR806);
  gppR806->SetMarkerStyle(24);
  gppR806->SetMarkerSize(gMarkerSize);
  gppR806->SetTitle("");
  gppR806->Draw("P");
  
  const Int_t nppbarUA1 = 1;
  Float_t xppbarUA1[] = { 630};
  Float_t yppbarUA1[] = { 290};
  Float_t dyppbarUA1[] = { 57};
  TGraphErrors* gppbarUA1 = new TGraphErrors(nppbarUA1, xppbarUA1, yppbarUA1, 0, dyppbarUA1);
  gppbarUA1->SetMarkerStyle(25);
  gppbarUA1->SetMarkerSize(gMarkerSize);
  gppbarUA1->SetTitle("");
  gppbarUA1->Draw("P");

  const Int_t nppbarCDF = 1;
  Float_t xppbarCDF[] = { 1800  };
  Float_t yppbarCDF[] = { 953  };
  Float_t dyppbarCDF[] = { 27  };
  TGraphErrors* gppbarCDF = new TGraphErrors(nppbarCDF, xppbarCDF, yppbarCDF, 0, dyppbarCDF);
  gppbarCDF->SetMarkerStyle(26);
  gppbarCDF->SetMarkerSize(gMarkerSize);
  gppbarCDF->SetTitle("");
  gppbarCDF->Draw("P");

  //https://arxiv.org/abs/1303.5900
  const Int_t nppbarCMS = 1;
  Float_t xppbarCMS[] = { 7000  };
  Float_t yppbarCMS[] = { 2472.92  };
  Float_t dyppbarCMS[] = { 144.082   };
  TGraphErrors* gppbarCMS = new TGraphErrors(nppbarCMS, xppbarCMS, yppbarCMS, 0, dyppbarCMS);
  gppbarCMS->SetMarkerStyle(27);
  gppbarCMS->SetMarkerSize(3.0);
  gppbarCMS->SetTitle("");
  gppbarCMS->Draw("P");


  const int gStarColor = kRed+1;
  const int gStarMarker = 30;
  const int gPhenixColor = kBlue-3;
  const int gPhenixMarker = 20;
  // STAR: https://inspirehep.net/literature/2877008
  // 2011 data
  double StarY[1] = {500};
  double StarYErr[1] = {0};
  double StarSigma[1] = {189};
  double StarSigmaErr[1] = {34.4};
  TGraphErrors* StarUpsilonPp2011 = new TGraphErrors(1,StarY,StarSigma,StarYErr,StarSigmaErr);
  StarUpsilonPp2011->SetName("StarUpsilonPp2011");
  StarUpsilonPp2011->SetMarkerStyle(gStarMarker);
  StarUpsilonPp2011->SetMarkerColor(gStarColor);
  StarUpsilonPp2011->SetLineColor(gStarColor);
  StarUpsilonPp2011->SetMarkerSize(2.5);
  StarUpsilonPp2011->Draw("PZ");

  // STAR: https://inspirehep.net/literature/1269346
  // 2009 data
  double StarY09[1] = {200}; 
  double StarYErr09[1] = {0};
  double StarSigma09[1] = {64};
  double StarSigmaHighErr09[1] = {17.2};
  double StarSigmaLowErr09[1] = {15.6};
  TGraphAsymmErrors* StarUpsilonPp2009 = new TGraphAsymmErrors(1,StarY09,StarSigma09,StarYErr09,StarYErr09,StarSigmaLowErr09,StarSigmaHighErr09);
  StarUpsilonPp2009->SetName("StarUpsilonPp2009");
  StarUpsilonPp2009->SetMarkerStyle(gStarMarker);
  StarUpsilonPp2009->SetMarkerColor(gStarColor);
  StarUpsilonPp2009->SetLineColor(gStarColor);
  StarUpsilonPp2009->SetMarkerSize(2.5);
  StarUpsilonPp2009->Draw("PZ");

  // PHENIX: https://journals.aps.org/prc/abstract/10.1103/PhysRevC.91.024913
  // 2006 data
  double PhenixY06[1] = {200};
  double PhenixYErr06[1] = {0};
  double PhenixSigma06[1] = {108};
  double PhenixSigmaErr06[1] = {42.3};
  TGraphErrors* PhenixUpsilonPp2006 = new TGraphErrors(1,PhenixY06,PhenixSigma06,PhenixYErr06,PhenixSigmaErr06);
  PhenixUpsilonPp2006->SetName("PhenixUpsilonPp2006");
  PhenixUpsilonPp2006->SetMarkerStyle(gPhenixMarker);
  PhenixUpsilonPp2006->SetMarkerColor(gPhenixColor);
  PhenixUpsilonPp2006->SetLineColor(gPhenixColor);
  PhenixUpsilonPp2006->SetMarkerSize(2.0);
  PhenixUpsilonPp2006->Draw("PZ");


  TLegend* leg = new TLegend(0.15,0.65,0.45,0.88);
  leg->AddEntry(StarUpsilonPp2011,"STAR","P");
  leg->AddEntry(PhenixUpsilonPp2006,"PHENIX","P");
  leg->AddEntry(g2,"NLO CEM, MRST HO,","L");
  leg->AddEntry(g2,"m=4.75 GeV/c^{2}, m/#mu=1","");
  leg->Draw();

  TLegend* leg2 = new TLegend(0.5,0.2,0.9,0.35);
  leg2->SetNColumns(3);
  leg2->AddEntry(gppCCOR,"CCOR","P");
  leg2->AddEntry(gppR209,"R209","P");
  leg2->AddEntry(gppR806,"R806","P");
  leg2->AddEntry(gppbarUA1,"UA1","P");
  leg2->AddEntry(gppbarCDF,"CDF","P");
  leg2->AddEntry(gppbarCMS,"CMS","P");

  leg2->Draw();

  TLatex tex(0.5,0.5," ");
  tex.SetNDC();
  //tex.SetTextSize(0.05);
  tex.DrawLatex(0.15, 0.9,"#varUpsilon(1S+2S+3S) in p+p, p+#bar{p}");

  if(savePlot)
    {
      c->SaveAs("figs/Ups_xsec_pp.pdf");
    } 

  return;
}
