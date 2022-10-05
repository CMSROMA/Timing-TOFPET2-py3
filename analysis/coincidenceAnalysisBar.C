#define coincidenceAnalysisBar_cxx
#include "coincidenceAnalysisBar.h"
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void coincidenceAnalysisBar::LoadPedestals(TString pedestalFile)
{
  TFile* f=new TFile(pedestalFile);
  f->ls();
  pedMean = (TH1F*) f->Get("h1_pedTotMean");
  pedRms = (TH1F*) f->Get("h1_pedTotRms");
  pedValue = (TH1F*) f->Get("h1_pedTotValue");
  pedSlope = (TH1F*) f->Get("h1_pedTotSlope");
  
  if (!pedMean or !pedRms or !pedValue or !pedSlope)
    std::cout << "Pedestal histograms not found in " << pedestalFile << std::endl;

  // return;
}

void coincidenceAnalysisBar::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L coincidenceAnalysisBar.C
//      Root > coincidenceAnalysisBar t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

  //channels[0]=59;
  //channels[1]=315;
  //channels[2]=291;

  cout << "channels: " << channels[0] << " , " << channels[1] << " , " << channels[2] << " , " << channels[3] << endl;

  TH1F* h1_energyTot_bar;
  TH1F* h1_energy1_bar;
  TH1F* h1_energy2_bar;
  TH1F* h1_energyDiff_bar;
  TH2F* h2_energy1VSenergy2_bar;
  TH2F* h2_energy1VSTime_bar;
  TH2F* h2_energy2VSTime_bar;
  TH2F* h2_ped1VSTime_bar;
  TH2F* h2_ped2VSTime_bar;
  TH1F* h1_energyTot_bar_coinc;
  TH1F* h1_energy1_bar_coinc;
  TH1F* h1_energy2_bar_coinc;
  TH1F* h1_energyDiff_bar_coinc;
  TH2F* h2_energy1VSenergy2_bar_coinc;
  TH1F* h1_energy_ref_coinc;
  TH1F* h1_energy_ref;
  TH2F* h2_energy1VSTime_ref;
  TH2F* h2_energy2VSTime_ref;
  TH2F* h2_ped1VSTime_ref;
  TH2F* h2_ped2VSTime_ref;
  TH2F* h2_energyRefVSenergyBar_coinc;

  std::vector<TObject*> objectsToStore;
  h1_energyTot_bar=new TH1F(Form("h1_energyTot_bar"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energyTot_bar); 
  h1_energy1_bar=new TH1F(Form("h1_energy1_bar"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy1_bar); 
  h1_energy2_bar=new TH1F(Form("h1_energy2_bar"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy2_bar); 
  h1_energyDiff_bar=new TH1F(Form("h1_energyDiff_bar"), "", 100, -50, 50);
  objectsToStore.push_back(h1_energyDiff_bar); 
  h2_energy1VSenergy2_bar=new TH2F(Form("h2_energy1VSenergy2_bar"), "", 250, 0, 250, 250, 0, 250);
  objectsToStore.push_back(h2_energy1VSenergy2_bar); 
  h1_energyTot_bar_coinc=new TH1F(Form("h1_energyTot_bar_coinc"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energyTot_bar_coinc); 
  h1_energy1_bar_coinc=new TH1F(Form("h1_energy1_bar_coinc"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy1_bar_coinc); 
  h1_energy2_bar_coinc=new TH1F(Form("h1_energy2_bar_coinc"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy2_bar_coinc); 
  h1_energyDiff_bar_coinc=new TH1F(Form("h1_energyDiff_bar_coinc"), "", 100, -50, 50);
  objectsToStore.push_back(h1_energyDiff_bar_coinc); 
  h1_energy_ref=new TH1F(Form("h1_energy_ref"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy_ref); 
  h1_energy_ref_coinc=new TH1F(Form("h1_energy_ref_coinc"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy_ref_coinc); 
  h2_energy1VSenergy2_bar_coinc=new TH2F(Form("h2_energy1VSenergy2_bar_coinc"), "", 250, 0, 250, 250, 0, 250);
  objectsToStore.push_back(h2_energy1VSenergy2_bar_coinc); 
  h2_energyRefVSenergyBar_coinc=new TH2F(Form("h2_energyRefVSenergyBar_coinc"), "", 250, 0, 250, 250, 0, 250);
  objectsToStore.push_back(h2_energyRefVSenergyBar_coinc); 

  h2_energy1VSTime_bar=new TH2F(Form("h2_energy1VSTime_bar"), "", 2400, 0, 3600, 250, 0, 250);
  objectsToStore.push_back(h2_energy1VSTime_bar); 
  h2_energy2VSTime_bar=new TH2F(Form("h2_energy2VSTime_bar"), "", 2400, 0, 3600, 250, 0, 250);
  objectsToStore.push_back(h2_energy2VSTime_bar); 

  h2_energy1VSTime_ref=new TH2F(Form("h2_energy1VSTime_ref"), "", 2400, 0, 3600, 250, 0, 250);
  objectsToStore.push_back(h2_energy1VSTime_ref); 
  h2_energy2VSTime_ref=new TH2F(Form("h2_energy2VSTime_ref"), "", 2400, 0, 3600, 250, 0, 250);
  objectsToStore.push_back(h2_energy2VSTime_ref); 

  h2_ped1VSTime_bar=new TH2F(Form("h2_ped1VSTime_bar"), "", 2400, 0, 3600, 100, -10, 10);
  objectsToStore.push_back(h2_ped1VSTime_bar); 
  h2_ped2VSTime_bar=new TH2F(Form("h2_ped2VSTime_bar"), "", 2400, 0, 3600, 100, -10, 10);
  objectsToStore.push_back(h2_ped2VSTime_bar); 

  h2_ped1VSTime_ref=new TH2F(Form("h2_ped1VSTime_ref"), "", 2400, 0, 3600, 100, -10, 10);
  objectsToStore.push_back(h2_ped1VSTime_ref); 
  h2_ped2VSTime_ref=new TH2F(Form("h2_ped2VSTime_ref"), "", 2400, 0, 3600, 100, -10, 10);
  objectsToStore.push_back(h2_ped2VSTime_ref); 

  TFile *fIn=new TFile(outputFile);
  TProfile* h1_pedVsTime[4*4];
  for (int ch=0; ch<4; ++ch)
    for (int tac=0; tac<4; ++tac)
      {
	h1_pedVsTime[ch*4+tac] = (TProfile*) fIn->Get(Form("h1_pedVsTime_ch%d_tac%d",ch,tac));
	//h1_pedVsTime[ch*4+tac]->Print();
      }

  std::map<int,int> chMap;
  for (int ch=0;ch<4;++ch)
    chMap[channels[ch]]=ch;
  
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if (jentry % 100000 == 0) 
       std::cout << "Processing event " << jentry << std::endl;
     
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      bool barON=true;
      if( energy[2]==-9. || energy[3]==-9. )
	barON=false;

      bool refON=true;
      if( energy[0]==-9. || energy[1]==-9. )
	refON=false;

      float energy1 = 0;
      float energy2 = 0;
      float energyBar =  0;
      float energyRef1 = 0;
      float energyRef2 = 0;
      float energyRef =  0;

      if (barON)
	{
	  float ped1=pedValue->GetBinContent(channels[2]*4+tacID[2]+1)+pedSlope->GetBinContent(channels[2]*4+tacID[2]+1)*(tot[2]/1000-305)/5.;
	  float ped2=pedValue->GetBinContent(channels[3]*4+tacID[3]+1)+pedSlope->GetBinContent(channels[3]*4+tacID[3]+1)*(tot[3]/1000-305)/5.;
	  float pedTime1=h1_pedVsTime[2*4+tacID[2]]->Interpolate(time[2]/1E12);
	  float pedTime2=h1_pedVsTime[3*4+tacID[3]]->Interpolate(time[3]/1E12);
	  energy1 = energy[2]-ped1-pedTime1;
	  energy2 = energy[3]-ped2-pedTime2;
	  energyBar =  energy1 + energy2;
	  h1_energyTot_bar->Fill(energyBar);
	  h1_energy1_bar->Fill(energy1);
	  h1_energy2_bar->Fill(energy2);
	  h1_energyDiff_bar->Fill(energy1-energy2);
	  h2_energy1VSenergy2_bar->Fill(energy1,energy2);
	  h2_energy1VSTime_bar->Fill(time[2]/1E12,energy1);
	  h2_energy2VSTime_bar->Fill(time[3]/1E12,energy2);
	  h2_ped1VSTime_bar->Fill(time[2]/1E12,pedTime1);
	  h2_ped2VSTime_bar->Fill(time[3]/1E12,pedTime2);
	}

      if (refON)
	{
	  float ped1=pedValue->GetBinContent(channels[0]*4+tacID[0]+1)+pedSlope->GetBinContent(channels[0]*4+tacID[0]+1)*(tot[0]/1000-305)/5.;
	  float ped2=pedValue->GetBinContent(channels[1]*4+tacID[1]+1)+pedSlope->GetBinContent(channels[1]*4+tacID[1]+1)*(tot[1]/1000-305)/5.;
	  float pedTime1=h1_pedVsTime[0*4+tacID[0]]->Interpolate(time[0]/1E12);
	  float pedTime2=h1_pedVsTime[1*4+tacID[1]]->Interpolate(time[1]/1E12);

	  energyRef1 = energy[0]-ped1-pedTime1;
	  energyRef2 = energy[1]-ped2-pedTime2;

	  energyRef=energyRef1+energyRef2;
	  h1_energy_ref->Fill(energyRef);

	  h2_energy1VSTime_ref->Fill(time[0]/1E12,energyRef1);
	  h2_energy2VSTime_ref->Fill(time[1]/1E12,energyRef2);
	  h2_ped1VSTime_ref->Fill(time[2]/1E12,pedTime1);
	  h2_ped2VSTime_ref->Fill(time[3]/1E12,pedTime2);
	}

      if (! (barON && refON))
	continue;

      //coinc histos
      h1_energyTot_bar_coinc->Fill(energyBar);
      h1_energy1_bar_coinc->Fill(energy1);
      h1_energy2_bar_coinc->Fill(energy2);
      h1_energyDiff_bar_coinc->Fill(energy1-energy2);
      h1_energy_ref_coinc->Fill(energyRef);
      h2_energy1VSenergy2_bar_coinc->Fill(energy1,energy2);
      h2_energyRefVSenergyBar_coinc->Fill(energyBar,energyRef);
   }

   fIn->Close();

   TFile *fOut=new TFile(outputFile,"UPDATE");
   fOut->cd();
   
   for ( auto &obj : objectsToStore)
     obj->Write();
   // fOut->Write();
   fOut->ls();
   fOut->Close();
}
