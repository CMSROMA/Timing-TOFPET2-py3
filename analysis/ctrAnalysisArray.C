#define ctrAnalysisArray_cxx
#include "ctrAnalysisArray.h"
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

//#define TEST_1_ARRAY

void ctrAnalysisArray::LoadPedestals(TString pedestalFile)
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

void ctrAnalysisArray::LoadCalibrations(TString calibFile)
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

void ctrAnalysisArray::getBarEnergy(int ibar,float& energy1, float& energy2)
{
  int i_array=arrayId(channels[ibar+2]);
#ifdef TEST_1_ARRAY
  float barTemp=h1_temp_array_VsTime[i_array]->Interpolate((time[ibar+2])/1E12);
  float ped1=pedValue->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)+pedSlope->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)*(tot[ibar+2]/1000-305)/5.;
  float pedTime1=h1_pedVsTime[(ibar+2)*4+tacID[ibar+2]]->Interpolate(time[ibar+2]/1E12);
  energy1 = (energy[ibar+2]-ped1-pedTime1)*(1 + (barTemp-4)*0.018); 
  float pedTime2=0;
  energy2 = 0;
#else

  float barTemp=h1_temp_array_VsTime[i_array]->Interpolate((time[ibar+2]+time[ibar+2+N_BARS])/2/1E12);
  float ped1=pedValue->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)+pedSlope->GetBinContent(channels[ibar+2]*4+tacID[ibar+2]+1)*(tot[ibar+2]/1000-305)/5.;
  float pedTime1=energy[ibar+2]>-9 ? h1_pedVsTime[(ibar+2)*4+tacID[ibar+2]]->Interpolate(time[ibar+2]/1E12) : 0.;
  // float pedTime1=0;
  
  //  energy1 = (energy[ibar+2]-ped1-pedTime1)*(1 + (barTemp-4)*0.018); //temp calibration to be optimised
  float calibBar= ((calibMap[i_array] != NULL) && applyCalib && calibMap[i_array]->GetBinContent(ibar+1)>0) ? calibMap[i_array]->GetBinContent(ibar+1) : 1.;
   
  energy1 = energy[ibar+2]>-9 ? (energy[ibar+2]-ped1-pedTime1)*calibBar : 0.;
  float ped2=pedValue->GetBinContent(channels[ibar+2+N_BARS]*4+tacID[ibar+2+N_BARS]+1)+pedSlope->GetBinContent(channels[ibar+2+N_BARS]*4+tacID[ibar+2+N_BARS]+1)*(tot[ibar+2+N_BARS]/1000-305)/5.;
  //  float pedTime2=h1_pedVsTime[(ibar+2+N_BARS)*4+tacID[ibar+2+N_BARS]]->Interpolate(time[ibar+2+N_BARS]/1E12);
  float pedTime2=(energy[ibar+2+N_BARS]>-9) ? h1_pedVsTime[(ibar+2+N_BARS)*4+tacID[ibar+2+N_BARS]]->Interpolate(time[ibar+2+N_BARS]/1E12) : 0. ;

  //  energy2 = (energy[ibar+2+N_BARS]-ped2-pedTime2)*(1 + (barTemp-4)*0.018);
  energy2 = energy[ibar+2+N_BARS]>-9 ? (energy[ibar+2+N_BARS]-ped2-pedTime2)*calibBar : 0.;
#endif
}

void ctrAnalysisArray::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L ctrAnalysisArray.C
//      Root > ctrAnalysisArray t
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
  h1_temp_ref_VsTime=(TProfile*) fIn->Get("h1_temp_ref_VsTime");
  
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

  std::vector<TObject*> objectsToStore;
  
  //time resolution
  TH1F* h1_deltaT12[N_BARS];
  TH1F* h1_CTR[N_BARS];
  TH1F* h1_deltaT12_barRef[N_BARS];

  TH1F* h1_tDiff[N_BARS];
  TH2F* h2_tDiffVsBarEnergy[N_BARS];
  TH2F* h2_tDiffVsBarEnergyRatio[N_BARS];
  TH2F* h2_tDiffVsTime[N_BARS];
  TH1F* h1_tDiffRef[N_BARS];
  TH2F* h2_tDiffRefVsRefEnergy[N_BARS];
  TH2F* h2_tDiffRefVsRefEnergyRatio[N_BARS];
  TH2F* h2_tDiffRefVsTime[N_BARS];

  //xtalk
  TH1F* h1_energy1Left_Xtalk[N_BARS];
  TH1F* h1_energy1Right_Xtalk[N_BARS];
  TH1F* h1_energy2Left_Xtalk[N_BARS];
  TH1F* h1_energy2Right_Xtalk[N_BARS];
  TH1F* h1_energyLeft_Xtalk[N_BARS];
  TH2F* h2_energyLeft_vs_energyBar_Xtalk[N_BARS];
  TH1F* h1_energyRight_Xtalk[N_BARS];
  TH2F* h2_energyRight_vs_energyBar_Xtalk[N_BARS];
  TH1F* h1_nhits_Xtalk[N_BARS];
  TH1F* h1_nhitsLeft_Xtalk[N_BARS];
  TH1F* h1_nhitsRight_Xtalk[N_BARS];
  TH1F* h1_nbars_Xtalk[N_BARS];
  TH2F* h2_nbars_vs_nhits_Xtalk[N_BARS];
  TH1F* h1_energySum_Xtalk[N_BARS];
  TH2F* h2_energySum_vs_energyBar_Xtalk[N_BARS];

  for (int ibar=0;ibar<16;++ibar)
    {
      //time resolution
      //time resolution
      h1_deltaT12[ibar] = new TH1F(Form("h1_deltaT12_bar%d",ibar), "", 1000, -5000, 5000);
      objectsToStore.push_back(h1_deltaT12[ibar]);
      h1_CTR[ibar]  = new TH1F(Form("h1_CTR_bar%d",ibar), "", 800, -10000, 10000);
      objectsToStore.push_back(h1_CTR[ibar]);
      h1_deltaT12_barRef[ibar] = new TH1F(Form("h1_deltaT12_barRef_coincBar%d",ibar), "", 1000, -5000, 5000);
      objectsToStore.push_back(h1_deltaT12_barRef[ibar]);
      h1_tDiff[ibar] = new TH1F(Form("h1_tDiff_bar%d",ibar),  "", 1000, -5000, 5000);
      objectsToStore.push_back(h1_tDiff[ibar]);
      h2_tDiffVsBarEnergy[ibar] = new TH2F(Form("h2_tDiffVsBarEnergy_bar%d",ibar),  "", 10,50,600,100, -1000, 1000);
      objectsToStore.push_back(h2_tDiffVsBarEnergy[ibar]);      
      h2_tDiffVsBarEnergyRatio[ibar] = new TH2F(Form("h2_tDiffVsBarEnergyRatio_bar%d",ibar),  "", 100,0,2,100, -1000, 1000);
      objectsToStore.push_back(h2_tDiffVsBarEnergyRatio[ibar]);
      h2_tDiffVsTime[ibar] = new TH2F(Form("h2_tDiffVsTime_bar%d",ibar),  "", 100,0,300,100, -1000, 1000);
      objectsToStore.push_back(h2_tDiffVsTime[ibar]);
      h1_tDiffRef[ibar] = new TH1F(Form("h1_tDiffRef_bar%d",ibar),  "", 1000, -5000, 5000);
      objectsToStore.push_back(h1_tDiffRef[ibar]);
      h2_tDiffRefVsRefEnergy[ibar] = new TH2F(Form("h2_tDiffRefVsRefEnergy_bar%d",ibar),  "", 10,50,600,100, -1000, 1000);
      objectsToStore.push_back(h2_tDiffRefVsRefEnergy[ibar]);
      h2_tDiffRefVsRefEnergyRatio[ibar] = new TH2F(Form("h2_tDiffRefVsRefEnergyRatio_bar%d",ibar),  "", 10,0,2,100, -1000, 1000);
      objectsToStore.push_back(h2_tDiffRefVsRefEnergyRatio[ibar]);
      h2_tDiffRefVsTime[ibar] = new TH2F(Form("h2_tDiffRefVsTime_bar%d",ibar),  "", 100,0,300,100, -1000, 1000);
      objectsToStore.push_back(h2_tDiffRefVsTime[ibar]);
      
      //xtalk
      h1_energy1Left_Xtalk[ibar] = new TH1F(Form("h1_energy1Left_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energy1Left_Xtalk[ibar]);
      h1_energy1Right_Xtalk[ibar] = new TH1F(Form("h1_energy1Right_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energy1Right_Xtalk[ibar]);
      h1_energy2Left_Xtalk[ibar] = new TH1F(Form("h1_energy2Left_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energy2Left_Xtalk[ibar]);
      h1_energy2Right_Xtalk[ibar] = new TH1F(Form("h1_energy2Right_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energy2Right_Xtalk[ibar]);
      h1_energyLeft_Xtalk[ibar] = new TH1F(Form("h1_energyLeft_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energyLeft_Xtalk[ibar]);
      h2_energyLeft_vs_energyBar_Xtalk[ibar] = new TH2F(Form("h2_energyLeft_vs_energyBar_bar%d_Xtalk",ibar), "", 250, -20, 230, 250, -20, 230);
      objectsToStore.push_back(h2_energyLeft_vs_energyBar_Xtalk[ibar]);
      h1_energyRight_Xtalk[ibar] = new TH1F(Form("h1_energyRight_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energyRight_Xtalk[ibar]);
      h2_energyRight_vs_energyBar_Xtalk[ibar] = new TH2F(Form("h2_energyRight_vs_energyBar_bar%d_Xtalk",ibar), "", 250, -20, 230, 250, -20, 230);
      objectsToStore.push_back(h2_energyRight_vs_energyBar_Xtalk[ibar]);
      h1_nhits_Xtalk[ibar] = new TH1F(Form("h1_nhits_bar%d_Xtalk",ibar), "", 8, 0, 8);
      objectsToStore.push_back(h1_nhits_Xtalk[ibar]);
      h1_nhitsLeft_Xtalk[ibar] = new TH1F(Form("h1_nhitsLeft_bar%d_Xtalk",ibar), "", 8, 0, 8);
      objectsToStore.push_back(h1_nhitsLeft_Xtalk[ibar]);
      h1_nhitsRight_Xtalk[ibar] = new TH1F(Form("h1_nhitsRight_bar%d_Xtalk",ibar), "", 8, 0, 8);
      objectsToStore.push_back(h1_nhitsRight_Xtalk[ibar]);
      h1_nbars_Xtalk[ibar] = new TH1F(Form("h1_nbars_bar%d_Xtalk",ibar), "", 4, 0, 4);
      objectsToStore.push_back(h1_nbars_Xtalk[ibar]);
      h2_nbars_vs_nhits_Xtalk[ibar] = new TH2F(Form("h2_nbars_vs_nhits_bar%d_Xtalk",ibar), "", 8, 0, 8, 4, 0, 4);
      objectsToStore.push_back(h2_nbars_vs_nhits_Xtalk[ibar]);
      h1_energySum_Xtalk[ibar] = new TH1F(Form("h1_energySum_bar%d_Xtalk",ibar), "", 70, -20, 50);
      objectsToStore.push_back(h1_energySum_Xtalk[ibar]);
      h2_energySum_vs_energyBar_Xtalk[ibar] = new TH2F(Form("h2_energySum_vs_energyBar_bar%d_Xtalk",ibar), "", 250, -20, 230, 250, -20, 230);
      objectsToStore.push_back(h2_energySum_vs_energyBar_Xtalk[ibar]);
    }

  Long64_t nentries = fChain->GetEntriesFast();
  //Long64_t nentries = 2000000;
  
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
    float calibEnergyRef = 0;
    double timeRef = 0;
    double tDiffRef = 0;

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
	    
	if (energyRef1 < 10. || energyRef2 < 10. )
	  continue;
	
	calibEnergyRef= energyRef/ref_511Peak_mean*511;
	timeRef = (time[0]+time[1])/2.;
	tDiffRef = (time[0]-time[1])/2.;

      }

    for (int ibar=0;ibar<16;++ibar)
      {

	if (alignedBar_511Peak_mean[ibar]==-9. || alignedBar_511Peak_sigma[ibar]==-9.)
	  continue;
	
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
	getBarEnergy(ibar,energy1,energy2);

	float energyBar =  energy1 + energy2;	
	float calibEnergyBar =  energyBar/alignedBar_511Peak_mean[ibar]*511.;	

#ifdef TEST_1_ARRAY
	double timeBar = time[ibar+2];
	double tDiff = 0;
#else
	double timeBar = (time[ibar+2]+time[ibar+2+N_BARS])/2.;
	double tDiff = (time[ibar+2]-time[ibar+2+N_BARS])/2.;
#endif
	//avoid back-scattering events
	if (abs(calibEnergyBar+calibEnergyRef-511)<50)
	  continue;

	/// === Time resolution ===
	float NsigmaCut = 1;
	//cout << "bar: " << ibar << " mean: " << alignedBar_511Peak_mean[ibar] << " sigma: " << alignedBar_511Peak_sigma[ibar] << endl;
	//cout << "bar: " << ibar << " energy " << energyBar << " energyRef " << energyRef << std::endl;
	if( (fabs(energyRef-ref_511Peak_mean)/ref_511Peak_sigma)<NsigmaCut
	    &&  (fabs(energyBar - alignedBar_511Peak_mean[ibar])/alignedBar_511Peak_sigma[ibar])<NsigmaCut)
	  { 
	    double deltaT = timeBar - timeRef; //clock shift to be understood
	    h1_CTR[ibar]->Fill(deltaT);

	    //CTR of bar in array using bar only
	    h1_deltaT12[ibar]->Fill(tDiff*2);
	    	    
	    //CTR of barRef using barRef only
	    h1_deltaT12_barRef[ibar]->Fill(tDiffRef);

	  }// cut for time resolution
	
	if (calibEnergyRef>50. && calibEnergyRef<600.)//no linearity corrections
	  {
	    h1_tDiff[ibar]->Fill(tDiff);
	    h2_tDiffVsBarEnergy[ibar]->Fill(calibEnergyBar,tDiff);
#ifndef TEST_1_ARRAY
	    h2_tDiffVsBarEnergyRatio[ibar]->Fill(energy1/energy2,tDiff);
#endif
	    h2_tDiffVsTime[ibar]->Fill(timeBar/1E12,tDiff);
	  }

	if (calibEnergyBar>50. && calibEnergyBar<600.)//no linearity corrections
	  {
	    h1_tDiffRef[ibar]->Fill(tDiffRef);
	    h2_tDiffRefVsRefEnergy[ibar]->Fill(calibEnergyRef,tDiffRef);
	    h2_tDiffRefVsRefEnergyRatio[ibar]->Fill(energyRef1/energyRef2,tDiffRef);
	    h2_tDiffRefVsTime[ibar]->Fill(timeRef/1E12,tDiffRef);
	  }
	
	// === Cross-talk ===
	int nhits_xtalk = 0;
	int nhitsLeft_xtalk = 0;
	int nhitsRight_xtalk = 0;
	int nbars_xtalk = 0;
	float energySum_xtalk = 0.;
	if (calibEnergyRef>100. && calibEnergyRef<600.)//no linearity corrections
	//	if( (fabs(energyRef-ref_511Peak_mean)/ref_511Peak_sigma)<NsigmaCut)
	  //	    &&  (fabs(energyBar - alignedBar_511Peak_mean[ibar])/alignedBar_511Peak_sigma[ibar])<NsigmaCut)
	  { 
	    
	    int At511Peak = 0; 
	    if(	(fabs(energyBar - alignedBar_511Peak_mean[ibar])/alignedBar_511Peak_sigma[ibar])<NsigmaCut )
	      At511Peak = 1;
	    
	    for (int idxbar=ibar-1;idxbar<ibar+2;++idxbar)
	      {
		if (idxbar<0 || idxbar>15)
		  continue; //edges
		
		if (idxbar==ibar)
		  continue; //central bar

#ifdef TEST_1_ARRAY
		if( energy[idxbar+2]==-9.)
#else
		if( energy[idxbar+2]==-9. && energy[idxbar+2+N_BARS]==-9. )
#endif
		  continue;

		nbars_xtalk++;
		
		float energy1_xtalk=0;
		float energy2_xtalk=0;
		getBarEnergy(idxbar,energy1_xtalk,energy2_xtalk);

		//energy 1
		if( energy[idxbar+2]>-9. )
		  {
		    if(idxbar == ibar-1 && At511Peak)
		      {
			h1_energy1Left_Xtalk[ibar]->Fill(energy1_xtalk);
			++nhitsLeft_xtalk;
		      }
		    if(idxbar == ibar+1 && At511Peak)
		      {
			h1_energy1Right_Xtalk[ibar]->Fill(energy1_xtalk);
			++nhitsRight_xtalk;
		      }
		      ++nhits_xtalk;
		  }


#ifndef TEST_1_ARRAY		
		//energy 2
		if( energy[idxbar+2+N_BARS]>-9. )
		  {
		    if(idxbar == ibar-1 && At511Peak)
		      {
			h1_energy2Left_Xtalk[ibar]->Fill(energy2_xtalk);
			++nhitsLeft_xtalk;
		      }
		    if(idxbar == ibar+1 && At511Peak)
		      {
			h1_energy2Right_Xtalk[ibar]->Fill(energy2_xtalk);
			++nhitsRight_xtalk;
		      }
		    ++nhits_xtalk;
		  }
#endif

#ifdef TEST_1_ARRAY		
		float energyBar_xtalk =  energy1_xtalk;
#else
		float energyBar_xtalk =  energy1_xtalk + energy2_xtalk;
#endif		
		if(idxbar == ibar-1)
		  {
		    if(At511Peak)
		      {
			h1_energyLeft_Xtalk[ibar]->Fill(energyBar_xtalk);
		      }
		    
		    h2_energyLeft_vs_energyBar_Xtalk[ibar]->Fill(energyBar,energyBar_xtalk);
		  }
		
		if(idxbar == ibar+1)
		  {		     
		    if(At511Peak)
		      {
			h1_energyRight_Xtalk[ibar]->Fill(energyBar_xtalk);
		      }           
		    h2_energyRight_vs_energyBar_Xtalk[ibar]->Fill(energyBar,energyBar_xtalk);
		  }
		energySum_xtalk += energyBar_xtalk;	
	      }
	    
	    if(At511Peak)
	      {
		h1_nhits_Xtalk[ibar]->Fill(nhits_xtalk);
		h1_nhitsLeft_Xtalk[ibar]->Fill(nhitsLeft_xtalk);           
		h1_nhitsRight_Xtalk[ibar]->Fill(nhitsRight_xtalk);           
		h1_nbars_Xtalk[ibar]->Fill(nbars_xtalk);           
		h2_nbars_vs_nhits_Xtalk[ibar]->Fill(nhits_xtalk,nbars_xtalk);
	      }	  
	    
	    if(nhits_xtalk>0)
	      {
		if(At511Peak)
		  h1_energySum_Xtalk[ibar]->Fill(energySum_xtalk);
		h2_energySum_vs_energyBar_Xtalk[ibar]->Fill(energyBar,energySum_xtalk);
	      }
	  } //cut for xtalk
	
      }//loop over bars
 
  }//loop over events

  TFile *fOut=new TFile(outputFile,"UPDATE");
  fOut->cd();
  
  for ( auto &obj : objectsToStore)
    obj->Write();
  // fOut->Write();
  fOut->ls();
  fOut->Close();
  
}
