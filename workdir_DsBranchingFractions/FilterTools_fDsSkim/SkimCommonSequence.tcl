# $Id: SkimCommonSequence.tcl,v 1.3 2004/05/03 22:19:49 fnc Exp $
#
# a set of common filters for all paths
#

sequence create SkimCommonSequence
sequence append SkimCommonSequence RecEventControlSequence

module clone TagFilterByName RecoBGSkimFilter
mod talk RecoBGSkimFilter
    andList set RecoBGFilter
exit
sequence append SkimCommonSequence RecoBGSkimFilter
