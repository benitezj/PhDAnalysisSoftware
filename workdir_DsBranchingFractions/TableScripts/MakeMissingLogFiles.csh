
#create missing log files with number of events for counting passing rates.

foreach c ( `/bin/ls CleanTagSkim/SP1005/Run1Off/MyDRecoCleanTagSkim_*` )
set d=`echo $c | sed s/.01.root/""/`
set de=`echo $d | sed s:/:"\\/":g`
set log=`echo $d | sed s/"CleanTagSkim\/SP1005\/Run1Off\/MyDRecoCleanTagSkim\_"/Snippet/`
touch CleanTagSkim/SP1005/Run1Off/${log}.log
set n=`KanCollUtil $d | sed s:${de}:"": | sed s/\(/""/ | sed s/\)/""/ | sed s/events/""/`
echo "AppAST:     0.00 |   ${n}  |      0.06 |        0.01765 |   0.00 |MyDRecoCleanTagSkimPathCounter" >> CleanTagSkim/SP1005/Run1Off/${log}.log
end
