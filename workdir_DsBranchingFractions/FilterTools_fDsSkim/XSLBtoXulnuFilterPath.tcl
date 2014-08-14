#
# $Id: XSLBtoXulnuFilterPath.tcl,v 1.7 2007/02/23 16:51:37 marchior Exp $
#
#
#  Define the XSLBtoXulnuFilter path
#
#

path create XSLBtoXulnuFilterPath
path append XSLBtoXulnuFilterPath XSLBtoXulnuFilter

talkto XSLBtoXulnuFilter {
    production set true
    verbose set false
}
 
# Add lists of composites to write
global BtaCandLists
lappend BtaCandLists XSLBtoXulnuSkimmedYlist

# Write UsrData
global writeUsrDataForSkim
set writeUsrDataForSkim 1
