#
#  $Id: BtoPhiKPhiTo3piSequence.tcl,v 1.1 2005/06/14 20:40:51 kazuhito Exp $
#
#################################################################
# list definitions for exclusive B -> phi(->pi+pi-pi0) K decays #
#################################################################

global BtoPhiKPhiTo3piSequenceDefinition

if [ expr ! [info exists BtoPhiKPhiTo3piSequenceDefinition ] ] {
    set BtoPhiKPhiTo3piSequenceDefinition 1
    
    set seqName BtoPhiKPhiTo3piSequence
    seq create $seqName

    set verbose f
    set debug   f

    set phiList   PhiTo3piB2body
    set KsList    KsLooseBtoPhiKPhiTo3pi
    set KpList    GoodTrackLooseBtoPhiKPhiTo3pi
    set phiKsList BtoPhiKsPhiTo3pi
    set phiKpList BtoPhiKpPhiTo3pi
    
#------------------------------------------------------------------------------

  createsmpmaker $seqName $phiList {
    decayMode          set "phi -> pi+ pi- pi0"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "GoodTracksLoose"
    daughterListNames  set "pi0AllLoose"
    postFitSelectors   set "Mass 0.89:1.11"
    postFitSelectors   set "CmsP 1.99:3.01"
    fittingAlgorithm   set "Add4"
    fitConstraints     set "PrimaryVertex"
    verbose            set $verbose
    debug              set $debug
  }

#------------------------------------------------------------------------------

  createsmpsublister $seqName $KsList {
    unrefinedListName  set "KsLoose"
    selectors          set "CmsP 1.99:3.01"
    verbose            set $verbose
    debug              set $debug
  }

  createsmpmaker $seqName $phiKsList {
    decayMode          set "B0 -> phi K_S0"
    daughterListNames  set $phiList
    daughterListNames  set $KsList
    postFitSelectors   set "Mes 5.19:"
    postFitSelectors   set "DeltaE -0.51:0.51"
    fittingAlgorithm   set "Add4"
    verbose            set $verbose
    debug              set $debug
  }

#------------------------------------------------------------------------------

  createsmpsublister $seqName $KpList {
    unrefinedListName  set "GoodTracksLoose"
    selectors          set "CmsP 1.99:3.01"
    verbose            set $verbose
    debug              set $debug
  }

  createsmpmaker $seqName $phiKpList {
    decayMode          set "B+ -> phi K+"
    daughterListNames  set $phiList
    daughterListNames  set $KpList
    postFitSelectors   set "Mes 5.19:"
    postFitSelectors   set "DeltaE -0.51:0.51"
    fittingAlgorithm   set "Add4"
    verbose            set $verbose
    debug              set $debug
  }
}
