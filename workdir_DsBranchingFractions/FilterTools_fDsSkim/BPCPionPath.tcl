#
# $Id: BPCPionPath.tcl,v 1.15 2003/10/10 16:07:24 fnc Exp $
# sets up BPCPionPath for BetaPidCalib
#

# generic setup
# sourceFoundFile BetaPidCalib/BtaPidCalibOprSequenceTight.tcl

global BtaCandLists

set particle Pion
sourceFoundFile FilterTools/BPCPath.tcl

# Add lists of composites to write
lappend BtaCandLists BtaPidKsSamplePionsPionSkim BtaPidTau31SamplePionsPionSkim BtaPidDstarSamplePionPionSkim BtaPidDstarSampleDstarPionSkim BtaPidgg4piCalibSamplePionsPionSkim
