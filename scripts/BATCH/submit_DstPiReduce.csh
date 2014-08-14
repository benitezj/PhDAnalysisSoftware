#! /bin/csh -f
# Note: be careful with spaces, and note array indexing from 1, nested if statements dont  work, do not create a variable called path because it will replace PATH
#
echo $1 $2 $3 $4 $5 $6

#$5=1 all, 2 data Kpi, 3 data K3pi,4 Signal Kpi, 5 Signal K3pi,6  Generic Kpi,7 Generic K3pi
set dir = ( "DstarCharged/DstarPi" "DstarCharged/DstarPi0" "Dstar0/Dstar0Pi" "Dstar0/Dstar0Pi0" )

set i = 1;
foreach dodata ($1 $2 $3 $4)  

  if ($dodata == 1)then

    echo "Will Submit $dir[$i]"
    
    switch ($5)
    case 1: 
     set datadir="$dir[$i]/Kpi";breaksw;
    case 2: 
     set datadir="$dir[$i]/K3pi";breaksw;
    case 3: 
     set datadir="$dir[$i]/MC/SP-1005/Kpi";breaksw;
    case 4: 
     set datadir="$dir[$i]/MC/SP-1005/K3pi";breaksw;
    case 5: 
     set datadir="$dir[$i]/MC/D2420/Kpi";breaksw;
    case 6: 
     set datadir="$dir[$i]/MC/D2420/K3pi";breaksw;
    case 7: 
     set datadir="$dir[$i]/MC/D2460/Kpi";breaksw;
    case 8: 
     set datadir="$dir[$i]/MC/D2460/K3pi";breaksw;
    default: 
     set datadir=""
    endsw

    echo $datadir 
    if ($6 == 1) source submit_NtupleReduce.csh $datadir     


  endif

  @ i++
end


exit 1

