#!/usr/bin/perl
# -w

$jobid=shift;
#$checkpending=`bjobs -q $queue -p | grep $queue |  wc -l`;
#$logexists=`[ -f ${modesubdir}/${barefile[0]}.log ] && echo 1 || echo 0`;

$jobidcheck=$jobid;

#get the initial memory and time
$jobstats=`ps v ${jobid}`;
@jobstatswords=split(" ",$jobstats);
$initmem=$jobstatswords[18];
$inittimestring=$jobstatswords[13];
@inittimewords=split(":",$inittimestring);
$inittime=60*$inittimewords[0]+$inittimewords[1];

while($jobidcheck==$jobid){
    
    #wait 2 seconds between checks;
    sleep 2;
    
    $jobstats=`ps v ${jobid}`;
    #print " $jobstats\n";
    @jobstatswords=split(" ",$jobstats);
    
    #when job is terminated this script will terminate
    $jobidcheck=$jobstatswords[10];
    
    #get the time
    $timestring=$jobstatswords[13];
    @timewords=split(":",$timestring);
    $time=60*$timewords[0]+$timewords[1] - $inittime;

    print "processId=$jobidcheck time=$time  memory=";
    $mem=10*$initmem;
    while($mem<10*$jobstatswords[18]){
	print "-";
	$mem++;
    }
    print ">$jobstatswords[18]%\n";
    

}



