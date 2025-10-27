#include "style.C"
#include "util.C"

//=====================
void Ds_over_D0(bool save_plot = true)
{
  string plotName = "Ds_over_D0";

  string axisTitles = ";p_{T} (GeV/c);D_{s}^{#pm}/D^{0}";

  // ===== Set the style =====
  gROOT->ProcessLine("set_style()");

  // ===== Graph configuration =====
  vector<plot_info> plot_infos;

  //STAR AuAu Phys.Rev.Lett. 127 (2021) 092301
  //0 -> 10%
  vector<data_point> star_AuAu_0_to_10_data;
  star_AuAu_0_to_10_data.push_back({1.951, 1.5, 2.5, 0.2568, 0.04141, 0.0520, 0.0520});
  star_AuAu_0_to_10_data.push_back({2.935, 2.5, 3.5, 0.2778, 0.02485, 0.04798, 0.04766});
  star_AuAu_0_to_10_data.push_back({4.116, 3.5, 5.0, 0.3673, 0.02938, 0.03102, 0.03014});
  star_AuAu_0_to_10_data.push_back({6.090, 5.0, 8.0, 0.3579, 0.04895, 0.03553, 0.03387});
  plot_infos.push_back(plot_info("0-10%", red_color, 8, star_AuAu_0_to_10_data));
  //10 -> 20%
  vector<data_point> star_AuAu_10_to_20_data;
  star_AuAu_10_to_20_data.push_back({1.951, 1.5, 2.5, 0.3539, 0.04557, 0.04613, 0.04613});
  star_AuAu_10_to_20_data.push_back({2.935, 2.5, 3.5, 0.3434, 0.02837, 0.03269, 0.03103});
  star_AuAu_10_to_20_data.push_back({4.116, 3.5, 5.0, 0.4367, 0.03377, 0.05282, 0.05056});
  star_AuAu_10_to_20_data.push_back({6.090, 5.0, 8.0, 0.4012, 0.04846, 0.06293, 0.05965});
  plot_infos.push_back(plot_info("10-20%", purple_color, 22, star_AuAu_10_to_20_data));
  //20 -> 40%
  vector<data_point> star_AuAu_20_to_40_data;
  star_AuAu_20_to_40_data.push_back({1.951, 1.5, 2.5, 0.3777, 0.03192, 0.03620, 0.03620});
  star_AuAu_20_to_40_data.push_back({2.935, 2.5, 3.5, 0.3579, 0.02020, 0.02475, 0.02230});
  star_AuAu_20_to_40_data.push_back({4.116, 3.5, 5.0, 0.4749, 0.02635, 0.05516, 0.05260});
  star_AuAu_20_to_40_data.push_back({6.090, 5.0, 8.0, 0.4572, 0.04239, 0.04106, 0.03412});
  plot_infos.push_back(plot_info("20-40%", blue_color, 21, star_AuAu_20_to_40_data));
  //40 -> 80%
  vector<data_point> star_AuAu_40_to_80_data;
  star_AuAu_40_to_80_data.push_back({1.951, 1.5, 2.5, 0.4063, 0.04417, 0.05798, 0.05798});
  star_AuAu_40_to_80_data.push_back({2.935, 2.5, 3.5, 0.3940, 0.03012, 0.04854, 0.04654});
  star_AuAu_40_to_80_data.push_back({4.116, 3.5, 5.0, 0.4099, 0.03470, 0.04721, 0.04346});
  star_AuAu_40_to_80_data.push_back({6.090, 5.0, 8.0, 0.4754, 0.05824, 0.05206, 0.04356});
  plot_infos.push_back(plot_info("40-80%", green_color, 29, star_AuAu_40_to_80_data));

  // ===== Draw histograms =====
   
  TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
  canvas->cd();
  //canvas->SetLogy();

  auto multigraph = new TMultiGraph();

  TLegend *legend = new TLegend(0.55, 0.20, 0.9, 0.35);
  legend->SetBorderSize(0);
  legend->SetLineColor(1);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(text_size);

  for (unsigned int i = 0; i < plot_infos.size(); ++i)
  {
    TGraphMultiErrors* myGraph = makeGraph(plot_infos[i]);
    multigraph->Add(myGraph);
    legend->AddEntry(myGraph, plot_infos[i].legend_info.c_str(), "P");
  }

  //multigraph->GetYaxis()->SetRangeUser(0.2, 0.7);
  multigraph->SetTitle(axisTitles.c_str());
  multigraph->Draw("APE1S ; E1 ; 5 s=0.1");

  legend->Draw();

  TPaveText *pt;
  pt = new TPaveText(0.15,0.75,0.45,0.92, "NDC");
  pt->SetTextSize(text_size);
  pt->SetFillColor(0);
  pt->SetFillStyle(0);
  pt->SetTextFont(42);
  TText *pt_LaTex;
  pt_LaTex = pt->AddText("STAR Au+Au");
  pt_LaTex = pt->AddText("#sqrt{S_{NN}} = 200 GeV");
  pt->SetBorderSize(0);
  pt->Draw();
  gPad->Modified();

  if(save_plot)
  {
    string extensions[] = {".png", ".pdf"};
    for (auto extension : extensions)
    {
      string output = "plots/" + plotName + extension;
      canvas->SaveAs(output.c_str());
    }
  }
}
