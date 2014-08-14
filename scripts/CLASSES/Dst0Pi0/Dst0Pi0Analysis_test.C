#include "dst0pi0analysis.h"

/////////
// #include "dst0pi0analysisfitmasspeak.c"
// #include "dst0pi0analysisroofitmasspeak.c"
// #include "dst0pi0analysisroofitws.c"
// #include "dst0pi0analysisresolutioneff.c"
// #include "dst0pi0analysisk3pidstarbkg.c"
#include "dst0pi0analysisplotdmvspstarcoshel.c"
// #include "dst0pi0analysisroofitdmlowcoshel.c"

classimp(dst0pi0analysis);
dst0pi0analysis::dst0pi0analysis(tchain* fchain,bool_t truthmatch,int_t matterorantimatter,int_t dstpilund,tstring outputdir,int_t d0mode):
  _fchain(fchain),
  _truthmatch(truthmatch),
  _matterorantimatter(matterorantimatter),							   
  _mydstpilund(dstpilund),
  _outputdir(outputdir), 
  _d0mode(d0mode),
  outputfile(null),
  reducedntuple(null),
  mcxcounterperevent(0),
  mcxcountertotal(0),
  recoxcountertotal(0)
{
  if(_fchain==null){cout<<"no chain"<<endl;}
   
  //init the database 
  database.settruthmatch(_truthmatch);
  database.setd0mode(_d0mode);
   
  //the phase space and q factor for the threshold
  phasefactor="((1/(2*(dstpideltam+2.0067)**2))*sqrt(((dstpideltam+2.0067)*(dstpideltam+2.0067) -  4.5869)*((dstpideltam+2.0067)*(dstpideltam+2.0067)- 3.5032)))";
  qfactor="((1/(2*(dstpideltam+2.0067)))*sqrt(((dstpideltam+2.0067)*(dstpideltam+2.0067) -  4.5869)*((dstpideltam+2.0067)*(dstpideltam+2.0067)- 3.5032)))";
  
  ///labels
  dstarpixlabel="m(d*^{0}#pi^{0})  -  m(d*^{0})   (gev/c^{2}) ";
  dstarpiylabel="entries/5 mev"; 

  cout<<"done initializing the analysis"<<endl;
}

dst0pi0analysis::~dst0pi0analysis(){  
  
}


int_t dst0pi0analysis::reducentuple(bool_t runintestmode){
  cout<<"going to reduce the ntuple"<<endl;

  if(_fchain==null){
    cout<<"no chain."<<endl;
    return 0;
  } 


  ///link the branches in the input chain
  setbranches();


  //the file must be created here so the 'new' histos and tree is created in hard drive 
  outputfile=new tfile(_outputdir+"/reduced.root","recreate",_outputdir+"/reduced.root",4);
  if(outputfile==null){cout<<"failed to create file"<<endl;return 0;}


  //create new reduced ntuple
  reducedntuple=new ttree("ntuple","reduced ntuple",99);        
 
  //create the branches in the reducedntuple
  makereducedntuplebranches();


  if(_truthmatch){
    //create the mc histos
    cout<<"going to init mc histograms"<<endl;
    hmcxmass=new th1f();
    sethistoxy(hmcxmass,"hmcxmass",database.getdst0pi0massnbins(),database.getdst0pi0massmin(),database.getdst0pi0massmax(),"x cand. mass (gev/c^{2})","");
  
    hmcxdeltam=new th1f();
    sethistoxy(hmcxdeltam,"hmcxdeltam",database.getdst0pi0deltamnbins(),database.getdst0pi0deltammin(),database.getdst0pi0deltammax(),"d*(2640) mass - d*(2010) (gev/c^{2})","");


    hmcxp3cm=new th1f();
    sethistoxy(hmcxp3cm,"hmcxp3cm",50,0,5,
	       "p* (gev/c)","entries/100mev");
  
    hmcxcosthcm=new th1f();
    sethistoxy(hmcxcosthcm,"hmcxcosthcm",20,-1.0001,1.0001,
	       "cos(#theta)*","");
  
    h2mcxcmpvstheta=new th2f();
    sethisto2d(h2mcxcmpvstheta,"h2mcxcmpvstheta",50,0,5,
	       "p* (gev/c)",20,-1.0001,1.0001,"cos(#theta*)","");  

    hmcnx=new th1f();
    sethistoxy(hmcnx,"hmcnx",11,-.5,10.5,
	       "nx/event","counts");
  }
  //////


  ///check how many candidates are removed by array size cut
  int_t removedcands=0;
  int_t initialdcands=0;

  //start the event loop;
  long_t maxnumberevents=1000000000;
  if(runintestmode)maxnumberevents=100000;

  cout<<"going to start the event loop"<<endl;
  int_t eventid=0;
  while(_fchain->getentry(eventid,0)>0&&eventid<maxnumberevents){   
    eventid++;
     
    if(eventid%50000==0)cout<<eventid<<" events done.   with "<<recoxcountertotal<<" candidates saved."<<endl;    
      
    eventnumber=eventid;
     
    initialdcands+=nx;
    if(nx>maxncands){
      cout<<"too many cands at event "<<eventid<<" only first "<<maxncands<<" of "<<nx<<" will be used"<<endl;
      removedcands+=nx-maxncands; 
      nx=maxncands;      
    }

    ///loop over the reconstructed
    recoxcounterperevent=0;      
    
    //------beam quantities
    tvector3 beamvtx(primvtxx,primvtxy,primvtxz);            

    for(xidx=0;xidx<nx;xidx++){
      
      ////////////cut out the unconverged fits
      if(xvtxstatus[xidx]!=0)continue;               


      ///determine the indexes for each particle
      dstaridx=xd1idx[xidx];
      pi1idx=xd2idx[xidx];
      gam1idx=pi0d1idx[pi1idx];
      gam2idx=pi0d2idx[pi1idx]; 
      d0idx=dstard1idx[dstaridx];
      slowpiidx=dstard2idx[dstaridx];
      slowgam1idx=pi0d1idx[slowpiidx];
      slowgam2idx=pi0d2idx[slowpiidx]; 
      kidx=d0d1idx[d0idx];
      ktrkidx=ktrkidx[kidx];
      piidx=d0d2idx[d0idx];
      pitrkidx=pitrkidx[piidx];
      d0pi2idx=d0d3idx[d0idx];
      d0pi2trkidx=pitrkidx[d0pi2idx];
      d0pi3idx=d0d4idx[d0idx];                              
      d0pi3trkidx=pitrkidx[d0pi3idx];
      
     
      //////fill reduced ntuple variables    
      dstpimass=xmass[xidx];      
      dstpipstar=xp3cm[xidx];
      dstpicosstar=xcosthcm[xidx];
      dstpideltam=xmass[xidx]-dstarmass[dstaridx];  
      if(tmath::prob(xchi2[xidx],xndof[xidx])>0)
	dstpilogvtxprob=log10(tmath::prob(xchi2[xidx],xndof[xidx]));
      else dstpilogvtxprob=-100;
      //determine d*pi combination: dstpicharge= -2=d*-pi- , -1=d*-pi+  , 1=d*+pi- , 2=d*+pi+
      dstpicharge=dstarlund[dstaridx]/abs(dstarlund[dstaridx]);
      if(dstpicharge==0)dstpicharge=dstarlund[dstaridx]/abs(dstarlund[dstaridx]);
      dstpinx=nx;  
      dstpiidx=xidx;

      //-------dstar quantities
      dstarmass=dstarmass[dstaridx];
      dstardeltam=dstarmass[dstaridx]-d0mass[d0idx];
      if(dstarvtxstatus[dstaridx]==0) dstarlogvtxprob=log10(tmath::prob(dstarchi2[dstaridx],dstarndof[dstaridx]));   
      else dstarlogvtxprob=0;
      dstarcostheta=computedstarangle();
      dstarpstar=dstarp3cm[dstaridx];
      dstarcharge=dstarlund[dstaridx]/abs(dstarlund[dstaridx]);

      //--pi1 
      pi1mass=pi0mass[pi1idx];
      pi1p3=pi0p3[pi1idx];
      pi1energy=sqrt(pi1mass*pi1mass+pi1p3*pi1p3);      
      //--gamma quantities
      gam1energy=gamp3[gam1idx];
      gam1cos=gamcosth[gam1idx]; 
      gam2energy=gamp3[gam2idx];
      gam2cos=gamcosth[gam2idx];
	    

      //-------d0 quantities
      d0mass=d0mass[d0idx];
      if(tmath::prob(d0chi2[d0idx],d0ndof[d0idx])>0) d0logvtxprob = log10(tmath::prob(d0chi2[d0idx],d0ndof[d0idx]));
      else d0logvtxprob=0;
      d0pstar=d0p3cm[d0idx];
      d0mode=0;
      if(d0pi2idx==-1&&d0pi3idx==-1) d0mode=1;
      if(d0pi2idx>=0&&d0pi3idx>=0) d0mode=2;
      if(d0mode==0)cout<<"error d0mode=0"<<endl;
      d0charge=dstarlund[dstaridx]/abs(dstarlund[dstaridx]);
      //d0costheta=computed0angle();

           
      //--slow pi0 
      slowpimass=pi0mass[slowpiidx];
      slowpip3=pi0p3[slowpiidx];
      slowpienergy=sqrt(slowpimass*slowpimass+slowpip3*slowpip3);      
      //--gamma quantities
      slowgam1energy=gamp3[slowgam1idx];
      slowgam1cos=gamcosth[slowgam1idx];
      slowgam2energy=gamp3[slowgam2idx];
      slowgam2cos=gamcosth[slowgam2idx];

     
      //track quantities
      kp3=kp3[kidx];
      kcharge=klund[kidx]/abs(klund[kidx]);
      //kcostheta=computekangle();

      pip3=pip3[piidx];
      picharge=pilund[piidx]/abs(pilund[piidx]);

      d0pi2p3=0;d0pi3p3=0;d0pi2charge=0;d0pi3charge=0;
      if(d0mode==2){
	d0pi2p3=pip3[d0pi2idx];      
	d0pi2charge=pilund[d0pi2idx]/abs(pilund[d0pi2idx]);      
	d0pi3p3=pip3[d0pi3idx];      
	d0pi3charge=pilund[d0pi3idx]/abs(pilund[d0pi3idx]);		
      }
          	      
     

      //---------selectors
      klh=0;
      for(int_t i=10;i<=14;i++){//10=notpion,11=veryloose,12=loose,13=tight,14=verytight
	if((kselectorsmap[ktrkidx] & (1<<i)) == (1<<i))
	  klh=i;

      }
            
      pilh=0;d0pi2lh=0;d0pi3lh=0;
      for(int_t i=2;i<=5;i++){//2=veryloose,3=loose,4=tight,5=verytight
	if((piselectorsmap[pitrkidx] & (1<<i) ) == (1<<i))
	  pilh=i;
	
	if(d0mode==2){
	  if((piselectorsmap[d0pi2trkidx] & (1<<i) ) == (1<<i))
	    d0pi2lh=i;
	  if((piselectorsmap[d0pi3trkidx] & (1<<i) ) == (1<<i))
	    d0pi3lh=i;
	}
      }
      
      
      //fore mc determine if candidate was truthmatched
      dstpimctrue=1;
      dstarmctrue=1;
      pi1mctrue=1;
      d0mctrue=1;
      slowpimctrue=1;
      kmctrue=1;
      pimctrue=1;     
      d0pi2mctrue=1;  
      d0pi3mctrue=1;  
      dstpimclund=0;
      dstpidmres=100000;
      if(_truthmatch){
	dstpimctrue=(xmcidx[xidx]>0);

	dstarmctrue=(dstarmcidx[dstaridx]>0);
	d0mctrue=(d0mcidx[d0idx]>0);
	pimctrue=(pimcidx[piidx]>0);
	slowpimctrue=(pimcidx[slowpiidx]>0);
	kmctrue=(kmcidx[kidx]>0);
	pi0mctrue=(pi0mcidx[pi0idx]>0);
	if(d0mode==2){
	  d0pi2mctrue=(pimcidx[d0pi2idx]>0);
	  d0pi3mctrue=(pimcidx[d0pi3idx]>0);
	}

	//////determine the mass resolution for mc
	dstpimclund=mclund[xmcidx[xidx]];
	if(dstpimctrue)      
	  dstpidmres=dstpideltam-(mcmass[xmcidx[xidx]]-mcmass[dstarmcidx[dstaridx]]);

      }

           
      //apply some simple cuts to minimize file size
      if(dstardeltam>.180||d0mass<database.getd0massmin()||database.getd0massmax()<d0mass||d0mode!=_d0mode) continue;
     
      reducedntuple->fill();	              
               
      recoxcountertotal++;
           
   
    }//x loop    
      
    ///-------------------------------
    ///fill the generated quantities
    ///-----------------------------
    if(_truthmatch){    
      mcxcounterperevent=0; 
      int_t mcid=-1;
      bool_t goodcand;
      while(mcid<mclen){
	mcid++;
       
	if(mcid>=maxncands||dauidx[mcid]>=maxncands){cout<<"mc idx larger than array size."<<endl;continue;}

	goodcand=false;
	if(abs(_matterorantimatter)==1&&mclund[mcid]==_matterorantimatter*_mydstpilund)goodcand=true;
	if(_matterorantimatter==0&&abs(mclund[mcid])==_mydstpilund)goodcand=true;
	if(goodcand&&abs(mclund[dauidx[mcid]])!=mydstarlund)goodcand=false;//d*2460 also decays to d0

	if(goodcand){
	  mcxcounterperevent++; 
	  mcxcountertotal++;
	 
	  hmcxmass->fill(mcmass[mcid]);
	  hmcxp3cm->fill(mcp3cm[mcid]); 
	  hmcxcosthcm->fill(mccosthcm[mcid]); 	
	  h2mcxcmpvstheta->fill(mcp3cm[mcid],mccosthcm[mcid]);

	  //calculate deltam :  mcmass[dauidx[mcid]]=first daughter; mcmass[dauidx[mcid] + 1] = second daughter ...	    
	  hmcxdeltam->fill(mcmass[mcid]-mcmass[dauidx[mcid]]);
	}
	
	//calculatedstrandpigenmassdist(mcid);      
      }
      hmcnx->fill(mcxcounterperevent);
    }

  }

  //print summary
  cout<<"--------summary-------"<<endl;
  cout<<"total events="<<eventid<<endl;
  cout<<"total generated="<<mcxcountertotal<<" reconstructed="<<recoxcountertotal<<endl;
  cout<<"total candidates removed "<<removedcands<<" ( "<<(int)(100*removedcands/initialdcands + .5)<<"%)"<<endl;
  cout<<"--------end summary---"<<endl;

  ////save the ntuple and histograms

  cout<<"going to save the ntuple and histograms"<<endl; 

  ///////////should not have to call ttree->write(), tfile->write() will doit
  if(reducedntuple->write()){ cout<<"ntuple written."<<endl;}
  else{ cout<<"failed to write ntuple"<<endl;return 0;}
  delete  reducedntuple;   

  if(_truthmatch){
    
    if(hmcxmass->write()&&
       hmcxdeltam->write()&&
       hmcxp3cm->write()&&
       hmcxcosthcm->write()&&     
       h2mcxcmpvstheta->write()&&     
       hmcnx->write()
       ){ 
      cout<<" histograms have been written"<<endl;
    }
    else{
      cout<<"failed to write histos"<<endl;return 0;
    }
  
  }
  
  
  outputfile->ls();//turns out this is quite important for the objects to be written.
  outputfile->close(); //this will delete all objects created inside the file if they were not written
  cout<<outputfile->getname()<<" has been closed."<<endl;

  delete outputfile;

  return 1;
}


int_t dst0pi0analysis::openreducedntuple(tstring filename){
  
  cout<<"going to open reduced ntuple."<<endl;


  if(!(outputfile=new tfile(_outputdir+"/"+filename,"read"))){
    cout<<"unable to open reduced file"<<endl;
    return 0;
  }
  if(outputfile->iszombie()){
    cout<<"zombie reduced ntuple file"<<endl;return 0;
  }
  cout<<"file opened: "<<outputfile<<" "<<outputfile->getname()<<endl;


  if(!(reducedntuple=(ttree*)outputfile->get("ntuple"))){
    cout<<"no reduced ntuple"<<endl;return 0;
  }
  

  
  //reducedntuple->setdirectory(0);
  cout<<"ntuple extracted: "<<reducedntuple<<" "<<reducedntuple->getname()<<"  with "<<reducedntuple->getentries()<<" entries"<<endl;

  setreducedntuplebranches();
  
  groot->cd();//******remember to do this when opening a file otherwise new objects are created in that file******//

  return 1;
}

int_t dst0pi0analysis::closereducedntuple(){
  cout<<"going to close the file."<<endl;
 
  outputfile->close();

  delete outputfile;
  
  return 1;
}


float_t dst0pi0analysis::computedstarangle(){
  tvector3 xp3vec(xp3cm[xidx]*sin(acos(xcosthcm[xidx]))*cos(xphicm[xidx]),
		      xp3cm[xidx]*sin(acos(xcosthcm[xidx]))*sin(xphicm[xidx]),
		      xp3cm[xidx]*xcosthcm[xidx]);
  
  float_t xenergy=sqrt(xp3vec*xp3vec + xmass[xidx]*xmass[xidx]);
  tvector3 xvelvec=(1/xenergy)*xp3vec;

  tvector3 dstarp3vec(dstarp3cm[dstaridx]*sin(acos(dstarcosthcm[dstaridx]))*cos(dstarphicm[dstaridx]),
		      dstarp3cm[dstaridx]*sin(acos(dstarcosthcm[dstaridx]))*sin(dstarphicm[dstaridx]),
		      dstarp3cm[dstaridx]*dstarcosthcm[dstaridx]); 
  
  tlorentzvector dstarp4(dstarp3vec,sqrt(dstarp3vec*dstarp3vec+dstarmass[dstaridx]*dstarmass[dstaridx]));

  //boost the dstar momentum into rest frame of x
  dstarp4.boost(-xvelvec);//watch the minus sign
  tvector3 dstarboostedp3vec=dstarp4.vect();

  //dot product of boosted dstar direction with x direction in cm frame
  float_t cosine=dstarboostedp3vec.unit()*xvelvec.unit();
  
  return cosine;
  
}



int_t dst0pi0analysis::setbranches(){
  if(_fchain==null){cout<<"no chain"<<endl;return 0;} 
  cout<<"setting chain branch addresses"<<endl;

  _fchain->setbranchaddress("nx",&nx);
  _fchain->setbranchaddress("xmass",xmass);
  _fchain->setbranchaddress("xp3",xp3);
  _fchain->setbranchaddress("xcosth",xcosth);
  _fchain->setbranchaddress("xphi",xphi);
  _fchain->setbranchaddress("xp3cm",xp3cm);
  _fchain->setbranchaddress("xcosthcm",xcosthcm);
  _fchain->setbranchaddress("xphicm",xphicm);
  _fchain->setbranchaddress("xlund",xlund); 
  _fchain->setbranchaddress("xd1lund",xd1lund);
  _fchain->setbranchaddress("xd1idx",xd1idx);
  _fchain->setbranchaddress("xd2lund",xd2lund);
  _fchain->setbranchaddress("xd2idx",xd2idx);
  _fchain->setbranchaddress("xvtxx",xvtxx);
  _fchain->setbranchaddress("xvtxy",xvtxy);
  _fchain->setbranchaddress("xvtxz",xvtxz);
  _fchain->setbranchaddress("xchi2",xchi2);
  _fchain->setbranchaddress("xndof",xndof);
  _fchain->setbranchaddress("xvtxstatus",xvtxstatus);
 
  _fchain->setbranchaddress("ndstar",&ndstar);
  _fchain->setbranchaddress("dstarmass",dstarmass);
  _fchain->setbranchaddress("dstarp3",dstarp3);
  _fchain->setbranchaddress("dstarcosth",dstarcosth);
  _fchain->setbranchaddress("dstarphi",dstarphi);
  _fchain->setbranchaddress("dstarp3cm",dstarp3cm);
  _fchain->setbranchaddress("dstarcosthcm",dstarcosthcm);
  _fchain->setbranchaddress("dstarphicm",dstarphicm);
  _fchain->setbranchaddress("dstarlund",dstarlund);
  _fchain->setbranchaddress("dstard1lund",dstard1lund);
  _fchain->setbranchaddress("dstard1idx",dstard1idx);
  _fchain->setbranchaddress("dstard2lund",dstard2lund);
  _fchain->setbranchaddress("dstard2idx",dstard2idx);
  _fchain->setbranchaddress("dstarchi2",dstarchi2);
  _fchain->setbranchaddress("dstarndof",dstarndof);
  _fchain->setbranchaddress("dstarvtxstatus",dstarvtxstatus);

  _fchain->setbranchaddress("npi0",&npi0);
  _fchain->setbranchaddress("pi0mass",pi0mass);
  _fchain->setbranchaddress("pi0p3",pi0p3);
  _fchain->setbranchaddress("pi0costh",pi0costh);
  _fchain->setbranchaddress("pi0phi",pi0phi);
  _fchain->setbranchaddress("pi0p3cm",pi0p3cm);
  _fchain->setbranchaddress("pi0costhcm",pi0costhcm);
  _fchain->setbranchaddress("pi0phicm",pi0phicm);
  _fchain->setbranchaddress("pi0lund",pi0lund);
  _fchain->setbranchaddress("pi0d1lund",pi0d1lund);
  _fchain->setbranchaddress("pi0d1idx",pi0d1idx);
  _fchain->setbranchaddress("pi0d2lund",pi0d2lund);
  _fchain->setbranchaddress("pi0d2idx",pi0d2idx);
  _fchain->setbranchaddress("pi0chi2",pi0chi2);
  _fchain->setbranchaddress("pi0ndof",pi0ndof);
  _fchain->setbranchaddress("pi0vtxstatus",pi0vtxstatus);

  _fchain->setbranchaddress("ngam",&ngam);
  _fchain->setbranchaddress("gamp3",gamp3);
  _fchain->setbranchaddress("gamcosth",gamcosth);
  _fchain->setbranchaddress("gamphi",gamphi);
  _fchain->setbranchaddress("gamp3cm",gamp3cm);
  _fchain->setbranchaddress("gamcosthcm",gamcosthcm);
  _fchain->setbranchaddress("gamphicm",gamphicm);
  _fchain->setbranchaddress("gamlund",gamlund);

  _fchain->setbranchaddress("nd0",&nd0);
  _fchain->setbranchaddress("d0mass",d0mass);
  _fchain->setbranchaddress("d0p3cm",d0p3cm); 
  _fchain->setbranchaddress("d0costhcm",d0costhcm); 
  _fchain->setbranchaddress("d0phicm",d0phicm);  
  _fchain->setbranchaddress("d0lund",d0lund);
  _fchain->setbranchaddress("d0d1lund",d0d1lund);
  _fchain->setbranchaddress("d0d1idx",d0d1idx);
  _fchain->setbranchaddress("d0d2lund",d0d2lund);
  _fchain->setbranchaddress("d0d2idx",d0d2idx);
  _fchain->setbranchaddress("d0chi2",d0chi2);
  _fchain->setbranchaddress("d0ndof",d0ndof);
  _fchain->setbranchaddress("d0vtxstatus",d0vtxstatus);
  //_fchain->setbranchaddress("d0ndaus",d0ndaus);
  _fchain->setbranchaddress("d0d3lund",d0d3lund);
  _fchain->setbranchaddress("d0d3idx",d0d3idx);
  _fchain->setbranchaddress("d0d4lund",d0d4lund);
  _fchain->setbranchaddress("d0d4idx",d0d4idx);
  
  _fchain->setbranchaddress("npi",&npi);
  _fchain->setbranchaddress("pip3",pip3);
  _fchain->setbranchaddress("picosth",picosth);
  _fchain->setbranchaddress("piphi",piphi);
  _fchain->setbranchaddress("pip3cm",pip3cm);
  _fchain->setbranchaddress("picosthcm",picosthcm);
  _fchain->setbranchaddress("piphicm",piphicm);
  _fchain->setbranchaddress("pilund",pilund);
  _fchain->setbranchaddress("pitrkidx",pitrkidx);

  _fchain->setbranchaddress("nk",&nk);
  _fchain->setbranchaddress("kp3",kp3);
  _fchain->setbranchaddress("kp3cm",kp3cm);
  _fchain->setbranchaddress("kcosthcm",kcosthcm);
  _fchain->setbranchaddress("kphicm",kphicm);
  _fchain->setbranchaddress("klund",klund);
  _fchain->setbranchaddress("ktrkidx",ktrkidx);

  _fchain->setbranchaddress("trknsvt",trknsvt);
  _fchain->setbranchaddress("trklund",trklund); 
  _fchain->setbranchaddress("piselectorsmap",piselectorsmap);
  _fchain->setbranchaddress("kselectorsmap",kselectorsmap);


  if(_truthmatch){
    _fchain->setbranchaddress("xmcidx",xmcidx);
    _fchain->setbranchaddress("dstarmcidx",dstarmcidx);
    _fchain->setbranchaddress("d0mcidx",d0mcidx);
    _fchain->setbranchaddress("kmcidx",kmcidx);
    _fchain->setbranchaddress("pimcidx",pimcidx);
    _fchain->setbranchaddress("pi0mcidx",pi0mcidx);
    _fchain->setbranchaddress("gammcidx",gammcidx);
    _fchain->setbranchaddress("mclund",mclund);
    _fchain->setbranchaddress("mothidx",mothidx);
    _fchain->setbranchaddress("dauidx",dauidx);
    _fchain->setbranchaddress("mclen",&mclen);
    _fchain->setbranchaddress("mcmass",mcmass);
    _fchain->setbranchaddress("mcp3",mcp3);
    _fchain->setbranchaddress("mccosth",mccosth);
    _fchain->setbranchaddress("mcphi",mcphi);
    _fchain->setbranchaddress("mcp3cm",mcp3cm);
    _fchain->setbranchaddress("mccosthcm",mccosthcm); 
    _fchain->setbranchaddress("mcphicm",mcphicm);
    _fchain->setbranchaddress("mcvtxx",mcvtxx);
    _fchain->setbranchaddress("mcvtxy",mcvtxy);
    _fchain->setbranchaddress("mcvtxz",mcvtxz);
  }


  cout<<"done linking chain branches"<<endl;
  return 1;
}



int_t dst0pi0analysis::makereducedntuplebranches(){
  cout<<"creating reduced ntuple branches"<<endl;

  reducedntuple->branch("eventnumber",&eventnumber,"eventnumber/i");

  reducedntuple->branch("dstpimass",&dstpimass,"dstpimass/f");
  reducedntuple->branch("dstpideltam",&dstpideltam,"dstpideltam/f");
  reducedntuple->branch("dstpipstar",&dstpipstar,"dstpipstar/f");
  reducedntuple->branch("dstpicosstar",&dstpicosstar,"dstpicosstar/f");
  reducedntuple->branch("dstpicharge",&dstpicharge,"dstpicharge/i");
  reducedntuple->branch("dstpinx",&dstpinx,"dstpinx/i");
  reducedntuple->branch("dstpilogvtxprob",&dstpilogvtxprob,"dstpilogvtxprob/f");
  reducedntuple->branch("dstpiidx",&dstpiidx,"dstpiidx/i");
  reducedntuple->branch("dstpimctrue",&dstpimctrue,"dstpimctrue/i");
  reducedntuple->branch("dstpidmres",&dstpidmres,"dstpidmres/f");
  reducedntuple->branch("dstpigenmass",&dstpigenmass,"dstpigenmass/f");
  reducedntuple->branch("dstpigendist",&dstpigendist,"dstpigendist/f");
  reducedntuple->branch("dstpimclund",&dstpimclund,"dstpimclund/i");

  reducedntuple->branch("dstarmass",&dstarmass,"dstarmass/f");
  reducedntuple->branch("dstarpstar",&dstarpstar,"dstarpstar/f");
  reducedntuple->branch("dstardeltam",&dstardeltam,"dstardeltam/f");
  reducedntuple->branch("dstarlogvtxprob",&dstarlogvtxprob,"dstarlogvtxprob/f");
  reducedntuple->branch("dstarcostheta",&dstarcostheta,"dstarcostheta/f");
  reducedntuple->branch("dstarcharge",&dstarcharge,"dstarcharge/i");
  reducedntuple->branch("dstarmctrue",&dstarmctrue,"dstarmctrue/i");

  reducedntuple->branch("pi1mass",&pi1mass,"pi1mass/f");
  reducedntuple->branch("pi1p3",&pi1p3,"pi1p3/f");
  reducedntuple->branch("pi1energy",&pi1energy,"pi1energy/f");
  reducedntuple->branch("pi1mctrue",&pi1mctrue,"pi1mctrue/i");
  ReducedNtuple->Branch("gam1energy",&gam1energy,"gam1energy/F");
  ReducedNtuple->Branch("gam1cos",&gam1cos,"gam1cos/F");
  ReducedNtuple->Branch("gam2energy",&gam2energy,"gam2energy/F");
  ReducedNtuple->Branch("gam2cos",&gam2cos,"gam2cos/F");

  ReducedNtuple->Branch("d0pstar",&d0pstar,"d0pstar/F");
  ReducedNtuple->Branch("d0logvtxprob",&d0logvtxprob,"d0logvtxprob/F");
  ReducedNtuple->Branch("d0charge",&d0charge,"d0charge/I");
  ReducedNtuple->Branch("d0mass",&d0mass,"d0mass/F"); 
  ReducedNtuple->Branch("d0costheta",&d0costheta,"d0costheta/F"); 
  ReducedNtuple->Branch("d0mode",&d0mode,"d0mode/I");
  ReducedNtuple->Branch("d0mctrue",&d0mctrue,"d0mctrue/I");

  ReducedNtuple->Branch("slowpi1mass",&slowpi1mass,"slowpi1mass/F");
  ReducedNtuple->Branch("slowpi1pstar",&slowpi1pstar,"slowpi1pstar/F");
  ReducedNtuple->Branch("slowpi1energy",&slowpi1energy,"slowpi1energy/F");
  ReducedNtuple->Branch("slowpi1mctrue",&slowpi1mctrue,"slowpi1mctrue/I");
  ReducedNtuple->Branch("slowgam1energy",&slowgam1energy,"slowgam1energy/F");
  ReducedNtuple->Branch("slowgam1cos",&slowgam1cos,"slowgam1cos/F");
  ReducedNtuple->Branch("slowgam2energy",&slowgam2energy,"slowgam2energy/F");
  ReducedNtuple->Branch("slowgam2cos",&slowgam2cos,"slowgam2cos/F");

  ReducedNtuple->Branch("kmctrue",&kmctrue,"kmctrue/I");
  ReducedNtuple->Branch("kp3",&kp3,"kp3/F");
  ReducedNtuple->Branch("kcostheta",&kcostheta,"kcostheta/F"); 
  ReducedNtuple->Branch("kcharge",&kcharge,"kcharge/I");
  ReducedNtuple->Branch("klh",&klh,"klh/I");

  ReducedNtuple->Branch("pimctrue",&pimctrue,"pimctrue/I");
  ReducedNtuple->Branch("pip3",&pip3,"pip3/F");
  ReducedNtuple->Branch("picharge",&picharge,"picharge/I");
  ReducedNtuple->Branch("pilh",&pilh,"pilh/I");

  ReducedNtuple->Branch("d0pi2p3",&d0pi2p3,"d0pi2p3/F");
  ReducedNtuple->Branch("d0pi2mctrue",&d0pi2mctrue,"d0pi2mctrue/I");
  ReducedNtuple->Branch("d0pi2charge",&d0pi2charge,"d0pi2charge/I");
  ReducedNtuple->Branch("d0pi2lh",&d0pi2lh,"d0pi2lh/I");

  ReducedNtuple->Branch("d0pi3p3",&d0pi3p3,"d0pi3p3/F");
  ReducedNtuple->Branch("d0pi3mctrue",&d0pi3mctrue,"d0pi3mctrue/I");
  ReducedNtuple->Branch("d0pi3charge",&d0pi3charge,"d0pi3charge/I");
  ReducedNtuple->Branch("d0pi3lh",&d0pi3lh,"d0pi3lh/I");
  
  cout<<"Done Making Reduced Ntuple Branches"<<endl;
  return 1;
}



Int_t Dst0Pi0Analysis::SetReducedNtupleBranches(){
  cout<<"Setting Reduced Ntuple Branch Addresses"<<endl;

  ReducedNtuple->SetBranchAddress("eventnumber",&eventnumber);


  ReducedNtuple->SetBranchAddress("dstpimass",&dstpimass);
  ReducedNtuple->SetBranchAddress("dstpideltam",&dstpideltam);
  ReducedNtuple->SetBranchAddress("dstpipstar",&dstpipstar);
  ReducedNtuple->SetBranchAddress("dstpicosstar",&dstpicosstar);
  ReducedNtuple->SetBranchAddress("dstpiIdx",&dstpiIdx);
  ReducedNtuple->SetBranchAddress("dstpimctrue",&dstpimctrue);
  ReducedNtuple->SetBranchAddress("dstpicharge",&dstpicharge);
  ReducedNtuple->SetBranchAddress("dstpinX",&dstpinX);
  ReducedNtuple->SetBranchAddress("dstpilogvtxprob",&dstpilogvtxprob);
  ReducedNtuple->SetBranchAddress("dstpidmres",&dstpidmres);
  ReducedNtuple->SetBranchAddress("dstpigenmass",&dstpigenmass);
  ReducedNtuple->SetBranchAddress("dstpigendist",&dstpigendist);
  ReducedNtuple->SetBranchAddress("dstpimclund",&dstpimclund);

  ReducedNtuple->SetBranchAddress("dstarmass",&dstarmass);
  ReducedNtuple->SetBranchAddress("dstarpstar",&dstarpstar);
  ReducedNtuple->SetBranchAddress("dstardeltam",&dstardeltam);
  ReducedNtuple->SetBranchAddress("dstarcharge",&dstarcharge);
  ReducedNtuple->SetBranchAddress("dstarlogvtxprob",&dstarlogvtxprob);
  ReducedNtuple->SetBranchAddress("dstarcostheta",&dstarcostheta);
  ReducedNtuple->SetBranchAddress("dstarmctrue",&dstarmctrue);

  ReducedNtuple->SetBranchAddress("pi1mass",&pi1mass);
  ReducedNtuple->SetBranchAddress("pi1p3",&pi1p3);
  ReducedNtuple->SetBranchAddress("pi1energy",&pi1energy);
  ReducedNtuple->SetBranchAddress("pi1mctrue",&pi1mctrue);
  ReducedNtuple->SetBranchAddress("gam1energy",&gam1energy);
  ReducedNtuple->SetBranchAddress("gam1cos",&gam1cos);
  ReducedNtuple->SetBranchAddress("gam2energy",&gam2energy);
  ReducedNtuple->SetBranchAddress("gam2cos",&gam2cos);

  ReducedNtuple->SetBranchAddress("slowpi1mass",&slowpi1mass);
  ReducedNtuple->SetBranchAddress("slowpi1p3",&slowpi1p3);
  ReducedNtuple->SetBranchAddress("slowpi1energy",&slowpi1energy);
  ReducedNtuple->SetBranchAddress("slowpi1mctrue",&slowpi1mctrue);
  ReducedNtuple->SetBranchAddress("slowgam1energy",&slowgam1energy);
  ReducedNtuple->SetBranchAddress("slowgam1cos",&slowgam1cos);
  ReducedNtuple->SetBranchAddress("slowgam2energy",&slowgam2energy);
  ReducedNtuple->SetBranchAddress("slowgam2cos",&slowgam2cos);


  ReducedNtuple->SetBranchAddress("d0mass",&d0mass);
  ReducedNtuple->SetBranchAddress("d0pstar",&d0pstar);
  ReducedNtuple->SetBranchAddress("d0charge",&d0charge);
  ReducedNtuple->SetBranchAddress("d0logvtxprob",&d0logvtxprob);
  ReducedNtuple->SetBranchAddress("d0mode",&d0mode);
  ReducedNtuple->SetBranchAddress("d0mctrue",&d0mctrue);

  ReducedNtuple->SetBranchAddress("kcharge",&kcharge);
  ReducedNtuple->SetBranchAddress("kp3",&kp3);
  ReducedNtuple->SetBranchAddress("kcostheta",&kcostheta);
  ReducedNtuple->SetBranchAddress("kmctrue",&kmctrue);
  ReducedNtuple->SetBranchAddress("klh",&klh);

  ReducedNtuple->SetBranchAddress("picharge",&picharge);
  ReducedNtuple->SetBranchAddress("pip3",&pip3);
  ReducedNtuple->SetBranchAddress("pimctrue",&pimctrue);
  ReducedNtuple->SetBranchAddress("pilh",&pilh);
  
  ReducedNtuple->SetBranchAddress("d0pi2p3",&d0pi2p3);
  ReducedNtuple->SetBranchAddress("d0pi2charge",&d0pi2charge);
  ReducedNtuple->SetBranchAddress("d0pi2mctrue",&d0pi2mctrue);
  ReducedNtuple->SetBranchAddress("d0pi2lh",&d0pi2lh);

  ReducedNtuple->SetBranchAddress("d0pi3p3",&d0pi3p3);
  ReducedNtuple->SetBranchAddress("d0pi3charge",&d0pi3charge);
  ReducedNtuple->SetBranchAddress("d0pi3mctrue",&d0pi3mctrue);
  ReducedNtuple->SetBranchAddress("d0pi3lh",&d0pi3lh);
  
  cout<<"Done linking ReducedNtuple Branches"<<endl;
  return 1;
}
