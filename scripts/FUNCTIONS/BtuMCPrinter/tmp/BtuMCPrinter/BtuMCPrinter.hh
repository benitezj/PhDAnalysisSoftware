#ifndef BtuMCPrinter_hh
#define BtuMCPrinter_hh

#include "BtuMCPrinter/BtuRootFile.hh"

#include <TString.h>
#include <TVector3.h>

class BtuMCPrinter : public BtuRootFile {


public:
  BtuMCPrinter(TTree* tree = NULL);
  ~BtuMCPrinter();

  void printEvent(Int_t event);
  void printTree(); // print tree of current event
  void printTree(Int_t theTrk, Int_t pos); // print tree, 
                                           //  specifying base particle
  void printTree(Int_t event, Int_t theTrk, Int_t pos); // print tree for an event 
                                                        // specifying base particle

private:
  TVector3 vectorize(Float_t rho, Float_t theta, Float_t phi);
};

#endif
