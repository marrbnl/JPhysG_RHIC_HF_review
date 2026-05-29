#include "style.C"

const int gStarColor = kRed+1;
const int gStarMarker = 30;
const int gPhenixColor = kBlue-3;
const int gPhenixMarker = 20;

void pol(const int savePlot);
void xsec(const int savePlot);
void ptdis(const int savePlot);
void rap(const int savePlot);

//================================================
void plot_Jpsi_pp()
{
  gROOT->ProcessLine("set_style()");

  //pol(1);
  //xsec(0);
  //ptdis(1);
  rap(1);
}


//================================================
void rap(const int savePlot)
{

  // PHENIX, pp 510 GeV
  // https://inspirehep.net/literature/1773662
  // 10% global

  // PHENIX, pp 200 GeV
  // https://inspirehep.net/literature/899065
  // 10% global

  const int nfiles = 2;
  TGraphAsymmErrors *gJpsiRap[nfiles];
  TGraphAsymmErrors *gJpsiRapSys[nfiles];
  const char* histo_names[nfiles] = {"gPhenixRap510GeV", "gPhenixRap200GeV"};
  TFile *fin = TFile::Open("inputs/Jpsi_pt_pp.root","read");
  for(int i=0; i<nfiles; i++)
    {
      gJpsiRap[i] = (TGraphAsymmErrors*)fin->Get(Form("%s", histo_names[i]));
      gJpsiRapSys[i] = (TGraphAsymmErrors*)fin->Get(Form("%s_Sys", histo_names[i]));
      int npoints = gJpsiRapSys[i]->GetN();
      for(int ipoint=0; ipoint<npoints; ipoint++)
	{
	  gJpsiRap[i]->SetPointError(ipoint, gJpsiRapSys[i]->GetErrorXlow(ipoint), gJpsiRapSys[i]->GetErrorXhigh(ipoint), gJpsiRap[i]->GetErrorYlow(ipoint), gJpsiRap[i]->GetErrorYhigh(ipoint));
	  gJpsiRapSys[i]->SetPointError(ipoint, 0.025, 0.025, gJpsiRapSys[i]->GetErrorYlow(ipoint), gJpsiRapSys[i]->GetErrorYhigh(ipoint));
	}
    }

  // Add data points for 510 GeV
  // PHENIX measurements: B*dsigma/dy (|y|<0.35), PRD 102 (2020) 7, 072008
  int npoints = gJpsiRap[0]->GetN();
  gJpsiRap[0]->SetPoint(npoints+1, 0, 97.6); // PHENIX
  gJpsiRap[0]->SetPointError(npoints+1, 0.35, 0.35, 3.6, 3.6);
  gJpsiRapSys[0]->SetPoint(npoints+1, 0, 97.6); 
  gJpsiRapSys[0]->SetPointError(npoints+1, 0.025, 0.025, 22.4, 22.4);

  // STAR measurements
  // 510: Phys.Rev.D 100 (2019) 5, 052009
  // 200: Phys.Lett.B 825 (2022) 136865
  const double star_ene[2] = {510, 200};
  const double star_xsec[2] = {67, 43.9}; // nb
  const double star_xsec_range[2] = {0.4, 0.5}; 
  const double star_xsec_stat[2] = {6, 0.7}; 
  const double star_xsec_sys[2] = {10.9, 5.0}; 
  TGraphAsymmErrors *gJpsiXsecStar[nfiles];
  TGraphAsymmErrors *gJpsiXsecStarSys[nfiles];
  for(int i=0; i<nfiles; i++)
    {
      gJpsiXsecStar[i] = new TGraphAsymmErrors(1);
      gJpsiXsecStar[i]->SetName(Form("gJpsiXsecStar_%d",i));
      gJpsiXsecStar[i]->SetPoint(0, 0, star_xsec[i]);
      gJpsiXsecStar[i]->SetPointError(0, star_xsec_range[i], star_xsec_range[i], star_xsec_stat[i], star_xsec_stat[i]);

      gJpsiXsecStarSys[i] = new TGraphAsymmErrors(1);
      gJpsiXsecStarSys[i]->SetName(Form("gJpsiXsecStarSys_%d",i));
      gJpsiXsecStarSys[i]->SetPoint(0, 0, star_xsec[i]);
      gJpsiXsecStarSys[i]->SetPointError(0, 0., 0., star_xsec_sys[i], star_xsec_sys[i]);
    }


  TCanvas *c = new TCanvas("cJpsiRap", "cJpsiRap", 800, 600);
  //gPad->SetLogy();
  TH1F *hplot = new TH1F("hplot", ";y;Br_{ll} #times d#sigma/dy (nb)", 1000, -3, 3);
  hplot->GetYaxis()->SetRangeUser(5, 180);
  //hplot->GetYaxis()->SetTitleOffset(1.3);
  hplot->Draw();
  const int markerStyle[nfiles] = {20, 21};
  const int markerColor[nfiles] = {kRed+2, kBlue+2};
  const double markerSize[nfiles] = {1.3, 1.3};
  const char* legend_names[nfiles] = {"p+p 510 GeV", "p+p 200 GeV"};
  TLegend *leg = new TLegend(0.15, 0.75, 0.85, 0.93);
  leg->SetTextSize(0.04);
  leg->SetHeader("Inclusive J/#psi");
  leg->SetNColumns(2);
  for(int i=0; i<nfiles; i++)
    {
      gJpsiRapSys[i]->SetFillStyle(1001);
      gJpsiRapSys[i]->SetFillColor(markerColor[i]-10);
      gJpsiRapSys[i]->SetMarkerSize(0);
      gJpsiRapSys[i]->SetLineWidth(0);
      gJpsiRapSys[i]->Draw("sameE5Z");
      
      gJpsiRap[i]->SetMarkerStyle(markerStyle[i]);
      gJpsiRap[i]->SetMarkerColor(markerColor[i]);
      gJpsiRap[i]->SetMarkerSize(markerSize[i]);
      gJpsiRap[i]->SetLineColor(markerColor[i]);
      gJpsiRap[i]->Draw("samesPEZ");

      leg->AddEntry(gJpsiRap[i], Form("PHENIX, %s", legend_names[i]), "P");
    }
  leg->Draw();

  gStyle->SetEndErrorSize(3);
  for(int i=0; i<nfiles; i++)
    {
      // gJpsiXsecStarSys[i]->SetFillStyle(3144);
      // gJpsiXsecStarSys[i]->SetFillColor(markerColor[i]);
      // gJpsiXsecStarSys[i]->SetMarkerSize(0);
      gJpsiXsecStarSys[i]->SetLineWidth(2);
      gJpsiXsecStarSys[i]->SetLineColor(markerColor[i]-2);
      gJpsiXsecStarSys[i]->Draw("sames[]");
      
      gJpsiXsecStar[i]->SetMarkerStyle(markerStyle[i]+4);
      gJpsiXsecStar[i]->SetMarkerColor(markerColor[i]-2);
      gJpsiXsecStar[i]->SetMarkerSize(markerSize[i]);
      gJpsiXsecStar[i]->SetLineColor(markerColor[i]-2);
      gJpsiXsecStar[i]->Draw("samesPEZ");

      leg->AddEntry(gJpsiXsecStar[i], Form("STAR, %s", legend_names[i]), "P");
    }
    
  if(savePlot)
    {
      c->SaveAs("figs/Jpsi_pp_rap.pdf");
    }


  
}

//================================================
void ptdis(const int savePlot)
{
  // PHENIX, pp 510 GeV, 1.2 < |y| < 2.2
  // https://inspirehep.net/literature/1773662
  // 10% global

  // PHENIX, pp 510 GeV, |y| < 0.35
  // https://inspirehep.net/literature/1798581
  // could not find data points

  // PHENIX, pp 200 GeV, 1.2 < |y| < 2.2
  // https://inspirehep.net/literature/731611
  // 10% global

  // PHENIX, pp 200 GeV, |y| < 0.35
  // https://inspirehep.net/literature/899065
  // 10% global

  // STAR, pp 510 GeV, |y| < 0.4 (low pt, 11.2% global); 500 GeV, |y| < 1 (high pt, 8.1% global)
  // https://inspirehep.net/literature/1735184

  // STAR, pp 200 GeV, |y| < 1
  // https://inspirehep.net/literature/1672453
  // 10% global for 0-1.5 GeV/c, and 8.5% for 1.5-14 GeV/c

  const int isTheory = 1;

  // 200 GeV
  const int nfiles_200 = 4;
  TGraphAsymmErrors *gJpsiPt200[nfiles_200];
  TGraphAsymmErrors *gJpsiPtSys200[nfiles_200];
  const char* histo_names_200[nfiles_200] = {"gPhenixMid200GeV", "gStarMidMu200GeV", "gStarMidEl200GeV", "gPhenixForward200GeV"};
  TFile *fin = TFile::Open("inputs/Jpsi_pt_pp.root","read");
  for(int i=0; i<nfiles_200; i++)
    {
      gJpsiPt200[i] = (TGraphAsymmErrors*)fin->Get(Form("%s", histo_names_200[i]));
      gJpsiPtSys200[i] = (TGraphAsymmErrors*)fin->Get(Form("%s_Sys", histo_names_200[i]));

      // multiple by pow(10, -i)
      double scale = pow(10, 0);
      if(isTheory)
	{
	  if(i==3) scale = 0.1;
	}
      int npoint = gJpsiPt200[i]->GetN();
      for(int ipoint=0; ipoint<npoint; ipoint++)
	{
	  gJpsiPt200[i]->SetPoint(ipoint, gJpsiPt200[i]->GetX()[ipoint], gJpsiPt200[i]->GetY()[ipoint]*scale);
	  gJpsiPt200[i]->SetPointError(ipoint, gJpsiPt200[i]->GetEXlow()[ipoint], gJpsiPt200[i]->GetEXhigh()[ipoint], gJpsiPt200[i]->GetEYlow()[ipoint]*scale, gJpsiPt200[i]->GetEYhigh()[ipoint]*scale);

	  gJpsiPtSys200[i]->SetPoint(ipoint, gJpsiPtSys200[i]->GetX()[ipoint], gJpsiPtSys200[i]->GetY()[ipoint]*scale);
	  gJpsiPtSys200[i]->SetPointError(ipoint, gJpsiPtSys200[i]->GetEXlow()[ipoint], gJpsiPtSys200[i]->GetEXhigh()[ipoint], gJpsiPtSys200[i]->GetEYlow()[ipoint]*scale, gJpsiPtSys200[i]->GetEYhigh()[ipoint]*scale);
	}
    }

  TLegend *leg = NULL;
  TCanvas *c = new TCanvas("cJpsiPt_200GeV", "cJpsiPt_200GeV", 800, 700);
  gPad->SetLogy();
  TH1F *hplot = new TH1F("hplot", ";p_{T} (GeV/c);Br_{ll}#timesd^{2}#sigma/2#pip_{T}dp_{T}dy (nb*(GeV/c)^{-2})", 1000, 0, 14);
  hplot->GetYaxis()->SetRangeUser(1e-6, 30);
  hplot->GetYaxis()->SetTitleOffset(1.3);
  hplot->Draw();

  const int theoryColor[4] = {kMagenta+1, kAzure+1, kOrange+10, kBlue-6};
  const int theoryFill[4] = {3144, 3001, 1001, 3005};
  if(isTheory)
    {
      TGraphAsymmErrors *gr_theory_200[3];
      TFile *fin_th_200 = TFile::Open("inputs/Jpsi_pt/jpsi_xsec_pp200_run12.root");
      gr_theory_200[0] = (TGraphAsymmErrors*)fin_th_200->Get("gYieldVsPt_pp_CGC_NRQCD");
      gr_theory_200[1] = (TGraphAsymmErrors*)fin_th_200->Get("gYieldVsPt_pp_NLO_NRQCD");
      gr_theory_200[2] = (TGraphAsymmErrors*)fin_th_200->Get("gYieldVsPt_pp_CEM_fill");

      // add B->Jpsi contribution from FONLL
      TFile *fmodel = TFile::Open("inputs/pAu200.models.root","read");
      TGraphAsymmErrors *gFONLL = (TGraphAsymmErrors*)fmodel->Get("FONLL_BtoJpsiXsec_pp200");
      for(int i=0; i<2; i++)
	{
	  int npoints2 = gFONLL->GetN();
	  int npoints = gr_theory_200[i]->GetN();
	  for(int ipoint=0; ipoint<npoints; ipoint++)
	    {
	      double pt = gr_theory_200[i]->GetX()[ipoint];
	      double val = gr_theory_200[i]->GetY()[ipoint];
	      double errh = gr_theory_200[i]->GetEYhigh()[ipoint];
	      double errl = gr_theory_200[i]->GetEYlow()[ipoint];
	      //cout << pt << "  " << val << "  " << errh << "  " << errl << endl;
	      
	      int index = -1;
	      for(int jpoint=0; jpoint<npoints2-1; jpoint++)
		{
		  double x1 = gFONLL->GetX()[jpoint];
		  double x2 = gFONLL->GetX()[jpoint+1];
		  if(pt>=x1 && pt<x2) 
		    {
		      index = jpoint;
		      break;
		    }
		}
	      if(index>=0)
		{
		  // linear extrapolation
		  double addon = gFONLL->GetY()[index] - (gFONLL->GetY()[index]-gFONLL->GetY()[index+1])/(gFONLL->GetX()[index+1]-gFONLL->GetX()[index])*(pt-gFONLL->GetX()[index]);
		  double adderrh = gFONLL->GetEYhigh()[index] - (gFONLL->GetEYhigh()[index]-gFONLL->GetEYhigh()[index+1])/(gFONLL->GetX()[index+1]-gFONLL->GetX()[index])*(pt-gFONLL->GetX()[index]);
		  double adderrl = gFONLL->GetEYlow()[index] - (gFONLL->GetEYlow()[index]-gFONLL->GetEYlow()[index+1])/(gFONLL->GetX()[index+1]-gFONLL->GetX()[index])*(pt-gFONLL->GetX()[index]);
		  gr_theory_200[i]->SetPoint(ipoint, pt, val+addon);
		  gr_theory_200[i]->SetPointError(ipoint, gr_theory_200[i]->GetEXlow()[ipoint], gr_theory_200[i]->GetEXhigh()[ipoint], 
						  sqrt(errl*errl+adderrl*adderrl), sqrt(errh*errh+adderrh*adderrh));
		  //printf("[i] Found index = %d, pt = [%2.4f, %2.4f), add = %2.4e + %2.4e - %2.4e, %2.4f%%\n", index, gFONLL->GetX()[index], gFONLL->GetX()[index+1], addon, adderrh, adderrl, addon/(addon+val)*100);
		  //cout << gFONLL->GetY()[index] << "  " << gFONLL->GetY()[index+1] << "  " << addon << endl;
		}
	    }
	}
 
      for(int i=0; i<3; i++)
	{
	  gr_theory_200[i]->SetFillStyle(theoryFill[i]);
	  gr_theory_200[i]->SetFillColorAlpha(theoryColor[i], 0.5);
	  gr_theory_200[i]->SetLineColor(0);
	  gr_theory_200[i]->Draw("samesE3");
	}

      leg = new TLegend(0.15, 0.15, 0.5, 0.28);
      leg->SetTextSize(0.033);
      leg->AddEntry(gr_theory_200[0], "CGC+NRQCD+FONLL", "F");
      leg->AddEntry(gr_theory_200[1], "NLO NRQCD+FONLL", "F");
      leg->AddEntry(gr_theory_200[2], "ICEM+FONLL", "F");
      leg->Draw();
    }
    
  const int markerStyle[nfiles_200] = {24, 25, 28, 30};
  const int markerColor[nfiles_200] = {kRed+2, kBlue+2, kGreen+2, kMagenta+2};
  const double markerSize[nfiles_200] = {1.3, 1.3, 1.8, 1.8};
  const char* legend_names_200[nfiles_200] = {"PHENIX, |y| < 0.35", "STAR, |y| < 0.5", "STAR, |y| < 1.0", "PHENIX, 1.2 < |y| < 2.2"};
  leg = new TLegend(0.46, 0.70, 0.82, 0.93);
  leg->SetTextSize(0.035);
  leg->SetHeader("Inclusive J/#psi in p+p 200 GeV");
  for(int i=0; i<nfiles_200; i++)
    {
      gJpsiPtSys200[i]->SetFillStyle(1001);
      gJpsiPtSys200[i]->SetFillColor(markerColor[i]-8);
      gJpsiPtSys200[i]->SetMarkerSize(0);
      gJpsiPtSys200[i]->SetLineColor(markerColor[i]-8);
      gJpsiPtSys200[i]->Draw("sameE5");
      
      gJpsiPt200[i]->SetMarkerStyle(markerStyle[i]);
      gJpsiPt200[i]->SetMarkerColor(markerColor[i]);
      gJpsiPt200[i]->SetMarkerSize(markerSize[i]);
      gJpsiPt200[i]->SetLineColor(markerColor[i]);
      gJpsiPt200[i]->Draw("samesPE");

      if(i<3) leg->AddEntry(gJpsiPt200[i], Form("%s", legend_names_200[i]), "P");
      else
	{
	  if(isTheory) leg->AddEntry(gJpsiPt200[i], Form("%s (#times0.1)", legend_names_200[i]), "P");
	  else leg->AddEntry(gJpsiPt200[i], Form("%s", legend_names_200[i]), "P");
	}
    }
  leg->Draw();
  if(savePlot)
    {
      if(isTheory) c->SaveAs("figs/Jpsi_pp200_pt_wTheory.pdf");
      else c->SaveAs("figs/Jpsi_pp200_pt.pdf");
    }
  

  // 500 GeV
  const int nfiles_500 = 3;
  TGraphAsymmErrors *gJpsiPt500[nfiles_500];
  TGraphAsymmErrors *gJpsiPtSys500[nfiles_500];
  const char* histo_names_500[nfiles_500] = {"gStarMidMu510GeV", "gStarMidEl500GeV", "gPhenixForward510GeV"};
  for(int i=0; i<nfiles_500; i++)
    {
      gJpsiPt500[i] = (TGraphAsymmErrors*)fin->Get(Form("%s", histo_names_500[i]));
      gJpsiPtSys500[i] = (TGraphAsymmErrors*)fin->Get(Form("%s_Sys", histo_names_500[i]));

      // multiple by pow(10, -i)
      double scale = pow(10, 0);
      if(isTheory)
	{
	  if(i==2) scale = 0.1;
	}
      int npoint = gJpsiPt500[i]->GetN();
      for(int ipoint=0; ipoint<npoint; ipoint++)
	{
	  gJpsiPt500[i]->SetPoint(ipoint, gJpsiPt500[i]->GetX()[ipoint], gJpsiPt500[i]->GetY()[ipoint]*scale);
	  gJpsiPt500[i]->SetPointError(ipoint, gJpsiPt500[i]->GetEXlow()[ipoint], gJpsiPt500[i]->GetEXhigh()[ipoint], gJpsiPt500[i]->GetEYlow()[ipoint]*scale, gJpsiPt500[i]->GetEYhigh()[ipoint]*scale);

	  gJpsiPtSys500[i]->SetPoint(ipoint, gJpsiPtSys500[i]->GetX()[ipoint], gJpsiPtSys500[i]->GetY()[ipoint]*scale);
	  gJpsiPtSys500[i]->SetPointError(ipoint, gJpsiPtSys500[i]->GetEXlow()[ipoint], gJpsiPtSys500[i]->GetEXhigh()[ipoint], gJpsiPtSys500[i]->GetEYlow()[ipoint]*scale, gJpsiPtSys500[i]->GetEYhigh()[ipoint]*scale);
	}
    }
  
  c = new TCanvas("cJpsiPt_500GeV", "cJpsiPt_500GeV", 800, 700);
  gPad->SetLogy();
  TH1F *hplot2 = new TH1F("hplot2", ";p_{T} (GeV/c);Br_{ll}#timesd^{2}#sigma/2#pip_{T}dp_{T}dy (nb*(GeV/c)^{-2})", 1000, 0, 20);
  hplot2->GetYaxis()->SetRangeUser(1e-6, 30);
  hplot2->GetYaxis()->SetTitleOffset(1.3);
  hplot2->Draw();

  if(isTheory)
    {
      TGraphAsymmErrors *gr_theory_500[3];
      TFile *fin_th_500 = TFile::Open("inputs/Jpsi_pp_500GeV_theory.root");
      gr_theory_500[0] = (TGraphAsymmErrors*)fin_th_500->Get("gCgcLowPt_added");
      gr_theory_500[1] = (TGraphAsymmErrors*)fin_th_500->Get("gCgcHighPt_added");
      gr_theory_500[2] = (TGraphAsymmErrors*)fin_th_500->Get("gICEM_added");
      for(int i=0; i<3; i++)
	{
	  gr_theory_500[i]->SetFillStyle(theoryFill[i]);
	  gr_theory_500[i]->SetFillColorAlpha(theoryColor[i], 0.5);
	  gr_theory_500[i]->SetLineColor(0);
	  gr_theory_500[i]->Draw("samesE3");
	}

      leg = new TLegend(0.15, 0.15, 0.5, 0.28);
      leg->SetTextSize(0.033);
      leg->AddEntry(gr_theory_500[0], "CGC+NRQCD+FONLL", "F");
      leg->AddEntry(gr_theory_500[1], "NLO NRQCD+FONLL", "F");
      leg->AddEntry(gr_theory_500[2], "ICEM+FONLL", "F");
      leg->Draw();
    }
  
  const char* legend_names_500[nfiles_500] = {"STAR, |y| < 0.4", "STAR, |y| < 1.0", "PHENIX, 1.2 < |y| < 2.2"};
  leg = new TLegend(0.46, 0.75, 0.82, 0.93);
  leg->SetTextSize(0.035);
  leg->SetHeader("Inclusive J/#psi in p+p 500/510 GeV");
  for(int i=0; i<nfiles_500; i++)
    {
      int index = i+1;
      gJpsiPtSys500[i]->SetFillStyle(1001);
      gJpsiPtSys500[i]->SetFillColor(markerColor[index]-8);
      gJpsiPtSys500[i]->SetMarkerSize(0);
      gJpsiPtSys500[i]->SetLineColor(markerColor[index]-8);
      gJpsiPtSys500[i]->Draw("sameE5");
      
      gJpsiPt500[i]->SetMarkerStyle(markerStyle[index]);
      gJpsiPt500[i]->SetMarkerColor(markerColor[index]);
      gJpsiPt500[i]->SetMarkerSize(markerSize[index]);
      gJpsiPt500[i]->SetLineColor(markerColor[index]);
      gJpsiPt500[i]->Draw("samesPE");

      //leg->AddEntry(gJpsiPt500[i], Form("%s (#times10^{-%d})", legend_names_500[i], i+1), "P");
      if(i<2) leg->AddEntry(gJpsiPt500[i], Form("%s", legend_names_500[i]), "P");
      else
	{
	  if(isTheory) leg->AddEntry(gJpsiPt500[i], Form("%s (#times0.1)", legend_names_500[i]), "P");
	  else leg->AddEntry(gJpsiPt500[i], Form("%s", legend_names_500[i]), "P");
	}
    }
  leg->Draw();
  if(savePlot)
    {
      if(isTheory) c->SaveAs("figs/Jpsi_pp500_pt_wTheory.pdf");
      else c->SaveAs("figs/Jpsi_pp500_pt.pdf");
    }
  
  // 500 GeV with theory
  //c = new TCanvas("cJpsiPt_500GeV_theory", "cJpsiPt_500GeV_theory", 800, 600);
  
  
}

//================================================
void xsec(const int savePlot)
{
  // Convert everything to dsigma/dy in unit of nb
  const double BR_ee = 0.0594;
  
  // PHENIX measurements
  // 510: B*dsigma/dy (|y|<0.35), PRD 102 (2020) 7, 072008
  // 200: PRL. 98 (2007) 232002
  const double phenix_ene[2] = {510, 200};
  const double phenix_xsec[2] = {97.6/BR_ee*1e-3, 43.4/BR_ee*1e-3}; // nb
  const double phenix_xsec_errh[2] = {22.7/BR_ee*1e-3, 5.4/BR_ee*1e-3}; // total error
  const double phenix_xsec_errl[2] = {22.7/BR_ee*1e-3, 5.4/BR_ee*1e-3}; // total error

  // STAR measurements
  // 510: Phys.Rev.D 100 (2019) 5, 052009
  // 200: Phys.Lett.B 825 (2022) 136865 
  const double star_ene[2] = {510, 200};
  const double star_xsec[2] = {67/BR_ee*1e-3, 43.9/BR_ee*1e-3}; // nb
  const double star_xsec_errh[2] = {13.6/BR_ee*1e-3, 6.1/BR_ee*1e-3}; // total error
  const double star_xsec_errl[2] = {13.6/BR_ee*1e-3, 6.1/BR_ee*1e-3}; // total error

  // CDF
  // 1960: sigma = 4.08±0.02(stat)−0.33+0.36(sys) ub, for |y|<0.6, PRD 71 (2005) 032001
  const double other_ene[1] = {1960};
  const double other_xsec[1] = {4.08/1.2}; // nb
  const double other_xsec_errh[1] = {0.36/1.2}; // total error
  const double other_xsec_errl[1] = {0.33/1.2}; // total error

  // ALICe
  // 13000: dσ /dy|y=0 = 8.97 ± 0.24 (stat) ± 0.48 (syst) ± 0.15 (lumi) μb, Eur. Phys. J. C 81 (2021) 1121
  // 2760: Phys. Lett. B 718 (2012) 295
  // 5020: JHEP 10 (2019) 084
  // 7000: Phys. Lett. B 704 (2011) 442
  const double alice_ene[4] = {13000, 2760, 5020, 7000};
  const double alice_xsec[4] = {8.97, 4.31, 5.64, 12.4/1.8}; // nb
  const double alice_xsec_errh[4] = {0.56, 1.26, 0.41, 2.8/1.8}; // total error
  const double alice_xsec_errl[4] = {0.56, 1.26, 0.41, 3.4/1.8}; // total error

  const double err_dummy[4] = {0, 0, 0, 0};

  TGraphAsymmErrors *gJpsiXsec[4];
  gJpsiXsec[0] = new TGraphAsymmErrors(2, phenix_ene, phenix_xsec, err_dummy, err_dummy, phenix_xsec_errl, phenix_xsec_errh);
  gJpsiXsec[0]->SetName("Phenix_Jpsi_xsec");
  gJpsiXsec[0]->SetMarkerStyle(gPhenixMarker);
  gJpsiXsec[0]->SetMarkerSize(1.8);
  gJpsiXsec[0]->SetMarkerColor(gPhenixColor);
  gJpsiXsec[0]->SetLineColor(gPhenixColor);
  
  gJpsiXsec[1] = new TGraphAsymmErrors(2, star_ene, star_xsec, err_dummy, err_dummy, star_xsec_errl, star_xsec_errh);
  gJpsiXsec[1]->SetName("Star_Jpsi_xsec");
  gJpsiXsec[1]->SetMarkerStyle(gStarMarker);
  gJpsiXsec[1]->SetMarkerSize(2);
  gJpsiXsec[1]->SetMarkerColor(gStarColor);
  gJpsiXsec[1]->SetLineColor(gStarColor);

  gJpsiXsec[2] = new TGraphAsymmErrors(1, other_ene, other_xsec, err_dummy, err_dummy, other_xsec_errl, other_xsec_errh);
  gJpsiXsec[2]->SetName("CDF_Jpsi_xsec");
  gJpsiXsec[2]->SetMarkerStyle(28);
  gJpsiXsec[2]->SetMarkerSize(2);
  // gJpsiXsec[2]->SetMarkerColor(gStarColor);
  // gJpsiXsec[2]->SetLineColor(gStarColor);

  gJpsiXsec[3] = new TGraphAsymmErrors(4, alice_ene, alice_xsec, err_dummy, err_dummy, alice_xsec_errl, alice_xsec_errh);
  gJpsiXsec[3]->SetName("Alice_Jpsi_xsec");
  gJpsiXsec[3]->SetMarkerStyle(25);
  gJpsiXsec[3]->SetMarkerSize(1.8);

  TCanvas *c = new TCanvas("JpsiXsec_vs_energy", "JpsiXsec_vs_energy", 800, 600);
  gPad->SetLogx();
  gPad->SetLogy();
  TH1F *hplot = new TH1F("hplot",";#sqrt{s_{NN}} (GeV);d#sigma/dy|_{y=0} (#mub)", 1100, 100, 28000);
  hplot->GetYaxis()->SetRangeUser(0.5, 20);
  hplot->GetYaxis()->CenterTitle(false);
  hplot->Draw();
  TString legName[4] = {"PHENIX", "STAR", "CDF", "ALICE"};
  TLegend *leg = new TLegend(0.18, 0.65, 0.53, 0.90);
  leg->SetHeader("Inclusive J/#psi in p+p, p+#bar{p}");
  leg->Draw();
  for(int i=0; i<4; i++)
    {
      leg->AddEntry(gJpsiXsec[i], legName[i].Data(), "P");
      gJpsiXsec[i]->Draw("samesPEZ");
    }

  if(savePlot)
    {
      c->SaveAs("figs/Jpsi_xsec_pp.pdf");
    } 
}

//================================================
void pol(const int savePlot)
{
  const char* frame[2] = {"Hx", "Cs"};
  const char* param[3] = {"Th", "Ph", "ThPh"};
  const char* param_title[3] = {"#lambda_{#theta}", "#lambda_{#phi}", "#lambda_{#theta#phi}"};
  const char* errtype[3] = {"stat", "sys", "tot"};
  const char* det[4] = {"PHENIX_pp510_fwd", "PHENIX_pp510_mid", "STAR_pp200_dimuon", "STAR_pp200_dielectron"};
  const int markers[4] = {34, 27, 30, 43};
  const int colors[4] = {gPhenixColor, kGreen+2, gStarColor, kMagenta+1};
  TGraphAsymmErrors *gJpsiPol[2][3][3][4];
  
  TFile *fin = TFile::Open("inputs/Jpsi_pol_pp.root", "read");
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<3; j++)
	{
	  for(int k=0; k<3; k++)
	    {
	      for(int d=0; d<4; d++)
		{
		  gJpsiPol[i][j][k][d] = NULL;
		  if(std::strcmp(det[d], "STAR_pp200_dimuon") == 0 && std::strcmp(param[j], "ThPh") == 0) continue;
		  gJpsiPol[i][j][k][d] = (TGraphAsymmErrors*)fin->Get(Form("%s_%s_%s_%s", det[d], errtype[k], frame[i], param[j]));
		  for(int ipoint=0; ipoint<gJpsiPol[i][j][k][d]->GetN(); ipoint++)
		    {
		      gJpsiPol[i][j][k][d]->SetPointEXhigh(ipoint, 0);
		      gJpsiPol[i][j][k][d]->SetPointEXlow(ipoint, 0);
		    }
		  if(std::strcmp(errtype[k],"stat")==0 || std::strcmp(errtype[k],"tot")==0 )
		    {
		      gJpsiPol[i][j][k][d]->SetMarkerStyle(markers[d]);
		      if(markers[d]==43) gJpsiPol[i][j][k][d]->SetMarkerSize(2.8);
		      else gJpsiPol[i][j][k][d]->SetMarkerSize(2.2);
		      gJpsiPol[i][j][k][d]->SetMarkerColor(colors[d]);
		      gJpsiPol[i][j][k][d]->SetLineColor(colors[d]);
		    }
		  else
		    {
		      gJpsiPol[i][j][k][d]->SetMarkerSize(0);
		      gJpsiPol[i][j][k][d]->SetFillStyle(0);
		      gJpsiPol[i][j][k][d]->SetFillColor(colors[d]-5);
		      gJpsiPol[i][j][k][d]->SetLineColor(colors[d]);
		    }
		}
	    }
	}
    }

  TCanvas *c = new TCanvas("JpsiPol_vs_pt", "JpsiPol_vs_pt", 1400, 700);
  c->Divide(3, 2);
  
  TH1F *hplot = new TH1F("hplot",";p_{T} (GeV/c);R_{AA}", 1100, 0, 10);
  hplot->GetYaxis()->SetRangeUser(-1, 1);
  hplot->GetYaxis()->CenterTitle(true);
  hplot->GetXaxis()->SetTitleSize(0.06);
  hplot->GetXaxis()->SetLabelSize(0.045);
  hplot->GetXaxis()->SetTitleOffset(0.9);
  hplot->GetYaxis()->SetTitleSize(0.08);
  hplot->GetYaxis()->SetLabelSize(0.045);
  hplot->GetYaxis()->SetTitleOffset(0.65);
  hplot->SetLineStyle(2);
  hplot->SetLineWidth(0);


  // theory curves
  const char* theoryName[4] = {"CGC", "JXW", "ZHF", "ICEM"};
  const int theoryColor[4] = {kRed+2, kAzure+1, kBlue-6, kOrange+10};
  const int theoryFillStyle[4] = {1001, 3335, 3244, 3305};

  
  TGraphAsymmErrors *gTheory[4][2][3];
  
  TFile *fTh[4];
  for(int i=0; i<4; i++)
    {
      if(i<3) fTh[i] = TFile::Open(Form("inputs/Jpsi_pol/theory_NRQCD_%s.root", theoryName[i]));
      else fTh[i] = TFile::Open(Form("inputs/Jpsi_pol/theory_%s.root", theoryName[i]));
      for(int fr=0; fr<2; fr++)
	{
	  for(int tn=0; tn<3; tn++)
	    {
	      gTheory[i][fr][tn] = NULL;
	      if(i!=0 && tn>1) continue;
	      if(i==3 && (fr>0 || tn>0)) continue;
	      if(i==0) gTheory[i][fr][tn] = (TGraphAsymmErrors*)fTh[i]->Get(Form("gErrAsy_%s_%s_%s_y1", theoryName[i], param[tn], frame[fr]));
	      if(i==1||i==2) gTheory[i][fr][tn] = (TGraphAsymmErrors*)fTh[i]->Get(Form("gErrAsy_%s_%s_%s_y1", theoryName[i], frame[fr], param[tn]));
	      if(i==3) gTheory[i][fr][tn] = (TGraphAsymmErrors*)fTh[i]->Get(Form("gErrAsy_%s_%s_%s", theoryName[i], frame[fr], param[tn]));

	      gTheory[i][fr][tn]->SetFillStyle(1001);
	      gTheory[i][fr][tn]->SetFillColorAlpha(theoryColor[i], 0.5);
	      gTheory[i][fr][tn]->SetLineColor(theoryColor[i]);
	    }
	}
    }

  TLatex tex(0.5,0.5," ");
  tex.SetNDC();
  tex.SetTextSize(0.05);
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<3; j++)
	{
	  c->cd(i*3+j+1);
	  hplot->GetYaxis()->SetTitle(Form("%s", param_title[j]));
	  hplot->DrawCopy();

	  for(int t=0; t<4; t++)
	    {
	      if(gTheory[t][i][j]) gTheory[t][i][j]->Draw("sames E3");
	    }

	  if(i==0)   tex.DrawLatex(0.15,0.9,"HX");
	  if(i==1)   tex.DrawLatex(0.15,0.9,"CS");

	  for(int d=0; d<4; d++)
	    {
	      if(std::strcmp(det[d], "STAR_pp200_dimuon") == 0 && std::strcmp(param[j], "ThPh") == 0) continue;
	      gJpsiPol[i][j][2][d]->Draw("samesPEZ");
	    }
	}
    }

  c->cd(1);
  TLegend *leg = new TLegend(0.25,0.75,0.68,0.94);
  leg->SetTextSize(0.05);
  leg->SetHeader("PHENIX, p+p 510 GeV, inclusive J/#psi");
  leg->AddEntry(gJpsiPol[0][0][2][0], "1.2 < y < 2.2", "P");
  leg->AddEntry(gJpsiPol[0][0][2][1], "-0.35 < y < 0.35", "P");
  leg->Draw();

  c->cd(2);
  leg = new TLegend(0.25,0.75,0.68,0.94);
  leg->SetTextSize(0.05);
  leg->SetHeader("STAR, p+p 200 GeV, inclusive J/#psi");
  leg->AddEntry(gJpsiPol[0][0][2][2], "-0.5 < y < 0.5", "P");
  leg->AddEntry(gJpsiPol[0][0][2][3], "-1.0 < y < 1.0", "P");
  leg->Draw();

  c->cd(3);
  leg = new TLegend(0.35,0.15,0.68,0.28);
  leg->SetTextSize(0.05);
  leg->SetHeader("p+p 200 GeV, direct J/#psi, |y| < 1.0");
  leg->AddEntry(gTheory[0][0][0], "CGC+NRQCD", "F");
  leg->Draw();

  leg = new TLegend(0.35,0.7,0.68,0.94);
  leg->SetTextSize(0.05);
  leg->SetHeader("p+p 200 GeV, prompt J/#psi, |y| < 1.0");
  leg->AddEntry(gTheory[1][0][0], "NLO NRQCD: B. Gong, et. al.", "F");
  leg->AddEntry(gTheory[2][0][0], "NLO NRQCD: H.-F. Zhang, et. al.", "F");
  leg->AddEntry(gTheory[3][0][0], "ICEM", "F");
  leg->Draw();

  if(savePlot)
    {
      c->SaveAs("figs/RHIC_Jpsi_polarization.pdf");
    } 
}
