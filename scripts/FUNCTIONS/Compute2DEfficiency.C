#include "../Globals.h"



void Fill2DRelativeEffErr(TH2F* heff,TH2F* hefferr){

    for(Int_t x=1;x<=heff->GetNbinsX();x++){
      for(Int_t y=1;y<=heff->GetNbinsY();y++){
	if(heff->GetBinContent(x,y)>0)
	  if(heff->GetBinError(x,y)/heff->GetBinContent(x,y)<.2)
	    hefferr->SetBinContent(x,y,heff->GetBinError(x,y)/heff->GetBinContent(x,y));      
	  else hefferr->SetBinContent(x,y,0);
	else hefferr->SetBinContent(x,y,0);
      }
    }        
  
}




TH2F* Interpolate2DEfficiencyFitX(TH2F* h2,TH1F* HEffChiX){
  if(!h2){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2->GetName())+"InterX";
  
  Int_t nbinsx=h2->GetNbinsX();
  Int_t nbinsy=h2->GetNbinsY();  
  TH2F* H2Interpolation=new TH2F(name,name,
				 nbinsx*10,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax(),
				 nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());

  TH1F HSlice("HSlice","HSlice",nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());  
  TF1 FSlice("FSlice","[0]+[1]*(x-[2])",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
  
  (*HEffChiX).SetNameTitle(name+"chi",name+"chi");
  (*HEffChiX).SetBins(nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  
  Float_t eff;
  Float_t error;
  Float_t chiperdf;
  Int_t ngood=0;
  for(Int_t by=1;by<=nbinsy;by++){
    ngood=0;
    for(Int_t bx=1;bx<=nbinsx;bx++){
      if(h2->GetBinContent(bx,by)>0){
	ngood++;
	HSlice.SetBinContent(bx,h2->GetBinContent(bx,by));
	HSlice.SetBinError(bx,h2->GetBinError(bx,by));
      }else{
	HSlice.SetBinContent(bx,0);
	HSlice.SetBinError(bx,0);
      }          
    }     
    HSlice.SetBinContent(nbinsx,0);//do not use last bin in fit
    HSlice.SetBinError(nbinsx,0);

    FSlice.SetParLimits(0,.0,.9);
    FSlice.SetParLimits(1,-1,1);
    FSlice.SetParameter(0,0);//HSlice.Integral(),0);
    FSlice.SetParameter(1,0);
    
    FSlice.FixParameter(2,HSlice.GetMean());
    if(ngood<3||HSlice.Fit(&FSlice,"QCF","",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax())!=0) continue;
    
    chiperdf=FSlice.GetChisquare()/FSlice.GetNDF();    
    (*HEffChiX).SetBinContent(by,chiperdf);


    //fill the new histo
    for(Int_t bx=1;bx<=H2Interpolation->GetNbinsX();bx++){ 

      eff=FSlice.Eval(H2Interpolation->GetXaxis()->GetBinCenter(bx));
      H2Interpolation->SetBinContent(bx,by,eff);
      
      error=sqrt(FSlice.GetParError(0)*FSlice.GetParError(0)
      		 +(H2Interpolation->GetXaxis()->GetBinCenter(bx)-FSlice.GetParameter(2))
		 *(H2Interpolation->GetXaxis()->GetBinCenter(bx)-FSlice.GetParameter(2))
		 *FSlice.GetParError(1)
		 *FSlice.GetParError(1));
      
      H2Interpolation->SetBinError(bx,by,error);
    }
    
  }
  

  
  return H2Interpolation;
}




TH2F* Interpolate2DEfficiencyFitY(TH2F* h2,TH1F* HEffChiY){
  if(!h2){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2->GetName())+"InterY";
  
  Int_t nbinsx=h2->GetNbinsX();
  Int_t nbinsy=h2->GetNbinsY();  
  TH2F* H2Interpolation=new TH2F(name,name,
				 nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax(),
				 nbinsy*10,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  
  TH1F HSlice("HSlice","HSlice",nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());  
  TF1 FSlice("FSlice","[0]+[1]*(x-[2])",h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  FSlice.SetParLimits(0,.001,.9);
  FSlice.SetParLimits(1,-1,1);
  FSlice.FixParameter(2,2.46);
  //
  (*HEffChiY).SetNameTitle(name+"chi",name+"chi");
  (*HEffChiY).SetBins(nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());

  Float_t eff;
  Float_t error;
  Float_t chiperdf;
  Int_t ngood=0;
  for(Int_t bx=1;bx<=nbinsx;bx++){
    //fill the slice then fit
    ngood=0;
    for(Int_t by=1;by<=nbinsy;by++){
      if(h2->GetBinContent(bx,by)>0){
	ngood++;
	HSlice.SetBinContent(by,h2->GetBinContent(bx,by));	
	//if(bx==20)cout<<" "<<by<<" "<<h2->GetBinContent(bx,by)<<endl;
	HSlice.SetBinError(by,h2->GetBinError(bx,by));
      }else{
	HSlice.SetBinContent(by,0);
	HSlice.SetBinError(by,0);
      }            
    }
    FSlice.SetParLimits(0,.0,.9);
    FSlice.SetParLimits(1,-1,1);
    FSlice.SetParameter(0,0);//HSlice.Integral(),0);
    FSlice.SetParameter(1,0);

    FSlice.FixParameter(2,HSlice.GetMean());
    if(ngood<3||HSlice.Fit(&FSlice,"QCF","",h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax())!=0) continue;

    chiperdf=FSlice.GetChisquare()/FSlice.GetNDF();    
    (*HEffChiY).SetBinContent(bx,chiperdf);   
            
    //fill the new histo
    for(Int_t by=1;by<=H2Interpolation->GetNbinsY();by++){ 
      eff=FSlice.Eval(H2Interpolation->GetYaxis()->GetBinCenter(by));
      H2Interpolation->SetBinContent(bx,by,eff);
     
      error=sqrt(FSlice.GetParError(0)*FSlice.GetParError(0)
      		 +(H2Interpolation->GetYaxis()->GetBinCenter(by)-FSlice.GetParameter(2))
		 *(H2Interpolation->GetYaxis()->GetBinCenter(by)-FSlice.GetParameter(2))
		 *FSlice.GetParError(1)
		 *FSlice.GetParError(1));
      H2Interpolation->SetBinError(bx,by,error);//*sqrt(chiperdf));

     
    }
    
      
  }
  

  
  return H2Interpolation;
}

Int_t Compute2DEfficiencyFit(TH2F*H2Reco,TH2F*H2Gen){
  

//   //Show the efficiency shapes
//   TH1F htcorr("htcorr","htcorr",H2Gen->GetYaxis()->GetNbins(),
// 	      H2Gen->GetYaxis()->GetXmin(),
// 	      H2Gen->GetYaxis()->GetXmax());
//   htcorr.Sumw2();
//   TH1F ht("ht","ht",H2Gen->GetYaxis()->GetNbins(),
// 	  H2Gen->GetYaxis()->GetXmin(),
// 	  H2Gen->GetYaxis()->GetXmax());

//   ///
//   TF1 fitf("fitf","[0]+[1]*x",2.1,3.1);
//   fitf.SetParameters(1,.01);
//   TH1*htcorraddcl[4];  
//   //Float_t totalN=H2Reco->Integral();
//   Int_t cid=1;
//   Canvas.Clear();
//   Canvas.Divide(2,2); 
//   for(Int_t pb=1;pb<=4;pb++){      
//     Canvas.cd(cid);    
//     for(Int_t y=1;y<=ht.GetNbinsX();y++){
//       htcorr.SetBinContent(y,H2Reco->GetBinContent(pb,y));	                       
//       htcorr.SetBinError(y,H2Reco->GetBinError(pb,y));
//       ht.SetBinContent(y,H2Gen->GetBinContent(pb,y));
//       ht.SetBinError(y,H2Gen->GetBinError(pb,y));	
//     }
//     htcorraddcl[cid-1]=(TH1*)htcorr.Clone();
//     htcorraddcl[cid-1]->SetName(TString(htcorr.GetName())+(long)cid);
//     htcorraddcl[cid-1]->Divide(&ht);
//     htcorraddcl[cid-1]->Fit(&fitf);      
    
//     sprintf(textlabel,"p*=[%.1f,%.1f], slope=%.2f+-%.2f",H2Reco->GetXaxis()->GetBinCenter(pb)-H2Reco->GetXaxis()->GetBinWidth(1)/2,H2Reco->GetXaxis()->GetBinCenter(pb+1)-H2Reco->GetXaxis()->GetBinWidth(1)/2,fitf.GetParameter(1),fitf.GetParError(1));
//     htcorraddcl[cid-1]->SetTitle(textlabel);
//     htcorraddcl[cid-1]->GetYaxis()->SetRangeUser(0,htcorraddcl[cid-1]->GetMaximum()*1.2);
//     htcorraddcl[cid-1]->SetStats(0);
//     htcorraddcl[cid-1]->GetYaxis()->SetTitle("Efficiency");
//     htcorraddcl[cid-1]->GetXaxis()->SetTitle(DstarPiXLabel);
//     htcorraddcl[cid-1]->Draw("pe");
//     cid++;
//   }    

//   Canvas.Print(filename);


//   //compare y projection of 3-D rec mass to 3D projection of gen mass
//   TH1*hgenmass=H2Gen->ProjectionY("hgenmass",1,H2Gen->GetNbinsX(),"");  
//   TH1*hrecmass=H2Reco->ProjectionY("hrecmass",1,H2Gen->GetNbinsX(),"");   
//   hrecmass->SetTitle("");
//   hrecmass->SetStats(0);
//   hrecmass->GetYaxis()->SetTitle("Efficiency");
//   hrecmass->GetXaxis()->SetTitle(DstarPiXLabel);
//   // delete Plot1DEfficiency(hgenmass,hrecmass,1);


//   TH1*hgenpstar=H2Gen->ProjectionX("hgenpstar",1,H2Gen->GetNbinsY(),"");  
//   TH1*hrecpstar=H2Reco->ProjectionX("hrecpstar",1,H2Gen->GetNbinsY(),"");   
//   hrecpstar->SetTitle("");
//   hrecpstar->SetStats(0);
//   hrecpstar->GetYaxis()->SetTitle("Efficiency");
//   hrecpstar->GetXaxis()->SetTitle("p*(D*#pi)");
//   //delete Plot1DEfficiency(hgenpstar,hrecpstar,1);
  //---------------------------------------

 

  Float_t maxefferr=.15;  
  //calculate the efficiency
  TH2F*H2Eff=(TH2F*)H2Reco->Clone();
  H2Eff->Sumw2();
  H2Eff->Divide(H2Gen);
  for(Int_t i=1;i<=H2Eff->GetXaxis()->GetNbins();i++)
    for(Int_t j=1;j<=H2Eff->GetYaxis()->GetNbins();j++)
      if(H2Eff->GetBinError(i,j)/H2Eff->GetBinContent(i,j)>maxefferr){
	H2Eff->SetBinError(i,j,0);
	H2Eff->SetBinContent(i,j,0);
      }

 
  H2Eff->GetZaxis()->SetTitle("Efficiency");
  H2Eff->SetStats(0);
  H2Eff->SetTitle(""); 
 
  TH2F* H2EffErr;   
  H2EffErr=(TH2F*)H2Eff->Clone();    
  Fill2DRelativeEffErr(H2Eff,H2EffErr);
 
  H2EffErr->SetStats(0);
  H2EffErr->SetTitle("");    
  H2EffErr->GetZaxis()->SetTitle("Efficiency Error/ Efficiency");
   
  H2Eff->SetName(TString("H2EfficiencyRaw")+H2Reco->GetTitle());
  H2Eff->Write();    
  H2EffErr->SetName(TString("H2EfficiencyRawErr")+H2Reco->GetTitle());
  H2EffErr->Write();


 
  //--------------------------------
  //Fit the x direction in bins of y direction
  //--------------------------------
  TH2F* H2EffInterX;
  TH1F HEffChiX;
  H2EffInterX=Interpolate2DEfficiencyFitX(H2Eff,&HEffChiX);
  H2EffErr=(TH2F*)H2EffInterX->Clone();   			     
  Fill2DRelativeEffErr(H2EffInterX,H2EffErr);
  
  H2EffInterX->SetName(TString("H2EfficiencyXFit")+H2Reco->GetTitle());
  H2EffInterX->Write();  
  H2EffErr->SetName(TString("H2EfficiencyXFitErr")+H2Reco->GetTitle());
  H2EffErr->Write();

    
  //-----------------------------------
  //Fit the y direction in the new fine x direction
  //----------------------------------
  TH2F* H2EffInterY;
  TH1F HEffChiY;
  H2EffInterY=Interpolate2DEfficiencyFitY(H2EffInterX,&HEffChiY);   
  H2EffErr=(TH2F*)H2EffInterY->Clone();   		
  Fill2DRelativeEffErr(H2EffInterY,H2EffErr);

  H2EffInterY->SetName(TString("H2Efficiency")+H2Reco->GetTitle());
  H2EffInterY->Write();  
  H2EffErr->SetName(TString("H2EfficiencyErr")+H2Reco->GetTitle());
  H2EffErr->Write();


  delete H2EffInterX; 
  delete H2EffInterY;


  return 1;   
}






