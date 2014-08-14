# $Id: SkimTag2BodySequence.tcl,v 1.6 2007/12/18 13:49:30 marchior Exp $
#
# 2-body tag modules
# All modules are disabled by default. Besides individual skims that should
# append/enable the specific module they need to their path, this sequence
# is also needed by SkimTag3BodySequence.
#
# This sequence is NOT appended to TagProdCreateSequence by default !
#
global SkimTag2BodySequenceDefinition
if [expr ![info exists SkimTag2BodySequenceDefinition]] {
  set SkimTag2BodySequenceDefinition 1

  sequence create SkimTag2BodySequence

  # define useful procs
  sourceFoundFile EventTagTools/TagProcs.tcl
  #
  # tag two body modules (Gautier)
  #
  loadTagModule TagK_S0TwoBody SkimTag2BodySequence {
    particleType set "K_S0"
    inputList    set "KsDefault"
    # recoiling particles
    recoilPartList   set "J/psi"
    recoilPartList   set "psi(2S)"
    recoilPartList   set "eta_c"
    recoilPartList   set "h_c"
    recoilPartList   set "chi_c0"
    recoilPartList   set "chi_c1"
    recoilPartList   set "chi_c2"
    recoilPartList   set "eta_c(2S)"
    # customization (verbose : only at initialization and summary)
    verbose       set false
    tolerance     set 0.2
  }

  #Ks->pi0pi0 is needed by the following tag module
  sourceFoundFile CompositionSequences/CompKs2Pi0Sequence.tcl
  sequence append SkimTag2BodySequence CompKs2Pi0Sequence

  loadTagModule TagK_S0Pi0Pi0TwoBody SkimTag2BodySequence {
    particleType set "K_S0"
    inputList    set "KsToPi0Pi0Loose"
    verbose       set false
  }

  loadTagModule TagKTwoBody SkimTag2BodySequence {
    particleType set "K+"
    inputList    set "KLHNotPion"
    # recoiling particles
    recoilPartList   set "J/psi"
    recoilPartList   set "psi(2S)"
    recoilPartList   set "eta_c"
    recoilPartList   set "h_c"
    recoilPartList   set "chi_c0"
    recoilPartList   set "chi_c1"
    recoilPartList   set "chi_c2"
    recoilPartList   set "eta_c(2S)"
    # customization (verbose : only at initialization and summary)
    verbose       set false
    tolerance     set 0.2
  }

  loadTagModule TagPi0TwoBody SkimTag2BodySequence {
    particleType set "pi0"
    inputList    set "pi0AllLoose"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagEtaTwoBody SkimTag2BodySequence {
    particleType set "eta"
    inputList    set "etaDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagEtaPTwoBody SkimTag2BodySequence {
    particleType set "eta'"
    inputList    set "etaPDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagEtaPeppTwoBody SkimTag2BodySequence {
    particleType set "eta'"
    inputList    set "etaPeppDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagEtaPrhogTwoBody SkimTag2BodySequence {
    particleType set "eta'"
    inputList    set "etaPrgDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagKstTwoBody SkimTag2BodySequence {
    particleType set "K*0"
    inputList    set "KstarNeutralLoose"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagKstpTwoBody SkimTag2BodySequence {
    particleType set "K*+"
    inputList    set "KstarChrgLoose"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagOmegaTwoBody SkimTag2BodySequence {
    particleType set "omega"
    inputList    set "omegaDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagRhoTwoBody SkimTag2BodySequence {
    particleType set "rho0"
    inputList    set "rho0Default"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagRhopTwoBody SkimTag2BodySequence {
    particleType set "rho+"
    inputList    set "rhoCDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagPhiTwoBody SkimTag2BodySequence {
    particleType set "phi"
    inputList    set "phiDefault"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagAssignKaonMass SkimTag2BodySequence {
    particleType set "K+"
    inputList    set "GoodTracksLoose"
    outputList   set "GoodTracksLooseKaonMass"
    verbose      set false
  }
  
  loadTagModule TagAssignProtonMass SkimTag2BodySequence {
    particleType set "p+"
    inputList    set "GoodTracksLoose"
    outputList   set "GoodTracksLooseProtonMass"
    verbose      set false
  }
  
  loadTagModule TagCTwoBody SkimTag2BodySequence {
    particleType set "pi+"
    inputList    set "GoodTracksLoose"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagKpTwoBody SkimTag2BodySequence {
    particleType set "K+"
    inputList    set "GoodTracksLooseKaonMass"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  loadTagModule TagPTwoBody SkimTag2BodySequence {
    particleType set "p+"
    inputList    set "GoodTracksLooseProtonMass"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  # multi-list 2-body decays
  loadTagModule TagBToPP SkimTag2BodySequence {
    # Ks
    tagList        set KsDefault
    tagLund        set K_S0
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagK_S0TwoBody_maxCmsP
    
    # pi0
    tagList        set pi0AllLoose
    tagLund        set pi0
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagPi0TwoBody_maxCmsP
    
    # eta
    tagList        set etaDefault
    tagLund        set eta
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagEtaTwoBody_maxCmsP

    # eta' -> eta pi pi
    tagList        set etaPeppDefault
    tagLund        set "eta'"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagEtaPeppTwoBody_maxCmsP

    # eta' -> rho gamma
    tagList        set etaPrgDefault
    tagLund        set "eta'"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagEtaPrhogTwoBody_maxCmsP
    
    # phi
    tagList        set phiDefault
    tagLund        set "phi"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagPhiTwoBody_maxCmsP

    # omega
    tagList        set omegaDefault
    tagLund        set "omega"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagOmegaTwoBody_maxCmsP

    # pi+
    tagList        set GoodTracksLoose
    tagLund        set "pi+"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagCTwoBody_maxCmsP

    # K+
    tagList        set GoodTracksLooseKaonMass
    tagLund        set "K+"
    tagMinMulti    set 1
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagKpTwoBody_maxCmsP

    # p+
    tagList        set GoodTracksLooseProtonMass
    tagLund        set "p+"
    tagMinMulti    set 2
    tagMaxMulti    set 2
    tagRequired    set false
    tagName        set TagPTwoBody_maxCmsP

    # etc.

    #verbose set t

    EstarSumCut   set 4.99
    EstarSumCutHi set 5.59
    BmassCut      set 5.20
    tagbit        set BToPP	
  }

  loadTagModule TagLambdaTwoBody SkimTag2BodySequence {
    particleType set "Lambda0"
    inputList    set "LambdaLoose"
    # customization (verbose : only at initialization and summary)
    verbose       set false
  }

  # Enable all modules on request. (used in setup3BodySkimPath)
  proc enableAll2BodyModules {} {
    lappend Tag2Bmodules \
        TagK_S0TwoBody TagK_S0Pi0Pi0TwoBody TagKTwoBody TagPi0TwoBody TagEtaTwoBody \
        TagEtaPTwoBody TagEtaPeppTwoBody TagEtaPrhogTwoBody TagKstTwoBody \
        TagKstpTwoBody TagOmegaTwoBody TagRhoTwoBody TagRhopTwoBody TagPhiTwoBody \
        TagAssignKaonMass TagAssignProtonMass TagCTwoBody TagKpTwoBody TagPTwoBody \
        TagBToPP TagLambdaTwoBody

     foreach module $Tag2Bmodules {module enable $module}
  }
}
