#!/usr/bin/perl
# -w

$sp=shift;
$r=shift;

$fileid=0;
$grandtotal=0;
$totalgoodfiles=0;
$nlogs=`ls  /u/br/aidanrc/ana50/AWG90/aidanrc/log/R24/Run${r}On/SP${sp} | grep DsToEllNu | grep .log | grep -v .log# |  grep -v .log~ |  wc -l`;
while($fileid<$nlogs){
    $nevents=0;
    $fileid++;#must be placed here because of next
    open(FILE,"< /u/br/aidanrc/ana50/AWG90/aidanrc/log/R24/Run${r}On/SP${sp}/DsToEllNu-SP${sp}-Run${r}On-${fileid}.log") or next;
    while($buffer=<FILE>){
	#print "$buffer";
	chomp $buffer;
	@words=split(" ",$buffer);
	if($words[0] eq "|"  and $words[1] eq "final" and $words[2] eq "lists" and $words[3] eq "|" and $nevents==0){ 
	    #|           final lists |          361 |
	    if($words[4]<1000){$nevents=$words[4];}		
	}	  	    	    	 
    }
    close FILE;	       		
    $grandtotal+=$nevents;
    $totalgoodfiles++;
    #print "$sp $r On $fileid $nevents\n";
}
print "SP${sp} Run${r} OnPeak     Grand Total= $grandtotal   total good files= $totalgoodfiles\n ";



$fileid=0;
$grandtotal=0;
$totalgoodfiles=0;
$nlogs=`ls  /u/br/aidanrc/ana50/AWG90/aidanrc/log/R24/Run${r}Off/SP${sp} | grep DsToEllNu | grep .log | grep -v .log# |  grep -v .log~ |  wc -l`;
while($fileid<$nlogs){
    $nevents=0;
    $fileid++;#must be here because of next
    open(FILE,"< /u/br/aidanrc/ana50/AWG90/aidanrc/log/R24/Run${r}Off/SP${sp}/DsToEllNu-SP${sp}-Run${r}Off-${fileid}.log") or next;
    while($buffer=<FILE>){
	#print "$buffer";
	chomp $buffer;
	@words=split(" ",$buffer);	 
	if($words[0] eq "|"  and $words[1] eq "final" and $words[2] eq "lists" and $words[3] eq "|" and $nevents==0){ 
	    #|           final lists |          361 |
	    if($words[4]<1000){$nevents=$words[4];}		
	}	  	    	    	    
    }
    close FILE;	       		
    $grandtotal+=$nevents;
    $totalgoodfiles++;
    #print "$sp $r Off $fileid $nevents\n";    
}
print "SP${sp} Run$r OffPeak     Grand Total= $grandtotal   total good files= $totalgoodfiles\n ";


