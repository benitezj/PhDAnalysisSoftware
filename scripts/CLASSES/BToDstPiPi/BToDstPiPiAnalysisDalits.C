
Int_t BToDstPiPiAnalysis::PlotDalits(){

  
  if(!OpenReducedNtuple())return 0;
    
  TFile SBFile(_OutputDir+"/../KPiSB/Ntuple.root","read");
  gROOT->cd();
  TTree*SBtree=(TTree*)SBFile.Get("Ntuple");
  if(!SBtree)return 0;
  

  filename=_OutputDir+"/Dalits.ps";
  Canvas.Print(filename+"[");


  TH2F HDalits("HDalits","",350,2,5.5,350,2,5.5);
  ReducedNtuple->Draw("bdstpi2mass:bdstpi1mass>>HDalits");
  Canvas.Clear();  
  HDalits.Draw("colz");
  Canvas.Print(filename);


  TH2F HDalitsSB("HDalitsSB","",350,2,5.5,350,2,5.5);
  SBtree->Draw("bdstpi2mass:bdstpi1mass>>HDalitsSB");
  Canvas.Clear();  
  HDalitsSB.Draw("colz");
  Canvas.Print(filename);


  TH2F HDalitsFolded("HDalitsFolded","",250,3,5.5,200,2,4);
  ReducedNtuple->Draw("bdstpi1massfolded:bdstpi2massfolded>>HDalitsFolded");
  Canvas.Clear();  
  HDalitsFolded.Draw("colz");
  Canvas.Print(filename);
 
  
  TH2F HDalitsFoldedSB1("HDalitsFoldedSB1","bmes<5.25",250,3,5.5,200,2,4);
  SBtree->Draw("bdstpi1massfolded:bdstpi2massfolded>>HDalitsFoldedSB1","bmes<5.25");
  Canvas.Clear();  
  HDalitsFoldedSB1.Draw("colz");
  Canvas.Print(filename);

  TH2F HDalitsFoldedSB2("HDalitsFoldedSB2","bmes>5.25",250,3,5.5,200,2,4);
  SBtree->Draw("bdstpi1massfolded:bdstpi2massfolded>>HDalitsFoldedSB2","bmes>5.25");
  Canvas.Clear();  
  HDalitsFoldedSB2.Draw("colz");
  Canvas.Print(filename);

  TH2F HDalitsFoldedSB("HDalitsFoldedSB","",250,3,5.5,200,2,4);
  SBtree->Draw("bdstpi1massfolded:bdstpi2massfolded>>HDalitsFoldedSB");
  Canvas.Clear();  
  HDalitsFoldedSB.Draw("colz");
  Canvas.Print(filename);


  TH1F HDstPi("HDstPi","",150,2,3.5);
  ReducedNtuple->Draw("bdstpi1massfolded>>HDstPi");
  TH1F HDstPiSB("HDstPiSB","",150,2,3.5);
  SBtree->Draw("bdstpi1massfolded>>HDstPiSB");
  Int_t NBackground=15055;
  cout<<"Check the Background Normalization="<<NBackground<<endl;
  HDstPiSB.Scale(NBackground/HDstPiSB.Integral());
  Canvas.Clear();  
  HDstPi.Draw("pe");
  HDstPiSB.Draw("pesame");
  Canvas.Print(filename);

  HDstPi.Sumw2();
  HDstPi.Add(&HDstPiSB,-1);
  HDstPi.GetYaxis()->SetRangeUser(-.1*HDstPi.GetMaximum(),1.1*HDstPi.GetMaximum());
  Canvas.Clear();  
  HDstPi.Draw("pe");
  cutline.DrawLine(HDstPi.GetXaxis()->GetXmin(),0,HDstPi.GetXaxis()->GetXmax(),0);
  Canvas.Print(filename);

  //cut on decay angle
  TH1F HDstPiCut("HDstPiCut","",150,2,3.5);
  ReducedNtuple->Draw("bdstpi1massfolded>>HDstPiCut","bdstpi1decayanglefolded<0");
  TH1F HDstPiCutSB("HDstPiCutSB","",150,2,3.5);
  SBtree->Draw("bdstpi1massfolded>>HDstPiCutSB","bdstpi1decayanglefolded<0");
  cout<<"Check the Background Normalization="<<NBackground<<endl;
  //HDstPiCutSB.Scale(NBackground/HDstPiCutSB.Integral());
  Canvas.Clear();  
  HDstPiCut.Draw("pe");
  HDstPiCutSB.Draw("pesame");
  Canvas.Print(filename);
  //--Subtract
  HDstPiCut.Sumw2();
  HDstPiCut.Add(&HDstPiCutSB,-1);
  HDstPiCut.GetYaxis()->SetRangeUser(-.1*HDstPiCut.GetMaximum(),1.1*HDstPiCut.GetMaximum());
  Canvas.Clear();  
  HDstPiCut.Draw("pe");
  cutline.DrawLine(HDstPiCut.GetXaxis()->GetXmin(),0,HDstPiCut.GetXaxis()->GetXmax(),0);
  Canvas.Print(filename);

   
  Canvas.Print(filename+"]");
  SBFile.Close();
  CloseReducedNtuple();
  return 1;
}

Int_t BToDstPiPiAnalysis::PlotSquareDalits(){

  
  if(!OpenReducedNtuple())return 0;
    
  TFile SBFile(_OutputDir+"/../KPiSB/Ntuple.root","read");
  gROOT->cd();
  TTree*SBtree=(TTree*)SBFile.Get("Ntuple");
  if(!SBtree)return 0;
  

  filename=_OutputDir+"/SquareDalits.ps";
  Canvas.Print(filename+"[");


  TH2F HDalitsFolded("HDalitsFolded","",70,2.1,3.5,20,-1,1);
  ReducedNtuple->Draw("bdstpi1decayanglefolded:bdstpi1massfolded>>HDalitsFolded");
  Canvas.Clear();  
  HDalitsFolded.Draw("colz");
  Canvas.Print(filename);

  TH2F HDalitsFoldedSig("HDalitsFoldedSig","",60,2.3,2.6,20,-1,1);
  ReducedNtuple->Draw("bdstpi1decayanglefolded:bdstpi1massfolded>>HDalitsFoldedSig");
  Canvas.Clear();  
  HDalitsFoldedSig.Draw("colz");
  Canvas.Print(filename);
 

  TH2F HDalitsFoldedSB("HDalitsFoldedSB","",70,2.1,3.5,20,-1,1);
  SBtree->Draw("bdstpi1decayanglefolded:bdstpi1massfolded>>HDalitsFoldedSB");
  Int_t NBackground=15055;
  cout<<"Check the Background Normalization="<<NBackground<<endl;
  HDalitsFoldedSB.Sumw2();
  HDalitsFoldedSB.Scale(NBackground/HDalitsFoldedSB.Integral());
  Canvas.Clear();  
  HDalitsFoldedSB.Draw("colz");
  Canvas.Print(filename);

  
  ///Total Angle Distribution
  TH1F HAngleFolded("HAngleFolded","",20,-1,1);
  ReducedNtuple->Draw("bdstpi1decayanglefolded>>HAngleFolded");
  TH1F HAngleFoldedSB("HAngleFoldedSB","",20,-1,1);
  SBtree->Draw("bdstpi1decayanglefolded>>HAngleFoldedSB");
  HAngleFoldedSB.Sumw2();
  HAngleFoldedSB.Scale(NBackground/HDalitsFoldedSB.Integral());
  Canvas.Clear();  
  HAngleFolded.GetYaxis()->SetRangeUser(0,HAngleFolded.GetMaximum()*1.1); 
  HAngleFolded.Draw("pe");
  HAngleFoldedSB.Draw("same"); 
  Canvas.Print(filename);
 

  //angle Distribution for D(2420)
  TH1F HAngleSigFolded("HAngleSigFolded","",20,-1,1);
  ReducedNtuple->Draw("bdstpi1decayanglefolded>>HAngleSigFolded","2.40<bdstpi1massfolded&&bdstpi1massfolded<2.44");
  TH1F HAngleSigFoldedSB("HAngleSigFoldedSB","",20,-1,1);
  SBtree->Draw("bdstpi1decayanglefolded>>HAngleSigFoldedSB","2.40<bdstpi1massfolded&&bdstpi1massfolded<2.44");
  HAngleSigFoldedSB.Sumw2();
  HAngleSigFoldedSB.Scale(NBackground/HDalitsFoldedSB.Integral());
  Canvas.Clear(); 
  HAngleSigFolded.GetYaxis()->SetRangeUser(0,HAngleSigFolded.GetMaximum()*1.1); 
  HAngleSigFolded.Draw("pe");
  HAngleSigFoldedSB.Draw("same"); 
  Canvas.Print(filename);

  //angle Distribution for D(2460)
  TH1F HAngleSig2Folded("HAngleSig2Folded","",20,-1,1);
  ReducedNtuple->Draw("bdstpi1decayanglefolded>>HAngleSig2Folded","2.44<bdstpi1massfolded&&bdstpi1massfolded<2.48");
  TH1F HAngleSig2FoldedSB("HAngleSig2FoldedSB","",20,-1,1);
  SBtree->Draw("bdstpi1decayanglefolded>>HAngleSig2FoldedSB","2.44<bdstpi1massfolded&&bdstpi1massfolded<2.48");
  HAngleSig2FoldedSB.Sumw2();
  HAngleSig2FoldedSB.Scale(NBackground/HDalitsFoldedSB.Integral()); 
  Canvas.Clear(); 
  HAngleSig2Folded.GetYaxis()->SetRangeUser(0,HAngleSig2Folded.GetMaximum()*1.1); 
  HAngleSig2Folded.Draw("pe");
  HAngleSig2FoldedSB.Draw("same"); 
  Canvas.Print(filename);


  ///Angle distribution Background subtracted
  HDalitsFolded.Sumw2();
  HDalitsFolded.Add(&HDalitsFoldedSB,-1);  
  Canvas.Clear(); 
  HDalitsFolded.Draw("colz");
  Canvas.Print(filename);
  
  Canvas.Clear();
  TH1D* H[24];
  Canvas.Divide(4,6);
  for(Int_t i=1;i<=24;i++){
    Canvas.cd(i);
    H[i-1]=HDalitsFolded.ProjectionY(TString("")+(long)i,i,i,"");
    //H[i-1]->GetYaxis()->SetRangeUser(-10,H[i-1]->GetMaximum()*1.1);
    H[i-1]->Draw("pe");
    cutline.DrawLine(H[i-1]->GetXaxis()->GetXmin(),0,H[i-1]->GetXaxis()->GetXmax(),0);
    text.DrawTextNDC(.1,.9,TString("Mass=")+(long)(1000*HDalitsFolded.GetXaxis()->GetBinCenter(i)));
  }
  Canvas.Print(filename);
  for(Int_t i=1;i<24;i++)
    delete H[i-1];

  Canvas.Clear();
  Canvas.Divide(4,6);
  for(Int_t i=1;i<=24;i++){
    Canvas.cd(i);
    H[i-1]=HDalitsFolded.ProjectionY(TString("")+(long)(i+24),i+24,i+24,"");
    //H[i-1]->GetYaxis()->SetRangeUser(-10,H[i-1]->GetMaximum()*1.1);
    H[i-1]->Draw("pe");
    cutline.DrawLine(H[i-1]->GetXaxis()->GetXmin(),0,H[i-1]->GetXaxis()->GetXmax(),0);
    text.DrawTextNDC(.1,.9,TString("Mass=")+(long)(1000*HDalitsFolded.GetXaxis()->GetBinCenter(i)));
  }
  Canvas.Print(filename);
  for(Int_t i=1;i<24;i++)
    delete H[i-1];

  
  Canvas.Print(filename+"]");
  SBFile.Close();
  CloseReducedNtuple();
  return 1;
}
