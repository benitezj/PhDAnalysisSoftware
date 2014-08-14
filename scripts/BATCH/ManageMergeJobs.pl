#!/usr/bin/perl
# -w
#
#This script creates files with a list of event collections 
#

#name of original collections (omit underscore)
$inname=shift;
#name of merged collections
$outname=shift;
#first collection to merge
$first=shift;
#last collection to merge
$last=shift;
#number of collections per merge
$nperfile=shift;
#id of first new collection
$firstid=shift;
#number of merges per batch job
$nmergesperjob=shift;


$queue="kanga";

$jobcounter=0;
$jobfirst=$first;
$joblast=$first+$nmergesperjob*$nperfile-1;
$jobfirstid=$firstid;
while($joblast<$last){ 

    system("bsub -q $queue -C 0 -J Merge$jobfirstid -o Merge$jobfirstid.log SOFTWARE/BATCH/MyCopyUtil.pl $inname $outname $jobfirst $joblast $nperfile $jobfirstid");
    #print "bsub -q $queue -C 0 -J Merge$jobfirstid -o Merge$jobfirstid.log SOFTWARE/BATCH/MyCopyUtil.pl $inname $outname $jobfirst $joblast $nperfile $jobfirstid \n";

    $jobfirst+=$nmergesperjob*$nperfile;
    $joblast+=$nmergesperjob*$nperfile;
    $jobfirstid+=$nmergesperjob;

    $jobcounter++;
}
   
#last job is special
system("bsub -q $queue -C 0 -J Merge$jobfirstid -o Merge$jobfirstid.log SOFTWARE/BATCH/MyCopyUtil.pl $inname $outname $jobfirst $last $nperfile $jobfirstid");
#print "bsub -q $queue -C 0 -J Merge$jobfirstid -o Merge$jobfirstid.log SOFTWARE/BATCH/MyCopyUtil.pl $inname $outname $jobfirst $last $nperfile $jobfirstid \n";
$jobcounter++;


print "$jobcounter jobs submitted\n";
exit;
