#include "Legendre.h"


double legendre(int l, double x){
  if(l<0||l>1000) return 0;
  
  double Plminus2=1;
  double Plminus1=x;
  double Pl=0;
  
  if(l==0)Pl= Plminus2;
  if(l==1)Pl= Plminus1;
  if(l>1){//use the recursive formula P_l= [(2*l-1)/l]*x*P_{l-1} -  [(l-1)/l]*P_{l-2}
    for(int i=2;i<=l;i++){// P_l
      Pl = ((float)(2*i-1)/(float)i)*x*Plminus1 -  ((float)(i-1)/(float)i)*Plminus2; 
      Plminus2=Plminus1;
      Plminus1=Pl;
    }
  }


  return Pl;
}
