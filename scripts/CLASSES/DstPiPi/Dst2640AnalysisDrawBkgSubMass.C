Int_t  Dst2640Analysis::DrawBkgSubMass(Float_t binsize,Float_t lowpstar,Float_t dstarhel){
  cout<<"Drawing Bkg Sub Mass plots"<<endl;
  cout<<"As of 9/29/2007 this action is obsolete due to no good background model"<<endl;
  return 1;

  if(TruthMatch){
    cout<<"This operation only for data"<<endl;
    return 0;
  }


  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }
  if(ReducedNtuple==NULL){
    cout<<"Ntuple is NULL"<<endl;
    return 0;
  }
  
  if(!OpenBkgNtuple()){
    cout<<"Failed to open the Bkg file"<<endl;
    return 0;
  }  
  if(ccbartree==NULL){
    cout<<"No Background ntuple."<<endl;
    return 0;
  }
  
  TCanvas Canvas("Canvas");
  TString filename;
  if(dstarhel<0)filename=ModeDir+"/BkgSubMass_"+TString("helMinus")+(long)(-10*dstarhel)+"_pstar"+(long)(10*lowpstar)+".ps";
  if(dstarhel>=0)filename=ModeDir+"/BkgSubMass_"+TString("helPlus")+(long)(10*dstarhel)+"_pstar"+(long)(10*lowpstar)+".ps";
  Canvas.Print(filename+"[");
  
  Float_t minmass=2.2;
  Float_t maxmass=3.5;  
  //Float_t binsize=.01;  
  Int_t nmassbins=(int)((maxmass-minmass)/binsize + .5);
  Int_t minmormbin=(int)(nmassbins*(2.8-minmass)/(maxmass-minmass)+.5);
 
    
  sprintf(textlabel,"%.2f<dst2640pstar&&dst2640pstar<4.8",lowpstar);//omitt &&pipicosine>.3&&dstarpipicosine>.6
  TCut ExtraSelectionCut=textlabel;

  sprintf(textlabel,"dstarcostheta<%.2f",dstarhel);
  TCut DstarHelCut=textlabel;
  
  //create cleaned trees
  TTree*cleanreduced=ReducedNtuple->CopyTree(database.GetCut(3,100)+ExtraSelectionCut+DstarHelCut);
  cout<<"reduced entries before="<<ReducedNtuple->GetEntries()<<" After ="<<cleanreduced->GetEntries()<<endl;
  TTree*cleanccbar=ccbartree->CopyTree(database.GetCut(3,100)+ExtraSelectionCut+DstarHelCut);
  cout<<"ccbar entries before="<<ccbartree->GetEntries()<<" After ="<<cleanccbar->GetEntries()<<endl;


  sprintf(textlabel,"pi1charge*pi2charge<0");
  TCut RightSignCut=textlabel;

  sprintf(textlabel,"pi1charge*pi2charge>0");
  TCut WrongSignCut=textlabel;
  
  TH1F HMCMDstarPiPi("HMCMDstarPiPi","HMCMDstarPiPi",nmassbins,minmass,maxmass);
  cleanccbar->Draw("dst2640deltam+2.01>>HMCMDstarPiPi",DstarHelCut+RightSignCut);
  TH1F HMCMDstarPiPiWS("HMCMDstarPiPiWS","HMCMDstarPiPiWS",nmassbins,minmass,maxmass);
  cleanccbar->Draw("dst2640deltam+2.01>>HMCMDstarPiPiWS",DstarHelCut+WrongSignCut);
  Canvas.Clear();
  HMCMDstarPiPi.Draw("pe");
  HMCMDstarPiPiWS.SetMarkerColor(4);
  HMCMDstarPiPiWS.SetLineColor(4);
  HMCMDstarPiPiWS.Draw("pesame");
  Canvas.Print(filename);
  

  //subtract the background
  TH1F HMCMDstarPiPiBkgSubRaw("HMCMDstarPiPiBkgSubRaw","HMCMDstarPiPiBkgSubRaw",nmassbins,minmass,maxmass);
  HMCMDstarPiPiBkgSubRaw.Add(&HMCMDstarPiPi);  
  HMCMDstarPiPiBkgSubRaw.Add(&HMCMDstarPiPiWS,-1);
  for(Int_t b=1;b<=HMCMDstarPiPiBkgSubRaw.GetXaxis()->GetNbins();b++){
    HMCMDstarPiPiBkgSubRaw.SetBinError(b,sqrt(HMCMDstarPiPi.GetBinError(b)*HMCMDstarPiPi.GetBinError(b)
					    +HMCMDstarPiPiWS.GetBinError(b)*HMCMDstarPiPiWS.GetBinError(b)));
    if(HMCMDstarPiPiWS.GetBinContent(b)<3)HMCMDstarPiPiBkgSubRaw.SetBinError(b,0);
  }
  Canvas.Clear();
  HMCMDstarPiPiBkgSubRaw.SetMarkerSize(1);
  HMCMDstarPiPiBkgSubRaw.GetYaxis()->SetRangeUser(0,1.1*HMCMDstarPiPiBkgSubRaw.GetMaximum());
  HMCMDstarPiPiBkgSubRaw.Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(minmass,0,maxmass,0); 
  cutbox.SetLineColor(4);
  cutbox.SetLineWidth(1);
  cutbox.SetFillColor(4);
  cutbox.SetFillStyle(1001);
  cutbox.DrawBox(2.423-.025/2,0,2.423+.025/2,.02*HMCMDstarPiPiBkgSubRaw.GetMaximum());//with 25
  cutbox.DrawBox(2.460-.030/2,0,2.460+.030/2,.02*HMCMDstarPiPiBkgSubRaw.GetMaximum());//width 30
  cutbox.SetLineColor(2);
  cutbox.SetFillColor(2);
  cutbox.DrawBox(2.637-.015/2,0,2.637+.015/2,.02*HMCMDstarPiPiBkgSubRaw.GetMaximum());//width 15
  Canvas.Print(filename);
  


  //now scale the wrong sign
  Float_t mcscale=HMCMDstarPiPi.Integral(minmormbin,nmassbins)/HMCMDstarPiPiWS.Integral(minmormbin,nmassbins);
  TH1F HMCMDstarPiPiWSScale("HMCMDstarPiPiWSScale","HMCMDstarPiPiWSScale",nmassbins,minmass,maxmass);
  HMCMDstarPiPiWSScale.Add(&HMCMDstarPiPiWS);
  HMCMDstarPiPiWSScale.Scale(mcscale);
  Canvas.Clear();
  HMCMDstarPiPi.Draw("pe");
  HMCMDstarPiPiWSScale.SetMarkerColor(4);
  HMCMDstarPiPiWSScale.SetLineColor(4);
  HMCMDstarPiPiWSScale.Draw("pesame");
  cutline.DrawLine(HMCMDstarPiPi.GetBinCenter(minmormbin),0,HMCMDstarPiPi.GetBinCenter(minmormbin),HMCMDstarPiPi.GetMaximum());
  Canvas.Print(filename);

  TH1F HMCMDstarPiPiBkgSubScale("HMCMDstarPiPiBkgSubScale","HMCMDstarPiPiBkgSubScale",nmassbins,minmass,maxmass);
  HMCMDstarPiPiBkgSubScale.Add(&HMCMDstarPiPi);  
  HMCMDstarPiPiBkgSubScale.Add(&HMCMDstarPiPiWSScale,-1);
  for(Int_t b=1;b<=HMCMDstarPiPiBkgSubScale.GetXaxis()->GetNbins();b++){
    HMCMDstarPiPiBkgSubScale.SetBinError(b,sqrt(HMCMDstarPiPi.GetBinError(b)*HMCMDstarPiPi.GetBinError(b)
					 +HMCMDstarPiPiWSScale.GetBinError(b)*HMCMDstarPiPiWSScale.GetBinError(b)));
    if(HMCMDstarPiPiWS.GetBinContent(b)<3)HMCMDstarPiPiBkgSubScale.SetBinError(b,0);
  }
  Canvas.Clear();
  HMCMDstarPiPiBkgSubScale.SetMarkerSize(1);
  HMCMDstarPiPiBkgSubScale.GetYaxis()->SetRangeUser(0,1.1*HMCMDstarPiPiBkgSubScale.GetMaximum());
  HMCMDstarPiPiBkgSubScale.Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(minmass,0,maxmass,0); 
  cutbox.SetLineColor(4);
  cutbox.SetLineWidth(1);
  cutbox.SetFillColor(4);
  cutbox.SetFillStyle(1001);
  cutbox.DrawBox(2.423-.025/2,0,2.423+.025/2,.02*HMCMDstarPiPiBkgSubScale.GetMaximum());//with 25
  cutbox.DrawBox(2.460-.030/2,0,2.460+.030/2,.02*HMCMDstarPiPiBkgSubScale.GetMaximum());//width 30
  cutbox.SetLineColor(2);
  cutbox.SetFillColor(2);
  cutbox.DrawBox(2.637-.015/2,0,2.637+.015/2,.02*HMCMDstarPiPiBkgSubScale.GetMaximum());//width 15
  cutbox.SetLineColor(3);
  cutbox.SetFillColor(0);
  cutbox.SetFillStyle(0);
  cutbox.DrawBox(HMCMDstarPiPiBkgSubScale.GetBinCenter(minmormbin),0,maxmass,.01*HMCMDstarPiPiBkgSubScale.GetMaximum());//norm region
  Canvas.Print(filename);


  //calculate a correction facotor assuming all ccbar is background with same shape as wrong sign
  TH1F HCorrFactor("HCorrFactor","HCorrFactor",nmassbins,minmass,maxmass);
  HCorrFactor.Add(&HMCMDstarPiPiWSScale);
  HCorrFactor.Divide((TH1*)&HMCMDstarPiPi);
  for(Int_t b=1;b<=HCorrFactor.GetXaxis()->GetNbins();b++){
    HCorrFactor.SetBinError(b,HCorrFactor.GetBinContent(b)*
			    sqrt(pow(HMCMDstarPiPiWSScale.GetBinError(b)/HMCMDstarPiPiWSScale.GetBinContent(b),2)+
				 pow(HMCMDstarPiPi.GetBinError(b)/HMCMDstarPiPi.GetBinContent(b),2)));
    if(HMCMDstarPiPi.GetBinContent(b)<3){
      HCorrFactor.SetBinError(b,0);
      HCorrFactor.SetBinContent(b,0);
    }
  }
  Canvas.Clear();
  HCorrFactor.GetYaxis()->SetRangeUser(0,1.5*HCorrFactor.GetMaximum());
  HCorrFactor.Draw();
  Canvas.Print(filename);

  TH1F HMCMDstarPiPiWSScaleCorr("HMCMDstarPiPiWSScaleCorr","HMCMDstarPiPiWSScaleCorr",nmassbins,minmass,maxmass);
  HMCMDstarPiPiWSScaleCorr.Add(&HMCMDstarPiPiWSScale);
  HMCMDstarPiPiWSScaleCorr.Divide((TH1*)&HCorrFactor);
 

  TH1F HMCMDstarPiPiBkgSub("HMCMDstarPiPiBkgSub","HMCMDstarPiPiBkgSub",nmassbins,minmass,maxmass);
  HMCMDstarPiPiBkgSub.Add(&HMCMDstarPiPi);  
  HMCMDstarPiPiBkgSub.Add(&HMCMDstarPiPiWSScaleCorr,-1); 
  for(Int_t b=1;b<=HMCMDstarPiPiBkgSub.GetXaxis()->GetNbins();b++){
    HMCMDstarPiPiBkgSub.SetBinError(b,0); 
  }
  Canvas.Clear();
  HMCMDstarPiPiBkgSub.SetMarkerSize(1);
  HMCMDstarPiPiBkgSub.Draw("pe");
  cutline.DrawLine(minmass,0,maxmass,0);
  cutline.DrawLine(HMCMDstarPiPiBkgSub.GetBinCenter(minmormbin),0,HMCMDstarPiPiBkgSub.GetBinCenter(minmormbin),HMCMDstarPiPiBkgSub.GetMaximum());
  Canvas.Print(filename);

  ///----------------------------Now do the data
  TH1F HMDstarPiPi("HMDstarPiPi","HMDstarPiPi",nmassbins,minmass,maxmass);
  cleanreduced->Draw("dst2640deltam+2.01>>HMDstarPiPi",DstarHelCut+RightSignCut);
  TH1F HMDstarPiPiWS("HMDstarPiPiWS","HMDstarPiPiWS",nmassbins,minmass,maxmass);
  cleanreduced->Draw("dst2640deltam+2.01>>HMDstarPiPiWS",DstarHelCut+WrongSignCut);
  Canvas.Clear();
  HMDstarPiPi.Draw("pe");
  HMDstarPiPiWS.SetMarkerColor(4);
  HMDstarPiPiWS.SetLineColor(4);
  HMDstarPiPiWS.Draw("pesame");
  Canvas.Print(filename);

  
  //subtract the background
  TH1F HMDstarPiPiBkgSubRaw("HMDstarPiPiBkgSubRaw","HMDstarPiPiBkgSubRaw",nmassbins,minmass,maxmass);
  HMDstarPiPiBkgSubRaw.Add(&HMDstarPiPi);  
  HMDstarPiPiBkgSubRaw.Add(&HMDstarPiPiWS,-1);
  for(Int_t b=1;b<=HMDstarPiPiBkgSubRaw.GetXaxis()->GetNbins();b++){
    HMDstarPiPiBkgSubRaw.SetBinError(b,sqrt(HMDstarPiPi.GetBinError(b)*HMDstarPiPi.GetBinError(b)
					    +HMDstarPiPiWS.GetBinError(b)*HMDstarPiPiWS.GetBinError(b)));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiBkgSubRaw.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPiBkgSubRaw.SetMarkerSize(1);
  HMDstarPiPiBkgSubRaw.GetYaxis()->SetRangeUser(0,1.1*HMDstarPiPiBkgSubRaw.GetMaximum());
  HMDstarPiPiBkgSubRaw.Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(minmass,0,maxmass,0); 
  cutbox.SetLineColor(4);
  cutbox.SetLineWidth(1);
  cutbox.SetFillColor(4);
  cutbox.SetFillStyle(1001);
  cutbox.DrawBox(2.423-.025/2,0,2.423+.025/2,.02*HMDstarPiPiBkgSubRaw.GetMaximum());//with 25
  cutbox.DrawBox(2.460-.030/2,0,2.460+.030/2,.02*HMDstarPiPiBkgSubRaw.GetMaximum());//width 30
  cutbox.SetLineColor(2);
  cutbox.SetFillColor(2);
  cutbox.DrawBox(2.637-.015/2,0,2.637+.015/2,.02*HMDstarPiPiBkgSubRaw.GetMaximum());//width 15
  //cutline.DrawLine(2.427,0,2.427,HMDstarPiPiBkgSubRaw.GetMaximum());//width 384
  //cutline.DrawLine(2.537,0,2.537,HMDstarPiPiBkgSubRaw.GetMaximum());//width 5
  Canvas.Print(filename);


  //------------Just scale the wrong sing no correction
  Float_t scaleraw=HMDstarPiPi.Integral(minmormbin,nmassbins)/HMDstarPiPiWS.Integral(minmormbin,nmassbins);
  TH1F HMDstarPiPiWSScaleRaw("HMDstarPiPiWSScaleRaw","HMDstarPiPiWSScaleRaw",nmassbins,minmass,maxmass);
  HMDstarPiPiWSScaleRaw.Add(&HMDstarPiPiWS);
  HMDstarPiPiWSScaleRaw.Scale(scaleraw);
  for(Int_t b=1;b<=HMDstarPiPiWSScaleRaw.GetXaxis()->GetNbins();b++){
    HMDstarPiPiWSScaleRaw.SetBinError(b,scaleraw*HMDstarPiPiWS.GetBinError(b));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiWSScaleRaw.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPi.Draw("pe");
  HMDstarPiPiWSScaleRaw.SetMarkerColor(4);
  HMDstarPiPiWSScaleRaw.SetLineColor(4);
  HMDstarPiPiWSScaleRaw.Draw("pesame");
  cutline.DrawLine(HMDstarPiPi.GetBinCenter(minmormbin),0,HMDstarPiPi.GetBinCenter(minmormbin),HMDstarPiPi.GetMaximum());
  Canvas.Print(filename);


  TH1F HMDstarPiPiBkgSubScaleRaw("HMDstarPiPiBkgSubScaleRaw","HMDstarPiPiBkgSubScaleRaw",nmassbins,minmass,maxmass);
  HMDstarPiPiBkgSubScaleRaw.Add(&HMDstarPiPi);  
  HMDstarPiPiBkgSubScaleRaw.Add(&HMDstarPiPiWSScaleRaw,-1);
  for(Int_t b=1;b<=HMDstarPiPiBkgSubScaleRaw.GetXaxis()->GetNbins();b++){
    HMDstarPiPiBkgSubScaleRaw.SetBinError(b,sqrt(HMDstarPiPi.GetBinError(b)*HMDstarPiPi.GetBinError(b)
					 +HMDstarPiPiWSScaleRaw.GetBinError(b)*HMDstarPiPiWSScaleRaw.GetBinError(b)));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiBkgSubScaleRaw.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPiBkgSubScaleRaw.SetMarkerSize(1);
  HMDstarPiPiBkgSubScaleRaw.GetYaxis()->SetRangeUser(0,1.1*HMDstarPiPiBkgSubScaleRaw.GetMaximum());
  HMDstarPiPiBkgSubScaleRaw.Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(minmass,0,maxmass,0); 
  cutbox.SetLineColor(4);
  cutbox.SetLineWidth(1);
  cutbox.SetFillColor(4);
  cutbox.SetFillStyle(1001);
  cutbox.DrawBox(2.423-.025/2,0,2.423+.025/2,.02*HMDstarPiPiBkgSubScaleRaw.GetMaximum());//with 25
  cutbox.DrawBox(2.460-.030/2,0,2.460+.030/2,.02*HMDstarPiPiBkgSubScaleRaw.GetMaximum());//width 30
  cutbox.SetLineColor(2);
  cutbox.SetFillColor(2);
  cutbox.DrawBox(2.637-.015/2,0,2.637+.015/2,.02*HMDstarPiPiBkgSubScaleRaw.GetMaximum());//width 15
  cutbox.SetLineColor(3);
  cutbox.SetFillColor(0);
  cutbox.SetFillStyle(0);
  cutbox.DrawBox(HMDstarPiPiBkgSubScaleRaw.GetBinCenter(minmormbin),0,maxmass,.01*HMDstarPiPiBkgSubScaleRaw.GetMaximum());//norm region
  Canvas.Print(filename);


 
  //------------------now correct wrong sign then scale 
  //first do correction
  TH1F HMDstarPiPiWSCorr("HMDstarPiPiWSCorr","HMDstarPiPiWSCorr",nmassbins,minmass,maxmass);
  HMDstarPiPiWSCorr.Add(&HMDstarPiPiWS);
  HMDstarPiPiWSCorr.Divide((TH1*)&HCorrFactor);
  for(Int_t b=1;b<=HMDstarPiPiWSCorr.GetXaxis()->GetNbins();b++){
    HMDstarPiPiWSCorr.SetBinError(b,HMDstarPiPiWSCorr.GetBinContent(b)*
				  sqrt(pow(HMDstarPiPiWS.GetBinError(b)/HMDstarPiPiWS.GetBinContent(b),2)+
				       pow(HCorrFactor.GetBinError(b)/HCorrFactor.GetBinContent(b),2)));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiWSCorr.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPi.Draw("pe");
  HMDstarPiPiWSCorr.SetMarkerColor(4);
  HMDstarPiPiWSCorr.SetLineColor(4);
  HMDstarPiPiWSCorr.Draw("pesame");
  Canvas.Print(filename);

  TH1F HMDstarPiPiBkgSubCorr("HMDstarPiPiBkgSubCorr","HMDstarPiPiBkgSubCorr",nmassbins,minmass,maxmass);
  HMDstarPiPiBkgSubCorr.Add(&HMDstarPiPi);  
  HMDstarPiPiBkgSubCorr.Add(&HMDstarPiPiWSCorr,-1);
  for(Int_t b=1;b<=HMDstarPiPiBkgSubCorr.GetXaxis()->GetNbins();b++){
    HMDstarPiPiBkgSubCorr.SetBinError(b,sqrt(HMDstarPiPi.GetBinError(b)*HMDstarPiPi.GetBinError(b)
					 +HMDstarPiPiWSCorr.GetBinError(b)*HMDstarPiPiWSCorr.GetBinError(b)));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiBkgSubCorr.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPiBkgSubCorr.SetMarkerSize(1);
  HMDstarPiPiBkgSubCorr.GetYaxis()->SetRangeUser(0,1.1*HMDstarPiPiBkgSubCorr.GetMaximum());
  HMDstarPiPiBkgSubCorr.Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(minmass,0,maxmass,0);  
  cutbox.SetLineColor(4);
  cutbox.SetLineWidth(1);
  cutbox.SetFillColor(0);
  cutbox.SetFillStyle(1001);
  cutbox.DrawBox(2.423-.025/2,0,2.423+.025/2,.02*HMDstarPiPiBkgSubCorr.GetMaximum());//with 25
  cutbox.DrawBox(2.460-.030/2,0,2.460+.030/2,.02*HMDstarPiPiBkgSubCorr.GetMaximum());//width 30
  cutbox.SetLineColor(2);
  cutbox.SetLineColor(2);
  cutbox.DrawBox(2.637-.015/2,0,2.637+.015/2,.02*HMDstarPiPiBkgSubCorr.GetMaximum());//width 15
  cutbox.SetLineColor(3);
  cutbox.SetFillColor(0);
  cutbox.SetFillStyle(0);
  cutbox.DrawBox(HMDstarPiPiBkgSubCorr.GetBinCenter(minmormbin),0,maxmass,.01*HMDstarPiPiBkgSubCorr.GetMaximum());//norm region
  Canvas.Print(filename);



  Float_t scale=HMDstarPiPi.Integral(minmormbin,nmassbins)/HMDstarPiPiWSCorr.Integral(minmormbin,nmassbins);
  TH1F HMDstarPiPiWSCorrScale("HMDstarPiPiWSCorrScale","HMDstarPiPiWSCorrScale",nmassbins,minmass,maxmass);
  HMDstarPiPiWSCorrScale.Add(&HMDstarPiPiWSCorr);
  HMDstarPiPiWSCorrScale.Scale(scale);
  for(Int_t b=1;b<=HMDstarPiPiWSCorrScale.GetXaxis()->GetNbins();b++){
    HMDstarPiPiWSCorrScale.SetBinError(b,scale*HMDstarPiPiWSCorr.GetBinError(b));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiWSCorrScale.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPi.Draw("pe");
  HMDstarPiPiWSCorrScale.SetMarkerColor(4);
  HMDstarPiPiWSCorrScale.SetLineColor(4);
  HMDstarPiPiWSCorrScale.Draw("pesame");
  cutline.DrawLine(HMDstarPiPi.GetBinCenter(minmormbin),0,HMDstarPiPi.GetBinCenter(minmormbin),HMDstarPiPi.GetMaximum());
  Canvas.Print(filename);
 

  TH1F HMDstarPiPiBkgSub("HMDstarPiPiBkgSub","HMDstarPiPiBkgSub",nmassbins,minmass,maxmass);
  HMDstarPiPiBkgSub.Add(&HMDstarPiPi);  
  HMDstarPiPiBkgSub.Add(&HMDstarPiPiWSCorrScale,-1);
  for(Int_t b=1;b<=HMDstarPiPiBkgSub.GetXaxis()->GetNbins();b++){
    HMDstarPiPiBkgSub.SetBinError(b,sqrt(HMDstarPiPi.GetBinError(b)*HMDstarPiPi.GetBinError(b)
					 +HMDstarPiPiWSCorrScale.GetBinError(b)*HMDstarPiPiWSCorrScale.GetBinError(b)));
    if(HMDstarPiPiWS.GetBinContent(b)<3)HMDstarPiPiBkgSub.SetBinError(b,0);
  }
  Canvas.Clear();
  HMDstarPiPiBkgSub.SetMarkerSize(1);
  HMDstarPiPiBkgSub.GetYaxis()->SetRangeUser(0,1.1*HMDstarPiPiBkgSub.GetMaximum());
  HMDstarPiPiBkgSub.Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(minmass,0,maxmass,0);  
  cutbox.SetLineColor(4);
  cutbox.SetLineWidth(1);
  cutbox.SetFillColor(0);
  cutbox.SetFillStyle(1001);
  cutbox.DrawBox(2.423-.025/2,0,2.423+.025/2,.02*HMDstarPiPiBkgSub.GetMaximum());//with 25
  cutbox.DrawBox(2.460-.030/2,0,2.460+.030/2,.02*HMDstarPiPiBkgSub.GetMaximum());//width 30
  cutbox.SetLineColor(2);
  cutbox.SetLineColor(2);
  cutbox.DrawBox(2.637-.015/2,0,2.637+.015/2,.02*HMDstarPiPiBkgSub.GetMaximum());//width 15
  cutbox.SetLineColor(3);
  cutbox.SetFillColor(0);
  cutbox.SetFillStyle(0);
  cutbox.DrawBox(HMDstarPiPiBkgSub.GetBinCenter(minmormbin),0,maxmass,.01*HMDstarPiPiBkgSub.GetMaximum());//norm region
  Canvas.Print(filename);




  Canvas.Print(filename+"]");


  if(!CloseBkgNtuple()){
    cout<<"Failed to close the Bkg file"<<endl;
    return 0;
  }
  if(!Close()){
    cout<<"Failed to close the Bkg file"<<endl;
    return 0;
  }

  return 1;
}
