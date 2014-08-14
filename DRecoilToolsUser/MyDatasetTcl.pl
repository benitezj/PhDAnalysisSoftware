#!/usr/bin/perl
# -w

##This script merges collections 

$outdir=shift;#directory where tcl files will be dumped
$collname=shift;#name of input collection with index number removed
$first=shift;#index of first collection
$last=shift;#index of last collection
$nperfile=shift;#number of collections to be merged together
$nevtperfile=shift;#number of events per individual collection, should not be less than actual but could be greater!

########################################
print "$outdir\n";
print "$collname\n";
print "$first\n";
print "$last\n";
print "$nperfile\n";
print "$nevtperfile\n";


system("rm -f $outdir/DataSet-1*.tcl");
system("rm -f $outdir/DataSet-2*.tcl");
system("rm -f $outdir/DataSet-3*.tcl");
system("rm -f $outdir/DataSet-*.tcl");
print "all DataSet*.tcl files removed\n";

$fileid=0;
$collid=$first;
while($collid<=$last){
    $fileid++;   
    open FILE, "> $outdir/DataSet-$fileid.tcl" or die "@_";
    $n=1;
    $NtotalEvt=0;
    while($n<=$nperfile && $collid<=$last){
	$n++;

	print FILE "lappend inputList ${collname}$collid\n";
	
	$collid++;
	$NtotalEvt+=$nevtperfile;
    }

    $n--;
    print FILE "## In this tcl file: $n collections, $NtotalEvt events\n";
    
    close FILE;
   
}
   
print "Created $fileid files\n";
exit;
    

