void PlotD0Bkg(){
  gROOT->cd();
  TTree* Ntuple=(TTree*)_file0->Get("Ntuple");
  TCanvas C("C");
  C.Divide(2,1);

  //Draw the D*->D0 gam 
  C.cd(1);
  Ntuple->Draw("dsmass>>Hb(50,1.82,2.12)","evthasD0gam&&!evthassignal");
  Hb->GetYaxis()->SetRangeUser(0,Hb->GetBinContent(Hb->GetMaximumBin())*1.2);
  Hb->GetXaxis()->SetTitle("m(Ds)");
  Hb->SetTitle("D*^{0} #rightarrow D^{0}#gamma");
  Hb->Draw();
  Ntuple->Draw("dsmass","evthasD0gam&&!evthassignal&&Kmctrue>0","same");
  Ntuple->Draw("dsmass","evthasD0gam&&!evthassignal&&Kmctrue>0&&tagmctrue>0","same");
  Ntuple->Draw("dsmass","evthasD0gam&&!evthassignal&&Kmctrue>0&&tagmctrue>0&&!evthasKK","same");
  Ntuple->Draw("dsmass","evthasD0gam&&!evthassignal&&Kmctrue>0&&tagmctrue>0&&!evthasKK&&abs(tagmctrueparent)==423","same");
  TLatex text;
  text.SetTextSize(.03);
  text.DrawTextNDC(.13,.8,"D*0");
  text.DrawTextNDC(.13,.77,"D*0,trueK");
  text.DrawTextNDC(.13,.74,"D*0,trueK,trueTag");
  text.DrawTextNDC(.13,.71,"D*0,trueK,trueTag,noKK");
  text.DrawTextNDC(.13,.68,"D*0,trueK,trueTag,noKK,D*0Tag");


  //Draw the signal components:
  C.cd(2);
  Ntuple->Draw("dsmass>>Hs(50,1.82,2.12)","evthassignal");
  Hs->GetYaxis()->SetRangeUser(0,Hb->GetBinContent(Hb->GetMaximumBin())*1.2);
  Hs->GetXaxis()->SetTitle("m(Ds)");
  Hs->SetTitle("D_{s}* #rightarrow D_{s}#gamma");
  Hs->Draw();
  Ntuple->Draw("dsmass","evthassignal&&Kmctrue>0","same");
  Ntuple->Draw("dsmass","evthassignal&&Kmctrue>0&&tagmctrue>0","same");
  text.DrawTextNDC(.13,.8,"Ds*");
  text.DrawTextNDC(.13,.77,"Ds*,trueK");
  text.DrawTextNDC(.13,.74,"Ds*,trueK,trueTag");
}


void PlotD0BkgDst0Recoil(){
  gROOT->cd();
  TFile _file0("DsTolnu/SP1005Skim/DsReco/FinalWS/Ntuple.root","read");
  TTree* Ntuple=(TTree*)_file0.Get("Ntuple");
  TCanvas C("C");
  //C.Divide(2,1);

  //Draw the D*->D0 gam 
  //C.cd(1);  
  //Ntuple->Draw("dsmass>>Hb(50,1.82,2.12)","evthasD0gam&&!evthassignal");

  TH1F Hb("Hb","",50,1.82,2.12);
  Ntuple->Draw("dsmass>>Hb","evthasD0gam&&!evthassignal&&tagangleToDst0<0");
  Hb.GetYaxis()->SetRangeUser(0,Hb.GetBinContent(Hb.GetMaximumBin())*1.2);
  Hb.GetXaxis()->SetTitle("m(Ds)");
  Hb.SetTitle("D*^{0} #rightarrow D^{0}#gamma");
  Hb.Draw();
  Ntuple->Draw("dsmass","evthasD0gam&&!evthassignal&&tagangleToDst0<0&&tagmctrue>0","same");
  Ntuple->Draw("dsmass","evthasD0gam&&!evthassignal&&tagangleToDst0<0&&tagmctrue>0&&!evthasKK","same");  
  TLatex text;
  text.SetTextSize(.03);
  text.DrawTextNDC(.13,.8,"D*0 ");
  text.DrawTextNDC(.13,.77,"D*0,trueTag");
  text.DrawTextNDC(.13,.74,"D*0,trueTag,noKK");


//   //Draw the signal components:
//   C.cd(2);
//   Ntuple->Draw("dsmass>>Hs(50,1.82,2.12)","evthassignal");
//   Hs->GetYaxis()->SetRangeUser(0,Hb.GetBinContent(Hb.GetMaximumBin())*1.2);
//   Hs->GetXaxis()->SetTitle("m(Ds)");
//   Hs->SetTitle("D_{s}* #rightarrow D_{s}#gamma");
//   Hs->Draw();
//   Ntuple->Draw("dsmass","evthassignal&&Kmctrue>0","same");
//   Ntuple->Draw("dsmass","evthassignal&&Kmctrue>0&&tagmctrue>0","same");
//   text.DrawTextNDC(.13,.8,"Ds*");
//   text.DrawTextNDC(.13,.77,"Ds*,trueK");
//   text.DrawTextNDC(.13,.74,"Ds*,trueK,trueTag");

}
