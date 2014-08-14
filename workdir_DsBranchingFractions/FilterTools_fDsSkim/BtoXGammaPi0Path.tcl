#
# $Id: BtoXGammaPi0Path.tcl,v 1.2 2005/06/09 13:08:57 fnc Exp $
#
# defines DstarKpiPath tag filter path
#
global BtaCandLists
lappend BtaCandLists bsgPi0CandList
lappend BtaCandLists bsgEtaCandList

path create BtoXGammaPi0Path

# need to run on MultiHadrons and NeutralHadron

# create tagbit filter
module clone TagFilterByName BtoXGammaPi0BGF

# accept tags that we are interested in
talkto BtoXGammaPi0BGF {
  orList set BGFMultiHadron
  orList set BGFNeutralHadron
}

path append BtoXGammaPi0Path BtoXGammaPi0BGF
path append BtoXGammaPi0Path BtoXGammaPi0Filter
