#include "MoveStatsBox.h"
#include <TPaveStats.h>
void MoveStatsBox(TH1* h,Int_t X,Int_t Y){
  
  if(h==NULL||X<-1||X>1||Y<-1||Y>1){
    cout<<"MoveStatsBox: Bad Arguments: h="<<h<<" X="<<X<<" Y="<<Y<<endl;
    return;
  }

  TPaveStats* s=NULL;
  s=(TPaveStats*)h->GetListOfFunctions()->FindObject("stats");
  
  if(s==NULL){
    cout<<"No stats found in histogram:"<<h->GetName()<<endl;
    cout<<"Try TH1F::Draw() then TCanvas::Update()"<<endl;
    return;
  }
  
  Float_t width=.18;
  Float_t height=.17;
  Float_t Xmin=.13;
  Float_t Xmax=.78;
  Float_t Ymin=.13;
  Float_t Ymax=.82;
  if(X==1){//right
    s->SetX1NDC(Xmax-width);
    s->SetX2NDC(Xmax);
  }
  if(Y==1){//top
    s->SetY1NDC(Ymax-height);
    s->SetY2NDC(Ymax);
  }
  if(X==-1){//left
    s->SetX1NDC(Xmin);
    s->SetX2NDC(Xmin+width);
  }
  if(Y==-1){//bottom
    s->SetY1NDC(Ymin);
    s->SetY2NDC(Ymin+height);
  }
  if(X==0){//mid-horizontal
    s->SetX1NDC((Xmin+Xmax-width)/2);
    s->SetX2NDC((Xmin+Xmax+width)/2);
  }
  if(Y==0){//mid-vertical
    s->SetY1NDC((Ymin+Ymax-height)/2);
    s->SetY2NDC((Ymin+Ymax+height)/2);
  }
    
      
}

