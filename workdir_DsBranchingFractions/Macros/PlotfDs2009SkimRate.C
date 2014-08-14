void PlotfDs2009SkimRate(TString taskname){

  ifstream file;
  file.open(taskname,ios_base::in);

  Int_t jobid;
  Int_t procevents;
  Int_t passevents;
  TH1F HSkimInput(TString("HSkimInput"),"",100000,0,100000);
  TH1F HSkimPass(TString("HSkimPass"),"",100000,0,100000);
  TH1F HSkimRate(TString("HSkimRate"),"",100000,0,100000);
  Int_t filecounter=0;
  Int_t proccounter=0;
  Int_t passcounter=0;
  while(!file.eof()){
    file>>jobid>>procevents>>passevents;
    cout<<jobid<<" "<<procevents<<" "<<passevents<<endl;
        
    HSkimInput.SetBinContent(jobid,procevents);
    HSkimPass.SetBinContent(jobid,passevents);
    HSkimRate.SetBinContent(jobid,passevents/(float)procevents);
  
    filecounter++;  
    proccounter+=procevents;
    passcounter+=passevents;
  }

  file.close();

  TCanvas C;
  C.Divide(1,3);
  TVirtualPad *pad=C.cd(1);
  pad->SetLeftMargin(.05);  pad->SetRightMargin(.05);  
  HSkimInput.GetXaxis()->SetRangeUser(0,filecounter);
  HSkimInput.GetXaxis()->SetTitle("job id");
  HSkimInput.GetYaxis()->SetTitle("Processed Events");
  HSkimInput.GetYaxis()->SetTitleOffset(.5);
  HSkimInput.SetTitle(TString("Total Jobs = ")+(long)filecounter+"    Total Events Processed= "+(long)proccounter);
  HSkimInput.SetStats(0);
  HSkimInput.Draw();
  pad=C.cd(2);
  pad->SetLeftMargin(.05);  pad->SetRightMargin(.05);  
  HSkimPass.GetXaxis()->SetRangeUser(0,filecounter);
  HSkimPass.GetXaxis()->SetTitle("job id");
  HSkimPass.GetYaxis()->SetTitle("Passed Events");
  HSkimPass.GetYaxis()->SetTitleOffset(.5);
  HSkimPass.SetTitle(TString("Total Events Passed= ")+(long)passcounter);
  HSkimPass.SetStats(0);
  HSkimPass.Draw();
  pad=C.cd(3);
  pad->SetLeftMargin(.05);  pad->SetRightMargin(.05);  
  HSkimRate.GetXaxis()->SetRangeUser(0,filecounter);
  HSkimRate.GetXaxis()->SetTitle("job id");
  HSkimRate.GetYaxis()->SetTitle("Skim Rate");
  HSkimRate.GetYaxis()->SetTitleOffset(.5);
  HSkimRate.SetStats(0);
  HSkimRate.Draw();
  C.Print(taskname+".ps");

}
