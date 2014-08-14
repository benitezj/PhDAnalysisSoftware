//
// Dto3PSFilter
//
// Filter module for "D3PS" (D to 3 PsuedoScater) tag word.
// See also: EventTagTools/TagDto3PseudoScalars.*
//
// $Id: Dto3PSFilter.hh,v 1.1 2003/09/10 01:56:40 davidw Exp $
//
// Copyright (c) 2003  University of California, Santa Cruz
//
#ifndef Dto3PSFilter_hh
#define Dto3PSFilter_hh

#include "TagModules/TagFilterModule.hh"

#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "Framework/AbsParmBool.hh"

class Dto3PSFilter : public TagFilterModule
{
	public:
        Dto3PSFilter( const char * const name,
                      const char * const descr );

        virtual AppResult beginJob( AbsEvent* anEvent );
        virtual AppResult endJob( AbsEvent* anEvent );
        virtual AppResult event( AbsEvent *event );
        
        
        protected:

        AppResult Failed();
        AppResult Passed();
        AppResult Error();
        
        AbsParmString		tagVariable;
        
        AbsParmBool		isCharged;
        AbsParmGeneral<int>	numberNeutral;
        AbsParmGeneral<int>	minPiPlus;
        AbsParmGeneral<int>	minPiZero;
        AbsParmGeneral<int>	minKPlus;
        AbsParmGeneral<int>	minKShort;
        AbsParmGeneral<int>	minEta;

        unsigned long nEvents, nPassed, nError;
};

#endif
