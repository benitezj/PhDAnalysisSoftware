# $Id: sourceFoundFilterTools.tcl,v 1.1 2001/01/08 20:48:00 yury Exp $
#
# define proc for locating user scripts
#
proc sourceFoundFilterTools file {
    if [ file exists ./$file ] {
        echo Sourcing ./$file
        uplevel 1 source ./$file

    } else {
        sourceFoundFile FilterTools/$file
    }
}
