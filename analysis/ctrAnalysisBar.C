#define ctrAnalysisBar_new_cxx
#include "ctrAnalysisBar.h"
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void ctrAnalysisBar_new::LoadPedestals(TString pedestalFile)
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

void ctrAnalysisBar_new::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L ctrAnalysisBar_new.C
//      Root > ctrAnalysisBar_new t
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
   if (fChain == 0) return;
  
   //channels[0]=59;
   //channels[1]=315;
   //channels[2]=291;
 
   cout << "channels: " << channels[0] << " , " << channels[1] << " , " << channels[2] << " , " << channels[3] << endl ;

   std::vector<TObject*> objectsToStore;
   TH1F* h1_CTR = new TH1F("h1_CTR", "", 800, -10000, 10000);
   TH1F* h1_tDiff = new TH1F("h1_tDiff", "", 1000, -5000, 5000);
   TH2F* h2_tDiffVsBarEnergy = new TH2F("h2_tDiffVsBarEnergy", "", 10,50,600,100, -1000, 1000);
   TH2F* h2_tDiffVsBarEnergyRatio = new TH2F("h2_tDiffVsBarEnergyRatio", "", 100,0,2,100, -1000, 1000);
   TH2F* h2_tDiffVsTime = new TH2F("h2_tDiffVsTime", "", 100,0,300,100, -1000, 1000);
   TH1F* h1_tDiffRef = new TH1F("h1_tDiffRef", "", 1000, -5000, 5000);
   TH2F* h2_tDiffRefVsRefEnergy = new TH2F("h2_tDiffRefVsRefEnergy", "", 10,50,600,100, -1000, 1000);
   TH2F* h2_tDiffRefVsRefEnergyRatio = new TH2F("h2_tDiffRefVsRefEnergyRatio", "", 10,0,2,100, -1000, 1000);
   TH2F* h2_tDiffRefVsTime = new TH2F("h2_tDiffRefVsTime", "", 100,0,300,100, -1000, 1000);
   objectsToStore.push_back(h1_CTR);
   objectsToStore.push_back(h1_tDiff);
   objectsToStore.push_back(h2_tDiffVsBarEnergy);
   objectsToStore.push_back(h2_tDiffVsBarEnergyRatio);
   objectsToStore.push_back(h2_tDiffVsTime);
   objectsToStore.push_back(h1_tDiffRef);
   objectsToStore.push_back(h2_tDiffRefVsRefEnergy);
   objectsToStore.push_back(h2_tDiffRefVsRefEnergyRatio);
   objectsToStore.push_back(h2_tDiffRefVsTime);


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
   
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if (jentry % 100000 == 0) 
       std::cout << "Processing event " << jentry << std::endl;

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if( energy[2]==-9. || energy[3]==-9. )
	continue;

      float ped2=pedValue->GetBinContent(channels[2]*4+tacID[2]+1)+pedSlope->GetBinContent(channels[2]*4+tacID[2]+1)*(tot[2]/1000-305)/5.;
      float ped3=pedValue->GetBinContent(channels[3]*4+tacID[3]+1)+pedSlope->GetBinContent(channels[3]*4+tacID[3]+1)*(tot[3]/1000-305)/5.;
      float pedTime2=h1_pedVsTime[2*4+tacID[2]]->Interpolate(time[2]/1E12);
      float pedTime3=h1_pedVsTime[3*4+tacID[3]]->Interpolate(time[3]/1E12);


      float energy1 = energy[2]-ped2-pedTime2;
      float energy2 = energy[3]-ped3-pedTime3;
      float energyBar =  energy1 + energy2;
      float calibEnergyBar =  energyBar/bar_511Peak_mean*511.;

      double timeBar = (time[2]+time[3])/2.;
      double tDiff = (time[2]-time[3])/2.;
      
      if (energy[0]>-9. && energy[1]>-9.)
	{
	  float ped0=pedValue->GetBinContent(channels[0]*4+tacID[0]+1)+pedSlope->GetBinContent(channels[0]*4+tacID[0]+1)*(tot[0]/1000-305)/5.;
	  float ped1=pedValue->GetBinContent(channels[1]*4+tacID[1]+1)+pedSlope->GetBinContent(channels[1]*4+tacID[1]+1)*(tot[1]/1000-305)/5.;
	  float pedTime0=h1_pedVsTime[0*4+tacID[0]]->Interpolate(time[0]/1E12);
	  float pedTime1=h1_pedVsTime[1*4+tacID[1]]->Interpolate(time[1]/1E12);

	  float energyRef1 = energy[0]-ped0-pedTime0;
	  float energyRef2 = energy[1]-ped1-pedTime1;
	  float energyRef = energyRef1+energyRef2;

	  float calibEnergyRef= energyRef/ref_511Peak_mean*511;
	  double timeRef = (time[0]+time[1])/2.;
	  double tDiffRef = (time[0]-time[1])/2.;
	  double deltaT = timeBar - timeRef;

	  ///CTR
	  float NsigmaCut = 1;
	  if( (fabs(energyRef-ref_511Peak_mean)/ref_511Peak_sigma)<NsigmaCut
              &&  (fabs(energyBar - bar_511Peak_mean)/bar_511Peak_sigma)<NsigmaCut)
	    { 
	      h1_CTR->Fill(deltaT);  
	    }

	  if (calibEnergyRef>50. && calibEnergyRef<600.)//no linearity corrections
	    {
	      h1_tDiff->Fill(tDiff);
	      h2_tDiffVsBarEnergy->Fill(calibEnergyBar,tDiff);
	      h2_tDiffVsBarEnergyRatio->Fill(energy1/energy2,tDiff);
	      h2_tDiffVsTime->Fill(timeBar/1E12,tDiff);
	    }
	  if (calibEnergyBar>50. && calibEnergyBar<600.)//no linearity corrections
	    {
	      h1_tDiffRef->Fill(tDiffRef);
	      h2_tDiffRefVsRefEnergy->Fill(calibEnergyRef,tDiffRef);
	      h2_tDiffRefVsRefEnergyRatio->Fill(energyRef1/energyRef2,tDiffRef);
	      h2_tDiffRefVsTime->Fill(timeRef/1E12,tDiffRef);
	    }
	}
   }

   h1_CTR->Print();

   fIn->Close();

   TFile *fOut=new TFile(outputFile,"UPDATE");
   fOut->cd();

   for ( auto &obj : objectsToStore)
     obj->Write();
   // fOut->Write();
   fOut->ls();
   fOut->Close();

}
