#-------------------------------------------------- 
#Reconstruction of:
#Author: Jose Benitez
#--------------------------------------------------

#--------------------------------------------------
#some default stuff
#--------------------------------------------------
sourceFoundFile ErrLogger/ErrLog.tcl
sourceFoundFile FrameScripts/FwkCfgVar.tcl
sourceFoundFile FrameScripts/talkto.tcl
sourceFoundFile FrameScripts/setProduction.tcl

#--------------------------------------------------
#Disable the use of envvars
#--------------------------------------------------
set ProdTclOnly true                                      

#--------------------------------------------------
#set the error logging level to 'warning'
#--------------------------------------------------
ErrLoggingLevel warning

#--------------------------------------------------
#
#--------------------------------------------------
FwkCfgVar BetaMiniReadPersistence "Kan"    

#--------------------------------------------------
#set the print frequency of event counter
#--------------------------------------------------
talkto EvtCounter {
    printFreq set 100
}

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "DsTolnu/SP5315/DsReco/Ntuples1/Ntuple1.root"

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 10000      

#--------------------------------------------------
#Set the first event to be analyzed
#--------------------------------------------------
talkto KanEventInput {
first set 1
}

#--------------------------------------------------
#should I use refit?
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source DsTolnu/SP5315/DsReco/Ntuples1/DataSet-1.tcl

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true
    
#--------------------------------------------------                            
#create Everything path and add core sequences to it
#--------------------------------------------------                        
sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

#--------------------------------------------------
#actions 
#--------------------------------------------------
sourceFoundFile BetaMiniSequences/BetaMiniActions.tcl
action disable HistoDirAction

#--------------------------------------------------
#Enable User defined Data
#--------------------------------------------------
sourceFoundFile UsrTools/UsrDataProcs.tcl
enableReadUsrData


#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates


#--------------------------------------------------
#configure Modules
#--------------------------------------------------
module talk RecoilTagInterface  
   #order matters, it is preferable to choose D*'s against D's because slow pi's will not fake muons.

    #D*+: Kpi,Kpipi0,K3pi
    TagList1 set DstarChrgKLooseRSPID
    TagListFit1 set true
    TagListMass1 set 2.0099
    TagListMassCut1 set .003
    TagListPstarCut1 set 2.0

   #D0: Kpi,Kpipi0,K3pi,Kspipi
   TagList2 set D0MainLoosePID
   TagListFit2 set true
   TagListMass2 set 1.8648
   TagListMassCut2 set .02
   TagListPstarCut2 set 2.0

    #D+: DcToKPiPi, DcToKPiPiPi0, DcToKsPi, DcToKsPiPi0, DcToKKPi, DcToKsK,
    TagList3 set DcLoosePID
    TagListFit3 set true
    TagListMass3 set 1.8692
    TagListMassCut3 set .02
    TagListPstarCut3 set 2.0

exit
module talk RecoilTagKaon
  KList set KLHVeryLoose
  K0List set KsDefaultMass
  KaonEnergyCutLo set .5
  KaonEnergyCutHi set 3.
  KaonAngleCut set .1
exit
module talk RecoilFragPions
    PiList set ChargedTracks
    Pi0List set pi0DefaultMass
    PionEnergyCutLo set .15
    PionEnergyCutHi set 3.
    FitFrag set false
exit
module talk RecoilDsStGam
  GamList set GoodPhotonLoose
  GamEnergyCutLo set .1
  GamEnergyCutHi set 1.
  GamAngleCut set -.1
exit
module talk RecoilCheckCharge
  #first particle (tag-D) is last digit: 4=negative,5=neutral,6=positive
  #complex congujate event is automatically included.

  #missing particle is Ds+(c sbar)
  
  #D0
  #   Gam,Frag0(u ubar), K-(s ubar), D0(cbar u)
  ChargeConfig1 set 5545 
  #   Gam,Frag-(d ubar), K0(s dbar), D0(cbar u)
  ChargeConfig2 set 5455 
  
  #D+
  #  Gam,Frag0(u ubar), K0(s dbar), D-(cbar d)
  ChargeConfig3 set 5554
  #  Gam,Frag+(u dbar), K-(s ubar), D-(cbar d)
  ChargeConfig4 set 5644

  #D*+ = D+

exit


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set RecoilCandidateList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set true 
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "Upsilon(4S)  Evt 4 100" 
    ntpBlockConfigs set "D_s*+  DsSt  2  100" 
    ntpBlockConfigs set "B+     Frag  20 100" 
    ntpBlockConfigs set "B0     Frag  20 100"
    ntpBlockConfigs set "D*+    Tag   4 100" 
    ntpBlockConfigs set "D+     Tag   4 100" 
    ntpBlockConfigs set "D0     Tag   4 100" 
    ntpBlockConfigs set "D_s+   Ds    0 100"  
    ntpBlockConfigs set "K+     K     0 100"  
    ntpBlockConfigs set "K_S0   Ks    2 100" 
    ntpBlockConfigs set "pi+    Pi    0 100" 
    ntpBlockConfigs set "pi0    Pi0   2 100" 
    ntpBlockConfigs set "gamma  Gam   0 100" 
 
    #UsrData(EvtUsrData)
    ntpBlockContents set "Evt:  Mass Momentum CMMomentum nDaughters"
    ntpBlockContents set "Frag: Mass Momentum CMMomentum nDaughters" 
    ntpBlockContents set "Tag:  Mass Vertex VtxChi2 Momentum Flight FlightBS CMMomentum nDaughters"
    ntpBlockContents set "DsSt: Mass Vertex VtxChi2 Momentum CMMomentum"
    ntpBlockContents set "Ds:   Mass Momentum Flight FlightBS CMMomentum"
    ntpBlockContents set "Ks:   Mass Vertex VtxChi2 Momentum Flight FlightBS CMMomentum"
    ntpBlockContents set "Pi:   Momentum Doca Poca CMMomentum "
    ntpBlockContents set "Pi0:  Mass Vertex VtxChi2 Momentum CMMomentum"
    ntpBlockContents set "K:    Momentum Doca Poca CMMomentum "
    ntpBlockContents set "Gam:  Momentum CMMomentum "
    
    checkCloneBlocks set "K Pi Gam"
}

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything RecoilTagInterface
path append Everything RecoilTagKaon
path append Everything RecoilFragPions
path append Everything RecoilDsStGam
path append Everything RecoilCheckCharge
path append Everything RecoilDsEvtCandListFit
#path append Everything RecoilTruthMatch
path append Everything BtuTupleMaker



#--------------------------------------------------
#start the job
#--------------------------------------------------
path list

if [ info exists NEvent ] {
    ErrMsg trace " Processing $NEvent events"
    ev begin -nev $NEvent
} else {
    ErrMsg trace " Processing all events"
    ev begin
}

ErrMsg trace "completed OK"
exit






