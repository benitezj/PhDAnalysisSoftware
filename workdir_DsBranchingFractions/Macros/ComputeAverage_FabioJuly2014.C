//
#include <TROOT.h>
#include <TMath.h>
#include <iostream>
#include <iomanip>
#include <TMatrix.h>
using namespace std;
                                                                                                                                        
#define N 3
#define NBF 2

void ComputeAverageErrorWeight(){

  
  Float_t y[N];
  Float_t V[N][N];
  Float_t VS[N][N];
  Float_t VT[N][N];
  Float_t w[N];

  TMatrix Vi(N,N);

  //read in the measured values
  y[0]=265.7;//munu
  y[1]=247;//TauNuE
  y[2]=243;//TauNuMu
  //the total stat errors
  Float_t ys[N];
  ys[0]=8.4;
  ys[1]=13;
  ys[2]=12;
  //the total syst errors
  Float_t yt[N];
  yt[0]=7.7;
  yt[1]=17;
  yt[2]=14;
  cout<<" Values :"<<endl;
  for(Int_t i=0;i<N;i++)
    cout<<y[i]<<" +- "<<ys[i]<<" +- "<<yt[i]<<endl;
  

  cout<<fixed<<setprecision(2);
  ///////////////Determine the Statistical covariance 
  for(Int_t i=0;i<N;i++){
    VS[i][i]=ys[i]*ys[i];
    for(Int_t j=0;j<N;j++){
      if(i!=j)VS[i][j]=0;
    }
  }
  cout<<" Statistical Covariance: "<<endl;
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++)
      cout<<VS[i][j]<<" ";
    cout<<endl;
  }
  ///////////////Determine the Systematic covariance 
  for(Int_t i=0;i<N;i++){
    VT[i][i]=yt[i]*yt[i];
    for(Int_t j=0;j<N;j++){
      if(i!=j)VT[i][j]=0;
    }
  }
  cout<<" Systematic Covariance: "<<endl;
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++)
      cout<<VT[i][j]<<" ";
    cout<<endl;
  }
  

  
  ///////////////Determine the total covariance 
  for(Int_t i=0;i<N;i++){
    V[i][i]=VS[i][i]+VT[i][i];
    for(Int_t j=0;j<N;j++){
      if(i!=j)V[i][j]=0;
      Vi[i][j]=V[i][j];
    }
  }
  cout<<" Total Covariance: "<<endl;
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++)
      cout<<V[i][j]<<" ";
    cout<<endl;
  }

  //actually invert the Inverse matrix:
  Vi.Invert();


  //Compute the weights
  Float_t den=0;
  for(Int_t k=0;k<N;k++)
    for(Int_t l=0;l<N;l++)
      den+=Vi[k][l];
  for(Int_t i=0;i<N;i++){
    w[i]=0;
    for(Int_t j=0;j<N;j++)
      w[i]+=Vi[i][j]/den;
  }
  cout<<" Weights: "<<endl;
  for(Int_t i=0;i<N;i++){
    cout<<w[i]<<" ";
  }  
  cout<<endl;


  //compute the weighted average
  Float_t Avg=0;
  for(Int_t i=0;i<N;i++)
    Avg+=w[i]*y[i];


  //Compute the errors
  Float_t ErrStat=0;
  Float_t ErrSyst=0;
  for(Int_t i=0;i<N;i++)
    for(Int_t j=0;j<N;j++){
      ErrStat+=w[i]*VS[i][j]*w[j];
      ErrSyst+=w[i]*VT[i][j]*w[j];
    }
  
  cout<<"Average ="<<Avg<<" +- "<<sqrt(ErrStat)<<" +- "<<sqrt(ErrSyst)<<endl;
}


void ComputeAverageErrorWeightCorrelated(){

  
  Float_t y[N];
  Float_t V[N][N];
  Float_t VS[N][N];
  Float_t VT[N][N];
  Float_t w[N];

  TMatrix Vi(N,N);

  //read in the measured values
  y[0]=265.7;//munu
  y[1]=247;//TauNuE
  y[2]=243;//TauNuMu
  //the total stat errors
  Float_t ys[N];
  ys[0]=8.4;
  ys[1]=13;
  ys[2]=12;
  //the total syst errors
  Float_t yt[N];
  // yt[0]=sqrt(pow(7.7,2)+pow(1.9,2));//add the PDG error 1.9
  // yt[1]=sqrt(pow(17,2)+pow(1.9,2));
  // yt[2]=sqrt(pow(14,2)+pow(1.9,2));
  yt[0]=7.7;//Jose (July 2014), the 1.9 MeV was being added twice, it was already added in the 7.7 value when it was computed.
  yt[1]=17;//Jose (July 2014), the 1.9 MeV was being added twice,
  yt[2]=14;//Jose (July 2014), the 1.9 MeV was being added twice,
  cout<<" Values :"<<endl;
  for(Int_t i=0;i<N;i++)
    cout<<y[i]<<" +- "<<ys[i]<<" +- "<<yt[i]<<endl;
  

  cout<<fixed<<setprecision(2);
  ///////////////Determine the Statistical covariance 
  for(Int_t i=0;i<N;i++){
    VS[i][i]=ys[i]*ys[i];
    for(Int_t j=0;j<N;j++){
      if(i!=j)VS[i][j]=0;
    }
  }
  ///set the offdiagonal elements:
  //correlation between MuNu and TauNuE
  VS[0][1]=(y[0]*0.0114/2) * (y[1]*0.0114/2);//stat error from denominator fully correlated: 1.14% is the percent error on the MuNu BF (should be ~ same  for all modes)
  //correlation between MuNu and TauNuMu
  VS[0][2]=(y[0]*0.0114/2) * (y[2]*0.0114/2);//stat error from denominator fully correlated 
  //correlation between TauNuE and TauNuMu
  VS[1][2]=(y[1]*0.0114/2) * (y[2]*0.0114/2);//stat error from denominator fully correlated 
  VS[1][0]=VS[0][1];
  VS[2][0]=VS[0][2];
  VS[2][1]=VS[1][2];

  cout<<" Statistical Covariance: "<<endl;
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++)
      cout<<VS[i][j]<<" ";
    cout<<endl;
  }


  ///////////////Determine the Systematic covariance 
  for(Int_t i=0;i<N;i++){
    VT[i][i]=yt[i]*yt[i];
    for(Int_t j=0;j<N;j++){
      if(i!=j)VT[i][j]=0;
    }
  }
  ///set the offdiagonal elements:
  //correlation between MuNu and TauNuE
  VT[0][1]=(y[0]*0.033/2) * (y[1]*0.033/2)//denominator systematics : should about the same for all modes
    +(y[0]*0.0083/2) * (y[1]*0.0083/2)//Tracking
    +(1.9) * (1.9) //Jose (July 2014), the 1.9 MeV was not being added 
    +0;//no more

  //correlation between MuNu and TauNuMu
  VT[0][2]=(y[0]*0.033/2) * (y[2]*0.033/2)//denominator systematics
    +(y[0]*0.0083/2) * (y[2]*0.0083/2)//Tracking
    +(y[0]*0.0188/2) * (y[2]*0.0191/2)//PID
    +(1.9) * (1.9) //Jose (July 2014), the 1.9 MeV was not being added 
    +0;//no more

  //correlation between TauNuE and TauNuMu
  VT[1][2]=(y[1]*0.033/2) * (y[2]*0.033/2)//denominator systematics
    +(y[1]*0.0083/2) * (y[2]*0.0083/2)//Tracking
    +(y[1]*0.055/2) * (y[2]*0.018/2)//signal model (exponential variation)//Jose (July 2014), corrected to match the tables
    +(y[1]*0.116/2) * (y[2]*0.096/2)//background model (quadratic variation)//Jose (July 2014), corrected to match the tables
    +(1.9) * (1.9) //Jose (July 2014), the 1.9 MeV was not being added 
    +0;//no more

  //it symmetric
  VT[1][0]=VT[0][1];
  VT[2][0]=VT[0][2];
  VT[2][1]=VT[1][2];

  cout<<" Systematic Covariance: "<<endl;
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++)
      cout<<VT[i][j]<<" ";
    cout<<endl;
  }
  

  
  ///////////////Determine the total covariance 
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++){
      V[i][j]=VS[i][j]+VT[i][j];
      Vi[i][j]=V[i][j];//fill the inverse
    }
  }
  cout<<" Total Covariance: "<<endl;
  for(Int_t i=0;i<N;i++){
    for(Int_t j=0;j<N;j++)
      cout<<V[i][j]<<" ";
    cout<<endl;
  }

  //actually invert the Inverse matrix:
  Vi.Invert();


  //Compute the weights
  Float_t den=0;
  for(Int_t k=0;k<N;k++)
    for(Int_t l=0;l<N;l++)
      den+=Vi[k][l];
  for(Int_t i=0;i<N;i++){
    w[i]=0;
    for(Int_t j=0;j<N;j++)
      w[i]+=Vi[i][j]/den;
  }
  cout<<" Weights: "<<endl;
  for(Int_t i=0;i<N;i++){
    cout<<w[i]<<" ";
  }  
  cout<<endl;


  //compute the weighted average
  Float_t Avg=0;
  for(Int_t i=0;i<N;i++)
    Avg+=w[i]*y[i];


  //Compute the errors
  Float_t ErrStat=0;
  Float_t ErrSyst=0;
  for(Int_t i=0;i<N;i++)
    for(Int_t j=0;j<N;j++){
      ErrStat+=w[i]*VS[i][j]*w[j];
      ErrSyst+=w[i]*VT[i][j]*w[j];
    }
  
  cout<<"Average ="<<Avg<<" \\pm "<<sqrt(ErrStat)<<" \\pm "<<sqrt(ErrSyst)<<endl;
}



void ComputeAverageErrorWeightCorrelatedBF(){

  
  Float_t y[NBF];
  Float_t V[NBF][NBF];
  Float_t VS[NBF][NBF];
  Float_t VT[NBF][NBF];
  Float_t w[NBF];

  TMatrix Vi(NBF,NBF);
  y[0]=5.07;//TauNuE
  y[1]=4.91;//TauNuMu
  //the total stat errors
  Float_t ys[NBF];
  ys[0]=.52;
  ys[1]=.47;
  //the total syst errors
  Float_t yt[NBF];
  yt[0]=.68;
  yt[1]=.54;
  cout<<" Values :"<<endl;
  for(Int_t i=0;i<NBF;i++)
    cout<<y[i]<<" +- "<<ys[i]<<" +- "<<yt[i]<<endl;
  

  
  cout<<fixed<<setprecision(3);
  ///////////////Determine the Statistical covariance 
  for(Int_t i=0;i<NBF;i++){
    VS[i][i]=ys[i]*ys[i];
    for(Int_t j=0;j<NBF;j++){
      if(i!=j)VS[i][j]=0;
    }
  }
  ///set the offdiagonal elements:
  //correlation between TauNuE and TauNuMu
  VS[0][1]=(y[0]*0.0114) * (y[1]*0.0114);//stat error from denominator fully correlated: 1.14% is the percent error on the MuNu BF (should be ~ same  for all modes) //Jose (July 10,2014): not sure I understand the 1.14%, looking at the denominator Ds yield this has a 2.2% error. I suspect one has to calculate the total error on the BF(Ds-->mu nu) including all the correlaions with the weights then this might become 1.14%. 

  //its symmetric
  VS[1][0]=VS[0][1];


  cout<<" Statistical Covariance: "<<endl;
  for(Int_t i=0;i<NBF;i++){
    for(Int_t j=0;j<NBF;j++)
      cout<<VS[i][j]<<" ";
    cout<<endl;
  }


  ///////////////Determine the Systematic covariance 
  for(Int_t i=0;i<NBF;i++){
    VT[i][i]=yt[i]*yt[i];
    for(Int_t j=0;j<NBF;j++){
      if(i!=j)VT[i][j]=0;
    }
  }
  ///set the offdiagonal elements:
  //correlation between TauNuE and TauNuMu
  VT[0][1]=(y[0]*0.033) * (y[1]*0.033)//denominator systematics,    //Jose (July 10,2014): that comes from table 6.6 & 6.7 rows 1 -> 7
    +(y[0]*0.0083) * (y[1]*0.0083)//Tracking
    +(y[0]*0.055) * (y[1]*0.018)//signal model (exponential variation)    //Jose (July 10,2014): that comes from table 6.6 & 6.7 row 9
    +(y[0]*0.116) * (y[1]*0.096)//background model (quadratic variation)    //Jose (July 10,2014): that comes from table 6.6 & 6.7 rows 8
    +0;//no more
 


  //its symmetric
  VT[1][0]=VT[0][1];

  cout<<" Systematic Covariance: "<<endl;
  for(Int_t i=0;i<NBF;i++){
    for(Int_t j=0;j<NBF;j++)
      cout<<VT[i][j]<<" ";
    cout<<endl;
  }
  

  
  ///////////////Determine the total covariance 
  for(Int_t i=0;i<NBF;i++){
    for(Int_t j=0;j<NBF;j++){
      V[i][j]=VS[i][j]+VT[i][j];
      Vi[i][j]=V[i][j];//fill the inverse
    }
  }
  cout<<" Total Covariance: "<<endl;
  for(Int_t i=0;i<NBF;i++){
    for(Int_t j=0;j<NBF;j++)
      cout<<V[i][j]<<" ";
    cout<<endl;
  }

  //actually invert the Inverse matrix:
  Vi.Invert();


  //Compute the weights
  Float_t den=0;
  for(Int_t k=0;k<NBF;k++)
    for(Int_t l=0;l<NBF;l++)
      den+=Vi[k][l];
  for(Int_t i=0;i<NBF;i++){
    w[i]=0;
    for(Int_t j=0;j<NBF;j++)
      w[i]+=Vi[i][j]/den;
  }
  cout<<" Weights: "<<endl;
  for(Int_t i=0;i<NBF;i++){
    cout<<w[i]<<" ";
  }  
  cout<<endl;


  //compute the weighted average
  Float_t Avg=0;
  for(Int_t i=0;i<NBF;i++)
    Avg+=w[i]*y[i];


  //Compute the errors
  Float_t ErrStat=0;
  Float_t ErrSyst=0;
  for(Int_t i=0;i<NBF;i++)
    for(Int_t j=0;j<NBF;j++){
      ErrStat+=w[i]*VS[i][j]*w[j];
      ErrSyst+=w[i]*VT[i][j]*w[j];
    }
  
  cout<<"Average ="<<Avg<<" \\pm "<<sqrt(ErrStat)<<" \\pm "<<sqrt(ErrSyst)<<endl;
}



