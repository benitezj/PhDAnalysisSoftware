#ifndef BtaCollAxisSlope_HH
#define BtaCollAxisSlope_HH 1

#include "BbrGeom/BbrPointErr.hh"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"

class BtaCollAxisSlope {

public:
  BtaCollAxisSlope(){;}
  BtaCollAxisSlope(const BbrPointErr& bs){
    compute(bs);
  };
  virtual ~BtaCollAxisSlope(){;}
  static void setDebug(const bool& d) { debug = d; }
  static HepMatrix compute(const BbrPointErr& bs);
  static double XZTilt() { return dxdz; }
  static double YZTilt() { return dydz; }
  static double XYTilt() { return dxdy; }
  static double tanDelta() { return tandelta; }

private:
  static double dxdz, dydz, dxdy, tandelta;
  static HepMatrix U;
  static HepSymMatrix _cachedSpotErrMat;
  static bool debug;
};

#endif
