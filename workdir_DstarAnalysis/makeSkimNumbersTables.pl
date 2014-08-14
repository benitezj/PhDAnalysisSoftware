#!/usr/bin/perl
# -w

####Option taks are
#SigSP: number of events for each signal SP mode 
#DataLumi: luminosity for each Run
#CleanTag: CleanTag Skim pass events
#
$action=shift;


#Note: you must be in yakut to execute

$SigName[1005]="\$c\\bar{c}\$";
$SigName[1235]="\$B^{+}B^{-}\$";
$SigName[1237]="\$B^0\\bar{B^0}\$";
$SigName[3429]="\$\\tau\\tau\$";
$SigName[998]="\$uds\$";

$SigName[9015]="\$D_2^{*0} \\rightarrow D^+_{K\\pi\\pi}\\pi^-\$";
$SigName[9632]="\$D_2^{*+} \\rightarrow D^0_{K\\pi}\\pi^+\$";
$SigName[8366]="\$D_1^0 \\rightarrow D^{*+}\\pi^-, D^{*+}\\rightarrow D^0_{K\\pi}\\pi^+\$";
$SigName[8367]="\$D_1^0 \\rightarrow D^{*+}\\pi^-, D^{*+}\\rightarrow D^0_{K3\\pi}\\pi^+\$";
$SigName[8368]="\$D_2^{*0} \\rightarrow D^{*+}\\pi^-, D^{*+}\\rightarrow D^0_{K\\pi}\\pi^+\$";
$SigName[8375]="\$D_2^{*0} \\rightarrow D^{*+}\\pi^-, D^{*+}\\rightarrow D^0_{K3\\pi}\\pi^+\$";
$SigName[9634]="\$D_2^{*+} \\rightarrow D^{*0}\\pi^+, D^{*0}\\rightarrow D^0_{K\\pi}\\pi^0\$";
$SigName[9635]="\$D_2^{*0} \\rightarrow D^{*+}\\pi^-, D^{*+}\\rightarrow D^+_{K\\pi\\pi}\\pi^+\$";
$SigList="9015 9632 8366 8367 8368 8375 9634 9635";

$crossect[1005]=1.3*1000000.;##From BaBar Physics Book, scale down to fb^-1
$crossect[1235]=(1.05/2.)*1000000.;
$crossect[1237]=(1.05/2.)*1000000.;
$crossect[3429]=(.94)*1000000.;
$crossect[998]=(.35+1.39+.35)*1000000.;
 

###############################################################################################################
if($action eq "DataEvents"){
 print "Working on Number of Data Events\n";

 #OnPeak Lumi
 print "Data On  & ";
 $total=0;
 foreach $run (1, 2, 3, 4, 5, 6){#
     $nevtfrac=0;
     foreach $file(`ls SKIMS/DCharged/DATARun${run}On | grep Snippet | grep log`){
	 chomp($file);
	 $line=`cat SKIMS/DCharged/DATARun${run}On/${file} | grep "EvtCounter:EvtCounter:     total number of"`;
	 @word=split(" ",$line);
	 @evtword=split("=",$word[5]);
	 $nevtfrac+=$evtword[1]/1000000.; 
     }
     $total+=$nevtfrac;
     printf(" & %.1f",$nevtfrac);
 }
 printf(" & %.1f  \\\\ \n",$total);

  #Now do OffPeak
  print "Data Off & ";
  $total=0;
  foreach $run (1, 2, 3, 4, 5, 6){
     $nevtfrac=0;
     foreach $file(`ls SKIMS/DCharged/DATARun${run}Off | grep Snippet | grep log`){
	 chomp($file);
	 $line=`cat SKIMS/DCharged/DATARun${run}Off/${file} | grep "EvtCounter:EvtCounter:     total number of"`;
	 @word=split(" ",$line);
	 @evtword=split("=",$word[5]);
	 $nevtfrac+=$evtword[1]/1000000.; 
     }
     $total+=$nevtfrac;
     printf(" & %.1f",$nevtfrac);
  }
  printf(" & %.1f \\\\ \n",$total);

}


###############################################################################################################
if($action eq "GenericEvents"){
 print "Working on Number of Generic Events\n";

 #OnPeak 
 print "\$c\\bar{c}\$ On  ";
 $total=0;
 foreach $run (1, 2, 3, 4, 5, 6){
     $line=`BbkDatasetTcl SP-1005-AllEventsSkim-Run${run}-R22d-v06 | grep Selected`;
     @word=split(" ",$line);
     @evtword=split("/",$word[3]);
     $nevtfrac=$evtword[1]/1000000.;
     $total+=$nevtfrac;
     printf(" & %.1f",$nevtfrac);
 }
 printf(" & %.1f  \\\\ \n",$total);

  #Now do OffPeak
  print "\$c\\bar{c}\$ Off ";
  $total=0;
  foreach $run (1, 2, 3, 4, 5, 6){
     $line=`BbkDatasetTcl SP-1005-AllEventsSkim-Run${run}-OffPeak-R22d-v06 | grep Selected`;
     @word=split(" ",$line);
     @evtword=split("/",$word[3]);
     $nevtfrac=$evtword[1]/1000000.;
     $total+=$nevtfrac;
     printf(" & %.1f",$nevtfrac);
  }
  printf(" & %.1f \\\\ \n",$total);

}

###############################################################################################################
if($action eq "SigEvents"){
    print "Working on Signal SP modes.\n";

foreach $signalMode ( split(" ",$SigList) ) { 
  $total=0;  
  print "$SigName[$signalMode] On & ${signalMode}";
  foreach $run (1, 2, 3, 4, 5, 6){
      $line=`BbkDatasetTcl SP-${signalMode}-Run${run}-R22 | grep Selected`;
      @word=split(" ",$line);
      @nevt=split("/",$word[3]);
      $nevtfrac=$nevt[1]/1000000;
      $total+=$nevtfrac;
      print " & ${nevtfrac}";
  }
  print " &  $total \\\\ \n";

  $total=0;  
  print "$SigName[$signalMode] Off & ${signalMode}";
  foreach $run (1, 2, 3, 4, 5, 6){
      $line=`BbkDatasetTcl SP-${signalMode}-Run${run}-OffPeak-R22 | grep Selected`;
      @word=split(" ",$line);
      @nevt=split("/",$word[3]);
      $nevtfrac=$nevt[1]/1000000;
      $total+=$nevtfrac;
      print " & ${nevtfrac}";
  }
  print " &  $total \\\\ \n";
  

}
}


###############################################################################################################
if($action eq "DataLumi"){
 print "Working on Data lumi.\n";

  #OnPeak Lumi
  print "Data On  ";
  $total=0;
  foreach $run (1, 2, 3, 4, 5, 6){
     @word=split(" ",`BbkLumi  --dataset AllEventsSkim-Run${run}-OnPeak-R22d-v04 | grep "Lumi   Processed"`);
     $nevtfrac=$word[2]/1000.;
     $total+=$nevtfrac;
     printf(" & %.1f",$nevtfrac);
  }
  printf(" & %.1f  \\\\ \n",$total);
 
 #Now do OffPeak
 print "Data Off ";
 $total=0;
 foreach $run (1, 2, 3, 4, 5, 6){
    @word=split(" ",`BbkLumi --dataset AllEventsSkim-Run${run}-OffPeak-R22d-v04 | grep "Lumi   Processed"`);
    $nevtfrac=$word[3]/1000.;
    $total+=$nevtfrac;
    printf(" & %.1f",$nevtfrac);
 }
 printf(" & %.1f \\\\ \n",$total);

}

###############################################################################################################
if($action eq "GenericLumi"){
 print "Working on  GenericLumi \n";

 #OnPeak Lumi
 open FILE, "< /u/eb/benitezj/BAD/note1926/introduction.tex";	
 while($buffer=<FILE>){
     #print $buffer;
     chomp $buffer;
     @word=split(" ",$buffer);

#     ##first get the Data lumi's On
#     if($word[0] eq "Data" and $word[1] eq "On"){
#	 print "Data On";
#	 foreach $run (1, 2, 3, 4, 5, 6, 7){
#	     $DataLumiOn[$run]=$word[1+2*$run];
#	     printf(" & %.2f",$DataLumiOn[$run]);
#	 }
#	 print " \\\\ \n";
#     }

     #print out the generic Lumi's
     if($word[0] eq "$SigName[1005]" and $word[1] eq "On" ){
	 print "$SigName[1005] On";
	 foreach $run (1, 2, 3, 4, 5, 6, 7){
	     $GenericLumiOn[1005][$run]=($word[1+2*$run] * 1000000.) / $crossect[1005] ;# scale up because numbers are in millions
	     printf(" & %.2f",$GenericLumiOn[1005][$run]);
	 }
	 print " \\\\ \n";

     }
     

#     ##first get the Data lumi's Off
#     if($word[0] eq "Data" and $word[1] eq "Off"){
#	 print "Data Off";
#	 foreach $run (1, 2, 3, 4, 5, 6, 7){
#	     $DataLumiOff[$run]=$word[3+2*$run];
#	     printf(" & %.2f",$DataLumiOff[$run]);
#	 }
#	 print " \\\\ \n";
#     }

     #print out the generic Lumi's
     if($word[0] eq "$SigName[1005]" and $word[1] eq "Off" ){
	 print "$SigName[1005] Off";
	 foreach $run (1, 2, 3, 4, 5, 6, 7){
	     $GenericLumiOff[1005][$run]=($word[1+2*$run] * 1000000.) / $crossect[1005] ;# scale up because numbers are in millions
	     printf(" & %.2f",$GenericLumiOff[1005][$run]);
	 }
	 print " \\\\ \n";

     }   

 }

# ##Print the Lumi scale factors
# print " \n"; 
# foreach $signalMode ( 1005 ) { #, 1235, 1237, 3429, 998
#     $totmclumiOn=0;
#     print "$signalMode On";
#     foreach $run (1, 2, 3, 4, 5, 6, 7){
#	 $GenericScaleOn[$signalMode][$run] = $GenericLumiOn[$signalMode][$run] / $DataLumiOn[$run] ;
#	 $fr=$procfrac[$signalMode];
#	 if($run==4 && ($signalMode==1235 || $signalMode==1237)){$fr=1.;}
#	 printf(" & %.2f",$GenericScaleOn[$signalMode][$run]*$fr);
#	 $totmclumiOn+=$GenericLumiOn[$signalMode][$run]*$fr;
#     }
#     $totmclumiOn=$totmclumiOn/$DataLumiOn[9];
#     printf(" & %.2f \\\\ \n",$totmclumiOn);
#
#     $totmclumiOff=0;
#     print "$signalMode Off";
#     foreach $run (1, 2, 3, 4, 5, 6, 7){
#	 $GenericScaleOff[$signalMode][$run] = $GenericLumiOff[$signalMode][$run] / $DataLumiOff[$run] ;
#	 $fr=$procfrac[$signalMode];
#	 if($run==4 && ($signalMode==1235 || $signalMode==1237)){$fr=1.;}
#	 printf(" & %.2f",$GenericScaleOff[$signalMode][$run]*$fr);
#	 $totmclumiOff+=$GenericLumiOff[$signalMode][$run]*$fr;
#     }
#     $totmclumiOff=$totmclumiOff/$DataLumiOff[7];
#     printf(" & %.2f \\\\ \n",$totmclumiOff);
# }
#
 print " \n";
}





