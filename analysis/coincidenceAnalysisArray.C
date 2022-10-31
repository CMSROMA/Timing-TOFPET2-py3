#define coincidenceAnalysisArray_cxx
#include "coincidenceAnalysisArray.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>


void coincidenceAnalysisArray::LoadPedestals(TString pedestalFile)
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

void coincidenceAnalysisArray::LoadCalibrations(TString calibFile)
{
  TFile* f=new TFile(calibFile);
  f->ls();
  for (int i=0;i<N_ARRAYS;++i)
    {
      calibMap[i]=(TH1F*) f->Get(Form("calibFinal_IARR%d",i));
      if (!calibMap[i])
	std::cout << "Calib for IARR " << i << "not found in " << calibFile << std::endl;
    }

  // return;
}

void coincidenceAnalysisArray::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L coincidenceAnalysisArray.C
//      Root > coincidenceAnalysisArray t
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
  if (!channelMap)
    {
      std::cout << "Channels Map not found" << std::endl;
      return;
    }

  int N_CHANNELS=channelMap->GetNbinsX();
  std::cout << "[";
  for (int i=1;i<channelMap->GetNbinsX()+1;++i)
    {
      channels[i-1]=channelMap->GetBinContent(i);
      std::cout << channels[i-1] << ",";
    }
  std::cout << "]"<< std::endl;

  std::map<int,int> chMap;
  for (int ch=0;ch<N_CHANNELS;++ch)
    chMap[channels[ch]]=ch;

  TFile *fIn=new TFile(outputFile);
  TProfile* h1_pedVsTime[N_CHANNELS*4];
  TProfile* h1_temp_array_VsTime[N_ARRAYS];
  TProfile* h1_temp_ref_VsTime=(TProfile*) fIn->Get("h1_temp_ref_VsTime");
  
  for (int i_array=0;i_array<N_ARRAYS;++i_array)
    {
      h1_temp_array_VsTime[i_array]=(TProfile*) fIn->Get(Form("h1_temp_array%d_VsTime",i_array+1));
      h1_temp_array_VsTime[i_array]->Print();
    }

  for (int ch=0; ch<N_CHANNELS; ++ch)
    for (int tac=0; tac<4; ++tac)
      {
	h1_pedVsTime[ch*4+tac] = (TProfile*) fIn->Get(Form("h1_pedVsTime_ch%d_tac%d",ch,tac));
	//h1_pedVsTime[ch*4+tac]->Print();
      }

  TH1F* h1_energyTot_bar[N_BARS];
  TH1F* h1_energy1_bar[N_BARS];
  TH1F* h1_energy2_bar[N_BARS];
  TH1F* h1_energyDiff_bar[N_BARS];
  TH2F* h2_energy1VSenergy2_bar[N_BARS];
  TH1F* h1_energyTot_bar_coinc[N_BARS];
  TH1F* h1_energy1_bar_coinc[N_BARS];
  TH1F* h1_energy2_bar_coinc[N_BARS];
  TH1F* h1_energyDiff_bar_coinc[N_BARS];
  TH2F* h2_energy1VSenergy2_bar_coinc[N_BARS];
  TH1F* h1_energy_ref_coinc[N_BARS];
  TH2F* h2_energyRefVSenergyBar_coinc[N_BARS];
  TH2F* h2_energy1VSTime_bar[N_BARS];
  TH2F* h2_energy2VSTime_bar[N_BARS];
  TH2F* h2_energy1VSTemp_bar[N_BARS];
  TH2F* h2_energy2VSTemp_bar[N_BARS];
  TH2F* h2_ped1VSTime_bar[N_BARS];
  TH2F* h2_ped2VSTime_bar[N_BARS];
  TH1F* h1_energy_ref;
  TH2F* h2_energy1VSTime_ref;
  TH2F* h2_energy2VSTime_ref;
  TH2F* h2_energy1VSTemp_ref;
  TH2F* h2_energy2VSTemp_ref;
  TH2F* h2_ped1VSTime_ref;
  TH2F* h2_ped2VSTime_ref;

  std::vector<TObject*> objectsToStore;
  h1_energy_ref=new TH1F(Form("h1_energy_ref"), "", 250, 0, 250);
  objectsToStore.push_back(h1_energy_ref); 
  h2_energy1VSTime_ref=new TH2F(Form("h2_energy1VSTime_ref"), "", 2400, 0, 3600, 250, 0, 250);
  objectsToStore.push_back(h2_energy1VSTime_ref); 
  h2_energy2VSTime_ref=new TH2F(Form("h2_energy2VSTime_ref"), "", 2400, 0, 3600, 250, 0, 250);
  objectsToStore.push_back(h2_energy2VSTime_ref); 
  h2_energy1VSTemp_ref=new TH2F(Form("h2_energy1VSTemp_ref"), "", 100, 0, 20, 250, 0, 250);
  objectsToStore.push_back(h2_energy1VSTemp_ref); 
  h2_energy2VSTemp_ref=new TH2F(Form("h2_energy2VSTemp_ref"), "", 100, 0, 20, 250, 0, 250);
  objectsToStore.push_back(h2_energy2VSTemp_ref); 
  h2_ped1VSTime_ref=new TH2F(Form("h2_ped1VSTime_ref"), "", 2400, 0, 3600, 100, -10, 10);
  objectsToStore.push_back(h2_ped1VSTime_ref); 
  h2_ped2VSTime_ref=new TH2F(Form("h2_ped2VSTime_ref"), "", 2400, 0, 3600, 100, -10, 10);
  objectsToStore.push_back(h2_ped2VSTime_ref); 

  for (int ibar=0;ibar<N_BARS;++ibar)
    {
      h1_energyTot_bar[ibar]=new TH1F(Form("h1_energyTot_bar%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energyTot_bar[ibar]); 
      h1_energy1_bar[ibar]=new TH1F(Form("h1_energy1_bar%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energy1_bar[ibar]); 
      h1_energy2_bar[ibar]=new TH1F(Form("h1_energy2_bar%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energy2_bar[ibar]); 
      h1_energyDiff_bar[ibar]=new TH1F(Form("h1_energyDiff_bar%d",ibar), "", 100, -50, 50);
      objectsToStore.push_back(h1_energyDiff_bar[ibar]); 
      h2_energy1VSenergy2_bar[ibar]=new TH2F(Form("h2_energy1VSenergy2_bar%d",ibar), "", 200, 0, 200, 200, 0, 200);
      objectsToStore.push_back(h2_energy1VSenergy2_bar[ibar]); 
      h1_energyTot_bar_coinc[ibar]=new TH1F(Form("h1_energyTot_bar_coinc%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energyTot_bar_coinc[ibar]); 
      h1_energy1_bar_coinc[ibar]=new TH1F(Form("h1_energy1_bar_coinc%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energy1_bar_coinc[ibar]); 
      h1_energy2_bar_coinc[ibar]=new TH1F(Form("h1_energy2_bar_coinc%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energy2_bar_coinc[ibar]); 
      h1_energyDiff_bar_coinc[ibar]=new TH1F(Form("h1_energyDiff_bar_coinc%d",ibar), "", 100, -50, 50);
      objectsToStore.push_back(h1_energyDiff_bar_coinc[ibar]); 
      h1_energy_ref_coinc[ibar]=new TH1F(Form("h1_energy_ref_coinc%d",ibar), "", 200, 0, 200);
      objectsToStore.push_back(h1_energy_ref_coinc[ibar]); 
      h2_energy1VSenergy2_bar_coinc[ibar]=new TH2F(Form("h2_energy1VSenergy2_bar_coinc%d",ibar), "", 200, 0, 200, 200, 0, 200);
      objectsToStore.push_back(h2_energy1VSenergy2_bar_coinc[ibar]); 
      h2_energyRefVSenergyBar_coinc[ibar]=new TH2F(Form("h2_energyRefVSenergyBar_coinc%d",ibar), "", 200, 0, 200, 200, 0, 200);
      objectsToStore.push_back(h2_energyRefVSenergyBar_coinc[ibar]); 

      h2_energy1VSTime_bar[ibar]=new TH2F(Form("h2_energy1VSTime_bar%d",ibar), "", 2400, 0, 3600, 250, 0, 250);
      objectsToStore.push_back(h2_energy1VSTime_bar[ibar]); 
      h2_energy2VSTime_bar[ibar]=new TH2F(Form("h2_energy2VSTime_bar%d",ibar), "", 2400, 0, 3600, 250, 0, 250);
      objectsToStore.push_back(h2_energy2VSTime_bar[ibar]); 
      
      h2_energy1VSTemp_bar[ibar]=new TH2F(Form("h2_energy1VSTemp_bar%d",ibar), "", 100, 0, 20, 250, 0, 250);
      objectsToStore.push_back(h2_energy1VSTemp_bar[ibar]); 
      h2_energy2VSTemp_bar[ibar]=new TH2F(Form("h2_energy2VSTemp_bar%d",ibar), "", 100, 0, 20, 250, 0, 250);
      objectsToStore.push_back(h2_energy2VSTemp_bar[ibar]); 

      h2_ped1VSTime_bar[ibar]=new TH2F(Form("h2_ped1VSTimebar%d",ibar), "", 2400, 0, 3600, 100, -10, 10);
      objectsToStore.push_back(h2_ped1VSTime_bar[ibar]); 
      h2_ped2VSTime_bar[ibar]=new TH2F(Form("h2_ped2VSTimebar%d",ibar), "", 2400, 0, 3600, 100, -10, 10);
      objectsToStore.push_back(h2_ped2VSTime_bar[ibar]); 

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

      bool refON=true;
      if( energy[0]==-9. || energy[1]==-9. )
	refON=false;

      float energyRef1 = 0;
      float energyRef2 = 0;
      float energyRef =  0;

    if (refON)
	{
	  float ped1=pedValue->GetBinContent(channels[0]*4+tacID[0]+1)+pedSlope->GetBinContent(channels[0]*4+tacID[0]+1)*(tot[0]/1000-305)/5.;
	  float ped2=pedValue->GetBinContent(channels[1]*4+tacID[1]+1)+pedSlope->GetBinContent(channels[1]*4+tacID[1]+1)*(tot[1]/1000-305)/5.;
#ifdef TEST_1_ARRAY
	  float pedTime1=h1_pedVsTime[0*4+tacID[0]]->Interpolate(time[0]/1E12);
	  float pedTime2=h1_pedVsTime[1*4+tacID[1]]->Interpolate(time[1]/1E12);
#else
	  float pedTime1=h1_pedVsTime[0*4+tacID[0]]->Interpolate(time[0]/1E12);
	  float pedTime2=h1_pedVsTime[1*4+tacID[1]]->Interpolate(time[1]/1E12);
#endif	  
	  float refTemp=h1_temp_ref_VsTime->Interpolate((time[0]+time[1])/2/1E12);
	  
	  // energyRef1 = (energy[0]-ped1-pedTime1)*(1 + (refTemp-4)*0.033);
	  // energyRef2 = (energy[1]-ped2-pedTime2)*(1 + (refTemp-4)*0.033);
	  energyRef1 = (energy[0]-ped1-pedTime1);
	  energyRef2 = (energy[1]-ped2-pedTime2);
	  energyRef=energyRef1+energyRef2;

	  h1_energy_ref->Fill(energyRef);
	  h2_energy1VSTime_ref->Fill(time[0]/1E12,energyRef1);
	  h2_energy2VSTime_ref->Fill(time[1]/1E12,energyRef2);
	  h2_ped1VSTime_ref->Fill(time[0]/1E12,pedTime1);
	  h2_ped2VSTime_ref->Fill(time[1]/1E12,pedTime2);
	  h2_energy1VSTemp_ref->Fill(refTemp,energyRef1);
	  h2_energy2VSTemp_ref->Fill(refTemp,energyRef2);
	}

      for (int ibar=0;ibar<16;++ibar)
	{
	  bool barON=true;
#ifdef TEST_1_ARRAY
	  if( energy[ibar+2]==-9. )
#else
	  if( energy[ibar+2]==-9. || energy[ibar+2+N_BARS]==-9. )
#endif
	    barON=false;

	  if (!barON)
	    continue;

	  float energy1 = 0;
	  float energy2 = 0;
	  float energyBar =  0;
	  
	  int i_array=arrayId(channels[ibar+2]);
	  float calibBar= calibMap[i_array]&&applyCalib&&calibMap[i_array]->GetBinContent(ibar+1)>0 ? calibMap[i_array]->GetBinContent(ibar+1) : 1.;
#ifdef TEST_1_ARRAY
	  float barTemp=h1_temp_array_VsTime[i_array]->Interpolate((time[ibar+2])/1E12);
	  float ped1=pedValue->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)+pedSlope->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)*(tot[ibar+2]/1000-305)/5.;
	  float pedTime1=h1_pedVsTime[(ibar+2)*4+tacID[ibar+2]]->Interpolate(time[ibar+2]/1E12);
	  //	  energy1 = (energy[ibar+2]-ped1-pedTime1)*(1 + (barTemp-4)*0.018); 
	  energy1 = (energy[ibar+2]-ped1-pedTime1);
	  float pedTime2=0;
#else
	  float barTemp=h1_temp_array_VsTime[i_array]->Interpolate((time[ibar+2]+time[ibar+2+N_BARS])/2/1E12);
	  float ped1=pedValue->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)+pedSlope->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)*(tot[ibar+2]/1000-305)/5.;
	  float pedTime1=h1_pedVsTime[(ibar+2)*4+tacID[ibar+2]]->Interpolate(time[ibar+2]/1E12);
	  //energy1 = (energy[ibar+2]-ped1-pedTime1)*(1 + (barTemp-4)*0.018); //temp calibration to be optimised
	  energy1 = (energy[ibar+2]-ped1-pedTime1)*calibBar;

	  float ped2=pedValue->GetBinContent(channels[ibar+2+N_BARS]*4+tacID[ibar+2+N_BARS]+1)+pedSlope->GetBinContent(channels[ibar+2+N_BARS]*4+tacID[ibar+2+N_BARS]+1)*(tot[ibar+2+N_BARS]/1000-305)/5.;
	  float pedTime2=h1_pedVsTime[(ibar+2+N_BARS)*4+tacID[ibar+2+N_BARS]]->Interpolate(time[ibar+2+N_BARS]/1E12);
	  //	  energy2 = (energy[ibar+2+N_BARS]-ped2-pedTime2)*(1 + (barTemp-4)*0.018);
	  energy2 = (energy[ibar+2+N_BARS]-ped2-pedTime2)*calibBar;
#endif
	  energyBar =  energy1 + energy2;
	  h1_energyTot_bar[ibar]->Fill(energyBar);
	  h1_energy1_bar[ibar]->Fill(energy1);
	  h1_energy2_bar[ibar]->Fill(energy2);
	  h1_energyDiff_bar[ibar]->Fill(energy1-energy2);
	  h2_energy1VSenergy2_bar[ibar]->Fill(energy1,energy2);
#ifdef TEST_1_ARRAY	 
	  h2_energy1VSTime_bar[ibar]->Fill(time[ibar+2]/1E12,energy1);
	  h2_energy1VSTemp_bar[ibar]->Fill(barTemp,energy1);
	  h2_ped1VSTime_bar[ibar]->Fill(time[ibar+2]/1E12,pedTime1);
#else
	  h2_energy1VSTime_bar[ibar]->Fill(time[ibar+2]/1E12,energy1);
	  h2_energy2VSTime_bar[ibar]->Fill(time[ibar+2+N_BARS]/1E12,energy2);
	  h2_energy1VSTemp_bar[ibar]->Fill(barTemp,energy1);
	  h2_energy2VSTemp_bar[ibar]->Fill(barTemp,energy2);
	  h2_ped1VSTime_bar[ibar]->Fill(time[ibar+2]/1E12,pedTime1);
	  h2_ped2VSTime_bar[ibar]->Fill(time[ibar+2+N_BARS]/1E12,pedTime2);
#endif

	  if( !refON )
	    continue;

	  h1_energyTot_bar_coinc[ibar]->Fill(energyBar);
	  h1_energy1_bar_coinc[ibar]->Fill(energy1);
	  h1_energy2_bar_coinc[ibar]->Fill(energy2);
	  h1_energyDiff_bar_coinc[ibar]->Fill(energy1-energy2);
	  h1_energy_ref_coinc[ibar]->Fill(energyRef);
	  h2_energy1VSenergy2_bar_coinc[ibar]->Fill(energy1,energy2);
	  h2_energyRefVSenergyBar_coinc[ibar]->Fill(energyBar,energyRef);
	}
 }

   TFile *fOut=new TFile(outputFile,"UPDATE");
   fOut->cd();

   for ( auto &obj : objectsToStore)
     obj->Write();
   // fOut->Write();
   fOut->ls();
   fOut->Close();

}
