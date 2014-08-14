#
# $Id: B0ToDstarA1CLoosePath.tcl,v 1.4 2005/05/30 05:44:05 hulsberg Exp $
#
# defines B0->D*a1 skim
#
global BtaCandLists

global writeUsrDataForSkim
set writeUsrDataForSkim 1
set BtaCandLists B0ToDstarA1CLoose
global UsrBCandLists
set UsrBCandLists $BtaCandLists

path create B0ToDstarA1CLoosePath

mod clone TagFilterByName B0ToDstarA1CLooseFilter
path append B0ToDstarA1CLoosePath  B0ToDstarA1CLooseFilter
talkto  B0ToDstarA1CLooseFilter {
#common tag
    andList set isBCMultiHadron
}

module clone B0ToDstarA1C_Default B0ToDstarA1C_Loose
path append B0ToDstarA1CLoosePath B0ToDstarA1C_Loose
talkto B0ToDstarA1C_Loose {
outputList         set B0ToDstarA1CLoose
inputList0         set DstarToD0PiMainDefault
inputList1         set a1CLoose
doVertex           set false
doKin              set false
doMass             set false
verbose            set false
massCutLoose       set 4.5
massCutUpperLoose  set 5.5
massCut            set 4.5
massCutUpper       set 5.5
deltaERawCut       set 0.2
r2Cut              set 999
openingAngleCut    set 999
evtShapeTrkList    set GoodTracksVeryLoose
evtShapeNeuList    set GoodPhotonLoose
maxCandidates      set 550
maxCandComplain    set false
beamMasshiCut      set 5.35
beamMassloCut      set 5.18
}

path append B0ToDstarA1CLoosePath B0ToDstarA1CLoose
talkto B0ToDstarA1CLoose {
  inputList set B0ToDstarA1CLoose
}
