#!/usr/bin/perl
# -w


$outdir=shift;
$collname=shift;
$first=shift;
$last=shift;
$nevtspercoll=shift;# !! must be integer number times $nevtssplit 
$nevtssplit=shift;# otherwise last file does not get created

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
    

