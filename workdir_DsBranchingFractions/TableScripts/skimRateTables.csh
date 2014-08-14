#!/bin/tcsh

set dirname="~gjackson/AWG90/24.3.6a_skimming/workdir"
#get task name 
foreach run (1- 2- 3- 4- 5- 6- 7-Y2S_ 7-Y3S_)
  foreach onoff (On Off)
        set task=skim-AllEvents-Run${run}${onoff}Peak-R24	
        rm -f $task.table
	touch $task.table
	echo ${task}.table has been created.

	set counter=0
	#get a list of log files
	foreach logFileNumber ( `/bin/ls ${dirname}/ntuples/${task}/` )
	    
	    set filename="${task}-${logFileNumber}.log"    
	    if ( -r ${dirname}/ntuples/${task}/${logFileNumber}/${task}-${logFileNumber}.log.gz ) then 
                set filename="${task}-${logFileNumber}.log.gz"
	    endif

	    #get the number of input events
	    set nInputEvents=`gunzip -c ${dirname}/ntuples/${task}/${logFileNumber}/${filename} | grep "EvtCounter:EvtCounter:     total number of events=" | awk -F'=' '{print $NF}'`

	    #get the number of output events 
	    set nOutputEvents=`gunzip -c ${dirname}/ntuples/${task}/${logFileNumber}/${filename} | grep "Copied/Merged/Read" | awk '{print $2}'`

            @ counter = $counter + 1

	    echo $logFileNumber $nInputEvents $nOutputEvents  >>  $task.table
	end

	echo Number of jobs=$counter 
  end
end





