{

RooRealVar x("x","x",0,1);

///now try with RooHistPdfs
TH1F h1("h1","",10,0,1);
TH1F h2("h2","",10,0,1);
TH1F h3("h3","",10,0,1);
TH1F h4("h4","",10,0,1);
TH1F hr("hr","",10,0,1);
for(Int_t i=1;i<=10;i++){
  h1.SetBinContent(i,i);
  h2.SetBinContent(i,2*i);
  h3.SetBinContent(i,3*i);
  h4.SetBinContent(i,4*i);
  hr.SetBinContent(i,i*i);
}
RooDataHist d1("d1","d1",RooArgSet(x),&h1);
RooDataHist d2("d2","d2",RooArgSet(x),&h2);
RooDataHist d3("d3","d3",RooArgSet(x),&h3);
RooDataHist d4("d4","d4",RooArgSet(x),&h4);
RooDataHist dr("dr","dr",RooArgSet(x),&hr);

RooHistPdf p1("p1","p1",RooArgSet(x),d1);
RooHistPdf p2("p2","p2",RooArgSet(x),d2);
RooHistPdf p3("p3","p3",RooArgSet(x),d3);
RooHistPdf p4("p4","p4",RooArgSet(x),d4);
RooHistPdf pr("pr","pr",RooArgSet(x),dr);

RooRealVar c1("c1","",.1);
RooAddPdf a1("a1","",p1,p2,c1);
RooRealVar c2("c2","",.1);
RooAddPdf a2("a2","",p3,p4,c2);

RooRealVar c("c","",.1);
//RooAddPdf f("f","",a1,a2,c);
RooAddPdf f("f","",RooArgList(p1,p2,p3,p4),RooArgList(c1,c2,c));
RooProdPdf p("p","",pr,f);

RooPlot*plot=x.frame();
f.plotOn(plot,LineColor(4));
p.plotOn(plot,LineColor(2));
pr.plotOn(plot,LineColor(3));
plot->Draw();


}
