//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuCandBlock.hh,v 1.9 2006/07/17 22:05:08 chcheng Exp $
//
// Description:
//      Class for filling candidate blocks info.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Chih-hsiang Cheng              Original Author
//
// Copyright Information:
//      Copyright (C) 2004      Lawrence Livermore National Laboratory
//
//-----------------------------------------------------------------------
#ifndef BTUCANDBLOCK_HH
#define BTUCANDBLOCK_HH

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmVector.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "CLHEP/Alist/AList.h"
#include "UsrData/UsrData.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTuple;
template <class T> class HTValOrderedVector;
class BtaCandidate;
class AbsEvent;
class EventInfo;
//              ---------------------
//              -- Class Interface --
//              ---------------------

class BtuCandBlock {

public:
  
  BtuCandBlock(std::string block);
  
  virtual ~BtuCandBlock();

  void init(AbsEvent *anEvent, std::string &keys, 
	    std::vector<std::string> &auxTag, 
	    std::vector<std::string> &auxkeys);

  void fillHTVectors(AbsEvent *anEvent, EventInfo *info,
		     BtaCandidate *c, std::vector<BtaCandidate*>& auxlist,
		     HepAList<BtaCandidate> *mclist=0, BtaMcAssoc *truthMap=0);
  void fillTupleColumns(HepTuple *t, std::string& nCname);

protected:

  typedef uint64_t  bitmap_t;

  void createHTVectors(AbsEvent *anEvent);
  void createHTVectors(AbsEvent *anEvent, bitmap_t bitmap,
		       std::map<std::string, std::vector<std::string> >& kargs,
		       std::string tag="");
  void createHTVectors(AbsEvent *anEvent, std::vector<bitmap_t>& bitmap,
		       std::vector<std::map<std::string, std::vector<std::string> > >& kargs,
		       std::vector<std::string> tags);

  void createUsrHTVectors(AbsEvent *anEvent, std::vector<std::string>& usrblks,
			  std::string tag="");
  void createPidHTVectors(AbsEvent *anEvent, std::vector<std::string>& selectors,
			  std::string tag="");

  void fillHTVectors(AbsEvent *anEvent, EventInfo *info,
		     BtaCandidate *c, bitmap_t bitmap, 
		     std::map<std::string, std::vector<std::string> > &kargs,
		     HepAList<BtaCandidate> *mclist=0, BtaMcAssoc *truthMap=0,
		     std::string tag="");
  void fillUsrHTVectors(AbsEvent *anEvent, BtaCandidate *c, 
			std::vector<std::string>& usrblks, std::string tag="");

  void fillPidHTVectors(AbsEvent *anEvent, BtaCandidate *c, 
			std::vector<std::string>& selectors, std::string tag="");

  void addBoolVar(std::string name);
  void addIntVar(std::string name);
  void addFloatVar(std::string name);

private:

  std::string _block;
  bool _initialized;
  bitmap_t _contMap;
  std::vector<bitmap_t> _auxMap;
  std::vector<std::string> _auxTag;
//   std::vector<std::string> _usrCandBlock, _auxUsrCandBlock;
//   std::vector<std::string> _pidWeights;

  std::map<std::string, HTValOrderedVector<bool>* >  _boolVars;
  std::map<std::string, HTValOrderedVector<int>* >   _intVars;
  std::map<std::string, HTValOrderedVector<float>* > _floatVars;

  std::map<std::string, std::vector<std::string> > _kwordargs;
  std::vector<std::map<std::string, std::vector<std::string> > > _auxkwordargs;

  // Allow only 64 keywords
  std::map<std::string, bitmap_t> _keywordBit;

  template <class T>
  void deleteHTVectors(std::map<std::string, HTValOrderedVector<T>* > &map);

  void setKeywordBitmap(std::string keys,bitmap_t &cm, 
			std::map<std::string, std::vector<std::string> > &kargs);

  void setKeywordBitmap(std::vector<std::string> keys, 
			std::vector<bitmap_t> &cm, 
			std::vector<std::map<std::string, std::vector<std::string> > > &kargs);

  void errorGetVar(std::string vname, std::string block);

  void calcV0Flight(const BtaCandidate *cand, const BbrPointErr& orig, 
		    double &flen, double &flerr, double &cosangle,
		    bool fitIfFail=false);

  // Do not exceed 64 items
  enum keywordbit{ MCIdx,Mass,Momentum,CMMomentum,VtxChi2,Vertex,VtxCov,Doca,DocaXY,Poca,PocaXY,PocaErr,PocaCov,UsrData,PIDWeight,nDaughters,TagVtx,Flight,FlightBS,Dalitz,DalitzMC,ThreeBodyAngles};

};
#endif

