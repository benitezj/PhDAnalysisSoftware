#!/usr/bin/perl
# -w

$directory=shift;
$action=shift;#look below for definition
$Njobs=shift;
if($Njobs<=0){$Njobs=1000000;}
$firstfile=shift;#id of first to create/manage
if($firstfile<=0){$firstfile=1;}

print "Working On ===> $directory $action $Njobs $firstfile\n";

@dirs=split(/\//,$directory);
$maindir=$dirs[0];
$modedir=$dirs[1];
$subdir=$dirs[2];



###remove all old  files
if($action==-1){
    system("rm -f ${directory}/*~");     
    system("rm -f ${directory}/Snippet*");    
    system("rm -f ${directory}/*.root");
    
    print "Snippet(.log,.tcl), *.root and *~ (back-up) files have been removed.\n";
}

#####Create the tcl files using CreateTCL.pl script: this will delete all Ntuple(.log,.root,.tcl) files first
if($action==0){
    #the config file template
    $infile=shift;
    #the output directory for the collections
    $outdir=shift;       
    if($outdir eq ""){
	$outdir=$directory;
    }

    print "${infile}\n";
    print "${outdir}\n";
    
    system("SOFTWARE/BATCH/CreateSkimTCL.pl $directory $Njobs $firstfile $infile $outdir"); 
    print "+++++Remember to clean out the .root files by hand.++++++\n";
}

############delete log files then submit job for each of the DataSet files
if($action==1){ 
    $queue=shift;

    system("rm -f $directory/*~");
    print "All ~  files will be removed.\n";

    opendir(DIR,$directory ) or die "can't opendir $modsubdir $!";
    @tclfiles = grep { /DataSet/ } readdir(DIR);
    closedir(DIR);


    $totalfiles=0;
    foreach $file (@tclfiles){
	@barefile=split(/\./,$file);

	$fileid=`echo $file | sed s/DataSet-/""/ | sed s/\.tcl/""/`;
	chomp($fileid);
	if($fileid<1||50000<$fileid){print "Bad file id at collection: $file\n";next;}
	
	if($firstfile<=$fileid && $fileid<($firstfile+$Njobs)){
	    system("rm -f ${directory}/Snippet${fileid}.log");
	    system("bsub -C 0 -q ${queue} -o ${directory}/Snippet${fileid}.log -J ${subdir}Snippet${fileid} SkimMiniApp ${directory}/Snippet${fileid}.tcl  &");
	    
	    $totalfiles++;
	}	    	    
    } 
    print "Number of jobs submited = $totalfiles \n";
}

###########only check log file for failed jobs
if($action==2){
    
    system("rm -f $directory/*~");

    opendir(DIR,$directory ) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep { /DataSet/ } readdir(DIR);
    closedir(DIR);
       
    $ngoodfiles=0;
    $totalfiles=0;
    foreach $file (@datasetfiles){

	##########determine the id of DataSet file
	$fileid=`echo $file | sed s/DataSet-/""/ | sed s/\.tcl/""/`;
	chomp($fileid);
	if($fileid<1||50000<$fileid){print "Bad file id at collection: $file\n";next;}

	##############only due the ones in the range
	if($firstfile<=$fileid && $fileid<($firstfile+$Njobs)){
	   	
	    $logfile="Snippet${fileid}.log";
	   
	    #make 3 checks to declare success
	    $exitcode=0; 
	    $cputime=0;
	    $evtproc=0;
	    $reachedlastcollection=0;
	    $SkimNEvt=0;
	   
	    open FILE, "< $directory/$logfile";	
	    while($buffer=<FILE>){
		chomp $buffer;

		@exitword=split(" ",$buffer);

		#reset checks due to multiple submissions without cleaning logfiles
		if("$exitword[0] $exitword[1]" eq "set SkimNEvent"){
		    $exitcode=0; 
		    $cputime=0;
		    $evtproc=0;
		    $reachedlastcollection=0;
		    $SkimNEvt=$exitword[2];
		}

		if($exitword[0] eq "Successfully" and $exitword[1] eq  "completed."){
		    $exitcode=$exitword[0];
		}

                #check that went throug all collections (Graham found jobs can report Successfully without actually finishing)
                #21:57:04.928 KanEventInput::KanEventInput.cc(500):reached end of last input collection
		if("$exitword[2] $exitword[3] $exitword[4] $exitword[5] $exitword[6]" eq "end of last input collection" ){
		    $reachedlastcollection=1;
		}
		
		if($exitword[0] eq "Exited" and $exitword[1] eq "with" and $exitword[2] eq "exit" and $exitword[3] eq "code"){
		    $exitcode=$exitword[4];
		}

		if($exitword[0] eq "CPU" and $exitword[1] eq "time" and $exitword[2] eq ":" and $exitword[4] eq "sec."){
		    $cputime=$exitword[3]; 
		}	
		
 		if($exitword[1] eq "EvtCounter:" and $exitword[2] eq "processing" 
 		   and $exitword[3] eq "event" and $exitword[4] eq "#"){
 		    $evtproc=$exitword[5];
 		}
		if($exitword[1] eq "EvtCounter:EvtCounter:"  and $exitword[2] eq "total" 
		   and $exitword[3] eq "number" and $exitword[4] eq "of"){
		    $evtproc=`echo $exitword[5] | sed s/events=/""/`; 
		}
	    }
	
	    $success=0;
	    if($cputime>60  && $exitcode eq "Successfully" && ($reachedlastcollection || abs($SkimNEvt-$evtproc)<2)){$success=1;}
    
	    if($success){
		$ngoodfiles++;
	    }
	
	    if(!$success){
		print "$directory/$logfile Failed: exit.code=${exitcode}, cpu=${cputime}, Nproc/NSet=${evtproc}/${SkimNEvt}, reached.last.coll=${reachedlastcollection} \n";
	    }
	    	    
	    $totalfiles++;
	    close FILE;
	}
	
    }
    
    print "Number of good jobs = $ngoodfiles/$totalfiles \n";
}

#######check which jobs failed and resubmit them
if($action==3){
    $queue=shift;

    print "Will remove failed collections and resubmit.\n";
    system("rm -f $directory/*~");

    opendir(DIR,$directory ) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep { /DataSet/ } readdir(DIR);
    closedir(DIR);


    $nbadfiles=0;
    $totalfiles=0;
    foreach $file (@datasetfiles){

	##########determine the id of DataSet file
	$fileid=`echo $file | sed s/DataSet-/""/ | sed s/\.tcl/""/`;
	chomp($fileid);
	if($fileid<1||50000<$fileid){print "Bad file id at collection: $file\n";next;}
	
	$barefilename="Snippet${fileid}";
	$totalfiles++;

	$exitcode=0; 
	$cputime=0;	    
	$reachedlastcollection=0;
	$evtproc=0;
	$SkimNEvt=0;
	open FILE, "< $directory/${barefilename}.log";	
	while($buffer=<FILE>){
	    chomp $buffer;
	    @exitword=split(" ",$buffer);

	    #reset checks due to multiple submissions without cleaning logfiles
	    if("$exitword[0] $exitword[1]" eq "set SkimNEvent"){
		$exitcode=0; 
		$cputime=0;
		$evtproc=0;
		$reachedlastcollection=0;
		$SkimNEvt=$exitword[2];
	    }

	    if("$exitword[2] $exitword[3] $exitword[4] $exitword[5] $exitword[6]" eq "end of last input collection" ){
		$reachedlastcollection=1;
	    }
	    if($exitword[0] eq "Successfully" and $exitword[1] eq  "completed."){
		$exitcode="Successfully";
	    }
	    if($exitword[0] eq "Exited" and $exitword[1] eq "with" and $exitword[2] eq "exit" and $exitword[3] eq "code"){
		$exitcode=$exitword[4];
	    }
	    if($exitword[0] eq "CPU" and $exitword[1] eq "time" and $exitword[2] eq ":" and $exitword[4] eq "sec."){
		$cputime=$exitword[3]; 
	    }
	    if($exitword[1] eq "EvtCounter:" and $exitword[2] eq "processing" 
	       and $exitword[3] eq "event" and $exitword[4] eq "#"){
		$evtproc=$exitword[5];
	    }
	    if($exitword[1] eq "EvtCounter:EvtCounter:"  and $exitword[2] eq "total" 
	       and $exitword[3] eq "number" and $exitword[4] eq "of"){
		$evtproc=`echo $exitword[5] | sed s/events=/""/`; 
	    }		
	}
	close FILE;		
	$success=0;
	if($cputime>60 and $exitcode eq "Successfully" && ($reachedlastcollection || abs($SkimNEvt-$evtproc)<2)){$success=1;}
	

	#if job failed then resubmit
	if(!$success){
	    if($firstfile<=$fileid && $fileid<($firstfile+$Njobs)){
		#remove collection if failed (doesnt work for /work/users/disk)
		system("rm -f ${directory}/MyDRecoCleanTagSkim_${fileid}.01.root");
		system("rm -f ${directory}/${barefilename}.log");

		system("bsub -C 0 -q ${queue} -o ${directory}/${barefilename}.log -J ${subdir}${barefilename} SkimMiniApp ${directory}/${barefilename}.tcl  &");
		
		#print "$fileid\n";

		$nbadfiles++;
	    }
	}	
    }
    print "Number of jobs resubmited = $nbadfiles/$totalfiles \n";
}



#######count the number of particles accepted per job
if($action==5){
    $totalselected;
    $totalanalyzed;
    $totalrequested;
    
    opendir(DIR,$directory ) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep { /DataSet/ } readdir(DIR);
    closedir(DIR);

    $totalfiles=0;
    foreach $file (@datasetfiles){
	
	##########determine the id of DataSet file
	$fileid=`echo $file | sed s/DataSet-/""/ | sed s/\.tcl/""/`;
	chomp($fileid);
	if($firstfile<=$fileid && $fileid<($firstfile+$Njobs)){	    
	    $totalfiles++;
	    $barefilename="Snippet${fileid}";
	    	    
	    $nanalyzed=0;
	    $nselected=0;
	    $nrequested=0;
	    $cputime=0;
	    open FILE, "< $directory/${barefilename}.log";	
	    while($buffer=<FILE>){
		chomp $buffer;
		@words=split(" ",$buffer);
		@evts=split(/=/,$words[5]);
		
		
		if("$words[0] $words[1]" eq "set SkimNEvent" &&  $nselected<1 ){
		    $nrequested=$words[2];		
		}	  
		if("$words[2] $words[3] $words[4] $evts[0]" eq "total number of events" &&  $nselected<1 ){
		    $nselected=$evts[1];
		}	  
		if("$words[1]     $words[2] $words[3] $words[4] $evts[0]" eq "EvtCounter:EvtCounter:     total number of events"){
		    $nanalyzed=$evts[1];
		}	  
		if($words[0] eq "CPU" and $words[1] eq "time" and $words[2] eq ":" and $words[4] eq "sec."){
		    $cputime=$words[3]; 
		}	

	    }
	    close FILE;	
	    
	    $ratio1=0;
	    $ratio2=0;
	    $ratio3=0;
	    if($nanalyzed!=0){$ratio1=$nselected/$nanalyzed;}
	    if($nrequested!=0){$ratio2=$nanalyzed/$nrequested;}
	    if($cputime!=0){$ratio3=$nanalyzed/$cputime;}
	    print " $barefilename:events selected/analyzed/requested $nselected/$nanalyzed/$nrequested/$cputime=$ratio1/$ratio2/$ratio3\n";
	    $totalrequested+=$nrequested;
	    $totalselected+=$nselected; 
	    $totalanalyzed+=$nanalyzed;  
	}


    }
    
    $ratio1=0;
    $ratio2=0;
    if($totalanalyzed!=0){$ratio1=$totalselected/$totalanalyzed;}
    if($totalrequested!=0){$ratio2=$totalanalyzed/$totalrequested;}
    print " $barefilename:total selected/analyzed/requested $totalselected/$totalanalyzed/$totalrequested=$ratio1/$ratio2\n";
}


#############
if($action==6){
    $totalselected;
    $totalanalyzed;
    
    opendir(DIR,$directory ) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep { /DataSet/ } readdir(DIR);
    closedir(DIR);

    $totalfiles=0;
    foreach $file (@datasetfiles){
	##########determine the id of DataSet file
	$fileid=`echo $file | sed s/DataSet-/""/ | sed s/\.tcl/""/`;
	chomp($fileid);
	if($firstfile<=$fileid && $fileid<($firstfile+$Njobs)){	    		    
	    $totalfiles++;
	    $barefilename="Snippet${fileid}";
	    	    
	    $nanalyzed=0;
	    $nselected=0;
	    open FILE, "< $directory/${barefilename}.log";	
	    while($buffer=<FILE>){
		chomp $buffer;
		@words=split(" ",$buffer);
		@evts=split(/=/,$words[5]);

		if("$words[1] $words[2] $words[3] $words[4] $evts[0]" eq "EvtCounter:EvtCounter: total number of events"){
		    $nanalyzed=$evts[1];
		}
		if("$words[1] $words[2] $words[3]" eq "MyDsStarFromCCbarSkim: nDs selected"){
		    #11:40:56.672 MyDsStarFromCCbarSkim: nDs selected 5424
		    $nselected=$words[4];		    
		}	  	  

	    }
	    close FILE;	
	    
	    $ratio1=0;
	    if($nanalyzed!=0){$ratio1=$nselected/$nanalyzed;}
	    print " $barefilename : selected/analyzed=$nselected/$nanalyzed=$ratio1\n";

	    $totalselected+=$nselected; 
	    $totalanalyzed+=$nanalyzed;  
	}


    }
    
    $ratio=0;
    if($totalanalyzed!=0){$ratio=$totalselected/$totalanalyzed;}
    print "====>totalFiles=$totalfiles selected/analyzed=$totalselected/$totalanalyzed=$ratio\n";
}
exit;
#############
