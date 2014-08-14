#!/usr/bin/perl
# -w
#
#This script creates files with a list of event collections 
#
$collname=shift;
$first=shift;
$last=shift;

$counter=0;
$collid=$first;
while($collid<=$last){
    system("KanUserAdmin del ${collname}_$collid");
    #print "KanUserAdmin del ${collname}_$collid\n";
    $collid++;
    $counter++;
}
   
print "deleted $counter collections\n";
exit;
