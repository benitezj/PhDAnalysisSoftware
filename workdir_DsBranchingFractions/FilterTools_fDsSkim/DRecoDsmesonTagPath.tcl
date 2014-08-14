#
#  Define the DRecoDsmesonTagSkim path
#

path create DRecoDsmesonTagPath
sourceFoundFile FilterTools/DsmesonCommonSequence.tcl
mod clone TagFilterMomentumCut DsmesonWordFilter
setDsmesonDecayModes DsmesonWordFilter
path append DRecoDsmesonTagPath DsmesonWordFilter

module talk DsmesonWordFilter
  orListCut clear
  #DsToPhiPiLoosePID       Ds -> Phi pi+      
  orListCut set 99
  #Ds3prongLoosePID        Ds -> Ks K+, K*0 K+, K*0 pi+, f0 pi+, Phi K+
  orListCut set 7
  #DsToPhiPiPiPiLoosePID   Ds -> Phi pi+ pi- pi+
  orListCut set 7
  #DsToPhiRhoCLoosePID     Ds -> Phi rho+
  orListCut set 7
  #DsToKsKPiPiLoosePID     Ds -> Ks K- pi+ pi+, Ks K+ pi+ pi-
  orListCut set 7
  #DsToEtaPiLoosePID       Ds -> eta pi+
  orListCut set 7
  #DsToEtaRhoCLoosePID     Ds -> eta rho+
  orListCut set 7
  #DsToEtaPPiLoosePID      Ds -> eta' pi+
  orListCut set 7
  #DsToEtaPRhoCLoosePID    Ds -> eta' rho+
  orListCut set 7
  #LambdaCToPKPiLoosePID   LambdaC -> p K- pi+ 
  orListCut set 99
  doHisto set true
exit

