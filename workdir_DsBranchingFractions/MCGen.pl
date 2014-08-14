#!/usr/bin/perl
# -w

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
 
$SigName[9949]="\$\\mu^{+}\\nu\\bar{\\nu}\$"; 
$SigName[9950]="\$\\mu^{-}\\nu\\bar{\\nu}\$"; 
$SigName[9967]="\$\\mu^{+}\\nu\\bar{\\nu}\$"; 
$SigName[9968]="\$\\mu^{-}\\nu\\bar{\\nu}\$"; 
$Run7Map[9949]=9967;
$Run7Map[9950]=9968;

$SigName[9951]="\$ e^{+}\\nu\\bar{\\nu}\$"; 
$SigName[9952]="\$ e^{-}\\nu\\bar{\\nu}\$"; 
$SigName[9969]="\$ e^{+}\\nu\\bar{\\nu}\$"; 
$SigName[9970]="\$ e^{-}\\nu\\bar{\\nu}\$"; 
$Run7Map[9951]=9969;
$Run7Map[9952]=9970;

$SigName[9965]="\$\\pi^{+}\\bar{\\nu}\$"; 
$SigName[9966]="\$\\pi^{-}\\nu\$"; 
$SigName[9984]="\$\\pi^{+}\\bar{\\nu}\$"; 
$SigName[9985]="\$\\pi^{-}\\nu\$"; 
$Run7Map[9965]=9984;
$Run7Map[9966]=9985;

#lets do the signal modes  
#( 3046, 3047, 9756, 9757, 9758, 9775, 9776, 9777, 9778  ) { 
#foreach $signalMode ( 9949, 9950, 9967, 9968, 9951, 9952, 9969, 9970, 9965, 9966, 9984, 9985 ) {
foreach $signalMode ( 9949, 9950, 9951, 9952, 9965, 9966, ) {
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
    @word=split(" ",`BbkDatasetTcl --dbname bbkr24 --dataset SP-${Run7Map[$signalMode]}-Run7-${run}_OnPeak-R24 | grep events`);
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
    @word=split(" ",`BbkDatasetTcl --dbname bbkr24 --dataset SP-${Run7Map[$signalMode]}-Run7-${run}_OffPeak-R24 | grep events`);
    @nevt=split("/",$word[3]);
    $nevtfrac=$nevt[0]/1000000;
    $total+=$nevtfrac;
    print " & ${nevtfrac}";
  }
  print " &  $total \\\\ \n";
}

