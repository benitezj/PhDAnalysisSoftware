#!/usr/bin/perl
# -w


$outdir=shift;
$collname=shift;
$first=shift;
$last=shift;
$nperfile=shift;
$nevtperfile=shift;#must not be less than actual!


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
    

