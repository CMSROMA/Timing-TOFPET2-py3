import ROOT as R

runs={

#[ RunID, Array, Reverse ] 
# Disclaimer: 
#  - Reverse=True  --> array bar code facing box door
#  - Reverse=False --> array bar code facing box rear (usual position)

############
#  First calibration 
############

####### IARR0
'0' :
[
['Run000101', '405', False ],
['Run000104', '405', True ],
['Run000107', '407', False ],
['Run000110', '407', True ],
['Run000113', '408', False ],
['Run000116', '408', True ],
['Run000119', '413', False ],
['Run000122', '413', True ],
],

}

import optparse
parser = optparse.OptionParser()
parser.add_option("-i", "--input", dest="inputDir",default="RESULTS/",
                  help="input directory")
parser.add_option("-o", "--output", dest="outputDir",default="/tmp/",
                  help="output directory")
parser.add_option("-n", "--iarr", dest="iarr",
                  help="iarr")
(opt, args) = parser.parse_args()

histos={}
histos['h1_LY_mean']=R.TH1F('h1_LY_mean','h1_LY_mean',100,40.,90.)
histos['h1_sigmaT_mean']=R.TH1F('h1_sigmaT_mean','h1_sigmaT_mean',50,100.,200.)
histos['h1_XT_mean']=R.TH1F('h1_XT_mean','h1_XT_mean',100,0.,0.2)

histos['h1_LY_rms']=R.TH1F('h1_LY_rms','h1_LY_rms',100,0.,0.1)
histos['h1_sigmaT_rms']=R.TH1F('h1_sigmaT_rms','h1_sigmaT_rms',50,0.,0.2)
histos['h1_XT_rms']=R.TH1F('h1_XT_rms','h1_XT_rms',100,0.,0.1)

for b in range(0,16):
    histos['h1_bar%d_LY_calib'%b]=R.TH1F('h1_bar%d_LY_calib'%b,'h1_bar%d_LY_calib'%b,200,0.5,1.5)
    histos['h1_bar%d_LY'%b]=R.TH1F('h1_bar%d_LY'%b,'h1_bar%d_LY'%b,400,0.,200.)
    histos['h1_bar%d_sigmaT'%b]=R.TH1F('h1_bar%d_sigmaT'%b,'h1_bar%d_sigmaT'%b,100,100.,200.)
    histos['h1_bar%d_XT'%b]=R.TH1F('h1_bar%d_XT'%b,'h1_bar%d_XT'%b,100,0.,0.4)

files={}
results={}
ev={}
histos['h1_LY_vsTemp']=R.TProfile('h1_LY_vsTemp','h1_LY_vsTemp',60,3,6)
for v in [ 'LY','sigmaT','XT']:
    histos['h1_%s_repro'%v]=R.TH1F('h1_%s_repro'%v,'h1_%s_repro'%v,200,-0.2,0.2)
for v in [ 'LY','sigmaT','XT']:
    histos['h1_%s_mean_repro'%v]=R.TH1F('h1_%s_mean_repro'%v,'h1_%s_mean_repro'%v,100,-0.2,0.2)


for run in runs[opt.iarr]:
    r=run[0]
    arrayCode=run[1]
    arrayConfig=run[2]
    if not 'h1_LY_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr) in histos.keys():
        histos['h1_LY_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr)]=R.TH1F('h1_LY_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr),'h1_LY_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr),400,0.,200.)
    if not 'h1_sigmaT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr) in histos.keys():
        histos['h1_sigmaT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr)]=R.TH1F('h1_sigmaT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr),'h1_sigmaT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr),100,100.,200.)
    if not 'h1_XT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr) in histos.keys():
        histos['h1_XT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr)]=R.TH1F('h1_XT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr),'h1_XT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr),100,0.,0.4)

for run in runs[opt.iarr]:
    r=run[0]
    arrayCode=run[1]
    arrayConfig=run[2]
    print("Analysying %s %s"%(r,arrayCode))
    print("%s/tree_%s_ARRAY%s.root"%(opt.inputDir,r,arrayCode.zfill(6)))
    files[r]=R.TFile.Open("%s/tree_%s_ARRAY%s.root"%(opt.inputDir,r,arrayCode.zfill(6)))
    results[r]=files[r].Get("results")
    results[r].GetEntry(0)
    for b in range(0,16):
        histos['h1_bar%d_LY_calib'%b].Fill(results[r].peak1_mean_barCoinc[b]/R.TMath.Mean(16,results[r].peak1_mean_barCoinc))
        histos['h1_bar%d_LY'%b].Fill(results[r].peak1_mean_barCoinc[b])
        histos['h1_bar%d_sigmaT'%b].Fill(results[r].deltaT12_sigma_barCoinc[b]/2.)
        histos['h1_bar%d_XT'%b].Fill(results[r].Xtalk_median_barCoinc[b])
        histos['h1_LY_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr)].Fill(results[r].peak1_mean_barCoinc[b])
        histos['h1_sigmaT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr)].Fill(results[r].deltaT12_sigma_barCoinc[b]/2.)
        histos['h1_XT_ARRAY%d_INV%d_IARR%s'%(int(arrayCode),arrayConfig,opt.iarr)].Fill(results[r].Xtalk_median_barCoinc[b])
        
    histos['h1_LY_mean'].Fill(R.TMath.Mean(16,results[r].peak1_mean_barCoinc))
    histos['h1_sigmaT_mean'].Fill(R.TMath.Mean(16,results[r].deltaT12_sigma_barCoinc)/2.)
    histos['h1_XT_mean'].Fill(R.TMath.Mean(16,results[r].Xtalk_median_barCoinc))
    histos['h1_LY_rms'].Fill(R.TMath.RMS(16,results[r].peak1_mean_barCoinc)/R.TMath.Mean(16,results[r].peak1_mean_barCoinc))
    histos['h1_sigmaT_rms'].Fill(R.TMath.RMS(16,results[r].deltaT12_sigma_barCoinc)/R.TMath.Mean(16,results[r].deltaT12_sigma_barCoinc))
    histos['h1_XT_rms'].Fill(R.TMath.RMS(16,results[r].Xtalk_median_barCoinc))

for run in runs[opt.iarr]:
    r=run[0]
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

for v in [ 'LY','sigmaT','XT', 'LY_calib']:
    histos['%s_vs_bar'%v]=R.TGraphErrors()
    histos['%s_vs_bar'%v].SetName('%s_vs_bar'%v)
for b in range(0,16):
    for v in [ 'LY','sigmaT','XT','LY_calib']:
        histos['%s_vs_bar'%v].SetPoint(b,b,histos['h1_bar%d_%s'%(b,v)].GetMean())
        histos['%s_vs_bar'%v].SetPointError(b,0.5,histos['h1_bar%d_%s'%(b,v)].GetRMS())

histos['h1_LY_unif']=R.TH1F('h1_LY_unif','h1_LY_unif',25,40.,90.)
histos['h1_sigmaT_unif']=R.TH1F('h1_sigmaT_unif','h1_sigmaT_unif',50,100.,200.)
histos['h1_XT_unif']=R.TH1F('h1_XT_unif','h1_XT_unif',25,0.,0.2)

for b in range(0,16):
    for v in [ 'LY','sigmaT','XT']:
        histos['h1_%s_unif'%v].Fill(histos['h1_bar%d_%s'%(b,v)].GetMean())

print("Writing %s/stability_IARR%s.root"%(opt.outputDir,opt.iarr))

out=R.TFile("%s/stability_IARR%s.root"%(opt.outputDir,opt.iarr),"RECREATE")
for hn,h in histos.iteritems():
    h.Write()
out.Close()

#Write final calibration factor
histos['intercalib_iarr%s'%opt.iarr]=R.TH1F('intercalib_iarr%s'%opt.iarr,'intercalib_iarr%s'%opt.iarr,16,-0.5,15.5)
histos['sigmaT_iarr%s'%opt.iarr]=R.TH1F('sigmaT_iarr%s'%opt.iarr,'sigmaT_iarr%s'%opt.iarr,16,-0.5,15.5)
for b in range(0,16):
    histos['intercalib_iarr%s'%opt.iarr].SetBinContent(b+1,histos['h1_bar%s_LY_calib'%(b)].GetMean())
    histos['intercalib_iarr%s'%opt.iarr].SetBinError(b+1,histos['h1_bar%s_LY_calib'%(b)].GetMeanError())
    histos['sigmaT_iarr%s'%opt.iarr].SetBinContent(b+1,histos['h1_bar%s_sigmaT'%(b)].GetMean())
    histos['sigmaT_iarr%s'%opt.iarr].SetBinError(b+1,histos['h1_bar%s_sigmaT'%(b)].GetMeanError())

print("Writing %s/calib_IARR%s.root"%(opt.outputDir,opt.iarr))
outCalib=R.TFile("%s/calib_IARR%s.root"%(opt.outputDir,opt.iarr),"RECREATE")
histos['intercalib_iarr%s'%opt.iarr].Write()
histos['sigmaT_iarr%s'%opt.iarr].Write()
outCalib.Close()
