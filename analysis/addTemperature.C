#define addTemperature_cxx
#include "addTemperature.h"
#include <TH2.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include "TObjString.h"

#include <iostream>
#include <fstream>

void parseTemperatures(std::ifstream *input)
{
  TString line;

  while(!input->eof()) 
    {
      line.ReadLine(*input);
      
      if(line.BeginsWith("#")) continue;
      if(line.BeginsWith("\n")) continue;
      if(line.BeginsWith("\r")) continue;
      
      TObjArray* tokens=line.Tokenize(",");

      if (! (tokens->GetEntries()==15 || tokens->GetEntries()==7) ) continue;

      long  unixTime=((TObjString *)(tokens->At(0)))->String().Atoi();
      
      float* tempValues = new float[14]; 
      if (tokens->GetEntries()==15) //4 arrays bench
	{
	  *(tempValues+0) = ((TObjString *)(tokens->At(1)))->String().Atof();//calibration: Es. +0.16;
	  *(tempValues+1) = ((TObjString *)(tokens->At(2)))->String().Atof();
	  *(tempValues+2) = ((TObjString *)(tokens->At(3)))->String().Atof();
	  *(tempValues+3) = ((TObjString *)(tokens->At(4)))->String().Atof();
	  *(tempValues+4) = ((TObjString *)(tokens->At(5)))->String().Atof();
	  *(tempValues+5) = ((TObjString *)(tokens->At(6)))->String().Atof();
	  *(tempValues+6) = ((TObjString *)(tokens->At(7)))->String().Atof();
	  *(tempValues+7) = ((TObjString *)(tokens->At(8)))->String().Atof();
	  *(tempValues+8) = ((TObjString *)(tokens->At(9)))->String().Atof();
	  *(tempValues+9) = ((TObjString *)(tokens->At(10)))->String().Atof();
	  *(tempValues+10) = ((TObjString *)(tokens->At(11)))->String().Atof();
	  *(tempValues+11) = ((TObjString *)(tokens->At(12)))->String().Atof();
	  *(tempValues+12) = ((TObjString *)(tokens->At(13)))->String().Atof();//humidity
	  *(tempValues+13) = ((TObjString *)(tokens->At(14)))->String().Atof();//dew point
	}
      else if (tokens->GetEntries()==7) //single array bench
	{
	  *(tempValues+0) = ((TObjString *)(tokens->At(2)))->String().Atof()+0.16; 
	  *(tempValues+1) = ((TObjString *)(tokens->At(3)))->String().Atof()-0.03; 
	  *(tempValues+2) = -999.;
	  *(tempValues+3) = -999.;
	  *(tempValues+4) = ((TObjString *)(tokens->At(4)))->String().Atof()-0.88; //assign temp as if array1
	  *(tempValues+5) = ((TObjString *)(tokens->At(1)))->String().Atof()-0.75; //assign temp as if array1
	  *(tempValues+6) = -999.;
	  *(tempValues+7) = -999.;
	  *(tempValues+8) = -999.;
	  *(tempValues+9) = -999.;
	  *(tempValues+10) = ((TObjString *)(tokens->At(6)))->String().Atof()+0.09;
	  *(tempValues+11) = ((TObjString *)(tokens->At(5)))->String().Atof()+0.22;
	  *(tempValues+12) = -999.;
	  *(tempValues+13) = -999.;
	}

      temperatures[unixTime]=tempValues;
    }
  
  std::cout << "Found #" << temperatures.size() << " temperature values" << std::endl;
}

void addTemperature::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L addTemperature.C
//      Root > addTemperature t
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

   //   TString cfgFile="../config_main_array.txt";
   std::ifstream tFile(tempFile);

   if(!tFile.is_open()) {
     std::cout << "Cannot open input file " << tempFile << ".\n";
     return ;
   }

   parseTemperatures(&tFile);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   long previousTime=temperatures.begin()->first;
   std::map<long,float*>::const_iterator tempEntry=temperatures.begin();

   for (Long64_t jentry=0; jentry<nentries;jentry++)
     {
       if (jentry%10000 == 0)
	 std::cout << "Processing hit " << jentry << std::endl;

       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   nbytes += nb;
       
       unixTime = long(time/1E12) + unixtimeStart;
       

       //scan sequentially thorugh the temperature file 
       int nLoop=0;
       while ( abs(unixTime-previousTime)>20 && nLoop<2 )
       	 {
       	   tempEntry++;
       	   previousTime=tempEntry->first;

       	   if (tempEntry == temperatures.end() ) //retry from beginning
       	     {
       	       tempEntry=temperatures.begin();
       	       ++nLoop;
       	     }
       	 }

       if (nLoop==2)
       	 std::cout << "Could not find a matching time " << std::endl;

       tempChipBar = *(tempEntry->second+0);
       tempHoldBar = *(tempEntry->second+1);
       tempChipArray1 = *(tempEntry->second+2);
       tempHoldArray1 = *(tempEntry->second+3);
       tempChipArray2 = *(tempEntry->second+4);
       tempHoldArray2 = *(tempEntry->second+5);
       tempChipArray3 = *(tempEntry->second+6);
       tempHoldArray3 = *(tempEntry->second+7);
       tempChipArray4 = *(tempEntry->second+8);
       tempHoldArray4 = *(tempEntry->second+9);
       tempAir = *(tempEntry->second+10);
       tempAirRef = *(tempEntry->second+11);
       humidityRef = *(tempEntry->second+12);
       dewPointRef = *(tempEntry->second+13);

       for ( auto& branch: newBranches )
	 branch->Fill();
       // if (Cut(ientry) < 0) continue;
     }

   //   fChain->Print();
   out->cd();
   fChain->Write("",TFile::kOverwrite);   
}
