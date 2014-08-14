#
#  Define the DRecoDmesonTagSkim path
#

path create DRecoDmesonTagPath
sourceFoundFile FilterTools/DmesonCommonSequence.tcl
mod clone TagFilterMomentumCut DmesonWordFilter
setDmesonDecayModes DmesonWordFilter
path append DRecoDmesonTagPath DmesonWordFilter

module talk DmesonWordFilter
  orListCut clear
#D0ToKPiLoosePID       D0 -> K-pi+ (PID)
  orListCut set 99
#D0MainLoosePID        D0 -> K-pi+, K-pi+pi0, K-pi+pi-pi+, K0spi+pi- (PID)
  orListCut set 8 
#D0CabibboLoosePID     D0 -> K+K- , pi+pi- (PID)
  orListCut set 99
#D0LoosePID            All D0 decay modes above (PID)
  orListCut set 99
#Dc3prongLoosePID      D+ -> K0spi+, K0sK+, K+K-pi+, pi+pi-pi+ (PID)
  orListCut set 8 
#DcToKsPiPi0LoosePID   D+ -> K0spi+pi0
  orListCut set 8 
#DcToKsPiPiPiLoosePID  D+ -> K0spi+pi-pi+
  orListCut set 8 
#DcToKPiPiPi0LoosePID  D+ -> K-pi+pi+pi0 (PID)
  orListCut set 8 
#DcToKsKPiPiLoosePID   D+ -> K0sK-pi+pi+
  orListCut set 99
#DcToKPiPiLoosePID     D+ -> K-pi+pi+
  orListCut set 99
  doHisto set true
exit

