#!/usr/bin/perl
# -w


$firstfile=shift;
$lastfile=shift;
$modenum=shift;


$outfile="SP-${modenum}.tcl";

open OUTFILE, "> ${outfile}" or die "@_";

foreach $fileid ($firstfile .. $lastfile){

 print OUTFILE "lappend inputList /work/users/benitezj/SP-${modenum}-Collection${fileid}\n";
	       

}
 close OUTFILE;



exit;
#############




