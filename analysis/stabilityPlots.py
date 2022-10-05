import os
import ROOT as R

R.gROOT.SetBatch(True)
runs=[ 

##Reprod runs (array 175 repositioned)
#'Run000083',
#'Run000086',
#'Run000089',
#'Run000092',
##Stability scan (array not repositioned different part of the thermal cycle)
#'Run000098',
#'Run000101',
#'Run000104',
#'Run000107',
#'Run000113',
#'Run000128',
#'Run000131',
#'Run000134',
#'Run000137',
#'Run000140',
#'Run000143'

####### ARRAY405
# 'Run000179',
# 'Run000191',
# 'Run000203',
# 'Run000215',
# 'Run000227',
#'Run000251' #reverse_array

####### ARRAY406
#'Run000170',
#'Run000182',
#'Run000194',
#'Run000206',
#'Run000218',
#'Run000230',
#'Run000254' #reverse_array

####### ARRAY407
#'Run000173',
#'Run000185',
#'Run000197',
#'Run000209',
#'Run000221',
#'Run000233',
#'Run000257' #reverse_array

####### ARRAY408
#'Run000176',
#'Run000188',
#'Run000200',
#'Run000212',
#'Run000224',
#'Run000236',
#'Run000260' #reverse_array

######## ARRAY 414 27/05/2022
# 'Run006499',
# 'Run006511',
# 'Run006523',
# 'Run006529',
# 'Run006535',
# 'Run006541',
# 'Run006547',
# 'Run006553',
# 'Run006559',
# 'Run006565'

######## ARRAY 405 27/05/2022
'Run006496',
'Run006508',
'Run006520',
'Run006526',
'Run006532',
'Run006538',
'Run006544',
'Run006550',
'Run006556',
'Run006562'

######## ARRAY 407 05/2022
# 'Run006365',
# 'Run006376',
# 'Run006388',
# 'Run006400',
# 'Run006412',
# 'Run006424',
# 'Run006442',
# 'Run006454',
# 'Run006466',
# 'Run006478'
]

import optparse
parser = optparse.OptionParser()
parser.add_option("-i", "--input", dest="inputDir",default="RESULTS/",
                  help="input directory")
parser.add_option("-o", "--output", dest="outputDir",default="/tmp/",
                  help="output directory")
parser.add_option("-n", "--arrayCode", dest="arrayCode", default=-99,
                  help="code of array")
parser.add_option("--pngAll", dest="pngAll", action='store_true', default=False,
                  help="save all histos as png")
(opt, args) = parser.parse_args()

os.system("mkdir -p %s"%opt.outputDir)

histos={}
histos['h1_LY_mean']=R.TH1F('h1_LY_mean','h1_LY_mean',100,40.,90.)
histos['h1_sigmaT_mean']=R.TH1F('h1_sigmaT_mean','h1_sigmaT_mean',50,100.,200.)
histos['h1_XT_mean']=R.TH1F('h1_XT_mean','h1_XT_mean',100,0.,0.2)

histos['h1_LY_rms']=R.TH1F('h1_LY_rms','h1_LY_rms',100,0.,0.1)
histos['h1_sigmaT_rms']=R.TH1F('h1_sigmaT_rms','h1_sigmaT_rms',50,0.,0.2)
histos['h1_XT_rms']=R.TH1F('h1_XT_rms','h1_XT_rms',100,0.,0.1)

#histos for individual bars
for b in range(0,16):
    histos['h1_bar%d_LY'%b]=R.TH1F('h1_bar%d_LY'%b,'h1_bar%d_LY'%b,200,40.,90.)
    histos['h1_bar%d_sigmaT'%b]=R.TH1F('h1_bar%d_sigmaT'%b,'h1_bar%d_sigmaT'%b,100,100.,200.)
    histos['h1_bar%d_XT'%b]=R.TH1F('h1_bar%d_XT'%b,'h1_bar%d_XT'%b,100,0.,0.4)

canvases={}
files={}
results={}
ev={}

histos['h1_LY_vsTemp']=R.TProfile('h1_LY_vsTemp','h1_LY_vsTemp',60,3,6)
for v in [ 'LY','sigmaT','XT']:
    histos['h1_%s_repro'%v]=R.TH1F('h1_%s_repro'%v,'h1_%s_repro'%v,200,-0.2,0.2)
for v in [ 'LY','sigmaT','XT']:
    histos['h1_%s_mean_repro'%v]=R.TH1F('h1_%s_mean_repro'%v,'h1_%s_mean_repro'%v,100,-0.2,0.2)

Nruns = len(runs)
for r in runs:
    files[r]=R.TFile.Open("%s/tree_%s_ARRAY%s.root"%(opt.inputDir,r,opt.arrayCode.zfill(6)))
    results[r]=files[r].Get("results")
    results[r].GetEntry(0)
    for b in range(0,16):
        histos['h1_bar%d_LY'%b].Fill(results[r].peak1_mean_barCoinc[b])
        histos['h1_bar%d_sigmaT'%b].Fill(results[r].deltaT12_sigma_barCoinc[b]/2.)
        histos['h1_bar%d_XT'%b].Fill(results[r].Xtalk_median_barCoinc[b])
    histos['h1_LY_mean'].Fill(R.TMath.Mean(16,results[r].peak1_mean_barCoinc))
    histos['h1_sigmaT_mean'].Fill(R.TMath.Mean(16,results[r].deltaT12_sigma_barCoinc)/2.)
    histos['h1_XT_mean'].Fill(R.TMath.Mean(16,results[r].Xtalk_median_barCoinc))
    histos['h1_LY_rms'].Fill(R.TMath.RMS(16,results[r].peak1_mean_barCoinc)/R.TMath.Mean(16,results[r].peak1_mean_barCoinc))
    histos['h1_sigmaT_rms'].Fill(R.TMath.RMS(16,results[r].deltaT12_sigma_barCoinc)/R.TMath.Mean(16,results[r].deltaT12_sigma_barCoinc))
    histos['h1_XT_rms'].Fill(R.TMath.RMS(16,results[r].Xtalk_median_barCoinc))

for r in runs:
    for b in range(0,16):
        histos['h1_LY_vsTemp'].Fill(results[r].temp_array1,results[r].peak1_mean_barCoinc[b]/histos['h1_bar%d_LY'%b].GetMean()-1.)
        for v in [ 'LY','sigmaT','XT']:
            if (v=='LY'):
                histos['h1_%s_repro'%v].Fill(results[r].peak1_mean_barCoinc[b]/histos['h1_bar%d_%s'%(b,v)].GetMean()-1.)
            elif (v=='sigmaT'):
                histos['h1_%s_repro'%v].Fill(results[r].deltaT12_sigma_barCoinc[b]/2./histos['h1_bar%d_%s'%(b,v)].GetMean()-1.)
            elif (v=='XT'):
                histos['h1_%s_repro'%v].Fill(results[r].Xtalk_median_barCoinc[b]-histos['h1_bar%d_%s'%(b,v)].GetMean())
    for v in [ 'LY','sigmaT','XT']:
        if (v=='LY'):
            histos['h1_%s_mean_repro'%v].Fill(R.TMath.Mean(16,results[r].peak1_mean_barCoinc)/histos['h1_%s_mean'%(v)].GetMean()-1.)
        elif (v=='sigmaT'):
            histos['h1_%s_mean_repro'%v].Fill(R.TMath.Mean(16,results[r].deltaT12_sigma_barCoinc)/2./histos['h1_%s_mean'%(v)].GetMean()-1.)
        elif (v=='XT'):
            histos['h1_%s_mean_repro'%v].Fill(R.TMath.Mean(16,results[r].Xtalk_median_barCoinc)-histos['h1_%s_mean'%(v)].GetMean())

for v in [ 'LY','sigmaT','XT']:
    histos['h1_%s_stability'%v]=R.TH1F('h1_%s_stability'%v,'h1_%s_stability'%v,100,0,0.1)

for b in range(0,16):

    for v in [ 'LY','sigmaT','XT']:

        mean=histos['h1_bar%d_%s'%(b,v)].GetMean()
        if (mean>0):
            histos['h1_%s_stability'%v].Fill(histos['h1_bar%d_%s'%(b,v)].GetRMS()/mean)
#            histos['h1_%s_stability'%v].Fill(histos['h1_bar%d_%s'%(b,v)].GetRMS())
        else:
            print("Warning: "+'h1_bar%d_%s'%(b,v))

# Draw plot of LY, sigmaT, XT vs bar number
colors = [R.kBlack, R.kBlue, R.kRed]
for i,v in enumerate(['LY','sigmaT','XT']):
    # canvases['%s_vs_bar'%v]=R.TCanvas('%s_vs_bar'%v,'%s_vs_bar'%v)
    # canvases['%s_vs_bar'%v].cd()

    histos['%s_vs_bar'%v]=R.TGraphErrors()
    histos['%s_vs_bar'%v].SetName('%s_vs_bar'%v)
    histos['%s_vs_bar'%v].GetXaxis().SetTitle("BAR ID")
    histos['%s_vs_bar'%v].GetYaxis().SetTitle("%s mean #pm RMS"%v)
    histos['%s_vs_bar'%v].SetMarkerStyle(8)
    histos['%s_vs_bar'%v].SetMarkerColor(colors[i])

    for b in range(0,16):
        histos['%s_vs_bar'%v].SetPoint(b,b,histos['h1_bar%d_%s'%(b,v)].GetMean())
        histos['%s_vs_bar'%v].SetPointError(b,0.000001,histos['h1_bar%d_%s'%(b,v)].GetRMS())
        
    # histos['%s_vs_bar'%v].Draw()
    # canvases['%s_vs_bar'%v].SaveAs('%s/%s_vs_bar.png'%(opt.outputDir,v))

# Draw plot of the RMS of LY, sigmaT, XT vs bar number
colors = [R.kBlack, R.kBlue, R.kRed]
for i,v in enumerate(['LY','sigmaT','XT']):
    # canvases['%s_rms_vs_bar'%v]=R.TCanvas('%s_rms_vs_bar'%v,'%s_rms_vs_bar'%v)
    # canvases['%s_rms_vs_bar'%v].cd()

    histos['%s_rms_vs_bar'%v]=R.TGraphErrors()
    histos['%s_rms_vs_bar'%v].SetName('%s_rms_vs_bar'%v)
    histos['%s_rms_vs_bar'%v].GetXaxis().SetTitle("BAR ID")
    histos['%s_rms_vs_bar'%v].GetYaxis().SetTitle("%s RMS "%v)
    histos['%s_rms_vs_bar'%v].SetMarkerStyle(8)
    histos['%s_rms_vs_bar'%v].SetMarkerColor(colors[i])

 
    for b in range(0,16):
        histos['%s_rms_vs_bar'%v].SetPoint(b,b,histos['h1_bar%d_%s'%(b,v)].GetRMS())
        histos['%s_rms_vs_bar'%v].SetPointError(b,0.00001,0.000001)
        
    # histos['%s_rms_vs_bar'%v].Draw()
    # canvases['%s_rms_vs_bar'%v].SaveAs('%s/%s_rms_vs_bar.png'%(opt.outputDir,v))

histos['h1_LY_unif']=R.TH1F('h1_LY_unif','h1_LY_unif',25,40.,90.)
histos['h1_sigmaT_unif']=R.TH1F('h1_sigmaT_unif','h1_sigmaT_unif',50,100.,200.)
histos['h1_XT_unif']=R.TH1F('h1_XT_unif','h1_XT_unif',25,0.,0.2)

if opt.pngAll:
    for histo in histos:
        histoname = histos[histo].GetName()
        canvases[histoname]=R.TCanvas(histoname, histoname)
        canvases[histoname].cd()
        histos[histo].Draw()
        canvases[histoname].SaveAs('%s/%s.png'%(opt.outputDir,histoname))

for b in range(1,15):
    for v in [ 'LY','sigmaT','XT']:
        histos['h1_%s_unif'%v].Fill(histos['h1_bar%d_%s'%(b,v)].GetMean())

out=R.TFile("%s/stability_ARRAY%s.root"%(opt.outputDir,opt.arrayCode.zfill(6)),"RECREATE")
for hn,h in histos.iteritems():
    h.Write()
out.Close()


