# # Xs(s,sbar)ll decays
# two pion modes
createsllbglists BToKsKKPiPi0MuMu SllBGSeq "B+ -> K_S0 K+ K- pi+ pi0 mu+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XsllMuList"
createsllbglists BToKKKPiPi0MuMu SllBGSeq  "B0 -> K+ K+ K- pi- pi0 mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XsllMuList"
createsllbglists BToKsKKPiPiMuMu SllBGSeq  "B0 -> K_S0 K+ K- pi+ pi- mu+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks ChargedTracks XsllMuList XsllMuList"
createsllbglists BToKKKPiPiMuMu SllBGSeq   "B+ -> K+ K+ K- pi+ pi- mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks ChargedTracks XsllMuList XsllMuList"
createsllbglists BToKsKKPiPi0EE SllBGSeq   "B+ -> K_S0 K+ K- pi+ pi0 e+ e-"   "KsDefault ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKKKPiPi0EE SllBGSeq    "B0 -> K+ K+ K- pi- pi0 e+ e-"     "KLHNotPion ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKsKKPiPiEE SllBGSeq    "B0 -> K_S0 K+ K- pi+ pi- e+ e-"   "KsDefault ChargedTracks ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKKKPiPiEE SllBGSeq     "B+ -> K+ K+ K- pi+ pi- e+ e-"     "KLHNotPion ChargedTracks ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"

# one pion modes
createsllbglists BToKsKKPi0MuMu SllBGSeq "B0 -> K_S0 K+ K- pi0 mu+ mu-" "KsDefault ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XsllMuList"
createsllbglists BToKKKPi0MuMu SllBGSeq  "B+ -> K+ K+ K- pi0 mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XsllMuList"
createsllbglists BToKsKKPiMuMu SllBGSeq  "B+ -> K_S0 K+ K- pi+ mu+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks XsllMuList XsllMuList"
createsllbglists BToKKKPiMuMu SllBGSeq   "B0 -> K+ K+ K- pi- mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks XsllMuList XsllMuList"
createsllbglists BToKsKKPi0EE SllBGSeq   "B0 -> K_S0 K+ K- pi0 e+ e-"   "KsDefault ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKKKPi0EE SllBGSeq    "B+ -> K+ K+ K- pi0 e+ e-"     "KLHNotPion ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKsKKPiEE SllBGSeq    "B+ -> K_S0 K+ K- pi+ e+ e-"   "KsDefault ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKKKPiEE SllBGSeq     "B0 -> K+ K+ K- pi- e+ e-"     "KLHNotPion ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"

# no pion modes
createsllbglists BToKKKMuMu SllBGSeq   "B+ -> K+ K+ K- mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks XsllMuList XsllMuList"
createsllbglists BToKKKEE SllBGSeq     "B+ -> K+ K+ K- e+ e-"     "KLHNotPion ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"

###################0 pion modes
createsllbglists BToKsMuE SllBGSeq "B0 -> K_S0 mu+ e-" "KsDefault XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKMuE SllBGSeq  "B+ -> K+ mu+ e-"   "KLHNotPion XsllMuList XslleBremRecoPidHllMerge"
###################single pion modes
createsllbglists BToKsPi0MuE SllBGSeq "B0 -> K_S0 pi0 mu+ e-" "KsDefault pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPi0MuE SllBGSeq  "B+ -> K+ pi0 mu+ e-"   "KLHNotPion pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKsPiMuE SllBGSeq  "B+ -> K_S0 pi+ mu+ e-" "KsDefault ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiMuE SllBGSeq   "B0 -> K+ pi- mu+ e-"   "KLHNotPion ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
###################2pion modes
createsllbglists BToKsPiPi0MuE SllBGSeq "B+ -> K_S0 pi+ pi0 mu+ e-" "KsDefault ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiPi0MuE SllBGSeq  "B0 -> K+ pi- pi0 mu+ e-"   "KLHNotPion ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKsPiPiMuE SllBGSeq  "B0 -> K_S0 pi+ pi- mu+ e-" "KsDefault ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiPiMuE SllBGSeq   "B+ -> K+ pi+ pi- mu+ e-"   "KLHNotPion ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
###################3pion modes
createsllbglists BToKsPiPiPi0MuE SllBGSeq "B0 -> K_S0 pi+ pi- pi0 mu+ e-" "KsDefault ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiPiPi0MuE SllBGSeq  "B+ -> K+ pi+ pi- pi0 mu+ e-"   "KLHNotPion ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKsPiPiPiMuE SllBGSeq  "B+ -> K_S0 pi+ pi- pi+ mu+ e-" "KsDefault ChargedTracks ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiPiPiMuE SllBGSeq   "B0 -> K+ pi+ pi- pi- mu+ e-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
###################4pion modes
createsllbglists BToKsPiPiPiPi0MuE SllBGSeq "B+ -> K_S0 pi+ pi- pi+ pi0 mu+ e-" "KsDefault ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiPiPiPi0MuE SllBGSeq  "B0 -> K+ pi- pi+ pi- pi0 mu+ e-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKsPiPiPiPiMuE SllBGSeq  "B0 -> K_S0 pi+ pi- pi+ pi- mu+ e-" "KsDefault ChargedTracks ChargedTracks ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToKPiPiPiPiMuE SllBGSeq   "B+ -> K+ pi+ pi- pi+ pi- mu+ e-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"

###################0 pion modes
createsllbglists BToKsEMu SllBGSeq "B0 -> K_S0 e+ mu-" "KsDefault XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKEMu SllBGSeq  "B+ -> K+ e+ mu-"   "KLHNotPion XslleBremRecoPidHllMerge XsllMuList"
###################single pion modes
createsllbglists BToKsPi0EMu SllBGSeq "B0 -> K_S0 pi0 e+ mu-" "KsDefault pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPi0EMu SllBGSeq  "B+ -> K+ pi0 e+ mu-"   "KLHNotPion pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKsPiEMu SllBGSeq  "B+ -> K_S0 pi+ e+ mu-" "KsDefault ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiEMu SllBGSeq   "B0 -> K+ pi- e+ mu-"   "KLHNotPion ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
###################2pion modes
createsllbglists BToKsPiPi0EMu SllBGSeq "B+ -> K_S0 pi+ pi0 e+ mu-" "KsDefault ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiPi0EMu SllBGSeq  "B0 -> K+ pi- pi0 e+ mu-"   "KLHNotPion ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKsPiPiEMu SllBGSeq  "B0 -> K_S0 pi+ pi- e+ mu-" "KsDefault ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiPiEMu SllBGSeq   "B+ -> K+ pi+ pi- e+ mu-"   "KLHNotPion ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
###################3pion modes
createsllbglists BToKsPiPiPi0EMu SllBGSeq "B0 -> K_S0 pi+ pi- pi0 e+ mu-" "KsDefault ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiPiPi0EMu SllBGSeq  "B+ -> K+ pi+ pi- pi0 e+ mu-"   "KLHNotPion ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKsPiPiPiEMu SllBGSeq  "B+ -> K_S0 pi+ pi- pi+ e+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiPiPiEMu SllBGSeq   "B0 -> K+ pi+ pi- pi- e+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
###################4pion modes
createsllbglists BToKsPiPiPiPi0EMu SllBGSeq "B+ -> K_S0 pi+ pi- pi+ pi0 e+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiPiPiPi0EMu SllBGSeq  "B0 -> K+ pi- pi+ pi- pi0 e+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKsPiPiPiPiEMu SllBGSeq  "B0 -> K_S0 pi+ pi- pi+ pi- e+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToKPiPiPiPiEMu SllBGSeq   "B+ -> K+ pi+ pi- pi+ pi- e+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"

###################dll modes
createsllbglists BToPiEMu SllBGSeq      "B+ -> pi+ e+ mu-"         "ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToPi0EMu SllBGSeq     "B0 -> pi0 e+ mu-"         "pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToPiP0EMu SllBGSeq    "B+ -> pi+ pi0 e+ mu-"     "ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToPiPiEMu SllBGSeq    "B0 -> pi+ pi- e+ mu-"     "ChargedTracks ChargedTracks XslleBremRecoPidHllMerge XsllMuList"
createsllbglists BToPiPiPi0EMu SllBGSeq "B0 -> pi+ pi- pi0 e+ mu-" "ChargedTracks ChargedTracks pi0DefaultMass XslleBremRecoPidHllMerge XsllMuList"

createsllbglists BToPiMuE SllBGSeq      "B+ -> pi+ mu+ e-"         "ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToPi0MuE SllBGSeq     "B0 -> pi0 mu+ e-"         "pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToPiP0MuE SllBGSeq    "B+ -> pi+ pi0 mu+ e-"     "ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToPiPiMuE SllBGSeq    "B0 -> pi+ pi- mu+ e-"     "ChargedTracks ChargedTracks XsllMuList XslleBremRecoPidHllMerge"
createsllbglists BToPiPiPi0MuE SllBGSeq "B0 -> pi+ pi- pi0 mu+ e-" "ChargedTracks ChargedTracks pi0DefaultMass XsllMuList XslleBremRecoPidHllMerge"

###############Double LFV modes
createsllbglists BToKEELFV SllBGSeq      "B+ -> K- e+ e+"         "KLHNotPion XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKMuMuLFV SllBGSeq    "B+ -> K- mu+ mu+"       "KLHNotPion XsllMuList XsllMuList"
createsllbglists BToKsPiEELFV SllBGSeq   "B+ -> K_S0 pi- e+ e+"   "KsDefault ChargedTracks XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKsPiMuMuLFV SllBGSeq "B+ -> K_S0 pi- mu+ mu+" "KsDefault ChargedTracks XsllMuList XsllMuList"
createsllbglists BToKPi0EELFV SllBGSeq   "B+ -> K- pi0 e+ e+"     "KLHNotPion pi0DefaultMass XslleBremRecoPidHllMerge XslleBremRecoPidHllMerge"
createsllbglists BToKPi0MuMuLFV SllBGSeq "B+ -> K- pi0 mu+ mu+"   "KLHNotPion pi0DefaultMass XsllMuList XsllMuList"