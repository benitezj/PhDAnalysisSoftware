#
# $Id: commonSettings.tcl,v 1.3 2003/04/15 14:41:01 fnc Exp $
#
# This file is sourced by streamPathSetup and stremPreCreate
# to set up common settings to be run both by stand-alone Elf and OPR
#
# G. Finocchiaro, 06/27/02
#
global noKanga
set noKanga 1

global streamSetup
set streamSetup 1

global writeSkimsInOPR
set writeSkimsInOPR 1

global skimGroupName
set skimGroupName prskims
