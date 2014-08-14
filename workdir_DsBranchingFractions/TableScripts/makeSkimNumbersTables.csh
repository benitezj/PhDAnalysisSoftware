#!/bin/tcsh
set choice=""
set choice=$1

if ( $choice == "" ) then 
    echo You need to enter a choice.
endif

#This script will make a table of input and output numbers for all the skimming tasks

if ( $choice == rawnumbers ) then 
        #prepare the output file
        set outputFile=makeSkimNumbersTable_rawnumbers.txt
	if ( ! -e ${outputFile}) then 
	    touch $outputFile
	endif
	#First of all lets sort the generics out (ie not the signal modes)
	#get the task name
	foreach task ( `/bin/ls skim*.txt | awk -F'.txt' '{print $1}' \
			| grep -v 3046 | grep -v 3047 | grep -v 9756  \
			| grep -v 9757 | grep -v 9758 | grep -v 9775  \
			| grep -v 9776 | grep -v 9777 | grep -v 9778` )
	    if ( `grep -c ${task} ${outputFile}` == 0 ) then 
	    	# 
	    	#get the number of input events from the tcl files
	    	set eventsInput=0
		set tclFileCounter=0
		set ntclFiles=`/bin/ls ${task}/tcl/ | wc -l`
	    	foreach tclFile ( `/bin/ls ${task}/tcl/ | grep tcl` )
		   @ tclFileCounter = $tclFileCounter + 1
		   if ( $tclFileCounter % 2500 == 0 ) then
		       echo `echo $tclFileCounter $ntclFiles | awk '{print $1*100/$2}'`% through tcl files for ${task} 
		   endif
	    	   @ eventsInput = $eventsInput + `grep "In this tcl file" ${task}/tcl/$tclFile | awk '{print $8}'`
	    	end
	    	#
	    	#get the number of output events
	    	set eventsOutput=0
		set logFileCounter=0
		set nlogFiles=`/bin/ls ntuples/${task}/ | wc -l`
	    	foreach logFileNumber ( `/bin/ls ntuples/${task}/` ) 
		   @ logFileCounter = $logFileCounter + 1
		   if ( $logFileCounter % 2500 == 0 ) then
		       echo `echo $logFileCounter $nlogFiles | awk '{print $1*100/$2}'`% through log files for ${task} 
		   endif
	    	   if ( -e ntuples/${task}/${logFileNumber}/${task}-${logFileNumber}.log.gz ) then 
	    		   @ eventsOutput = $eventsOutput + `gunzip -c ntuples/${task}/${logFileNumber}/${task}-${logFileNumber}.log.gz | grep "Copied/Merged/Read" | awk '{print $2}'`
	    	   else 
	    		   if ( -e ntuples/${task}/${logFileNumber}/${task}-${logFileNumber}.log ) then 
	    		       @ eventsOutput = $eventsOutput + `cat ntuples/${task}/${logFileNumber}/${task}-${logFileNumber}.log | grep "Copied/Merged/Read" | awk '{print $2}'`
	    		   endif
	    	   endif
	    	end     
	    	#
		echo $task $eventsInput $eventsOutput `echo $eventsInput $eventsOutput | awk '{printf "%.3f", $2*100/$1}'` >> $outputFile
	    else
		echo Info for ${task} is already in $outputFile
	    endif
	end
	#Now lets do the signal modes
	foreach signalMode ( 3046 3047 9756 9757 9758 9775 9776 9777 9778 )
	    #make the task names
	    foreach task ( skim-SP-${signalMode}-Run1-OffPeak-R24 \
                           skim-SP-${signalMode}-Run1-R24 \
                           skim-SP-${signalMode}-Run2-OffPeak-R24 \
                           skim-SP-${signalMode}-Run2-R24 \
                           skim-SP-${signalMode}-Run3-OffPeak-R24 \
                           skim-SP-${signalMode}-Run3-R24 \
                           skim-SP-${signalMode}-Run4-OffPeak-R24 \
                           skim-SP-${signalMode}-Run4-R24 \
                           skim-SP-${signalMode}-Run5-OffPeak-R24 \
                           skim-SP-${signalMode}-Run5-R24 \
                           skim-SP-${signalMode}-Run6-OffPeak-R24 \
                           skim-SP-${signalMode}-Run6-R24 \
                           skim-SP-${signalMode}-Run7-Y2S_OffPeak-R24 \
                           skim-SP-${signalMode}-Run7-Y2S_OnPeak-R24 \
                           skim-SP-${signalMode}-Run7-Y3S_OffPeak-R24 \
                           skim-SP-${signalMode}-Run7-Y3S_OnPeak-R24 )
			if ( `grep -c ${task} ${outputFile}` == 0 ) then 
			    #Set the output events to an effective 'N/A' value 
			    set eventsOutput=-999
			    #Now get the eventsInput from the database
			    set formattedTaskName=`echo $task | awk -F skim- '{print $2}'`
			    set eventsInput=`BbkUser --dbname bbkr24 --dataset $formattedTaskName events | grep -v EVENTS | grep -v rows | awk '{s+=$NF}END{if ( s == "" )print 0; else print s}'`
			    #echo $task $eventsInput $eventsOutput `echo $eventsInput $eventsOutput | awk '{printf "%.3f", $2*100/$1}'` >> $outputFile
			    echo $task $eventsInput $eventsOutput 1 >> $outputFile
			else
			    echo Info for ${task} is already in $outputFile	    
			endif
	     end
	end
endif



if ( $choice == rawnumberstidied ) then 
    #prepare the output file
    set outputFile=makeSkimNumbersTable_rawnumberstidied.txt
    if ( ! -e ${outputFile}) then 
	touch $outputFile
    else
	rm $outputFile
	touch $outputFile
    endif
    #
    set inputFile=makeSkimNumbersTable_rawnumbers.txt
    #
    foreach taskName ( `cat $inputFile | awk '{print $1}'` )
	#Form a name
	set name=""
	if ( `echo $taskName | awk -F'-' '{print $2}'` == SP ) then
	    set name=`echo $taskName | awk -F'-' '{print $2" "$3}'`
	endif
	if ( `echo $taskName | awk -F'-' '{print $2}'` == AllEvents ) then
	    set name=`echo $taskName | awk -F'-' '{print $2}'`
	endif
	#Get the run number
	set run=`echo $taskName | awk -F'Run' '{print $2}' | awk -F'-' '{print $1}'`
	#Get Y2S or Y3S if there is one
	set upsilon=""
	if ( $run == 7 && $upsilon == "" ) then 
	    set upsilon=`echo $taskName | grep -c Y2S | awk '{if( $1 == 1 )print "Y2S"; else print ""}'`
	endif
	if ( $run == 7 && $upsilon == "" ) then 
	    set upsilon=`echo $taskName | grep -c Y3S | awk '{if( $1 == 1 )print "Y3S"; else print ""}'`
	endif
	#
	#Get on or offpeak
	if ( `echo $taskName | grep -c OffPeak` == 1) then 
	    set peak=OffPeak
	else
	    set peak=OnPeak
	endif
	#Get the number of input events
	set inputEvents=`grep $taskName $inputFile | awk '{print $2}'`
	#Get the number of output events
	set outputEvents=`grep $taskName $inputFile | awk '{print $3}'`
	#Get the skim rate
	set rate=`grep $taskName $inputFile | awk '{print $4}'`
	#
	#And finally, only include R24 numbers
	if ( `echo $taskName | grep -c R24` == 1 ) then 
	    echo $name $run $upsilon $peak $inputEvents $outputEvents $rate >> $outputFile
	endif
    end
endif

if ( $choice == makerawtables ) then 
    echo Table 0 shows skim input numbers
    echo Table 1 shows skim output numbers
    echo Table 2 shows skim rate numbers
    echo ""
    foreach getField ( 0 1 2 )
    	#prepare the output file
    	set outputFile=makeSkimNumbersTable_makerawtable${getField}.txt
    	if ( ! -e ${outputFile}) then 
    		touch $outputFile
    	else
    		rm $outputFile
    		touch $outputFile
    	endif
    	#
    	#Get the input file
    	set inputFile=makeSkimNumbersTable_rawnumberstidied.txt
    	#Create the header
    	set header="Datatype Run1 Run2 Run3 Run4 Run5 Run6 Run7(Y2S) Run7(Y3S) Total"
	echo $header >> $outputFile
    	#Make a list of datatypes to loop over (in the order that you want)
    	foreach datatype ( AllEvents 1005 1235 1237 3429 998 3046 3047 9756 9757 9758 9775 9776 9777 9778 )
    	#foreach datatype ( AllEvents )
    		foreach peak ( On Off )
    		    set numberString=""
    		    set total=0
    		    foreach run ( 1 2 3 4 5 6 )
    			if ( $datatype == AllEvents  ) then 
    			    set thisNumber=`grep "$datatype $run $peak" $inputFile | awk -v var=${getField} '{print $(4+var)}'`
    			else
    			    set thisNumber=`grep "SP $datatype $run $peak" $inputFile | awk -v var=${getField} '{print $(5+var)}'`
    			endif
    			#Make a formatted number
    			if ( $getField == 2 ) then 
			    set thisNumber=`echo $thisNumber | awk '{printf "%12.3f", $1/1}'`
			else
			    set thisNumber=`echo $thisNumber | awk '{printf "%12.3f", $1/1000000}'`
			endif
    			#@ total = $total + $thisNumber
    			set total=`echo $total $thisNumber | awk '{print $1 + $2}'`
    			#
    			set numberString="${numberString} ${thisNumber}"		
    		    end
    		    #Now deal with the slightly trickier run 7, since we have Y2S and Y3S
    		    set run=7
    		    foreach upsilon ( Y2S Y3S )
    			if ( $datatype == AllEvents  ) then 
    			    set thisNumber=`grep "$datatype $run $upsilon $peak" $inputFile | awk -v var=${getField} '{print $(5+var)}'`
    			else
    			    set thisNumber=`grep "SP $datatype $run $upsilon $peak" $inputFile | awk -v var=${getField} '{print $(6+var)}'`
    			endif
    			#Make a formatted number
			if ( $getField == 2 ) then 
			    set thisNumber=`echo $thisNumber | awk '{printf "%12.3f", $1/1}'`
			else
			    set thisNumber=`echo $thisNumber | awk '{printf "%12.3f", $1/1000000}'`
			endif
    			#@ total = $total + $thisNumber
    			set total=`echo $total $thisNumber | awk '{print $1 + $2}'`
    			#
    			set numberString="${numberString} ${thisNumber}"	
    		    end
		    #
    		    #Make a formatted total
		    if ( $getField == 2 ) then 
			set temp=`printf %12.3"f" $total`
			set total=`echo ${temp} | awk '{print $1/7}'`
			set numberString="${numberString} ${total}"
		    else
			set total=`printf %12.3"f" $total`
			set numberString="${numberString} ${total}"
		    endif
    		    echo ${datatype} ${peak} ${numberString} >> $outputFile
    		end
    	end
    	cat $outputFile | column -t
	echo ""
    end
endif

#let's look at joses clean tag skim output...
if ( $choice == makejosesrawnumbers ) then 
    #prepare the output file
    set outputFile=makeSkimNumbersTable_josesrawnumbers.txt
    if ( ! -e ${outputFile}) then 
        touch $outputFile
    endif    
    #    
    #get joses directory 
    set inputPath=/nfs/farm/g/eb/u17/Charm/benitezj/CleanTagSkim/.
    #get the input datasets
    foreach dataset ( `/bin/ls $inputPath | grep -v .tcl` )
        #initialize the total number of events counter
        set totalNEvents=0
        #now get each runtype
        foreach runtype ( `/bin/ls ${inputPath}/${dataset}/` )
	    if ( `grep -c "${dataset} ${runtype}" ${outputFile}` == 0 ) then 
		#now get each log file
		foreach logFileNumber ( `/bin/ls ${inputPath}/${dataset}/${runtype} | grep Snippet | \
    				    grep -v .tcl | awk -F'Snippet' '{print $NF}' | awk -F'.log' '{print $1}'` )
		    #get the number of events for that file
		    set currentNEvents=`grep "EvtCounterTotalSelected:EvtCounterTotalSelected:     total number of events=" \
			${inputPath}/${dataset}/${runtype}/Snippet${logFileNumber}.log| awk -F'=' '{print $NF}'`
		    @ totalNEvents = $totalNEvents + $currentNEvents		    
		end
		echo ${dataset} ${runtype} ${totalNEvents} >> ${outputFile}
	    else
		echo NOTE there\'s already an entry for ${dataset}/${runtype} in the file ${outputFile}
	    endif
	end
    end
endif

if ( $choice == makejosestable ) then 
    	#prepare the output file
    	set outputFile=makeSkimNumbersTable_makejosestable.txt
    	if ( ! -e ${outputFile}) then 
    		touch $outputFile
    	else
    		rm $outputFile
    		touch $outputFile
    	endif
    	#foreach datatype ( 
    	#Get the input file
    	set inputFile=makeSkimNumbersTable_josesrawnumbers.txt
    	#Create the header
    	set header="Datatype Run1 Run2 Run3 Run4 Run5 Run6 Run7(Y2S) Run7(Y3S) Total"
	echo $header >> $outputFile
    	#Make a list of datatypes to loop over (in the order that you want)
	#My original nice list...
    	#foreach datatype ( AllEvents 1005 1235 1237 3429 998 3046 3047 9756 9757 9758 9777 )
	#Jose's daft list...
	foreach datatype ( Data 1005 3046 3047 9756 9757 9758 9775 9776 )
    	#foreach datatype ( Data )
	    foreach peak ( On Off )	
    		    set numberString=""
    		    set total=0
    		    foreach run ( 1 2 3 4 5 6 Y2 Y3 )
    			set thisNumber=`grep "$datatype Run${run}${peak}" $inputFile | awk '{print $3}'`
    			#Make a formatted number
			set thisNumber=`echo $thisNumber | awk '{printf "%12.3f", $1/1}'`
			set thisNumber=`echo $thisNumber | awk '{printf "%12.3f", $1/1000000}'`
    			set total=`echo $total $thisNumber | awk '{print $1 + $2}'`
    			#
    			set numberString="${numberString} ${thisNumber}"		
    		    end
		    #
    		    #Make a formatted total
		    set total=`printf %12.3"f" $total`
		    set numberString="${numberString} ${total}"
		    #
    		     echo ${datatype} ${peak} ${numberString} >> $outputFile
    		end
    	end
    	cat $outputFile | column -t
endif
if ( $choice == subnumbersfornames ) then 
    #get the files you want to manipulate
    foreach file ( `/bin/ls makeSkimNumbersTable_make*.txt` )
	#rm -f dummyFile.txt	
	cat ${file} | sed s/1005/\$c"\\bar{c}"\$/ \
		    | sed s/1235/\$B^{+}B^{-}\$/ \
		    | sed s/1237/\$B^0"\\bar{B^0}"\$/ \
		    | sed s/3429/\$"\\tau\\tau"\$/ \
		    | sed s/998/\$uds\$/ \
		    | sed s/3046/\$"{D_S}^{*+}\{\\rightarrow\}{D_S}^{+}\\gamma,{D_S}^{+}\{\\rightarrow}\\mu^{+}\\nu"\$/ \
		    | sed s/3047/\$"{D_S}^{*-}\{\\rightarrow}{D_S}^{-}\\gamma,{D_S}^{-}\{\\rightarrow}\\mu^{-}\\bar{\\nu}"\$/ \
		    | sed s/9756/\$"{D_S}^{*+}\{\\rightarrow}{D_S}^{+}\\gamma,{D_S}^{+}\{\\rightarrow}K^{0}K^{+}"\$/ \
		    | sed s/9757/\$"{D_S}^{*+}\{\\rightarrow}{D_S}^{+}\\gamma,{D_S}^{+}\{\\rightarrow}\\eta\\pi^{+}"\$/ \
		    | sed s/9758/\$"{D_S}^{*+}\{\\rightarrow}{D_S}^{+}\\gamma,{D_S}^{+}\{\\rightarrow}\\tau^{+}\\nu,\\tau^{+}\{\\rightarrow}\\pi^{+}\\bar{\\nu}"\$/ \
		    | sed s/9775/\$"{D_S}^{*+}\{\\rightarrow}{D_S}^{+}\\gamma,{D_S}^{+}\{\\rightarrow}K^{+}K^{-}\\pi^{+}"\$/ 
		    | sed s/9776/\$"{D_S}^{*-}\{\\rightarrow}{D_S}^{-}\\gamma,{D_S}^{-}\{\\rightarrow}K^{+}K^{-}\\pi^{-}"\$/ 
		    | sed s/9777/\$"{D_S}^{*+}\{\\rightarrow}{D_S}^{+}\\gamma,{D_S}^{+}\{\\rightarrow}\e^{+}\\nu"\$/ 
		    | sed s/9778/\$"{D_S}^{*-}\{\\rightarrow}{D_S}^{-}\\gamma,{D_S}^{-}\{\\rightarrow}\e^{-}\\nu"\$/ 
		    #> dummyFile.txt
	#rm -f ${file}
	#mv dummyFile.txt ${file}
    end
endif

#I feel that this section is overkill and that the tables that will have been produced 
#before this point should suffice
#
# if ( $choice == makelatextables ) then 
# set intputFile=makeSkimNumbersTable_makerawtable0.txt 
# set outputFile=makeSkimNumbersTable_makelatextable0.tex
# #
# if ( ! -e ${outputFile}) then 
# 	touch $outputFile
# else
# 	rm $outputFile
# 	touch $outputFile
# endif
# #
# echo "\NeedsTeXFormat{LaTeX2e}" >> $outputFile
# echo "\documentclass[dvips]{letter}" >> $outputFile
# echo "%epsfig was needed when I wanted to resize a table." >> $outputFile
# echo "\usepackage{epsfig}" >> $outputFile
# echo "%rotating was needed when I wanted to rotate a table. " >> $outputFile
# echo "\usepackage{rotating}" >> $outputFile
# echo "%\usepackage{color}" >> $outputFile
# echo "%array needed due to some of the options below." >> $outputFile
# echo "\usepackage{array}" >> $outputFile
# echo "%For Rows that span more than one column" >> $outputFile
# echo "\usepackage{multirow}" >> $outputFile
# echo "\\begin{document}" >> $outputFile
# echo "" >> $outputFile
# echo "% P Dauncey\'s format code to give more text per page" >> $outputFile
# echo "\oddsidemargin=0pt           % No extra space wasted after first inch." >> $outputFile
# echo "\\evensidemargin=0pt          % Ditto (for two-sided output)." >> $outputFile
# echo "\\topmargin=0pt               % Same for top of page." >> $outputFile
# echo "\headheight=0pt              % Don\'t waste any space on unused headers." >> $outputFile
# echo "\headsep=0pt" >> $outputFile
# echo "\\textwidth=20cm              % Effectively controls the right margin." >> $outputFile
# echo "\\textheight=24cm" >> $outputFile
# echo "" >> $outputFile
# echo "\small" >> $outputFile
# echo "" >> $outputFile
# echo "\\begin{tabular}{ | l | c | c | c | c | c | c | c | c | c | c | }" >> $outputFile
# echo "\hline" >> $outputFile
# #Here's where the table goes


# #
# echo "\hline" >> $outputFile
# echo "\\end{tabular}" >> $outputFile
# echo "" >> $outputFile
# echo "\\end{document} " >> $outputFile
# # 
# endif
