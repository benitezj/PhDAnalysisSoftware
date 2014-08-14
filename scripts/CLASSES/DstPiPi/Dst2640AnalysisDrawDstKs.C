
Int_t Dst2640Analysis::DrawDstKs(){

  //-------------------
  //create a canvas
  //-------------------
  //  TCanvas Canvas("Canvas","Canvas");
  //TString filename;
  filename=_OutputDir+"/DstKsMass.ps";
  Canvas.Print(filename+"[");

  if(!OpenReducedNtuple()){
    cout<<"Failed to open ntuple"<<endl;
    return 0;
  }
  
  sprintf(textlabel,"abs(pipimass-%.5f)<%.5f",K0PDGMass,.01);
  TCut kscut=textlabel;
  TH1F HDstKsMass("HDstKsMass","HDstKsMass",200,2.0,3.0);
  ReducedNtuple->Draw("dst2640deltam+2.01>>HDstKsMass",database.GetCut(1,2)+database.GetCut(3,7)+database.GetCut(9,100)+kscut);

  Canvas.Clear();
  HDstKsMass.Draw("pe");
  Canvas.Print(filename);

  TH1F HKsMass("HKsMass","HKsMass",1000,0,1.);
  ReducedNtuple->Draw("pipimass>>HKsMass",database.GetCut(1,2)+database.GetCut(3,7)+database.GetCut(9,100));

  Canvas.Clear();
  HKsMass.Draw("pe");
  Canvas.Print(filename);
  

  if(!Close()){
    cout<<"Failed to close file"<<endl;
    return 0;
  }

  Canvas.Print(filename+"]");  
  ////////done saving plots
 
  
 
  return 1;
}



