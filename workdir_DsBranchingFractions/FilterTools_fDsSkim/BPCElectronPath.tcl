#
# $Id: BPCElectronPath.tcl,v 1.14 2003/10/10 16:07:24 fnc Exp $
# sets up BPCElectronPath for BetaPidCalib
#

# generic setup
# sourceFoundFile BetaPidCalib/BtaPidCalibOprSequenceTight.tcl

global BtaCandLists

set particle Electron
sourceFoundFile FilterTools/BPCPath.tcl

# Add lists of composites to write
lappend BtaCandLists BtaPideeeeCalibSampleOtherTrackElectronSkim BtaPideeeeCalibSampleElectronsElectronSkim
