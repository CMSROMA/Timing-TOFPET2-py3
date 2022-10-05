import ROOT as R

runs={

#[ RunID, Array, Reverse ] 
# Disclaimer: 
#  - Reverse=True  --> array bar code facing fridge door
#  - Reverse=False --> array bar code facing fridge rear (usual position)

############
#  First calibration 
############
#'0' :
#[
#['Run000263', '405', True ],
#['Run000275', '405', True ],
#['Run000287', '405', False ],
#['Run000299', '405', False ],
#['Run000311', '405', True ],
#['Run000323', '405', True ],
#['Run000347', '408', False ],
#['Run000359', '408', False ],
#['Run000383', '408', True ],
#['Run000407', '408', True ],
#['Run000419', '407', False ],
#['Run000431', '407', False ],
#['Run000443', '407', True ],
#['Run000455', '407', True ],
#['Run000467', '406', False ],
#['Run000479', '406', False ],
#['Run000491', '406', True ],
#['Run000503', '406', True ],
#],
#
#
#
######## IARR1
#'1' :
#[
#['Run000266', '406', True ],
#['Run000278', '406', True ],
#['Run000290', '406', False ],
#['Run000302', '406', False ],
#['Run000314', '406', True ],
#['Run000326', '406', True ],
#['Run000350', '405', False ],
#['Run000362', '405', False ],
#['Run000386', '405', True ],
#['Run000410', '405', True ],
#['Run000422', '408', False ],
#['Run000434', '408', False ],
#['Run000446', '408', True ],
#['Run000458', '408', True ],
#['Run000470', '407', False ],
#['Run000482', '407', False ],
#['Run000494', '407', True ],
#['Run000506', '407', True ],
#],
#
#
#
######## IARR2
#'2' :
#[
#['Run000269', '407', True ],
#['Run000281', '407', True ],
#['Run000293', '407', False ],
#['Run000305', '407', False ],
#['Run000317', '407', True ],
#['Run000329', '407', True ],
#['Run000353', '406', False ],
#['Run000365', '406', False ],
#['Run000389', '406', True ],
#['Run000413', '406', True ],
#['Run000425', '405', False ],
#['Run000437', '405', False ],
#['Run000449', '405', True ],
#['Run000461', '405', True ],
#['Run000473', '408', False ],
#['Run000485', '408', False ],
#['Run000497', '408', True ],
#['Run000509', '408', True ],
#],
#
#
#
######## IARR3
#'3' :
#[
#['Run000272', '408', True ],
#['Run000284', '408', True ],
#['Run000296', '408', False ],
#['Run000308', '408', False ],
#['Run000320', '408', True ],
#['Run000332', '408', True ],
#['Run000356', '407', False ],
#['Run000368', '407', False ],
#['Run000392', '407', True ],
#['Run000416', '407', True ],
#['Run000428', '406', False ],
#['Run000440', '406', False ],
#['Run000452', '406', True ],
#['Run000464', '406', True ],
#['Run000476', '405', False ],
#['Run000488', '405', False ],
#['Run000500', '405', True ],
#['Run000512', '405', True ],
#]
#
#}
#

############
#  Calib 10/9/2021
############

# '0' :
# [
# ['Run003313', '405', False ],
# ['Run003325', '405', False ],
# ['Run003337', '405', True ],
# ['Run003349', '405', True ],
# ['Run003361', '408', False ],
# ['Run003373', '408', False ],
# ['Run003385', '408', True ],
# ['Run003397', '408', True ],
# ['Run003409', '407', False ],
# ['Run003421', '407', False ],
# ['Run003433', '407', True ],
# ['Run003445', '407', True ],
# ['Run003457', '406', False ],
# ['Run003469', '406', False ],
# ['Run003481', '406', True ],
# ['Run003493', '406', True ],
# ],



####### IARR1
# '1' :
# [
# ['Run003316', '406', False ],
# ['Run003328', '406', False ],
# ['Run003340', '406', True ],
# ['Run003352', '406', True ],
# ['Run003364', '405', False ],
# ['Run003376', '405', False ],
# ['Run003388', '405', True ],
# ['Run003400', '405', True ],
# ['Run003412', '408', False ],
# ['Run003424', '408', False ],
# ['Run003436', '408', True ],
# ['Run003448', '408', True ],
# ['Run003460', '407', False ],
# ['Run003472', '407', False ],
# ['Run003484', '407', True ],
# ['Run003496', '407', True ],
# ],



####### IARR2
# '2' :
# [
# ['Run003319', '407', False ],
# ['Run003331', '407', False ],
# ['Run003343', '407', True ],
# ['Run003355', '407', True ],
# ['Run003367', '406', False ],
# ['Run003379', '406', False ],
# ['Run003391', '406', True ],
# ['Run003403', '406', True ],
# ['Run003415', '405', False ],
# ['Run003427', '405', False ],
# ['Run003439', '405', True ],
# ['Run003451', '405', True ],
# ['Run003463', '408', False ],
# ['Run003475', '408', False ],
# ['Run003487', '408', True ],
# ['Run003499', '408', True ],
# ],

# ####### IARR3
# '3' :
# [
# ['Run003322', '408', False ],
# ['Run003334', '408', False ],
# ['Run003346', '408', True ],
# ['Run003358', '408', True ],
# ['Run003370', '407', False ],
# ['Run003382', '407', False ],
# ['Run003394', '407', True ],
# ['Run003406', '407', True ],
# ['Run003418', '406', False ],
# ['Run003430', '406', False ],
# ['Run003442', '406', True ],
# ['Run003454', '406', True ],
# ['Run003466', '405', False ],
# ['Run003478', '405', False ],
# ['Run003490', '405', True ],
# ['Run003502', '405', True ],
# ]
#
#}

##########
# Calibration 27/06/2022
# array 405, 414, 407, 408
##########

####### IARR0
# '0' :
# [
# ['Run006600', '405', False ],
# ['Run006612', '408', False ],
# ['Run006624', '407', False ],
# ['Run006636', '414', False ],
# ['Run006648', '405', True ],
# ['Run006660', '408', True ],
# ['Run006694', '407', True ],
# ['Run006715', '414', True ],
# ],

# ####### IARR1
# '1' :
# [
# ['Run006603', '414', False ],
# ['Run006615', '405', False ],
# ['Run006627', '408', False ],
# ['Run006639', '407', False ],
# ['Run006651', '414', True ],
# ['Run006685', '405', True ],
# ['Run006706', '408', True ],
# ['Run006718', '407', True ],
# ],

# ####### IARR2
# '2' :
# [
# ['Run006606', '407', False ],
# ['Run006618', '414', False ],
# ['Run006630', '405', False ],
# ['Run006642', '408', False ],
# ['Run006654', '407', True ],
# ['Run006688', '414', True ],
# ['Run006709', '405', True ],
# ['Run006721', '408', True ],
# ],

# ####### IARR3
# '3' :
# [
# ['Run006609', '408', False ],
# ['Run006621', '407', False ],
# ['Run006633', '414', False ],
# ['Run006645', '405', False ],
# ['Run006657', '408', True ],
# ['Run006691', '407', True ],
# ['Run006712', '414', True ],
# ['Run006724', '405', True ],
# ]

# }

##########
# Calibration 30/06/2022
# array 405, 414, 407, 408
##########

####### IARR0
'0' :
[
['Run006600', '405', False ],
['Run006612', '408', False ],
['Run006624', '407', False ],
['Run006636', '414', False ],
['Run006648', '405', True ],
['Run006660', '408', True ],
['Run006694', '407', True ],
['Run006715', '414', True ],
['Run006751', '405', False ],
['Run006774', '408', False ],
['Run006786', '407', False ],
['Run006798', '414', False ],
['Run006822', '405', True ],
['Run006846', '408', True ],
['Run006858', '407', True ],
['Run006882', '414', True ],
],

####### IARR1
'1' :
[
['Run006603', '414', False ],
['Run006615', '405', False ],
['Run006627', '408', False ],
['Run006639', '407', False ],
['Run006651', '414', True ],
['Run006685', '405', True ],
['Run006706', '408', True ],
['Run006718', '407', True ],
['Run006763', '414', False ],
['Run006777', '405', False ],
['Run006789', '408', False ],
['Run006801', '407', False ],
['Run006825', '414', True ],
['Run006849', '405', True ],
['Run006861', '408', True ],
['Run006873', '407', True ],
],

####### IARR2
'2' :
[
['Run006606', '407', False ],
['Run006618', '414', False ],
['Run006630', '405', False ],
['Run006642', '408', False ],
['Run006654', '407', True ],
['Run006688', '414', True ],
['Run006709', '405', True ],
['Run006721', '408', True ],
['Run006757', '407', False ],
['Run006780', '414', False ],
['Run006792', '405', False ],
['Run006804', '408', False ],
['Run006828', '407', True ],
['Run006852', '414', True ],
['Run006864', '405', True ],
['Run006876', '408', True ],
],

####### IARR3
'3' :
[
['Run006609', '408', False ],
['Run006621', '407', False ],
['Run006633', '414', False ],
['Run006645', '405', False ],
['Run006657', '408', True ],
['Run006691', '407', True ],
['Run006712', '414', True ],
['Run006724', '405', True ],
['Run006760', '408', False ],
['Run006783', '407', False ],
['Run006795', '414', False ],
['Run006807', '405', False ],
['Run006831', '408', True ],
['Run006855', '407', True ],
['Run006867', '414', True ],
['Run006885', '405', True ],
]

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
    print "Analysying %s %s"%(r,arrayCode)
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

for b in range(1,15):
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
