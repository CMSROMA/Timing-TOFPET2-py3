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

#--------------------------------------------------------

usage = "usage: python analysis/analyze_alignArray_with_barRef.py -i /media/cmsdaq/ext/data/ALIGNMENT/ALIGNARRAY_11_02_2020 -o /media/cmsdaq/ext/data/ALIGNMENT/ALIGNARRAY_11_02_2020"

parser = optparse.OptionParser(usage)

parser.add_option("-i", "--input", dest="inputDir",
                  help="input directory")

parser.add_option("-o", "--output", dest="outputDir",
                  help="output directory")

(opt, args) = parser.parse_args()

if not opt.inputDir:   
    parser.error('input directory not provided')

if not opt.outputDir:   
    parser.error('output directory not provided')

#--------------------------------------------------------

gROOT.SetBatch(True)

gStyle.SetOptTitle(0)
gStyle.SetOptStat("e")
gStyle.SetOptFit(1111111)
gStyle.SetStatH(0.09)

#--------------------------------------------------------

list_allfiles = os.listdir(opt.inputDir)
#print (list_allfiles)

#alignBarNumber = 0
#alignBarCh1 = 2 
#alignBarCh2 = 18
alignBarNumber = 8
alignBarCh1 = 10
alignBarCh2 = 26

nbins = 3001
minVal = -0.5
maxVal = 300.5
#histoX = []
#histoY = []
#listARRAY = []
histoX = {}
histoY = {}
listARRAY = []

for file in list_allfiles:

    if ("_coincidences.root" in file):
        input_filename_coinc = opt.inputDir + "/" + file
        print (input_filename_coinc)

        ARRAY = int(input_filename_coinc.split("/")[-1].split("_")[3].replace("IARR",""))
        POS = int(input_filename_coinc.split("/")[-1].split("_")[4].replace("POS",""))
        X = float(input_filename_coinc.split("/")[-1].split("_")[5].replace("X",""))
        Y = float(input_filename_coinc.split("/")[-1].split("_")[6].replace("Y",""))
        Z = float(input_filename_coinc.split("/")[-1].split("_")[7].replace("Z",""))

        print (ARRAY, POS, X, Y, Z)

        if ARRAY not in listARRAY:        
            listARRAY.append(ARRAY)
            #histoX.append(TH1F("histoX"+"_array_"+str(ARRAY),"histoX"+"_array_"+str(ARRAY),nbins,minVal,maxVal))
            #histoY.append(TH1F("histoY"+"_array_"+str(ARRAY),"histoY"+"_array_"+str(ARRAY),nbins,minVal,maxVal))
            histoX[ARRAY]=TH1F("histoX"+"_array_"+str(ARRAY),"histoX"+"_array_"+str(ARRAY),nbins,minVal,maxVal)
            histoY[ARRAY]=TH1F("histoY"+"_array_"+str(ARRAY),"histoY"+"_array_"+str(ARRAY),nbins,minVal,maxVal)

        tfile = TFile.Open(input_filename_coinc)
        tree = tfile.Get("data")
        #tree.Print()

        nhits_coinc = 0 
        for event in range (0,tree.GetEntries()):
            tree.GetEntry(event)
                    
            #if(tree.energy[0]>-9 and tree.energy[1]>-9 and tree.energy[alignBarCh1]>-9 and tree.energy[alignBarCh2]>-9):
            if(tree.energy[0]>-9 and tree.energy[1]>-9 and tree.energy[alignBarCh1]>-9):
                nhits_coinc = nhits_coinc + 1

        print (nhits_coinc)

        #scan X
        if (POS >= 0 and POS <= 16):
            histoX[ARRAY].SetBinContent(histoX[ARRAY].GetXaxis().FindBin(X),nhits_coinc)
            histoX[ARRAY].SetBinError(histoX[ARRAY].GetXaxis().FindBin(X),sqrt(nhits_coinc))

        #scan Y
        if (POS >= 17 and POS <= 33):
            histoY[ARRAY].SetBinContent(histoY[ARRAY].GetXaxis().FindBin(Y),nhits_coinc)
            histoY[ARRAY].SetBinError(histoY[ARRAY].GetXaxis().FindBin(Y),sqrt(nhits_coinc))

        tfile.Close()

# available arrays
print (listARRAY)

posARRAY = {}

for iarr in listARRAY:

    #fit
    fitx = histoX[iarr].Fit("pol2","S")
    func_fitx = histoX[iarr].GetFunction("pol2")

    fity = histoY[iarr].Fit("pol2","S")
    func_fity = histoY[iarr].GetFunction("pol2")

    #maximum
    #max_X = func_fitx.GetX(func_fitx.GetMaximum())
    #max_Y = func_fity.GetX(func_fity.GetMaximum())
    max_X = func_fitx.GetMaximumX()
    max_Y = func_fity.GetMaximumX()
    print ("=================================")    
    print ("==== ARRAY, maxX, maxY: " , iarr, max_X, max_Y)
    print ("=================================")
    posARRAY[iarr]=(round(max_X,1),round(max_Y,1))
    
    #style
    histoX[iarr].GetXaxis().SetTitle("X (array "+str(iarr)+", bar"+str(alignBarNumber)+") [mm]")
    histoX[iarr].GetYaxis().SetTitle("Number of coincidences")
    histoX[iarr].GetYaxis().SetTitleOffset(1.6)
    histoX[iarr].SetMarkerStyle(20)
    histoX[iarr].SetMarkerSize(0.7)

    histoY[iarr].GetXaxis().SetTitle("Y (array "+str(iarr)+", bar"+str(alignBarNumber)+") [mm]")
    histoY[iarr].GetYaxis().SetTitle("Number of coincidences")
    histoY[iarr].GetYaxis().SetTitleOffset(1.6)
    histoY[iarr].SetMarkerStyle(20)
    histoY[iarr].SetMarkerSize(0.7)
    
    text1=TLatex()
    text1.SetTextSize(0.04)
    
    #plots
    c1 = TCanvas("c1","",500,500)
    histoX[iarr].Draw("pe")
    histoX[iarr].GetYaxis().SetLimits(histoX[iarr].GetMinimum()*0.8,histoX[iarr].GetMaximum()*1.2)
    histoX[iarr].GetYaxis().SetRangeUser(histoX[iarr].GetMinimum()*0.8,histoX[iarr].GetMaximum()*1.2)
    c1.SetGridx()
    c1.SetGridy()
    text1.DrawLatexNDC(0.11,0.93,"X_{max} = %.1f mm" % max_X)
    
    c2 = TCanvas("c2","",500,500)
    histoY[iarr].Draw("pe")
    histoY[iarr].GetYaxis().SetLimits(histoY[iarr].GetMinimum()*0.8,histoY[iarr].GetMaximum()*1.2)
    histoY[iarr].GetYaxis().SetRangeUser(histoY[iarr].GetMinimum()*0.8,histoY[iarr].GetMaximum()*1.2)
    c2.SetGridx()
    c2.SetGridy()
    text1.DrawLatexNDC(0.11,0.93,"Y_{max} = %.1f mm" % max_Y)
    
    c1.SaveAs(opt.outputDir+"/"+"alignArray_X"+"_array_"+str(iarr)+".png")
    c1.SaveAs(opt.outputDir+"/"+"alignArray_X"+"_array_"+str(iarr)+".root")
    c2.SaveAs(opt.outputDir+"/"+"alignArray_Y"+"_array_"+str(iarr)+".png")
    c2.SaveAs(opt.outputDir+"/"+"alignArray_Y"+"_array_"+str(iarr)+".root")

print ("Position of bar"+str(alignBarNumber)+" (counting from 0) of each array")
print ("ARRAY NUMBER ; (X, Y)")    
for iarr in listARRAY:
    print (iarr, posARRAY[iarr])
