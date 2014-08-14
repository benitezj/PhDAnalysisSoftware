#!/usr/bin/perl
# -w

#
#This script creates tcl files for each job
#

$jobdir=shift;
$outdir=shift;
$configfile=shift;
$decfile=shift;
$filter=shift;
$firstjob=shift;
$lastjob=shift;
$Nevent=shift; #number of events per file



foreach $fileid ($firstjob .. $lastjob){
   
    $firstevent=$Nevent*$fileid+1;
    
    $outfile="Collection${fileid}";

    $changeverifier=0;

    #Change the parameter of the .tcl file to choose which events you want to analyze
    open OUTFILE, "> ${jobdir}/${outfile}.tcl" or die "@_";
    open INFILE, "< ${configfile}" or die "@_";
    while($buffer=<INFILE>){
	
	@word=split / /,$buffer;
	
	if($word[1] eq "RUNNUM"){
	    print OUTFILE "set RUNNUM 98765${fileid}\n";
	    $changeverifier++;
	}
	if($word[1] eq "SimAppBkndFirstEvent"){
	    print OUTFILE "set SimAppBkndFirstEvent $firstevent\n";
	    $changeverifier++;
	}
	if($word[1] eq "NEVENT"){
	    print OUTFILE "set NEVENT ${Nevent}\n";
	    $changeverifier++;
	}
	if($word[1] eq "UDECAY"){
	    print OUTFILE "set UDECAY $decfile\n";
	    $changeverifier++;
	}
	if($word[1] eq "MooseHBookFile"){
	    print OUTFILE "set MooseHBookFile  ${jobdir}/${outfile}.root \n";
	    $changeverifier++;
	}
	if($word[1] eq "MooseOutputCollection"){
	    print OUTFILE "set MooseOutputCollection  ${outdir}/${outfile} \n";
	    $changeverifier++;
	}
	if($word[0] eq "source"){
	    print OUTFILE "source $filter \n"; 
	    $changeverifier++;
	}
       

	if( $word[1] ne "RUNNUM"
	    and $word[1] ne "SimAppBkndFirstEvent"
	    and $word[1] ne "NEVENT"
	    and $word[1] ne "UDECAY"
	    and $word[1] ne "MooseHBookFile"
	    and $word[1] ne "MooseOutputCollection"
	    and $word[0] ne "source"
	    ){
	    print OUTFILE "${buffer}";
	}
	
	

    }
    close INFILE;
    close OUTFILE;


    if($changeverifier!=7){
	print "${changeverifier} changes made.\n";
    }

}
exit;
#############




