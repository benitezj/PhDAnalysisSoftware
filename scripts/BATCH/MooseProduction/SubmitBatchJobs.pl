#!/usr/bin/perl
# -w


$firstjob=shift;
$lastjob=shift;
$queue=shift;
$modenum=shift;

#D0ToKPi Plus10001 Minus10002
#D0ToKPiPi0 Plus10003 Minus10004
#D0ToK3Pi Plus10005 Minus10006

$maindir="Dst2640MooseProduction";

$outdir="${maindir}/SP-${modenum}";

foreach $fileid ($firstjob .. $lastjob){
   
    $tclfile="SP-${modenum}-Collection${fileid}";

    #need unset this variable other wise jobs crash
    #unsetenv CFG_DEFAULT_IMPL 

    system("bsub -q ${queue} -o ${outdir}/${tclfile}.log -J ${fileid} MooseApp  ${outdir}/${tclfile}.tcl  &");
        
}

exit;
#############




