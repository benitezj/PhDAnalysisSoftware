#!/usr/bin/perl
# -w
#
#This script creates files with a list of event collections 
#
$inname=shift;
$outname=shift;
$first=shift;
$last=shift;
$nperfile=shift;
$firstid=shift;


$collcounter=0;
$fileid=$firstid;
$collid=$first;
while($collid<=$last){ 
    $n=1;
    $collstring="";
    while($n<=$nperfile && $collid<=$last){
	$n++;
	$collstring="$collstring ${inname}_$collid";
	$collid++;
    }
    system("KanCopyUtil -o ${outname}_${fileid} $collstring");
    #print "KanCopyUtil -o ${outname}_$fileid $collstring\n";
    $fileid++;  
    $collcounter++;
}
   
print "Created $collcounter files\n";
exit;
