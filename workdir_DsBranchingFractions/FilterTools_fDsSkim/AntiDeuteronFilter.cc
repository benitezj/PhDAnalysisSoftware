//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AntiDeuteronFilter.cc,v 1.8 2007/12/15 15:00:20 avtelnov Exp $
//
// Description:
//	Class AntiDeuteronFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Valerie Halyo                  Original Author
//
// Copyright Information:
//	Copyright (C) 1997
//
// History:
//      Alexandre Telnov (Princeton University), 2007/12/14:
//         removed explicit calls to DchBetheBloch::ionization();
//         rewrote all SVT and DCH dE/dx-related functions to use 
//         the new BetaPid/PidDEDXCdbXxx methods.
//               
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "AntiDeuteronFilter.hh"
#include "CLHEP/Random/RandFlat.h"
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
// #ifdef __REL12__
// #include "CLHEP/AList.h"
// #else
// #include "ProxyDict/IfdHepAList.hh"
// #endif

#include "BtaEnv/BtaEnv.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "Beta/BtaCandidate.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"

#include "PDT/PdtEntry.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtPid.hh"

#include "PepEnv/PepEnv.hh"
#include "PepEnvData/PepBeams.hh"  
#include "BbrGeom/BbrPointErr.hh" 
#include "TrajGeom/TrkLineTraj.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "BetaRecoAdapter/BtaAbsRecoObject.hh"

//#include "DchCalib/DchBetheBloch.hh"
#include "BetaPid/PidDEDXCdbDch.hh"
#include "BetaPid/PidDEDXCdbSvt.hh"

#include "DchEnv/DchEnv.hh"
#include "TrkBase/TrkFit.hh"
#include "TrkBase/TrkPoca.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkDifTraj.hh"

using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

// in general, a module constructor should not do much.  The begin(job) or
// begin(run) members are better places to put initialization
AntiDeuteronFilter::AntiDeuteronFilter( const char* const theName, 
					const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _eventInfoList( "eventInfoList", this, "Default" )
  , _btaChargedList("trackCandidates",this,"ChargedTracks")
{
  commands()->append( & _eventInfoList);
  commands()->append(& _btaChargedList );
}

//--------------
// Destructor --
//--------------

// The destructor should be limited to undoing the work of the constructor
AntiDeuteronFilter::~AntiDeuteronFilter() {
}


//--------------
// Operations --
//--------------


// The begin(AppJob*) member function is run before any events are
// processed.  In sample code, it opens the output histogram file
// and then books a histogram and optionally an ntuple.
AppResult
AntiDeuteronFilter::beginJob( AbsEvent* anEvent ) {

  _nEventsRead = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}

// end(AppJob*) function is called after all events have been processed.

AppResult
AntiDeuteronFilter::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  cout << "AntiDeuteron Filter:" << endl
       << "Read " << _nEventsRead << endl
       << "Passed " << _nEventsPassed << endl;        

  return AppResult::OK;
}


// event function is called once per event
AppResult
AntiDeuteronFilter::event( AbsEvent* anEvent ) {
  /////////////////////////////////
  //
  // get everything from the event
  //
  /////////////////////////////////
  // Keep a count of how many events have been processed 
  _nEventsRead++;

  // get event info 
  HepAList< EventInfo > * infoList  = Ifd< HepAList< EventInfo > >::get(anEvent, _eventInfoList.value() ); assert( infoList );
  EventInfo             * eventInfo = infoList->first(); assert( eventInfo );

  //   BtaBooster              theBooster( eventInfo->cmFrame() );
  // get charged trks
  HepAList<BtaCandidate>* trkList;
  trkList = 
    Ifd< HepAList<BtaCandidate> >::get(anEvent, _btaChargedList.value() );
  if ( 0 == trkList ) {
    ErrMsg(warning) << "no charged trkList found, skipping event" << endmsg;
    return AppResult::OK;
  }
  // Get primary vertex
				 
  double xPrimaryVtx(-1000.) ; 
  double yPrimaryVtx(-1000.) ; 
  double zPrimaryVtx(-1000.) ; 
   
  const BtaAbsVertex* pVtx = eventInfo->primaryVtx();
   
  if ( pVtx != 0 )
    {
      xPrimaryVtx = pVtx->point().x();
      yPrimaryVtx = pVtx->point().y();
      zPrimaryVtx = pVtx->point().z();
       
    }else { 
      // cout << name() << ": no primary vertex " << endl;
    }

  const double primaryvtx(pVtx!=0 ? xPrimaryVtx : -99.) ;
  const double primaryvty(pVtx!=0 ? yPrimaryVtx : -99.) ;
  const double primaryvtz(pVtx!=0 ? zPrimaryVtx : -99.) ;

  HepPoint PrimVtx(primaryvtx,primaryvty,primaryvtz);

  // Get BeamSpot

  const PepBeams * pepBeams = gblEnv->getPep()->pepBeams();
  const Hep3Vector boostVector = -(pepBeams->boostCMtoLab());
  const BbrPointErr & beamSpot = pepBeams->interaction3Point();

  CollisionAxisSlope(beamSpot);

  double xZTilt=0;
  double yZTilt=0;
  double xYTilt=0;

  xZTilt = XZTilt();
  yZTilt = YZTilt();
  xYTilt = XYTilt();

  // Build a trajectory along the beamspot direction
  Hep3Vector BeamDirection(xZTilt, yZTilt, 1.);
  BeamDirection = BeamDirection.unit();
  TrkLineTraj BeamTrajectory(beamSpot, BeamDirection, -9999, 9999);

#if 1
  const BbrLorentzVectorErr &ip(gblEnv->getBta()->pepBeams()->interactionPoint());
  const HepPoint bs(ip.x(), ip.y(), ip.z());
#endif

  // I am going to set 4 trigger lines for this job 
  /////////////////////////////////////////////////
  // p<2 de/dx DCA>>  prescaled
  // p>2 de/dx && DIRC DCA>> prescaled
  // p<2 de/dx DCA<< 
  // p>2 de/dx && DIRC DCA<<

  vector<int> ldcaldedx,hdcaldedx,alldedxlp,alldedxdirchp,ldcahdedxdirc,hdcahdedxdirc;
  vector<int> ldcaldedxneg,ldcaldedxpos,ldcaldedxnev,ldcahdedxdircpos,ldcahdedxdircneg;

  const BtaPidQual* trkPidQual(NULL);
  const BtaTrkQual *trkTrkQual(NULL);

  HepAListIterator<BtaCandidate> itertrk(*trkList);
  BtaCandidate *trk(NULL);	
  int  count(0);

  while( trk = itertrk() ) {    
    trkPidQual=trk->getMicroAdapter()->getPidQual();
    trkTrkQual=trk->getMicroAdapter()->getTrkQual();

    if(trkPidQual&&trkTrkQual){
      PdtEntry* proton ;
      int q = (int) trk->charge() ;
      proton = Pdt::lookup(PdtPid::proton,q) ; 
      
      int ringmin = (int)((trkPidQual->ringNExPhot(proton)-2*sqrt(1.*trkPidQual->ringNExPhot(proton))));
      
      if(ringmin<10) ringmin = 10;
      if(ringmin>15) ringmin = 15;

      ClosestApproach(BeamTrajectory,trk);

      double   trkDocaxy = docaxy();
      double docaZ(-99);
      docaZ = fabs(poca().z()-bs.z());

      int Dchhits(40);
      double trkPt(-99.);
      trkPt = trk->pt();
	
      if  (trkPt>.1)  Dchhits = trkTrkQual->nDchHits();
	
      if(Dchhits>12)  {    
	  	  
	if( ((trk->p()<0.5)&&(consistencysvt(trk)>-0.4))  
	    ||  ((trk->p()<1.8)&&(consistency(trk)>-0.4)) ){
	  alldedxlp.push_back(count);
	  //	if(trk->docaXY(PrimVtx)/trk->docaXYErr(PrimVtx)<3){
	  if(trkDocaxy<0.3&&docaZ<7.){
	    if(trk->charge()>0.5){
	      ldcaldedxpos.push_back(count);
	    }
	    if(trk->charge()<-0.5){
	      ldcaldedxneg.push_back(count);
	    }
	  }else{
	    hdcaldedx.push_back(count);
	  }
	}else if((trk->p()>1.74)&&(trk->p()<5.)&&(consistency(trk)>-0.4)&&
		 (trkPidQual->ringNExPhot(proton)>4)&&
		 (trkPidQual->ringNPhot()<ringmin)){
	    
	  alldedxdirchp.push_back(count);
	    
	  if(trkDocaxy<0.3&&docaZ<7.){
	    if(trk->charge()>0.5){
	      ldcahdedxdircpos.push_back(count);
	    }
	    if(trk->charge()<-0.5){
	      ldcahdedxdircneg.push_back(count);
	    }
	  }else{
	    hdcahdedxdirc.push_back(count);
	  }
	}
      }
    }    
  
    count++;
  }

  
  itertrk.rewind();

  int  passprescaled1(0),passprescaled2(0);
  if((hdcaldedx.size()==alldedxlp.size())&&(hdcaldedx.size()>0)){
    if(RandFlat::shoot()<0.01){
      passprescaled1=1;
    }
  }

  if((hdcahdedxdirc.size()==alldedxdirchp.size())&&(hdcahdedxdirc.size()>0)){
    if(RandFlat::shoot()<0.01){
      passprescaled2=1;
    }
  }
	
  int  passprescaled1a(0),passprescaled2a(0); 
  if(ldcaldedxpos.size()){
    if (RandFlat::shoot()<0.01){
      passprescaled1a=1;  
    }
  }

  if(ldcahdedxdircpos.size()){
    if (RandFlat::shoot()<0.01){
      passprescaled2a=1;  
    }
  }


  if(ldcaldedxneg.size()>0||ldcahdedxdircneg.size()>0||
     passprescaled1||passprescaled2||
     passprescaled1a || passprescaled2a){
    _nEventsPassed++;
    setPassed(1);
  }else   setPassed(0);




  //if (passed()) ErrMsg(routine) << " event Micro Filter" << (passed()?" passed":" failed") << endmsg;

  // done
  return AppResult::OK;
}


// avtelnov, 2007/12/13: completely rewrote AntiDeuteronFilter::consistency 
// to use the new detailed DCH dE/dx parameterization.
// Logic originally intended by Valerie Halyo is preserved.
double 
AntiDeuteronFilter::consistency(const BtaCandidate *c) const {

  static const float d_MASS = 1.875613; // deutron mass
  
// return -1500. if something is broken
  if (c==0) return -1500.;
  const BtaPidQual* pQual = c->getMicroAdapter()->getPidQual();
  if (pQual==0) return -1500.;
  
  double measured = pQual->dEdXDch();
// return -999 if dE/dx information is null or flawed 
  if (measured < 0.1 || measured > 14500) return -999.;

  double expected = PidDEDXCdbDch::dEdxExpected(c->p()+pQual->deltaDchMomentum(), d_MASS, c->p3().theta(),
						c->p3().phi(), static_cast<int>(c->charge()));

// Return relative deviation from the expected value. Note that the denominator
// now contains the measured value, not the expected one - see BAD 1500.
  return 1.0-expected/measured; 
}

// avtelnov, 2007/12/13: completely rewrote AntiDeuteronFilter::consistencysvt 
// to use the new detailed SVT dE/dx parameterization.
// Logic originally intended by Valerie Halyo is preserved.
double
AntiDeuteronFilter::consistencysvt(const BtaCandidate *c) const { 
                                 
  static const float d_MASS = 1.875613;

// return -1500. if something is broken
  if (c==0) return -1500.;
  const BtaPidQual* pQual = c->getMicroAdapter()->getPidQual();
  if (pQual==0) return -1500.;
  
  double measured = pQual->dEdXSvt();
// return -999 if dE/dx information is null
  if (measured < 0.01) return -999.;

  double expected = PidDEDXCdbSvt::dEdxExpected(c->p(), d_MASS, c->p3().theta(),
						c->p3().phi(), static_cast<int>(c->charge()));

// Return relative deviation from the expected value. Note that the denominator
// now contains the measured value, not the expected one - see BAD 1500.
  return 1.0-expected/measured; 
}


bool 
AntiDeuteronFilter::mapCoord(HepMatrix &transMat, int *coordMap)
{
  int i,j=0;
  double sep= sqrt(0.5);
  for (i=0;i<3;i++) {
    for (j=0;j<3;j++) 
      if ( fabs(transMat[i][j]) > sep )
	coordMap[i]= j;
  }
  
  if ( coordMap[0]== coordMap[1] || 
       coordMap[1]== coordMap[2] ||
       coordMap[2]== coordMap[0] ) 
    return false;
  else
    return true;
}

void
AntiDeuteronFilter::CollisionAxisSlope(const BbrPointErr & beamSpot ) 
{ 
  //feed the constructor the covarince matrix
  HepSymMatrix spotErrMat(beamSpot.covMatrix());
  _transMat = diagonalize(&spotErrMat);
  // This calculates the permuation of the diagonalized matrix:
  _ok=mapCoord(_transMat, _coordMap);
  if (!_ok)
    ErrMsg(error) << "Beam spot calculation has failed" << endmsg;
}	 

double AntiDeuteronFilter::XZTilt() const {
  
  if (_ok)
    return _transMat[0][_coordMap[2]]/_transMat[2][_coordMap[2]];
  else   return 0;
}

double AntiDeuteronFilter::YZTilt() const {
  if (_ok)  
    return _transMat[1][_coordMap[2]]/_transMat[2][_coordMap[2]];
  else
    return 0;
}

double AntiDeuteronFilter::XYTilt() const {
  if (_ok)   
    return _transMat[1][_coordMap[0]]/_transMat[0][_coordMap[0]];
  else
    return 0;
}

void
AntiDeuteronFilter::ClosestApproach( TrkLineTraj BeamTrajectory,
				     BtaCandidate* thecand){

  const TrkFit *testCand3(0);  
  if (thecand->recoTrk())  testCand3 = thecand->recoTrk()->fitResult();

  if (testCand3){

    TrkPoca MyPocaCalc(testCand3->traj(), 0.,  BeamTrajectory, 0.);
    HepPoint pntL = testCand3->traj().position(MyPocaCalc.flt1());
    HepPoint pntR = BeamTrajectory.position(MyPocaCalc.flt2());
    
    
    double    Pocax = testCand3->position(MyPocaCalc.flt1()).x();
    double    Pocay = testCand3->position(MyPocaCalc.flt1()).y();
    double    Pocaz = testCand3->position(MyPocaCalc.flt1()).z();

    _poca = HepPoint(Pocax,Pocay,Pocaz);
    
    _Docaxy = sqrt(pow((pntL.x()-pntR.x()),2)+
		   pow((pntL.y()-pntR.y()),2));

    _Docaz = sqrt(pow((pntL.z()-pntR.z()),2));

  }else{
    _Docaxy = -99.;
    _Docaz  = -99.;
    _poca   = HepPoint(-99.,-99.,-99.);
  }

}


double AntiDeuteronFilter::docaxy() const {
  return _Docaxy;
}

double AntiDeuteronFilter::docaz() const {
  return _Docaz;
}

HepPoint AntiDeuteronFilter::poca() const {
  return _poca;
}
