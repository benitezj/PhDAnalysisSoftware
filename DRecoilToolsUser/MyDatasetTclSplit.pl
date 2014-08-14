#!/usr/bin/perl
# -w

##This script divides collections into smaller ones 

$outdir=shift;#directory where tcl files will be dumped
$collname=shift;#name of input collection with index number removed
$first=shift;#index of first collection
$last=shift;#index of last collection
$nevtspercoll=shift;# number of events per individual collection, !! must be integer number times $nevtssplit  otherwise last file does not get created
$nevtssplit=shift;# number of events desired in each output collection


########################################################################
#round up
$nsplit=int($nevtspercoll/$nevtssplit +.5);

print "$outdir\n";
print "$collname\n";
print "$first\n";
print "$last\n";
print "$nsplit\n";


system("rm -f $outdir/DataSet*.tcl");

$fileid=0;
$collid=$first;
while($collid<=$last){
    $firstevent=1;
    $lastevent=$nevtssplit;

    #deal with only one input collection with no indexing
    $collidmodified=$collid;
    if($first==-1 and $last==-1){$collidmodified="";}
	

    while($lastevent<=$nevtspercoll){
	$fileid++;

	open FILE, "> $outdir/DataSet-$fileid.tcl" or die "@_";	
	print FILE "lappend inputList ${collname}${collidmodified}%selectEventSequence=$firstevent-$lastevent\n";
	print FILE "## In this tcl file: 1 collections, $nevtssplit events\n";	
	close FILE;

	$firstevent=$lastevent+1;
	$lastevent+=$nevtssplit;	


    }

    $collid++;      
}
   
print "Created $fileid files\n";
exit;
    

