#define singleAnalysis_cxx
#include "singleAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <TH1F.h>
#include <TProfile.h>
#include <vector>

#include <iostream>

void singleAnalysis::LoadPedestals(TString pedestalFile)
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

void singleAnalysis::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L singleAnalysis.C
//      Root > singleAnalysis t
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

  cout << "channels: " << channels[0] << " , " << channels[1] << " , " << channels[2] << " , " << channels[3] << endl;

  // refChId = 59;
  // myPedestals.pedMean[std::make_pair< int, int>(59,0)]=250;
  // myPedestals.pedMean[std::make_pair< int, int>(59,1)]=250;
  // myPedestals.pedMean[std::make_pair< int, int>(59,2)]=250;
  // myPedestals.pedMean[std::make_pair< int, int>(59,3)]=250;

  std::map<int,int> chMap;
  for (int ch=0;ch<4;++ch)
    chMap[channels[ch]]=ch;

  std::vector<TObject*> objectsToStore;
  // TH1F* h1_energy_ref = new TH1F("h1_energy_ref", "", 250, 0, 250);
  // objectsToStore.push_back(h1_energy_ref);

  TH1F* h1_temp_ref = new TH1F("h1_temp_ref", "", 1000, -50, 50);
  objectsToStore.push_back(h1_temp_ref);
  TH1F* h1_temp_bar = new TH1F("h1_temp_bar", "", 1000, -50, 50);
  objectsToStore.push_back(h1_temp_bar);

  TProfile* h1_temp_ref_VsTime = new TProfile("h1_temp_ref_VsTime", "", 1200, 0, 360);
  objectsToStore.push_back(h1_temp_ref_VsTime);
  TProfile* h1_temp_bar_VsTime = new TProfile("h1_temp_bar_VsTime", "", 1200, 0, 360);
  objectsToStore.push_back(h1_temp_bar_VsTime);

  TH1F* h1_temp_chip_ref = new TH1F("h1_temp_chip_ref", "", 1000, -50, 50);
  objectsToStore.push_back(h1_temp_chip_ref);
  TH1F* h1_temp_chip_bar = new TH1F("h1_temp_chip_bar", "", 1000, -50, 50);
  objectsToStore.push_back(h1_temp_chip_bar);

  TProfile* h1_temp_chip_ref_VsTime = new TProfile("h1_temp_chip_ref_VsTime", "", 1200, 0, 360);
  objectsToStore.push_back(h1_temp_chip_ref_VsTime);
  TProfile* h1_temp_chip_bar_VsTime = new TProfile("h1_temp_chip_bar_VsTime", "", 1200, 0, 360);
  objectsToStore.push_back(h1_temp_chip_bar_VsTime);

  TProfile* h1_pedVsTime[16];
  TProfile* h1_pedVsTemp[16];
  for (int ch=0; ch<4; ++ch)
    for (int tac=0; tac<4; ++tac)
      {
	h1_pedVsTime[ch*4+tac] = new TProfile(Form("h1_pedVsTime_ch%d_tac%d",ch,tac), Form("h1_pedVsTime_ch%d_tac%d",ch,tac), 120,0,3600);
	objectsToStore.push_back(h1_pedVsTime[ch*4+tac]);
	h1_pedVsTemp[ch*4+tac] = new TProfile(Form("h1_pedVsTemp_ch%d_tac%d",ch,tac), Form("h1_pedVsTemp_ch%d_tac%d",ch,tac), 1000,-50,50);
	objectsToStore.push_back(h1_pedVsTemp[ch*4+tac]);
      }

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if (jentry % 100000 == 0) 
       std::cout << "Processing event " << jentry << std::endl;

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // if( channelID != refChId )
      // 	continue;

      // h1_energy_ref->Fill(energy-pedMean->GetBinContent(channelID*4+tacID+1));
      h1_temp_ref->Fill(tempHoldBar);
      h1_temp_bar->Fill(tempHoldArray1);
      h1_temp_chip_ref->Fill(tempChipBar);
      h1_temp_chip_bar->Fill(tempChipArray1);

      h1_temp_ref_VsTime->Fill(time/1E12,tempHoldBar);
      h1_temp_bar_VsTime->Fill(time/1E12,tempHoldArray1);
      h1_temp_chip_ref_VsTime->Fill(time/1E12,tempChipBar);
      h1_temp_chip_bar_VsTime->Fill(time/1E12,tempChipArray1);

      if (step1!=1)
	continue;

      float ped=pedValue->GetBinContent(channelID*4+tacID+1)+pedSlope->GetBinContent(channelID*4+tacID+1)*(tot/1000-305)/5.;
      float en=energy-ped;

      h1_pedVsTime[chMap[channelID]*4+tacID]->Fill(time/1E12,en);
      if (chMap[channelID]<2)
	h1_pedVsTemp[chMap[channelID]*4+tacID]->Fill(tempChipBar,energy-pedSlope->GetBinContent(channelID*4+tacID+1)*(tot/1000-305)/5);
      else
	h1_pedVsTemp[chMap[channelID]*4+tacID]->Fill(tempChipArray1,energy-pedSlope->GetBinContent(channelID*4+tacID+1)*(tot/1000-305)/5);
   }

   TFile *fOut=new TFile(outputFile,"RECREATE");
   fOut->cd();

   for ( auto &obj : objectsToStore)
     obj->Write();
   // fOut->Write();
   fOut->ls();
   fOut->Close();
}
