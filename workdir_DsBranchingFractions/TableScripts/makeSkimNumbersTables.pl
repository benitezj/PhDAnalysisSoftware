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
$SigName[3046]="\$\\mu^{+}\\nu\$";
$SigName[3047]="\$\\mu^{-}\\bar{\\nu}\$";
$SigName[9756]="\$K^{0}K^{\\pm}\$";
$SigName[9757]="\$\\eta\\pi^{\\pm}\$";
$SigName[9758]="\$\\tau^{\\pm}_{\\pi^{\\pm}\\bar{\\nu}}\\nu\$";
$SigName[9775]="\$K^{+}K^{-}\\pi^{+}\$"; 
$SigName[9776]="\$K^{+}K^{-}\\pi^{-}\$"; 
$SigName[9777]="\$e^{+}\\nu\$"; 
$SigName[9778]="\$e^{-}\\nu\$"; 


$crossect[1005]=1.3*1000000.;##From BaBar Physics Book, scale down to fb^-1
$crossect[1235]=(1.05/2.)*1000000.;
$crossect[1237]=(1.05/2.)*1000000.;
$crossect[3429]=(.94)*1000000.;
$crossect[998]=(.35+1.39+.35)*1000000.;

#amount of available MC that was actually used
$procfrac[1005]=1.;
$procfrac[1235]=.166;
$procfrac[1237]=.166;
$procfrac[3429]=1.;
$procfrac[998]=.5;
#@procfrac[1005]={0,1.,1.,1.,1.,1.,1.,1.,1.};
#@procfrac[1235]={0,1./6.,1./6.,1./6.,1./6.,1./6.,1./6.,1./6.,1./6.};
#@procfrac[1237]={0,1./6.,1./6.,1./6.,1./6.,1./6.,1./6.,1./6.,1./6.};
#@procfrac[3429]={0,1.,1.,1.,1.,1.,1.,1.,1.};
#@procfrac[998]={0,1./2.,1./2.,1./2.,1./2.,1./2.,1./2.,1./2.,1./2.};
#@procfrac[1005]=split(",","0,1.,1.,1.,1.,1.,1.,1.,1.");
#@procfrac[1235]=split(",","0,.1666,.1666,.1666,1.,.1666,.1666,.1666,.1666");
#@procfrac[1237]=split(",","0,.1666,.1666,.1666,1.,.1666,.1666,.1666,.1666");
#@procfrac[3429]=split(",","0,1.,1.,1.,1.,1.,1.,1.,1.");
#@procfrac[998]=split(",","0,.5,.5,.5,.5,.5,.5,.5,.5");


if($action eq "SigSP"){
    print "Working on Signal SP modes.\n";
#lets do the signal modes  
foreach $signalMode ( 3046, 3047, 9756, 9757, 9758, 9775, 9776, 9777, 9778  ) { 
  $total=0;  
  print "\\hspace{1mm} $SigName[$signalMode] On & ${signalMode}";
  foreach $run (1, 2, 3, 4, 5, 6){
    @word=split(" ",`BbkDatasetTcl --dbname bbkr24 --dataset SP-${signalMode}-Run${run}-R24 | grep events`);
    @nevt=split("/",$word[3]);
    $nevtfrac=$nevt[0]/1000000;
    $total+=$nevtfrac;
    print " & ${nevtfrac}";
  }
  foreach $run (Y2S, Y3S){
    @word=split(" ",`BbkDatasetTcl --dbname bbkr24 --dataset SP-${signalMode}-Run7-${run}_OnPeak-R24 | grep events`);
    @nevt=split("/",$word[3]);
    $nevtfrac=$nevt[0]/1000000;
    $total+=$nevtfrac;
    print " & ${nevtfrac}";
  }
  print " &  $total \\\\ \n";
  
  $total=0;  
  print "\\hspace{1mm} $SigName[$signalMode] Off & ${signalMode}";
  foreach $run (1, 2, 3, 4, 5, 6){
    @word=split(" ",`BbkDatasetTcl --dbname bbkr24 --dataset SP-${signalMode}-Run${run}-OffPeak-R24 | grep events`);
    @nevt=split("/",$word[3]);
    $nevtfrac=$nevt[0]/1000000;
    $total+=$nevtfrac;
    print " & ${nevtfrac}";
  }
  foreach $run (Y2S, Y3S){
    @word=split(" ",`BbkDatasetTcl --dbname bbkr24 --dataset SP-${signalMode}-Run7-${run}_OffPeak-R24 | grep events`);
    @nevt=split("/",$word[3]);
    $nevtfrac=$nevt[0]/1000000;
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
 print "Lumi.\$(fb^{-1})\$ On  & ";
 $total=0;
 foreach $run (1, 2, 3, 4, 5, 6){
    @word=split(" ",`BbkLumi --dbname bbkr24 --dataset AllEvents-Run${run}-R24 | grep "Lumi   Processed"`);
    $nevtfrac=$word[2]/1000.;
    $total+=$nevtfrac;
    printf(" & %.1f",$nevtfrac);
 }
 foreach $run (Y2S, Y3S){
    @word=split(" ",`BbkLumi --dbname bbkr24 --dataset AllEvents-Run7-${run}_OnPeak-R24 | grep "Lumi   Processed"`);
    $nevtfrac=$word[2]/1000.;
    $total+=$nevtfrac;
    printf(" & %.1f",$nevtfrac);
 }
 printf(" & %.1f  \\\\ \n",$total);

 #Now do OffPeak
 print "Lumi.\$(fb^{-1})\$ Off & ";
 $total=0;
 foreach $run (1, 2, 3, 4, 5, 6){
    @word=split(" ",`BbkLumi --dbname bbkr24 --dataset AllEvents-Run${run}-OffPeak-R24 | grep "Lumi   Processed"`);
    $nevtfrac=$word[3]/1000.;
    $total+=$nevtfrac;
    printf(" & %.1f",$nevtfrac);
 }
 foreach $run (Y2S, Y3S){
    @word=split(" ",`BbkLumi --dbname bbkr24 --dataset AllEvents-Run7-${run}_OffPeak-R24 | grep "Lumi   Processed"`);
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
 open FILE, "< /u/eb/benitezj/BAD/note2170/DataSamples.tex";	
 while($buffer=<FILE>){
     #print $buffer;
     chomp $buffer;
     @word=split(" ",$buffer);

     ##first get the Data lumi's On
     if($word[0] eq "%Lumi" and $word[2] eq "On"){
	 print "Data On";
	 foreach $run (1, 2, 3, 4, 5, 6, 7, 8, 9){
	     $DataLumiOn[$run]=$word[3+2*$run];
	     printf(" & %.2f",$DataLumiOn[$run]);
	 }
	 print " \\\\ \n";
     }
	 
     #print out the generic Lumi's
     foreach $signalMode ( 1005, 1235, 1237, 3429, 998 ) { 
	 if($word[1] eq "On" and $word[3] eq "$signalMode"){
	     print "$signalMode On";
	     foreach $run (1, 2, 3, 4, 5, 6, 7, 8, 9){
		 $GenericLumiOn[$signalMode][$run]=($word[3+2*$run] * 1000000.) / $crossect[$signalMode] ;# scale up because numbers are in millions
		 printf(" & %.2f",$GenericLumiOn[$signalMode][$run]);
	     }
	     print " \\\\ \n";
	 }
     }
     

     ##first get the Data lumi's Off
     if($word[0] eq "%Lumi" and $word[2] eq "Off"){
	 print "Data Off";
	 foreach $run (1, 2, 3, 4, 5, 6, 7, 8, 9){
	     $DataLumiOff[$run]=$word[3+2*$run];
	     printf(" & %.2f",$DataLumiOff[$run]);
	 }
	 print " \\\\ \n";
     }
	 
     #print out the generic Lumi's
     foreach $signalMode ( 1005, 1235, 1237, 3429, 998 ) { 
	 if($word[1] eq "Off" and $word[3] eq "$signalMode"){
	     print "$signalMode Off";
	     foreach $run (1, 2, 3, 4, 5, 6, 7, 8, 9){
		 $GenericLumiOff[$signalMode][$run]=($word[3+2*$run] * 1000000.) / $crossect[$signalMode] ;# scale up because numbers are in millions
		 printf(" & %.2f",$GenericLumiOff[$signalMode][$run]);
	     }
	     print " \\\\ \n";
	 }
     }
     

 }

 ##Print the Lumi scale factors
 print " \n"; 
 foreach $signalMode ( 1005, 1235, 1237, 3429, 998 ) { 
     $totmclumiOn=0;
     print "$signalMode On";
     foreach $run (1, 2, 3, 4, 5, 6, 7, 8){
	 $GenericScaleOn[$signalMode][$run] = $GenericLumiOn[$signalMode][$run] / $DataLumiOn[$run] ;
	 $fr=$procfrac[$signalMode];
	 if($run==4 && ($signalMode==1235 || $signalMode==1237)){$fr=1.;}
	 printf(" & %.2f",$GenericScaleOn[$signalMode][$run]*$fr);
	 $totmclumiOn+=$GenericLumiOn[$signalMode][$run]*$fr;
     }
     $totmclumiOn=$totmclumiOn/$DataLumiOn[9];
     printf(" & %.2f \\\\ \n",$totmclumiOn);

     $totmclumiOff=0;
     print "$signalMode Off";
     foreach $run (1, 2, 3, 4, 5, 6, 7, 8){
	 $GenericScaleOff[$signalMode][$run] = $GenericLumiOff[$signalMode][$run] / $DataLumiOff[$run] ;
	 $fr=$procfrac[$signalMode];
	 if($run==4 && ($signalMode==1235 || $signalMode==1237)){$fr=1.;}
	 printf(" & %.2f",$GenericScaleOff[$signalMode][$run]*$fr);
	 $totmclumiOff+=$GenericLumiOff[$signalMode][$run]*$fr;
     }
     $totmclumiOff=$totmclumiOff/$DataLumiOff[9];
     printf(" & %.2f \\\\ \n",$totmclumiOff);
 }

 print " \n";
}





###############################################################################################################
if($action eq "fDsSkim"){
  print "Working on fDsSkim events.\n";

###Data
  $total=0;  
  print "Data On";
  foreach $run ("1-", "2-", "3-", "4-", "5-", "6-", "7-Y2S_", "7-Y3S_"){
      $dir="~gjackson/AWG90/24.3.6a_skimming/workdir/ntuples/skim-AllEvents-Run${run}OnPeak-R24";
      $nevtfrac=0;
      foreach $id (`/bin/ls ${dir}`){
       #if($nevtfrac>.1){last;}
       chomp($id);
       $file=`/bin/ls ${dir}/${id} | grep skim | grep log`;
       chomp($file);
       @filew=split(/\./,$file);
       if($filew[2] eq "gz"){
	   @word=split(" ",`gzcat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`);
       }else {
	   @word=split(" ",`cat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`); 
       }

       if($word[3]<10 || $word[3]> 100000){
	   print "ERROR number of events is wrong file=${dir}/${id}/${file}\n";}
       $nevtfrac+=$word[3]/1000000.;    
  
      }    
      printf(" & %.3f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.3f \\\\ \n",$total);

  $total=0;  
  print "Data Off";
  foreach $run ("1-", "2-", "3-", "4-", "5-", "6-", "7-Y2S_", "7-Y3S_"){
      $dir="~gjackson/AWG90/24.3.6a_skimming/workdir/ntuples/skim-AllEvents-Run${run}OffPeak-R24";
      $nevtfrac=0;
      foreach $id (`/bin/ls ${dir}`){
       chomp($id);
       $file=`/bin/ls ${dir}/${id} | grep skim | grep log`;
       chomp($file);
       @filew=split(/\./,$file);
       if($filew[2] eq "gz"){
	   @word=split(" ",`gzcat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`);
       }else {
	   @word=split(" ",`cat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`); 
       }

       if($word[3]<10 || $word[3]> 100000){
	   print "ERROR number of events is wrong file=${dir}/${id}/${file}\n";}
       $nevtfrac+=$word[3]/1000000.;    
  
      }    
      printf(" & %.3f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.3f \\\\ \n",$total);


###SP
foreach $signalMode ("SP-1005", "SP-1235", "SP-1237", "SP-3429", "SP-998") { 
  $total=0;  
  print "${signalMode} On";
  foreach $run ("1-", "2-", "3-", "4-", "5-", "6-", "7-Y2S_", "7-Y3S_"){
      $dir="~gjackson/AWG90/24.3.6a_skimming/workdir/ntuples/skim-${signalMode}-Run${run}R24";
      $nevtfrac=0;
      foreach $id (`/bin/ls ${dir}`){
       chomp($id);
       $file=`/bin/ls ${dir}/${id} | grep skim | grep log`;
       chomp($file);
       @filew=split(/\./,$file);
       if($filew[2] eq "gz"){
	   @word=split(" ",`gzcat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`);
       }else {
	   @word=split(" ",`cat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`); 
       }

       if($word[3]<10 || $word[3]> 100000){
	   print "ERROR number of events is wrong file=${dir}/${id}/${file}\n";}
       $nevtfrac+=$word[3]/1000000.;    
  
      }    
      printf(" & %.3f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.3f \\\\ \n",$total);

  $total=0;  
  print "${signalMode} Off";
  foreach $run ("1-", "2-", "3-", "4-", "5-", "6-", "7-Y2S_", "7-Y3S_"){
      $dir="~gjackson/AWG90/24.3.6a_skimming/workdir/ntuples/skim-${signalMode}-Run${run}OffPeak-R24";
      $nevtfrac=0;
      foreach $id (`/bin/ls ${dir}`){
       chomp($id);
       $file=`/bin/ls ${dir}/${id} | grep skim | grep log`;
       chomp($file);
       @filew=split(/\./,$file);
       if($filew[2] eq "gz"){
	   @word=split(" ",`gzcat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`);
       }else {
	   @word=split(" ",`cat ${dir}/${id}/${file} | grep "\|fDsPathCounter"`); 
       }

       if($word[3]<10 || $word[3]> 100000){
	   print "ERROR number of events is wrong file=${dir}/${id}/${file}\n";}
       $nevtfrac+=$word[3]/1000000.;    
  
      }    
      printf(" & %.3f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.3f \\\\ \n",$total);

}
}




###############################################################################################################
if($action eq "CleanTag"){
    $inout=shift;
    print "Working on CleanTag events.\n";
#Data, SP1005, SP1235, SP1237, SP3429, SP998, SP3046, SP3047, SP9756, SP9757, SP9758, SP9775, SP9776, SP9777, SP9778 
foreach $signalMode ( SP3046, SP3047, SP9756, SP9757, SP9758, SP9775, SP9776, SP9777, SP9778 ) { 
  $total=0;  
  print "${signalMode} On";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
      $nevtfrac=0;
      foreach $file (`/bin/ls CleanTagSkim/${signalMode}/Run${run}On | grep Snippet | grep log`){
       chomp($file);
       $waszipped=0;
       @filew=split(/\./,$file);
       if($filew[2] eq "gz"){`gunzip -q CleanTagSkim/${signalMode}/Run${run}On/${file}`;$waszipped=1;}
       
       if($inout eq "input"){
	 @word=split(" ",`tail -n 200 CleanTagSkim/${signalMode}/Run${run}On/${filew[0]}.log | grep "|EvtCounter"`);
       }
       if($inout eq "output"){
         @word=split(" ",`tail -n 2750 CleanTagSkim/${signalMode}/Run${run}On/${filew[0]}.log | head -n 100 | grep "\|MyDRecoCleanTagSkimPathCounter"`);
        #@word=split(" ","AppAST:     0.00 |    3399 |      0.06 |        0.01765 |   0.00 |MyDRecoCleanTagSkimPathCounter");        
       }
       if($word[3]<10 || $word[3]> 1000000){
	   print "ERROR number of events is wrong file=CleanTagSkim/${signalMode}/Run${run}On/${filew[0]}\n";
       }
       $nevtfrac+=$word[3]/1000000.;  
       
       #if($waszipped==1){`gzip -q CleanTagSkim/${signalMode}/Run${run}On/${filew[0]}.log`;}
      }    
      printf(" & %.4f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.4f \\\\ \n",$total);

  $total=0;  
  print "${signalMode} Off";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
      $nevtfrac=0;
      foreach $file (`/bin/ls CleanTagSkim/${signalMode}/Run${run}Off | grep Snippet | grep log`){
       chomp($file);
       $waszipped=0;
       @filew=split(/\./,$file);
       if($filew[2] eq "gz"){`gunzip -q CleanTagSkim/${signalMode}/Run${run}Off/${file}`;$waszipped=1;}
  
       if($inout eq "input"){
	 @word=split(" ",`tail -n 200 CleanTagSkim/${signalMode}/Run${run}Off/${filew[0]}.log | grep "|EvtCounter"`);
       }
       if($inout eq "output"){
         @word=split(" ",`tail -n 2750 CleanTagSkim/${signalMode}/Run${run}Off/${filew[0]}.log | head -n 100 | grep "\|MyDRecoCleanTagSkimPathCounter"`); 
       }
       if($word[3]<10 || $word[3]> 1000000){
	   print "ERROR number of events is wrong file=CleanTagSkim/${signalMode}/Run${run}Off/${filew[0]}\n";}
       $nevtfrac+=$word[3]/1000000.; 
       #if($waszipped==1){`gzip -q CleanTagSkim/${signalMode}/Run${run}Off/${filew[0]}.log`;}
      }    
      printf(" & %.4f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.4f \\\\ \n",$total);
}
}


####Input Signal into CleanTag
if($action eq "SigSPSkimInput"){
    print "Working on Signal SP modes.\n";
#lets do the signal modes    
foreach $signalMode (3046, 3047, 9756, 9757, 9758, 9775, 9776, 9777, 9778) { 
  $total=0;  
  print "\\hspace{1mm} $SigName[$signalMode] On & ${signalMode}";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
    opendir(DIR,"CleanTagSkim/SP${signalMode}/Run${run}On") or die "can't opendir  $!";
    @tclfiles = grep{/log/} grep { /Snippet/ } readdir(DIR);
    closedir(DIR);
    $nevtfrac=0;
    foreach $logfile(@tclfiles){
      @word=split(" ",`cat CleanTagSkim/SP${signalMode}/Run${run}On/${logfile} | grep \\|MyDRecoCleanTagSkimPathTagBitReset`);
      $nevtfrac+=$word[3]/1000000;
      $total+=$word[3]/1000000;
    }    
    printf(" & %.3f ",$nevtfrac);
  }
  printf(" & %.3f \\\\ \n",$total);

  $total=0;  
  print "\\hspace{1mm} $SigName[$signalMode] Off & ${signalMode}";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
    opendir(DIR,"CleanTagSkim/SP${signalMode}/Run${run}Off") or die "can't opendir  $!";
    @tclfiles = grep{/log/} grep { /Snippet/ } readdir(DIR);
    closedir(DIR);
    $nevtfrac=0;
    foreach $logfile(@tclfiles){
      @word=split(" ",`cat CleanTagSkim/SP${signalMode}/Run${run}Off/${logfile} | grep \\|MyDRecoCleanTagSkimPathTagBitReset`);
      $nevtfrac+=$word[3]/1000000;
      $total+=$word[3]/1000000;
    }    
    printf(" & %.3f ",$nevtfrac);
  }
  printf(" & %.3f \\\\ \n",$total);
  

}
}



###############################################################################################################
if($action eq "NtupleInput"){
  $dir=shift;
  print "Working on NtupleInput events ${dir}.\n";
  $total=0;  
  print "$dir On";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
      $nevtfrac=0;
      foreach $file (`/bin/ls ${dir}/Run${run}On | grep Ntuple | grep log`){
       #print $file;
       chomp($file);
       @word=split(" ",`tail -n 100 $dir/Run${run}On/$file | grep "EvtCounter:EvtCounter:     total number of"`);
       #EvtCounter:EvtCounter:     total number of events=37636;        
       @evts=split("=",$word[4]);
       $nevtfrac+=$evts[1]/1000000.;  
      }    
      printf(" & %.4f",$nevtfrac);
      $total+=$nevtfrac;
  }         
  printf(" & %.4f \\\\ \n",$total);

 $total=0;  
 print "$dir Off";
 foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
     $nevtfrac=0;
     foreach $file (`/bin/ls ${dir}/Run${run}Off | grep Ntuple | grep log`){
      #print $file;
      chomp($file);
      @word=split(" ",`tail -n 100 $dir/Run${run}Off/$file | grep "EvtCounter:EvtCounter:     total number of"`);
      #EvtCounter:EvtCounter:     total number of events=37636;        
      @evts=split("=",$word[4]);
      $nevtfrac+=$evts[1]/1000000.;  
     }    
     printf(" & %.4f",$nevtfrac);
     $total+=$nevtfrac;
 }     
 printf(" & %.4f \\\\ \n",$total);

}

###############################################################################################################
if($action eq "NtupleOutput"){
  ##calculates number of reconstructions passed 
  $dir=shift;
  print "Working on NtupleOutput candidates ${dir}.\n";
  $total=0;  
  print "$dir On";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
      $nevtfrac=0;
      foreach $file (`/bin/ls ${dir}/Run${run}On | grep Ntuple | grep log`){
	  #print $file;
	  chomp($file);	  
	  @word=split(" ",`tail -n 300 $dir/Run${run}On/$file | grep "final candidates"`);
	  #|       final candidates |         4490 |
	  $nevtfrac+=$word[4];  
       }    
      printf " & $nevtfrac";
      $total+=$nevtfrac;
  }         
  printf(" & %.0f \\\\ \n",$total);

 $total=0;  
 print "$dir Off";
 foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
     $nevtfrac=0;
     foreach $file (`/bin/ls ${dir}/Run${run}Off | grep Ntuple | grep log`){
	 #print $file;
	 chomp($file);
	 @word=split(" ",`tail -n 300 $dir/Run${run}Off/$file | grep "final candidates"`);
	 #|       final candidates |         4490 |
	 $nevtfrac+=$word[4];  
     }    
     printf " & $nevtfrac";
     $total+=$nevtfrac;
 }     
 printf(" & %.0f \\\\ \n",$total);

}

if($action eq "NtupleOutputKKpi"){
  ##calculates number of reconstructions passed
  $dir=shift;
  print "Working on NtupleOutput candidates ${dir}.\n";
  $total=0;  
  print "$dir On";
  foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
      $nevtfrac=0;
      foreach $file (`/bin/ls ${dir}/Run${run}On | grep Ntuple | grep log`){
	  #print $file;
	  chomp($file);
	  @word=split(" ",`tail -n 300 $dir/Run${run}On/$file | grep "final Ups"`);
	  #|       final Ups |         4490 |
	  $nevtfrac+=$word[4]; 
       }    
      printf " & $nevtfrac";
      $total+=$nevtfrac;
  }         
  printf(" & %.0f \\\\ \n",$total);

 $total=0;  
 print "$dir Off";
 foreach $run (1, 2, 3, 4, 5, 6, Y2S, Y3S){
     $nevtfrac=0;
     foreach $file (`/bin/ls ${dir}/Run${run}Off | grep Ntuple | grep log`){
	 #print $file;
	 chomp($file);
	 @word=split(" ",`tail -n 300 $dir/Run${run}Off/$file | grep "final Ups"`);
	 #|       final Ups |         4490 |
	 $nevtfrac+=$word[4]; 
     }    
     printf " & $nevtfrac";
     $total+=$nevtfrac;
 }     
 printf(" & %.0f \\\\ \n",$total);

}
