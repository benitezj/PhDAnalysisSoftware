#
### $Id: SkimCompProdSequence.tcl,v 1.14 2007/07/27 23:11:28 cschill Exp $
#
# This sequence is meant to hold user-defined sequences useful for skim definition
#

global SkimCompProdSequence
if [ expr ! [info exists SkimCompProdSequence ] ] {

  set SkimCompProdSequence 1
  sequence create SkimCompProdSequence

  sourceFoundFile FilterTools/FilterToolsProcs.tcl

  # Sequence for EEToKKpi, EEToKKpipi, EEToPSPS, EEToKKEta skims
  addSkimSequence EEToLightMesonsSequence FilterTools/EEToLightMesonsSequence.tcl

  # Sequence for DcToKpipiPrompt_DCS skim
  addSkimSequence DcToKpipiPrompt_DCSSequence FilterTools/DcToKpipiPrompt_DCSSequence.tcl

  # Sequence for BtoXGamma skims
  addSkimSequence BtoXGammaSequence FilterTools/BtoXGammaSequence.tcl

  # Sequence for BToXsll skims
  addSkimSequence BToXsllSequence FilterTools/BToXsllSequence.tcl

  # Sequence for InclPhiTo3pi and BtoPhiKPhiTo3pi skims
  addSkimSequence BtoPhiKPhiTo3piSequence FilterTools/BtoPhiKPhiTo3piSequence.tcl 

}
