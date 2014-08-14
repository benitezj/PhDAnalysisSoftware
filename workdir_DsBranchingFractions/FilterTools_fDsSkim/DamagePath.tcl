# $Id: DamagePath.tcl,v 1.1 2001/01/08 20:48:00 yury Exp $
#
# defines Damage filter path
# 
path create DamagePath

# create tagbit filter
module clone TagFilterByName DamageTagFilter
path append DamagePath DamageTagFilter
#
# accept tags that we are interested in
#
talkto DamageTagFilter {
    andList set DigiFDataDamage
}
