#include "DstPiAnalysis.h"
#include "../../FUNCTIONS/CalculateIntegral.C"

#define PstarMin 3.0
#define PstarMax 5.0
#define NPstarBins 4
#define CosMin -1.
#define CosMax .5
#define NCosHelBins 3


Int_t DstPiAnalysis::PlotDMVsPstarCosHel(Float_t dmmin,Float_t dmmax,Int_t nbins){

  cout<<"Going to plot deltaM vs. Pstar and CosHel"<<endl;

  //ps file
  filename=_OutputDir+"/DMVsPstarCosHel.ps";
  Canvas.Print(filename+"[");

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(2)!=1) return 0;
    
  
 //  //Binning
//   Float_t dmmin=database.GetDstPiDeltaMRangeMin();
//   Float_t dmmax=database.GetDstPiDeltaMRangeMax();
//   Int_t nbins=database.GetDstPiDeltaMRangeNbins();


  ///Make a plot for all the p* and coshel combined
  TH1F HDMAll("HDMAll","HDMAll",nbins,dmmin,dmmax);  
  ReducedNtuple->Draw("dstpimdm>>HDMAll","","");

  Canvas.Clear();
  HDMAll.Draw();
  Canvas.Print(filename);
  
  //Create histograms for each p* slice   
  TH1F* HDM[NCosHelBins][NPstarBins];
  Float_t pstarstep=(PstarMax-PstarMin)/NPstarBins;
  Float_t cosstep=(CosMax-CosMin)/NCosHelBins;
  TString title;
  TCut pcut;
  TCut coscut;

 
  for(Int_t c=0;c<NCosHelBins;c++){    
    sprintf(textlabel,"%.2f<dstarcostheta&&dstarcostheta<%.2f",-1.+c*cosstep,-1.+(c+1)*cosstep);    
    coscut=textlabel;
    for(Int_t p=0;p<NPstarBins;p++){         
    
      sprintf(textlabel,"%.2f<dstpipstar&&dstpipstar<%.2f",PstarMin+p*pstarstep,PstarMin+(p+1)*pstarstep);
      pcut=textlabel;
    
      title=TString("HDM_")+(const char*)coscut+"_"+(const char*)pcut;  
      HDM[c][p]=new TH1F(title,"",nbins,dmmin,dmmax);  
      ReducedNtuple->Draw(TString("dstpimdm>>")+title,coscut+pcut);
      SetHistoXYLabels(HDM[c][p],database.GetDstPiDeltaMLabel(),"GeV/c^{2}");

      Canvas.Clear();
      HDM[c][p]->Draw("pe");
      Canvas.Print(filename);
      
      
    }
  } 
  //summary slide
  Canvas.Clear();
  Canvas.Divide(NPstarBins,NCosHelBins);
  Int_t cid=0;
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      cid++;
      Canvas.cd(cid);
      HDM[c][p]->SetLineWidth(1);
      HDM[c][p]->SetMarkerSize(.2);
      HDM[c][p]->GetYaxis()->SetTitle(" ");
      HDM[c][p]->GetYaxis()->SetLabelSize(.05);
      HDM[c][p]->GetXaxis()->SetLabelSize(.05);
      HDM[c][p]->GetYaxis()->SetNoExponent(0);
      HDM[c][p]->Draw("pe");  
    }
  Canvas.Print(filename);
 



  //Inclusive cuts  
  TH1F* HDMInclusive[NCosHelBins][NPstarBins];
  for(Int_t c=0;c<NCosHelBins;c++){    
    sprintf(textlabel,"dstarcostheta<%.2f",-1.+(c+1)*cosstep);    
    coscut=textlabel;
    for(Int_t p=0;p<NPstarBins;p++){         
    
      sprintf(textlabel,"%.2f<dstpipstar",PstarMin+p*pstarstep);
      pcut=textlabel;
    
      title=TString("HDMInclusive_")+(const char*)coscut+"_"+(const char*)pcut;  
      HDMInclusive[c][p]=new TH1F(title,"",nbins,dmmin,dmmax);  
      ReducedNtuple->Draw(TString("dstpimdm>>")+title,coscut+pcut);
      SetHistoXYLabels(HDMInclusive[c][p],database.GetDstPiDeltaMLabel(),"GeV/c^{2}");
      Canvas.Clear();
      HDMInclusive[c][p]->Draw("pe");
      Canvas.Print(filename);           
    }
  }
  //make a summary plot
  Canvas.Clear();
  Canvas.Divide(NPstarBins,NCosHelBins);
  cid=0;
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      cid++;
      Canvas.cd(cid);
      HDMInclusive[c][p]->SetLineWidth(1);
      HDMInclusive[c][p]->SetMarkerSize(.2);
      HDMInclusive[c][p]->GetYaxis()->SetTitle(" ");
      HDMInclusive[c][p]->GetYaxis()->SetLabelSize(.05);
      HDMInclusive[c][p]->GetXaxis()->SetLabelSize(.05);
      HDMInclusive[c][p]->GetYaxis()->SetNoExponent(0);
      HDMInclusive[c][p]->Draw("pe");  
    }
  Canvas.Print(filename);


  //clean up  
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      delete HDM[c][p];
      delete HDMInclusive[c][p];
    }

  Canvas.Print(filename+"]");  
  
  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}

Int_t DstPiAnalysis::PlotMassVsPstar(Float_t massmin,Float_t massmax,Int_t nbins,Float_t min,Float_t max,Int_t Nslices){

  cout<<"Going to plot M vs. Pstar "<<endl;

  //ps file
  filename=_OutputDir+"/MassVsPstar.ps";
  Canvas.Print(filename+"[");

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(2)!=1) return 0;
    
  //Create histograms for each p* slice   
  TH1F* HMASS[100];
  Float_t step=(max-min)/Nslices;
  TString title;
  TCut cut;
  Canvas.Clear();
  Canvas.Divide(2,Nslices/2);
  for(Int_t p=0;p<Nslices;p++){         
    Canvas.cd(p+1);
    
    sprintf(textlabel,"%.2f<dstpipstar&&dstpipstar<%.2f",min+p*step,min+(p+1)*step);
    cut=textlabel;
    
    title=TString("HMASS_")+(const char*)cut;  
    HMASS[p]=new TH1F(title,"",nbins,massmin,massmax);  
    ReducedNtuple->Draw(TString("dstpimdm>>")+title,cut);
    //SetHistoXYLabels(HMASS[p],"m(D*#pi)","GeV");
       

    Canvas.GetPad(p+1)->Clear();
    HMASS[p]->SetLineWidth(1);
    HMASS[p]->SetMarkerSize(.6);
    HMASS[p]->GetYaxis()->SetNoExponent(0);
    HMASS[p]->GetYaxis()->SetLabelSize(.08);
    HMASS[p]->GetYaxis()->SetNdivisions(5);
    HMASS[p]->GetXaxis()->SetTitle("");
    HMASS[p]->GetXaxis()->SetLabelSize(.09);
    HMASS[p]->GetXaxis()->SetNdivisions(5); 
    HMASS[p]->SetStats(0);
    HMASS[p]->Draw("pe");  
    text.SetTextSize(.15);
    text.DrawTextNDC(.7,.7,TString("")+(long)(p+1));
    
  }
 
  Canvas.Print(filename);
 

  //clean up  
  for(Int_t p=0;p<Nslices;p++){
    delete HMASS[p];
  }
  
  Canvas.Print(filename+"]");  
  
  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}




Int_t DstPiAnalysis::PlotMassVsCosDstar(Float_t massmin,Float_t massmax,Int_t nbins,Float_t min,Float_t max,Int_t Nslices){

  cout<<"Going to plot M vs. CosDstar "<<endl;

  //ps file
  filename=_OutputDir+"/MassVsCosDstar.ps";
  Canvas.Print(filename+"[");

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(2)!=1) return 0;
    
  //Create histograms for each p* slice   
  TH1F* HMASS[100];
  Float_t step=(max-min)/Nslices;
  TString title;
  TCut cut;
  Canvas.Clear();
  Canvas.Divide(2,Nslices/2);
  for(Int_t p=0;p<Nslices;p++){         
    Canvas.cd(p+1);
    
    sprintf(textlabel,"%.2f<dstarcostheta&&dstarcostheta<%.2f",min+p*step,min+(p+1)*step);
    cut=textlabel;
    
    title=TString("HMASS_")+(const char*)cut;  
    HMASS[p]=new TH1F(title,"",nbins,massmin,massmax);  
    ReducedNtuple->Draw(TString("dstpimdm>>")+title,cut);
    //SetHistoXYLabels(HMASS[p],"m(D*#pi)","GeV");
       

    Canvas.GetPad(p+1)->Clear();
    HMASS[p]->SetLineWidth(1);
    HMASS[p]->SetMarkerSize(.6);
    HMASS[p]->GetYaxis()->SetNoExponent(0);
    HMASS[p]->GetYaxis()->SetLabelSize(.08);
    HMASS[p]->GetYaxis()->SetNdivisions(5);
    HMASS[p]->GetXaxis()->SetTitle("");
    HMASS[p]->GetXaxis()->SetLabelSize(.09);
    HMASS[p]->GetXaxis()->SetNdivisions(5); 
    HMASS[p]->SetStats(0);
    HMASS[p]->Draw("pe");  
    text.SetTextSize(.15);
    text.DrawTextNDC(.7,.7,TString("")+(long)(p+1));
    
  }
 
  Canvas.Print(filename);
 

  //clean up  
  for(Int_t p=0;p<Nslices;p++){
    delete HMASS[p];
  }
  
  Canvas.Print(filename+"]");  
  
  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}


Int_t DstPiAnalysis::PlotMassVsHelicity(Float_t massmin,Float_t massmax,Int_t nbins,Float_t min,Float_t max,Int_t Nslices){

  cout<<"Going to plot M vs. Helicity "<<endl;

  //ps file
  filename=_OutputDir+"/MassVsHelicity.ps";
  Canvas.Print(filename+"[");

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(2)!=1) return 0;
    
  //Create histograms for each p* slice   
  TH1F* HMASS[100];
  Float_t step=(max-min)/Nslices;
  TString title;
  TCut cut;
  Canvas.Clear();
  Canvas.Divide(2,Nslices/2);
  for(Int_t p=0;p<Nslices;p++){         
    Canvas.cd(p+1);
    
    sprintf(textlabel,"%.2f<dstarhelicity&&dstarhelicity<%.2f",min+p*step,min+(p+1)*step);
    cut=textlabel;
    
    title=TString("HMASS_")+(const char*)cut;  
    HMASS[p]=new TH1F(title,"",nbins,massmin,massmax);  
    ReducedNtuple->Draw(TString("dstpimdm>>")+title,cut);
    //SetHistoXYLabels(HMASS[p],"m(D*#pi)","GeV");
       

    Canvas.GetPad(p+1)->Clear();
    HMASS[p]->SetLineWidth(1);
    HMASS[p]->SetMarkerSize(.6);
    HMASS[p]->GetYaxis()->SetNoExponent(0);
    HMASS[p]->GetYaxis()->SetLabelSize(.08);
    HMASS[p]->GetYaxis()->SetNdivisions(5);
    HMASS[p]->GetXaxis()->SetTitle("");
    HMASS[p]->GetXaxis()->SetLabelSize(.09);
    HMASS[p]->GetXaxis()->SetNdivisions(5); 
    HMASS[p]->SetStats(0);
    HMASS[p]->Draw("pe");  
    text.SetTextSize(.15);
    text.DrawTextNDC(.7,.7,TString("")+(long)(p+1));
    
  }
 
  Canvas.Print(filename);
 

  //clean up  
  for(Int_t p=0;p<Nslices;p++){
    delete HMASS[p];
  }
  
  Canvas.Print(filename+"]");  
  
  if(CloseReducedNtuple()!=1) return 0;
  return 1;
}



/*
Int_t DstPiAnalysis::PlotDMVsPstarCosHel(){

  cout<<"Going to plot deltaM vs. Pstar and CosHel"<<endl;

  //ps file
  filename=_OutputDir+"/DMVsPstarCosHel.ps";
  Canvas.Print(filename+"[");

  
  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(1)!=1) return 0;//need full Ntuple to compare with WS
    
  //apply some cuts 
  TCut TotalCut=database.GetCut(2,100);
  
  //Binning
  Float_t dmmin=database.GetDstPiDeltaMRangeMin();
  Float_t dmmax=database.GetDstPiDeltaMRangeMax();
  Int_t nbins=database.GetDstPiDeltaMRangeNbins();

  //------------------------------
  //define the data to be fitted
  //------------------------------
  TCut rscut="abs(dstpicharge)==1";//choose right sign
  TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(TotalCut+rscut);
  ReducedNtupleCopy->SetBranchStatus("*",0);
  ReducedNtupleCopy->SetBranchStatus("dstpideltam",1); 
  ReducedNtupleCopy->SetBranchStatus("dstpipstar",1); 
  ReducedNtupleCopy->SetBranchStatus("dstarcostheta",1); 

  //the Wrong sign ntuple
  TCut wscut="abs(dstpicharge)==2";//choose right sign 
  TTree* ReducedNtupleCopyWS=ReducedNtuple->CopyTree(TotalCut+wscut);
  ReducedNtupleCopyWS->SetBranchStatus("*",0);  
  ReducedNtupleCopyWS->SetBranchStatus("dstpideltam",1); 
  ReducedNtupleCopyWS->SetBranchStatus("dstpipstar",1); 
  ReducedNtupleCopyWS->SetBranchStatus("dstarcostheta",1); 

  if(CloseReducedNtuple()!=1) return 0;
  

  ///Make a plot for all the p* and coshel combined
  TH1F HDMAll("HDMAll","HDMAll",nbins,dmmin,dmmax);  
  ReducedNtupleCopy->Draw("dstpideltam>>HDMAll","","");

  TH1F HDMAllWS("HDMAllWS","HDMAllWS",nbins,dmmin,dmmax);  
  ReducedNtupleCopyWS->Draw("dstpideltam>>HDMAllWS","","");

  Float_t scalemin=.150;
  Float_t scalemax=.250;
  Float_t ScaleAll=CalculateIntegral(scalemin,scalemax,&HDMAll)/CalculateIntegral(scalemin,scalemax,&HDMAllWS);
  HDMAllWS.Scale(ScaleAll);
  Canvas.Clear();
  HDMAll.Draw();
  HDMAllWS.Draw("same");
  Canvas.Print(filename);
  
  //Create histograms for each p* slice   
  TH1F* HDM[NCosHelBins][NPstarBins];
  Float_t pstarstep=(PstarMax-PstarMin)/NPstarBins;
  Float_t cosstep=(CosMax-CosMin)/NCosHelBins;
  TString title;
  TCut pcut;
  TCut coscut;
 
  TH1F* HDMWS[NCosHelBins][NPstarBins];
  Float_t Scale[NCosHelBins][NPstarBins];
 
  for(Int_t c=0;c<NCosHelBins;c++){    
    sprintf(textlabel,"%.2f<dstarcostheta&&dstarcostheta<%.2f",-1.+c*cosstep,-1.+(c+1)*cosstep);    
    coscut=textlabel;
    for(Int_t p=0;p<NPstarBins;p++){         
    
      sprintf(textlabel,"%.2f<dstpipstar&&dstpipstar<%.2f",PstarMin+p*pstarstep,PstarMin+(p+1)*pstarstep);
      pcut=textlabel;
    
      title=TString("HDM_")+(const char*)coscut+"_"+(const char*)pcut;  
      HDM[c][p]=new TH1F(title,title,nbins,dmmin,dmmax);  
      ReducedNtupleCopy->Draw(TString("dstpideltam>>")+title,coscut+pcut);
      SetHistoXYLabels(HDM[c][p],database.GetDstPiDeltaMLabel(),"GeV/c^{2}");

      title=TString("HDMWS_")+(const char*)coscut+"_"+(const char*)pcut;
      HDMWS[c][p]=new TH1F(title,title,nbins,dmmin,dmmax);  
      ReducedNtupleCopyWS->Draw(TString("dstpideltam>>")+title,coscut+pcut);
      Scale[c][p]=CalculateIntegral(scalemin,scalemax,HDM[c][p])/CalculateIntegral(scalemin,scalemax,HDMWS[c][p]);
      HDMWS[c][p]->Scale(Scale[c][p]);

      Canvas.Clear();
      HDM[c][p]->Draw("pe");
      HDMWS[c][p]->Draw("same");
      Canvas.Print(filename);
      
      
    }
  } 
  //summary slide
  Canvas.Clear();
  Canvas.Divide(NPstarBins,NCosHelBins);
  Int_t cid=0;
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      cid++;
      Canvas.cd(cid);
      HDM[c][p]->SetLineWidth(1);
      HDM[c][p]->SetMarkerSize(.2);
      HDM[c][p]->GetYaxis()->SetTitle(" ");
      HDM[c][p]->GetYaxis()->SetLabelSize(.05);
      HDM[c][p]->GetXaxis()->SetLabelSize(.05);
      HDM[c][p]->GetYaxis()->SetNoExponent(0);
      HDM[c][p]->Draw("pe");  
    }
  Canvas.Print(filename);
 



  //Inclusive cuts  
  TH1F* HDMInclusive[NCosHelBins][NPstarBins];
  for(Int_t c=0;c<NCosHelBins;c++){    
    sprintf(textlabel,"dstarcostheta<%.2f",-1.+(c+1)*cosstep);    
    coscut=textlabel;
    for(Int_t p=0;p<NPstarBins;p++){         
    
      sprintf(textlabel,"%.2f<dstpipstar",PstarMin+p*pstarstep);
      pcut=textlabel;
    
      title=TString("HDMInclusive_")+(const char*)coscut+"_"+(const char*)pcut;  
      HDMInclusive[c][p]=new TH1F(title,title,nbins,dmmin,dmmax);  
      ReducedNtupleCopy->Draw(TString("dstpideltam>>")+title,coscut+pcut);
      SetHistoXYLabels(HDMInclusive[c][p],database.GetDstPiDeltaMLabel(),"GeV/c^{2}");
      Canvas.Clear();
      HDMInclusive[c][p]->Draw("pe");
      Canvas.Print(filename);           
    }
  }
  //make a summary plot
  Canvas.Clear();
  Canvas.Divide(NPstarBins,NCosHelBins);
  cid=0;
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      cid++;
      Canvas.cd(cid);
      HDMInclusive[c][p]->SetLineWidth(1);
      HDMInclusive[c][p]->SetMarkerSize(.2);
      HDMInclusive[c][p]->GetYaxis()->SetTitle(" ");
      HDMInclusive[c][p]->GetYaxis()->SetLabelSize(.05);
      HDMInclusive[c][p]->GetXaxis()->SetLabelSize(.05);
      HDMInclusive[c][p]->GetYaxis()->SetNoExponent(0);
      HDMInclusive[c][p]->Draw("pe");  
    }
  Canvas.Print(filename);


  //clean up  
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      delete HDM[c][p];
      delete HDMWS[c][p];
      delete HDMInclusive[c][p];
    }

  delete ReducedNtupleCopyWS;
  delete ReducedNtupleCopy;

  Canvas.Print(filename+"]");  
  
  return 1;
}
*/
