#
# $Id: BPCPath.tcl,v 1.5 2004/04/01 10:30:57 fnc Exp $
# sets up a filter path for BetaPidCalib
#

# filters for this path
sourceFoundFile BetaPidCalib/Bta${particle}TagSelectSequence.tcl
sourceFoundFile BetaPidCalib/Bta${particle}MicroSelectSequence.tcl

path create BPC${particle}Path 

# tag sequence
path append BPC${particle}Path Bta${particle}TagSelectSequence

# now read micro and filter
#path append BPC${particle}Path MicroSource

# its own filter
path append BPC${particle}Path Bta${particle}MicroSelectSequence

# all singing-and-dancing sequence from BetaPidCalib
# Vuko Brigljevic X-15-01: switch this off, since already 
#                          run earlier in skim job path
# path append BPC${particle}Path BtaPidCalibOprSequenceTight

# and disable ntuples
module disable ${particle}TagPlots 
