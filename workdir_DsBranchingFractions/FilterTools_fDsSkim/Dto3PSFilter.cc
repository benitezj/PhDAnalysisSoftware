//
// Dto3PSFilter
//
// Filter module for "D3PS" (D to 3 PsuedoScater) tag word.
// See also: EventTagTools/TagDto3PseudoScalars.*
//
// $Id: Dto3PSFilter.cc,v 1.1 2003/09/10 01:56:40 davidw Exp $
//
// Copyright (c) 2003  University of California, Santa Cruz
//
#include "BaBar/BaBar.hh"

#include "FilterTools/Dto3PSFilter.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"

#include "AbsEventTag/AbsEventTag.hh"
 
#include "ErrLogger/ErrLog.hh"


//
// Constructor
//
Dto3PSFilter::Dto3PSFilter( const char * const name,
                            const char * const desc )
	: TagFilterModule( name, desc ),
          tagVariable( "tagVariable", this, "D3PSKNotPion" ),
          isCharged( "isCharged", this, true ),
          numberNeutral( "numberNeutral", this, 0 ),
          minPiPlus( "minPiPlus", this, 0 ),
          minPiZero( "minPiZero", this, 0 ),
          minKPlus( "minKPlus", this, 0 ),
          minKShort( "minKShort", this, 0 ),
          minEta( "minEta", this, 0 )
{
	commands()->append( &tagVariable );
        commands()->append( &isCharged );
        commands()->append( &numberNeutral );
        commands()->append( &minPiPlus );
        commands()->append( &minPiZero );
        commands()->append( &minKPlus );
        commands()->append( &minKShort );
        commands()->append( &minEta );
}


//
// beginJob
//
AppResult Dto3PSFilter::beginJob( AbsEvent* anEvent )
{
        nEvents = nPassed = nError = 0;
        return AppResult::OK;
}


//
// endJob
//
AppResult Dto3PSFilter::endJob( AbsEvent* anEvent )
{
        ErrMsg(routine) << name() << ": endJob summary: \n" 
                        << "\t Events processed: " << nEvents << "\n"
                        << "\t Events passed   : " << nPassed << "\n"
                        << "\t Events error    : " << nError  << endmsg;

        return AppResult::OK;
}


//
// event
//
AppResult Dto3PSFilter::event( AbsEvent *event )
{
	//
        // Fetch tag accessor
        //
        TagFilterModule::event( event );
        if (tag() == 0) return Error();

	//
        // Get integer
        //
        int tagint;
        
        if (tag()->getInt( tagint, tagVariable.value() )==0) return Error();
        
        //
        // Check charge
        //
        if ((tagint&(isCharged.value() ? 2 : 1)) == 0) return Failed();
        
        //
        // Number neutrals
        //
        int mask = (1<<(5-numberNeutral.value()));
        if ((tagint&mask) == 0) return Failed();
        
        //
        // Max number particles
        //
        const int targs[5] = { minPiPlus.value(),
                               minPiZero.value(),
                               minKPlus.value(),
                               minKShort.value(),
                               minEta.value() };

	int np = tagint >> 6;
	for( int i=0; i<5; ++i ) {
                if ((np&0x3) < targs[i]) return Failed();
                
                np >>= 2;
        }
        
        //
        // Success
        //
        return Passed();
}


//
// Helper methods (protected)
//
AppResult Dto3PSFilter::Failed()
{
        nEvents++;

        setPassed(false);
        return AppResult::OK;
}

AppResult Dto3PSFilter::Passed()
{
        nEvents++;
        nPassed++;

        setPassed(true);
        return AppResult::OK;
}

AppResult Dto3PSFilter::Error()
{
        nEvents++;
        nError++;

        setPassed(false);
        return AppResult::OK;
}
