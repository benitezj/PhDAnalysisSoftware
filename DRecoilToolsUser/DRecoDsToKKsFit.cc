#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDsToKKsFit.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"
//#include "CharmTagTools/ChmTagToolsSequence.hh"
//#include "CharmTagTools/ChmSlowPionSelector.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"

#include "TrkBase/TrkAbsFit.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "Beta/BtaAbsVertex.hh"
#include "ProbTools/probab.hh"

#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "BetaMicroAdapter/BtaPidInfo.hh"
#include "BetaMicroAdapter/BtaIfrQual.hh"

#include "TrkBase/TrkRecoTrk.hh"
#include "BtaTupleMaker/BtuUtil.hh"
#include "BbrGeom/BbrVectorErr.hh"

#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaFoxWolfMom.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "BetaCoreTools/BtaPrintTree.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BetaCoreTools/BtaThrustFitter.hh"

using std::cout ;
using std::endl ;
using std::ostream ;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoDsToKKsFit::DRecoDsToKKsFit( const char* const theName,const char* const theDescription )
  :        TagFilterModule (  theName, theDescription                           )
  ,             FitProbCut ( "FitProbCut"         , this , 0.                   )
  ,            KChargeList ( "KChargeList"        , this , "ChargedTracks"      )
  ,              trackList ( "trackList"          , this , "ChargedTracks"      )

  ,            chargedList ( "chargedList"        , this , "ChargedTracks"      )
  ,            neutralList ( "neutralList"        , this , "CalorNeutral"       )
  ,                 KsList ( "KsList"             , this , "KsDefault"          ) 
  ,               _scalers ( "DRecoDsToKKsFit"                                  )
  ,      DRecoUsrBlockName ( "DRecoUsrBlockKKs" , this , "DRecoUsrBlockKKs"     )

  ,     TagUncMass ( "TagUncMass"     , 0.   )
  ,    DsStUncMass ( "DsStUncMass"    , 0.   )
  ,      DsUncMass ( "DsUncMass"      , 0.   )
  ,         DsMass ( "DsMass"         , 0.   )
   				            
  ,       qualSigK ( "qualSigK"       , 0    )
   				            
  ,     MyGamMCIdx ( "MyGamMCIdx"     , -1   )
  ,     MyTagMCIdx ( "MyTagMCIdx"     , -1   )
  ,       MyKMCIdx ( "MyKMCIdx"       , -1   )
  ,    MySigKMCIdx ( "MySigKMCIdx"    , -1   )
   				            

   //Missing Mass Raw  (MMRaw) 
  ,       ProbChi2MMRaw ( "DsToKKsProbChi2MMRaw", 0    )
  ,           Chi2MMRaw ( "DsToKKsChi2MMRaw"    , 0    )
  ,           ndofMMRaw ( "DsToKKsndofMMRaw"    , 0    )				              				            
  ,      KsmassMMRaw ( "KsmassMMRaw"      , 0    )
   				            
  ,        KspxMMRaw ( "KspxMMRaw"        , 0    )
  ,        KspyMMRaw ( "KspyMMRaw"        , 0    )
  ,        KspzMMRaw ( "KspzMMRaw"        , 0    )
  ,    KsenergyMMRaw ( "KsenergyMMRaw"    , 0    )

  ,        KpxMMRaw ( "KpxMMRaw"          , 0    )
  ,        KpyMMRaw ( "KpyMMRaw"          , 0    )
  ,        KpzMMRaw ( "KpzMMRaw"          , 0    )
  ,    KenergyMMRaw ( "KenergyMMRaw"      , 0    )

  ,      DsmassMMRaw ( "DsmassMMRaw"      , 0    )
  ,        DspxMMRaw ( "DspxMMRaw"        , 0    )
  ,        DspyMMRaw ( "DspyMMRaw"        , 0    )
  ,        DspzMMRaw ( "DspzMMRaw"        , 0    )
  ,    DsenergyMMRaw ( "DsenergyMMRaw"    , 0    )
   				            
  //Missing Mass Fitted Constrained (MMCFit) 


  ,       ProbChi2MMCFit ( "DsToKKsProbChi2MMFit", 0    )
  ,           Chi2MMCFit ( "DsToKKsChi2MMFit"    , 0    )
  ,           ndofMMCFit ( "DsToKKsndofMMFit"    , 0    )
   				            
  ,      KsmassMMCFit ( "KsmassMMCFit"      , 0    )
   				            
  ,        KspxMMCFit ( "KspxMMCFit"        , 0    )
  ,        KspyMMCFit ( "KspyMMCFit"        , 0    )
  ,        KspzMMCFit ( "KspzMMCFit"        , 0    )
  ,    KsenergyMMCFit ( "KsenergyMMCFit"    , 0    )  				      		 
  ,        KpxMMCFit ( "KpxMMCFit"          , 0    )
  ,        KpyMMCFit ( "KpyMMCFit"          , 0    )
  ,        KpzMMCFit ( "KpzMMCFit"          , 0    )
  ,    KenergyMMCFit ( "KenergyMMCFit"      , 0    )

  ,      DsmassMMCFit ( "DsmassMMCFit"      , 0    )   				            
  ,        DspxMMCFit ( "DspxMMCFit"        , 0    )
  ,        DspyMMCFit ( "DspyMMCFit"        , 0    )
  ,        DspzMMCFit ( "DspzMMCFit"        , 0    )
  ,    DsenergyMMCFit ( "DsenergyMMCFit"    , 0    ) 

  ,        ThrustX ( "ThrustX"        , -999 )
  ,        ThrustY ( "ThrustY"        , -999 )
  ,        ThrustZ ( "ThrustZ"        , -999 )
  ,      ThrustMag ( "ThrustMag"      , -999 )
   				            
  ,             R0 ( "R0"             , -999 )
  ,             R1 ( "R1"             , -999 )
  ,             R2 ( "R2"             , -999 )
  ,             R3 ( "R3"             , -999 )
  ,             R4 ( "R4"             , -999 )
   				            
  ,  EExtraNeutral ( "EExtraNeutral"  , -999 )
  ,  NExtraNeutral ( "NExtraNeutral"  , -999 )
   				            
  ,     PMissCosTh ( "PMissCosTh"     , -999 )
  ,       PMissPhi ( "PMissPhi"       , -999 )
  ,          PMiss ( "PMiss"          , -999 )
  ,          EMiss ( "EMiss"          , -999 )
 

//List Fitted Unconstrained Variables	(ListUFit)					 
  ,          DspxListUFit ( "DspxListUFit"          , 0    )
  ,          DspyListUFit ( "DspyListUFit"          , 0    )
  ,          DspzListUFit ( "DspzListUFit"          , 0    )
  ,      DsenergyListUFit ( "DsenergyListUFit"      , 0    )
  ,        DsmassListUFit ( "DsmassListUFit"        , 0    )
   		 		        	 
  ,           KpxListUFit ( "KpxListUFit"           , 0    )
  ,           KpyListUFit ( "KpyListUFit"           , 0    )
  ,           KpzListUFit ( "KpzListUFit"           , 0    )
  ,       KenergyListUFit ( "KenergyListUFit"       , 0    )
  ,         KmassListUFit ( "KmassListUFit"         , 0    )
   		 		        	 
  ,          KspxListUFit ( "KspxListUFit"          , 0    )
  ,          KspyListUFit ( "KspyListUFit"          , 0    )
  ,          KspzListUFit ( "KspzListUFit"          , 0    )
  ,      KsenergyListUFit ( "KsenergyListUFit"      , 0    )
  ,        KsmassListUFit ( "KsmassListUFit"        , 0    )
   		 
  ,        Ksd1pxListUFit ( "Ksd1pxListUFit"          , 0    )
  ,        Ksd1pyListUFit ( "Ksd1pyListUFit"          , 0    )
  ,        Ksd1pzListUFit ( "Ksd1pzListUFit"          , 0    )
  ,    Ksd1energyListUFit ( "Ksd1energyListUFit"      , 0    )
  ,      Ksd1massListUFit ( "Ksd1massListUFit"        , 0    )
   		   		      
  ,        Ksd2pxListUFit ( "Ksd2pxListUFit"          , 0    )
  ,        Ksd2pyListUFit ( "Ksd2pyListUFit"          , 0    )
  ,        Ksd2pzListUFit ( "Ksd2pzListUFit"          , 0    )
  ,    Ksd2energyListUFit ( "Ksd2energyListUFit"      , 0    )
  ,      Ksd2massListUFit ( "Ksd2massListUFit"        , 0    )
   
  ,     ProbChi2ListUFit ( "DsToKKsProbChi2ListUFit", 0   )
  ,         Chi2ListUFit ( "DsToKKsChi2ListUFit"    , 0   )
  ,         ndofListUFit ( "DsToKKsndofListUFit"    , 0   )
  ,        FitStatusUFit ( "DsToKKsFitStatusUFit"   , 0   )

  //List Fitted Constrained Variables (ListCFit)
  ,          DspxListCFit ( "DspxListCFit"          , 0    )
  ,          DspyListCFit ( "DspyListCFit"          , 0    )
  ,          DspzListCFit ( "DspzListCFit"          , 0    )
  ,      DsenergyListCFit ( "DsenergyListCFit"      , 0    )
  ,        DsmassListCFit ( "DsmassListCFit"        , 0    )
   		     		        	 
  ,           KpxListCFit ( "KpxListCFit"           , 0    )
  ,           KpyListCFit ( "KpyListCFit"           , 0    )
  ,           KpzListCFit ( "KpzListCFit"           , 0    )
  ,       KenergyListCFit ( "KenergyListCFit"       , 0    )
  ,         KmassListCFit ( "KmassListCFit"         , 0    )
   		     		        	 
  ,          KspxListCFit ( "KspxListCFit"          , 0    )
  ,          KspyListCFit ( "KspyListCFit"          , 0    )
  ,          KspzListCFit ( "KspzListCFit"          , 0    )
  ,      KsenergyListCFit ( "KsenergyListCFit"      , 0    )
  ,        KsmassListCFit ( "KsmassListCFit"        , 0    )
   		     
  ,        Ksd1pxListCFit ( "Ksd1pxListCFit"          , 0    )
  ,        Ksd1pyListCFit ( "Ksd1pyListCFit"          , 0    )
  ,        Ksd1pzListCFit ( "Ksd1pzListCFit"          , 0    )
  ,    Ksd1energyListCFit ( "Ksd1energyListCFit"      , 0    )
  ,      Ksd1massListCFit ( "Ksd1massListCFit"        , 0    )
   		     		      
  ,        Ksd2pxListCFit ( "Ksd2pxListCFit"          , 0    )
  ,        Ksd2pyListCFit ( "Ksd2pyListCFit"          , 0    )
  ,        Ksd2pzListCFit ( "Ksd2pzListCFit"          , 0    )
  ,    Ksd2energyListCFit ( "Ksd2energyListCFit"      , 0    )
  ,      Ksd2massListCFit ( "Ksd2massListCFit"        , 0    )
   		     
  ,      ProbChi2ListCFit ( "DsToKKsProbChi2ListCFit", 0   )
  ,          Chi2ListCFit ( "DsToKKsChi2ListCFit"    , 0   )
  ,          ndofListCFit ( "DsToKKsndofListCFit"    , 0   )
  ,         FitStatusCFit ( "DsToKKsFitStatusCFit"   , 0   )

  //List Raw Variables (ListRaw)
  ,          DspxListRaw ( "DspxListRaw"          , 0    )
  ,          DspyListRaw ( "DspyListRaw"          , 0    )
  ,          DspzListRaw ( "DspzListRaw"          , 0    )
  ,      DsenergyListRaw ( "DsenergyListRaw"      , 0    )
  ,        DsmassListRaw ( "DsmassListRaw"        , 0    )
   		     	        	 
  ,           KpxListRaw ( "KpxListRaw"           , 0    )
  ,           KpyListRaw ( "KpyListRaw"           , 0    )
  ,           KpzListRaw ( "KpzListRaw"           , 0    )
  ,       KenergyListRaw ( "KenergyListRaw"       , 0    )
  ,         KmassListRaw ( "KmassListRaw"         , 0    )
   		     	        	 
  ,          KspxListRaw ( "KspxListRaw"          , 0    )
  ,          KspyListRaw ( "KspyListRaw"          , 0    )
  ,          KspzListRaw ( "KspzListRaw"          , 0    )
  ,      KsenergyListRaw ( "KsenergyListRaw"      , 0    )
  ,        KsmassListRaw ( "KsmassListRaw"        , 0    )
   		      
  ,        Ksd1pxListRaw ( "Ksd1pxListRaw"          , 0    )
  ,        Ksd1pyListRaw ( "Ksd1pyListRaw"          , 0    )
  ,        Ksd1pzListRaw ( "Ksd1pzListRaw"          , 0    )
  ,    Ksd1energyListRaw ( "Ksd1energyListRaw"      , 0    )
  ,      Ksd1massListRaw ( "Ksd1massListRaw"        , 0    )
   		     	      
  ,        Ksd2pxListRaw ( "Ksd2pxListRaw"          , 0    )
  ,        Ksd2pyListRaw ( "Ksd2pyListRaw"          , 0    )
  ,        Ksd2pzListRaw ( "Ksd2pzListRaw"          , 0    )
  ,    Ksd2energyListRaw ( "Ksd2energyListRaw"      , 0    )
  ,      Ksd2massListRaw ( "Ksd2massListRaw"        , 0    )
   		     
  ,    ProbChi2ListRaw ( "DsToKKsProbChi2ListRaw", 0   )
  ,        Chi2ListRaw ( "DsToKKsChi2ListRaw"    , 0   )
  ,        ndofListRaw ( "DsToKKsndofListRaw"    , 0   )

   
{
  commands()->append( &FitProbCut       ) ;
  commands()->append( &KChargeList      ) ;
  commands()->append( &trackList        ) ;
  commands()->append( &chargedList      ) ;
  commands()->append( &neutralList      ) ;
  commands()->append( &KsList           ) ;



  DRecoUsrBlock.addVariable( TagUncMass    ) ;
  DRecoUsrBlock.addVariable( DsStUncMass   ) ;
  DRecoUsrBlock.addVariable( DsUncMass     ) ;
  DRecoUsrBlock.addVariable( DsMass        ) ;
					  
					  
  DRecoUsrBlock.addVariable( qualSigK      ) ;
					  
  DRecoUsrBlock.addVariable( MyGamMCIdx    ) ;
  DRecoUsrBlock.addVariable( MyTagMCIdx    ) ;
  DRecoUsrBlock.addVariable( MyKMCIdx      ) ;
  DRecoUsrBlock.addVariable( MySigKMCIdx   ) ;
					  
					  
  DRecoUsrBlock.addVariable( ProbChi2MMRaw   ) ;
  DRecoUsrBlock.addVariable( Chi2MMRaw       ) ;
  DRecoUsrBlock.addVariable( ndofMMRaw       ) ;					  
  DRecoUsrBlock.addVariable( KsmassMMRaw     ) ; 					  
  DRecoUsrBlock.addVariable( KspxMMRaw       ) ;
  DRecoUsrBlock.addVariable( KspyMMRaw       ) ;
  DRecoUsrBlock.addVariable( KspzMMRaw       ) ;
  DRecoUsrBlock.addVariable( KsenergyMMRaw   ) ;					  
  DRecoUsrBlock.addVariable( KpxMMRaw        ) ;
  DRecoUsrBlock.addVariable( KpyMMRaw        ) ;
  DRecoUsrBlock.addVariable( KpzMMRaw        ) ;
  DRecoUsrBlock.addVariable( KenergyMMRaw    ) ;
  DRecoUsrBlock.addVariable( DsmassMMRaw     ) ; 					  
  DRecoUsrBlock.addVariable( DspxMMRaw       ) ;
  DRecoUsrBlock.addVariable( DspyMMRaw       ) ;
  DRecoUsrBlock.addVariable( DspzMMRaw       ) ;
  DRecoUsrBlock.addVariable( DsenergyMMRaw   ) ;


  DRecoUsrBlock.addVariable( ProbChi2MMCFit   ) ;
  DRecoUsrBlock.addVariable( Chi2MMCFit       ) ;
  DRecoUsrBlock.addVariable( ndofMMCFit       ) ;					  
  DRecoUsrBlock.addVariable( KsmassMMCFit     ) ;  					  
  DRecoUsrBlock.addVariable( KspxMMCFit       ) ;
  DRecoUsrBlock.addVariable( KspyMMCFit       ) ;
  DRecoUsrBlock.addVariable( KspzMMCFit       ) ;
  DRecoUsrBlock.addVariable( KsenergyMMCFit   ) ;					  
  DRecoUsrBlock.addVariable( KpxMMCFit        ) ;
  DRecoUsrBlock.addVariable( KpyMMCFit        ) ;
  DRecoUsrBlock.addVariable( KpzMMCFit        ) ;
  DRecoUsrBlock.addVariable( KenergyMMCFit    ) ;
  DRecoUsrBlock.addVariable( DsmassMMCFit     ) ;  					  
  DRecoUsrBlock.addVariable( DspxMMCFit       ) ;
  DRecoUsrBlock.addVariable( DspyMMCFit       ) ;
  DRecoUsrBlock.addVariable( DspzMMCFit       ) ;
  DRecoUsrBlock.addVariable( DsenergyMMCFit   ) ;


					  
  DRecoUsrBlock.addVariable( ThrustX       ) ;
  DRecoUsrBlock.addVariable( ThrustY       ) ;
  DRecoUsrBlock.addVariable( ThrustZ       ) ;
  DRecoUsrBlock.addVariable( ThrustMag     ) ;
					  
  DRecoUsrBlock.addVariable( R0            ) ;
  DRecoUsrBlock.addVariable( R1            ) ;
  DRecoUsrBlock.addVariable( R2            ) ;
  DRecoUsrBlock.addVariable( R3            ) ;
  DRecoUsrBlock.addVariable( R4            ) ;

  DRecoUsrBlock.addVariable( EExtraNeutral ) ;
  DRecoUsrBlock.addVariable( NExtraNeutral ) ;
					  
  DRecoUsrBlock.addVariable( PMissCosTh    ) ;
  DRecoUsrBlock.addVariable( PMissPhi      ) ;
  DRecoUsrBlock.addVariable( PMiss         ) ;
  DRecoUsrBlock.addVariable( EMiss         ) ;
					  
  //new variables			  
  DRecoUsrBlock.addVariable( DspxListUFit          ) ;
  DRecoUsrBlock.addVariable( DspyListUFit          ) ;
  DRecoUsrBlock.addVariable( DspzListUFit          ) ;
  DRecoUsrBlock.addVariable( DsenergyListUFit      ) ;
  DRecoUsrBlock.addVariable( DsmassListUFit        ) ;					  
  DRecoUsrBlock.addVariable( KpxListUFit           ) ;
  DRecoUsrBlock.addVariable( KpyListUFit           ) ;
  DRecoUsrBlock.addVariable( KpzListUFit           ) ;
  DRecoUsrBlock.addVariable( KenergyListUFit       ) ;
  DRecoUsrBlock.addVariable( KmassListUFit         ) ;					  
  DRecoUsrBlock.addVariable( KspxListUFit          ) ;
  DRecoUsrBlock.addVariable( KspyListUFit          ) ;
  DRecoUsrBlock.addVariable( KspzListUFit          ) ;
  DRecoUsrBlock.addVariable( KsenergyListUFit      ) ;
  DRecoUsrBlock.addVariable( KsmassListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1pxListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1pyListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1pzListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1energyListUFit    ) ;
  DRecoUsrBlock.addVariable( Ksd1massListUFit      ) ;			     		   
  DRecoUsrBlock.addVariable( Ksd2pxListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd2pyListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd2pzListUFit        ) ;
  DRecoUsrBlock.addVariable( Ksd2energyListUFit    ) ;
  DRecoUsrBlock.addVariable( Ksd2massListUFit      ) ;					   
  DRecoUsrBlock.addVariable( ProbChi2ListUFit      ) ;
  DRecoUsrBlock.addVariable( Chi2ListUFit          ) ;
  DRecoUsrBlock.addVariable( ndofListUFit          ) ; 
  DRecoUsrBlock.addVariable( FitStatusUFit         ) ;

  DRecoUsrBlock.addVariable( DspxListCFit          ) ;
  DRecoUsrBlock.addVariable( DspyListCFit          ) ;
  DRecoUsrBlock.addVariable( DspzListCFit          ) ;
  DRecoUsrBlock.addVariable( DsenergyListCFit      ) ;
  DRecoUsrBlock.addVariable( DsmassListCFit        ) ;					  
  DRecoUsrBlock.addVariable( KpxListCFit           ) ;
  DRecoUsrBlock.addVariable( KpyListCFit           ) ;
  DRecoUsrBlock.addVariable( KpzListCFit           ) ;
  DRecoUsrBlock.addVariable( KenergyListCFit       ) ;
  DRecoUsrBlock.addVariable( KmassListCFit         ) ;					  
  DRecoUsrBlock.addVariable( KspxListCFit          ) ;
  DRecoUsrBlock.addVariable( KspyListCFit          ) ;
  DRecoUsrBlock.addVariable( KspzListCFit          ) ;
  DRecoUsrBlock.addVariable( KsenergyListCFit      ) ;
  DRecoUsrBlock.addVariable( KsmassListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1pxListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1pyListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1pzListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd1energyListCFit    ) ;
  DRecoUsrBlock.addVariable( Ksd1massListCFit      ) ;			     		   
  DRecoUsrBlock.addVariable( Ksd2pxListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd2pyListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd2pzListCFit        ) ;
  DRecoUsrBlock.addVariable( Ksd2energyListCFit    ) ;
  DRecoUsrBlock.addVariable( Ksd2massListCFit      ) ;					   
  DRecoUsrBlock.addVariable( ProbChi2ListCFit      ) ;
  DRecoUsrBlock.addVariable( Chi2ListCFit          ) ;
  DRecoUsrBlock.addVariable( ndofListCFit          ) ;
  DRecoUsrBlock.addVariable( FitStatusCFit         ) ;  

  DRecoUsrBlock.addVariable( DspxListRaw          ) ;
  DRecoUsrBlock.addVariable( DspyListRaw          ) ;
  DRecoUsrBlock.addVariable( DspzListRaw          ) ;
  DRecoUsrBlock.addVariable( DsenergyListRaw      ) ;
  DRecoUsrBlock.addVariable( DsmassListRaw        ) ;					  
  DRecoUsrBlock.addVariable( KpxListRaw           ) ;
  DRecoUsrBlock.addVariable( KpyListRaw           ) ;
  DRecoUsrBlock.addVariable( KpzListRaw           ) ;
  DRecoUsrBlock.addVariable( KenergyListRaw       ) ;
  DRecoUsrBlock.addVariable( KmassListRaw         ) ;					  
  DRecoUsrBlock.addVariable( KspxListRaw          ) ;
  DRecoUsrBlock.addVariable( KspyListRaw          ) ;
  DRecoUsrBlock.addVariable( KspzListRaw          ) ;
  DRecoUsrBlock.addVariable( KsenergyListRaw      ) ;
  DRecoUsrBlock.addVariable( KsmassListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd1pxListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd1pyListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd1pzListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd1energyListRaw    ) ;
  DRecoUsrBlock.addVariable( Ksd1massListRaw      ) ;			     		   
  DRecoUsrBlock.addVariable( Ksd2pxListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd2pyListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd2pzListRaw        ) ;
  DRecoUsrBlock.addVariable( Ksd2energyListRaw    ) ;
  DRecoUsrBlock.addVariable( Ksd2massListRaw      ) ;					   
  DRecoUsrBlock.addVariable( ProbChi2ListRaw      ) ;
  DRecoUsrBlock.addVariable( Chi2ListRaw          ) ;
  DRecoUsrBlock.addVariable( ndofListRaw          ) ; 

  

}

//--------------
// Destructor --
//--------------
DRecoDsToKKsFit::~DRecoDsToKKsFit()
{}

//--------------
// Operations --
//--------------
AppResult DRecoDsToKKsFit::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDsToKKsFit::event( AbsEvent* anEvent )
{

  _scalers.sum("#  events ") ;
  
  // Create new UsrBlock
  UsrIfd<UsrCandBlock>::put(anEvent,&DRecoUsrBlock,DRecoUsrBlockName.value()) ;
  
  // This block comes from previous module
  UsrCandBlock* DRecoUsrBlockIn = Ifd<UsrCandBlock>::get(anEvent,"DRecoUsrBlock") ;
  
  // Create evt list and add to event
  HepAList<BtaCandidate>* output_list = new HepAList<BtaCandidate> ;
  if(output_list==0) ErrMsg(fatal) << "Failed to create an output_list list" << endmsg ;
  
  // Create proxy to dump Ups_out list
  IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(output_list) ;
  if(proxy == 0) ErrMsg(fatal) << "Failed to create output_list list proxy" << endmsg ;
  if(!Ifd<HepAList<BtaCandidate> >::put(anEvent, proxy,"DsToKKsCandidateList"))
    ErrMsg(fatal) << "failed to add DsToKKsCandidateList list to event using proxy" << endmsg ; 
  
  // Retreive the event candidates
  HepAList< BtaCandidate >* DRecoEvtCandidateList = Ifd<HepAList<BtaCandidate > >::get(anEvent,"DRecoCandidateList") ;
  if(DRecoEvtCandidateList==0)  ErrMsg(fatal) <<"DRecoCandidateList not found" << endmsg ;
  
  // Pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default") ;
  if(eventInfoListPtr==0) ErrMsg(fatal) << "Could not locate event info list in the event. This is likely to be a configuration error." << endmsg ;
  EventInfo* eventInfo = eventInfoListPtr->first() ;
  if(eventInfo==0) ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg ;
  
  // The beam spot below has been smeared in MC and fixed error=10mu data/MC
  const BbrPointErr beamSpot = eventInfoListPtr->first()->beamSpot() ;
  
  // Get the MC Truth info
  BtaMcAssoc* _truthMap = Ifd< BtaMcAssoc >::get( anEvent,"GHit") ;
  HepAList<BtaCandidate>* _mcList(0) ;
  if(_truthMap) _mcList = BtuUtil::rearrangeMcList( Ifd< HepAList<BtaCandidate> >::get(anEvent,"MCTruth" )) ;    
  
 // Get signal lists
  const std::string&    kaonListKey = KChargeList.valueString() ;
  HepAList<BtaCandidate>*    kaon_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, kaonListKey.c_str()) ;
  if(kaon_list==0)    ErrMsg(fatal) << "Could not locate the list " << KChargeList.valueString()    << ".  This is likely a configuration error." << endmsg ;
  HepAListIterator<BtaCandidate>    kaon_iterator(*kaon_list) ;
  
  const std::string&    trackListKey = trackList.valueString() ;
  HepAList<BtaCandidate>*    track_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, trackListKey.c_str()) ;
  if(track_list==0)    ErrMsg(fatal) << "Could not locate the list " << trackList.valueString()    << ".  This is likely a configuration error." << endmsg ;
  HepAListIterator<BtaCandidate>    track_iterator(*track_list) ;
  
  HepAList<BtaCandidate>* signal_list = kaon_list ;
  
  // Get list of EvtCandidates from previous module
  HepAListIterator<BtaCandidate> Ups_iterator(*DRecoEvtCandidateList) ;
  int n_cands = 0 ;
  
  while(BtaCandidate* Ups_in = Ups_iterator())
  { // Loop over EvtCandidates
    _scalers.sum("initial lists") ;	 
    
    //these particles are from the DKXg tag
    BtaCandidate*  recCharm  = NULL ;
    BtaCandidate* recCharmMM = NULL ;
    BtaCandidate* recCharmL  = NULL ;
    
    BtaCandidate*   recKaon  = NULL ;
    BtaCandidate*  recKaonMM = NULL ;
    BtaCandidate*  recKaonL  = NULL ;
    
    BtaCandidate*   recfrag  = NULL ;
    BtaCandidate*  recfragMM = NULL ;
    BtaCandidate*  recfragL  = NULL ;
    
    BtaCandidate*    recgam  = NULL ;
    BtaCandidate*   recgamMM = NULL ;
    BtaCandidate*   recgamL  = NULL ;
    
    BtaCandidate*   recDsSt  = NULL ;
    BtaCandidate*  recDsStMM = NULL ;
    BtaCandidate*  recDsStL  = NULL ;
    
    BtaCandidate*     recDs  = NULL ;
    BtaCandidate*    recDsMM = NULL ;
    BtaCandidate*    recDsL  = NULL ;
    
    BtaCandidate*     recPK  = NULL ;
    BtaCandidate*    recPKMM = NULL ;
    BtaCandidate*    recPKL  = NULL ;
    
    HepAListIterator<BtaCandidate> Ups_daughter_iterator = Ups_in->daughterIterator() ; 
    
    while(BtaCandidate* Ups_daughter = Ups_daughter_iterator())
    {
      switch( abs(Ups_daughter->pdtEntry()->lundId()))
      {
      case 411:     //D+
      case 421:     //D0
      case 413:     //D+*
      case 423:     //D0*
      case 4122:    //LambdaC
      case 431:     //Ds
	recCharm = Ups_daughter;
	break;
	
      case 321:    //K+
      case 310:    //Ks
	recKaon = Ups_daughter;
	break;
	
      case 433:   //Ds*
	recDsSt = Ups_daughter;
	break;
	
      case 5132:  //Chib
      case 5232:  //Chib0
	recPK = Ups_daughter;
	break;
	
      default:     //pions
	recfrag = Ups_daughter;
	break;
      }
    }
    
    HepAListIterator<BtaCandidate> DsSt_daughter_iterator = recDsSt->daughterIterator() ;
    while(BtaCandidate* DsSt_daughter = DsSt_daughter_iterator())
    {
      if( abs(DsSt_daughter->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::gamma    )->lundId())) recgam = DsSt_daughter ;
      if( abs(DsSt_daughter->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_plus )->lundId()))  recDs = DsSt_daughter ;
    }
    if(recCharm)
    {
      recCharmMM = recCharm ;
      recCharmL  = recCharm ;
    }
    if(recKaon)
    {
      recKaonMM = recKaon ;
      recKaonL  = recKaon ;
    }
    if(recfrag)
    {
      recfragMM = recfrag ;
      recfragL  = recfrag ;
    } 
    if(recgam)
    {
      recgamMM = recgam ;
      recgamL  = recgam ;
    }
    if(recDsSt)
    {
      recDsStMM = recDsSt ;
      recDsStL  = recDsSt ;
    }
    if(recDs)
    {
      recDsMM = recDs ;
      recDsL  = recDs ;
    }	  
	  
    int CharmLund = 0;
    if(recCharm)
    {
      CharmLund = (recCharm->pdtEntry()->lundId());
      switch(abs(CharmLund))
      {
      case 411 :  // D plus
      case 421 :  // D0
      case 413 :  // D*+  
      case 423 :  // D*0   
	if(!recKaon) ErrMsg(fatal) << "Cannot find required kaon aborting" << endmsg;
	break;
      case 4122 :
	if(!recPK)  ErrMsg(fatal) << "Cannot find required Lambda aborting" << endmsg;
	break;
      case 431 :
	break;
      default:
	ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
      }
    }
    else ErrMsg(fatal) << "Cannot find charm tag aborting" << endmsg;
    
    float dsMass = recDs->p4().mag() ;
    DsMass = dsMass ;
      
    //Kaon Block
    HepAListIterator<BtaCandidate> iter(*signal_list) ;
    iter.rewind() ;
    while(BtaCandidate *recSigKaonTemp = iter())
    { // Loop over kaons
      BtaCandidate* recSigKaon = new BtaCandidate(*recSigKaonTemp) ;
      if(recSigKaon->charge()>0) recSigKaon->setType(Pdt::lookup(PdtLund::K_plus )) ;
      if(recSigKaon->charge()<0) recSigKaon->setType(Pdt::lookup(PdtLund::K_minus)) ;
      
      BtaCandidate* recSigKaonMM = recSigKaon;
      BtaCandidate* recSigKaonL  = recSigKaon;
      if(output_list->length()>=300) continue ;
      //test for overlap; skip event if true
      if(recSigKaon->overlaps(*Ups_in)) continue ;
      _scalers.sum("no K+ overlap") ;
      //if(recSigKaon->charge()*recDs->charge()<0) continue ;
      
      const std::string& chargedListKey = chargedList.valueString() ;
      HepAList<BtaCandidate>* charged_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, chargedListKey.c_str()) ;
      if(charged_list==0) ErrMsg(fatal) << "Could not locate the list " << chargedList.valueString() << ". This is likely a configuration error." << endmsg ;
      HepAListIterator<BtaCandidate> charged_iterator(*charged_list) ;
      
      //Ks Block
      const std::string& ksListKey = KsList.valueString() ;
      HepAList<BtaCandidate>* ks_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, ksListKey.c_str()) ;
      if(ks_list==0) ErrMsg(fatal) << "Could not locate the list " << KsList.valueString()    << ".  This is likely a configuration error." << endmsg ;
      HepAListIterator<BtaCandidate> ks_iterator(*ks_list) ;
      
      while(BtaCandidate *recSigKs = ks_iterator())
      { // Loop over k shorts
	if(output_list->length()>=300) continue ;
	//test for overlap; skip event if true
	if(recSigKs->overlaps(*Ups_in)) continue ;
	if(recSigKs->overlaps(*recSigKaon)) continue ;
	_scalers.sum("no Ks overlap") ;
	
	const std::string& chargedListKey = chargedList.valueString() ;
	HepAList<BtaCandidate>* charged_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, chargedListKey.c_str()) ;
	if(charged_list==0) ErrMsg(fatal) << "Could not locate the list " << chargedList.valueString() << ". This is likely a configuration error." << endmsg ;
	HepAListIterator<BtaCandidate> charged_iterator(*charged_list) ;
	
	// Compute missing 4-vector
	BbrLorentzVectorErr p4ups = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;
	HepLorentzVector Ksp4miss = recDs->p4() - recSigKaon->p4() ;
	
	KspxMMRaw     = Ksp4miss.px() ;
	KspyMMRaw     = Ksp4miss.py() ;
	KspzMMRaw     = Ksp4miss.pz() ;
	KsenergyMMRaw = Ksp4miss.e() ;
	KsmassMMRaw   = Ksp4miss.m() ;
	
	KpxMMRaw      = recSigKaon->p4().px() ;
	KpyMMRaw      = recSigKaon->p4().py() ;
	KpzMMRaw      = recSigKaon->p4().pz() ;
	KenergyMMRaw  = recSigKaon->p4().e() ;
	
	BbrVectorErr eMinusMom(eventInfo->electronBeam().p3WCov()) ;
	BbrVectorErr  ePlusMom(eventInfo->positronBeam().p3WCov()) ;
	eMinusMom.setCovMatrix(eMinusMom.covMatrix()*2.5 ) ;
	ePlusMom.setCovMatrix(ePlusMom.covMatrix()*2.5) ;
	
	//-----------------------------------------------
	// Now create the Missing Mass Ups
	//-----------------------------------------------
	BtaCandidate candKsMM(Ksp4miss) ;
	candKsMM.setType(Pdt::lookup(PdtLund::K_S0)) ;
	setBeamConstraint(candKsMM, eventInfo) ;
	
	BtaOpMakeTree combDsMM ;  
	BtaCandidate candDsMM = combDsMM.combine(candKsMM, *recSigKaonMM) ;
	candDsMM.setType(recDs->pdtEntry()) ;
	setMassConstraint(candDsMM) ;
	
	BtaOpMakeTree combDsStMM ;
	BtaCandidate candDsStMM = combDsStMM.combine(candDsMM, *recgamMM) ;
	candDsStMM.setType(recDsSt->pdtEntry()) ;
	setMassConstraint(candDsStMM) ;
	
	BtaOpMakeTree combUpsMM ;
	BtaCandidate candUpsMM ;
	
	switch(abs(CharmLund))
	{
	case  411 ://D+
	case  421 ://D0
	case  413 ://D+*
	case  423 ://D0*
	  if(recfrag!=NULL) { candUpsMM = combUpsMM.combine(*recCharmMM,  candDsStMM, *recKaonMM, *recfragMM) ; }
	  else { candUpsMM = combUpsMM.combine(*recCharmMM, candDsStMM, *recKaonMM) ; }
	  break;
	case 4122 :
	  if(recfrag!=NULL) { candUpsMM = combUpsMM.combine(*recCharmMM, candDsStMM, *recPKMM, candDsStMM) ; }
	  else { candUpsMM = combUpsMM.combine(*recCharmMM, candDsStMM, *recPKMM) ; }
	  break;
	case 431 :
	  if(recfrag!=NULL) { candUpsMM = combUpsMM.combine(*recCharmMM, candDsStMM, *recfragMM) ; }
	  else { candUpsMM = combUpsMM.combine(*recCharmMM, candDsMM) ; }
	  break;
	default:
	  ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
	}
	candUpsMM.setType(Pdt::lookup(PdtLund::Upsilon_4S)) ;
	

	//----------------------------------
	// Fit the event
	//----------------------------------
	
	setBeamConstraint(candUpsMM, eventInfo) ;
	setEnergyConstraint(candUpsMM, eMinusMom, ePlusMom, 0.5) ;
	// Fit the upsilon
	VtxFitterOper ksfitterMM(candUpsMM,VtxAbsAlgorithm::TreeFitter) ;
	ksfitterMM.invalidateFit() ;
	ksfitterMM.fitAll() ;
	candUpsMM = ksfitterMM.getFittedTree() ;
	if((candUpsMM.decayVtx())->status()!=BtaAbsVertex::Success) continue ;
	int nDofMM = (int)candUpsMM.decayVtx()->nDof() ;
	float chiSquaredMM =  candUpsMM.decayVtx()->chiSquared() ;
	if(probab(nDofMM, chiSquaredMM) < FitProbCut.value()) continue ;
	ProbChi2MMCFit = probab(nDofMM, chiSquaredMM) ;
	Chi2MMCFit = chiSquaredMM ;
	ndofMMCFit = nDofMM ;
	_scalers.sum("fit-prob") ;


	BtaCandidate KsFittedMM = ksfitterMM.getFitted(candKsMM); 
	KspxMMCFit     = KsFittedMM.p4().px(); 
	KspyMMCFit     = KsFittedMM.p4().py(); 
	KspzMMCFit     = KsFittedMM.p4().pz(); 
	KsenergyMMCFit = KsFittedMM.p4().e(); 
	KsmassMMCFit   = KsFittedMM.p4().m(); 

	BtaCandidate DsFittedMM = ksfitterMM.getFitted(candDsMM); 
	DspxMMCFit     = DsFittedMM.p4().px(); 
	DspyMMCFit     = DsFittedMM.p4().py(); 
	DspzMMCFit     = DsFittedMM.p4().pz(); 
	DsenergyMMCFit = DsFittedMM.p4().e(); 
	DsmassMMCFit   = DsFittedMM.p4().m(); 


	
	//-----------------------------------------------
	// Now create the Ks fit using list Ks's--do not constrain DsMass
	// Create the New Ups
	// Append "UList" to variable names to distinguish from above
	//-----------------------------------------------
	

	BtaCandidate candKsUList(*recSigKs) ;
	setBeamConstraint(candKsUList, recSigKaonL) ;
	BtaOpMakeTree combDsUList ;   
	BtaCandidate candDsUList = combDsUList.combine(candKsUList, *recSigKaonL) ;
	setBeamConstraint(candDsUList, eventInfo) ;
	candDsUList.setType(recDsL->pdtEntry())  ;

	BtaOpMakeTree combDsStUList ;
	BtaCandidate candDsStUList = combDsStUList.combine(candDsUList, *recgamL) ;
	candDsStUList.setType(recDsStL->pdtEntry())  ;
	setMassConstraint(candDsStUList) ;
	BtaOpMakeTree combUpsUList ;
	BtaCandidate candUpsUList ;

	KspxListRaw       = recSigKs->p4().px();
	KspyListRaw       = recSigKs->p4().py();
	KspzListRaw       = recSigKs->p4().pz();
	KsenergyListRaw   = recSigKs->p4().e();
	KsmassListRaw     = recSigKs->p4().m();

	DspxListRaw       = candDsUList.p4().px();
	DspyListRaw       = candDsUList.p4().py();
	DspzListRaw       = candDsUList.p4().pz();
	DsenergyListRaw   = candDsUList.p4().e();
	DsmassListRaw     = candDsUList.p4().m();

	
	switch(abs(CharmLund))
	{
	case 411:
	case 421:
	case 413: //D+*
	case 423: //D0*
	  if(recfrag!=NULL) { candUpsUList = combUpsUList.combine(*recCharmL, candDsStUList, *recKaonL, *recfragL) ; }
	  else { candUpsUList = combUpsUList.combine(*recCharmL, candDsStUList, *recKaonL) ; }
	  break;
	case 4122:
	  if(recfrag!=NULL) { candUpsUList = combUpsUList.combine(*recCharmL, candDsStUList, *recPKL, *recfragL) ; }
	  else { candUpsUList = combUpsUList.combine(*recCharmL, candDsStUList, *recPKL) ; }
	  break;
	case 431:
	  if(recfrag!=NULL) { candUpsUList = combUpsUList.combine(*recCharmL, candDsStUList, *recfragL) ; }
	  else { candUpsUList = combUpsUList.combine(*recCharmL, candDsStUList) ; }
	  break;
	default:
	  ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
	}
	candUpsUList.setType(Pdt::lookup(PdtLund::Upsilon_4S)) ; 
	
	VtxFitterOper ksfitterUList(candUpsUList,VtxAbsAlgorithm::TreeFitter) ;
	ksfitterUList.invalidateFit() ;
	ksfitterUList.fitAll() ;
	
	// Extract 
	candUpsUList = ksfitterUList.getFittedTree() ;
	
	//	if((candUpsUList.decayVtx())->status()!=BtaAbsVertex::Success) continue ;
	//	_scalers.sum("fit-success") ;
	
	int nDofUList = (int)candUpsUList.decayVtx()->nDof() ;
	float chiSquaredUList =  candUpsUList.decayVtx()->chiSquared() ;
	ProbChi2ListUFit = probab(nDofUList, chiSquaredUList) ;
	Chi2ListUFit = chiSquaredUList ;
	ndofListUFit = nDofUList ;
	int FitStatusUFitTemp = candUpsUList.decayVtx()->status() ;
	FitStatusUFit = FitStatusUFitTemp ;
	//	_scalers.sum("fit-prob") ;

	
	//must also use daughter iterator to fill fields of Ks daughters
	HepAListIterator<BtaCandidate> UKsd_iterator = recSigKs->daughterIterator() ;
	BtaCandidate* UfittedKsd1 = UKsd_iterator() ;
	BtaCandidate* UfittedKsd2 = UKsd_iterator() ;


	Ksd1pxListUFit     = UfittedKsd1->p4().px(); 
	Ksd1pyListUFit     = UfittedKsd1->p4().py();
	Ksd1pzListUFit     = UfittedKsd1->p4().pz();
	Ksd1energyListUFit = UfittedKsd1->p4().e();
	Ksd1massListUFit   = UfittedKsd1->p4().m();
	Ksd2pxListUFit     = UfittedKsd2->p4().px(); 
	Ksd2pyListUFit     = UfittedKsd2->p4().py();
	Ksd2pzListUFit     = UfittedKsd2->p4().pz();
	Ksd2energyListUFit = UfittedKsd2->p4().e();
	Ksd2massListUFit   = UfittedKsd2->p4().m();

 
	BtaCandidate UfittedDs = ksfitterUList.getFitted(candDsUList); 
	DspxListUFit     = UfittedDs.p4().px();
	DspyListUFit     = UfittedDs.p4().py();
	DspzListUFit     = UfittedDs.p4().pz();
	DsenergyListUFit = UfittedDs.p4().e();
	DsmassListUFit   = UfittedDs.p4().m();

	
	BtaCandidate UfittedKs = ksfitterUList.getFitted(candKsUList); 
	KspxListUFit     = UfittedKs.p4().px(); 
	KspyListUFit     = UfittedKs.p4().py(); 
	KspzListUFit     = UfittedKs.p4().pz(); 
	KsenergyListUFit = UfittedKs.p4().e(); 
	KsmassListUFit   = UfittedKs.p4().m(); 



	//--------------------------------------------
	//
	//Still using List Kaons
	//Repeat fit, but Constrain Ds Mass
	//Append "CList" to distinguish from above
	//--------------------------------------------



	BtaCandidate candKsCList(*recSigKs) ;
	BtaOpMakeTree combDsCList ;   
	BtaCandidate candDsCList = combDsCList.combine(candKsCList, *recSigKaonL) ;
	candDsCList.setType(recDsL->pdtEntry())  ;
       	setMassConstraint(candDsCList) ;
	BtaOpMakeTree combDsStCList ;
	BtaCandidate candDsStCList = combDsStCList.combine(candDsCList, *recgamL) ;
	candDsStCList.setType(recDsStL->pdtEntry())  ;
	setMassConstraint(candDsStCList) ;
	BtaOpMakeTree combUpsCList ;
	BtaCandidate candUpsCList ;
	
	switch(abs(CharmLund))
	{
	case 411:
	case 421:
	case 413: //D+*
	case 423: //D0*
	  if(recfrag!=NULL) { candUpsCList = combUpsCList.combine(*recCharmL, candDsStCList, *recKaonL, *recfragL) ; }
	  else { candUpsCList = combUpsCList.combine(*recCharmL, candDsStCList, *recKaonL) ; }
	  break;
	case 4122:
	  if(recfrag!=NULL) { candUpsCList = combUpsCList.combine(*recCharmL, candDsStCList, *recPKL, *recfragL) ; }
	  else { candUpsCList = combUpsCList.combine(*recCharmL, candDsStCList, *recPKL) ; }
	  break;
	case 431:
	  if(recfrag!=NULL) { candUpsCList = combUpsCList.combine(*recCharmL, candDsStCList, *recfragL) ; }
	  else { candUpsCList = combUpsCList.combine(*recCharmL, candDsStCList) ; }
	  break;
	default:
	  ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
	}
	candUpsCList.setType(Pdt::lookup(PdtLund::Upsilon_4S)) ; 
	
	VtxFitterOper ksfitterCList(candUpsCList,VtxAbsAlgorithm::TreeFitter) ;
	ksfitterCList.invalidateFit() ;
	ksfitterCList.fitAll() ;
	
	// Extract 
      	candUpsCList = ksfitterCList.getFittedTree() ;
	
	//	if((candUpsCList.decayVtx())->status()!=BtaAbsVertex::Success) continue ;
	//	_scalers.sum("fit-success") ;
	
	int nDofCList = (int)candUpsCList.decayVtx()->nDof() ;
	float chiSquaredCList =  candUpsCList.decayVtx()->chiSquared() ;
	ProbChi2ListCFit = probab(nDofCList, chiSquaredCList) ;
	Chi2ListCFit = chiSquaredCList ;
	ndofListCFit = nDofCList ;
	int FitStatusCFitTemp = candUpsCList.decayVtx()->status() ;
	FitStatusCFit = FitStatusCFitTemp ;
	//	_scalers.sum("fit-prob") ;

	
	//must also use daughter iterator to fill fields of Ks daughters
	HepAListIterator<BtaCandidate> CKsd_iterator = recSigKs->daughterIterator() ;
	BtaCandidate* CfittedKsd1 = CKsd_iterator() ;
	BtaCandidate* CfittedKsd2 = CKsd_iterator() ;


	Ksd1pxListCFit     = CfittedKsd1->p4().px(); 
	Ksd1pyListCFit     = CfittedKsd1->p4().py();
	Ksd1pzListCFit     = CfittedKsd1->p4().pz();
	Ksd1energyListCFit = CfittedKsd1->p4().e();
	Ksd1massListCFit   = CfittedKsd1->p4().m();
	Ksd2pxListCFit     = CfittedKsd2->p4().px(); 
	Ksd2pyListCFit     = CfittedKsd2->p4().py();
	Ksd2pzListCFit     = CfittedKsd2->p4().pz();
	Ksd2energyListCFit = CfittedKsd2->p4().e();
	Ksd2massListCFit   = CfittedKsd2->p4().m();

 
	BtaCandidate CfittedDs = ksfitterCList.getFitted(candDsCList); // get the fitted version of the candDs2
	DspxListCFit     = CfittedDs.p4().px();
	DspyListCFit     = CfittedDs.p4().py();
	DspzListCFit     = CfittedDs.p4().pz();
	DsenergyListCFit = CfittedDs.p4().e();
	DsmassListCFit   = CfittedDs.p4().m();

	
	BtaCandidate CfittedKs = ksfitterCList.getFitted(candKsCList); // get the fitted version of the candDs2
	KspxListCFit     = CfittedKs.p4().px(); 
	KspyListCFit     = CfittedKs.p4().py(); 
	KspzListCFit     = CfittedKs.p4().pz(); 
	KsenergyListCFit = CfittedKs.p4().e(); 
	KsmassListCFit   = CfittedKs.p4().m(); 




	//----------------------------------
	// Quality of kaon
	//----------------------------------
	qualSigK = 1 ;
	const BtaTrkQual* TrkQual = recSigKaon->getMicroAdapter()->getTrkQual() ;
	int n_SVTHits   = TrkQual->nSvtHits() ;
	int n_DCHHits   = TrkQual->nDchHits() ;
	int SVT_pattern = TrkQual->SvtPattern() ;
	int n_SVTzHits  = 0 ;
	int power_of_2  = 2 ;
	for(int i=0 ; i<10 ; i++)
	{ // Quality
	  if(SVT_pattern%power_of_2 && i>4) n_SVTzHits++ ;
	  SVT_pattern = SVT_pattern - SVT_pattern%power_of_2 ;
	  power_of_2 = power_of_2*2 ;
	}
	if(n_SVTHits<4)  qualSigK = 0 ;
	if(n_SVTzHits<2) qualSigK = 0 ;
	if(n_DCHHits<12) qualSigK = 0 ;

	
	//----------------------------------
	// Get the thrust, Fox-Wolfram moments, visible charge
	//----------------------------------
	
	const std::string& neutralListKey = neutralList.valueString() ;	
	HepAList<BtaCandidate>* neutral_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, neutralListKey.c_str()) ;
	if(neutral_list==0) ErrMsg(fatal) << "Could not locate the list " << neutralList.valueString() << ". This is likely a configuration error." << endmsg ;


	HepAListIterator<BtaCandidate> neutral_iterator(*neutral_list) ;
	BbrLorentzVectorErr eep4 = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;
	float PMiss_x    = eep4.x() ;
	float PMiss_y    = eep4.y() ;
	float PMiss_z    = eep4.z() ;
	float EMiss_temp = eep4.e() ;
	charged_iterator.rewind() ;
	int n_extra_neutral = 0 ;
	float extra_energy_neutral = 0 ;
	HepAList<BtaCandidate>* all_list = new HepAList<BtaCandidate> ;
	while(BtaCandidate* neutral = neutral_iterator())
	{
	  PMiss_x    = PMiss_x    - neutral->p4().x() ;
	  PMiss_y    = PMiss_y    - neutral->p4().y() ;
	  PMiss_z    = PMiss_z    - neutral->p4().z() ;
	  EMiss_temp = EMiss_temp - neutral->p4().e() ;
	  all_list->append(neutral);
	  if( !neutral->overlaps(candUpsMM) && neutral->energy()>=0.03 )
	  {
	    extra_energy_neutral += neutral->energy() ;
	    n_extra_neutral++ ;
	  }
	}

	while(BtaCandidate* charged = charged_iterator())
	{
	  all_list->append( charged ) ;
	  PMiss_x    = PMiss_x    - charged->p4().x() ;
	  PMiss_y    = PMiss_y    - charged->p4().y() ;
	  PMiss_z    = PMiss_z    - charged->p4().z() ;
	  EMiss_temp = EMiss_temp - charged->p4().e() ;	     	 
	}
	
	neutral_iterator.rewind() ;
	EExtraNeutral = extra_energy_neutral ;
	NExtraNeutral = n_extra_neutral ;
	
	// Thrust
	BtaThrustFitter thruster ;
	thruster.compute(*all_list) ;
	ThrustX = thruster.thrustAxis().x() ;
	ThrustY = thruster.thrustAxis().y() ;
	ThrustZ = thruster.thrustAxis().z() ;
	ThrustMag = thruster.thrust() ;
	
	// Fox-Wolfram moments
	BtaFoxWolfMom foxWolf(all_list, eventInfo, 4) ;
	R0 = foxWolf.FWzero() ;
	R1 = foxWolf.FWnorm(1) ;
	R2 = foxWolf.FWnorm(2) ;
	R3 = foxWolf.FWnorm(3) ;
	R4 = foxWolf.FWnorm(4) ;
	
	//----------------------------------
	// Fill UsrData
	//----------------------------------
	DRecoUsrBlockIn->get(*Ups_in,  TagUncMass  ) ;
	DRecoUsrBlockIn->get(*Ups_in,  DsStUncMass ) ;
	DRecoUsrBlockIn->get(*Ups_in,  DsUncMass   ) ;
	DRecoUsrBlockIn->get(*Ups_in,  MyGamMCIdx  ) ;
	DRecoUsrBlockIn->get(*Ups_in,  MyKMCIdx    ) ;
	DRecoUsrBlockIn->get(*Ups_in,  MyTagMCIdx  ) ;
	
	PMiss_x = PMiss_x ;
	PMiss_y = PMiss_y ;
	PMiss_z = PMiss_z ;
	EMiss   = EMiss ;
	float PMiss_temp = sqrt(PMiss_x*PMiss_x + PMiss_y*PMiss_y + PMiss_z*PMiss_z) ;
	PMissCosTh = PMiss_z/PMiss_temp ;
	PMissPhi   = atan2(PMiss_y, PMiss_x) ;
	PMiss      = PMiss_temp ;
	EMiss      = EMiss_temp ;
	
	if( !DRecoUsrBlock.put( candUpsMM , TagUncMass     )) cout << "Error saving TagUncMass UsrData"     << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsStUncMass    )) cout << "Error saving DsStUncMass UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsUncMass      )) cout << "Error saving DsUncMass UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsMass         )) cout << "Error saving DsMass UsrData"         << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , qualSigK       )) cout << "Error saving qualSigK UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , MyGamMCIdx     )) cout << "Error saving MyMuMCIdx UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , MyKMCIdx       )) cout << "Error saving MyKMCIdx UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , MyTagMCIdx     )) cout << "Error saving MyTagMCIdx UsrData"     << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , MySigKMCIdx    )) cout << "Error saving MyEMCIdx UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ProbChi2MMRaw  )) cout << "Error saving ProbChi2 UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Chi2MMRaw      )) cout << "Error saving Chi2 UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ndofMMRaw      )) cout << "Error saving ndof UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsmassMMRaw      )) cout << "Error saving NuMassRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspxMMRaw        )) cout << "Error saving KspxMMRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspyMMRaw        )) cout << "Error saving KspyMMRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspzMMRaw        )) cout << "Error saving KspzMMRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsenergyMMRaw    )) cout << "Error saving KsenergyMMRaw UsrData"   << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpxMMRaw         )) cout << "Error saving KpxMMRaw UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpyMMRaw         )) cout << "Error saving KpyMMRaw UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpzMMRaw         )) cout << "Error saving KpzMMRaw UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KenergyMMRaw     )) cout << "Error saving KenergyMMRaw UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsmassMMRaw      )) cout << "Error saving DsMassRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspxMMRaw        )) cout << "Error saving DspxMMRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspyMMRaw        )) cout << "Error saving DspyMMRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspzMMRaw        )) cout << "Error saving DspzMMRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsenergyMMRaw    )) cout << "Error saving DsenergyMMRaw UsrData"   << endl ;


	if( !DRecoUsrBlock.put( candUpsMM , ProbChi2MMCFit  )) cout << "Error saving ProbChi2 UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Chi2MMCFit      )) cout << "Error saving Chi2 UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ndofMMCFit      )) cout << "Error saving ndof UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsmassMMCFit      )) cout << "Error saving NuMassRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspxMMCFit        )) cout << "Error saving KspxMMCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspyMMCFit        )) cout << "Error saving KspyMMCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspzMMCFit        )) cout << "Error saving KspzMMCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsenergyMMCFit    )) cout << "Error saving KsenergyMMCFit UsrData"   << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpxMMCFit         )) cout << "Error saving KpxMMCFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpyMMCFit         )) cout << "Error saving KpyMMCFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpzMMCFit         )) cout << "Error saving KpzMMCFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KenergyMMCFit     )) cout << "Error saving KenergyMMCFit UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsmassMMCFit      )) cout << "Error saving DsMassRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspxMMCFit        )) cout << "Error saving DspxMMCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspyMMCFit        )) cout << "Error saving DspyMMCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspzMMCFit        )) cout << "Error saving DspzMMCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsenergyMMCFit    )) cout << "Error saving DsenergyMMCFit UsrData"   << endl ;

	if( !DRecoUsrBlock.put( candUpsMM , ThrustX        )) cout << "Error saving thrust_x UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ThrustY        )) cout << "Error saving thrust_y UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ThrustZ        )) cout << "Error saving thrust_z UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ThrustMag      )) cout << "Error saving thrust_mag UsrData"     << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , R0             )) cout << "Error saving R0 UsrData"             << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , R1             )) cout << "Error saving R1 UsrData"             << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , R2             )) cout << "Error saving R2 UsrData"             << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , R3             )) cout << "Error saving R3 UsrData"             << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , R4             )) cout << "Error saving R4 UsrData"             << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , EExtraNeutral  )) cout << "Error saving EExtraNeutral UsrData"  << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , NExtraNeutral  )) cout << "Error saving NExtraNeutral UsrData"  << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , PMissCosTh     )) cout << "Error saving PMissCosTh UsrData"     << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , PMissPhi       )) cout << "Error saving PMissPhi UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , PMiss          )) cout << "Error saving PMiss UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , EMiss          )) cout << "Error saving EMiss UsrData"          << endl ;
	//new variables added	       
	if( !DRecoUsrBlock.put( candUpsMM , DspxListRaw           )) cout << "Error saving DspxListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspyListRaw           )) cout << "Error saving DspyListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspzListRaw           )) cout << "Error saving DspzListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsenergyListRaw       )) cout << "Error saving DsenergyListRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsmassListRaw         )) cout << "Error saving DsmassListRaw UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpxListRaw            )) cout << "Error saving KpxListRaw UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpyListRaw            )) cout << "Error saving KpyListRaw UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpzListRaw            )) cout << "Error saving KpzListRaw UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KenergyListRaw        )) cout << "Error saving KenergyListRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KmassListRaw          )) cout << "Error saving KmassListRaw UsrData"         << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspxListRaw           )) cout << "Error saving KspxListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspyListRaw           )) cout << "Error saving KspyListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspzListRaw           )) cout << "Error saving KspzListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsenergyListRaw       )) cout << "Error saving KsenergyListRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsmassListRaw         )) cout << "Error saving KsmassListRaw UsrData"        << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pxListRaw         )) cout << "Error saving Ksd1spxListRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pyListRaw         )) cout << "Error saving Ksd1pyListRaw UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pzListRaw         )) cout << "Error saving Ksd1pzListRaw UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1energyListRaw     )) cout << "Error saving Ksd1energyListRaw UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1massListRaw       )) cout << "Error saving Ksd1massListRaw UsrData"      << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pxListRaw         )) cout << "Error saving Ksd2spxListRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pyListRaw         )) cout << "Error saving Ksd2py2ListRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pzListRaw         )) cout << "Error saving Ksd2pz2ListRaw UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2energyListRaw     )) cout << "Error saving Ksd2energyListRaw UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2massListRaw       )) cout << "Error saving Ksd2masListRaws UsrData"      << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , ProbChi2ListRaw       )) cout << "Error saving ProbChi2ListRaw UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Chi2ListRaw           )) cout << "Error saving Chi2ListRaw UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ndofListRaw           )) cout << "Error saving ndofListRaw UsrData"          << endl ;
	
	if( !DRecoUsrBlock.put( candUpsMM , DspxListCFit          )) cout << "Error saving DspxListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspyListCFit          )) cout << "Error saving DspyListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspzListCFit          )) cout << "Error saving DspzListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsenergyListCFit      )) cout << "Error saving DsenergyListCFit UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsmassListCFit        )) cout << "Error saving DsmassListCFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpxListCFit           )) cout << "Error saving KpxListCFit UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpyListCFit           )) cout << "Error saving KpyListCFit UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpzListCFit           )) cout << "Error saving KpzListCFit UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KenergyListCFit       )) cout << "Error saving KenergyListCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KmassListCFit         )) cout << "Error saving KmassListCFit UsrData"         << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspxListCFit          )) cout << "Error saving KspxListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspyListCFit          )) cout << "Error saving KspyListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspzListCFit          )) cout << "Error saving KspzListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsenergyListCFit      )) cout << "Error saving KsenergyListCFit UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsmassListCFit        )) cout << "Error saving KsmassListCFit UsrData"        << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pxListCFit        )) cout << "Error saving Ksd1spxListCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pyListCFit        )) cout << "Error saving Ksd1pyListCFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pzListCFit        )) cout << "Error saving Ksd1pzListCFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1energyListCFit    )) cout << "Error saving Ksd1energyListCFit UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1massListCFit      )) cout << "Error saving Ksd1massListCFit UsrData"      << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pxListCFit        )) cout << "Error saving Ksd2spxListCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pyListCFit        )) cout << "Error saving Ksd2py2ListCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pzListCFit        )) cout << "Error saving Ksd2pz2ListCFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2energyListCFit    )) cout << "Error saving Ksd2energyListCFit UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2massListCFit      )) cout << "Error saving Ksd2massListCFit UsrData"      << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , ProbChi2ListCFit      )) cout << "Error saving ProbChi2ListCFit UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Chi2ListCFit          )) cout << "Error saving Chi2ListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ndofListCFit          )) cout << "Error saving ndofListCFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , FitStatusCFit         )) cout << "Error saving FitStatusCFit UsrData"         << endl ;

	if( !DRecoUsrBlock.put( candUpsMM , DspxListUFit          )) cout << "Error saving DspxListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspyListUFit          )) cout << "Error saving DspyListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DspzListUFit          )) cout << "Error saving DspzListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsenergyListUFit      )) cout << "Error saving DsenergyListUFit UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , DsmassListUFit        )) cout << "Error saving DsmassListUFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpxListUFit           )) cout << "Error saving KpxListUFit UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpyListUFit           )) cout << "Error saving KpyListUFit UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KpzListUFit           )) cout << "Error saving KpzListUFit UsrData"           << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KenergyListUFit       )) cout << "Error saving KenergyListUFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KmassListUFit         )) cout << "Error saving KmassListUFit UsrData"         << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspxListUFit          )) cout << "Error saving KspxListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspyListUFit          )) cout << "Error saving KspyListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KspzListUFit          )) cout << "Error saving KspzListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsenergyListUFit      )) cout << "Error saving KsenergyListUFit UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , KsmassListUFit        )) cout << "Error saving KsmassListUFit UsrData"        << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pxListUFit        )) cout << "Error saving Ksd1spxListUFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pyListUFit        )) cout << "Error saving Ksd1pyListUFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1pzListUFit        )) cout << "Error saving Ksd1pzListUFit UsrData"        << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1energyListUFit    )) cout << "Error saving Ksd1energyListUFit UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd1massListUFit      )) cout << "Error saving Ksd1massListUFit UsrData"      << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pxListUFit        )) cout << "Error saving Ksd2spxListUFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pyListUFit        )) cout << "Error saving Ksd2py2ListUFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2pzListUFit        )) cout << "Error saving Ksd2pz2ListUFit UsrData"       << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2energyListUFit    )) cout << "Error saving Ksd2energyListUFit UsrData"    << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Ksd2massListUFit      )) cout << "Error saving Ksd2massListUFit UsrData"      << endl ;	
	if( !DRecoUsrBlock.put( candUpsMM , ProbChi2ListUFit      )) cout << "Error saving ProbChi2ListUFit UsrData"      << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , Chi2ListUFit          )) cout << "Error saving Chi2ListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , ndofListUFit          )) cout << "Error saving ndofListUFit UsrData"          << endl ;
	if( !DRecoUsrBlock.put( candUpsMM , FitStatusUFit         )) cout << "Error saving FitStatusUFit UsrData"         << endl ;


	n_cands++ ;
	output_list->append(new BtaCandidate(candUpsMM)) ;
	_scalers.sum("final lists") ;
      } //Ks Iterator
    } // K iterator
  }// signal mode iterator
  return AppResult::OK ;
}


AppResult DRecoDsToKKsFit::endJob( AbsEvent* anEvent )
{
  // endJob
  ostream& o=cout ;
  _scalers.print(o) ;
  o << endl ;

  return AppResult::OK ;
}

/*
HepLorentzVector DRecoDsToEllNuFit::nup4Corrected(HepLorentzVector nup4, HepLorentzVector Ellp4, int signal_mode)
{ 
  // nup3 correction
  // Steve's numerical method re-hashed
  const double mDs_PDG = Pdt::lookup(PdtLund::D_s_plus)->mass() ;
  const double mMu_PDG =  Pdt::lookup(PdtLund::mu_plus)->mass() ;
  const double  mE_PDG =   Pdt::lookup(PdtLund::e_plus)->mass() ;

  double mEll = 0 ;
  if(signal_mode==0) mEll = mMu_PDG ;
  if(signal_mode==1) mEll = mE_PDG ;
  if(signal_mode==2) mEll = mMu_PDG ;
  if(signal_mode==3) mEll = mE_PDG ;
  if(mEll==0) ErrMsg(fatal) << "Cannot determine signal mode.  The mass of the lepton must be known for nup4 correction" << endmsg ;
  double M2 = (mDs_PDG*mDs_PDG-mEll*mEll)*0.5 ;
  double E_Ell = Ellp4.e() ;
  double p_Ell = Ellp4.rho() ;

  // Determine the distance of closest approach of the missing momentun recoiling against the DKXgamma system.
  // On-shell solutions fall on an ellipse defined by
  //
  // cos(psi) = [ E_ell - (m_Ds^2 - m_ell^2)/(2p_nu) ]/p_ell
  //                         or
  // p_nu = [ m_Ds^2 - m_ell^2 ]/2*( E_ell - p_ell*cos(psi) )
  //
  // Rotate about psi until a minimum solution is found

  const double pi = 3.14159265 ;
  int number_of_steps = 1000 ;
  double psi_step_size = 2*pi/number_of_steps ;
  double closest_distance = 1.e6 ;
  double closest_psi = -2 ;
  double second_closest_distance = 1.e6 ;
  double second_closest_psi = -2 ;

  Hep3Vector nup3 = nup4.vect() ;
  Hep3Vector Ellp3 = Ellp4.vect() ;
  Hep3Vector rotation_axis = nup3.unit().cross(Ellp3.unit()) ;

  Hep3Vector closest_p3(0,0,0) ;

  for(int i=0 ; i<number_of_steps ; i++)
  {
    double angle = i*psi_step_size ;
    Hep3Vector rotated_nup3 = nup3 ;
    rotated_nup3.rotate(angle, rotation_axis.unit()) ;
    double psi = Ellp3.angle(rotated_nup3) ;
    double p_nu_mag = M2/(E_Ell-p_Ell*cos(psi)) ;
    rotated_nup3.setMag(p_nu_mag) ;
    Hep3Vector difference = rotated_nup3 - nup3 ;
    double distance = difference.mag() ;

    if(distance<closest_distance)
    {
      second_closest_distance = closest_distance ;
      second_closest_psi = closest_psi ;
      closest_distance = distance ;
      closest_psi = psi ;
      closest_p3 = rotated_nup3 ;
    }
    else
    {
      if(distance<second_closest_distance)
      {
	second_closest_distance = distance ;
	second_closest_psi = psi ;
      }
    }
  }

  double lower_distance = 1.e6 ;
  double lower_psi = 1.e6 ;
  double upper_distance = 1.e6 ;
  double upper_psi = 1.e6 ;
  if(closest_psi<second_closest_psi)
  {
    lower_distance = closest_distance ;
    lower_psi = closest_psi ;
    upper_distance = second_closest_distance ;
    upper_psi = second_closest_psi ;
  }
  else
  {
    upper_distance = closest_distance ;
    upper_psi = closest_psi ;
    lower_distance = second_closest_distance ;
    lower_psi = second_closest_psi ;
  }

  HepLorentzVector nup4_new(closest_p3, closest_p3.mag()) ;
  return nup4_new ;
}
*/

