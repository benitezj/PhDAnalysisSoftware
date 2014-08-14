#!/usr/bin/perl
# -w

$firstjob=shift;
$lastjob=shift;
$queue=shift;
$modenum=shift;

$maindir="Ds1pMooseProduction";
$outdir="${maindir}/SP-${modenum}";

foreach $fileid ($firstjob .. $lastjob){
   
    $tclfile="SP-${modenum}-Collection${fileid}";

    #need unset this variable other wise jobs crash
    #if($fileid==$firstjob){system("unsetenv CFG_DEFAULT_IMPL")};

    system("bsub -q ${queue} -o ${outdir}/${tclfile}.log -J ${fileid} MooseApp  ${outdir}/${tclfile}.tcl  &");
        
}

exit;
#############




