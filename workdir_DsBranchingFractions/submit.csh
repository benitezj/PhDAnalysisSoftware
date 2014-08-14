
#################################
###ccbar
#################################
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run1Off 1 47 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run2Off 1 147 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run3Off 1 55 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run4Off 1 178 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run5Off 1 316 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run6Off 1 172 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY2SOn 1 252 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY2SOff 1 26 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY3SOn 1 591 1 kanga
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY3SOff 1 66 1 kanga

foreach x (1 2 3 4 5 6 Y2S Y3S)
ManageSkimJobs.pl CleanTagSkim/SP1005/Run${x}On 2 10000 1
ManageSkimJobs.pl CleanTagSkim/SP1005/Run${x}Off 2 10000 1
end

#ManageSkimJobs.pl CleanTagSkim/SP1005/Run1Off 2 47 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run2Off 2 147 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run3Off 2 55 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run4Off 2 178 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run5Off 2 316 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/Run6Off 2 172 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY2SOn 2 252 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY2SOff 2 26 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY3SOn 2 591 1 
#ManageSkimJobs.pl CleanTagSkim/SP1005/RunY3SOff 2 66 1 

#################################
#####data
#################################
#ManageSkimJobs.pl CleanTagSkim/Data/Run1Off 1 51 1 kanga
#ManageSkimJobs.pl CleanTagSkim/Data/Run2Off 1 150 1 kanga
#ManageSkimJobs.pl CleanTagSkim/Data/Run3Off 1 53 1 kanga
#ManageSkimJobs.pl CleanTagSkim/Data/Run4Off 1 199 1 kanga
#ManageSkimJobs.pl CleanTagSkim/Data/Run5Off 1 309 1 kanga
#ManageSkimJobs.pl CleanTagSkim/Data/Run6Off 1 171 1 kanga

foreach x (1 2 3 4 5 6 Y2S Y3S)
ManageSkimJobs.pl CleanTagSkim/Data/Run${x}On 2 10000 1 
ManageSkimJobs.pl CleanTagSkim/Data/Run${x}Off 2 10000 1 
end

#ManageSkimJobs.pl CleanTagSkim/Data/Run1Off 2 51 1  
#ManageSkimJobs.pl CleanTagSkim/Data/Run2Off 2 150 1  
#ManageSkimJobs.pl CleanTagSkim/Data/Run3Off 2 53 1  
#ManageSkimJobs.pl CleanTagSkim/Data/Run4Off 2 199 1  
#ManageSkimJobs.pl CleanTagSkim/Data/Run5Off 2 309 1  
#ManageSkimJobs.pl CleanTagSkim/Data/Run6Off 2 171 1  



