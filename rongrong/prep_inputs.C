
void jpsi_pA(const int saveHistos);
void jpsi_pol(const int saveHistos);
void jpsi_pt(const int saveHistos);

//================================================
void prep_inputs()
{

  //jpsi_pA(0);
  //jpsi_pol(0);
  jpsi_pt(1);
}

//================================================
void jpsi_pt(const int saveHistos)
{

  const int nfiles = 9;
  const char* file_names[nfiles] = {"HEPData-ins1773662-v1-Figure_5.root", "HEPData-ins731611-v1-Table_2.root", "HEPData-ins1735184-v1-Table_2_from_Fig_6.root", "HEPData-ins1735184-v1-Table_4_from_Fig_12.root",
				    "", "", "",
				    "HEPData-ins1773662-v1-Figure_6.root", "HEPData-ins899065-v1-Figure_21.root"};
  
  const char* dir_names[nfiles] = {"Figure 5", "Table 2", "Table 2 from Fig 6", "Table 4 from Fig 12",
				   "", "", "",
				   "Figure 6", "Figure 21"};
  
  const char* histo_names[nfiles] = {"gPhenixForward510GeV", "gPhenixForward200GeV", "gStarMidMu510GeV", "gStarMidEl500GeV",
				     "gStarMidMu200GeV", "gStarMidEl200GeV", "gPhenixMid200GeV",
				     "gPhenixRap510GeV", "gPhenixRap200GeV"};

  TGraphAsymmErrors *gJpsiPt[nfiles];
  TGraphAsymmErrors *gJpsiPtSys[nfiles];

  for(int i=0; i<nfiles; i++)
    {
      if(i>3 && i<7) continue;
      TFile *fin = TFile::Open(Form("inputs/Jpsi_pt/%s", file_names[i]), "read");
      TH1F *histo = (TH1F*)fin->Get(Form("%s/Hist1D_y1", dir_names[i]));
      TH1F *hstat = (TH1F*)fin->Get(Form("%s/Hist1D_y1_e1", dir_names[i]));
      TH1F *hsys = (TH1F*)fin->Get(Form("%s/Hist1D_y1_e2", dir_names[i]));
      int npoints = histo->GetNbinsX();
      gJpsiPt[i] = new TGraphAsymmErrors(npoints);
      gJpsiPt[i]->SetName(Form("%s", histo_names[i]));

      gJpsiPtSys[i] = new TGraphAsymmErrors(npoints);
      gJpsiPtSys[i]->SetName(Form("%s_Sys", histo_names[i]));
  
      for(int ipoint=0; ipoint<npoints; ipoint++)
	{
	  int bin = ipoint + 1;
	  gJpsiPt[i]->SetPoint(ipoint, histo->GetBinCenter(bin), histo->GetBinContent(bin));
	  gJpsiPtSys[i]->SetPoint(ipoint, histo->GetBinCenter(bin), histo->GetBinContent(bin));
	  
	  if(i<4)
	    {
	      gJpsiPt[i]->SetPointError(ipoint, histo->GetBinWidth(bin)/2, histo->GetBinWidth(bin)/2, hstat->GetBinContent(bin), hstat->GetBinContent(bin));
	      gJpsiPtSys[i]->SetPointError(ipoint, 0.2, 0.2, hsys->GetBinContent(bin), hsys->GetBinContent(bin));
	    }
	  else
	    {
	      gJpsiPt[i]->SetPointError(ipoint, 0, 0, hstat->GetBinContent(bin), hstat->GetBinContent(bin));
	      gJpsiPtSys[i]->SetPointError(ipoint, histo->GetBinWidth(bin)/2, histo->GetBinWidth(bin)/2, hsys->GetBinContent(bin), hsys->GetBinContent(bin));
	    }
	}
    }

  TFile *fin1 = TFile::Open(Form("inputs/Jpsi_pt/Paper.Run15_JpsiRpA.Final.root"), "read");
  gJpsiPt[4] = (TGraphAsymmErrors*)fin1->Get("STAR_Run15_pp200_JpsiXsec_muon");
  gJpsiPtSys[4] = (TGraphAsymmErrors*)fin1->Get("STAR_Run15_pp200_JpsiXsecSys_muon");

  gJpsiPt[5] = (TGraphAsymmErrors*)fin1->Get("STAR_2012_JpsiXsec_pp");
  gJpsiPtSys[5] = (TGraphAsymmErrors*)fin1->Get("STAR_2012_JpsiXsec_pp_sys");

  gJpsiPt[6] = (TGraphAsymmErrors*)fin1->Get("PHENIX_JpsiXsec_pp_2011");
  gJpsiPtSys[6] = (TGraphAsymmErrors*)fin1->Get("PHENIX_JpsiXsec_pp_sys_2011");
  
  for(int i=4; i<7; i++)
    {
      gJpsiPt[i]->SetName(Form("%s", histo_names[i]));
      gJpsiPtSys[i]->SetName(Form("%s_Sys", histo_names[i]));

      for(int ipoint=0; ipoint<gJpsiPtSys[i]->GetN(); ipoint++)
	{
	  gJpsiPtSys[i]->SetPointEXhigh(ipoint, 0.2);
	  gJpsiPtSys[i]->SetPointEXlow(ipoint, 0.2);
	}
    }

  
  if(saveHistos)
    {
      TFile *fout = TFile::Open("inputs/Jpsi_pt_pp.root","recreate");
        for(int i=0; i<nfiles; i++)
	  {
	    gJpsiPt[i]->Write();
	    gJpsiPtSys[i]->Write();
	  }
    }
}

//================================================
void jpsi_pol(const int saveHistos)
{
  const char* frame[2] = {"Hx", "Cs"};
  const char* param[3] = {"Th", "Ph", "ThPh"};
  const char* errtype[3] = {"stat", "sys", "tot"};
  
  // STAR dielectron
  TFile *fin1 = TFile::Open("inputs/Jpsi_pol/dielectronResult.root", "read");
  TGraphAsymmErrors *gPolStarDiEl[2][3][3];
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<3; j++)
	{
	  for(int k=0; k<3; k++)
	    {
	      gPolStarDiEl[i][j][k] = (TGraphAsymmErrors*)fin1->Get(Form("gErrAsy_dielectron_%s_%s_%s", errtype[k], frame[i], param[j]));
	      gPolStarDiEl[i][j][k]->SetName(Form("STAR_pp200_dielectron_%s_%s_%s", errtype[k], frame[i], param[j]));
	    }
	}
    }

  // STAR dimuon
  TFile *fin2 = TFile::Open("inputs/Jpsi_pol/dimuonResult.root", "read");
  TGraphAsymmErrors *gPolStarDiMu[2][2][3];
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<2; j++)
	{
	  for(int k=0; k<3; k++)
	    {
	      gPolStarDiMu[i][j][k] = (TGraphAsymmErrors*)fin2->Get(Form("gErrAsy_dimuon_%s_%s_%s", errtype[k], frame[i], param[j]));
	      gPolStarDiMu[i][j][k]->SetName(Form("STAR_pp200_dimuon_%s_%s_%s", errtype[k], frame[i], param[j]));
	    }
	}
    }

  // PHENIX PRD 102 (2020) 072008
  // pp 510 GeV, mid-rap
  TFile *fin3 = TFile::Open("inputs/Jpsi_pol/HEPData-ins1798581-v1-root.root", "read");
  TGraphAsymmErrors *gPolPhenixMid[2][3][3];
  const char *frame_index[2] = {"a", "b"};
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<3; j++)
	{
	  int fig_index = j+8;
	  TH1F *histo = (TH1F*)fin3->Get(Form("Figure %d%s/Hist1D_y1", fig_index, frame_index[i]));
	  TH1F *hstat1 = (TH1F*)fin3->Get(Form("Figure %d%s/Hist1D_y1_e1plus", fig_index, frame_index[i]));
	  TH1F *hstat2 = (TH1F*)fin3->Get(Form("Figure %d%s/Hist1D_y1_e1minus", fig_index, frame_index[i]));
	  TH1F *hsys1 = (TH1F*)fin3->Get(Form("Figure %d%s/Hist1D_y1_e2plus", fig_index, frame_index[i]));
	  TH1F *hsys2 = (TH1F*)fin3->Get(Form("Figure %d%s/Hist1D_y1_e2minus", fig_index, frame_index[i]));	  
	  int npoints = histo->GetNbinsX();
	  for(int k=0; k<3; k++)
	    {
	      gPolPhenixMid[i][j][k] = new TGraphAsymmErrors(npoints);
	      gPolPhenixMid[i][j][k]->SetName(Form("PHENIX_pp510_mid_%s_%s_%s", errtype[k], frame[i], param[j]));

	      for(int ipoint=0; ipoint<npoints; ipoint++)
		{
		  int bin = ipoint + 1;
		  double x = histo->GetBinCenter(bin);
		  double xerr = histo->GetBinWidth(bin)/2;
		  double y = histo->GetBinContent(bin);
		  double yerrplus = hstat1->GetBinContent(bin);
		  double yerrminus = fabs(hstat2->GetBinContent(bin));
		  double ysysplus = hsys1->GetBinContent(bin);
		  double ysysminus = fabs(hsys2->GetBinContent(bin));

		  gPolPhenixMid[i][j][k]->SetPoint(ipoint, x, y);
		  if(k==0)
		    {
		      gPolPhenixMid[i][j][k]->SetPointError(ipoint, xerr, xerr, yerrminus, yerrplus);
		    }
		  if(k==1)
		    {
		      gPolPhenixMid[i][j][k]->SetPointError(ipoint, xerr, xerr, ysysminus, ysysplus);
		    }
		  if(k==2)
		    {
		      gPolPhenixMid[i][j][k]->SetPointError(ipoint, xerr, xerr, sqrt(yerrminus*yerrminus+ysysminus*ysysminus), sqrt(yerrplus*yerrplus+ysysplus*ysysplus));
		    }
		}
	    }
	  
	}
    }

  
  // PHENIX PRD 95 (2017) 092003
  // pp 510 GeV, for-rap
  TFile *fin4 = TFile::Open("inputs/Jpsi_pol/HEPData-ins1505176-v1-root.root", "read");
  TGraphAsymmErrors *gPolPhenixFwd[2][3][3];
  for(int i=0; i<2; i++)
    {
      for(int j=0; j<3; j++)
	{
	  int fig_index = -1;
	  if(j==0) fig_index = 6;
	  if(j==1) fig_index = 8;
	  if(j==2) fig_index = 7;
	  TH1F *histo = (TH1F*)fin4->Get(Form("Figure %d/Hist1D_y%d", fig_index, i+1));
	  TH1F *hstat = (TH1F*)fin4->Get(Form("Figure %d/Hist1D_y%d_e1", fig_index, i+1));
	  TH1F *hsys1 = (TH1F*)fin4->Get(Form("Figure %d/Hist1D_y%d_e2plus", fig_index, i+1));
	  TH1F *hsys2 = (TH1F*)fin4->Get(Form("Figure %d/Hist1D_y%d_e2minus", fig_index, i+1));	  
	  int npoints = histo->GetNbinsX();
	  for(int k=0; k<3; k++)
	    {
	      gPolPhenixFwd[i][j][k] = new TGraphAsymmErrors(npoints);
	      gPolPhenixFwd[i][j][k]->SetName(Form("PHENIX_pp510_fwd_%s_%s_%s", errtype[k], frame[i], param[j]));

	      for(int ipoint=0; ipoint<npoints; ipoint++)
		{
		  int bin = ipoint + 1;
		  double x = histo->GetBinCenter(bin);
		  double xerr = histo->GetBinWidth(bin)/2;
		  double y = histo->GetBinContent(bin);
		  double yerr = hstat->GetBinContent(bin);
		  double ysysplus = hsys1->GetBinContent(bin);
		  double ysysminus = fabs(hsys2->GetBinContent(bin));

		  gPolPhenixFwd[i][j][k]->SetPoint(ipoint, x, y);
		  if(k==0)
		    {
		      gPolPhenixFwd[i][j][k]->SetPointError(ipoint, xerr, xerr, yerr, yerr);
		    }
		  if(k==1)
		    {
		      gPolPhenixFwd[i][j][k]->SetPointError(ipoint, xerr, xerr, ysysminus, ysysplus);
		    }
		  if(k==2)
		    {
		      gPolPhenixFwd[i][j][k]->SetPointError(ipoint, xerr, xerr, sqrt(yerr*yerr+ysysminus*ysysminus), sqrt(yerr*yerr+ysysplus*ysysplus));
		    }
		}
	    }
	  
	}
    }

  if(saveHistos)
    {
      TFile *fout = TFile::Open("inputs/Jpsi_pol_pp.root","recreate");
      for(int i=0; i<2; i++)
	{
	  for(int j=0; j<3; j++)
	    {
	      for(int k=0; k<3; k++)
		{
		  gPolStarDiEl[i][j][k]->Write();
		}
	    }
	}

      for(int i=0; i<2; i++)
	{
	  for(int j=0; j<2; j++)
	    {
	      for(int k=0; k<3; k++)
		{
		  gPolStarDiMu[i][j][k]->Write();
		}
	    }
	}

      for(int i=0; i<2; i++)
	{
	  for(int j=0; j<3; j++)
	    {
	      for(int k=0; k<3; k++)
		{
		  gPolPhenixFwd[i][j][k]->Write();
		}
	    }
	}

      for(int i=0; i<2; i++)
	{
	  for(int j=0; j<3; j++)
	    {
	      for(int k=0; k<3; k++)
		{
		  gPolPhenixMid[i][j][k]->Write();
		}
	    }
	}
    }
}

//================================================
void jpsi_pA(const int saveHistos)
{
  // RpAu, RdAu, RAA
  TFile *fpA = TFile::Open("/Users/admin/Work/STAR/run15_pAu/Rootfiles/Run15.RpA.root", "read");
  TGraphErrors *gJpsiRpA = (TGraphErrors*)fpA->Get("Run15_MTD_Jpsi_RpA");
  TGraphErrors *gJpsiRpASys = (TGraphErrors*)fpA->Get("Run15_MTD_Jpsi_RpA_Sys");
  TBox *globalSys_pAu = (TBox*)fpA->Get("Run15_MTD_Jpsi_RpA_Sys_global");
  for(int i=0; i<gJpsiRpASys->GetN(); i++)
    {
      gJpsiRpASys->SetPointError(i, 0.15, gJpsiRpASys->GetEY()[i]);
    }

  TFile *fpub = TFile::Open("/Users/admin/Work/STAR/run15_pAu/Rootfiles/Paper.Run15_JpsiRpA.root", "read");
  TGraphAsymmErrors *grPhenixRdAu = (TGraphAsymmErrors*)fpub->Get("PHENIX_dAu200_JpsiRdAu");
  TGraphAsymmErrors *grPhenixRdAuSys = (TGraphAsymmErrors*)fpub->Get("PHENIX_dAu200_JpsiRdAu_sys");
  TBox *globalSys_dAu = (TBox*)fpub->Get("PHENIX_dAu200_JpsiRdAu_sys_global");
  for(int i=0; i<grPhenixRdAuSys->GetN(); i++)
    {
      grPhenixRdAuSys->SetPointEXlow(i, 0.15);
      grPhenixRdAuSys->SetPointEXhigh(i, 0.15);
    }


  TFile *fmodel = TFile::Open("/Users/admin/Work/STAR/run15_pAu/Rootfiles/models.root","read");
  const int nModel = 7;
  const char* modelName[nModel] = {"ICEM", "nCTEQ15", "CGC+ICEM", "Comover", "TAMU", "ELoss", "EPPS16"};
  TGraphAsymmErrors *gJpsiRpAModel[nModel];
  TString hName;
  for(int i=0; i<nModel; i++)
    {
      if(strcmp(modelName[i],"CGC+ICEM")==0 || strcmp(modelName[i],"ICEM")==0 || strcmp(modelName[i],"TAMU")==0) hName = Form("%s_RpA_pAu200", modelName[i]);
      else             hName = Form("%s_RpA_dAu200", modelName[i]);
      gJpsiRpAModel[i] = (TGraphAsymmErrors*)fmodel->Get(Form("%s",hName.Data()));
    }

  if(saveHistos)
    {
      TFile *fout = TFile::Open("inputs/Jpsi_pA.root","recreate");
      gJpsiRpA->Write("Run15_MTD_Jpsi_RpA");
      gJpsiRpASys->Write("Run15_MTD_Jpsi_RpA_Sys");
      globalSys_pAu->Write("Run15_MTD_Jpsi_RpA_Sys_global");
      grPhenixRdAu->Write();
      grPhenixRdAuSys->Write();
      globalSys_dAu->Write("PHENIX_dAu200_JpsiRdAu_sys_global");
      for(int i=0; i<nModel; i++)
	{
	  gJpsiRpAModel[i]->Write();
	}
    }
}
