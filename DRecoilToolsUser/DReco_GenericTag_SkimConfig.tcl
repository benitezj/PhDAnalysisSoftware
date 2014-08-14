
#-------------------------------------
#Create ChmDrecoSequence and Refine the tag modes before using them
#-------------------------------------
sourceFoundFile DRecoilToolsUser/DReco_RefinedLists.tcl
path append Everything DRecoRefinedSequence

#--------------------------------------------------
#configure the module for mass window, vtx prob. and p* cuts.
#--------------------------------------------------
module talk DRecoTagInterface
  #######################################
  ##Cut at optimized mass window
  #######################################
  TagList1 set DRecoRefinedD0ToKPi
  TagListFit1 set true
  TagListMass1 set 1.8638
  TagListMassCut1 set 0.016
  TagListPstarCut1 set 2
  TagListProbminCut1 set 0.000251189
   
  TagList2 set DRecoRefinedD0ToKPiPi0
  TagListFit2 set true
  TagListMass2 set 1.86256
  TagListMassCut2 set 0.023
  TagListPstarCut2 set 2
  TagListProbminCut2 set 6.30957e-05
   
  TagList3 set DRecoRefinedD0ToK3Pi
  TagListFit3 set true
  TagListMass3 set 1.86369
  TagListMassCut3 set 0.009
  TagListPstarCut3 set 2
  TagListProbminCut3 set 0.0001
   
  TagList4 set DRecoRefinedD0ToKsPiPi
  TagListFit4 set true
  TagListMass4 set 1.86388
  TagListMassCut4 set 0.01
  TagListPstarCut4 set 2
  TagListProbminCut4 set 1e-05
   
  TagList5 set DRecoRefinedD0ToK3PiPi0
  TagListFit5 set true
  TagListMass5 set 1.8631
  TagListMassCut5 set 0.012
  TagListPstarCut5 set 3
  TagListProbminCut5 set 1e-05
   
  TagList6 set DRecoRefinedD0ToKsPiPiPi0
  TagListFit6 set true
  TagListMass6 set 1.86267
  TagListMassCut6 set 0.017
  TagListPstarCut6 set 2.8
  TagListProbminCut6 set 2.51189e-05
   
  TagList7 set DRecoRefinedDPlusToKPiPi
  TagListFit7 set true
  TagListMass7 set 1.86854
  TagListMassCut7 set 0.011
  TagListPstarCut7 set 2
  TagListProbminCut7 set 0.000630957
   
  TagList8 set DRecoRefinedDPlusToKPiPiPi0
  TagListFit8 set true
  TagListMass8 set 1.86717
  TagListMassCut8 set 0.014
  TagListPstarCut8 set 2.9
  TagListProbminCut8 set 1e-05
   
  TagList9 set DRecoRefinedDPlusToKsPi
  TagListFit9 set true
  TagListMass9 set 1.86895
  TagListMassCut9 set 0.011
  TagListPstarCut9 set 2
  TagListProbminCut9 set 1e-05
   
  TagList10 set DRecoRefinedDPlusToKsPiPi0
  TagListFit10 set true
  TagListMass10 set 1.86785
  TagListMassCut10 set 0.022
  TagListPstarCut10 set 2.8
  TagListProbminCut10 set 1e-05
   
  TagList11 set DRecoRefinedDPlusToKs3Pi
  TagListFit11 set true
  TagListMass11 set 1.86867
  TagListMassCut11 set 0.009
  TagListPstarCut11 set 2.8
  TagListProbminCut11 set 0.000158489
   

#   TagList12 set DRecoRefinedDsToKsK
#   TagListFit12 set true
#   TagListMass12 set 1.96761
#   TagListMassCut12 set 0.012
#   TagListPstarCut12 set 2.5
#   TagListProbminCut12 set 1e-05
   
#   TagList13 set DRecoRefinedDsToPhiPi
#   TagListFit13 set true
#   TagListMass13 set 1.96716
#   TagListMassCut13 set 0.016
#   TagListPstarCut13 set 2.4
#   TagListProbminCut13 set 1e-05
   
#   TagList14 set DRecoRefinedDsToPhiRhoC
#   TagListFit14 set true
#   TagListMass14 set 1.96624
#   TagListMassCut14 set 0.022
#   TagListPstarCut14 set 2.7
#   TagListProbminCut14 set 1e-05
   
#   TagList15 set DRecoRefinedDsToKsKPiPi
#   TagListFit15 set true
#   TagListMass15 set 1.96739
#   TagListMassCut15 set 0.008
#   TagListPstarCut15 set 3
#   TagListProbminCut15 set 1e-05
   
#   TagList16 set DRecoRefinedDsToEtaPPi
#   TagListFit16 set true
#   TagListMass16 set 1.96772
#   TagListMassCut16 set 0.015
#   TagListPstarCut16 set 2.9
#   TagListProbminCut16 set 0.01
   
#   TagList17 set DRecoRefinedDsToPhiRho0pi
#   TagListFit17 set true
#   TagListMass17 set 1.96792
#   TagListMassCut17 set 0.008
#   TagListPstarCut17 set 2.7
#   TagListProbminCut17 set 1e-05
   
#   TagList18 set DRecoRefinedDsToKKstar0
#   TagListFit18 set true
#   TagListMass18 set 1.96741
#   TagListMassCut18 set 0.01
#   TagListPstarCut18 set 2.7
#   TagListProbminCut18 set 1e-05
   


  TagList19 set DRecoRefinedLambdaCTopKpi
  TagListFit19 set true
  TagListMass19 set 2.2857
  TagListMassCut19 set 0.009
  TagListPstarCut19 set 2
  TagListProbminCut19 set 0.000251189
   
  TagList20 set DRecoRefinedLambdaCTopKpipi0
  TagListFit20 set true
  TagListMass20 set 2.28433
  TagListMassCut20 set 0.015
  TagListPstarCut20 set 2.5
  TagListProbminCut20 set 1.58489e-05
   
  TagList21 set DRecoRefinedLambdaCTopKs
  TagListFit21 set true
  TagListMass21 set 2.28588
  TagListMassCut21 set 0.014
  TagListPstarCut21 set 2
  TagListProbminCut21 set 6.30957e-05
   
  TagList22 set DRecoRefinedLambdaCTopKspipi
  TagListFit22 set true
  TagListMass22 set 2.28563
  TagListMassCut22 set 0.007
  TagListPstarCut22 set 2.5
  TagListProbminCut22 set 1.58489e-05
   
  TagList23 set DRecoRefinedLambdaCToLzpi
  TagListFit23 set true
  TagListMass23 set 2.2863
  TagListMassCut23 set 0.014
  TagListPstarCut23 set 2.1
  TagListProbminCut23 set 1e-05
   
  TagList24 set DRecoRefinedLambdaCToLzpipi0
  TagListFit24 set true
  TagListMass24 set 2.28373
  TagListMassCut24 set 0.028
  TagListPstarCut24 set 2
  TagListProbminCut24 set 1e-05
   
  TagList25 set DRecoRefinedLambdaCToLzpipipi
  TagListFit25 set true
  TagListMass25 set 2.28555
  TagListMassCut25 set 0.013
  TagListPstarCut25 set 2.3
  TagListProbminCut25 set 6.30957e-05
   
  TagList26 set DRecoRefinedLambdaCToSzpi
  TagListFit26 set true
  TagListMass26 set 2.28587
  TagListMassCut26 set 0.009
  TagListPstarCut26 set 2.1
  TagListProbminCut26 set 6.30957e-05
exit 

