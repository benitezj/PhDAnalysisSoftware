#!/usr/bin/perl
# -w
#

###input the directory
$modesubdir=shift; #for example in the follwing format: Ds1pToDstKs/D0ToKPi/SP-7796 
$infile=shift;
$workdir="/u/eb/benitezj/ana42/workdir";
$fullmodesubdir="${workdir}/${modesubdir}";


print "CreatTCL for ${modesubdir}\n";


###remove all old Ntuple files
system("rm -f ${modesubdir}/*~");
print "${modesubdir}/*~ have been removed.\n";

system("rm -f ${modesubdir}/Ntuple1*.log");
system("rm -f ${modesubdir}/Ntuple2*.log");
system("rm -f ${modesubdir}/Ntuple3*.log");
system("rm -f ${modesubdir}/Ntuple4*.log");
system("rm -f ${modesubdir}/Ntuple5*.log");
system("rm -f ${modesubdir}/Ntuple*.log");
print "${modesubdir}/Ntuple*.log have been removed.\n";

system("rm -f ${modesubdir}/Ntuple1*.root");
system("rm -f ${modesubdir}/Ntuple2*.root");
system("rm -f ${modesubdir}/Ntuple3*.root");
system("rm -f ${modesubdir}/Ntuple4*.root");
system("rm -f ${modesubdir}/Ntuple5*.root");
system("rm -f ${modesubdir}/Ntuple*.root");
print "${modesubdir}/Ntuple*.root have been removed.\n";

system("rm -f ${modesubdir}/Ntuple1*.tcl");
system("rm -f ${modesubdir}/Ntuple2*.tcl");
system("rm -f ${modesubdir}/Ntuple3*.tcl");
system("rm -f ${modesubdir}/Ntuple4*.tcl");
system("rm -f ${modesubdir}/Ntuple5*.tcl");
system("rm -f ${modesubdir}/Ntuple*.tcl");
print "${modesubdir}/Ntuple*.tcl have been removed.\n";
##############

###Now create one tcl file for each of the DataSet files
opendir(DIR,$modesubdir ) or die "can't opendir $fullmodsubdir $!";
@datasetfiles = grep { /DataSet/ } readdir(DIR);
closedir(DIR);

####create tcl files according to the following one.
$nfiles=0;
foreach $collectionfile (@datasetfiles){
    $nfiles++;
    
    
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
	print "Bad file id at collection: $collectionfile\n";	
    }
    ##############


    #name of Ntuple file 
    $outfile="Ntuple${fileid}";

    #check the number changes done to the default tcl file
    $changeverifier=0;

    #Change the parameters of the .tcl file to choose which events you want to analyze
    open OUTFILE, "> ${modesubdir}/${outfile}.tcl" or die "@_";
    open INFILE, "< ${infile}" or die "@_";
    while($buffer=<INFILE>){
	#chomp $buffer;
	@word=split / /,$buffer;
	
	if($word[1] eq "histFileName"){
	    print OUTFILE "FwkCfgVar histFileName \"${modesubdir}/${outfile}.root\" \n";
	    #if($whichhome==1){print OUTFILE "FwkCfgVar histFileName \"${modesubdir}/${outfile}.root\" \n";}
	    #if($whichhome==2){print OUTFILE "FwkCfgVar histFileName \"/data/benitezj${maindir}${modedir}${subdir}${outfile}.root\" \n";}
	    $changeverifier++;
	}
	if($word[1] eq "NEvent"){
	    print OUTFILE "FwkCfgVar NEvent ${Nevent}\n";
	    $changeverifier++;
	}
#	if($word[1] eq "ConfigPatch"){
#	    if($DATAorMC eq "DATA"){ 
#		print OUTFILE "FwkCfgVar ConfigPatch \"Run2\"\n";
#		$changeverifier++;
#	    }
#	    if($DATAorMC eq "MC"){ 
#		print OUTFILE "FwkCfgVar ConfigPatch \"MC\"\n";
#		$changeverifier++;
#	    }
#	    
#	}
	if($word[0] eq "source"){
	    print OUTFILE "source $modesubdir/$collectionfile \n";
	    $changeverifier++;
	}

	
#	if($word[1] eq "MCTruth"){
#	    print OUTFILE "FwkCfgVar MCTruth $TruthMatch\n";
#	    $changeverifier++;
#	}
#	if($word[0] eq "writeEveryEvent"){
#	    print OUTFILE "writeEveryEvent set $FillMC\n";
#	    $changeverifier++;
#	}
#	if($word[0] eq "fillMC"){
#	    print OUTFILE "fillMC set $FillMC\n";
#	    $changeverifier++;
#	}

	if(
	   $word[1] ne "histFileName"
	   and $word[1] ne "NEvent" 
#	   and $word[1] ne "ConfigPatch" 
	   and $word[0] ne "source" 
#	   and $word[1] ne "MCTruth"
#	   and $word[0] ne "writeEveryEvent"
#	   and $word[0] ne "fillMC"
	   ){
	    print OUTFILE "${buffer}";
	}

    }
    close INFILE;
    close OUTFILE;


    if($changeverifier!=3){
	print "warning: ${changeverifier}/7 changes made \n";
    }

}

print "$nfiles tcl files created.\n";

exit;
#############




