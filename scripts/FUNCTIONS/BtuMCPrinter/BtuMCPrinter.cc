#include "BtuMCPrinter/BtuMCPrinter.hh"
#include "BtuMCPrinter/BtuPdt.hh"

#include <TString.h>
#include <iostream>

using namespace std;


BtuMCPrinter::BtuMCPrinter(TTree* tree)
{
  Init(tree);
}

BtuMCPrinter::~BtuMCPrinter()
{
}

void
BtuMCPrinter::printEvent(Int_t event)
{

  if ( fChain->LoadTree(event) < 0) {
    return;
  }
  if ( fChain->GetEvent(event) < 0) {
    return;
  }


  printTree(event, 0, 0);
 
}

void
BtuMCPrinter::printTree( ) {
  printTree(0,0);
}

void 
BtuMCPrinter::printTree(Int_t entry, Int_t theTrk, Int_t pos) {
  if (fChain == 0) return;
  fChain->GetEntry(entry);

  printTree(theTrk,pos);
  
}

void 
BtuMCPrinter::printTree(Int_t theTrk, Int_t pos) {
  if( -1 == theTrk )
    return;
  
  // Transverse momentum: 
  TVector3 momentum = vectorize(mcp3[theTrk], acos(mccosth[theTrk]), mcphi[theTrk]);
  // TVector3 cmmomentum = vectorize(pmc[theTrk], thetamc[theTrk], phimc[theTrk]);
  double pt = sqrt(momentum.X()*momentum.X()+momentum.Y()*momentum.Y());
  
  // Make a PDT object
  BtuPdt pdt;
  string pdtname = pdt.getName(mcLund[theTrk]);

  printf("%s: P=(%.3f,%.3f,%.3f) Pt=%.3f  Index=%d \n", // P*=%.3f
	 pdtname.c_str(),
	 momentum.X(),
	 momentum.Y(),
	 momentum.Z(),
	 pt,
// 	 cmmomentum.Mag(),
	 theTrk);


  for (Int_t j = 0; j < mcLen; j++) {
    if (mothIdx[j] == theTrk) {
      for(Int_t i=0; i<pos+1;i++) cout <<" ";
      cout <<"-->";
      printTree(j,pos+4);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// Return a TVector3 from a rho, theta, and phi
//////////////////////////////////////////////////////////////////////////////

TVector3
BtuMCPrinter::vectorize(Float_t rho, Float_t theta, Float_t phi) {
  TVector3 null(0.0);
  TVector3 vout(1.0);
  TVector3 v(1.0);

  if (rho > 0.0) {
    v.SetMag(rho);
    v.SetTheta(theta);
    v.SetPhi(phi);
    vout = v;
  } else {
    vout = null;
  }
  
  return vout;
}


