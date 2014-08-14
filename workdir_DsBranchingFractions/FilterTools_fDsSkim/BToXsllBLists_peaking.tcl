#Refined Muon list
mod clone SmpSubListerDefiner XsllMuListPeaking
seq append Sll_peakingSeq XsllMuListPeaking
catch { setProduction XsllMuListPeaking }
talkto XsllMuListPeaking {
    unrefinedListName set "muNNVeryLoose"
    selectors         set "LabP 0.5:"
}

#Refined ChargedTracks-as-Muon list
mod clone SmpSubListerDefiner XsllMuListCT
seq append Sll_peakingSeq XsllMuListCT
catch { setProduction XsllMuListCT }
talkto XsllMuListCT {
    unrefinedListName set "ChargedTracks"
    selectors         set "LabP 0.5:"
}

#0 pion modes
peakingcreateshortbkslists BToKsMuCTMu "B0 -> K_S0 mu+ mu-" "KsDefault XsllMuListCT XsllMuListPeaking"
peakingcreateshortblists BToKMuCTMu  "B+ -> K+ mu+ mu-"   "KLHNotPion XsllMuListCT XsllMuListPeaking"
#single pion modes
peakingcreateshortbkslists BToKsPi0MuCTMu "B0 -> K_S0 pi0 mu+ mu-" "KsDefault pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreateshortblists BToKPi0MuCTMu  "B+ -> K+ pi0 mu+ mu-"   "KLHNotPion pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreateshortbkslists BToKsPiMuCTMu  "B+ -> K_S0 pi+ mu+ mu-" "KsDefault ChargedTracks XsllMuListCT XsllMuListPeaking"
peakingcreateshortblists BToKPiMuCTMu   "B0 -> K+ pi- mu+ mu-"   "KLHNotPion ChargedTracks XsllMuListCT XsllMuListPeaking"
#2pion modes
peakingcreateshortbkslists BToKsPiPi0MuCTMu "B+ -> K_S0 pi+ pi0 mu+ mu-" "KsDefault ChargedTracks pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreateshortblists BToKPiPi0MuCTMu  "B0 -> K+ pi- pi0 mu+ mu-"   "KLHNotPion ChargedTracks pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreateshortbkslists BToKsPiPiMuCTMu  "B0 -> K_S0 pi+ pi- mu+ mu-" "KsDefault ChargedTracks ChargedTracks XsllMuListCT XsllMuListPeaking"
peakingcreateshortblists BToKPiPiMuCTMu   "B+ -> K+ pi+ pi- mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks XsllMuListCT XsllMuListPeaking"
###################3pion modes
peakingcreatelongbkslists BToKsPiPiPi0MuCTMu "B0 -> K_S0 pi+ pi- pi0 mu+ mu-" "KsDefault ChargedTracks ChargedTracks pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreatelongblists BToKPiPiPi0MuCTMu  "B+ -> K+ pi+ pi- pi0 mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreatelongbkslists BToKsPiPiPiMuCTMu  "B+ -> K_S0 pi+ pi- pi+ mu+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks XsllMuListCT XsllMuListPeaking"
peakingcreatelongblists BToKPiPiPiMuCTMu   "B0 -> K+ pi+ pi- pi- mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks XsllMuListCT XsllMuListPeaking"
###################4pion modes
peakingcreatelongbkslists BToKsPiPiPiPi0MuCTMu "B+ -> K_S0 pi+ pi- pi+ pi0 mu+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreatelongblists BToKPiPiPiPi0MuCTMu  "B0 -> K+ pi- pi+ pi- pi0 mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks pi0DefaultMass XsllMuListCT XsllMuListPeaking"
peakingcreatelongbkslists BToKsPiPiPiPiMuCTMu  "B0 -> K_S0 pi+ pi- pi+ pi- mu+ mu-" "KsDefault ChargedTracks ChargedTracks ChargedTracks ChargedTracks XsllMuListCT XsllMuListPeaking"
peakingcreatelongblists BToKPiPiPiPiMuCTMu   "B+ -> K+ pi+ pi- pi+ pi- mu+ mu-"   "KLHNotPion ChargedTracks ChargedTracks ChargedTracks ChargedTracks XsllMuListCT XsllMuListPeaking"

peakingDaughterLister  BToKsMuCTMu    "XsllKsList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKMuCTMu     "XsllKList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"

peakingDaughterLister  BToKsPi0MuCTMu    "XsllKsList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPi0MuCTMu     "XsllKList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKsPiMuCTMu     "XsllKsList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiMuCTMu      "XsllKList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"

peakingDaughterLister  BToKsPiPi0MuCTMu    "XsllKsList_peaking XsllPiList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiPi0MuCTMu     "XsllKList_peaking XsllPiList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKsPiPiMuCTMu     "XsllKsList_peaking XsllPiList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiPiMuCTMu      "XsllKList_peaking XsllPiList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"

peakingDaughterLister  BToKsPiPiPi0MuCTMu    "XsllKsList_peaking XsllPiList_peaking XsllPiList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiPiPi0MuCTMu     "XsllKList_peaking XsllPiList_peaking XsllPiList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKsPiPiPiMuCTMu     "XsllKsList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiPiPiMuCTMu      "XsllKList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"

peakingDaughterLister  BToKsPiPiPiPi0MuCTMu    "XsllKsList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiPiPiPi0MuCTMu     "XsllKList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllPi0List_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKsPiPiPiPiMuCTMu     "XsllKsList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"
peakingDaughterLister  BToKPiPiPiPiMuCTMu      "XsllKList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllPiList_peaking XsllDauMuList_peaking XsllDauMuList_peaking"

