#! /usr/bin/env python

import os
import sys
import optparse
import datetime
import subprocess
from glob import glob
from collections import defaultdict
from collections import OrderedDict
from array import array

from ROOT import *

gROOT.SetBatch(True)

usage = "usage: python analysis/launch_analyze_run_array_lscan.py --firstRun 1 -i /home/cmsdaq/Workspace/TOFPET/Timing-TOFPET/output/TestArray -o /home/cmsdaq/Workspace/TOFPET/Timing-TOFPET/output/TestArray/RESULTS --arrayCode 0"

parser = optparse.OptionParser(usage)

parser.add_option("-r", "--firstRun", dest="firstRun",
                  help="first run number of the position scan")

parser.add_option("-i", "--input", dest="inputDir",default="/data/TOFPET/LYSOARRAYS",
                  help="input directory")

parser.add_option("-o", "--output", dest="outputDir",default="/data/TOFPET/LYSOARRAYS/RESULTS",
                  help="output directory")

parser.add_option("-b", "--arrayCode", dest="arrayCode", default=-99,
                  help="code of the crystal array")

(opt, args) = parser.parse_args()

if not opt.firstRun:   
    parser.error('first run number not provided')

if not opt.inputDir:   
    parser.error('input directory not provided')

if not opt.outputDir:   
    parser.error('output directory not provided')

#------------------------------------------------

nFilesInScan = 15

#maybe this can be passed from command line 
pos = [ 0, 1, 2, 3, 4, 5, 6, 7, -2, -7,-6, -5, -4, -3, -1 ]


summaryFile = str(opt.outputDir)+"/"+"summary_"+"FirstRun" + str(opt.firstRun.zfill(6)) + "_LastRun" + str((int(opt.firstRun)+(nFilesInScan-1)*3)).zfill(6) + "_ARRAY" + str(opt.arrayCode.zfill(6))+".root"

#### Analysis Summary #####
histos={}

for v in [ 'LY','sigmaT','CTR','XT']:
    histos['%s_vs_pos'%v]=TGraphErrors()
    histos['%s_vs_pos'%v].SetName('%s_vs_bar'%v)

tfileResults={}
treeResults={}

lscan_step=4 #4mm step 

for step in range(0,nFilesInScan):
    tfileResults[pos[step]] = TFile.Open(opt.inputDir+"/tree_Run%s_ARRAY%s.root"%(str(int(opt.firstRun)+step*3).zfill(6),opt.arrayCode.zfill(6)))
    print opt.inputDir+"/tree_Run%s_ARRAY%s.root" % (str(int(opt.firstRun)+step*3).zfill(6),opt.arrayCode.zfill(6))
    treeResults[pos[step]] = tfileResults[pos[step]].Get("results")

    treeResults[pos[step]].GetEntry(0)

    print TMath.Mean(16,treeResults[pos[step]].peak1_mean_barCoinc)
    print TMath.Mean(16,treeResults[pos[step]].deltaT12_sigma_barCoinc)/2.

    histos['LY_vs_pos'].SetPoint(step,pos[step]*lscan_step,TMath.Mean(16,treeResults[pos[step]].peak1_mean_barCoinc))
    histos['LY_vs_pos'].SetPointError(step,0.5,TMath.RMS(16,treeResults[pos[step]].peak1_mean_barCoinc))
    histos['sigmaT_vs_pos'].SetPoint(step,pos[step]*lscan_step,TMath.Mean(16,treeResults[pos[step]].deltaT12_sigma_barCoinc)/2.)
    histos['sigmaT_vs_pos'].SetPointError(step,0.5,TMath.RMS(16,treeResults[pos[step]].deltaT12_sigma_barCoinc)/2.)
    histos['CTR_vs_pos'].SetPoint(step,pos[step]*lscan_step,TMath.Mean(16,treeResults[pos[step]].CTR_sigma_barCoinc))
    histos['CTR_vs_pos'].SetPointError(step,0.5,TMath.RMS(16,treeResults[pos[step]].CTR_sigma_barCoinc))
    histos['XT_vs_pos'].SetPoint(step,pos[step]*lscan_step,TMath.Mean(16,treeResults[pos[step]].Xtalk_median_barCoinc))
    histos['XT_vs_pos'].SetPointError(step,0.5,TMath.RMS(16,treeResults[pos[step]].Xtalk_median_barCoinc))

##########################################

mergedLabel = str(opt.outputDir)+"/"+"tree_"+"FirstRun" + str(opt.firstRun.zfill(6)) + "_LastRun" + str((int(opt.firstRun)+(nFilesInScan-1)*3)).zfill(6) + "_ARRAY" + str(opt.arrayCode.zfill(6))

c1_LY = TCanvas("phe_peak", "phe_peak", 900, 700)     

c1_LY.cd()  
gStyle.SetOptStat(1111);
c1_LY.SetGrid();
for hh in ['LY','sigmaT','CTR','XT']:
    histos['%s_vs_pos'%hh].GetXaxis().SetTitle("Y (mm)")
    if (hh=='LY'):
        histos['%s_vs_pos'%hh].GetYaxis().SetTitle("Energy (QDC)")
    elif (hh=='sigmaT'):
        histos['%s_vs_pos'%hh].GetYaxis().SetTitle("#sigma_{t} (ps)")
    elif (hh=='CTR'):
        histos['%s_vs_pos'%hh].GetYaxis().SetTitle("CTR (ps)")
    elif (hh=='XT'):
        histos['%s_vs_pos'%hh].GetYaxis().SetTitle("XT")
    histos['%s_vs_pos'%hh].SetLineColor( 2 )
    histos['%s_vs_pos'%hh].SetMarkerColor( 1 )
    histos['%s_vs_pos'%hh].SetMarkerStyle( 21 )
    histos['%s_vs_pos'%hh].Draw("APE")
    if (hh=='LY'):
        histos['%s_vs_pos'%hh].GetYaxis().SetLimits(70,140)
        histos['%s_vs_pos'%hh].GetYaxis().SetRangeUser(70,140)
    elif (hh=='sigmaT'):
        histos['%s_vs_pos'%hh].GetYaxis().SetLimits(70,110)
        histos['%s_vs_pos'%hh].GetYaxis().SetRangeUser(70,110)
    elif (hh=='CTR'):
        histos['%s_vs_pos'%hh].GetYaxis().SetLimits(140,220)
        histos['%s_vs_pos'%hh].GetYaxis().SetRangeUser(140,220)
    elif (hh=='XT'):
        histos['%s_vs_pos'%hh].GetYaxis().SetLimits(0.,0.5)
        histos['%s_vs_pos'%hh].GetYaxis().SetRangeUser(0.,0.5)

    c1_LY.SaveAs(mergedLabel+"_"+"LSCAN_SUMMARY_%s.png"%hh)



