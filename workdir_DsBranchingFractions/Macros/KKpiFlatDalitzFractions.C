
#define  Npoints 800
void KKpiFlatDalitzFractions(){

  Float_t M=1.9686;
  Float_t m1=.49368;
  Float_t m2=.49368;
  Float_t m3=.13957;
  

  //Define the Histogram with the boundaries
  Float_t XMin=.8;
  Float_t XMax=3.5;
  Float_t YMin=.3;
  Float_t YMax=2.3;
  TH2F HTop("HTop","",Npoints,XMin,XMax,Npoints,YMin,YMax); 
  HTop.SetStats(0);
  TH2F HBot("HBot","",Npoints,XMin,XMax,Npoints,YMin,YMax);
  HBot.SetStats(0);
  Double_t x=0;
  Double_t y=0;
  Float_t xmin=(m1+m2)*(m1+m2);
  Float_t xmax=(M-m3)*(M-m3);
  Float_t xstep=(xmax-xmin)/Npoints;
  Float_t x0=0;
  Float_t y0=0;

  Float_t E2,E3;
  
  //First do the top points from left to right
  for(Int_t p=0;p<Npoints;p++){
    x=xmin+p*xstep;
    E2=(x-m1*m1+m2*m2)/(2*sqrt(x));
    E3=(M*M-x-m3*m3)/(2*sqrt(x));
    y=(E2+E3)*(E2+E3)-pow((sqrt(E2*E2-m2*m2)-sqrt(E3*E3-m3*m3)),2);      
    HTop.Fill(x,y,1);
  }

  //Now do the bottom points from right to left
  for(Int_t p=0;p<Npoints;p++){
    x=xmax-p*xstep;//move backwards to draw with line   
    E2=(x-m1*m1+m2*m2)/(2*sqrt(x));
    E3=(M*M-x-m3*m3)/(2*sqrt(x));
    y=(E2+E3)*(E2+E3)-pow((sqrt(E2*E2-m2*m2)+sqrt(E3*E3-m3*m3)),2);  
    HBot.Fill(x,y,1);
  }
  
  for(Int_t bx=1;bx<=Npoints;bx++)
    for(Int_t by=1;by<=Npoints;by++){
      if(HTop.GetBinContent(bx,by)>0)HTop.SetBinContent(bx,by,1);
      if(HBot.GetBinContent(bx,by)>0)HBot.SetBinContent(bx,by,1);
    }
 
  TCanvas C;
  C.Print("KKpiFlatDalitzFractions.ps[");

  C.Clear();
  HTop.Draw("col");
  C.Print("KKpiFlatDalitzFractions.ps");
  C.Clear();
  HBot.Draw("col");
  C.Print("KKpiFlatDalitzFractions.ps");

  C.Clear();
  HTop.Draw("col");
  HBot.Draw("col same");
  C.Print("KKpiFlatDalitzFractions.ps");



  ////////////////////////////////
  //Create the arrays which map the y bin for each x bin
  Int_t TopBound[Npoints];
  Int_t BotBound[Npoints];
  for(Int_t bx=0;bx<Npoints;bx++){
    TopBound[bx]=0;
    BotBound[bx]=Npoints;
    for(Int_t by=0;by<Npoints;by++){
      if(HTop.GetBinContent(bx+1,by+1)>0)TopBound[bx]=by;
      if(HBot.GetBinContent(bx+1,by+1)>0)BotBound[bx]=by;
    }
  }
  
  //Dalitz1 Bounds
  Float_t xwidth=(XMax-XMin)/Npoints;
  Float_t ywidth=(YMax-YMin)/Npoints;
  
  Int_t Dalitz1Xmin=(int)((.95-XMin)/xwidth);
  Int_t Dalitz1Xmax=(int)((1.15-XMin)/xwidth);

  Int_t Dalitz2Ymin=(int)((.6-YMin)/ywidth);
  Int_t Dalitz2Ymax=(int)((1.-YMin)/ywidth);


  ////////////Throw random points on the full range and determine if it is inside  the physical region
  TH2F HDalitzSig("HDalitzSig","",Npoints,XMin,XMax,Npoints,YMin,YMax); 
  HDalitzSig.SetStats(0);
  TH2F HDalitz1("HDalitz1","",Npoints,XMin,XMax,Npoints,YMin,YMax); 
  HDalitz1.SetStats(0);
  TH2F HDalitz2("HDalitz2","",Npoints,XMin,XMax,Npoints,YMin,YMax); 
  HDalitz2.SetStats(0);
  TH2F HDalitz3("HDalitz3","",Npoints,XMin,XMax,Npoints,YMin,YMax); 
  HDalitz3.SetStats(0);
  TRandom rand;
  for(Int_t i=0;i<Npoints*Npoints*2;i++){
    Int_t xr=(int)rand.Uniform(1,Npoints+1);
    Int_t yr=(int)rand.Uniform(1,Npoints+1);
    if(BotBound[xr]<=yr&&yr<=TopBound[xr]){
      HDalitzSig.SetBinContent(xr,yr,HDalitzSig.GetBinContent(xr,yr)+1);
      
      if(Dalitz1Xmin<=xr&&xr<=Dalitz1Xmax){
	HDalitz1.SetBinContent(xr,yr,HDalitz1.GetBinContent(xr,yr)+1);
      }
      else if(Dalitz2Ymin<=yr&&yr<=Dalitz2Ymax){
	HDalitz2.SetBinContent(xr,yr,HDalitz2.GetBinContent(xr,yr)+1);
      }else HDalitz3.SetBinContent(xr,yr,HDalitz3.GetBinContent(xr,yr)+1);

      
    }
  }

  C.Clear();
  //HTop.Draw("col");
  //HBot.Draw("col same");
  HDalitzSig.Draw("col");
  C.Print("KKpiFlatDalitzFractions.ps");

  C.Clear();
  //HTop.Draw("col");
  //HBot.Draw("col same");
  HDalitz1.Draw("col ");
  C.Print("KKpiFlatDalitzFractions.ps");

  C.Clear();
  //HTop.Draw("col");
  //HBot.Draw("col same");
  HDalitz2.Draw("col");
  C.Print("KKpiFlatDalitzFractions.ps");

  C.Clear();
  //HTop.Draw("col");
  //HBot.Draw("col same");
  HDalitz3.Draw("col");
  C.Print("KKpiFlatDalitzFractions.ps");

  cout<<"Fraction1 = "<<HDalitz1.Integral()/(float)HDalitzSig.Integral()<<endl;
  cout<<"Fraction2 = "<<HDalitz2.Integral()/(float)HDalitzSig.Integral()<<endl;
  cout<<"Fraction3 = "<<HDalitz3.Integral()/(float)HDalitzSig.Integral()<<endl;


  C.Print("KKpiFlatDalitzFractions.ps]");
  

}
