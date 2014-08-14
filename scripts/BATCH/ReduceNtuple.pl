#!/usr/bin/perl
# -w
 
$modesubdir=shift;
$action=shift;

$logfile="$modesubdir/Ntuple.log";
$macrofile="$modesubdir/batch_Ntuple.C";
$newrootfile="$modesubdir/Ntuple_New";
$rootfile="$modesubdir/Ntuple";


if($action==1){
    
    $queue=shift;#"medium";

    print "Will clean and submit jobs.\n";
    
    print("rm -f $logfile\n");  
    system("rm -f $logfile");    

    print("rm -f ${newrootfile}.root\n");
    system("rm -f ${newrootfile}.root");

    print("bsub -q $queue -o $logfile -C 0 -J $modesubdir root -b Load.C  $macrofile\n");
    system("bsub -q $queue -o $logfile -C 0 -J $modesubdir root -b Load.C $macrofile");

}


if($action==2){
    print "Will check for failed jobs\n";
                            
    $success=0;
    $events=0;    
    open FILE, "< $logfile";
    while($buffer=<FILE>){
	chomp $buffer;
	@word=split(" ",$buffer);
	if("$word[0] $word[1] $word[2]" eq "Total events ="){
	    $events=$word[3];
	}
	if($buffer eq "Successfully completed."){
	    $success=$buffer;
	}
	if("$word[0] $word[1] $word[2] $word[3] $word[4]" eq "*** Break *** segmentation violation"){
	    $breaksev=$buffer;
	}
    }
    
    print "$logfile : $success   Events=$events  $breaksev\n";
}

if($action==3){
    print "mv ${newrootfile}.root ${rootfile}.root\n";
    system("mv ${newrootfile}.root ${rootfile}.root");
}
