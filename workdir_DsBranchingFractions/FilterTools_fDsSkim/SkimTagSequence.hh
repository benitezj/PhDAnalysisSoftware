//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: SkimTagSequence.hh,v 1.3 2004/08/16 01:57:09 bartoldu Exp $
//
// Description:
//	function SkimTagSequence
//
//      This function will add the "standard" set of modules 
//      to a user executable. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Yury Kolomensky
//
// Copyright Information:
//	Copyright (C) 2000		Caltech
//
//------------------------------------------------------------------------

#ifndef SKIMTAGSEQUENCE_HH
#define SKIMTAGSEQUENCE_HH

class AppUserBuild;
class AppSequence;

AppSequence* SkimTagSequence(AppUserBuild* forWho);

#endif




