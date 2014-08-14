#!/usr/bin/perl
# -w
$firstjob=shift;
$lastjob=shift;
$modenum=shift;
$CheckOrDelete=shift;

$mycolldir="/work/users/benitezj";

foreach $fileid ($firstjob .. $lastjob){
    
    #####give the command some time
    foreach $x (0 .. 50){
        foreach $p (0 .. 5000){
	    $d=2*2/2+2-4/2-4+2;
	}
	print "-";
    }
    print "$fileid\n";    

    $coll="SP-${modenum}-Collection${fileid}";

    if($CheckOrDelete==1){system("KanCollUtil -L $mycolldir/${coll} ");}
    if($CheckOrDelete==2){system("KanUserAdmin delete $mycolldir/${coll} ");}
        
}

exit;
#############




