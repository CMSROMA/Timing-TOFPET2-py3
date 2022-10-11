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

usage = "usage: python analysis/launch_analyze_run_array.py --firstRun 1 -i /home/cmsdaq/Workspace/TOFPET/Timing-TOFPET/output/TestArray -o /home/cmsdaq/Workspace/TOFPET/Timing-TOFPET/output/TestArray/RESULTS --arrayCode 0"

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

nFilesInScan = 1

mergedTree = str(opt.outputDir)+"/"+"tree_"+"FirstRun" + str(opt.firstRun.zfill(6)) + "_LastRun" + str((int(opt.firstRun)+(nFilesInScan-1)*3)).zfill(6) + "_ARRAY" + str(opt.arrayCode.zfill(6))+".root"

summaryFile = str(opt.outputDir)+"/"+"summary_"+"FirstRun" + str(opt.firstRun.zfill(6)) + "_LastRun" + str((int(opt.firstRun)+(nFilesInScan-1)*3)).zfill(6) + "_ARRAY" + str(opt.arrayCode.zfill(6))+".root"

commandMerge = "hadd -f "+mergedTree
print(commandMerge)

for step in range(0,nFilesInScan):

    #Launch analysis for each step of the position scan
#    print(step
    currentRun = int(opt.firstRun) + step*3    
#    command = "python analysis/analyze_run_array.py --run "+ str(currentRun) +" --arrayCode "+str(opt.arrayCode)+" -i "+str(opt.inputDir)+" -o "+str(opt.outputDir)
#    print(command
#    os.system(command)
#
    #Update command to merge trees
    commandMerge = commandMerge+" "+str(opt.outputDir)+"/"+"tree"+"_Run"+str(currentRun).zfill(6)+"_*"
    
print(commandMerge)
os.system(commandMerge)

#### Analysis Summary #####

tfileResults = TFile.Open(mergedTree)

treeResults = tfileResults.Get("results")

histos={}
histos['h1_LY_bar'] = TH1F("h1_LY_bar", "", 1000, 0, 100)
histos['h1_sigmaT_bar'] = TH1F("h1_sigmaT_bar", "", 5000, 0, 500)
histos['h1_CTR_bar'] = TH1F("h1_CTR_bar", "", 5000, 0, 500)
histos['h1_XT_bar'] = TH1F("h1_XT_bar", "", 1000, 0, 1.)

for v in [ 'LY','sigmaT','CTR','XT']:
    histos['%s_vs_bar'%v]=TGraphErrors()
    histos['%s_vs_bar'%v].SetName('%s_vs_bar'%v)

for event in range (0,treeResults.GetEntries()):
    treeResults.GetEntry(event)

    for b in range(1,15):
        if (treeResults.peak1_mean_barCoinc[b]<=0 or treeResults.deltaT12_sigma_barCoinc[b]<=0):
            continue
        histos['h1_LY_bar'].Fill(treeResults.peak1_mean_barCoinc[b])
        histos['h1_sigmaT_bar'].Fill(treeResults.deltaT12_sigma_barCoinc[b]/2.)
        histos['h1_CTR_bar'].Fill(treeResults.CTR_sigma_barCoinc[b])
        histos['h1_XT_bar'].Fill(treeResults.Xtalk_median_barCoinc[b])

    ib=0
    for b in range(0,16):
        if (treeResults.peak1_mean_barCoinc[b]<=0 or treeResults.deltaT12_sigma_barCoinc[b]<=0):
            continue
        histos['LY_vs_bar'].SetPoint(ib,b,treeResults.peak1_mean_barCoinc[b])
        histos['LY_vs_bar'].SetPointError(ib,0.5,treeResults.err_peak1_mean_barCoinc[b])
        histos['sigmaT_vs_bar'].SetPoint(ib,b,treeResults.deltaT12_sigma_barCoinc[b]/2.)
        histos['sigmaT_vs_bar'].SetPointError(ib,0.5,treeResults.err_deltaT12_sigma_barCoinc[b]/2.)
        histos['CTR_vs_bar'].SetPoint(ib,b,treeResults.CTR_sigma_barCoinc[b])
        histos['CTR_vs_bar'].SetPointError(ib,0.5,treeResults.err_CTR_sigma_barCoinc[b])
        histos['XT_vs_bar'].SetPoint(ib,b,treeResults.Xtalk_median_barCoinc[b])
        histos['XT_vs_bar'].SetPointError(ib,0.5,0.01)
        ib+=1
##########################################

mergedLabel = str(opt.outputDir)+"/"+"tree_"+"FirstRun" + str(opt.firstRun.zfill(6)) + "_LastRun" + str((int(opt.firstRun)+(nFilesInScan-1)*3)).zfill(6) + "_ARRAY" + str(opt.arrayCode.zfill(6))

summary=TFile(summaryFile,"RECREATE")

c1_LY = TCanvas("phe_peak", "phe_peak", 900, 700)     

#edits Livia
mean1 = TPaveText(0.12, 0.12, 0.45, 0.2, "brNDC")
mean1.SetFillColor(kWhite)
mean1.SetBorderSize(0)
mean1.AddText(Form("mean = %.2f #pm %.2f QDC"%(histos['h1_LY_bar'].GetMean(),histos['h1_LY_bar'].GetRMS())))

c1_LY.cd()  
gStyle.SetOptStat(1111);
c1_LY.SetGrid();
histos['LY_vs_bar'].GetXaxis().SetTitle("BAR ID")
histos['LY_vs_bar'].GetYaxis().SetTitle("Energy (QDC)")
histos['LY_vs_bar'].SetLineColor( 2 )
histos['LY_vs_bar'].SetMarkerColor( 1 )
histos['LY_vs_bar'].SetMarkerStyle( 21 )
histos['LY_vs_bar'].Draw("APE")
mean1.Draw()

b_LY=TBox(0,histos['h1_LY_bar'].GetMean()-histos['h1_LY_bar'].GetRMS(),16,histos['h1_LY_bar'].GetMean()+histos['h1_LY_bar'].GetRMS())
b_LY.SetFillStyle(3001)
b_LY.SetFillColorAlpha(kYellow,0.7)
b_LY.Draw()
l_LY=TLine(0,histos['h1_LY_bar'].GetMean(),16,histos['h1_LY_bar'].GetMean())
l_LY.SetLineColor(kRed)
l_LY.SetLineStyle(2)
l_LY.SetLineWidth(3)
l_LY.Draw()
histos['LY_vs_bar'].Draw("PESAME")
c1_LY.SaveAs(mergedLabel+"_"+"SUMMARY_phePeak.png")
c1_LY.Write()

c2_sigmaT = TCanvas("sigmaT_sigma", "sigmaT_sigma", 900, 700)

mean2 = TPaveText(0.15, 0.75, 0.45, 0.89, "brNDC")
mean2.SetFillColor(kWhite)
mean2.SetBorderSize(0)
mean2.AddText(Form("mean = %.1f #pm %.1f ps"%(histos['h1_sigmaT_bar'].GetMean(),histos['h1_sigmaT_bar'].GetRMS())))

c2_sigmaT.cd()
gStyle.SetOptStat(1111);
c2_sigmaT.SetGrid();
histos['sigmaT_vs_bar'].GetXaxis().SetTitle("BAR ID")
histos['sigmaT_vs_bar'].GetYaxis().SetTitle("sigmaT #sigma (ps)")
histos['sigmaT_vs_bar'].SetLineColor( 2 )
histos['sigmaT_vs_bar'].SetMarkerColor( 1 )
histos['sigmaT_vs_bar'].SetMarkerStyle( 21 )
histos['sigmaT_vs_bar'].Draw("APE")
mean2.Draw()

b_sigmaT=TBox(0,histos['h1_sigmaT_bar'].GetMean()-histos['h1_sigmaT_bar'].GetRMS(),16,histos['h1_sigmaT_bar'].GetMean()+histos['h1_sigmaT_bar'].GetRMS())
b_sigmaT.SetFillStyle(3001)
b_sigmaT.SetFillColorAlpha(kYellow,0.7)
b_sigmaT.Draw()
l_sigmaT=TLine(0,histos['h1_sigmaT_bar'].GetMean(),16,histos['h1_sigmaT_bar'].GetMean())
l_sigmaT.SetLineColor(kRed)
l_sigmaT.SetLineStyle(2)
l_sigmaT.SetLineWidth(3)
l_sigmaT.Draw()
histos['sigmaT_vs_bar'].Draw("PESAME")

c2_sigmaT.SaveAs(mergedLabel+"_"+"SUMMARY_sigmaT.png")
c2_sigmaT.Write()

c2_CTR = TCanvas("CTR_sigma", "CTR_sigma", 900, 700)

mean2 = TPaveText(0.15, 0.75, 0.45, 0.89, "brNDC")
mean2.SetFillColor(kWhite)
mean2.SetBorderSize(0)
mean2.AddText(Form("mean = %.1f #pm %.1f ps"%(histos['h1_CTR_bar'].GetMean(),histos['h1_CTR_bar'].GetRMS())))

c2_CTR.cd()
gStyle.SetOptStat(1111);
c2_CTR.SetGrid();
histos['CTR_vs_bar'].GetXaxis().SetTitle("BAR ID")
histos['CTR_vs_bar'].GetYaxis().SetTitle("CTR #sigma (ps)")
histos['CTR_vs_bar'].SetLineColor( 2 )
histos['CTR_vs_bar'].SetMarkerColor( 1 )
histos['CTR_vs_bar'].SetMarkerStyle( 21 )
histos['CTR_vs_bar'].Draw("APE")
mean2.Draw()

b_CTR=TBox(0,histos['h1_CTR_bar'].GetMean()-histos['h1_CTR_bar'].GetRMS(),16,histos['h1_CTR_bar'].GetMean()+histos['h1_CTR_bar'].GetRMS())
b_CTR.SetFillStyle(3001)
b_CTR.SetFillColorAlpha(kYellow,0.7)
b_CTR.Draw()
l_CTR=TLine(0,histos['h1_CTR_bar'].GetMean(),16,histos['h1_CTR_bar'].GetMean())
l_CTR.SetLineColor(kRed)
l_CTR.SetLineStyle(2)
l_CTR.SetLineWidth(3)
l_CTR.Draw()
histos['CTR_vs_bar'].Draw("PESAME")

c2_CTR.SaveAs(mergedLabel+"_"+"SUMMARY_CTR.png")
c2_CTR.Write()

c3_XT = TCanvas("cross-talk", "cross-talk", 900, 700)
mean3 = TPaveText(0.6, 0.78, 0.9, 0.88, "brNDC")
mean3.SetFillColor(kWhite)
mean3.SetBorderSize(0)
mean3.AddText(Form("mean = %.2f #pm %.2f"%(histos['h1_XT_bar'].GetMean(),histos['h1_XT_bar'].GetRMS())))
c3_XT.cd()
gStyle.SetOptStat(1111);
c3_XT.SetGrid();
histos['XT_vs_bar'].GetXaxis().SetTitle("BAR ID")
histos['XT_vs_bar'].GetYaxis().SetTitle("XT")
histos['XT_vs_bar'].SetLineColor( 2 )
histos['XT_vs_bar'].SetMarkerColor( 1 )
histos['XT_vs_bar'].SetMarkerStyle( 21 )
histos['XT_vs_bar'].Draw("APE")
mean3.Draw()

b_XT=TBox(0,histos['h1_XT_bar'].GetMean()-histos['h1_XT_bar'].GetRMS(),16,histos['h1_XT_bar'].GetMean()+histos['h1_XT_bar'].GetRMS())
b_XT.SetFillStyle(3001)
b_XT.SetFillColorAlpha(kYellow,0.7)
b_XT.Draw()
l_XT=TLine(0,histos['h1_XT_bar'].GetMean(),16,histos['h1_XT_bar'].GetMean())
l_XT.SetLineColor(kRed)
l_XT.SetLineStyle(2)
l_XT.SetLineWidth(3)
l_XT.Draw()
histos['XT_vs_bar'].Draw("PESAME")

c3_XT.SaveAs(mergedLabel+"_"+"SUMMARY_xtalk.png")
c3_XT.Write()

for hn,h in histos.items():
    h.Write()
summary.Print()
summary.Close()

###################################################
LY_mean = histos['h1_LY_bar'].GetMean()
LY_RMS = histos['h1_LY_bar'].GetRMS()

sigmaT_sigma_mean = histos['h1_sigmaT_bar'].GetMean()
sigmaT_sigma_RMS = histos['h1_sigmaT_bar'].GetRMS()

CTR_sigma_mean = histos['h1_CTR_bar'].GetMean()
CTR_sigma_RMS = histos['h1_CTR_bar'].GetRMS()

XT_mean = histos['h1_XT_bar'].GetMean()
XT_RMS = histos['h1_XT_bar'].GetRMS()

import requests

def logMatterMost(incoming_hook_url,payload):
    r = requests.post(incoming_hook_url, json=payload)
    if r.status_code != 200:
        raise RuntimeError(r.text)
    else:
        print(r.text)

mattermost_hook=os.environ['WEB_HOOK']

#printing for ELOG
payload_text= "ARRAY" + str(opt.arrayCode.zfill(6))+" RUNS: \n"
for step in range(0,nFilesInScan):
    payload_text+= str(int(opt.firstRun) + step*3) + " - \n"
payload_text+= "LY_mean="+str("{:10.2f}\n".format(LY_mean))
payload_text+= "LY_RMS="+str("{:10.2f}\n".format(LY_RMS))
payload_text+= "sigmaT_sigma_mean="+str("{:10.1f}".format(sigmaT_sigma_mean))+" ps\n"
payload_text+= "sigmaT_sigma_RMS="+str("{:10.1f}".format(sigmaT_sigma_RMS))+" ps\n"
payload_text+= "CTR_sigma_mean="+str("{:10.1f}".format(CTR_sigma_mean))+" ps\n"
payload_text+= "CTR_sigma_RMS="+str("{:10.1f}".format(CTR_sigma_RMS))+" ps\n"
payload_text+= "XT_mean="+str("{:10.2f}\n".format(XT_mean))
payload_text+= "XT_RMS="+str("{:10.2f}\n".format(XT_RMS))
print(payload_text)

import socket
host='10.0.0.33'
payload_text+= "http://%s/ARRAYS/index.php?match=Run%s"%(host,str(int(opt.firstRun)).zfill(6))

this_payload={
  "username": "array-bench",
  "icon_url": "https://mattermost.org/wp-content/uploads/2016/04/icon.png",
  "text": "#### Summary Run%s\n"%(str(opt.firstRun.zfill(6)))+payload_text
}
logMatterMost(mattermost_hook,this_payload)
