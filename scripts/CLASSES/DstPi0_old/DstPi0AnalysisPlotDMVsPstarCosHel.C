#include "DstPi0Analysis.h"
#include "../../FUNCTIONS/CalculateIntegral.C"

#define PstarMin 3.0
#define PstarMax 5.0
#define NPstarBins 4
#define CosMin -1.
#define CosMax .5
#define NCosHelBins 3


Int_t DstPi0Analysis::PlotDMVsPstarCosHel(){

  cout<<"Going to plot deltaM vs. Pstar and CosHel"<<endl;

  //ps file
  TString filename=_OutputDir+"/DMVsPstarCosHel.ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");

  
  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  

  ///Open the reduced Ntuple
  if(OpenReducedNtuple("Reduced.root")!=1) return 0;
    
  //apply some cuts 
  TCut TotalCut=database.GetCut(1,100);
  
  //Binning
  Float_t dmmin=database.GetDstPi0DeltaMRangeMin();
  Float_t dmmax=database.GetDstPi0DeltaMRangeMax();
  Int_t nbins=database.GetDstPi0DeltaMRangeNbins()/4;

  //------------------------------
  //
  //------------------------------
  TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(TotalCut);
  ReducedNtupleCopy->SetBranchStatus("*",0);
  ReducedNtupleCopy->SetBranchStatus("dstpideltam",1); 
  ReducedNtupleCopy->SetBranchStatus("dstpipstar",1); 
  ReducedNtupleCopy->SetBranchStatus("dstarcostheta",1); 

  //close the file 
  if(CloseReducedNtuple()!=1) return 0;
  
  ///Make a plot for all the p* and coshel combined
  TH1F HDMAll("HDMAll","HDMAll",nbins,dmmin,dmmax);  
  ReducedNtupleCopy->Draw("dstpideltam>>HDMAll","","");
  SetHistoXYLabels(&HDMAll,database.GetDstPiDeltaMLabel(),"GeV/c^{2}");  
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
      HDM[c][p]=new TH1F(title,title,nbins,dmmin,dmmax);  
      ReducedNtupleCopy->Draw(TString("dstpideltam>>")+title,coscut+pcut);
      SetHistoXYLabels(HDM[c][p],database.GetDstPiDeltaMLabel(),"GeV/c^{2}");
      Canvas.Clear();
      HDM[c][p]->Draw("pe");
      Canvas.Print(filename);           
    }
  }
  //make a summary plot
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

  //clean up  
  for(Int_t c=0;c<NCosHelBins;c++) 
    for(Int_t p=0;p<NPstarBins;p++){
      delete HDM[c][p];
    }


  
  //Create histograms for each p* and coshel cut   
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
      delete HDMInclusive[c][p];
    }
  


  delete ReducedNtupleCopy;
    

  Canvas.Print(filename+"]");  
  
  return 1;
}
