#!/usr/bin/perl
# -w


$modesubdir=shift;
$action=shift;
$workdir="/u/eb/benitezj/ana42/workdir";
$queue="kanga";
$fullmodesubdir="${workdir}/${modesubdir}";

print "Will work on $fullmodesubdir\n";

############delete log and root files then submit
if($action==1){
    print "Will clean and submit jobs\n";

    system("rm -f $fullmodesubdir/*.log");
    system("rm -f $fullmodesubdir/*.root");
    print "All .root and .log files have been removed.\n";

    opendir(DIR,$fullmodesubdir ) or die "can't opendir $modsubdir $!";
    @tclfiles = grep { /Collection/ } readdir(DIR);
    closedir(DIR);

    $totalfiles=0;
    foreach $file (@tclfiles){
	@barefile=split(/\./,$file);

	if($barefile[1] eq "tcl"){
	    #print "submit $fullmodesubdir/$barefile[0]\n";


	    system("bsub -C 0 -q ${queue} -o ${fullmodesubdir}/${barefile[0]}.log -J ${fullmodesubdir}${barefile[0]} MooseApp ${fullmodesubdir}/${barefile[0]}.tcl&");
	    #print(" bsub -C 0 -q ${queue} -o ${fullmodesubdir}/${barefile[0]}.log -J ${barefile[0]} MooseApp ${fullmodesubdir}/${barefile[0]}.tcl&\n");
	    
	    $totalfiles++;
	}

    } 
    print "Number of jobs submited = $totalfiles \n";
}

###########only check log file for failed jobs
if($action==2){
    print "Will check for failed jobs\n";
    
    system("rm -f $fullmodesubdir/*~");

    opendir(DIR,$fullmodesubdir ) or die "can't opendir $modsubdir $!";
    @datasetfiles = grep { /tcl/ } readdir(DIR);
    closedir(DIR);


    $ngoodfiles=0;
    $totalfiles=0;
    foreach $file (@datasetfiles){
	#print "check $fullmodesubdir/$file\n";
	
	
	##########determine the id of DataSet file
	$fileid="-1";
	@extword=split /\./,$file;
	if($extword[1] ne "tcl"){
	    print "Extension is not .tcl\n";
	}else {
	    $fileid=$extword[0];   
	}

	if($fileid eq "-1"){
	    print "Bad file id at collection: $file\n";	
	}
	##############

	
	$logfile="${fileid}.log";

	$success=0; 
        $exitcode=0;
	open FILE, "< $fullmodesubdir/$logfile";	
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
	}
	
	if(!$success){
	    print "$fullmodesubdir/$logfile: Failed   Exit Code=$exitcode \n";
	}
	
	
	$totalfiles++;
	close FILE;

    }
    
    print "Number of good jobs = $ngoodfiles/$totalfiles \n";
}

#######check which jobs failed and resubmit them
if($action==3){
    print "Will resubmit the failed jobs\n";
    
    system("rm -f $fullmodesubdir/*~");

    opendir(DIR,$fullmodesubdir ) or die "can't opendir $modsubdir $!";
    #@datasetfiles = grep { /\.log$/ } readdir(DIR);
    @datasetfiles = grep { /tcl/ } readdir(DIR);
    closedir(DIR);


    $nbadfiles=0;
    $totalfiles=0;
    foreach $file (@datasetfiles){
	#@barefile=split(/\./,$file);      
	#$barefilename=$barefile[0];


	##########determine the id of DataSet file
	$fileid="-1";
	@extword=split /\./,$file;
	if($extword[1] ne "tcl"){
	    print "Extension is not .tcl\n";
	}else {
	    $fileid=$extword[0];   
	}
	
	if($fileid eq "-1"){
	    print "Bad file id at collection: $file\n";	
	}
	##############
	
	$barefilename=$fileid;


	#print "check $fullmodesubdir/$file\n";
	$success=0;
	open FILE, "< $fullmodesubdir/${barefilename}.log";	
	while($buffer=<FILE>){
	    chomp $buffer;
	    if($buffer eq "Successfully completed."){
		$success=1;
	    }	  
	}
	close FILE;	
	
	#if job failed then resubmit
	if(!$success and $fileid ne "-1"){

	    system("rm -f ${fullmodesubdir}/${barefilename}.log");
	    system("rm -f ${fullmodesubdir}/${barefilename}.root");
	    
	    system("bsub -C 0 -q ${queue} -o ${fullmodesubdir}/${barefilename}.log -J ${barefilename} MooseApp ${modesubdir}/${barefilename}.tcl  &");
	    
	    $nbadfiles++;
	}

	$totalfiles++;
	
    }
    print "Number of jobs resubmited = $nbadfiles/$totalfiles \n";
}



