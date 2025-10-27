#include "style.C"
#include "util.C"

//=====================
void Lc_over_D0(bool save_plot = true)
{
  string plotName = "Lc_over_D0";

  string axisTitles = ";p_{T} (GeV/c);#Lambda_{c}^{+}/D^{0}";

  // ===== Set the style =====
  gROOT->ProcessLine("set_style()");

  // ===== Graph configuration =====
  vector<plot_info> plot_infos;

  //STAR AuAu PRL 124 (2020) 172301
  vector<data_point> star_AuAu_data;
  star_AuAu_data.push_back({2.92, 2.5, 3.5, 1.1903, 0.2059, 0.3366, 0.3366});
  star_AuAu_data.push_back({4.07, 3.5, 5, 0.7995, 0.1031, 0.1870, 0.1870});
  star_AuAu_data.push_back({5.94, 5, 8, 0.4818, 0.0893, 0.1206, 0.1206});
  plot_infos.push_back(plot_info("STAR, AuAu 200 GeV 10-80% |y| < 1", red_color, 8, star_AuAu_data));

  //ALICE 7 TeV pp JHEP 04 (2018) 108, 2018
  vector<data_point> ALICE_pp_7_data;
  ALICE_pp_7_data.push_back({1.5, 1, 2, 0.568, 0.118, 0.193, 0.193});
  ALICE_pp_7_data.push_back({2.5, 2, 3, 0.546, 0.075, 0.066, 0.066});
  ALICE_pp_7_data.push_back({3.5, 3, 4, 0.532, 0.066, 0.062, 0.062});
  ALICE_pp_7_data.push_back({5, 4, 6, 0.464, 0.048, 0.053, 0.052});
  ALICE_pp_7_data.push_back({7, 6, 8, 0.386, 0.076, 0.056, 0.056});
  plot_infos.push_back(plot_info("ALICE, pp 7 TeV |y| < 0.5", green_color, 22, ALICE_pp_7_data));

  //ALICE 13 TeV pp JHEP 12 (2023) 086
  vector<data_point> ALICE_pp_13_data;
  ALICE_pp_13_data.push_back({0.5, 0, 1, 0.472, 0.086, 0.058, 0.058});
  ALICE_pp_13_data.push_back({1.5, 1, 2, 0.438, 0.049, 0.044, 0.043});
  ALICE_pp_13_data.push_back({2.5, 2, 3, 0.459, 0.03, 0.043, 0.043});
  ALICE_pp_13_data.push_back({3.5, 3, 4, 0.434, 0.025, 0.038, 0.038});
  ALICE_pp_13_data.push_back({4.5, 4, 5, 0.385, 0.023, 0.029, 0.029});
  ALICE_pp_13_data.push_back({5.5, 5, 6, 0.413, 0.024, 0.031, 0.032});
  ALICE_pp_13_data.push_back({6.5, 6, 7, 0.321, 0.024, 0.025, 0.025});
  ALICE_pp_13_data.push_back({7.5, 7, 8, 0.314, 0.026, 0.027, 0.027});
  ALICE_pp_13_data.push_back({9, 8, 10, 0.266, 0.021, 0.023, 0.023});
  plot_infos.push_back(plot_info("ALICE, pp 13 TeV |y| < 0.5", blue_color, 21, ALICE_pp_13_data));

  // ===== Draw histograms =====
   
  TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
  canvas->cd();
  //canvas->SetLogy();

  auto multigraph = new TMultiGraph();

  TLegend *legend = new TLegend(0.35, 0.72, 0.9, 0.9);
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

  multigraph->SetTitle(axisTitles.c_str());
  multigraph->Draw("APE1S ; E1 ; 5 s=0.1");

  legend->Draw();

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
