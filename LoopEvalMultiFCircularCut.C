/*
> LoopEvalMultiFCircularCut.C 
- Tower energy plots with manual clustering
- Eta and Circular Cuts
- Combined FEMC+FHCAL
- Standalone
- Output file - energy_verification_EtaCut_CircularCut_FHCAL_FEMC.root
- Plots made - [sigma_e vs ge], [mean_e vs ge], [chi2_e vs ge], slices for the
  [(te-ge)/ge vs ge], [te/ge vs ge], [(te-ge)/ge vs ge], both with and without circular cuts.
*/

/*
  authors - Sagar Joshi      (ee190002054@iiti.ac.in)
            Siddhant Rathi   (me190003061@iiti.ac.in)
	    
  version - 1.0

*/

#include <iostream>
#include <eicqa_modules/EvalRootTTree.h>
#include <eicqa_modules/EvalHit.h>
#include "TMath.h"
#include "TStyle.h"

R__LOAD_LIBRARY(libeicqa_modules.so)

void LoopEvalMultiFCircularCut(int print = 1, int debug = 0)
{
  
  TString detector = "FHCAL_FEMC";
  TFile *f1 = new TFile("merged_Eval_FHCAL.root","READ"); 
  TFile *f2 = new TFile("merged_Eval_FEMC.root","READ"); 

  TTree* T1 = (TTree*)f1->Get("T");
  EvalRootTTree *evaltree1 = nullptr;
  
  TTree* T2 = (TTree*)f2->Get("T");
  EvalRootTTree *evaltree2 = nullptr;
    
  TCanvas *c = new TCanvas();
  c->SetTickx();
  c->SetTicky();

  // Modifying default plotting style  
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(102);
  gStyle->SetTitleXOffset(1);
  gStyle->SetTitleYOffset(1);
  gStyle->SetLabelSize(0.05);  
  gStyle->SetTitleXSize(0.05);  
  gStyle->SetTitleYSize(0.05);

  long double total_te = 0; 
  long double total_te_CircularCut = 0;
  long double total_ge = 0; 

  int nSlices = 15; // Number of ge-axis slices taken for making sigma_e vs ge plot
  Double_t x_radius_FHCAL = 0.2; //radius of circular cut in FHCAL
  Double_t y_radius_FHCAL = 0.5;
  Double_t x_radius_FEMC = 0.15; //radius of circular cut in FEMC
  Double_t y_radius_FEMC = 0.45;

  TH2D *te_minus_ge_by_ge_ge_EtaCut = new TH2D("te_minus_ge_by_ge_ge_EtaCut","#frac{#Delta e_{agg}}{truth e} vs truth e",200,0,30,200,-2,1);
  TH2D *te_minus_ge_by_ge_ge_EtaCut_CircularCut = new TH2D("te_minus_ge_by_ge_ge_EtaCut_CircularCut","#frac{#Delta e_{agg}}{truth e} vs truth e",200,0,30,200,-2,1);
  TH2D *te_minus_ge_by_ge_ge_EtaCut_temp = new TH2D("te_minus_ge_by_ge_ge_EtaCut_temp","#frac{#Delta e_{agg}}{truth e} vs truth e",nSlices,0,30,2000,-0.99,0.5);

  TH2D *te_by_ge_ge_EtaCut = new TH2D("te_by_ge_ge_EtaCut","te_{agg}/ge vs ge",200,0,30,200,-0.5,1.5);
  TH2D *te_by_ge_ge_EtaCut_CircularCut = new TH2D("te_by_ge_ge_EtaCut_CircularCut","te_{agg}/ge vs ge",200,0,30,200,-0.5,1.5);
  
  Double_t eta_min = 1.3;
  Double_t eta_max = 3.3;
  TString cut_text = " {1.3 < geta < 3.3} ";

  T1->SetBranchAddress("DST#EvalTTree_FHCAL",&evaltree1);
  T2->SetBranchAddress("DST#EvalTTree_FEMC",&evaltree2);

  for(int i=0; i<T1->GetEntries(); i++) // We assume same no. of entries, since no cuts are applied
    {
      T1->GetEntry(i);
      T2->GetEntry(i);

      if(debug==1){
	std::cout<<"\n\n\n------------------------------------------\nParticle: "<<i<<"\n\n";
	std::cout<<"Initial Parameters "<<"\n";
      }

      Double_t geta1 = evaltree1->get_geta();

      if(debug==1){
	std::cout<<"geta: "<<geta1<<"\n";
      }

      if(geta1>=eta_min && geta1<=eta_max){

	if(debug==1){    
	  cout<<"\ngeta cut applied (1.3, 3.3)"<<"\n\n";
	}
	Double_t ge = evaltree1->get_ge();
	if(debug==1){
	  std::cout<<"ge: "<<ge<<"\n";
	}

	Double_t gphi = evaltree1->get_gphi();
	if(debug==1){
	  std::cout<<"gphi: "<<gphi<<"\n";
	}

	Double_t gtheta = evaltree1->get_gtheta();
	if(debug==1){
	  std::cout<<"gtheta: "<<gtheta<<"\n";
	}
 
	total_ge += ge;
	if(debug==1){
	  std::cout<<"total_ge till now = "<<total_ge<<"\n";
	}     

	Double_t te_aggregate = 0;
	Double_t te_aggregate_CircularCut = 0;    
	for (int j=0; j<evaltree1->get_ntowers(); j++){
	
	  if(debug==1){
	    std::cout<<"\nFHCAL Tower: "<<j<<"\n";
	  }

	  EvalTower *twr1 = evaltree1->get_tower(j);
	  if (twr1){ 
	  
	    if(debug==1){
	      cout<<"non-empty FHCAL tower\n";
	    }

	    Double_t tphi = twr1->get_tphi();
	    if(debug==1){
	      std::cout<<"tphi: "<<tphi<<"\n";
	    }

	    Double_t ttheta = twr1->get_ttheta();
	    if(debug==1){
	      std::cout<<"ttheta: "<<ttheta<<"\n";
	    }

	    Double_t dphi = tphi - gphi;
	    if(debug==1){
	      std::cout<<"tphi-gphi: "<<dphi<<"\n";
	    }

	    Double_t dtheta = ttheta - gtheta;
	    if(debug==1){
	      std::cout<<"ttheta-gtheta: "<<dtheta<<"\n";
	    }

	    Double_t te = twr1->get_te();
	    if(debug==1){
	      std::cout<<"te: "<<te<<"\n";
	    }

	    te_aggregate += te;
     
	    if(debug==1){
	      std::cout<<"FHCAL: pow(dphi/y_radius,2)+pow(dtheta/x_radius,2) = "<<pow(dphi/y_radius_FHCAL,2)+pow(dtheta/x_radius_FHCAL,2)<<"\n";    
	    }

	    if (pow(dphi/y_radius_FHCAL,2)+pow(dtheta/x_radius_FHCAL,2)<=1){
	      if(debug==1){
		cout<<"FHCAL Tower included after circular cut\n";
	      }	      
	      te_aggregate_CircularCut += te;
	    }
	    if(debug==1){
	      cout<<"te_aggregate till now = "<<te_aggregate<<"\n";
	      std::cout<<"te += "<<twr1->get_te()<<"\n";
	    }
	  }
	}

	for (int j=0; j<evaltree2->get_ntowers(); j++){
	

	  if(debug==1){
	    std::cout<<"\nFEMC Tower: "<<j<<"\n";
	  }

	  EvalTower *twr2 = evaltree2->get_tower(j);
	  if (twr2){ 

	    if(debug==1){
	      cout<<"non-empty FEMC tower\n";
	    }

	    Double_t tphi = twr2->get_tphi();
	    if(debug==1){
	      std::cout<<"tphi: "<<tphi<<"\n";
	    }

	    Double_t ttheta = twr2->get_ttheta();
	    if(debug==1){
	      std::cout<<"ttheta: "<<ttheta<<"\n";
	    }

	    Double_t dphi = tphi - gphi;
	    if(debug==1){
	      std::cout<<"tphi-gphi: "<<dphi<<"\n";
	    }

	    Double_t dtheta = ttheta - gtheta;
	    if(debug==1){
	      std::cout<<"ttheta-gtheta: "<<dtheta<<"\n";
	    }

	    Double_t te = twr2->get_te();
	    if(debug==1){
	      std::cout<<"te: "<<te<<"\n";
	    }

	    te_aggregate += te;
     
	    if(debug==1){
	      std::cout<<"FEMC: pow(dphi/y_radius,2)+pow(dtheta/x_radius,2): "<<pow(dphi/y_radius_FEMC,2)+pow(dtheta/x_radius_FEMC,2)<<"\n";    
	    }

	    if (pow(dphi/y_radius_FEMC,2)+pow(dtheta/x_radius_FEMC,2)<=1){
	      if(debug==1){
		cout<<"FEMC Tower included after circular cut\n";
	      }
	      te_aggregate_CircularCut += te;
	    }

	    if(debug==1){
	      cout<<"te_aggregate till now = "<<te_aggregate<<"\n";
	      std::cout<<"te += "<<twr2->get_te()<<"\n";
	    }
	  }
	}
       
	total_te += te_aggregate;
	total_te_CircularCut += te_aggregate_CircularCut;

	if(debug==1){
	  cout<<"total_te till now = "<<total_te<<"\n";
	  cout<<"total_te_CircularCut till now = "<<total_te_CircularCut<<"\n\n";
	}

	te_minus_ge_by_ge_ge_EtaCut->Fill(ge, (te_aggregate-ge)/ge);

	if(debug==1){
	  cout<<"(ge, (te_aggregate-ge)/ge): ("<<ge<<", "<<(te_aggregate-ge)/ge<<")\n";
	}

	te_minus_ge_by_ge_ge_EtaCut_CircularCut->Fill(ge, (te_aggregate_CircularCut-ge)/ge);
	if(debug==1){
	  cout<<"(ge, (te_aggregate_CircularCut-ge)/ge): ("<<ge<<", "<<(te_aggregate_CircularCut-ge)/ge<<")\n";
	}

	te_minus_ge_by_ge_ge_EtaCut_temp->Fill(ge, (te_aggregate_CircularCut-ge)/ge);
   
	te_by_ge_ge_EtaCut->Fill(ge, te_aggregate/ge);

	if(debug==1){	
	  cout<<"(ge, te_aggregate/ge): ("<<ge<<", "<<te_aggregate/ge<<")\n";
	}

	te_by_ge_ge_EtaCut_CircularCut->Fill(ge, te_aggregate_CircularCut/ge);
	if(debug==1){
	  cout<<"(ge, te_aggregate_CircularCut/ge): ("<<ge<<", "<<te_aggregate_CircularCut/ge<<")\n";   
	}      
      }  
    }

  if(debug==1){
    cout<<"\neta cut if ends"<<"\n";
  }



  TString arr[nSlices]; // Used for naming fitted slices used in sigma_e vs ge 
  for(int sno = 0; sno < nSlices; sno++){
    arr[sno] = TString::Itoa(sno + 1, 10);
  }

  // Generating sigma_e vs ge plot
  te_minus_ge_by_ge_ge_EtaCut_temp->FitSlicesY(0, 0, -1, 0, "QN");
  TH2D *te_minus_ge_by_ge_ge_EtaCut_temp_2 = (TH2D*)gDirectory->Get("te_minus_ge_by_ge_ge_EtaCut_temp_2");
  TH2D *te_minus_ge_by_ge_ge_EtaCut_temp_1 = (TH2D*)gDirectory->Get("te_minus_ge_by_ge_ge_EtaCut_temp_1");
  TH2D *te_minus_ge_by_ge_ge_EtaCut_temp_chi2 = (TH2D*)gDirectory->Get("te_minus_ge_by_ge_ge_EtaCut_temp_chi2");

  TH1D* slices[nSlices];

  // Generating plots for individual slices
  for(int sno = 0; sno < nSlices; sno++){
    int plusOne = sno+1;
    TString sname = "slice " + arr[sno];
    slices[sno] = te_minus_ge_by_ge_ge_EtaCut_temp->ProjectionY(sname, plusOne, plusOne);
  }

  // Initialising fit functions 
  TF1 *fit = new TF1("fit", "gaus");   
  TF1 *fExp = new TF1("fExp","0.1 + 0.5/sqrt(x)",0,30);
  TF1 *fTrue = new TF1("fTrue","[0] + [1]/sqrt(x)",0,30);


  if(debug==1){
    cout<<"\nHistogram Formatting\n\n";
  }

  te_minus_ge_by_ge_ge_EtaCut->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_minus_ge_by_ge_ge_EtaCut->GetXaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut->GetXaxis()->SetTitleSize(0.05);
  te_minus_ge_by_ge_ge_EtaCut->GetYaxis()->SetTitle("(te_{agg}-ge)/ge");
  te_minus_ge_by_ge_ge_EtaCut->GetYaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut->GetYaxis()->SetTitleSize(0.05);

  te_minus_ge_by_ge_ge_EtaCut_CircularCut->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_minus_ge_by_ge_ge_EtaCut_CircularCut->GetXaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_CircularCut->GetXaxis()->SetTitleSize(0.05);
  te_minus_ge_by_ge_ge_EtaCut_CircularCut->GetYaxis()->SetTitle("(te_{agg}-ge)/ge");
  te_minus_ge_by_ge_ge_EtaCut_CircularCut->GetYaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_CircularCut->GetYaxis()->SetTitleSize(0.05);

  te_by_ge_ge_EtaCut->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_by_ge_ge_EtaCut->GetXaxis()->SetLabelSize(0.05);  
  te_by_ge_ge_EtaCut->GetXaxis()->SetTitleSize(0.05);
  te_by_ge_ge_EtaCut->GetYaxis()->SetTitle("te_{agg}/ge");
  te_by_ge_ge_EtaCut->GetYaxis()->SetLabelSize(0.05);  
  te_by_ge_ge_EtaCut->GetYaxis()->SetTitleSize(0.05);

  te_by_ge_ge_EtaCut_CircularCut->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_by_ge_ge_EtaCut_CircularCut->GetXaxis()->SetLabelSize(0.05);  
  te_by_ge_ge_EtaCut_CircularCut->GetXaxis()->SetTitleSize(0.05);
  te_by_ge_ge_EtaCut_CircularCut->GetYaxis()->SetTitle("te_{agg}/ge");
  te_by_ge_ge_EtaCut_CircularCut->GetYaxis()->SetLabelSize(0.05);  
  te_by_ge_ge_EtaCut_CircularCut->GetYaxis()->SetTitleSize(0.05);

  te_minus_ge_by_ge_ge_EtaCut_temp_2->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_minus_ge_by_ge_ge_EtaCut_temp_2->GetXaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_temp_2->GetXaxis()->SetTitleSize(0.05);
  te_minus_ge_by_ge_ge_EtaCut_temp_2->GetYaxis()->SetTitle("#sigma_{e_{agg}}");
  te_minus_ge_by_ge_ge_EtaCut_temp_2->GetYaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_temp_2->GetYaxis()->SetTitleSize(0.05);
  //te_minus_ge_by_ge_ge_EtaCut_2->SetTitle("#sigma_{e_{agg}} vs true_e" + cut_text);

  te_minus_ge_by_ge_ge_EtaCut_temp_chi2->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_minus_ge_by_ge_ge_EtaCut_temp_chi2->GetXaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_temp_chi2->GetXaxis()->SetTitleSize(0.05);
  te_minus_ge_by_ge_ge_EtaCut_temp_chi2->GetYaxis()->SetTitle("Reduced_#chi^{2}_{e_{agg}}");
  te_minus_ge_by_ge_ge_EtaCut_temp_chi2->GetYaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_temp_chi2->GetYaxis()->SetTitleSize(0.04);
  //te_minus_ge_by_ge_ge_EtaCut_chi2->SetTitle("#chi^{2}_{e_{agg}} vs true_e" + cut_text);

  te_minus_ge_by_ge_ge_EtaCut_temp_1->GetXaxis()->SetTitle("Generated Energy (GeV)");
  te_minus_ge_by_ge_ge_EtaCut_temp_1->GetXaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_temp_1->GetXaxis()->SetTitleSize(0.05);
  te_minus_ge_by_ge_ge_EtaCut_temp_1->GetYaxis()->SetTitle("Mean_{e_{agg}}");
  te_minus_ge_by_ge_ge_EtaCut_temp_1->GetYaxis()->SetLabelSize(0.05);  
  te_minus_ge_by_ge_ge_EtaCut_temp_1->GetYaxis()->SetTitleSize(0.05);
  //te_minus_ge_by_ge_ge_EtaCut_1->SetTitle("mean_{e_{agg}} vs true_e" + cut_text);
	
  for(int sno = 0; sno < nSlices; sno++){
    slices[sno]->GetXaxis()->SetTitle("#Delta e^{agg}/ ge");
    slices[sno]->GetXaxis()->SetLabelSize(0.05);  
    slices[sno]->GetXaxis()->SetTitleSize(0.05);
    slices[sno]->GetYaxis()->SetTitle("Counts");
    slices[sno]->GetYaxis()->SetLabelSize(0.05);  
    slices[sno]->GetYaxis()->SetTitleSize(0.05);
  }

  if(debug==1){
    std::cout<<"\nWrite Histograms to File\n";
  }

  TFile *f = new TFile("energy_verification_EtaCut_CircularCut" + detector + ".root","RECREATE"); 
    
  f->GetList()->Add(te_by_ge_ge_EtaCut);
  f->GetList()->Add(te_by_ge_ge_EtaCut_CircularCut);
  f->GetList()->Add(te_minus_ge_by_ge_ge_EtaCut);
  f->GetList()->Add(te_minus_ge_by_ge_ge_EtaCut_CircularCut);
  f->GetList()->Add(te_minus_ge_by_ge_ge_EtaCut_temp);
  f->GetList()->Add(te_minus_ge_by_ge_ge_EtaCut_temp_2);
  f->GetList()->Add(te_minus_ge_by_ge_ge_EtaCut_temp_1);
  f->GetList()->Add(te_minus_ge_by_ge_ge_EtaCut_temp_chi2);
  
  for(int sno = 0; sno < nSlices; sno++){
    f->GetList()->Add(slices[sno]);
  }

  f->Write();

  if(print==1){

    if(debug==1){
      std::cout<<"\nSaving Histograms as .png\n";
    }

    gStyle -> SetOptStat(0);

    te_minus_ge_by_ge_ge_EtaCut_temp_1->Draw("colz");
    c->Print(detector + "meanE_ge_EtaCut_CircularCut.png");
    te_minus_ge_by_ge_ge_EtaCut_temp_chi2->Draw("colz");
    c->Print(detector + "chi2E_ge_EtaCut_CircularCut.png");
 
    gStyle -> SetOptStat(0);
    gStyle -> SetOptFit(0);
    fExp->SetLineColor(4); //38
    fTrue->SetLineColor(2); //46
    te_minus_ge_by_ge_ge_EtaCut_temp_2->SetMarkerStyle(kFullCircle);
    te_minus_ge_by_ge_ge_EtaCut_temp_2->SetMarkerColor(46); //30
    te_minus_ge_by_ge_ge_EtaCut_temp_2->SetMarkerSize(0.75);
    te_minus_ge_by_ge_ge_EtaCut_temp_2->SetAxisRange(0,1,"Y");

    te_minus_ge_by_ge_ge_EtaCut_temp_2->Fit("fTrue", "M+");
    te_minus_ge_by_ge_ge_EtaCut_temp_2->Draw("same");
    fExp->Draw("same");
  
    TLegend* legend = new TLegend(1.75,1.75);
    legend->SetHeader("Legend", "C");
    legend->AddEntry(te_minus_ge_by_ge_ge_EtaCut_temp_2, "#sigma_{e_{agg}} vs Generated Energy", "flep");
    legend->AddEntry((TObject*)0,"","");
    legend->AddEntry(fTrue, "p_{0} + p_{1}/#sqrt{ge} (Fitted)", "l");
    legend->AddEntry((TObject*)0,"","");
    legend->AddEntry(fExp, "0.1 + 0.5/#sqrt{ge} (Requirement)", "l");
    legend->SetTextSize(0.033);
    legend->Draw();

    cout<<"reduced_chi2 of fit: "<<fTrue->GetChisquare()<<"\n";
    cout<<"parameters: "<<fTrue->GetParameters()<<"\n";

    c->Print(detector + "sigmaE_ge_EtaCut_CircularCut.png");

    gStyle -> SetOptStat(11);
    gStyle -> SetOptFit(102);
    for(int sno = 0; sno < nSlices; sno++){
      TString plusOne = (TString)(sno + 1);
      TString nameF = detector + "_sigma_slice" + arr[sno] + "_EtaCut_CircularCut.png";
      slices[sno] -> Fit("fit", "M+");
      slices[sno] -> Draw("hist same");
      c->Print(nameF);
    }

    gStyle -> SetOptStat(1);

    te_by_ge_ge_EtaCut->Draw("colz");
    c->Print(detector + "_te_by_ge_ge_EtaCut.png");
    te_by_ge_ge_EtaCut_CircularCut->Draw("colz");
    c->Print(detector + "_te_by_ge_ge_EtaCut_CircularCut.png");

    te_minus_ge_by_ge_ge_EtaCut->Draw("colz");
    c->Print(detector + "_te_minus_ge_by_ge_ge_EtaCut.png");
    te_minus_ge_by_ge_ge_EtaCut_CircularCut->Draw("colz");
    c->Print(detector + "_te_minus_ge_by_ge_ge_EtaCut_CircularCut.png");
  }

  std::cout << "The total te is: " << total_te << endl;
  std::cout << "The total te_CircularCut is: " << total_te_CircularCut << endl;
  std::cout << "The total ge is: " << total_ge << endl; 
  c->Close();
  std::cout<<"\n\nDone\n----------------------------------------------------------------------\n\n";
 
}
