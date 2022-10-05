#define singleAnalysisArray_cxx
#include "singleAnalysisArray.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <TH1F.h>
#include <TProfile.h>
#include <vector>

#include <iostream>

void singleAnalysisArray::LoadPedestals(TString pedestalFile)
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

//mapping is dependent on the cabling configuration!!!
int arrayId(int chId)
{
  int chipId=chId/64;
  if (chipId==2)
    return 0;
  if (chipId==4)
    return 1;
  if (chipId==5)
    return 1;
  if (chipId==10)
    return 2;
  if (chipId==12)
    return 3;

  return -1;
}

void singleAnalysisArray::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L singleAnalysisArray.C
//      Root > singleAnalysisArray t
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

  // rely on pedestals file to discover which channels are enabled
  if (!pedMean)
    {
      std::cout << "Need a pedestal file to be loaded. Giving up" << std::endl;
      return;
    }

  int nchannels=0;
  for (int i=1;i<pedMean->GetNbinsX()+1;i=i+4) //just look for TACid==0
    {
      if (pedMean->GetBinContent(i)>0)
	channels[nchannels++]=(i-1)/4;
    }


  std::vector<TObject*> objectsToStore;
  // TH1F* h1_energy_ref = new TH1F("h1_energy_ref", "", 250, 0, 250);
  // objectsToStore.push_back(h1_energy_ref);

  //store the channelsMap
  TH1F* channelsMap = new TH1F("channelsMap", "", nchannels, -0.5, nchannels-0.5);
  std::map<int,int> chMap;
  for (int ch=0;ch<nchannels;++ch)
    {
      chMap[channels[ch]]=ch;
      channelsMap->SetBinContent(ch+1,channels[ch]);
    }

  TH1F* h1_temp_ref = new TH1F("h1_temp_ref", "", 1000, -50, 50);
  objectsToStore.push_back(h1_temp_ref);
  TProfile* h1_temp_ref_VsTime = new TProfile("h1_temp_ref_VsTime", "", 1200, 0, 360);
  objectsToStore.push_back(h1_temp_ref_VsTime);
  TH1F* h1_temp_chip_ref = new TH1F("h1_temp_chip_ref", "", 1000, -50, 50);
  objectsToStore.push_back(h1_temp_chip_ref);
  TProfile* h1_temp_chip_ref_VsTime = new TProfile("h1_temp_chip_ref_VsTime", "", 1200, 0, 360);
  objectsToStore.push_back(h1_temp_chip_ref_VsTime);

  TH1F* h1_temp_array[N_ARRAYS];
  TProfile* h1_temp_array_VsTime[N_ARRAYS];
  TH1F* h1_temp_chip_array[N_ARRAYS];
  TProfile* h1_temp_chip_array_VsTime[N_ARRAYS];

  for (int i_array=0;i_array<N_ARRAYS;++i_array)
    {
      h1_temp_array[i_array] =  new TH1F(Form("h1_temp_array%d",i_array+1), "", 1000, -50, 50);
      objectsToStore.push_back(h1_temp_array[i_array]);  
      h1_temp_array_VsTime[i_array] =  new TProfile(Form("h1_temp_array%d_VsTime",i_array+1), "", 1200, 0, 360);
      objectsToStore.push_back(h1_temp_array_VsTime[i_array]);
      h1_temp_chip_array[i_array] =  new TH1F(Form("h1_temp_chip_array%d",i_array+1), "", 1000, -50, 50);
      objectsToStore.push_back(h1_temp_chip_array[i_array]);
      h1_temp_chip_array_VsTime[i_array] =  new TProfile(Form("h1_temp_chip_array%d_VsTime",i_array+1), "", 1200, 0, 360);
      objectsToStore.push_back(h1_temp_chip_array_VsTime[i_array]);
    }

  TProfile* h1_pedVsTime[nchannels*4];
  TProfile* h1_pedVsTemp[nchannels*4];
  for (int ch=0; ch<nchannels; ++ch)
    for (int tac=0; tac<4; ++tac)
      {
	h1_pedVsTime[ch*4+tac] = new TProfile(Form("h1_pedVsTime_ch%d_tac%d",ch,tac), Form("h1_pedVsTime_ch%d_tac%d",ch,tac), 100,0,3900);
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
      int i_array=arrayId(channelID);
      if (i_array>=0)
	{
	  h1_temp_array[i_array]->Fill(tempHoldArray[i_array]);
	  h1_temp_chip_array[i_array]->Fill(tempChipArray[i_array]);
	  h1_temp_array_VsTime[i_array]->Fill(time/1E12,tempHoldArray[i_array]);
	  h1_temp_chip_array_VsTime[i_array]->Fill(time/1E12,tempChipArray[i_array]);
	}
      else
	{
	  //assuming reference channels
	  h1_temp_ref->Fill(tempHoldBar);
	  h1_temp_chip_ref->Fill(tempChipBar);
	  h1_temp_ref_VsTime->Fill(time/1E12,tempHoldBar);
	  h1_temp_chip_ref_VsTime->Fill(time/1E12,tempChipBar);
	}

      if (step1!=1)
	continue;

      //Pedestal VsTime and VsTemp
      float ped=pedValue->GetBinContent(channelID*4+tacID+1)+pedSlope->GetBinContent(channelID*4+tacID+1)*(tot/1000-305)/5.;
      float en=energy-ped;

      h1_pedVsTime[chMap[channelID]*4+tacID]->Fill(time/1E12,en);
      if (chMap[channelID]<2) //ref channels (works only if first 2 channels!)
	h1_pedVsTemp[chMap[channelID]*4+tacID]->Fill(tempChipBar,energy-pedSlope->GetBinContent(channelID*4+tacID+1)*(tot/1000-305)/5);
      else if (i_array>=0)
	h1_pedVsTemp[chMap[channelID]*4+tacID]->Fill(tempChipArray[i_array],energy-pedSlope->GetBinContent(channelID*4+tacID+1)*(tot/1000-305)/5);
   }

   TFile *fOut=new TFile(outputFile,"RECREATE");
   fOut->cd();

   for ( auto &obj : objectsToStore)
     if (obj)
       obj->Write();
   // fOut->Write();
   fOut->ls();
   fOut->Close();
}
