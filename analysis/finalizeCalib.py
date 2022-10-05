import ROOT as R

#ref_arrays=[405,406,407,408]
ref_arrays=[405,414,407,408] #ref. arrays after 05/2022

c=R.TFile('data/stability.root')
ff=R.TFile('data/intercalib.root','UPDATE')
histos={}
for iarr in range(1,4):
    histos['h1_calib_IARR%d'%iarr]=R.TH1F('h1_calib_IARR%d'%iarr,'h1_calib_IARR%d'%iarr,100,0.8,1.2)
for arr in ref_arrays:
    for inv in range(0,2):
        h_ref=c.Get('h1_LY_ARRAY%d_INV%d_IARR%s'%(arr,inv,0))
        for iarr in range(1,4):
            h=c.Get('h1_LY_ARRAY%d_INV%d_IARR%s'%(arr,inv,iarr))
            print(iarr,h.GetMean()/h_ref.GetMean())
            histos['h1_calib_IARR%d'%iarr].Fill(h.GetMean()/h_ref.GetMean())

ff.cd()
for hn,h in histos.iteritems():
    h.Write()

for iarr in range(0,4):
    histos['calibFinal_IARR%d'%iarr]=R.TH1F('calibFinal_IARR%d'%iarr,'calibFinal_IARR%d'%iarr,16,-0.5,15.5)

for iarr in range(0,4):
    intercalib=ff.Get("intercalib_iarr%d"%iarr)
    for bar in range(0,16):
        if (iarr!=0):
            histos['calibFinal_IARR%d'%iarr].SetBinContent(bar+1,1./(intercalib.GetBinContent(bar+1)*histos['h1_calib_IARR%d'%iarr].GetMean()))
        else:
            histos['calibFinal_IARR%d'%iarr].SetBinContent(bar+1,1./(intercalib.GetBinContent(bar+1)))
    histos['calibFinal_IARR%d'%iarr].Print()
    histos['calibFinal_IARR%d'%iarr].Write()

for iarr in range(0,4):
    histos['sigmaTuniformity_IARR%d'%iarr]=R.TH1F('sigmaTuniformity_IARR%d'%iarr,'sigmaTuniformity_IARR%d'%iarr,100,120,150)

for iarr in range(0,4):
    sigmaT=ff.Get("sigmaT_iarr%d"%iarr)
    for bar in range(1,15):
        histos['sigmaTuniformity_IARR%d'%iarr].Fill(sigmaT.GetBinContent(bar+1))      
    histos['sigmaTuniformity_IARR%d'%iarr].Print()
    histos['sigmaTuniformity_IARR%d'%iarr].Write()

ff.Close()
c.Close()
