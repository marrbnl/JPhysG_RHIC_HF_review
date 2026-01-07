#include "style.C"

const int gStarColor = kRed+1;
const int gStarMarker = 30;
const int gPhenixColor = kBlue-3;
const int gPhenixMarker = 20;

void pA(const int savePlot);
void BES(const int savePlot);
void RaaVsPt(const int savePlot);

//================================================
void plot_Jpsi()
{
  gROOT->ProcessLine("set_style()");

  //pA(1);
  //BES(1);
  //RaaVsPt(0);
}

//================================================
void RaaVsPt(const int savePlot)
{
  const char* cent = "0020";
  const char* cent_title = "0-20%";
  TFile *fin = TFile::Open("inputs/Jpsi_AA.root", "read");
  TGraphAsymmErrors *gStarJpsiRAA = (TGraphAsymmErrors*)fin->Get(Form("STAR_Run14MTD_JpsiRaaVsPt_%s", cent));
  TGraphAsymmErrors *gStarJpsiRAASys = (TGraphAsymmErrors*)fin->Get(Form("STAR_Run14MTD_JpsiRaaVsPt_%s_sys", cent));
  TBox *globalSys_Star = (TBox*)fin->Get(Form("STAR_Run14MTD_JpsiRaaVsPt_%s_globalsys", cent));
  gStarJpsiRAA->SetMarkerStyle(gStarMarker);
  gStarJpsiRAA->SetMarkerSize(2.5);
  gStarJpsiRAA->SetMarkerColor(gStarColor);
  gStarJpsiRAA->SetLineColor(gStarColor);
  gStarJpsiRAASys->SetMarkerSize(0);
  gStarJpsiRAASys->SetFillStyle(0);
  gStarJpsiRAASys->SetFillColor(kRed-5);
  gStarJpsiRAASys->SetLineColor(gStarColor);
  globalSys_Star->SetFillStyle(1001);
  globalSys_Star->SetLineColor(gStarColor);
  globalSys_Star->SetFillColor(gStarColor);
  globalSys_Star->SetX1(14.85);
  globalSys_Star->SetX2(15);

  TGraphAsymmErrors *grPhenixJpsiRAA = (TGraphAsymmErrors*)fin->Get("PHENIX_InclJpsiRaaVsPt_020");
  TGraphAsymmErrors *grPhenixJpsiRAASys = (TGraphAsymmErrors*)fin->Get("PHENIX_InclJpsiRaaVsPt_020_sys");
  TBox *globalSys_Phenix = new TBox(14.6, 1-0.1, 14.85, 1+0.1);
  grPhenixJpsiRAA->SetMarkerStyle(gPhenixMarker);
  grPhenixJpsiRAA->SetMarkerSize(2);
  grPhenixJpsiRAA->SetMarkerColor(gPhenixColor);
  grPhenixJpsiRAA->SetLineColor(gPhenixColor);
  grPhenixJpsiRAASys->SetMarkerSize(0);
  grPhenixJpsiRAASys->SetFillStyle(1001);
  grPhenixJpsiRAASys->SetFillColor(kBlue-10);
  grPhenixJpsiRAASys->SetLineColor(kBlue-10);
  globalSys_Phenix->SetFillColor(gPhenixColor);
  globalSys_Phenix->SetLineColor(gPhenixColor);

  TCanvas *c = new TCanvas("RAA_vs_pt", "RAA_vs_pt", 800, 600);
  TH1F *hplot = new TH1F("hplot",";p_{T} (GeV/c);R_{AA}", 1100, 0, 15);
  hplot->GetYaxis()->SetRangeUser(0, 1.6);
  hplot->GetYaxis()->CenterTitle(false);
  hplot->Draw();
  TLine *line = new TLine(0, 1, 15, 1);
  line->SetLineStyle(2);
  line->Draw();

  const char* model_name[1] = {"TAMU model"};
  TFile *fmodel = TFile::Open("inputs/AuAu200.models.root", "read");
  TGraphErrors *gModel = (TGraphErrors*)fmodel->Get(Form("gRaaVsPt_cent%s_TAMU", cent));
  gModel->SetFillStyle(3344);
  gModel->SetFillColor(1);
  gModel->SetLineColor(1);
  gModel->SetMarkerColor(1);
  gModel->Draw("samesE4");
  TLegend *leg = new TLegend(0.53,0.77,0.68,0.82);
  leg->AddEntry(gModel, model_name[0], "F");
  leg->Draw();
  
  TLatex tex(0.5,0.5," ");
  tex.SetNDC();
  tex.DrawLatex(0.16,0.9,Form("Au+Au #sqrt{s_{NN}} = 200 GeV, %s", cent_title));

  leg = new TLegend(0.2,0.7,0.5,0.88);
  leg->SetHeader("Inclusive J/#psi");
  leg->AddEntry(gStarJpsiRAA, "STAR, |y| < 0.5", "P");
  leg->AddEntry(grPhenixJpsiRAA, "PHENIX, |y| < 0.35", "P");
  leg->Draw();
  
  grPhenixJpsiRAASys->Draw("samesE5");
  grPhenixJpsiRAA->Draw("samesPEZ");
  globalSys_Phenix->Draw("fsames");

  gStarJpsiRAASys->Draw("samesE5");
  gStarJpsiRAA->Draw("samesPEZ");
  globalSys_Star->Draw("fsames");

  if(savePlot)
    {
      c->SaveAs("figs/RHIC_Jpsi_RAAvsPt.pdf");
    }
  
}

//================================================
void BES(const int savePlot)
{
  TFile *fin = TFile::Open("inputs/JpsiRaa_BES.root", "read");
  TGraphErrors *gJpsiVsE = (TGraphErrors*)fin->Get("Raa_CME_stat");
  TGraphErrors *gJpsiVsEsys = (TGraphErrors*)fin->Get("Raa_CME_sys");
  for(int ipoint=0; ipoint<gJpsiVsEsys->GetN(); ipoint++)
    {
      gJpsiVsEsys->SetPointError(ipoint, gJpsiVsEsys->GetEX()[ipoint], gJpsiVsEsys->GetEY()[ipoint]*gJpsiVsEsys->GetY()[ipoint]);
    }

  // SPS, RHIC, LHC
  const int ncol = 3;
  const char* col_name[ncol] = {"SPS", "RHIC", "LHC"};
  const int markerStyle[ncol] = {24, 21, 25};
  const int gcolor[ncol] = {kMagenta, kRed+1, kViolet+1};
  TGraphErrors *gJpsiRaa[ncol];
  TGraphAsymmErrors *gJpsiRaaSys[ncol];
  for(int i=0; i<ncol; i++)
    {
      gJpsiRaa[i] = (TGraphErrors*)gJpsiVsE->Clone(Form("gJpsiRaa_%s", col_name[i]));
      gJpsiRaaSys[i] = new TGraphAsymmErrors(gJpsiVsEsys->GetN(), gJpsiVsEsys->GetX(), gJpsiVsEsys->GetY(), gJpsiVsEsys->GetEX(), gJpsiVsEsys->GetEX(), gJpsiVsEsys->GetEY(), gJpsiVsEsys->GetEY());
      
      for(int ipoint=0; ipoint<gJpsiRaa[i]->GetN(); ipoint++)
	{
	  gJpsiRaaSys[i]->SetPointEXlow(ipoint, gJpsiRaaSys[i]->GetX()[ipoint]*(1-pow(10, -0.025)));
	  gJpsiRaaSys[i]->SetPointEXhigh(ipoint, gJpsiRaaSys[i]->GetX()[ipoint]*(pow(10, 0.025)-1));
	  
	  if((i==0 && ipoint>0)
	     || (i==1 && (ipoint==0 || ipoint>4))
	     || (i==2 && ipoint<5) )
	    {
	      gJpsiRaa[i]->SetPoint(ipoint, -1, -1);
	      gJpsiRaa[i]->SetPointError(ipoint, 0, 0);
	      gJpsiRaaSys[i]->SetPoint(ipoint, -1, -1);
	      gJpsiRaaSys[i]->SetPointError(ipoint, 0, 0, 0, 0);
	    }
	}
      gJpsiRaa[i]->SetMarkerStyle(markerStyle[i]);
      gJpsiRaa[i]->SetMarkerSize(1.6);
      gJpsiRaa[i]->SetMarkerColor(gcolor[i]);
      gJpsiRaa[i]->SetLineColor(gcolor[i]);
      gJpsiRaaSys[i]->SetMarkerSize(0);
      gJpsiRaaSys[i]->SetFillStyle(0);
      gJpsiRaaSys[i]->SetLineColor(gcolor[i]);
    }

  // theory
  TGraph *gTamuAll = (TGraph*)fin->Get("Model_total_TAMU");
  TGraph *gTamuPri = (TGraph*)fin->Get("Model_primordial_TAMU");
  TGraph *gTamuReg = (TGraph*)fin->Get("Model_regeneration_TAMU");

  TCanvas *c = new TCanvas("Raa_vs_energy", "Raa_vs_energy", 800, 600);
  gPad->SetLogx();
  TH1F *hplot = new TH1F("hplot",";#sqrt{s_{NN}} (GeV);R_{AA}", 1100, 10, 9000);
  hplot->GetYaxis()->SetRangeUser(0, 1.2);
  hplot->GetYaxis()->CenterTitle(false);
  hplot->Draw();

  gTamuAll->Draw("samesL");
  gTamuPri->SetLineStyle(2);
  gTamuPri->Draw("samesL");
  gTamuReg->SetLineStyle(4);
  gTamuReg->Draw("samesL");

  for(int i=0; i<ncol; i++)
    {
      gJpsiRaaSys[i]->Draw("samesE5");
      gJpsiRaa[i]->Draw("samesPEZ");
    }

  TLegend *leg = new TLegend(0.18, 0.83, 0.53, 0.94);
  leg->SetHeader("Inclusive J/#psi at mid-rapidity");
  leg->SetNColumns(3);
  for(int i=0; i<ncol; i++)
    {
      leg->AddEntry(gJpsiRaa[i], col_name[i], "P");
    }
  leg->Draw();

  leg = new TLegend(0.18, 0.7, 0.65, 0.8);
  leg->SetNColumns(3);
  leg->SetHeader("TAMU model");
  leg->AddEntry(gTamuAll, "Total", "L");
  leg->AddEntry(gTamuPri, "Primordial", "L");
  leg->AddEntry(gTamuReg, "Regenerated", "L");
  leg->Draw();
  
  TLatex tex(0.5,0.5," ");
  tex.SetTextSize(0.025);
  tex.SetNDC();
  tex.DrawLatex(0.16,0.5,"Pb+Pb");
  tex.DrawLatex(0.16,0.47,"0-20%");
  tex.DrawLatex(0.29,0.56,"Au+Au");
  tex.DrawLatex(0.29,0.53,"0-20%");
  tex.DrawLatex(0.45,0.56,"Au+Au");
  tex.DrawLatex(0.45,0.53,"0-10%");
  tex.DrawLatex(0.77,0.72,"Pb+Pb");
  tex.DrawLatex(0.77,0.69,"0-10%");
  tex.DrawLatex(0.85,0.85,"Pb+Pb");
  tex.DrawLatex(0.85,0.82,"0-5%");

  if(savePlot)
    {
      c->SaveAs("figs/Jpsi_RAAvsEnergy.pdf");
    }
}

//================================================
void pA(const int savePlot)
{
  // RpAu, RdAu
  TFile *fpA = TFile::Open("inputs/Jpsi_pA.root", "read");
  TGraphErrors *gJpsiRpA = (TGraphErrors*)fpA->Get("Run15_MTD_Jpsi_RpA");
  TGraphErrors *gJpsiRpASys = (TGraphErrors*)fpA->Get("Run15_MTD_Jpsi_RpA_Sys");
  TBox *globalSys_pAu = (TBox*)fpA->Get("Run15_MTD_Jpsi_RpA_Sys_global");
  gJpsiRpA->SetMarkerStyle(30);
  gJpsiRpA->SetMarkerSize(2.5);
  gJpsiRpA->SetMarkerColor(kRed+1);
  gJpsiRpA->SetLineColor(gJpsiRpA->GetMarkerColor());
  gJpsiRpASys->SetMarkerSize(0);
  gJpsiRpASys->SetFillStyle(0);
  gJpsiRpASys->SetFillColor(kRed-5);
  gJpsiRpASys->SetLineColor(gJpsiRpA->GetMarkerColor());
  globalSys_pAu->SetFillStyle(1001);
  globalSys_pAu->SetLineColor(gJpsiRpA->GetMarkerColor());
  globalSys_pAu->SetFillColor(gJpsiRpA->GetMarkerColor());
  globalSys_pAu->SetX1(9.7);
  globalSys_pAu->SetX2(9.85);

  TGraphAsymmErrors *grPhenixRdAu = (TGraphAsymmErrors*)fpA->Get("PHENIX_dAu200_JpsiRdAu");
  TGraphAsymmErrors *grPhenixRdAuSys = (TGraphAsymmErrors*)fpA->Get("PHENIX_dAu200_JpsiRdAu_sys");
  TBox *globalSys_dAu = (TBox*)fpA->Get("PHENIX_dAu200_JpsiRdAu_sys_global");
  grPhenixRdAu->SetMarkerStyle(20);
  grPhenixRdAu->SetMarkerSize(2);
  grPhenixRdAu->SetMarkerColor(kBlue-3);
  grPhenixRdAu->SetLineColor(kBlue-3);
  grPhenixRdAuSys->SetMarkerSize(0);
  grPhenixRdAuSys->SetFillStyle(1001);
  grPhenixRdAuSys->SetFillColor(kBlue-10);
  grPhenixRdAuSys->SetLineColor(kBlue-10);
  globalSys_dAu->SetFillColor(grPhenixRdAu->GetMarkerColor());
  globalSys_dAu->SetX1(9.85);
  globalSys_dAu->SetX2(10);

  // model comparison
  const int nModel = 7;
  const char* modelName[nModel] = {"ICEM", "nCTEQ15", "CGC+ICEM", "Comover", "TAMU", "ELoss", "EPPS16"};
  const int fillStyle[nModel] = {1001, 3244, 1001, 1001, 3013, 1001, 1001};
  const int fillColor[nModel] = {kOrange-4, kYellow+2, kBlue+2, kViolet+1, kGreen+1, kMagenta, kAzure-9};
  const char* modelLegendName[nModel] = {"ICEM+NLO EPS09", "Lansberg: nCTEQ15", "CGC+ICEM", "Comover", "TAMU", "Eloss+Broadening", "Lansberg: EPPS16"};
  const char* modelLegendStyle[nModel] = {"F", "F", "F", "L", "F", "L", "F"};
  
  TString hName;
  TGraphAsymmErrors *gJpsiRpAModel[nModel];
  for(int i=0; i<nModel; i++)
    {
      if(strcmp(modelName[i],"CGC+ICEM")==0 || strcmp(modelName[i],"ICEM")==0 || strcmp(modelName[i],"TAMU")==0) hName = Form("%s_RpA_pAu200", modelName[i]);
      else             hName = Form("%s_RpA_dAu200", modelName[i]);

 
      gJpsiRpAModel[i] = (TGraphAsymmErrors*)fpA->Get(Form("%s",hName.Data()));
      gJpsiRpAModel[i]->SetFillStyle(fillStyle[i]);
      gJpsiRpAModel[i]->SetMarkerColor(fillColor[i]);
      gJpsiRpAModel[i]->SetLineColor(fillColor[i]);
      gJpsiRpAModel[i]->SetFillColor(fillColor[i]);
    }


  TCanvas *c = new TCanvas("RpAu_vs_model", "RpAu_vs_model", 800, 600);
  TH1F *hplot = new TH1F("hplot",";p_{T} (GeV/c);R_{pAu}, R_{dAu}", 1100, 0, 10);
  hplot->GetYaxis()->SetRangeUser(0, 2.0);
  hplot->GetYaxis()->CenterTitle(false);
  hplot->Draw();
  TLine *line = new TLine(0, 1, 10, 1);
  line->SetLineStyle(2);
  line->Draw();
  
  TLegend *leg = new TLegend(0.15,0.78,0.5,0.94);
  leg->SetHeader("Inclusive J/#psi, #sqrt{s_{NN}} = 200 GeV");
  leg->AddEntry(gJpsiRpA, "STAR p+Au, |y| < 0.5", "P");
  leg->AddEntry(grPhenixRdAu, "PHENIX d+Au, |y| < 0.35", "P");
  leg->Draw();

  leg = new TLegend(0.15,0.15,0.45,0.29);
  leg->SetTextSize(0.035);
  for(int j=0; j<3; j++)
    {
      int i = j;
      if(j==0) i = 6;
      if(j==1) i = 1;
      if(j==2) i = 5;
      TGraphAsymmErrors *gPlot = (TGraphAsymmErrors*)gJpsiRpAModel[i]->Clone(Form("%s_plot",gJpsiRpAModel[i]->GetName()));
      if(strcmp(modelName[i],"ELoss")==0)
	{
	  gPlot->SetLineColor(kGreen+2);
	  gPlot->Draw("samesL");
	  leg->AddEntry(gPlot, "Energy loss (Arleo et al.)", "L");
	}
      if(strcmp(modelName[i],"EPPS16")==0)
	{
	  gPlot->Draw("samesE3Z");
	  leg->AddEntry(gPlot, "EPPS16 reweight (Lansberg et al.)", "F");
	}
      if(strcmp(modelName[i],"nCTEQ15")==0)
	{
	  gPlot->SetFillStyle(1001);
	  gPlot->SetMarkerColor(kRed-10);
	  gPlot->SetLineColor(kRed-10);
	  gPlot->SetFillColorAlpha(kRed-10, 0.75);
	  gPlot->Draw("samesE3Z");
	  leg->AddEntry(gPlot, "nCTEQ15 reweight (Lansberg et al.)", "F");
	}
   
    }
  grPhenixRdAuSys->Draw("samesE5");
  grPhenixRdAu->Draw("samesPEZ");
  globalSys_dAu->Draw("fsames");
  
  gJpsiRpASys->Draw("samesE5");
  gJpsiRpA->Draw("samesPEZ");
  globalSys_pAu->Draw("fsames");

  leg->Draw();


  if(savePlot)
    {
      c->SaveAs("figs/RHIC_Jpsi_RpA_wModel.pdf");
    }
}
