
#-------------------------------------
#creat decay modes: According to Shanes skim 
#-------------------------------------
FwkCfgVar writeChmTagNtuples 0
#sourceFoundFile CharmTagTools/ChmTagLists.tcl
module disable ChmPi0Analysis
module disable ChmMiniAnalysisDeltaM
module disable ChmMiniAnalysis

# ##++++++++++++++++++++++++++++++++++++++
# ### Merge the D0 modes
# ##----------------------------------------
# createsmpmerger DRecoSequence DRecoD0Main {
#     inputListNames    set "ChmD0ToKPi"
#     inputListNames    set "ChmD0ToKPiPi0"
#     inputListNames    set "ChmD0ToK3Pi"
#     inputListNames    set "ChmD0ToK3PiPi0Tight" 
#     inputListNames    set "ChmD0ToKShortPiPi"
#     inputListNames    set "ChmD0ToKShortPiPiPi0" 
# }

# ##++++++++++++++++++++++++++++++++++++++
# ### Merge the D+ modes
# ##++++++++++++++++++++++++++++++++++++++
# createsmpmerger DRecoSequence DRecoDPlusMain {
#     inputListNames    set "ChmDPlusToKPiPi"
#     inputListNames    set "ChmDPlusToKPiPiPi0"
#     inputListNames    set "ChmDPlusToKShortPi"
#     inputListNames    set "ChmDPlusToKShortPiPi0"
#     inputListNames    set "ChmDPlusToKShort3Pi"
# }

# seq append DRecoSequence ChmD0ToKPi
# seq append DRecoSequence ChmD0ToKPiPi0
# seq append DRecoSequence ChmD0ToK3Pi
# seq append DRecoSequence ChmD0ToK3PiPi0Tight
# seq append DRecoSequence ChmD0ToKShortPiPi
# seq append DRecoSequence ChmD0ToKShortPiPiPi0
# seq append DRecoSequence ChmDPlusToKPiPi
# seq append DRecoSequence ChmDPlusToKPiPiPi0
# seq append DRecoSequence ChmDPlusToKShortPi
# seq append DRecoSequence ChmDPlusToKShortPiPi0
# seq append DRecoSequence ChmDPlusToKShort3Pi


sourceFoundFile CharmTagTools/Chm_DaughterLists.tcl
sourceFoundFile CharmTagTools/Chm_k3pi.tcl
sourceFoundFile CharmTagTools/Chm_k3pipi0.tcl
sourceFoundFile CharmTagTools/Chm_kpi.tcl
sourceFoundFile CharmTagTools/Chm_kpipi.tcl
sourceFoundFile CharmTagTools/Chm_kpipi0.tcl
sourceFoundFile CharmTagTools/Chm_kpipipi0.tcl
sourceFoundFile CharmTagTools/Chm_ks3pi.tcl
sourceFoundFile CharmTagTools/Chm_kspi.tcl
sourceFoundFile CharmTagTools/Chm_kspipi.tcl
sourceFoundFile CharmTagTools/Chm_kspipi0.tcl
sourceFoundFile CharmTagTools/Chm_kspipipi0.tcl

#--------------------------------------------------
#configure Modules
#--------------------------------------------------
module talk DRecoTagInterface  
    
#      #D0: Kpi,Kpipi0,K3pi,Kspipi,K3pipi0,Kspipipi0
#      TagList1 set DRecoD0Main
#      TagListFit1 set true
#      TagListMass1 set 1.8648
#      TagListMassCut1 set .015
#      TagListPstarCut1 set 2.0

#      #D+: DcToKPiPi, DcToKPiPiPi0, DcToKsPi, DcToKsPiPi0, DcToKs3Pi
#      TagList2 set DRecoDPlusMain
#      TagListFit2 set true
#      TagListMass2 set 1.8692
#      TagListMassCut2 set .015
#      TagListPstarCut2 set 2.0

      TagList1 set ChmD0ToKPi
      TagListFit1 set true
      TagListMass1 set 1.86361  
      TagListMassCut1 set .024
      TagListPstarCut1 set 2.0

      TagList2 set ChmD0ToKPiPi0
      TagListFit2 set true
      TagListMass2 set 1.86185  
      TagListMassCut2 set .042 
      TagListPstarCut2 set 2.0

      TagList3 set ChmD0ToK3Pi
      TagListFit3 set true
      TagListMass3 set 1.86384  
      TagListMassCut3 set .018 
      TagListPstarCut3 set 2.0

      TagList4 set ChmD0ToK3PiPi0Tight
      TagListFit4 set true
      TagListMass4 set 1.86258  
      TagListMassCut4 set .033
      TagListPstarCut4 set 2.0

      TagList5 set ChmD0ToKShortPiPi
      TagListFit5 set true
      TagListMass5 set 1.86356  
      TagListMassCut5 set .018
      TagListPstarCut5 set 2.0

      TagList6 set ChmD0ToKShortPiPiPi0
      TagListFit6 set true
      TagListMass6 set 1.8628  
      TagListMassCut6 set  .03
      TagListPstarCut6 set 2.0

      TagList7 set ChmDPlusToKPiPi
      TagListFit7 set true
      TagListMass7 set 1.86689 
      TagListMassCut7 set .036
      TagListPstarCut7 set 2.0

      TagList8 set ChmDPlusToKPiPiPi0
      TagListFit8 set true
      TagListMass8 set 1.86735  
      TagListMassCut8 set .042
      TagListPstarCut8 set 2.0

      TagList9 set ChmDPlusToKShortPi
      TagListFit9 set true
      TagListMass9 set 1.86848 
      TagListMassCut9 set .021 
      TagListPstarCut9 set 2.0

      TagList10 set ChmDPlusToKShortPiPi0
      TagListFit10 set true
      TagListMass10 set 1.86903  
      TagListMassCut10 set .021
      TagListPstarCut10 set 2.0

      TagList11 set ChmDPlusToKShort3Pi
      TagListFit11 set true
      TagListMass11 set 1.86864  
      TagListMassCut11 set .018
      TagListPstarCut11 set 2.0

exit




