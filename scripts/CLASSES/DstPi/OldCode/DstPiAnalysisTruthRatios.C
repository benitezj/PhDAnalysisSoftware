#include "DstPiAnalysis.h"
#include "../../FUNCTIONS/Fits.h"

Int_t DstPiAnalysis::DrawTruthRatios(){
  cout<<"Drawing the truth ratios"<<endl;
  if(!_TruthMatch){
    cout<<"Error: This operation is only MC."<<endl;
    return 0;
  }

  ///Open the reduced Ntuple 
  if(!OpenReducedNtuple(1))return 0; 

  //ps file
  filename=_OutputDir+"/TruthRatios.ps";
  Canvas.Print(filename+"[");
  
  Float_t Ntrue;
  Float_t Ntotal;
  Float_t D0TruthRatio=0;
  Float_t DstarTruthRatio=0;
  Float_t DstPiTruthRatio=0;


  TCut truecut;  
  sprintf(textlabel,"%.5f<dstpimdm&&dstpimdm<%.5f&&abs(dstpicharge)==1",database.GetDstPiMassMin(),database.GetDstPiMassMax());  
  TCut signalcut=textlabel;

  //create a clean copy
  TTree*Ntuple=ReducedNtuple->CopyTree(database.GetCut(2,100)+signalcut);
  if(Ntuple==NULL){
    cout<<"Ntuple is NULL, cannot do truth match ratios."<<endl;
    return 0;
  }
  
  truecut="d0mctrue==1"; 
  Ntrue=Ntuple->GetEntries(database.GetCut(2,100)+truecut+signalcut);
  Ntotal=Ntuple->GetEntries(database.GetCut(2,100)+signalcut);
  D0TruthRatio=Ntrue/Ntotal;

  truecut="dstarmctrue==1"; 
  Ntrue=Ntuple->GetEntries(database.GetCut(2,100)+truecut+signalcut);
  Ntotal=Ntuple->GetEntries(database.GetCut(2,100)+signalcut);
  DstarTruthRatio=Ntrue/Ntotal;

  truecut="dstpimctrue==1";  
  Ntrue=Ntuple->GetEntries(database.GetCut(2,100)+truecut+signalcut);
  Ntotal=Ntuple->GetEntries(database.GetCut(2,100)+signalcut);
  DstPiTruthRatio=Ntrue/Ntotal;


  TH1F HTruthRatioVsParticle("HTruthRatioVsParticle","",10,.5,10.5);
  HTruthRatioVsParticle.GetXaxis()->SetTitle("Particle Type");
  HTruthRatioVsParticle.GetYaxis()->SetTitle("TruthMatched/Reconstructed");
//   HTruthRatioVsParticle.SetBinContent(5,D0TruthRatio);
//   HTruthRatioVsParticle.SetBinContent(7,DstarTruthRatio); 
//   HTruthRatioVsParticle.SetBinContent(10,DstPiTruthRatio);  
  HTruthRatioVsParticle.GetYaxis()->SetRangeUser(0,1); 
  HTruthRatioVsParticle.SetBarWidth(.9);
  

  text.SetTextSize(.03);  
  Canvas.Clear();
  //HTruthRatioVsParticle.SetFillStyle(1);
  //HTruthRatioVsParticle.SetLineColor(1);
  //HTruthRatioVsParticle.Draw("b");
  HTruthRatioVsParticle.SetStats(kFALSE);
  HTruthRatioVsParticle.SetMarkerColor(1);
  HTruthRatioVsParticle.SetMarkerSize(.07);
  HTruthRatioVsParticle.Draw("p"); 
  cutline.SetLineColor(1);cutline.SetLineWidth(2);

  text.DrawText(5-.2,.1,"D0");
  cutline.DrawLine(5-.5,D0TruthRatio,5+.5,D0TruthRatio);
  text.DrawText(5-.35,D0TruthRatio*.9,TString("")+long(100*D0TruthRatio)+"."+long(1000*D0TruthRatio)%10+"%");

  text.DrawText(7-.2,.1,"D*");
  cutline.DrawLine(7-.5,DstarTruthRatio,7+.5,DstarTruthRatio);
  text.DrawText(7-.35,DstarTruthRatio*.9,TString("")+long(100*DstarTruthRatio)+"."+long(1000*DstarTruthRatio)%10+"%");

  text.DrawText(10-.2,.1,"DstPi");
  cutline.DrawLine(10-.5,DstPiTruthRatio,10+.5,DstPiTruthRatio);
  text.DrawText(10-.35,DstPiTruthRatio*.9,TString("")+long(100*DstPiTruthRatio)+"."+long(1000*DstPiTruthRatio)%10+"%");

  Canvas.Print(filename);

 
  TH1F HDstPiNoTrue("HDstPiNoTrue","",database.GetDstPiMassNbins(),database.GetDstPiMassMin(),database.GetDstPiMassMax());
  TH1F HDstPiTrue("HDstPiTrue","",database.GetDstPiMassNbins(),database.GetDstPiMassMin(),database.GetDstPiMassMax());
  TH1F HDstPiTrueBkg("HDstPiTrueBkg","",database.GetDstPiMassNbins(),database.GetDstPiMassMin(),database.GetDstPiMassMax());

  Ntuple->Draw("dstpimdm>>HDstPiNoTrue",database.GetCut(2,100)+signalcut);
  truecut="dstpimctrue==1"; 
  Ntuple->Draw("dstpimdm>>HDstPiTrue",database.GetCut(2,100)+truecut+signalcut);
  TCut truebkgcut="dstpimctruebad!=1"; 
  Ntuple->Draw("dstpimdm>>HDstPiTrueBkg",database.GetCut(2,100)+truebkgcut+signalcut);

  SetHistoXYLabels(&HDstPiNoTrue,"m(D*#pi)","(GeV/c^{2})");
  SetHistoXYLabels(&HDstPiTrueBkg,"m(D*#pi)","(GeV/c^{2})");
  Canvas.Clear(); 
  HDstPiNoTrue.SetStats(0);
  HDstPiNoTrue.Draw("pe");
  HDstPiTrueBkg.SetStats(0);
  HDstPiTrueBkg.Draw("same");
  Canvas.Print(filename);
  
  //fit the untruthmatched signal
  Float_t pars[7]={100,2.422,.02,1,2,3,4};
  TF1* f1=FitHistogramBreitWignerPolN(&HDstPiNoTrue,2.3,2.6,2,pars);
  TF1* f2=FitHistogramBreitWignerPolN(&HDstPiTrueBkg,2.3,2.6,2,pars);
  Canvas.Clear();  
  TF1 BW("BW","[0]*([2]/(2*3.14159265358979312))/((x-[1])**2+.25*[2]*[2])",database.GetDstPiMassMin(),database.GetDstPiMassMax());
  BW.SetParameters(f1->GetParameter(0),f1->GetParameter(1),f1->GetParameter(2));
  Ntotal=BW.Integral(database.GetDstPiMassMin(),database.GetDstPiMassMax())*HDstPiNoTrue.GetBinWidth(1);
  BW.SetParameters(f2->GetParameter(0),f2->GetParameter(1),f2->GetParameter(2));
  Ntrue=BW.Integral(database.GetDstPiMassMin(),database.GetDstPiMassMax())*HDstPiNoTrue.GetBinWidth(1);
  
  Canvas.Clear(); 
  f1->SetNpx(10000);
  HDstPiNoTrue.SetStats(0);
  HDstPiNoTrue.Draw();
  f2->SetNpx(10000);
  HDstPiTrueBkg.SetStats(0);
  HDstPiTrueBkg.Draw("same");
  sprintf(textlabel,"Ratio=%.3f",Ntrue/Ntotal);
  text.DrawTextNDC(.6,.3,textlabel);
  Canvas.Print(filename);


  Canvas.Clear(); 
  HDstPiTrueBkg.SetStats(0);
  HDstPiTrueBkg.GetYaxis()->SetRangeUser(0,HDstPiTrueBkg.GetMaximum());
  HDstPiTrueBkg.Draw("pe");
  Canvas.Print(filename);

  cout<<Ntrue/Ntotal<<endl;


  Canvas.Print(filename+"]");
  return 1;
}


