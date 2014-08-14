# $Id: SkimTag3BodySequence.tcl,v 1.15 2007/12/20 04:19:37 bloom Exp $
#
# 3-body tag modules
# All modules are disabled by default. FilterTools/FilterToolsProcs.tcl
# has a some tcl procs that should be used to assemble a suitable path for 
# any skim that needs one of these modules (some depend on the 2body sequence).
#
# This sequence is NOT appended to TagProdCreateSequence by default!
#

global SkimTag3BodySequenceDefinition
if [expr ![info exists SkimTag3BodySequenceDefinition]] {
  set SkimTag3BodySequenceDefinition 1

  sequence create SkimTag3BodySequence

  # define useful procs
  sourceFoundFile EventTagTools/TagProcs.tcl

  # classic 3-body charmless 
  loadTagModule TagBCCC3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set GoodTracksVeryLoose
    3bodytagbit set BCCC3body
  }

  #Ks->pi0pi0 is needed by the following merger
  sourceFoundFile CompositionSequences/CompKs2Pi0Sequence.tcl
  sequence append SkimTag3BodySequence CompKs2Pi0Sequence

  # This merger is needed by the following tag module
  mod clone CompMergeLists ThreeBody3KsMergepi0pi
  loadTagModule ThreeBody3KsMergepi0pi SkimTag3BodySequence {
    inputList0 set KsDefault
    inputList1 set KsToPi0Pi0Loose
    outputList set ThreeBody3KsList
  }  
  loadTagModule TagBKsKsKs3body SkimTag3BodySequence {
    List1 set ThreeBody3KsList
    List2 set ThreeBody3KsList
    List3 set ThreeBody3KsList
    dEcut set 0.45
    3bodytagbit set BKsKsKs3body
  }

  loadTagModule TagBPi0Pi0Pi03body SkimTag3BodySequence {
    theList set pi0AllLoose
    minDaug set 3
    maxDaug set 4
    dEcut set 0.45
    3bodytagbit set BPi0Pi0Pi03body
    verbose set true
  }
  loadTagModule TagBCCPi03body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set pi0AllLoose
    3bodytagbit set BCCPi03body
  }
  loadTagModule TagBCCEta3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set etaDefault
    3bodytagbit set BCCEta3body
  }
  loadTagModule TagBCCKs3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set KsDefault
    3bodytagbit set BCCKs3body
  }
  loadTagModule TagBPi0Pi0Ks3body SkimTag3BodySequence {
    List1 set pi0AllLoose
    List2 set pi0AllLoose
    List3 set KsDefault
    dEcut set 0.45
    3bodytagbit set BPi0Pi0Ks3body
  }
  loadTagModule TagBPi0Pi0C3body SkimTag3BodySequence {
    List1 set pi0AllLoose
    List2 set pi0AllLoose
    List3 set GoodTracksVeryLoose
    dEcut set 0.45
    3bodytagbit set BPi0Pi0C3body
  }
  loadTagModule TagBKsKsC3body SkimTag3BodySequence {
    List1 set KsDefault
    List2 set KsDefault
    List3 set GoodTracksVeryLoose
    3bodytagbit set BKsKsC3body
  }
  loadTagModule TagBKsKsPi03body SkimTag3BodySequence {
    List1 set KsDefault
    List2 set KsDefault
    List3 set pi0AllLoose
    3bodytagbit set BKsKsPi03body
  }
  loadTagModule TagBCPi0Ks3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set pi0AllLoose
    List3 set KsDefault
    3bodytagbit set BCPi0Ks3body
  }

  # new CM2 3-body charmless 
  loadTagModule TagBCCC03a3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set GoodTracksVeryLoose
    3bodytagbit set BCCC03a3body
  }
  loadTagModule TagBKsKsKs03a3body SkimTag3BodySequence {
    List1 set ThreeBody3KsList
    List2 set ThreeBody3KsList
    List3 set ThreeBody3KsList
    dEcut set 0.45
    3bodytagbit set BKsKsKs03a3body
  }
  loadTagModule TagBPi0Pi0Pi003a3body SkimTag3BodySequence {
    List1 set pi0AllLoose
    List2 set pi0AllLoose
    List3 set pi0AllLoose
    dEcut set 0.45
    3bodytagbit set BPi0Pi0Pi003a3body
  }
  loadTagModule TagBCCPi003a3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set pi0AllLoose
    3bodytagbit set BCCPi003a3body
  }
  loadTagModule TagBCCEta03a3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set etaDefault
    3bodytagbit set BCCEta03a3body
  }
  loadTagModule TagBCCKs03a3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set KsDefault
    3bodytagbit set BCCKs03a3body
  }

  loadTagModule TagBCCKsPi0Pi003a3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set KsToPi0Pi0Loose
    3bodytagbit set BCCKsPi0Pi003a3body
  }

  loadTagModule TagBCCKsPi0Pi03body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set GoodTracksVeryLoose
    List3 set KsToPi0Pi0Loose
    3bodytagbit set BCCKsPi0Pi03body
  }

  loadTagModule TagBPi0Pi0Ks03a3body SkimTag3BodySequence {
    List1 set pi0AllLoose
    List2 set pi0AllLoose
    List3 set KsDefault
    dEcut set 0.45
    3bodytagbit set BPi0Pi0Ks03a3body
  }
  loadTagModule TagBPi0Pi0C03a3body SkimTag3BodySequence {
    List1 set pi0AllLoose
    List2 set pi0AllLoose
    List3 set GoodTracksVeryLoose
    dEcut set 0.45
    3bodytagbit set BPi0Pi0C03a3body
  }
  loadTagModule TagBKsKsC03a3body SkimTag3BodySequence {
    List1 set KsDefault
    List2 set KsDefault
    List3 set GoodTracksVeryLoose
    3bodytagbit set BKsKsC03a3body
  }
  loadTagModule TagBKsKsPi003a3body SkimTag3BodySequence {
    List1 set KsDefault
    List2 set KsDefault
    List3 set pi0AllLoose
    3bodytagbit set BKsKsPi003a3body
  }
  loadTagModule TagBCPi0Ks03a3body SkimTag3BodySequence {
    List1 set GoodTracksVeryLoose
    List2 set pi0AllLoose
    List3 set KsDefault
    3bodytagbit set BCPi0Ks03a3body
  }

  # TagKKKL is used only by BToKKKL skim... moved to that path
  # loadTagModule TagKKKL SkimTag3BodySequence
  # TagKsKsKIncl is used only by KsKsKIncl and BToKsKsKlDefault skims... 
  # moved to those path
  # loadTagModule TagKsKsKIncl SkimTag3BodySequence
  
  #
  # multi-list 3-body decays
  # These rely on tag bits from SkimTag2BodySequence
  #
  loadTagModule TagBToPPP SkimTag3BodySequence {
    # Ks
    tagList        set KsDefault
    tagLund        set K_S0
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagK_S0TwoBody_maxCmsP

    # Ks->pi0pi0
    tagList        set KsToPi0Pi0Loose
    tagLund        set K_S0
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagK_S0Pi0Pi0TwoBody_maxCmsP
    
    # pi0
    tagList        set pi0AllLoose
    tagLund        set pi0
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPi0TwoBody_maxCmsP
    
    # eta
    tagList        set etaDefault
    tagLund        set eta
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaTwoBody_maxCmsP
    
    # eta'
    tagList        set etaPrgDefault
    tagLund        set "eta'"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaPrhogTwoBody_maxCmsP
    
    # eta'->etapipi
    tagList        set etaPeppDefault
    tagLund        set "eta'"
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaPeppTwoBody_maxCmsP

    # phi
    tagList        set phiDefault
    tagLund        set "phi"
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPhiTwoBody_maxCmsP

    # omega
    tagList        set omegaDefault
    tagLund        set "omega"
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagOmegaTwoBody_maxCmsP

    # pi+
    tagList        set GoodTracksLoose
    tagLund        set "pi+"
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagCTwoBody_maxCmsP

    # K+
    tagList        set GoodTracksLooseKaonMass
    tagLund        set "K+"
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagKpTwoBody_maxCmsP

    # p+
    tagList        set GoodTracksLooseProtonMass
    tagLund        set "p+"
    tagMinMulti    set 2
    tagMaxMulti    set 2
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPTwoBody_maxCmsP

    #verbose set t

    EstarSumCut   set 4.99
    EstarSumCutHi set 5.59
    BmassCut      set 5.20
    tagbit        set BToPPP	
  }

  loadTagModule TagBToKstarXX SkimTag3BodySequence {
    # Ks
    tagList        set KsDefault
    tagLund        set K_S0
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    #tagWishList    set true
    tagWishList    set false
    tagName        set TagK_S0TwoBody_maxCmsP
    
    # phi
    tagList        set phiDefault
    tagLund        set "phi"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    #tagWishList    set true
    tagWishList    set false
    tagName        set TagPhiTwoBody_maxCmsP

    # K*0
    tagList        set KstarNeutralLoose
    tagLund        set "K*0"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagKstTwoBody_maxCmsP

    # K*+
    tagList        set KstarChrgLoose
    tagLund        set "K*+"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagKstpTwoBody_maxCmsP

    #verbose set t

    EstarSumCut   set 4.99
    EstarSumCutHi set 5.59
    BmassCut      set 5.20
    tagbit        set BToKstarXX
  }

  loadTagModule TagBToLambda0XX SkimTag3BodySequence {
    # Lambda
    tagList        set LambdaLoose
    tagLund        set "Lambda0"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set true
    tagWishList    set true
    tagName        set TagLambdaTwoBody_maxCmsP

    # Ks
    tagList        set KsDefault
    tagLund        set K_S0
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagK_S0TwoBody_maxCmsP
    
    # K+
    tagList        set GoodTracksLooseKaonMass
    tagLund        set K+
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagKpTwoBody_maxCmsP

    # p+
    tagList        set GoodTracksLooseProtonMass
    tagLund        set "p+"
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPTwoBody_maxCmsP

    # pi+
    tagList        set GoodTracksLoose
    tagLund        set "pi+"
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagCTwoBody_maxCmsP

    # pi0
    tagList        set pi0AllLoose
    tagLund        set pi0
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPi0TwoBody_maxCmsP
    
    # eta
    tagList        set etaDefault
    tagLund        set eta
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaTwoBody_maxCmsP
    
    # eta'->etapipi
    tagList        set etaPeppDefault
    tagLund        set "eta'"
    tagMinMulti    set 0
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaPeppTwoBody_maxCmsP

    #verbose set t

    EstarSumCut   set 4.99
    EstarSumCutHi set 5.59
    BmassCut      set 5.20
    tagbit        set BToLambda0XX
  }

  #
  # multi-list 4-body decays
  # These rely on tag bits from SkimTag2BodySequence
  #
  loadTagModule TagBToPPPP SkimTag3BodySequence {
    # Ks
    tagList        set KsDefault
    tagLund        set K_S0
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagK_S0TwoBody_maxCmsP

    # Ks->pi0pi0
    #tagList        set KsToPi0Pi0Loose
    #tagLund        set K_S0
    #tagMinMulti    set 1
    #tagMaxMulti    set 1
    #tagRequired    set false
    #tagWishList    set true
    #tagName        set TagK_S0Pi0Pi0TwoBody_maxCmsP
    
    # pi0
    tagList        set pi0AllLoose
    tagLund        set pi0
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPi0TwoBody_maxCmsP
    
    # eta
    tagList        set etaDefault
    tagLund        set eta
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaTwoBody_maxCmsP
    
    # eta'
    tagList        set etaPrgDefault
    tagLund        set "eta'"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaPrhogTwoBody_maxCmsP
    
    # eta'->etapipi
    tagList        set etaPeppDefault
    tagLund        set "eta'"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagEtaPeppTwoBody_maxCmsP

    # phi
    tagList        set phiDefault
    tagLund        set "phi"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagPhiTwoBody_maxCmsP

    # omega
    tagList        set omegaDefault
    tagLund        set "omega"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagOmegaTwoBody_maxCmsP

    # pi+
    tagList        set GoodTracksLoose
    tagLund        set "pi+"
    tagMinMulti    set 1
    tagMaxMulti    set 3
    tagRequired    set false
    tagWishList    set true
    tagName        set TagCTwoBody_maxCmsP

    # K+
    tagList        set GoodTracksLooseKaonMass
    tagLund        set "K+"
    tagMinMulti    set 1
    tagMaxMulti    set 1
    tagRequired    set false
    tagWishList    set true
    tagName        set TagKpTwoBody_maxCmsP

    # p+
    #tagList        set GoodTracksLooseProtonMass
    #tagLund        set "p+"
    #tagMinMulti    set 2
    #tagMaxMulti    set 2
    #tagRequired    set false
    #tagWishList    set true
    #tagName        set TagPTwoBody_maxCmsP

    #verbose set t

    EstarSumCut   set 4.99
    EstarSumCutHi set 5.59
    BmassCut      set 5.20
    ThrustCut     set 0.8
    UseNeutrals   set true
    tagbit        set BToPPPP	
  }


  # This list is not used anymore, but we leave it here anyways
  lappend Tag3Bmodules TagBCCC3body  TagBKsKsKs3body \
      TagBPi0Pi0Pi03body TagBCCPi03body TagBCCEta3body TagBCCKs3body \
      TagBPi0Pi0Ks3body TagBPi0Pi0C3body TagBKsKsC3body TagBKsKsPi03body \
      TagBCPi0Ks3body TagBCCC03a3body TagBKsKsKs03a3body TagBPi0Pi0Pi003a3body \
      TagBCCPi003a3body TagBCCEta03a3body TagBCCKs03a3body TagBCCKsPi0Pi003a3body \
      TagBCCKsPi0Pi03body TagBPi0Pi0Ks03a3body TagBPi0Pi0C03a3body \
      TagBKsKsC03a3body TagBKsKsPi003a3body TagBCPi0Ks03a3body \
      TagBToPPP TagBToKstarXX TagBToLambda0XX TagBToPPPP
#     TagKKKL TagKsKsKIncl

# all modules are disabled by default via the loadTagModule proc now
#  foreach module $Tag3Bmodules {catch {module disable $module}}

}
