#!/usr/bin/perl
# -w
#
#This script creates a config tcl file for each skim job
#
#NOTE: execute this script from workdir

###input the directory
$modesubdir=shift;#where job config files will be written
$maxnumberJobs=shift; #total number of jobs 
if($maxnumberJobs<=0){$maxnumberJobs=1000000;}
$firstfile=shift;#first job
if($firstfile<=0){$firstfile=1;}
$infile=shift;#template config file
$outdir=shift;#where collections will be written: needed if writting to /work/users/ then job tcl/log files cannot go there.

print "$modesubdir\n";
print "$maxnumberJobs\n";
print "$firstfile\n";
print "$infile\n";
print "$outdir\n";

@patharray=split(/\//,$modesubdir);
$maindir=$patharray[1];
$subdir=$patharray[1];

####


###remove 
system("rm -f ${modesubdir}/*~");
print "*~ (back-up) files have been removed.\n";
##############

###Now create one tcl file for each of the DataSet files
opendir(DIR,$modesubdir ) or die "can't opendir $fullmodsubdir $!";
@datasetfiles = grep { /DataSet/ } readdir(DIR);
closedir(DIR);

####create tcl files according to the following one.
#$infile="${workdir}/SOFTWARE/BATCH/${maindir}_Configuration.tcl";
#$infile="SkimTest/SkimSnippet.tcl";

$nfiles=0;
foreach $collectionfile (@datasetfiles){
    
    #determine number of events in the dataset
    $Nevent=0;
    open DATASETFILE, "< $modesubdir/$collectionfile" or die "@_";
    while($buffer=<DATASETFILE>){
	chomp $buffer;
	@word=split / /,$buffer;	
	if($word[1] eq "In" && $word[2] eq "this" && $word[3] eq "tcl" && $word[4] eq "file:"){
	    $Nevent=$word[7];
	}	
    }
    if($Nevent==0){
	$Nevent=10000000;
	print "Nevent not found in $collectionfile. Set to: ${Nevent}. \n";
    }
    ########################
   
    ##########determine the id of DataSet file
    $fileid="-1";
    @idword=split /-/,$collectionfile;
    if($idword[0] ne "DataSet"){
	print "File name is not right\n";
    }else{
	@extword=split /\./,$idword[1];
	if($extword[1] ne "tcl"){
	    print "Extension is not .tcl\n";
	}else {
	    $fileid=$extword[0];   
	}
    }
    if($fileid eq "-1"){
	print "Bad file id at collection: $collectionfile\n";	
    }
    ##############


    if($firstfile<=$fileid && $fileid<($firstfile+$maxnumberJobs)){
	$nfiles++;
    

	#name of Ntuple file 
	$outfile="Snippet${fileid}";
	
	#check the number changes done to the default tcl file
	$changeverifier=0;
	
	#Change the parameters of the .tcl file to choose which events you want to analyze
	open OUTFILE, "> ${modesubdir}/${outfile}.tcl" or die "@_";
	open INFILE, "< ${infile}" or die "@_";
	while($buffer=<INFILE>){
	    #chomp $buffer;
	    @word=split / /,$buffer;
	    
	    if($word[1] eq "SkimOutputDir"){
		#print OUTFILE "set SkimOutputDir ${modesubdir}\n";
		print OUTFILE "set SkimOutputDir $outdir\n";
		#print OUTFILE $buffer;
		$changeverifier++;
	    }
	    if($word[1] eq "SkimInputTcl"){
		print OUTFILE "set SkimInputTcl ${modesubdir}/${collectionfile}\n";
		$changeverifier++;
	    }
	    if($word[1] eq "SkimOutputSuffix"){
		print OUTFILE "set SkimOutputSuffix _${fileid}\n";
		$changeverifier++;
	    }
	    if($word[1] eq "SkimNEvent"){
		print OUTFILE "set SkimNEvent $Nevent\n";
	    $changeverifier++;
	    }
	    if($word[1] ne "SkimInputTcl"
	       and $word[1] ne "SkimOutputSuffix"
	       and $word[1] ne "SkimNEvent"
	       and $word[1] ne "SkimOutputDir"){
		print OUTFILE "${buffer}";
	    }
	    
	}
	close INFILE;
	close OUTFILE;
	
	
	if($changeverifier!=4){
	    print "warning: ${changeverifier}/4 changes made \n";
	}
	
    }
    
}

print "$nfiles tcl files created.\n";

exit;
#############




