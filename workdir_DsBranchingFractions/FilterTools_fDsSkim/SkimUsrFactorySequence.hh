//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: SkimUsrFactorySequence.hh,v 1.1 2005/12/03 00:40:51 fwinkl Exp $
//
// Description:
//	Sequence SkimUsrFactorySequence
//
//      This sequence registers user selection and user calculators.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Frank Winklmeier
//
// Copyright Information:
//	Copyright (C) 2005	  Colorado State University
//
//--------------------------------------------------------------------------

#ifndef SKIMUSRFACTORYSEQUENCE_HH
#define SKIMUSRFACTORYSEQUENCE_HH

class AppUserBuild;
class AppSequence;

AppSequence* SkimUsrFactorySequence(AppUserBuild* forWho);

#endif




