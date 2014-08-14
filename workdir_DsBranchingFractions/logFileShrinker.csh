#!/bin/tcsh 

###############################
##Clean out unwanted lines and gzip log files
###############################
set inputDir=$1

echo "directory size before:"
du -sh ${inputDir}

set counter=0
foreach job ( `/bin/ls ${inputDir}/ | grep Snippet | grep log` )
 rm -f ${inputDir}/dummyLogFile.log
 
 cat ${inputDir}/${job} | \
 grep -v "BtaLoadGHitAssoc::RecoCompGMatchK.cc(308):sizes don't match ! components" > ${inputDir}/dummyLogFile.log
 
 rm -f ${inputDir}/${job}
 cp -p ${inputDir}/dummyLogFile.log ${inputDir}/${job}
 gzip ${inputDir}/${job} 

 @ counter = $counter + 1
end

rm -f ${inputDir}/dummyLogFile.log

echo "shrank ${counter} log files" 
echo "directory size after:"
du -sh ${inputDir}
