{
// Total cands saved =8965
// Total X daughters= 80509
// # of X fake daughters= 17386
// # of X daughters from X= 42030
// # of X daughters from D= 6422
// # of X daughters from K= 505
// # of X daughters from Ds= 14166

// Total X Pi daughters= 33552
// # of X Pi fake daughters= 15
// # of X Pi daughters from X= 24119
// # of X Pi daughters from D= 2792
// # of X Pi daughters from K= 0
// # of X Pi daughters from Ds= 6626

// Total X Pi0 daughters= 29373
// # of X Pi0 fake daughters= 17086
// # of X Pi0 daughters from X= 8571
// # of X Pi0 daughters from D= 1168
// # of X Pi0 daughters from K= 45
// # of X Pi0 daughters from Ds= 2503

// Total X Gam daughters= 17584
// # of X Gam fake daughters= 285
// # of X Gam daughters from X= 9340
// # of X Gam daughters from D= 2462
// # of X Gam daughters from K= 460
// # of X Gam daughters from Ds= 5037

TH1F HXComp("HXComp","",4,0,4);
HXComp.SetBinContent(1,80509);
HXComp.SetBinContent(2,33552);
HXComp.SetBinContent(3,29373);
HXComp.SetBinContent(4,17584);

TH1F HXFake("HXFake","",4,0,4);
HXFake.SetBinContent(1,17386);
HXFake.SetBinContent(2,15);
HXFake.SetBinContent(3,17086);
HXFake.SetBinContent(4,285);

TH1F HXD("HXD","",4,0,4);
HXD.SetBinContent(1,HXFake.GetBinContent(1)+6422);
HXD.SetBinContent(2,HXFake.GetBinContent(2)+2792);
HXD.SetBinContent(3,HXFake.GetBinContent(3)+1168);
HXD.SetBinContent(4,HXFake.GetBinContent(4)+2462);

TH1F HXK("HXK","",4,0,4);
HXK.SetBinContent(1,HXD.GetBinContent(1)+505);
HXK.SetBinContent(2,HXD.GetBinContent(2)+0);
HXK.SetBinContent(3,HXD.GetBinContent(3)+45);
HXK.SetBinContent(4,HXD.GetBinContent(4)+460);

TH1F HXDs("HXDs","",4,0,4);
HXDs.SetBinContent(1,HXK.GetBinContent(1)+14166);
HXDs.SetBinContent(2,HXK.GetBinContent(2)+6626);
HXDs.SetBinContent(3,HXK.GetBinContent(3)+2503);
HXDs.SetBinContent(4,HXK.GetBinContent(4)+5037);

TH1F HXX("HXX","",4,0,4);
HXX.SetBinContent(1,HXDs.GetBinContent(1)+42030);
HXX.SetBinContent(2,HXDs.GetBinContent(2)+24119);
HXX.SetBinContent(3,HXDs.GetBinContent(3)+8571);
HXX.SetBinContent(4,HXDs.GetBinContent(4)+9340);

HXComp->GetYaxis()->SetRangeUser(0,HXComp->GetMaximum()*1.1);
HXComp.GetXaxis()->SetNdivisions(0,0);
HXComp.SetStats(0);
HXComp.GetXaxis()->SetTitle("#pi^{+}+#pi^{0}+#gamma           #pi^{+}                 #pi^{0}                 #gamma         ");
HXComp.Draw();
HXX.SetLineColor(3);HXX.SetFillColor(3);HXX.Draw("same");
HXDs.SetLineColor(6);HXDs.SetFillColor(6);HXDs.Draw("same");
HXK.SetLineColor(4);HXK.SetFillColor(4);HXK.Draw("same");
HXD.SetLineColor(7);HXD.SetFillColor(7);HXD.Draw("same");
HXFake.SetLineColor(2);HXFake.SetFillColor(2);HXFake.Draw("same");
HXComp.Draw("same");
TText text;text.SetTextSize(.05);
text.SetTextColor(1);text.DrawTextNDC(.65,.8,"Source:");
text.SetTextColor(3);text.DrawTextNDC(.7,.75,"X");
text.SetTextColor(6);text.DrawTextNDC(.7,.7,"Ds");
text.SetTextColor(4);text.DrawTextNDC(.7,.65,"K");
text.SetTextColor(7);text.DrawTextNDC(.7,.6,"D");
text.SetTextColor(2);text.DrawTextNDC(.7,.55,"Fake");


}
