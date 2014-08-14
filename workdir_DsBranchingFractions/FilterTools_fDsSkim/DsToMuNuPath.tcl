#
# $Id: DsToMuNuPath.tcl,v 1.1 2005/11/16 23:51:46 fwinkl Exp $
#
#
#  Define the DsToMuNu path
#
#

path create DsToMuNuPath

# append tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagDsToMuNu DsToMuNuPath {
# D0
    CheckLists set D0ToKPiLoosePID              
    CheckLists set D0ToKPiPi0LoosePID
    CheckLists set D0ToK3PiLoosePID
# D+-
    CheckLists set DcToKPiPiDefault
    CheckLists set DcToKPiPiPi0Default
    CheckLists set DcToKsPiSemiLoose
    CheckLists set DcToKsPiPi0Loose
    CheckLists set DcToKsPiPiPiDefault
    CheckLists set DcToKKPiDefault
    CheckLists set DcToKsKLoosePID
# Ds
    CheckLists set DsToPhiPiTightPID
    CheckLists set DsToKsKLoosePID
    CheckLists set DsToPhiRhoCLoosePID
    CheckLists set DsToKsKPiPisupLoosePID
    CheckLists set DsToKsKPiPifavLoosePID
# D*+-
    CheckLists set DstarChrgKLooseRSPID
    CheckLists set DstarNeutralKLoosePID
# D*0
    CheckLists set Dstar0ChrgKDefault
# Ds*+
    CheckLists set DsstarDefault
# Ds* -> gammaDs -> phipi control sample
    DsPhiPiList set DsToPhiPiDefault
# D*0 -> gammaD0 -> Kpi control sample
    DKPiList set D0ToKPiLoosePID
# Muons and Electrons
    MuonList set muNNLoose
    ElecList set eBremRecoELoose
}

# filter on tag bits
mod clone TagFilterByName DsToMuNuFilter
addTagModule DsToMuNuFilter DsToMuNuPath {
  orList set DsToMuNuCharmTagAndLept
  orList set DsToMuNuCharmTagAndDsPhiPi
  orList set DsToMuNuCharmTagAndDKPi
}