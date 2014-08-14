#
# defines D*+ to D0pi+, D0 to KS,KL pi+ pi- & cc decays
#


path create D0ToK0PiPiSkimPath

# only run on MultiHadrons
path append D0ToK0PiPiSkimPath FilterBGFMultiHadron

path append D0ToK0PiPiSkimPath D0ToK0PiPiSkim

talkto D0ToK0PiPiSkim {
production set true
}

#global BtaCandLists
#lappend BtaCandLists DstarToKLList
#lappend BtaCandLists DstarToKSList

#mod talk D0ToK0PiPiSkim
#      pionList set piLHLoose
#      vtxFitAlgo set TreeFitter
#      vtxFitMode set Standard
#      slowPiMomLo set 0.05
#      slowPiMomHi set 1.65
#      slowPiRdocaMax set 0.2
#      slowPiZdocaMax set 3.0
#      slowPiSvtHit set 6
#      slowPiDchHit set 11
#      deltaMassCutHi set 0.16
#      deltaMassCutLo set 0.138
#      pStarCut set 0.5
#      dxymax set 0.1
#      dzmax set 2.0
#      pkmin set 0.0
#      pkmax set 4.0
#      pksmin set 0.0
#      pksmax set 4.0
#      mkstarmin set 0.75
#      mkstarmax set 1.05
#      ecalmin set 0.0
#      eksmin set 0.0
#      helmin set 0.0
#exit
