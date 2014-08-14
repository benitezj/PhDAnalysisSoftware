#
# $Id: PrescalePath.tcl,v 1.3 2002/08/26 21:33:11 bartoldu Exp $
#
# defines Prescale filter path
# 08/28/00, Jinlong Zhang
# 

path create PrescalePath

# create tagbit filter
module clone TagFilterByName PrescaleTagFilter
path append PrescalePath PrescaleTagFilter
#
# accept tags that we are interested in
#
talkto PrescaleTagFilter {
    orList set DigiFL1Open
    orList set DigiFL3Open
    orList set DigiFDchEmcPreVeto
}
