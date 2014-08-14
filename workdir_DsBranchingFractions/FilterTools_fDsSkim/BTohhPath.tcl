#
# $Id: BTohhPath.tcl,v 1.2 2008/03/10 15:56:16 marchior Exp $
#
#
# defines BTohh filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

sourceFoundFile EventTagTools/TagProcs.tcl

path create BTohhPath

sequence create TagB0ToPi0Pi0Sequence
path append BTohhPath TagB0ToPi0Pi0Sequence

module clone TagFromList B0ToPi0Pi0TagFromListNoEnergyFilter
talkto B0ToPi0Pi0TagFromListNoEnergyFilter {
    tagName set B0ToPi0Pi0TagNoEnergyFilter
    verbose set false
    production set true
    listsToTag set B0ToPi0Pi0Tag
    #show
}
sequence append TagB0ToPi0Pi0Sequence B0ToPi0Pi0TagFromListNoEnergyFilter

module clone TagFromTag B0ToPi0Pi0TagFromTag
talkto B0ToPi0Pi0TagFromTag {
    assertIfMissing set true
    andList set RecoEnergyFilter
    andList set B0ToPi0Pi0TagNoEnergyFilter
    tagName set B0ToPi0Pi0Tag
    #show
}
sequence append TagB0ToPi0Pi0Sequence B0ToPi0Pi0TagFromTag

sequence create TagBTohhSequence
path append BTohhPath TagBTohhSequence

# Define lists of composites to tag
lappend BtaCandLists BToKsPiTag B0ToPiPiTag B0ToKsKsTag BToPi0PiTag B0ToKsPi0Tag B0ToPPTag B0ToLambdaLambdaTag BToLambdaPTag

foreach btalist $BtaCandLists {
    module clone TagFromList ${btalist}FromList
    talkto ${btalist}FromList {
	tagName set ${btalist}
	verbose set false
	production set true
        listsToTag set ${btalist}
	#show
    }
    sequence append TagBTohhSequence ${btalist}FromList
}

lappend BtaCandLists B0ToPi0Pi0Tag

module clone TagFilterByName TagBTohhFilter
talkto TagBTohhFilter {
    verbose set false
    production set true
    assertIfMissing set false
    foreach btalist $BtaCandLists {
	orList set ${btalist}
    }
    #show
}

path append BTohhPath TagBTohhFilter

# Don't want to write composites, so following code is commented out

# Add lists of composites with UsrData
#set UsrBCandLists $BtaCandLists

# Write UsrData
#set writeUsrDataForSkim 1
