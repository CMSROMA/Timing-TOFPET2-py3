#define findCoincidences_cxx
#include "findCoincidences.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <TObjArray.h>
#include <TObjString.h>

#define MAX_EVENT_HITS 100

using namespace std ;

void findCoincidences::parseConfig(std::ifstream *input)
{

  TString line;

  while(!input->eof()) {
    line.ReadLine(*input);
    //cout << line.Data() << endl;

    if(line.BeginsWith("#")) continue;
    if(!line.BeginsWith("CH ")) continue;

    TObjArray* tokens=line.Tokenize(" ");
    //cout << tokens->GetEntries() << endl;
    //for (int xx=0; xx<tokens->GetEntries(); xx++)
    //  {
    //	cout << ((TObjString *)(tokens->At(xx)))->String() ;
    //  }
    //cout << endl;
    if (tokens->GetEntries()!=16) continue;

    int chId= ((TObjString *)(tokens->At(1)))->String().Atoi();
    int iChip = ((TObjString *)(tokens->At(5)))->String().Atoi();
    int iChannel = ((TObjString *)(tokens->At(6)))->String().Atoi();
    int channelId = 64*iChip + iChannel;

    //cout << chId << " " << iChip << " " << iChannel << " " << channelId << endl;

    chMap[channelId]=chId;
    std::cout << "TOFPET Channel:" << channelId << "->CH" << chId << std::endl;
  }

  channelMap=new TH1F("channelMap","channelMap",chMap.size(),-0.5,chMap.size()-0.5);
  for(auto& item: chMap)
    channelMap->SetBinContent(item.second+1,item.first);

  std::cout << "Found #" << chMap.size() << " channels in config" << std::endl;
}

// data format used as bridge between the high level structures and the root tree
struct treeStructData
{
  unsigned int n_channels;
  unsigned int n_coincidences;

   ULong64_t        unixTime;
   double        tempChipBar;
   double        tempHoldBar;
   double        tempChipArray1;
   double        tempHoldArray1;
   double        tempChipArray2;
   double        tempHoldArray2;
   double        tempChipArray3;
   double        tempHoldArray3;
   double        tempChipArray4;
   double        tempHoldArray4;
   double        tempAir;
   double        tempAirRef;
   double        humidityRef;
   double        dewPointRef;

  unsigned int chID[MAX_EVENT_HITS];
  unsigned int tacID[MAX_EVENT_HITS];
  double  energy[MAX_EVENT_HITS];
  double  time[MAX_EVENT_HITS];
  double  tot[MAX_EVENT_HITS];
  double  tqT[MAX_EVENT_HITS];
  double  tqE[MAX_EVENT_HITS];
};

struct hit
{
  unsigned int chID;
  unsigned int tacID;
  double energy;
  double time;
  double tot;
  double tqT;
  double tqE;
} ;

struct eventProperties
{
  unsigned int nChannels;
  unsigned int nHits;
  
  ULong64_t unixTime;

  double        tempChipBar;
  double        tempHoldBar;
  double        tempChipArray1;
  double        tempHoldArray1;
  double        tempChipArray2;
  double        tempHoldArray2;
  double        tempChipArray3;
  double        tempHoldArray3;
  double        tempChipArray4;
  double        tempHoldArray4;
  double        tempAir;
  double        tempAirRef;
  double        humidityRef;
  double        dewPointRef;

  void clear()
  {
    nChannels=0;
    nHits=0;

    unixTime=0;

    tempChipBar=0.;
    tempHoldBar=0.;
    tempChipArray1=0.;
    tempHoldArray1=0.;
    tempChipArray2=0.;
    tempHoldArray2=0.;
    tempChipArray3=0.;
    tempHoldArray3=0.;
    tempChipArray4=0.;
    tempHoldArray4=0.;
    tempAir=0.;
    tempAirRef=0.;
    humidityRef=0.;
    dewPointRef=0.;

  };
} ;


struct Event
{
  Event (TFile * outFile, TTree * outTree) :
    outFile_ (outFile), 
    outTree_ (outTree) 
  { 
    createOutBranches (outTree_, thisTreeEvent_) ;   
  }

  ~Event () { }

  eventProperties id;
  std::vector<hit> hits;

  void clear () ;
  void Fill () ;

private :
  
  TFile * outFile_ ;
  TTree * outTree_ ;
  treeStructData thisTreeEvent_ ;

  void fillTreeData (treeStructData & treeData) ;
  void createOutBranches (TTree* tree,treeStructData& treeData) ;
} ;

void Event::createOutBranches (TTree* tree,treeStructData& treeData)
{
  tree->Branch( "nch", &treeData.n_channels, "nch/I" );
  tree->Branch( "ncoinc", &treeData.n_coincidences, "ncoinc/I" );
  tree->Branch( "chId", treeData.chID, "chId[nch]/I" );
  tree->Branch( "energy", treeData.energy, "energy[nch]/D" );
  tree->Branch( "time", treeData.time, "time[nch]/D" );
  tree->Branch( "tot", treeData.tot, "tot[nch]/D" );
  tree->Branch( "tqT", treeData.tqT, "tqT[nch]/D" );
  tree->Branch( "tqE", treeData.tqE, "tqE[nch]/D" );
  tree->Branch( "tacID", treeData.tacID, "tacID[nch]/I" );

  tree->Branch( "unixTime", &treeData.unixTime, "unixTime/L" );
  tree->Branch( "tempChipBar", &treeData.tempChipBar, "tempChipBar/D" );
  tree->Branch( "tempHoldBar", &treeData.tempHoldBar, "tempHoldBar/D" );
  tree->Branch( "tempChipArray1", &treeData.tempChipArray1, "tempChipArray1/D" );
  tree->Branch( "tempHoldArray1", &treeData.tempHoldArray1, "tempHoldArray1/D" );
  tree->Branch( "tempChipArray2", &treeData.tempChipArray2, "tempChipArray2/D" );
  tree->Branch( "tempHoldArray2", &treeData.tempHoldArray2, "tempHoldArray2/D" );
  tree->Branch( "tempChipArray3", &treeData.tempChipArray3, "tempChipArray3/D" );
  tree->Branch( "tempHoldArray3", &treeData.tempHoldArray3, "tempHoldArray3/D" );
  tree->Branch( "tempChipArray4", &treeData.tempChipArray4, "tempChipArray4/D" );
  tree->Branch( "tempHoldArray4", &treeData.tempHoldArray4, "tempHoldArray4/D" );
  tree->Branch( "tempAir", &treeData.tempAir, "tempAir/D" );
  tree->Branch( "tempAirRef", &treeData.tempAirRef, "tempAirRef/D" );
  tree->Branch( "humidityRef", &treeData.humidityRef, "humidityRef/D" );
  tree->Branch( "dewPointRef", &treeData.dewPointRef, "dewPointRef/D" );

  return ;
 } 

 void Event::fillTreeData (treeStructData & treeData)
 {
   treeData.n_channels = id.nChannels ;
   treeData.n_coincidences = id.nHits ;

   treeData.unixTime = id.unixTime ;
   treeData.tempChipBar = id.tempChipBar;
   treeData.tempHoldBar = id.tempHoldBar;
   treeData.tempChipArray1 = id.tempChipArray1;
   treeData.tempHoldArray1 = id.tempHoldArray1;
   treeData.tempChipArray2 = id.tempChipArray2;
   treeData.tempHoldArray2 = id.tempHoldArray2;
   treeData.tempChipArray3 = id.tempChipArray3;
   treeData.tempHoldArray3 = id.tempHoldArray3;
   treeData.tempChipArray4 = id.tempChipArray4;
   treeData.tempHoldArray4 = id.tempHoldArray4;
   treeData.tempAir = id.tempAir;
   treeData.tempAirRef = id.tempAirRef;
   treeData.humidityRef = id.humidityRef;
   treeData.dewPointRef = id.dewPointRef;

   for (unsigned int i = 0 ;i<fmin (MAX_EVENT_HITS, chMap.size()) ;++i)
     {
       treeData.chID[i] = -9;
       treeData.tacID[i] = -9;
       treeData.energy[i] = -9;
       treeData.time[i] = -9;
       treeData.tot[i] = -9;
       treeData.tqT[i] = -9;
       treeData.tqE[i] = -9;
     }

   for (unsigned int i = 0 ;i<hits.size();++i)
     {
       treeData.chID[chMap[hits[i].chID]] = hits[i].chID;
       treeData.tacID[chMap[hits[i].chID]] = hits[i].tacID;
       treeData.energy[chMap[hits[i].chID]] = hits[i].energy;
       treeData.time[chMap[hits[i].chID]] = hits[i].time;
       treeData.tot[chMap[hits[i].chID]] = hits[i].tot;
       treeData.tqT[chMap[hits[i].chID]] = hits[i].tqT;
       treeData.tqE[chMap[hits[i].chID]] = hits[i].tqE;
     }

   return ;
 }

 void Event::Fill ()
 {
   fillTreeData (thisTreeEvent_) ;
   outTree_->Fill () ;
   return ;
 }

 void Event::clear ()
 {
   id.clear () ;
   hits.clear () ; 
   return;
 }


void findCoincidences::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L findCoincidences.C
//      Root > findCoincidences t
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

   //   TString cfgFile="../config_main_array.txt";
   std::ifstream input(configFile);

   if(!input.is_open()) {
     std::cout << "Cannot open input file " << configFile << ".\n";
     return ;
   }

   parseConfig(&input);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   Long64_t jentry=0;

   Long64_t goodEvents=0;

   TFile *outFile=new TFile(outputFile,"RECREATE");
   TTree *outTree=new TTree("data", "data");
   Event *event=new Event(outFile,outTree);

   while (jentry<nentries-1)
     {
       Long64_t tRef=0;
       Long64_t tDiff=0;
       int nHits=0;
  
       if (jentry%10000 == 0)
	 std::cout << "Processing hit " << jentry << std::endl;

       while ( abs(tDiff) < 20000 ) //50ns
	 {
	   ++jentry;
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;

	   if (nHits==0)
	     {
	       event->clear();
	       tRef = time;
	       event->id.nChannels = chMap.size();

	       event->id.unixTime = unixTime;
	       event->id.tempChipBar = tempChipBar;
	       event->id.tempHoldBar = tempHoldBar;
	       event->id.tempChipArray1 = tempChipArray1;
	       event->id.tempHoldArray1 = tempHoldArray1;
	       event->id.tempChipArray2 = tempChipArray2;
	       event->id.tempHoldArray2 = tempHoldArray2;
	       event->id.tempChipArray3 = tempChipArray3;
	       event->id.tempHoldArray3 = tempHoldArray3;
	       event->id.tempChipArray4 = tempChipArray4;
	       event->id.tempHoldArray4 = tempHoldArray4;
	       event->id.tempAir = tempAir;
	       event->id.tempAirRef = tempAirRef;
	       event->id.humidityRef = humidityRef;
	       event->id.dewPointRef = dewPointRef;
	     }
	   else
	       tDiff = time - tRef;

	   if (abs(tDiff) < 20000)
	     {
	       ++nHits;
	       event->id.nHits++;
	       hit aHit;
	       aHit.chID = channelID;
	       aHit.tacID = tacID;
	       aHit.energy = energy;
	       aHit.time = time;
	       aHit.tot = tot;
	       aHit.tqT = tqT;
	       aHit.tqE = tqE;
	       event->hits.push_back(aHit);
	     }
 	 }

       if (nHits>1)
	 {
	   goodEvents++;
	   event->Fill();
	 }
       jentry-=1; //replay the last hit for next search window
     }

   channelMap->Write();
   outTree->Write();
   outFile->Close();
   std::cout << "Processed: " << nentries << " hits\nFound " << goodEvents << " events with coincidences" << std::endl;
}
