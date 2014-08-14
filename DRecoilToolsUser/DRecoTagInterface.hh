
#ifndef DRecoTagInterface_HH
#define DRecoTagInterface_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "BetaMiniUser/NamedScalers.hh"
#include "Framework/AbsParmDouble.hh"
#include "VtxBase/VtxParmAlgId.hh"
#include "VtxBase/VtxParmAlgMode.hh"
#include "Framework/AbsParmBool.hh"
#include "CLHEP/Alist/AList.h"

class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoTagInterface : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoTagInterface( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoTagInterface( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmIfdStrKey eventInfoList;

  AbsParmIfdStrKey TagList1;
  AbsParmIfdStrKey TagList2;
  AbsParmIfdStrKey TagList3;
  AbsParmIfdStrKey TagList4;
  AbsParmIfdStrKey TagList5;
  AbsParmIfdStrKey TagList6;
  AbsParmIfdStrKey TagList7;
  AbsParmIfdStrKey TagList8;
  AbsParmIfdStrKey TagList9;
  AbsParmIfdStrKey TagList10;
  AbsParmIfdStrKey TagList11;
  AbsParmIfdStrKey TagList12;
  AbsParmIfdStrKey TagList13;
  AbsParmIfdStrKey TagList14;
  AbsParmIfdStrKey TagList15;
  AbsParmIfdStrKey TagList16;
  AbsParmIfdStrKey TagList17;
  AbsParmIfdStrKey TagList18;
  AbsParmIfdStrKey TagList19;
  AbsParmIfdStrKey TagList20;
  AbsParmIfdStrKey TagList21;
  AbsParmIfdStrKey TagList22;
  AbsParmIfdStrKey TagList23;
  AbsParmIfdStrKey TagList24;
  AbsParmIfdStrKey TagList25;
  AbsParmIfdStrKey TagList26;
  AbsParmIfdStrKey TagList27;
  AbsParmIfdStrKey TagList28;
  AbsParmIfdStrKey TagList29;
  AbsParmIfdStrKey TagList30;

  //Vertexing
  AbsParmBool TagListFit1;
  AbsParmBool TagListFit2;
  AbsParmBool TagListFit3;
  AbsParmBool TagListFit4;
  AbsParmBool TagListFit5;
  AbsParmBool TagListFit6;
  AbsParmBool TagListFit7;
  AbsParmBool TagListFit8;
  AbsParmBool TagListFit9;
  AbsParmBool TagListFit10;
  AbsParmBool TagListFit11;
  AbsParmBool TagListFit12;
  AbsParmBool TagListFit13;
  AbsParmBool TagListFit14;
  AbsParmBool TagListFit15;
  AbsParmBool TagListFit16;
  AbsParmBool TagListFit17;
  AbsParmBool TagListFit18;
  AbsParmBool TagListFit19;
  AbsParmBool TagListFit20;
  AbsParmBool TagListFit21;
  AbsParmBool TagListFit22;
  AbsParmBool TagListFit23;
  AbsParmBool TagListFit24;
  AbsParmBool TagListFit25;
  AbsParmBool TagListFit26;
  AbsParmBool TagListFit27;
  AbsParmBool TagListFit28;
  AbsParmBool TagListFit29;
  AbsParmBool TagListFit30;



  //Mass centers
  AbsParmDouble TagListMass1;
  AbsParmDouble TagListMass2;
  AbsParmDouble TagListMass3;
  AbsParmDouble TagListMass4;
  AbsParmDouble TagListMass5;
  AbsParmDouble TagListMass6;
  AbsParmDouble TagListMass7;
  AbsParmDouble TagListMass8;
  AbsParmDouble TagListMass9;
  AbsParmDouble TagListMass10;
  AbsParmDouble TagListMass11;
  AbsParmDouble TagListMass12;
  AbsParmDouble TagListMass13;
  AbsParmDouble TagListMass14;
  AbsParmDouble TagListMass15;
  AbsParmDouble TagListMass16;
  AbsParmDouble TagListMass17;
  AbsParmDouble TagListMass18;
  AbsParmDouble TagListMass19;
  AbsParmDouble TagListMass20;
  AbsParmDouble TagListMass21;
  AbsParmDouble TagListMass22;
  AbsParmDouble TagListMass23;
  AbsParmDouble TagListMass24;
  AbsParmDouble TagListMass25;
  AbsParmDouble TagListMass26;
  AbsParmDouble TagListMass27;
  AbsParmDouble TagListMass28;
  AbsParmDouble TagListMass29;
  AbsParmDouble TagListMass30;


  //Mass cuts
  AbsParmDouble TagListMassCut1;
  AbsParmDouble TagListMassCut2;
  AbsParmDouble TagListMassCut3;
  AbsParmDouble TagListMassCut4;
  AbsParmDouble TagListMassCut5;
  AbsParmDouble TagListMassCut6;
  AbsParmDouble TagListMassCut7;
  AbsParmDouble TagListMassCut8;
  AbsParmDouble TagListMassCut9;
  AbsParmDouble TagListMassCut10;
  AbsParmDouble TagListMassCut11;
  AbsParmDouble TagListMassCut12;
  AbsParmDouble TagListMassCut13;
  AbsParmDouble TagListMassCut14;
  AbsParmDouble TagListMassCut15;
  AbsParmDouble TagListMassCut16;
  AbsParmDouble TagListMassCut17;
  AbsParmDouble TagListMassCut18;
  AbsParmDouble TagListMassCut19;
  AbsParmDouble TagListMassCut20;
  AbsParmDouble TagListMassCut21;
  AbsParmDouble TagListMassCut22;
  AbsParmDouble TagListMassCut23;
  AbsParmDouble TagListMassCut24;
  AbsParmDouble TagListMassCut25;
  AbsParmDouble TagListMassCut26;
  AbsParmDouble TagListMassCut27;
  AbsParmDouble TagListMassCut28;
  AbsParmDouble TagListMassCut29;
  AbsParmDouble TagListMassCut30;


  //p* cuts
  AbsParmDouble TagListPstarCut1;
  AbsParmDouble TagListPstarCut2;
  AbsParmDouble TagListPstarCut3;
  AbsParmDouble TagListPstarCut4;
  AbsParmDouble TagListPstarCut5;
  AbsParmDouble TagListPstarCut6;
  AbsParmDouble TagListPstarCut7;
  AbsParmDouble TagListPstarCut8;
  AbsParmDouble TagListPstarCut9;
  AbsParmDouble TagListPstarCut10;
  AbsParmDouble TagListPstarCut11;
  AbsParmDouble TagListPstarCut12;
  AbsParmDouble TagListPstarCut13;
  AbsParmDouble TagListPstarCut14;
  AbsParmDouble TagListPstarCut15;
  AbsParmDouble TagListPstarCut16;
  AbsParmDouble TagListPstarCut17;
  AbsParmDouble TagListPstarCut18;
  AbsParmDouble TagListPstarCut19;
  AbsParmDouble TagListPstarCut20;
  AbsParmDouble TagListPstarCut21;
  AbsParmDouble TagListPstarCut22;
  AbsParmDouble TagListPstarCut23;
  AbsParmDouble TagListPstarCut24;
  AbsParmDouble TagListPstarCut25;
  AbsParmDouble TagListPstarCut26;
  AbsParmDouble TagListPstarCut27;
  AbsParmDouble TagListPstarCut28;
  AbsParmDouble TagListPstarCut29;
  AbsParmDouble TagListPstarCut30;

  //probmin cuts
  AbsParmDouble TagListProbminCut1;
  AbsParmDouble TagListProbminCut2;
  AbsParmDouble TagListProbminCut3;
  AbsParmDouble TagListProbminCut4;
  AbsParmDouble TagListProbminCut5;
  AbsParmDouble TagListProbminCut6;
  AbsParmDouble TagListProbminCut7;
  AbsParmDouble TagListProbminCut8;
  AbsParmDouble TagListProbminCut9;
  AbsParmDouble TagListProbminCut10;
  AbsParmDouble TagListProbminCut11;
  AbsParmDouble TagListProbminCut12;
  AbsParmDouble TagListProbminCut13;
  AbsParmDouble TagListProbminCut14;
  AbsParmDouble TagListProbminCut15;
  AbsParmDouble TagListProbminCut16;
  AbsParmDouble TagListProbminCut17;
  AbsParmDouble TagListProbminCut18;
  AbsParmDouble TagListProbminCut19;
  AbsParmDouble TagListProbminCut20;
  AbsParmDouble TagListProbminCut21;
  AbsParmDouble TagListProbminCut22;
  AbsParmDouble TagListProbminCut23;
  AbsParmDouble TagListProbminCut24;
  AbsParmDouble TagListProbminCut25;
  AbsParmDouble TagListProbminCut26;
  AbsParmDouble TagListProbminCut27;
  AbsParmDouble TagListProbminCut28;
  AbsParmDouble TagListProbminCut29;
  AbsParmDouble TagListProbminCut30;

  // Use double for requireLepton so that in future we can choose mu and/or e
  AbsParmIfdStrKey muonList;
  AbsParmIfdStrKey electronList;
  AbsParmDouble requireLepton;


private:
  NamedScalers _scalers; 

  HepAList< AbsParmIfdStrKey > ListOfTagList;
  HepAList< AbsParmBool >   ListOfTagListFit;
  HepAList< AbsParmDouble > ListOfTagListMass;
  HepAList< AbsParmDouble > ListOfTagListMassCut;
  HepAList< AbsParmDouble > ListOfTagListPstarCut;
  HepAList< AbsParmDouble > ListOfTagListProbminCut;


};


#endif
