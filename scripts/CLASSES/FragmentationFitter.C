#include "FragmentationFitter.h"

ClassImp(FragmentationFitter);
FragmentationFitter::FragmentationFitter():
  TNamed("FragmentationFitter","FragmentationFitter"),
  Histo(0),
  OutPutDir(".")
{
 
}

Int_t FragmentationFitter::Init(TH1* histo,TString Dir){

  if(!histo){
    cout<<" FragmentationFitter Warning: no histo"<<endl;
  }
  Histo=histo;
  OutPutDir=Dir;

  ModelName[0]="Peterson";
  Formula[0]="([2]*x<1)*[0]/([2]*x*(1.-1./([2]*x)-[1]/(1-[2]*x))**2)";
  ParName[0][1]="epsilon";ParName[0][3]="0";
  
  ModelName[1]="Kart";
  Formula[1]="([2]*x<1)*[0]*(([2]*x)**[1])*(1-[2]*x)";
  ParName[1][1]="alpha";ParName[1][3]="0";

  ModelName[2]="Collins";
  Formula[2]="([2]*x<1)*[0]*(1+([2]*x)**2)*(((1-([2]*x))/([2]*x) + (2-([2]*x))*[1]/(1-([2]*x))) / (1.-1./([2]*x)-[1]/(1-[2]*x))**2)";
  ParName[2][1]="epsilon";ParName[2][3]="0";

  ModelName[3]="Colangelo";
  Formula[3]="([2]*x<1)*[0]*(([2]*x)**[1])*(abs((1-[2]*x))**[3])";
  ParName[3][1]="alpha";ParName[3][3]="beta";

  ModelName[4]="Bowler";
  Formula[4]="([2]*x<1)*[0]*(abs(1-[2]*x)**[1])*(([2]*x)**(-(1+[3])))*exp(-[3]/([2]*x))";
  ParName[4][1]="alpha";ParName[4][3]="bm";

  
  if(!inputstream.OpenFitPars(OutPutDir+"/FragmentationPars.txt")){cout<<"bad config filename"<<endl; return 0;}
  
  //Peterson Model
  for(Int_t i=0;i<NMODELS;i++){
    ParName[i][0]="Norm";
    ParName[i][2]="scale";

    FModel[i]=new TF1((const char*)ModelName[i],(const char*)Formula[i],
		      (Histo ? Histo->GetXaxis()->GetXmin():inputstream.GetFitPar("histo","min"),
		      (Histo ? Histo->GetXaxis()->GetXmax():inputstream.GetFitPar("histo","max")))
		      );
    for(Int_t j=0;j<4;j++)
      if(ParName[i][j]!="0"){
	 FModel[i]->SetParName(j,ParName[i][j]); 
	 FModel[i]->SetParameter(ParName[i][j],inputstream.GetFitPar(ModelName[i],ParName[i][j]));
      }

    FModel[i]->SetParLimits(0,0,1e8);
    FModel[i]->SetParLimits(1,0,1);
    FModel[i]->SetParLimits(2,.1,10);
    if(ModelName[i]=="Colangelo")
      FModel[i]->SetParLimits(3,.1,2);
    if(ModelName[i]=="Bowler")
      FModel[i]->SetParLimits(3,.001,100);

    //Fix the scale parameter now that using correct p*_max
    //FModel[i]->FixParameter(2,1.);

    
  }

  inputstream.CloseFitPars();

  return 1;
}


FragmentationFitter::~FragmentationFitter(){
 
  for(Int_t i=0;i<NMODELS;i++)
    if(FModel[i]) delete FModel[i];

}


Int_t FragmentationFitter::Fit(){
  if(!Histo){
    cout<<" No histo "<<endl;
    return 0;
  }

  for(Int_t i=0;i<NMODELS;i++){
    if(!FModel[i])return 0;
    //Do not use "L" because it is eff weighted
    FitStatus[i]=Histo->Fit(FModel[i],"QNI","",Histo->GetXaxis()->GetXmin(),Histo->GetXaxis()->GetXmax());
  }
    
  return 1;
}

Int_t FragmentationFitter::Save(){
  if(!Histo){
    cout<<" No histo "<<endl;
    return 0;
  }
  
 
  TString filename=OutPutDir+"/FragmentaionFits.ps";
  TCanvas Canvas;
  Canvas.Print(filename+"[");

  Histo->SetMarkerSize(1.5);
  Histo->SetMarkerColor(1);
  Histo->SetLineColor(1);

  Histo->GetYaxis()->SetRangeUser(0,Histo->GetMaximum()*1.1);
  Histo->SetStats(0);
  Histo->SetTitle("");
  


  Canvas.Clear();
  Histo->GetYaxis()->SetRangeUser(0,Histo->GetMaximum()*1.1);
  Histo->Draw("pe"); 
  Canvas.Print(filename);
  

  //Chisquare vs. Model
  
  


  //---------------------------------
  //save the parameters to a file 
  //----------------------------------
  ofstream ParameterOutput; 
  ParameterOutput.open((const char*)(OutPutDir+"/FragmentationPars.txt"));   

  ParameterOutput<<"histo"<<" integral "<<Histo->Integral()<<endl;
  ParameterOutput<<"histo"<<" min "<<Histo->GetXaxis()->GetXmin()<<endl;
  ParameterOutput<<"histo"<<" max "<<Histo->GetXaxis()->GetXmax()<<endl;
  ParameterOutput<<"histo"<<" binwidth "<<Histo->GetXaxis()->GetBinWidth(1)<<endl;


  for(Int_t i=0;i<NMODELS;i++){
 
    for(Int_t j=0;j<4;j++)
      if(TString(ParName[i][j])!=TString("0")){
	ParameterOutput<<ModelName[i]<<" "<<FModel[i]->GetParName(j)<<" "<<FModel[i]->GetParameter(j)<<endl;
	ParameterOutput<<ModelName[i]<<" "<<FModel[i]->GetParName(j)<<"error "<<FModel[i]->GetParError(j)<<endl;
      }

    ParameterOutput<<ModelName[i]<<" integral "<<FModel[i]->Integral(0,Histo->GetXaxis()->GetXmax())/Histo->GetXaxis()->GetBinWidth(1)<<endl;    
    ParameterOutput<<ModelName[i]<<" chisquare "<<FModel[i]->GetChisquare()<<endl;
    ParameterOutput<<ModelName[i]<<" ndf "<<FModel[i]->GetNDF()<<endl;
    ParameterOutput<<ModelName[i]<<" status "<<FitStatus[i]<<endl;

//     cout<<ModelName[i]<<endl;
    
//     for(Int_t j=0;j<4;j++)
//       if(FModel[i]->GetParName(j)!="0")
// 	cout<<FModel[i]->GetParName(j)<<"= "<<FModel[i]->GetParameter(j)<<"+-"<<FModel[i]->GetParError(j)<<endl;
			    
//     cout<<" Integral="<<FModel[i]->Integral(0,Histo->GetXaxis()->GetXmax())/Histo->GetXaxis()->GetBinWidth(1)<<endl
// 	<<" RawIntegral="<<Histo->Integral()<<endl
// 	<<" chisq/df="<<FModel[i]->GetChisquare()/FModel[i]->GetNDF()<<endl
// 	<<" status="<<FitStatus[i]<<endl
// 	<<endl; 

    Canvas.Clear();
    Histo->Draw("pe"); 
    FModel[i]->Draw("lsame");
    Canvas.Print(filename);
  
  }

  ParameterOutput<<"END"<<endl;
  ParameterOutput.close();
  ParameterOutput.clear();  


  Canvas.Print(filename+"]");

  return 1;
}


Int_t FragmentationFitter::CreateTable(){
 
  if(!inputstream.OpenFitPars(OutPutDir+"/FragmentationPars.txt")){cout<<"bad config filename"<<endl; return 0;}

  ofstream ParameterOutput; 
  ParameterOutput.open((const char*)(OutPutDir+"/FragmentationTable.txt"));
  cout<<(const char*)(OutPutDir+"/FragmentationTable.txt")<< " has been created"<<endl;
  ParameterOutput<<"Model && \\alpha && \\beta &&{{\\chi2^2}\\over{df}} \\hline\\hline"<<endl;

  for(Int_t i=0;i<NMODELS;i++){
    ParameterOutput<<ModelName[i]<<" && ";
       
    ParameterOutput<<inputstream.GetFitPar(ModelName[i],ParName[i][1])<<"\\pm"<<inputstream.GetFitPar(ModelName[i],ParName[i][1]+TString("error"))<<" && ";
    if(ParName[i][3]!="0")
      ParameterOutput<<inputstream.GetFitPar(ModelName[i],ParName[i][3])<<"\\pm"<<inputstream.GetFitPar(ModelName[i],ParName[i][3]+TString("error"))<<" && ";
    else
      ParameterOutput<<"--"<<" && ";
      
    ParameterOutput<<inputstream.GetFitPar(ModelName[i],"chisquare")/inputstream.GetFitPar(ModelName[i],"ndf")<<" && ";
    ParameterOutput<<inputstream.GetFitPar(ModelName[i],"integral")<<" \\hline"<<endl;
  } 


  ParameterOutput.close();
  ParameterOutput.clear();  


  inputstream.CloseFitPars();
  return 1;
}
