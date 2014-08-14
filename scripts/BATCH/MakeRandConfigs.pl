#!/usr/bin/perl
# -w
###input the directory
$subdir=shift;
$Ncopies=shift;
$action=shift;



####create tcl files according to the following one.
if($action==0){
    $datasetid=shift;
    if($datasetid ==0) {
	die "bad datasetid\n";
    }
    
    $firstfile=shift;
    if($firstfile ==0) {
	die "bad firstfile\n";
    }

###remove all old 
    #system("rm -f ${subdir}/InitRooFitPars_*");
    #system("rm -f ${subdir}/*.log");
    #system("rm -f ${subdir}/FitPars*");
    system("rm -f ${subdir}/*~");
##############

    
foreach $fileid ($firstfile ... ($firstfile+$Ncopies) ){
         
    #name of Ntuple file 
    $outfile="InitRooFitPars_${fileid}";

    #clean
    system("rm -f ${subdir}/${outfile}.txt");
    system("rm -f ${subdir}/${outfile}.C");
    system("rm -f ${subdir}/${outfile}.log");


    $changecountermax=3;
    $changecounter=$changecountermax;
    $sigorbkg=0;

    open OUTFILE, "> ${subdir}/${outfile}.txt" or die "@_";
    open INFILE, "< ${subdir}/InitRooFitPars.txt" or die "@_";
    while($buffer=<INFILE>){
	chomp $buffer;
	@word=split / /,$buffer;

	if($changecounter==$changecountermax){
	    print OUTFILE "${buffer}\n";
	}
	if($changecountermax==3){	
	    if($changecounter<$changecountermax){	   
		$randval=$word[1]+rand()*($word[2]-$word[1]);
		print OUTFILE "$randval $word[1] $word[2] $word[3]\n";
		
		$changecounter++;
	    }
        }
	if($changecountermax==10){
	    if($changecounter==0){	   

		$randval=$word[1]+rand()*($word[2]-$word[1]);
		print OUTFILE "$randval $word[1] $word[2] $word[3]\n";

		
		$changecounter++;
	    }else{ 
		if($changecounter<$changecountermax){	   
		    $randval=$word[2]+rand()*($word[3]-$word[2]);
		    print OUTFILE "${word[0]} ${randval} ${word[2]} ${word[3]} ${word[4]}\n";
		    
		    $changecounter++;
		}
	    }
        }
	
	if($word[0] eq "Signal"){
	    $changecounter=0;
	    $changecountermax=3;
	}
	if($word[0] eq "Background"){
	    $changecounter=0;
	    $changecountermax=10;
	}

    }
    close INFILE;
    close OUTFILE;

    open OUTFILE, "> ${subdir}/${outfile}.C" or die "@_";
    print OUTFILE "{\n";
 
    print OUTFILE "gROOT->ProcessLine(\".x SOFTWARE/RooLogon.C\");\n";
    print OUTFILE "gROOT->ProcessLine(\".x SOFTWARE/Load.C\");\n";
    print OUTFILE "DCPiAnalysis* a=analyzeDCPi(${datasetid},0);\n";
    print OUTFILE "MassFitter*mf=a->GetMassFitter();\n";
    print OUTFILE "if(!mf->Initialize(a->GetMassHisto(\"HDCPi\"),\"${subdir}/${outfile}.txt\"))return 0;\n";
    print OUTFILE "if(mf->Fit()<0)return 0;\n";
    print OUTFILE "mf->SetFileTag(\"_${outfile}\");\n";
    print OUTFILE "mf->Save(\"${subdir}\");\n";
   
    print OUTFILE "}\n";

    close OUTFILE;
}

print "$Ncopies files created.\n";

exit;

}

if($action==1){
    $queue=shift;
    if($queue ne "short" and  $queue ne "medium" and $queue ne "kanga" and $queue ne "long" and $queue ne "xlong" and $queue ne "xxl"){
	exit "Bad queue name\n";
    }

    $firstfile=shift;
    if($firstfile ==0) {
	die "bad firstfile\n";
    }

foreach $fileid ($firstfile ... ($firstfile+$Ncopies) ){
         
    $outfile="InitRooFitPars_${fileid}";    
    ###remove all old 
    system("rm -f ${subdir}/FitPars${outfile}.*");
    system("rm -f ${subdir}/${outfile}.log");

    system("bsub -q ${queue} -o ${subdir}/${outfile}.log -C 0 -J ${subdir}${outfile} root -b ${subdir}/${outfile}.C");

 

}

}


if($action==2){
    $firstfile=shift;
    if($firstfile ==0) {
	die "bad firstfile\n";
    }

    foreach $fileid ($firstfile ... ($firstfile+$Ncopies) ){

    #name of Ntuple file 
    $outfile="InitRooFitPars_${fileid}";    
   
    $EXIT=0; 
    $CPUT=0;
    $success=0;
    $fittime=0;
   
    open INFILE, "< ${subdir}/${outfile}.log" or die "@_";
    while($buffer=<INFILE>){
	chomp $buffer;
	$buffer=~ s/^\s+//;
	$buffer=~ s/\s+$//;
	@word=split / /,$buffer;


	#print "$buffer\n";
	#print "$word[0] $word[1]\n";

	if($word[1] eq "fit" and $word[2] eq "status" and $word[4]==0){
	    $success=1;
	}
	
	if($word[1] eq "fit" and $word[2] eq "time"){
	    $fittime=$word[4];
	}
	
	
	if($word[0] eq "Exited" and $word[1] eq "with" and $word[2] eq "exit" and $word[3] eq "code"){
	    $EXIT=$word[4];
	}
	if($word[0] eq "CPU" and $word[1] eq "time"){
	    @secstring=split /:/,$buffer;
	    $secstring[1]=~ s/^\s+//;
	    @secs=split /:/,$secstring[1];
	    $CPUT=$secs[0];
	}
	
	
    }
    close INFILE;

    print "$outfile: ";
    if($success!=1){
	print "failed code = $EXIT    ";

    }else { print "fit status = 0   ";}
    
    print "CPU Time = $CPUT      ";

    print "fit time = $fittime      ";

    print "\n";
}

}
#############
