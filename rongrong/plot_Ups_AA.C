#include "style.C"

void Ebinding(const int savePlot);

//================================================
void plot_Ups_AA()
{
  gROOT->ProcessLine("set_style()");

  Ebinding(0);
}

//================================================
void Ebinding(const int savePlot)
{
   // psi(2S), Y(3S), Y(2S), J/psi, Y(1S)
  const int nPart = 5;

  //const double temp[nPart] = {0.95, 1.01, 1.25, 1.37, 2.66}; // melting temperature JHEP12(2015)101
  const double temp[nPart] = {0.044, 0.205, 0.533, 0.634, 1.096}; // binding energy NP A 946 (2016) 49

  //========= LHC (not used)
  // PbPb
  // Y(1S) - 0-5%, pt > 0, |y| < 2.4 - CMS, PLB 790 (2019) 270
  // Y(2S) - 0-5%, pt > 0, |y| < 2.4 - CMS, CMS-PAS-HIN-21-007
  // Y(3S) - 0-30%, pt > 0, |y| < 2.4 - CMS, CMS-PAS-HIN-21-007
  // J/psi, Psi(2S) - 0-10%, 6.5-30 GeV/c, |y| < 1.6 - CMS, Eur. Phys. J. C 78 (2018) 509
  const double raa_lhc[nPart] = {0.138, 0.050, 0.076, 0.251, 0.319}; // Psi(2S) is an upper limit 95% CL
  const double raa_lhc_stat[nPart] = {-999, 0.016, 0.018, 0.005, 0.019};
  const double raa_lhc_sys[nPart] = {-999, 0.011, 0.004, 0.020, 0.020};
  const double raa_lhc_global[nPart] = {0, 0, 0, 0.057, 0}; // percentage

  //========== RHIC

  // AuAu
  // Jpsi - 0-10%, pt > 5 GeV/c, |y| < 0.5 - Phys. Lett. B 797 (2019) 134917
  // Y(1S, 2S, 3S) - 0-60%, pt > 0, |y| < 0.5 - 2207.06568
  const double raa_rhic[nPart] = {-999, 0.17, 0.26, 0.3265, 0.40}; // Y(3S) is an upper limit 95% CL
  const double raa_rhic_stat[nPart] = {-999, -999, 0.08, 0.0411, 0.03};
  const double raa_rhic_sys[nPart] = {-999, -999, 0.03, 0.0457, 0.04};
  const double raa_rhic_global[nPart] = {0, 0, 0.205, 0.15, 0.20};

  const int gcolors[nPart] = {kBlack, kRed+2, kBlue+2, kGreen+2, kOrange+2};
  const int gfillcolors[nPart] = {kGray, kRed-10, kBlue-10, kGreen-10, kOrange-8};
  const int gmarkers_aa[nPart] = {20, 21, 29, 33, 34};
  const double markersize[nPart] = {2, 2, 2.5, 2.5, 2.5};

  TGraphErrors *gRaa[2][nPart];
  TGraphErrors *gRaa_sys[2][nPart];
  const double gwidth = 0.025;
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<nPart; j++)
      	{
      	  gRaa[i][j] = new TGraphErrors(1);
      	  gRaa[i][j]->SetName(Form("gRaa_%d_%d",i,j));
      	  gRaa_sys[i][j] = new TGraphErrors(1);
      	  gRaa_sys[i][j]->SetName(Form("gRaa_sys_%d_%d",i,j));
	  if(i==0)
	    {
	      if(j==0) continue;
	      gRaa[i][j]->SetPoint(0, temp[j], raa_lhc[j]);
	      gRaa[i][j]->SetPointError(0, 0, raa_lhc_stat[j]);

	      gRaa_sys[i][j]->SetPoint(0, temp[j], raa_lhc[j]);
	      gRaa_sys[i][j]->SetPointError(0, gwidth, sqrt(pow(raa_lhc_sys[j],2)+pow(raa_lhc_global[j]*raa_lhc[j],2)));
	    }
	  if(i==1)
	    {
	      if(j==1) continue;
	      gRaa[i][j]->SetPoint(0, temp[j], raa_rhic[j]);
	      gRaa[i][j]->SetPointError(0, 0, raa_rhic_stat[j]);

	      gRaa_sys[i][j]->SetPoint(0, temp[j], raa_rhic[j]);
	      gRaa_sys[i][j]->SetPointError(0, gwidth, sqrt(pow(raa_rhic_sys[j],2)+pow(raa_rhic_global[j]*raa_rhic[j],2)));
	    }
      	}
    }

  // plotting
  const char* partName[5] = {"#psi(2S)", "#Upsilon(3S)", "#Upsilon(2S)", "J/#psi", "#Upsilon(1S)"};
  const char* collider[2] = {"LHC", "RHIC"};

  const char* leg_aa_lhc[5] = {"#psi(2S) 0-10% |y| < 1.6, 6.5 - 30 GeV/c",
			       "#Upsilon(3S) 0-30% |y| < 2.4, p_{T} > 0 GeV/c",
			       "#Upsilon(2S) 0-5% |y| < 2.4, p_{T} > 0 GeV/c",
			       "J/#psi 0-10% |y| < 1.6, 6.5 - 30 GeV/c",
			       "#Upsilon(1S) 0-5% |y| < 2.4, p_{T} > 0 GeV/c"};

  const char* leg_aa_rhic[5] = {"",
			       "#Upsilon(3S) 0-60%, p_{T} > 0 GeV/c",
			       "#Upsilon(2S) 0-60%, p_{T} > 0 GeV/c",
			       "J/#psi 0-10%, p_{T} > 5 GeV/c",
			       "#Upsilon(1S) 0-60%, p_{T} > 0 GeV/c"};
  
  TCanvas *c = NULL;
  for(int i=1; i<2; i++)
    {
      c = new TCanvas(Form("c%s",collider[i]), Form("c%s",collider[i]), 800, 600);
      if(i==1)
	{
	  TH1F *hplot = new TH1F(Form("hplot_%d",i), ";Binding energy;R_{AA}", 100, 0, 1.2);
	  hplot->GetYaxis()->SetRangeUser(0, 0.8);
	  hplot->Draw();

	  TLegend *leg = new TLegend(0.16,0.65,0.54,0.94);
	  leg->SetHeader("Au+Au #sqrt{s_{NN}} = 200 GeV, |y| < 0.5");

	  for(int j=nPart-1; j>=0; j--)
	    {
	      if(j==1)
		{
		  TArrow *ar1 = new TArrow(temp[j],raa_rhic[j],temp[j],0);
		  ar1->SetLineColor(gcolors[j]);
		  ar1->SetAngle(40);
		  ar1->SetArrowSize(0.03);
		  ar1->SetLineWidth(2);
		  ar1->Draw();
		  TLine *line = new TLine(temp[j]-0.02, raa_rhic[j], temp[j]+0.02, raa_rhic[j]);
		  line->Draw();
		  leg->AddEntry(ar1, leg_aa_rhic[j], "L");
		}
	      else if(j!=0)
		{
		  gRaa_sys[i][j]->SetFillStyle(1001);
		  gRaa_sys[i][j]->SetFillColor(gfillcolors[j]);
		  gRaa_sys[i][j]->SetLineColor(gfillcolors[j]);
		  gRaa_sys[i][j]->SetLineWidth(2);
		  gRaa_sys[i][j]->SetMarkerSize(markersize[j]);
		  gRaa_sys[i][j]->Draw("samesE5Z");
	      
		  gRaa[i][j]->SetMarkerStyle(gmarkers_aa[j]);
		  gRaa[i][j]->SetMarkerColor(gcolors[j]);
		  gRaa[i][j]->SetLineColor(gcolors[j]);
		  gRaa[i][j]->SetMarkerSize(markersize[j]);
		  gRaa[i][j]->Draw("samesPEZ");

		  leg->AddEntry(gRaa[i][j], leg_aa_rhic[j], "PL");
		}
	      double val = raa_rhic[j];
	      TPaveText *t1;
	      if(j==1) t1 = new TPaveText(temp[j]-0.02, val+0.02, temp[j]+0.02, val+0.04);
	      else     t1 = new TPaveText(temp[j]-0.02, val+0.09, temp[j]+0.02, val+0.12);
	      t1->SetFillStyle(0);
	      t1->SetBorderSize(0);
	      t1->SetTextSize(0.03);
	      t1->AddText(partName[j]);
	      t1->SetTextFont(42);
	      t1->Draw();
	    }
	  leg->Draw();
	}
      if(savePlot)
	{
	  c->SaveAs("figs/RHIC_Quarkonia_RAAvsBinding.pdf");
	}
    }
}
