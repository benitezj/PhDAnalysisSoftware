#!/usr/bin/perl
# -w


$modesubdir=shift;
$action=shift;
if($action <0 || $action >6) {die "bad action\n";}
print "Will work on $modesubdir\n";



#####Create the tcl files using CreateTCL.pl script: this will delete all Ntuple(.log,.root,.tcl) files first
if($action==0){
    print "Will clean and create config tcl files.\n";

    #give arguments needed
    $infile=shift;  
  
    system("/u/eb/benitezj/ana42/workdir/SOFTWARE/BATCH/CreateTCL.pl $modesubdir $infile "); 
}

############delete log and root files then submit
if($action==1){
    $queue=shift;
    $nmaxjobs=shift;
    
    if($queue ne "short" 
       and $queue ne "medium" 
       and $queue ne "long" 
       and $queue ne "kanga" 
       and $queue ne "xlong" 
       and $queue ne "xxl") {die "bad queue\n";}
 
    if($nmaxjobs<1){$nmaxjobs=1000;}
    
    $checkpending=`bjobs -q $queue -p | grep $queue |  wc -l`;
    if($checkpending>2000){die "$checkpending>2000\n";}
    

    print "Will submit jobs\n";

    opendir(DIR,$modesubdir ) or die "can't opendir $modsubdir $!";
    @tclfiles = grep{/tcl/} grep { /Ntuple/ } readdir(DIR);
    closedir(DIR);

    $totalfiles=0;
    foreach $file (@tclfiles){
	
	if($totalfiles==$nmaxjobs){last;}

	@barefile=split(/\./,$file);

	$logexists=`[ -f ${modesubdir}/${barefile[0]}.log ] && echo 1 || echo 0`;
	if($logexists==0 and $totalfiles<$nmaxjobs){
	    system("bsub -C 0 -q ${queue} -o ${modesubdir}/${barefile[0]}.log -J ${modesubdir}${barefile[0]} BetaMiniApp ${modesubdir}/${barefile[0]}.tcl&");
	    system("touch ${modesubdir}/${barefile[0]}.log");
	    #create log file so same job will not be submitted twice
	    
	    $totalfiles++;
	}

    } 
    print "Number of jobs submited = $totalfiles \n";
}

###########only check log file for failed jobs
if($action==2){
    print "Will check for failed jobs\n";
    
    system("rm -f $modesubdir/*~");

    opendir(DIR,$modesubdir ) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep {/log/} grep { /Ntuple/ } readdir(DIR);
    closedir(DIR);


    $ngoodfiles=0;
    $totalfiles=0;
    foreach $file (@datasetfiles){
	#print "check $modesubdir/$file\n";		
	
	$logfile="$file";

	$success=0; 
        $exitcode=0;
	open FILE, "< $modesubdir/$logfile" or die "$modesubdir/$logfile not found";	
	while($buffer=<FILE>){
	    chomp $buffer;
	    #print "$buffer \n";

	    @exitword=split(" ",$buffer);

	    #reset checks due to multiple submissions without cleaning logfiles
	    if("$exitword[0] $exitword[1]" eq "set NEvent"){
		$exitcode=0;
		$success=0;
	    }

	    if($exitword[0] eq "Successfully" and $exitword[1] eq  "completed."){
		$success=1;
	    }	    
	    if($exitword[0] eq "Exited" and $exitword[1] eq "with" and $exitword[2] eq "exit" and $exitword[3] eq "code"){		$exitcode=$exitword[4];
	    }


	}
	
	if($success){
	    $ngoodfiles++;
	    #print "$modesubdir/$logfile: Success   Exit Code=$exitcode \n";
	}
	
	if(!$success){
	    print "$modesubdir/$logfile: Failed   Exit Code=$exitcode \n";
	}
	
	
	$totalfiles++;
	close FILE;

    }
    
    print "Number of good jobs = $ngoodfiles/$totalfiles \n";
}

#######check which jobs failed and resubmit them
if($action==3){
    $queue=shift;
    
    print "Will resubmit the failed jobs\n";
    
    system("rm -f $modesubdir/*~");

    opendir(DIR,$modesubdir ) or die "can't opendir $modsubdir $!";
    @datasetfiles =  grep { /DataSet/ } readdir(DIR);
    closedir(DIR);


    $nbadfiles=0;
    $totalfiles=0;
    foreach $file (@datasetfiles){
	#@barefile=split(/\./,$file);      
	#$barefilename=$barefile[0];


	##########determine the id of DataSet file
	$fileid="-1";
	@idword=split /-/,$file;
	if($idword[0] ne "DataSet"){
	    print "File name is not DataSet\n";
	}else{
	    @extword=split /\./,$idword[1];
	    if($extword[1] ne "tcl"){
		print "Extension is not .tcl\n";
	    }else {
		$fileid=$extword[0];   
	    }
	}
	if($fileid eq "-1"){
	    print "Bad file id at collection: $file\n";	
	}
	##############
	
	$barefilename="Ntuple${fileid}";


	#print "check $modesubdir/$file\n";
	$success=0;
	open FILE, "< $modesubdir/${barefilename}.log";	
	while($buffer=<FILE>){
	    chomp $buffer;
	    if($buffer eq "Successfully completed."){
		$success=1;
	    }	  
	}
	close FILE;	
	
	#if job failed then resubmit
	if(!$success and $fileid ne "-1"){

	    system("rm -f ${modesubdir}/${barefilename}.log");
	    system("rm -f ${modesubdir}/${barefilename}.root");
	    
	    system("bsub -C 0 -q ${queue} -o ${modesubdir}/${barefilename}.log -J ${modesubdir}${barefilename} BetaMiniApp ${modesubdir}/${barefilename}.tcl  &");
	    
	    $nbadfiles++;
	}

	$totalfiles++;
	
    }
    print "Number of jobs resubmited = $nbadfiles/$totalfiles \n";
}




#######count the number of particles accepted per job
if($action==5){
    print "Will determin # of particles per event\n";
	
    print "filename: NRequested NAnalyzed EventEff time eventrate nparticles multiplicity\n";

    opendir(DIR,$modesubdir) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep{ /log/} grep { /Ntuple/ } readdir(DIR);
    closedir(DIR);
    
    $totalfiles=0;
    $totalparticles=0;
    $totalevents=0;
    $eventsrequested=0;
    $totaleventsrequested=0;
    $totaltime=0;
    foreach $file (@datasetfiles){

	$nevents=0;
	$nparticles=0;
	$time=0;
	open FILE, "< $modesubdir/$file";	
	while($buffer=<FILE>){
	    chomp $buffer;
	    @words=split(" ",$buffer);
	 
	    #events in DataSet
	    if($words[0] eq "set" and $words[1] eq "NEvent"){
		$eventsrequested=$words[2];
	    }

	    #events succesfully analyzed
            if($words[0] eq "EvtCounter:"  and $words[1] eq "processing" and $words[2] eq "event" and $words[3] eq "#"){
		#EvtCounter: processing event # 150000 		
		$nevents=$words[4];		
	    }	  
	    if($words[0] eq "EvtCounter:EvtCounter:"  and $words[1] eq "total" and $words[2] eq "number" and $words[3] eq "of"){ 
		#EvtCounter:EvtCounter:     total number of events=163296
                @evts=split("=",$words[4]);
		$nevents=$evts[1];		
	    }	  
	    
	    #particles found
	    if($words[1] eq "cosDst" and  $nparticles <1 ){
		$nparticles=$words[3];
	    }
	    
	    #time spent
	    if($words[0] eq "CPU" and $words[1] eq "time"){
		$time=$words[3];
	    }
	    
	    
	}
	close FILE;	
	
	$partperevent=0;
	$eventeff=0;
	$timeeff=0;
	if($nevents!=0 && $eventsrequested!=0 && $time!=0){
	    $partperevent=sprintf("%.2f",$nparticles/$nevents);
	    $eventeff=sprintf("%.3f",$nevents/$eventsrequested);
	    $timeeff=sprintf("%.1f",$nevents/$time);

	    $totaleventsrequested+=$eventsrequested;	    
	    $totalparticles+=$nparticles;
	    $totalevents+=$nevents;
	    $totaltime+=$time;
	}else{
	    print "Bad file: $barefilename \n";
	}
		
	
	print "$file: $eventsrequested $nevents $eventeff $time $timeeff $nparticles $partperevent\n";

    }

    $avgpartperevt=sprintf("%.2f",$totalparticles/$totalevents);
    $overalleventeff=sprintf("%.3f",$totalevents/$totaleventsrequested);
    $evtpersec=sprintf("%.2f",$totalevents/$totaltime);
    print "$totaleventsrequested $totalevents $overalleventeff $totaltime $evtpersec  $totalparticles $avgpartperevt \n";
}
#############




#######remove bad files
if($action==6){
    print "Will check for empty root files and remove them\n";

    opendir(DIR,$modesubdir) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep { /DataSet/ } readdir(DIR);
    closedir(DIR);
    
    $nremoved=0;
    $nfiles=0;
    foreach $file (@datasetfiles){
	$nfiles++;

	##########determine the id of DataSet file
	$fileid="-1";
	@idword=split /-/,$file;
	if($idword[0] ne "DataSet"){
	    print "File name is not DataSet\n";
	}else{
	    @extword=split /\./,$idword[1];
	    if($extword[1] ne "tcl"){
		print "Extension is not .tcl\n";
	    }else {
		$fileid=$extword[0];   
	    }
	}
	if($fileid eq "-1"){
	    print "Bad file id at collection: $file\n";	
	}
	##############
	
	$barefilename="Ntuple${fileid}";
	
	$nevents=0;
	$norootf=0;
	open FILE, "< $modesubdir/${barefilename}.log";	
	while($buffer=<FILE>){
	    chomp $buffer;
	    @words=split(" ",$buffer);	

	    #events succesfully analyzed
	    if($words[0] eq "EvtCounter:EvtCounter:"  and $words[1] eq "total" and $words[2] eq "number" and $words[3] eq "of"){ 
		#EvtCounter:EvtCounter:     total number of events=163296
                @evts=split("=",$words[4]);
		$nevents=$evts[1];		
	    }
	    
	    #SysError in <TFile::Flush>: error flushing file DstarCharged/DstarPi/BFieldChange/DATARun1Off/Ntuple1.root
	    if($words[0] eq "SysError" and $words[2] eq "<TFile::Flush>:"){
		$norootf=1;
	    }
	    
	}
	close FILE;		
	print "$nevents $norootf\n";

	if($nevents<100 or $norootf==1){
	    print " remove $barefilename: $nevents events , NoRootFile=$norootf\n";
	    #system("rm -f $modesubdir/DataSet-${fileid}.tcl");
	    #system("rm -f $modesubdir/${barefilename}.tcl");
	    print("rm -f $modesubdir/${barefilename}.log\n");
	    system("rm -f $modesubdir/${barefilename}.log");
	    print("rm -f $modesubdir/${barefilename}.root\n");
	    system("rm -f $modesubdir/${barefilename}.root");
	    $nremoved++;	    
	}
	    
    }


    print "Removed $nremoved/$nfiles files\n";
}
exit;
#############


