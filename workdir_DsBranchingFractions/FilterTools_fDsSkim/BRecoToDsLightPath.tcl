#
# $Id: BRecoToDsLightPath.tcl,v 1.14 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BRecoToDsLightPath tag filter path
#
global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BRecoToDsLightPath

# only run on MultiHadrons
path append BRecoToDsLightPath FilterBGFMultiHadron

# create tag bits
sequence create TagBRecoToDsLight

useTagFromList BchToDsPi0 TagBRecoToDsLight

makeTagFromList B0ToDsPiDefault          B0ToDsPi            TagBRecoToDsLight  ;# B0->Dspi
makeTagFromList B0ToDsstarPiDefault      B0ToDSstarPi        TagBRecoToDsLight  ;# B0->Ds*pi
makeTagFromList B0ToDsKDefault           B0ToDsK             TagBRecoToDsLight  ;# B0->DsK
makeTagFromList B0ToDsstarKDefault       B0ToDSstarK         TagBRecoToDsLight  ;# B0->Ds*K
makeTagFromList BchToDsstarPi0Default    BchToDSstarPi0      TagBRecoToDsLight  ;# B->Ds*pi0
makeTagFromList BchToDsPhiDefault        BchToDsPhi          TagBRecoToDsLight  ;# B->Dsphi
makeTagFromList BchToDsstarPhiDefault    BchToDSstarPhi      TagBRecoToDsLight  ;# B->Ds*phi
makeTagFromList B0ToDsRhoCDefault        B0ToDsRhoC          TagBRecoToDsLight  ;# B0->Dsrho
makeTagFromList B0ToDsstarRhoCDefault    B0ToDSstarRhoC      TagBRecoToDsLight  ;# B0->Ds*rho
makeTagFromList BchToDsRho0Default       BchToDsRho0         TagBRecoToDsLight  ;# B->Dsrho0
makeTagFromList BchToDsstarRho0Default   BchToDSstarRho0     TagBRecoToDsLight  ;# B->Ds*rho0
makeTagFromList B0ToDsA1CDefault         B0ToDsA1C           TagBRecoToDsLight  ;# B0->Dsa1
makeTagFromList B0ToDsstarA1CDefault     B0ToDSstarA1C       TagBRecoToDsLight  ;# B0->Ds*a1
makeTagFromList BchToDsA10Default        BchToDsA10          TagBRecoToDsLight  ;# B->Dsa10
makeTagFromList BchToDsstarA10Default    BchToDSstarA10      TagBRecoToDsLight  ;# B->Ds*a10
makeTagFromList B0ToDsKstarCDefault      B0ToDsKstarC        TagBRecoToDsLight  ;# B0->DsK*
makeTagFromList B0ToDsstarKstarCDefault  B0ToDSstarKstarC    TagBRecoToDsLight  ;# B0->Ds*K*
makeTagFromList BchToDsKstar0Default     BchToDsKstar0       TagBRecoToDsLight  ;# B->DsK*0
makeTagFromList BchToDsstarKstar0Default BchToDSstarKstar0   TagBRecoToDsLight  ;# B->Ds*K*0

path append BRecoToDsLightPath TagBRecoToDsLight


# clone and customize filter module
mod clone TagFilterByName BRecoToDsLightFilter
path append BRecoToDsLightPath BRecoToDsLightFilter
talkto BRecoToDsLightFilter {
    orList set B0ToDsPi
    orList set B0ToDSstarPi
    orList set B0ToDsK
    orList set B0ToDSstarK
    orList set BchToDsPi0
    orList set BchToDSstarPi0
    orList set BchToDsPhi
    orList set BchToDSstarPhi
    orList set B0ToDsRhoC
    orList set B0ToDSstarRhoC
    orList set BchToDsRho0
    orList set BchToDSstarRho0
    orList set B0ToDsKstarC
    orList set B0ToDSstarKstarC
    orList set BchToDsKstar0
    orList set BchToDSstarKstar0
    orList set B0ToDsA1C
    orList set B0ToDSstarA1C
    orList set BchToDsA10
    orList set BchToDSstarA10
#
    assertIfMissing set true
}

# Add lists of composites to write
lappend BtaCandLists B0ToDsPiDefault B0ToDsstarPiDefault B0ToDsKDefault B0ToDsstarKDefault BchToDsPi0Default BchToDsstarPi0Default BchToDsPhiDefault BchToDsstarPhiDefault B0ToDsRhoCDefault B0ToDsstarRhoCDefault BchToDsRho0Default BchToDsstarRho0Default B0ToDsKstarCDefault B0ToDsstarKstarCDefault BchToDsKstar0Default BchToDsstarKstar0Default B0ToDsA1CDefault B0ToDsstarA1CDefault BchToDsA10Default BchToDsstarA10Default

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
set writeUsrDataForSkim 1
