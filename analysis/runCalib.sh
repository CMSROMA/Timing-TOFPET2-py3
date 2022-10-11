#!/bin/sh

for iarr in `seq 0 3`; do python analysis/makeCalib.py -i /data/cmsdaq/LYSOMULTIARRAYNEWLAB4ARRAYS/RESULTS/ -o /tmp -n ${iarr}; done
hadd -f /home/cmsdaq/Workspace/TOFPET/Timing-TOFPET2-py3/data/stability.root /tmp/stability_IARR*.root
hadd -f /home/cmsdaq/Workspace/TOFPET/Timing-TOFPET2-py3/data/intercalib.root /tmp/calib_IARR*.root
python analysis/finalizeCalib.py
