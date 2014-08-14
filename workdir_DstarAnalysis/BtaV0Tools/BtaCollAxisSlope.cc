//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCollisionAxisSlope.cc
//
// Description:
//      Class BtaCollAxisSlope - class to compute the tilt angles dxdz
//      and dydz of the collision axis. The computation is performed only
//      when the the beam spot covariance changes.  
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bill Lockman - adapted from Bill Dunowwdie's fortran routine,
//                     which is documented in 
// http://www.slac.stanford.edu/~wmd/bbkinematics/Collision_Axis_Tilt.note
//   
// Copyright Information:        
//      Copyright (C) 2004              SLAC, UC Santa Cruz
//    
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BtaV0Tools/BtaCollAxisSlope.hh"

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "ErrLogger/ErrLog.hh"
#include "CLHEP/Matrix/SymMatrix.h"
using std::cout;
using std::endl;

//----------------
// Constructors --
//----------------

double BtaCollAxisSlope::dxdz=0;
double BtaCollAxisSlope::dydz=0;
double BtaCollAxisSlope::dxdy=0;
double BtaCollAxisSlope::tandelta=0;
HepMatrix BtaCollAxisSlope::U(3,3,1);
HepSymMatrix BtaCollAxisSlope::_cachedSpotErrMat(3,0);
bool BtaCollAxisSlope::debug=false;

HepMatrix
BtaCollAxisSlope::compute(const BbrPointErr& beamSpot)
{
   //feed the constructor the covarince matrix
   HepSymMatrix spotErrMat(beamSpot.covMatrix());

   // recompute only if spot error matrix has changed
   if (spotErrMat(1,1) != _cachedSpotErrMat(1,1) ||
       spotErrMat(1,2) != _cachedSpotErrMat(1,2) ||
       spotErrMat(1,3) != _cachedSpotErrMat(1,3) ||
       spotErrMat(2,2) != _cachedSpotErrMat(2,2) ||
       spotErrMat(2,3) != _cachedSpotErrMat(2,3) ||
       spotErrMat(3,3) != _cachedSpotErrMat(3,3))
     {
       _cachedSpotErrMat = spotErrMat;
       // get the upper triangular part
       double c1 = spotErrMat(1,1);
       double c2 = spotErrMat(1,2);
       double c3 = spotErrMat(1,3);
       double c4 = spotErrMat(2,2);
       double c5 = spotErrMat(2,3);
       double c6 = spotErrMat(3,3);
       
       if (debug) 
	 cout << "ci " << c1 << " " << c2 << " " << c3 << " " << c4 << " " 
	      << c5 << " " << c6 << endl;
       double t1 =  c1*c4*c6;
       double t2 = -c1*c5*c5;
       double t3 =  c2*c3*c5;
       double t4 = -c2*c2*c6;
       double t5 =  c2*c3*c5;
       double t6 = -c3*c3*c4;
       
       if (debug) 
	 cout << "ti " << t1 << " " << t2 << " " << t3 << " " << t4 << " " 
	      << t5 << " " << t6 << endl;
       
       double detC = t1+t2+t3+t4+t5+t6;
       double trace = c1+c4+c6;
       double quadc = (c1*c4 - c2*c2) + (c1*c6-c3*c3) + (c4*c6-c5*c5);
       
       double tphi = c5/c3;
       double top = -c3*c3*sqrt(1+tphi*tphi);
       double bot =  c3*c6 - c2*c5 - c1*c3;
       double ttheta = top/bot;
       double ttheta3;
       ttheta3 = ttheta*(1-ttheta*ttheta);
       ttheta3 = ttheta*(1 - ttheta3*ttheta3);
       ttheta3 = ttheta*(1 - ttheta3*ttheta3);
       
       dxdz = (c3*c3/bot);
       double tfac = 1-ttheta*ttheta;
       double dxdz3 = dxdz*tfac;
       dydz = (c3*c5/bot);
       double dydz3 = dydz*tfac;
       
       if (debug) {
	 cout << "dxdz, dydz, dxdz3, dydz3 " << dxdz << " " << dydz << " " 
	      << dxdz3 << " " << dydz3 << endl;
	 cout << "tan(phi): " << tphi << endl;
       }
       
       top = c1*c3*c3 - c4*c5*c5 - c6*(c3*c3-c5*c5)*ttheta3*ttheta3;
       bot = (c3*c3-c5*c5)*(1-ttheta3*ttheta3);
       double sigxsq = top/bot;
       double sigx = sqrt(sigxsq);
       
       top = c6*(c3*c3 - c5*c5) - (c1*c3*c3 - c4*c5*c5)*ttheta3*ttheta3;
       double sigzsq = top/bot;
       double sigz = sqrt(sigzsq);
       
       double sigysq = (c4*c3*c3 - c1*c5*c5)/(c3*c3 - c5*c5);
       double sigy = sqrt(sigysq);
       if (debug) 
	 cout << "sigx " << sigx << " sigy " << sigy << " sigz " << sigz 
	      << endl;
       
       double traceold;
       if (debug) {
	 traceold = sigxsq + sigysq + sigzsq;
	 cout << "trace check " << traceold << " " << trace << endl;
       }

       double tr1old;
       if (debug) {
	 tr1old = sigxsq*sigysq + sigxsq*sigzsq + sigysq*sigzsq;
	 cout << "Quadratic check " << tr1old << " " << quadc << endl;
       }

       double detold;
       if (debug) {
	 detold = sigxsq * sigysq * sigzsq;
	 cout << "Determinant check: " << detold << " " << detC << endl;
       }
       
       if (debug) { 
	 double c2constrained = c3*c5*(c4-c1)/(c5*c5 - c3*c3);
	 cout <<"c2constrained check " << c2 << " " << c2constrained << endl;
       }

       // now allow phi0 to be nonzero, iterate to improve
       double tp = tphi;
       double tp0;
       double tt = ttheta3;
       double s1old = sigx;
       double s2old = sigy;
       double s3old = sigz;
       
       int niter = 1;
       for (int loop = 0; loop < niter; loop++) {
	 // calculate tan(phi0) = tp0
	 double tp2 = tp*tp;
	 double tt2 = tt*tt;
	 double s1 = s1old*s1old;
	 double s2 = s2old*s2old;
	 double s3 = s3old*s3old;
	 
	 double top = sqrt(1+tt2)*(1+tt2)*((c1-c4)*tp - c2*(1-tp2));
	 double bot = c6*tt2*(1+tp2)+c4*(1-tp2-tt2) - c1*(1-tp2*(1-tt2))
	   -2*c2*tp*(2-tt2);
	 tp0 = top/bot;
	 double q = tp0*tp0;
	 
	 // correct tan phi for tan(phi0)
	 double r3 = 1+tt2;
	 double r5 = 1+tp2;
	 double r1 = sqrt(r5);
	 double r2 = sqrt(r3);
	 double r4 = 1+tp0*tp0;
	 //     top = c5*r3*r4*r1 - tt*(s1*(tp0*r2-tp)   + s2*(tp0*r2-tp*q));
	 //     bot = c3*r3*r4*r1 - tt*(s1*(tp*tp0*r2-1) - s2*(tp*tp0*r2+q));
	 //     double tpnew = sqrt(top/bot);
	 double tpnew = tp;
	 
	 // correct tan(theta) for tan(phi0)
	 double fac1 = 1-tp*tp0*r2;
	 double fac2 = tp0+tp*r2;
	 //     top = c1*r3*r4*r5 - s1*fac1*fac1 - s2*fac*fac;
	 //     bot = c6*r3*r4 - tt2*(s1+s2*q);
	 //     double ttnew = sqrt(top/bot);
	 double ttnew = tt;
	 
	 // now calculate the new sigmas using tan(phi0) and the new values of 
	 // tan(phi) and tan(theta)
	 
	 double r6 = tpnew*tpnew;
	 double r7 = ttnew*ttnew;
	 double r8 = 1./sqrt(1+r7);
	 double r9 = 2*tp0*tpnew*r8;
	 
	 bot = (1-q)*(1-r6)*(1-r7) + r9*(r7-2);
	 top = c1*(1-r9+q*r6*(1-r7)) - c4*(r6+r9+q*(1-r7)) - c6*r7*(1-r6-r9);
	 double s1new = sqrt(top/bot);
	 
	 top = c1*(-r6*(1-r7)-r9-q)  + c4*(1-r7-r9 + q*r6) + c6*r7*(q*(1-r6)+r9);
	 double s2new = sqrt(top/bot);
	 
	 top = c1*r7*(-(1-q)*r9)+c4*r7*(r6*(1-q)+r9)+c6*((1-r6)*(1-q)-r9*(2+r7));
	 double s3new = sqrt(top/bot);
	 t1 = s1new*s1new;
	 t2 = s2new*s2new;
	 t3 = s3new*s3new;
	 double detnew = t1*t2*t3;
	 double tr1new = t1*t2 + t1*t3 + t2*t3;
	 double tracenew = t1+t2+t3;
	 
	 if (debug) {
	   cout << " tan(phi0) " << tp0 << endl;
	   cout << " tan(phi): old, new " << tp << " " << tpnew << endl;
	   cout << " tan(theta): old, new " << tt << " " << ttnew << endl;
	   cout << " sigmax: old, new " << s1old << " " << s1new << endl;
	   cout << " sigmay: old, new " << s2old << " " << s2new << endl;
	   cout << " sigmaz: old, new " << s3old << " " << s3new << endl;
	   cout << " trace:  old, new " << traceold << " " << tracenew 
		<< " trace " << trace << endl;
	   cout << " Quadratic: old, new " << tr1old << " " << tr1new 
		<< " quadc " << quadc << endl;
	   cout << " Determinant: old, new " << detold << " " << detnew 
		<< " detC " << detC << endl;
	 }
	 
	 // update
	 tp = tpnew;
	 tt = ttnew;
	 s1old = s1new;
	 s2old = s2new;
	 s3old = s3new;
       }
       
       bot = sqrt(1+tp0*tp0);
       double sp0 = tp0/bot;
       double cp0 = 1/bot;
       
       bot = sqrt(1+tt*tt);
       double sp = -tp/bot;
       double cp = -1./bot;
       
       bot = sqrt(1+tt*tt);
       double st = tt/bot;
       double ct = 1./bot;
       
       U(1,1) =  sp0*sp - cp0*cp*ct;
       U(1,2) = -sp0*cp - cp0*sp*ct;
       U(1,3) =  cp0*st;
       
       U(2,1) =  cp0*sp + sp0*cp*ct;
       U(2,2) = -cp0*cp + sp0*sp*ct;
       U(2,3) = -sp0*st;
       
       U(3,1) = cp*st;
       U(3,2) = sp*st;
       U(3,3) = ct;
       
       dxdz = U(3,1)/U(3,3);
       dydz = U(3,2)/U(3,3);
       dxdy = U(1,2)/U(1,1);

       tandelta = U(1,2)/sqrt(U(1,1)*U(1,1) + U(1,3)*U(1,3));
     }

   if (debug) {
     cout << "dxdz " << dxdz << " dydz " << dydz << " tandelta " << tandelta 
	  << " dxdy " << dxdy << endl;
     cout << "transform matrix: " << U << endl;
     cout << "Unitarity test:  " << U*U.T() << endl;
     cout << "Similarity test: " << spotErrMat.similarity(U) << endl;
   }
   return U;
}


