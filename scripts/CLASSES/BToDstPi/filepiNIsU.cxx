//
// File generated by rootcint at Mon Nov  3 11:24:53 2008

// Do NOT change. Changes will be lost next time file is generated
//

#include "RConfig.h"
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "filepiNIsU.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

/********************************************************
* /afs/slac.stanford.edu/u/eb/benitezj/ana42/workdir/./SOFTWARE/CLASSES/BToDstPi/filepiNIsU.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

extern "C" void G__cpp_reset_tagtablefilepiNIsU();

extern "C" void G__set_cpp_environmentfilepiNIsU() {
  G__add_compiledheader("TObject.h");
  G__add_compiledheader("TMemberInspector.h");
  G__add_compiledheader("/afs/slac.stanford.edu/u/eb/benitezj/ana42/workdir/./SOFTWARE/CLASSES/BToDstPi/BToDstPiAnalysisControl.C");
  G__cpp_reset_tagtablefilepiNIsU();
}
#include <new>
extern "C" int G__cpp_dllrevfilepiNIsU() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* Setting up global function */
static int G__filepiNIsU__0_828(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   switch (libp->paran) {
   case 2:
      G__letint(result7, 85, (long) analyzeBToDstPi((Int_t) G__int(libp->para[0]), (Bool_t) G__int(libp->para[1])));
      break;
   case 1:
      G__letint(result7, 85, (long) analyzeBToDstPi((Int_t) G__int(libp->para[0])));
      break;
   case 0:
      G__letint(result7, 85, (long) analyzeBToDstPi());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}


/*********************************************************
* Member function Stub
*********************************************************/

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncfilepiNIsU {
 public:
  G__Sizep2memfuncfilepiNIsU(): p(&G__Sizep2memfuncfilepiNIsU::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncfilepiNIsU::*p)();
};

size_t G__get_sizep2memfuncfilepiNIsU()
{
  G__Sizep2memfuncfilepiNIsU a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritancefilepiNIsU() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetablefilepiNIsU() {

   /* Setting up typedef entry */
   G__search_typename2("Int_t",105,-1,0,-1);
   G__setnewtype(-1,"Signed integer 4 bytes (int)",0);
   G__search_typename2("Bool_t",103,-1,0,-1);
   G__setnewtype(-1,"Boolean (0=false, 1=true) (bool)",0);
   G__search_typename2("vector<UInt_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__filepiNIsULN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__filepiNIsULN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<unsigned int>",117,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TVectorT<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TVectorTlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TVectorT<Double_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TVectorTlEdoublegR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTBase<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTBaselEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTBase<Double_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTBaselEdoublegR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("set<std::string>",117,G__get_linked_tagnum(&G__filepiNIsULN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("set<string>",117,G__get_linked_tagnum(&G__filepiNIsULN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("set<string,less<string>>",117,G__get_linked_tagnum(&G__filepiNIsULN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<std::string,std::string>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,string>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,string,less<string>>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("set<std::pair<RooAbsArg*,RooAbsArg*> >",117,G__get_linked_tagnum(&G__filepiNIsULN_setlEpairlERooAbsArgmUcORooAbsArgmUgRcOlesslEpairlERooAbsArgmUcORooAbsArgmUgRsPgRcOallocatorlEpairlERooAbsArgmUcORooAbsArgmUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("set<pair<RooAbsArg*,RooAbsArg*>>",117,G__get_linked_tagnum(&G__filepiNIsULN_setlEpairlERooAbsArgmUcORooAbsArgmUgRcOlesslEpairlERooAbsArgmUcORooAbsArgmUgRsPgRcOallocatorlEpairlERooAbsArgmUcORooAbsArgmUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("set<pair<RooAbsArg*,RooAbsArg*>,less<pair<RooAbsArg*,RooAbsArg*> >>",117,G__get_linked_tagnum(&G__filepiNIsULN_setlEpairlERooAbsArgmUcORooAbsArgmUgRcOlesslEpairlERooAbsArgmUcORooAbsArgmUgRsPgRcOallocatorlEpairlERooAbsArgmUcORooAbsArgmUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("deque<RooAbsCache*>",117,G__get_linked_tagnum(&G__filepiNIsULN_dequelERooAbsCachemUcOallocatorlERooAbsCachemUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<int,std::string>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEintcOstringcOlesslEintgRcOallocatorlEpairlEconstsPintcOstringgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<int,string>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEintcOstringcOlesslEintgRcOallocatorlEpairlEconstsPintcOstringgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<int,string,less<int>>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEintcOstringcOlesslEintgRcOallocatorlEpairlEconstsPintcOstringgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<StreamConfig>",117,G__get_linked_tagnum(&G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__filepiNIsULN_reverse_iteratorlEvectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__filepiNIsULN_reverse_iteratorlEvectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<RooMsgService::StreamConfig>",117,G__get_linked_tagnum(&G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<std::string,std::ostream*>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,basic_ostream<char,char_traits<char> >*>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,basic_ostream<char,char_traits<char> >*,less<string>>",117,G__get_linked_tagnum(&G__filepiNIsULN_maplEstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<Double_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__filepiNIsULN_reverse_iteratorlEvectorlEdoublecOallocatorlEdoublegRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__filepiNIsULN_reverse_iteratorlEvectorlEdoublecOallocatorlEdoublegRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<double>",117,G__get_linked_tagnum(&G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("RooCacheManager<std::vector<Double_t> >",117,G__get_linked_tagnum(&G__filepiNIsULN_RooCacheManagerlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("list<RooArgSet*>",117,G__get_linked_tagnum(&G__filepiNIsULN_listlERooArgSetmUcOallocatorlERooArgSetmUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("list<RooAbsLValue*>",117,G__get_linked_tagnum(&G__filepiNIsULN_listlERooAbsLValuemUcOallocatorlERooAbsLValuemUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixT<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTRow_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTRow_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTColumn_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTColumn_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTDiag_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTDiag_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTFlat_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTFlat_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTSub_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTSub_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTSparseRow_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTSparseRow_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTSparseDiag_const<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTSparseDiag_constlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTRow<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTRowlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTColumn<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTColumnlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTDiag<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTDiaglEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTFlat<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTFlatlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTSub<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTSublEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTSparseRow<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTSparseRowlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TMatrixTSparseDiag<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TMatrixTSparseDiaglEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TElementActionT<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TElementActionTlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("TElementPosActionT<Float_t>",117,G__get_linked_tagnum(&G__filepiNIsULN_TElementPosActionTlEfloatgR),0,-1);
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */
extern "C" void G__cpp_setup_memvarfilepiNIsU() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/

/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncfilepiNIsU() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {
}

static void G__cpp_setup_global2() {
}

static void G__cpp_setup_global3() {
}

static void G__cpp_setup_global4() {
}

static void G__cpp_setup_global5() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalfilepiNIsU() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
  G__cpp_setup_global2();
  G__cpp_setup_global3();
  G__cpp_setup_global4();
  G__cpp_setup_global5();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
   G__memfunc_setup("analyzeBToDstPi", 1501, G__filepiNIsU__0_828, 85, G__get_linked_tagnum(&G__filepiNIsULN_BToDstPiAnalysis), -1, 0, 2, 1, 1, 0, 
"i - 'Int_t' 0 '-1' WhichDATASet g - 'Bool_t' 0 '0' ReduceNtuple", (char*) NULL
, (void*) NULL, 0);

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcfilepiNIsU() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__filepiNIsULN_TVectorTlEfloatgR = { "TVectorT<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTBaselEfloatgR = { "TMatrixTBase<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTlEfloatgR = { "TMatrixT<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTRow_constlEfloatgR = { "TMatrixTRow_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTColumn_constlEfloatgR = { "TMatrixTColumn_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTDiag_constlEfloatgR = { "TMatrixTDiag_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTFlat_constlEfloatgR = { "TMatrixTFlat_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTSub_constlEfloatgR = { "TMatrixTSub_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTRowlEfloatgR = { "TMatrixTRow<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTColumnlEfloatgR = { "TMatrixTColumn<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTDiaglEfloatgR = { "TMatrixTDiag<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTFlatlEfloatgR = { "TMatrixTFlat<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTSublEfloatgR = { "TMatrixTSub<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTSparseRow_constlEfloatgR = { "TMatrixTSparseRow_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTSparseRowlEfloatgR = { "TMatrixTSparseRow<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTSparseDiag_constlEfloatgR = { "TMatrixTSparseDiag_const<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTSparseDiaglEfloatgR = { "TMatrixTSparseDiag<float>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TVectorTlEdoublegR = { "TVectorT<double>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_TMatrixTBaselEdoublegR = { "TMatrixTBase<double>" , 97 , -1 };
G__linked_taginfo G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR = { "vector<unsigned int,allocator<unsigned int> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<unsigned int,allocator<unsigned int> >::iterator>" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR = { "set<string,less<string>,allocator<string> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR = { "map<string,string,less<string>,allocator<pair<const string,string> > >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_setlEpairlERooAbsArgmUcORooAbsArgmUgRcOlesslEpairlERooAbsArgmUcORooAbsArgmUgRsPgRcOallocatorlEpairlERooAbsArgmUcORooAbsArgmUgRsPgRsPgR = { "set<pair<RooAbsArg*,RooAbsArg*>,less<pair<RooAbsArg*,RooAbsArg*> >,allocator<pair<RooAbsArg*,RooAbsArg*> > >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_dequelERooAbsCachemUcOallocatorlERooAbsCachemUgRsPgR = { "deque<RooAbsCache*,allocator<RooAbsCache*> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_maplEintcOstringcOlesslEintgRcOallocatorlEpairlEconstsPintcOstringgRsPgRsPgR = { "map<int,string,less<int>,allocator<pair<const int,string> > >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR = { "vector<RooMsgService::StreamConfig,allocator<RooMsgService::StreamConfig> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_reverse_iteratorlEvectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgRcLcLiteratorgR = { "reverse_iterator<vector<RooMsgService::StreamConfig,allocator<RooMsgService::StreamConfig> >::iterator>" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_maplEstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUgRsPgRsPgR = { "map<string,basic_ostream<char,char_traits<char> >*,less<string>,allocator<pair<const string,basic_ostream<char,char_traits<char> >*> > >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR = { "vector<double,allocator<double> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_reverse_iteratorlEvectorlEdoublecOallocatorlEdoublegRsPgRcLcLiteratorgR = { "reverse_iterator<vector<double,allocator<double> >::iterator>" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_RooCacheManagerlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgR = { "RooCacheManager<vector<double,allocator<double> > >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_listlERooArgSetmUcOallocatorlERooArgSetmUgRsPgR = { "list<RooArgSet*,allocator<RooArgSet*> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_listlERooAbsLValuemUcOallocatorlERooAbsLValuemUgRsPgR = { "list<RooAbsLValue*,allocator<RooAbsLValue*> >" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_TElementActionTlEfloatgR = { "TElementActionT<float>" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_TElementPosActionTlEfloatgR = { "TElementPosActionT<float>" , 99 , -1 };
G__linked_taginfo G__filepiNIsULN_BToDstPiAnalysis = { "BToDstPiAnalysis" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtablefilepiNIsU() {
  G__filepiNIsULN_TVectorTlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTBaselEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTRow_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTColumn_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTDiag_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTFlat_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTSub_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTRowlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTColumnlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTDiaglEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTFlatlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTSublEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTSparseRow_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTSparseRowlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTSparseDiag_constlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTSparseDiaglEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TVectorTlEdoublegR.tagnum = -1 ;
  G__filepiNIsULN_TMatrixTBaselEdoublegR.tagnum = -1 ;
  G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__filepiNIsULN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_setlEpairlERooAbsArgmUcORooAbsArgmUgRcOlesslEpairlERooAbsArgmUcORooAbsArgmUgRsPgRcOallocatorlEpairlERooAbsArgmUcORooAbsArgmUgRsPgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_dequelERooAbsCachemUcOallocatorlERooAbsCachemUgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_maplEintcOstringcOlesslEintgRcOallocatorlEpairlEconstsPintcOstringgRsPgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR.tagnum = -1 ;
  G__filepiNIsULN_reverse_iteratorlEvectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__filepiNIsULN_maplEstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUgRsPgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR.tagnum = -1 ;
  G__filepiNIsULN_reverse_iteratorlEvectorlEdoublecOallocatorlEdoublegRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__filepiNIsULN_RooCacheManagerlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_listlERooArgSetmUcOallocatorlERooArgSetmUgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_listlERooAbsLValuemUcOallocatorlERooAbsLValuemUgRsPgR.tagnum = -1 ;
  G__filepiNIsULN_TElementActionTlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_TElementPosActionTlEfloatgR.tagnum = -1 ;
  G__filepiNIsULN_BToDstPiAnalysis.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtablefilepiNIsU() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TVectorTlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTBaselEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTRow_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTColumn_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTDiag_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTFlat_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTSub_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTRowlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTColumnlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTDiaglEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTFlatlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTSublEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTSparseRow_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTSparseRowlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTSparseDiag_constlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTSparseDiaglEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TVectorTlEdoublegR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TMatrixTBaselEdoublegR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_setlEpairlERooAbsArgmUcORooAbsArgmUgRcOlesslEpairlERooAbsArgmUcORooAbsArgmUgRsPgRcOallocatorlEpairlERooAbsArgmUcORooAbsArgmUgRsPgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_dequelERooAbsCachemUcOallocatorlERooAbsCachemUgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_maplEintcOstringcOlesslEintgRcOallocatorlEpairlEconstsPintcOstringgRsPgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_vectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_reverse_iteratorlEvectorlERooMsgServicecLcLStreamConfigcOallocatorlERooMsgServicecLcLStreamConfiggRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_maplEstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcObasic_ostreamlEcharcOchar_traitslEchargRsPgRmUgRsPgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_vectorlEdoublecOallocatorlEdoublegRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_reverse_iteratorlEvectorlEdoublecOallocatorlEdoublegRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_RooCacheManagerlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_listlERooArgSetmUcOallocatorlERooArgSetmUgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_listlERooAbsLValuemUcOallocatorlERooAbsLValuemUgRsPgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TElementActionTlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_TElementPosActionTlEfloatgR);
   G__get_linked_tagnum_fwd(&G__filepiNIsULN_BToDstPiAnalysis);
}
extern "C" void G__cpp_setupfilepiNIsU(void) {
  G__check_setup_version(30051515,"G__cpp_setupfilepiNIsU()");
  G__set_cpp_environmentfilepiNIsU();
  G__cpp_setup_tagtablefilepiNIsU();

  G__cpp_setup_inheritancefilepiNIsU();

  G__cpp_setup_typetablefilepiNIsU();

  G__cpp_setup_memvarfilepiNIsU();

  G__cpp_setup_memfuncfilepiNIsU();
  G__cpp_setup_globalfilepiNIsU();
  G__cpp_setup_funcfilepiNIsU();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncfilepiNIsU();
  return;
}
class G__cpp_setup_initfilepiNIsU {
  public:
    G__cpp_setup_initfilepiNIsU() { G__add_setup_func("filepiNIsU",(G__incsetup)(&G__cpp_setupfilepiNIsU)); G__call_setup_funcs(); }
   ~G__cpp_setup_initfilepiNIsU() { G__remove_setup_func("filepiNIsU"); }
};
G__cpp_setup_initfilepiNIsU G__cpp_setup_initializerfilepiNIsU;

