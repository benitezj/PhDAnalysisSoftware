#!/bin/csh

##
## prepare BtuRootFile code needed for this package
##
unalias cp 
unalias mv

if ( -e "BtuRootFile.C" && -e "BtuRootFile.h" ) then
    mv -f BtuRootFile.C BtuRootFile.cc
    mv -f BtuRootFile.h BtuRootFile.rdl

    # fix the includes of BtuRootFile.hh in the .cc file
    cat BtuRootFile.cc | sed -e 's/#include "BtuRootFile\.h"/#include "BtuMCPrinter\/BtuRootFile.hh"/' >! tmp.cc
    mv -f tmp.cc BtuRootFile.cc

    # fix the array boundaries in the .rdl file. They are too small
    cat BtuRootFile.rdl | sed -e 's/\[[0-9]*\]/[1000]/' >! tmp.rdl
    mv -f tmp.rdl BtuRootFile.rdl

    # remove the delete of the current tree in the destructor. This always causes a seg fault
    cat BtuRootFile.rdl | grep -v "delete fChain->GetCurrentFile();" >! tmp.rdl
    mv -f tmp.rdl BtuRootFile.rdl

    echo "The original BtuRootFile.C and BtuRootFile.h files have been converted to "
    echo "BtuRootFile.cc and BtuRootFile.rdl and made usable for this package."
else
    echo "You need to make the BtuRootFile.C and BtuRootFile.h files using TTree::MakeClass on your TTree."
endif

