# Tag mode           Composite 	D_s+	D+	D0	Lam_c+	Lam0	Sig0	p+	eta' 	rho0 	rho+ 	phi 	eta 	K+	K_S0 	pi+ 	pi0 	gamma 
# list used in	       list 	(Tag)	(Tag)	(Tag)	(Tag)	(Lam0)	(Sig0)	(P)	(EtaP)	(Rho)	(Rho)	(Phi)	(Eta)	(K)	(Ks)	(Pi)	(Pi0)	(Gam)
# fDsPath.tcl	       names	
																		
# D0ToKPiLoosePID				1										1		1		
# D0ToKPiPi0LoosePID				1										1		1	1	2
# D0ToKsPiPiLoosePID				1											1	4		
# D0ToK3PiLoose				1										1		3		
# D0ToKsPimPipPi0				1											1	4	1	2
# D0ToKmPipPipPimPi0				1										1		3	1	2
																		
# DcToKsPiLoosePID			1												1	3		
# DcToKPiPiLoosePID			1											1		2		
# DcToKsPiPi0LoosePID			1												1	3	1	2
# DcToKPiPiPi0LoosePID			1											1		2	1	2
# DcToKsPiPiPiLoosePID			1												1	5		
																		
# DsToKsKLoosePID		1													1	2		
# DsToKKPiPrompt		1												2		1		
# DsToPhiRhoCLoosePID	L1 L2	1										1		2		1	1	2
# DsToKsKPiPiLoosePID		1												1	1	4		
# DsTo3PiPrompt		        1 														3		
# DsToPhiPiPiPiLoosePID	L1	1										1		2		3		
# DsToEtaRhoCLoosePID	L2 L3	1									1		1			3	2	4
# DsToEtaPRhoCLoosePID	L2 L4	1							1		1		1			5	2	4
# DsToEtaPiLoosePID	L3	1											1			3	1	2
# DsToEtaPPiLoosePID	L5 (L6)	1							1	1			1			5	1	2
																		
# LambdaCTopKpi					1			1						1		1		
# LambdaCTopKpipi					1			1						1		1	1	2
# LambdaCTopKs					1			1							1	2		
# LambdaCTopKspipi					1			1							1	4		
# LambdaCToLzpi	L7				1	1										2		
# LambdaCToLzpipi	L7				1	1										2	1	2
# LambdaCToLzpipipi	L7				1	1										4		
# LambdaCToSzpi	L8				1		1									2		
# LambdaCToSzpipipi	L8				1		1									4		
																		
# Maxiumum number		1	1	1	1	1	1	1	1		1	1	1	2	1	5	2	4
# x20 Tag candidates		20	20	20	20	20	20	20	20		20	20	20	40	20	100	40	80


# L1 = phiDefault	phi->KK																	
# L2 = rhoCTight	pipi0																	
# L3 = etaDefault	"eta->gg, eta->pipipi0"																	
# L4 = etaPeppDefault	etaP->pipieta																	
# L5 = etaPDefault	"etaP->pipieta, etaP->rho0g"
# L6 = rho0Default	rho0->pipi
# L7 = Lambda_LambdaC	
# L8 = Sigma_LambdaC	


#-------------------------------------------------- 
#Reconstruction of:
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
FwkCfgVar histFileName "DsTolnu/SP1005Skim/TagOpt/Ntuples/Run3/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 100

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
source DsTolnu/SP1005Skim/TagOpt/NtuplesData/Run3/DataSet-1.tcl


#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
    
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


##----------------
##create this analysis sequence
##-------------------------
sequence create DRecoSequence


#-------------------------------------
#creat decay modes: According to Shanes skim 
#-------------------------------------
FwkCfgVar writeChmTagNtuples 0
sourceFoundFile CharmTagTools/ChmTagLists.tcl
seq append DRecoSequence ChmDrecoSequence

##++++++++++++++++++++++++++++++++++++++
### Merge the D0 modes
##----------------------------------------
createsmpmerger DRecoSequence DRecoD0Main {
    inputListNames    set "ChmD0ToKPi"
    inputListNames    set "ChmD0ToKPiPi0"
    inputListNames    set "ChmD0ToK3Pi"
    inputListNames    set "ChmD0ToK3PiPi0Tight" 
    inputListNames    set "ChmD0ToKShortPiPi"
    inputListNames    set "ChmD0ToKShortPiPiPi0" 
}


##++++++++++++++++++++++++++++++++++++++
### Merge the D+ modes
##++++++++++++++++++++++++++++++++++++++
createsmpmerger DRecoSequence DRecoDPlusMain {
    inputListNames    set "ChmDPlusToKPiPi"
    inputListNames    set "ChmDPlusToKPiPiPi0"
    inputListNames    set "ChmDPlusToKShortPi"
    inputListNames    set "ChmDPlusToKShortPiPi0"
    inputListNames    set "ChmDPlusToKShort3Pi"
}


#--------------------------------------------------
#configure Modules
#--------------------------------------------------
module talk DRecoTagInterface  
    
     #D0: Kpi,Kpipi0,K3pi,Kspipi,K3pipi0,Kspipipi0
     TagList1 set DRecoD0Main
     TagListFit1 set true
     TagListMass1 set 1.8648
     TagListMassCut1 set .12
     TagListPstarCut1 set 2.0

     #D+: DcToKPiPi, DcToKPiPiPi0, DcToKsPi, DcToKsPiPi0, DcToKs3Pi
     TagList2 set DRecoDPlusMain
     TagListFit2 set true
     TagListMass2 set 1.8692
     TagListMassCut2 set .12
     TagListPstarCut2 set 2.0
exit


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DRecoTagList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set false
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    

    ntpBlockConfigs set "D+     Tag   5 30" 
    ntpBlockConfigs set "D0     Tag   5 30" 
    ntpBlockConfigs set "K+     K     0 100"  
    ntpBlockConfigs set "K_S0   Ks    2 100" 
    ntpBlockConfigs set "pi+    Pi    0 100" 
    ntpBlockConfigs set "pi0    Pi0   2 200" 
    ntpBlockConfigs set "gamma  Gam   0 100" 
 
    ntpBlockContents set "Tag:  Mass Vertex VtxChi2 FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Ks:   Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "Pi:   CMMomentum"
    ntpBlockContents set "Pi0:  Mass CMMomentum"
    ntpBlockContents set "K:    CMMomentum"
    ntpBlockContents set "Gam:  CMMomentum"
    
    checkCloneBlocks set "K Pi"
    ntpBlockToTrk set "K Pi"
    trkExtraContents set HOTS
    trkExtraContents set BitMap:piSelectorsMap,KSelectorsMap   
}


#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
sequence append DRecoSequence DRecoTagInterface
sequence append DRecoSequence DRecoTagDump
sequence append DRecoSequence DRecoCleanUp
sequence append DRecoSequence BtuTupleMaker

path append Everything DRecoSequence 

#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
#these produce ntuples in root file
module disable ChmPi0Analysis
module disable ChmMiniAnalysisDeltaM
module disable ChmMiniAnalysis

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






