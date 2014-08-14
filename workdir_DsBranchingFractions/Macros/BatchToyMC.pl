#!/usr/bin/perl
# -w
 
$modesubdir="DsTolnu/SP1005Skim/DsReco/Final/ToyMC";
#$modesubdir=shift;
$action=shift;
$firstjob=shift;
$lastjob=shift;


#create config file
if($action==0){
    $ncreated=0;
    print("rm -f logfile\n");
    print("rm -f macrofile\n");
    print("rm -f datafile.root\n"); 
    for($i=$firstjob;$i<=$lastjob;$i++){
	$logfile="$modesubdir/CreateData${i}.log";
	$macrofile="$modesubdir/batchToy${i}.C";    
	$datafile="$modesubdir/ToyData${i}.root";

	#print("rm -f $logfile\n");  
	system("rm -f $logfile");    
	#print("rm -f $macrofile\n");  
	system("rm -f $macrofile");    	
	#print("rm -f $datafile\n");
	system("rm -f $datafile");
	
	
	open OUTFILE, "> $macrofile" or die "@_";
	print OUTFILE "{\n";
	print OUTFILE "DRecoDsInclusiveAnalysis*a=analyzeDsInclusive(2,0);\n";
	print OUTFILE "a->CreateToys(${i});\n";
	print OUTFILE "}\n";
	close OUTFILE;
	
	$ncreated++;
    }
    
    print "Created $ncreated files\n";

}


#create data sets
if($action==1){
    $queue=shift;
    
    for($i=$firstjob;$i<=$lastjob;$i++){
	$logfile="$modesubdir/CreateData${i}.log";
	$macrofile="$modesubdir/batchToy${i}.C";    
	$datafile="$modesubdir/ToyData${i}.root";

	print("rm -f $logfile\n");  
	system("rm -f $logfile");    
	
	print("rm -f $datafile\n");
	system("rm -f $datafile");
	
	print("bsub -q $queue -o $logfile -C 0 -J ToyData${i} root -b Load.C  $macrofile\n");
	system("bsub -q $queue -o $logfile -C 0 -J ToyData${i} root -b Load.C $macrofile");
   }
}

#check creation of data success
if($action==2){
    $Nsuccess=0;
    for($i=$firstjob;$i<=$lastjob;$i++){
	$logfile="$modesubdir/CreateData${i}.log";

	$successtring=`grep Successfully $logfile`;
	@successwords=split(" ",$successtring);
	#if($successtring eq "Successfully completed."){
	if($successwords[0] eq "Successfully"){
	    $Nsuccess++;
	}else{
	    print("Bad $logfile\n");    
	}
       		
    }
    $Nfiles=$lastjob-$firstjob+1;
    print("Total success=$Nsuccess/$Nfiles\n");
}


#Create Fit macros
if($action==3){
    $ncreated=0;
    print("rm -f logfile\n");
    print("rm -f psfile\n");
    print("rm -f macrofile\n");
    print("rm -f rootfile.root\n");
    for($i=$firstjob;$i<=$lastjob;$i++){
	$logfile="$modesubdir/FitToy${i}.log";
	$macrofile="$modesubdir/batchToyFit${i}.C";    
	$psfile="$modesubdir/DsMassRooFitvsnXRvsnXTSimul${i}.ps";
	$rootfile="$modesubdir/FitResult${i}.root";

#	print("rm -f $logfile\n");  
	system("rm -f $logfile");    

#	print("rm -f ${datafile}.root\n");
	system("rm -f $macrofile");

#       print("rm -f ${datafile}.root\n");	
	system("rm -f $psfile");

#       print("rm -f ${datafile}.root\n");	
	system("rm -f $rootfile");
	
	
	open OUTFILE, "> $macrofile" or die "@_";
	print OUTFILE "{\n";
	print OUTFILE "gROOT->ProcessLine(\".L Macros/FitToyMC.C+\");\n";
	print OUTFILE "gROOT->ProcessLine(\"FitToyMC(${i})\");\n";

	print OUTFILE "}\n";
	close OUTFILE;

	$ncreated++;
    }
           
    print "Created $ncreated files\n";                                                                         
}


#fit data sets
if($action==4){
    $queue=shift;
    
    for($i=$firstjob;$i<=$lastjob;$i++){
	$logfile="$modesubdir/ToyFit${i}.log";
	$macrofile="$modesubdir/batchToyFit${i}.C"; 
	$psfile="$modesubdir/DsMassRooFitvsnXRvsnXTSimul${i}.ps";
	$rootfile="$modesubdir/FitResult${i}.root";

#	print("rm -f $logfile\n");  
	system("rm -f $logfile");    
#       print("rm -f $psfile\n");	
	system("rm -f $psfile");
#       print("rm -f $rootfile\n");	
	system("rm -f $rootfile");
	
	print("bsub -q $queue -o $logfile -C 0 -J FitToy${i} root -b Load.C  $macrofile\n");
	system("bsub -q $queue -o $logfile -C 0 -J FiToy${i} root -b Load.C $macrofile");
   }
}

#check of fit success
if($action==5){
    $Nsuccess=0;
    for($i=$firstjob;$i<=$lastjob;$i++){
	$logfile="$modesubdir/ToyFit${i}.log";
	$psfile="$modesubdir/DsMassRooFitvsnXRvsnXTSimul${i}.ps";
	system("rm -f $psfile");

	$successtring=`grep Successfully $logfile`;
	@successwords=split(" ",$successtring);
	#if($successtring eq "Successfully completed."){
	if($successwords[0] eq "Successfully"){
	    $Nsuccess++;
	}else{
	    print("Bad $logfile\n");    
	}
       		
    }
    $Nfiles=$lastjob-$firstjob+1;
    print("Total success=$Nsuccess/$Nfiles\n");
}
