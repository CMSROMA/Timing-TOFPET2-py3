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
import time
import re
import numpy as np
import copy

runNumberFileName="/home/cmsdaq/Workspace/TOFPET/Timing-TOFPET2-py3/data/RunNumbersTest.txt"

sys.path.insert(1, os.path.join(sys.path[0], 'arduino/tablexy/'))
from xyMover import XYMover

usage = "usage: run from Timing-TOFPET2-py3: \n python run_DAQ_multiarrays.py -c list_config_main_array.txt -o data/LYSOMULTIARRAYTEST -n TEST" 
parser = optparse.OptionParser(usage)
parser.add_option("-c", "--config", dest="configFile",
                  help="list with config file for different arrays")
parser.add_option("-o", "--outFolder", dest="outputFolder",
                  help="output directory")
parser.add_option("-l", "--lscan", dest="lscan",action="store_true",default=False,
                  help="longitudinal scan")
parser.add_option("-s", "--thscan", dest="thscan",action="store_true",default=False,
                  help="th1 scan")
parser.add_option("--pedAllChannels", dest="pedAllChannels", default=0, 
                  help="Set to 1 to collect pedestals for all channels (default is 0)")
parser.add_option("-n", "--name", dest="nameLabel",
                  help="label for output files")
parser.add_option("-t", "--tag", dest="tag",default='',
                  help="additional tag")

(opt, args) = parser.parse_args()
if not opt.configFile:   
    parser.error('list of config files not provided')
if not opt.outputFolder:   
    parser.error('output folder not provided')
if not opt.nameLabel:   
    parser.error('label for output files not provided')

if "_" in opt.nameLabel:
    parser.error('do not include the symbol _ in the nameLabel option (-n or --name)')

#############################
## Input and output
#############################

cfileNames = []
for ff in open(opt.configFile):
    ff = ff.rstrip()
    cfileNames.append(ff)
print (cfileNames)

commandOutputDir = "mkdir -p "+opt.outputFolder
print (commandOutputDir)
os.system(commandOutputDir)

#############################
## Daq script
#############################

def RUN(configArray,runtype,time,ov,ovref,gate,label,enabledCh,thresholds,thresholdsT1,nloops,sleep,timePed):

    ###############
    ## Current time
    ###############
    current_time = datetime.datetime.now()
    simpletimeMarker = "%04d-%02d-%02d-%02d-%02d-%02d" % (current_time.year,current_time.month,current_time.day,current_time.hour,current_time.minute,current_time.second)
    print (simpletimeMarker)

    ####################
    ## Update run number
    ####################
    currentRun = 0
    #outputFileName = opt.outputFolder+"/RunNumbers.txt"
    ##outputFileName = "/media/cmsdaq/ext/data/RunNumbers.txt" 
    outputFileName = runNumberFileName

    with open(outputFileName) as file:
        arrayRuns = file.readlines()

    file_runs = open(outputFileName, 'a+')
    
    #lastRun = subprocess.check_output(['tail', '-1', outputFileName])
    lastRun = arrayRuns[-1]
    lastRun = lastRun.rstrip('\n')
    
    if not lastRun:
        currentRun = 1
    else:
        currentRun = int(lastRun) + 1
    file_runs.write(str(currentRun)+'\n')    
    file_runs.close()

    #################
    ## Write commands
    #################
    newlabel = "Run"+str(currentRun).zfill(6)+"_"+simpletimeMarker+"_"+label
    
    if(runtype == "PED"):
        commandRun = "python run_TOFPET.py -c "+ configArray+" --runType PED -d acquire_pedestal_data " + "-t "+ str(time)+" -v "+str(ov)+" --ovref "+str(ovref)+" -l "+str(newlabel)+" -g "+str(gate)+" -o "+opt.outputFolder+" --pedAllChannels " + str(opt.pedAllChannels) + " --nloops " + str(nloops) + " --sleep " + str(sleep)  
        if (enabledCh!=""):
            commandRun = commandRun +" --enabledChannels " + str(enabledCh) 
    if(runtype == "PHYS"):
        commandRun = "python run_TOFPET.py -c "+ configArray+" --runType PHYS -d my_acquire_sipm_data " + "-t "+ str(time)+" -v "+str(ov)+" --ovref "+str(ovref)+" -l "+str(newlabel)+" -g "+str(gate)+" -o "+opt.outputFolder + " --nloops " + str(nloops) + " --sleep " + str(sleep) + " --timePed " + str(timePed)
        if (enabledCh!=""):
            commandRun = commandRun +" --enabledChannels " + str(enabledCh) 
            if (thresholds!=""):
                commandRun = commandRun + " --energyThr " + str(thresholds)
            if (thresholdsT1!=""):
                commandRun = commandRun + " --energyThrT1 " + str(thresholdsT1)

    print (commandRun)

## without Airtable
    os.system(commandRun)
    return;

### with Airtable
#    tags=newlabel.split('_')
#    print(tags)
#    posX=float(tags[5].replace('X',''))
#    posY=float(tags[6].replace('Y',''))
#    dbCommand = ". ~/AutoProcess/setup.sh; python3 ~/AutoProcess/insertRun.py --id=%s --type=%s --tag=%s --ov=%1.f --posx=%.1f --posy=%.1f "%(tags[0],runtype,newlabel,ov,posX,posY)
#    if (runtype == 'PHYS'):
#        dbCommand += ' --xtal=%s'%tags[2]
#    print(dbCommand)
##    insertDBStatus=os.WEXITSTATUS(os.system(dbCommand))
#    insertDBStatus=subprocess.call(dbCommand,shell=True)
#    if (not insertDBStatus==0):
#        print('Error writing %s to runDB. Giving up'%tags[0])
#        return
#
##    exitStatus=os.WEXITSTATUS(os.system(commandRun))
#    exitStatus=subprocess.call(commandRun,shell=True)
#
#    dbCommandCompleted = ". ~/AutoProcess/setup.sh; python3 ~/AutoProcess/updateRun.py --id=%s --status='%s'"%(tags[0],'DAQ COMPLETED' if exitStatus==0 else 'FAILED')
#    print(dbCommandCompleted)
##    insertDBStatus=os.WEXITSTATUS(os.system(dbCommandCompleted))
#    insertDBStatus=subprocess.call(dbCommandCompleted,shell=True)
#    if (not insertDBStatus==0):
#        print('Error writing %s to runDB. Giving up'%tags[0])
#        return
#    else:
#        print('%s successfully inserted into RunDB'%tags[0])
#
#    return;



###################
## Daq settings
###################

## array ##

channelList="0_1_10_26"
nch = len(channelList.split("_"))
#
#energyThrValue = 20
#energyThrList = '_'.join([str(energyThrValue)] * nch)
energyThrList = "5_5_5_5"
#
#t1ThrValue = 35
#t1ThrList = '_'.join([str(t1ThrValue)] * nch)
t1ThrList = "35_35_35_35"
#
t_ped = 0.3 #s
t_phys = 10 #s
#ov_values = [8,4] #V
ov_values = [8] #V
ovref_values = [8] #V
gate_values = [34] # # MIN_INTG_TIME/MAX_INTG_TIME 34 = (34 x 4 - 78) x 5 ns = 290ns (for values in range 32...127). Check TOFPET2C ASIC guide.
#names = opt.nameLabel.split(',')
names = opt.nameLabel
t_ped_in_phys = 0. #s
nloops = 1
sleep = 0
# 
# xyz position of the center of the arrays (for example, position of bar 8 [counting from 0])
dict_array_x_y_z = {
    0: np.array([20, 24, 0]),
    #1: np.array([25.0, 25.0, 0.0])
    #2: np.array([216.1, 138.3, 23.]),
    #3: np.array([215.1, 42.5, 23.])
}

########################
#Scan for array
########################

dict_Scan = {

    #DEFAULT SINGLE RUN
    #0: [np.array([0, 0, 0]),channelList,energyThrList,t1ThrList,nloops,sleep],

    #POSITION SCAN: X
    0: [np.array([-15., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    1: [np.array([-12., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    2: [np.array([-9., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    3: [np.array([-6., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    4: [np.array([-4., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    5: [np.array([-3., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    6: [np.array([-2., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    7: [np.array([-1., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    8: [np.array([0., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    9: [np.array([1., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    10: [np.array([2, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    11: [np.array([3, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    12: [np.array([4, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    13: [np.array([6, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    14: [np.array([9, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    15: [np.array([12, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    16: [np.array([15, 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],

    #POSITION SCAN: Y
    17: [np.array([0., -15., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    18: [np.array([0., -12., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    19: [np.array([0., -9., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    20: [np.array([0., -6., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    21: [np.array([0., -4., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    22: [np.array([0., -3., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    23: [np.array([0., -2., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    24: [np.array([0., -1, 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    25: [np.array([0., 0., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    26: [np.array([0., 1., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    27: [np.array([0., 2., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    28: [np.array([0., 3., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    29: [np.array([0., 4., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    30: [np.array([0., 6., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    31: [np.array([0., 9., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    32: [np.array([0., 12., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
    33: [np.array([0., 15., 0.]),channelList,energyThrList,t1ThrList,nloops,sleep],
}

#print "Scan" , dict_Scan

###################################################################
########################### Run DAQ ############################### 
###################################################################

#
aMoverTop=XYMover(8820)
print (aMoverTop.estimatedPosition())
#aMoverTop.moveAbsoluteXY(10,22)
#aMoverTop.lightsOn() #no lights in array bench single

aMoverBottom=XYMover(8821)
print (aMoverBottom.estimatedPosition())
aMoverBottom.moveAbsoluteXY(24,24)

sys.stdout.flush()

#print (len(names),len(cfileNames))

#if len(cfileNames) != len(names):
#    print("Mismatch in number of arrays. Please check that you have specified the correct number of arrays in the config")
#    exit(-1)

try:
    for iarr,arr in enumerate(cfileNames):
        print ("")
        print ("=== Run daq for array ", iarr)
        print (arr)
        #if int(names[iarr])==-1:
        #    continue

        # edit dictionary with position of current array
        dict_Scan_current = copy.deepcopy(dict_Scan) 
        for kStep, kInfo in dict_Scan.items():   
            posModifier = dict_Scan[kStep][0]
            dict_Scan_current[kStep][0] = dict_array_x_y_z[iarr] + posModifier
            dict_Scan_current[kStep][0] = dict_Scan_current[kStep][0].round(1)
            #print dict_Scan_current

        # run sequence
        for ov in ov_values:
            for ovref in ovref_values:
                for gate in gate_values:
                    for kStep, kInfo in dict_Scan_current.items():

                        print ("++++ Centering Bar: "+str(kStep)+": X="+str(kInfo[0][0])+" Y="+str(kInfo[0][1])+" Z="+str(kInfo[0][2])+" Channels="+str(kInfo[1])+" +++++")
                        print (aMoverTop.moveAbsoluteXY(kInfo[0][0],kInfo[0][1]))
                        if ( aMoverTop.moveAbsoluteXY(kInfo[0][0],kInfo[0][1]) == "error"):
                            print ("== Out of range: skipping this position ==")
                            continue
                        print (aMoverTop.estimatedPosition())
                        print ("++++ Done +++++")                    
                        sys.stdout.flush()
                        #=== file name
                        #thisname = 'ARRAY'+str(names[iarr]).zfill(6)+"_IARR"+str(iarr)+"_POS"+str(kStep)+"_X"+str(kInfo[0][0])+"_Y"+str(kInfo[0][1])+"_Z"+str(kInfo[0][2])
                        thisname = 'ARRAY'+str(names).zfill(6)+"_IARR"+str(iarr)+"_POS"+str(kStep)+"_X"+str(kInfo[0][0])+"_Y"+str(kInfo[0][1])+"_Z"+str(kInfo[0][2])
                    

                        # Add the additional tag (e.g. REF_DAILY)
                        thisname = thisname + '_%s'%opt.tag

                        print(thisname)
                        #aMoverTop.lightsOff()
                        sys.stdout.flush()
                        time.sleep(5)
                        #============================================
                        #RUN(arr,"PED",t_ped,ov,ovref,gate,thisname,kInfo[1],"","",1,0,0.)
                        RUN(arr,"PHYS",t_phys,ov,ovref,gate,thisname,kInfo[1],kInfo[2],kInfo[3],kInfo[4],kInfo[5],t_ped_in_phys) 
                        #RUN(arr,"PED",t_ped,ov,ovref,gate,thisname,kInfo[1],"","",1,0,0.)
                        #============================================
                        time.sleep(5)
                        #aMoverTop.lightsOn()
                        sys.stdout.flush()

except KeyboardInterrupt:
    print("Ctrl+C pressed")

print ("Moving back to final position...")
aMoverTop.moveAbsoluteXY(20,48)
print (aMoverTop.estimatedPosition())
#print (aMoverTop.lightsOff())
aMoverBottom.moveAbsoluteXY(24,1)
print (aMoverBottom.estimatedPosition())
print ("++++ Run completed +++++")                    
sys.stdout.flush()
