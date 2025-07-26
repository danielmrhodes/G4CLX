#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TVector3.h"
#include "TMath.h"

#include "/home/rhodes25/programs/tmpSim/include/Data_Format.hh"

////These should match the parameters defined in the simulation input////
//Masses in MeV/c^2

const int beamZ = 62;
//const double beam_mass = 72582.36; //78Kr
const double beam_mass = 147.9*931.49;

//MeV
//Reduce this value by the energy loss in the target
//const double beam_en = 329.6; //78Kr Aug 2023 Pb Target
//const double beam_en = 300.0; //78Kr Aug 2023 Pb Target
const double beam_en = 620.0;
//const double beam_en = 490.0;

const int targZ = 82;
const double targ_mass = 193688.0; //208Pb

//Silicon detector Z-offsets (downstream and upstream) (cm)
const double DS_Offset = 3.3;
const double US_Offset = 3.0;

//Beam spot position (cm)
//const double beam_X = -0.12; //78Kr Aug 2023 208Pb
//const double beam_Y = 0.05; //78Kr Aug 2023 208Pb
const double beam_X = 0.0;
const double beam_Y = 0.0;
const double beam_Z = 0.00;

const double target_width = 0.970; //208Pb 1.1 mg/cm2

/////////////////////////////////////////////////////////////////////////

//SRIM file names
//const std::string beam_srim_name = "kr78_in_pb208";
//const std::string targ_srim_name = "pb208_in_pb208";

const int nSegs = 36;
const int nDets = 120;
std::map<int,int> secMap = {{0,1},{1,0},{2,31},{3,30},{4,29},{5,28},{6,27},{7,26},{8,25},{9,24},{10,23},{11,22},{12,21},{13,20},{14,19},{15,18},{16,17},{17,16},{18,15},{19,14},{20,13},{21,12},{22,11},{23,10},{24,9},{25,8},{26,7},{27,6},{28,5},{29,4},{30,3},{31,2}};

//Gate on S3 rings (inclusive)
const int Ri = 1;
const int Rf = 24;

////Greta Intrinsic Energy Resolution////
double Sigma(double en) {
  return 0.7088 + en*0.00034535; //Not real
}
////////////////////////////////////////

////Kinematics////
double Theta_CM_FP(double ThetaLAB, double Ep, bool sol2=false, double Ex=0.0) {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  if(std::sin(ThetaLAB) > 1.0/tau) {
    
    ThetaLAB = std::asin(1.0/tau);
    if(ThetaLAB < 0)
      ThetaLAB += TMath::Pi();

    return std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB;
  }

  if(!sol2)
    return std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB;

  return std::asin(tau*std::sin(-ThetaLAB)) + ThetaLAB + TMath::Pi();

}

double Theta_CM_FR(double ThetaLAB, double Ep, bool sol2=false, double Ex=0.0) {

  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  if(std::sin(ThetaLAB) > 1.0/tau) {
    
    ThetaLAB = std::asin(1.0/tau);
    if(ThetaLAB < 0)
      ThetaLAB += TMath::Pi();

    return std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB;
  }

  if(!sol2)
    return TMath::Pi() - (std::asin(tau*std::sin(ThetaLAB)) + ThetaLAB);

  return -std::asin(tau*std::sin(-ThetaLAB)) - ThetaLAB;

}

double Theta_LAB_Max(double Ep, double Ex=0.0) {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  if(tau < 1.0)
    return TMath::Pi();
  
  return std::asin(1.0/tau);
  
}

double Theta_LAB(double thetaCM, double Ep, double Ex=0.0) {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  double tanTheta = std::sin(thetaCM)/(std::cos(thetaCM) + tau);

  if(tanTheta > 0)
    return std::atan(tanTheta);

  return std::atan(tanTheta) + TMath::Pi();
  
}

double Recoil_Theta_LAB(double thetaCM, double Ep, double Ex=0.0) {

  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));
  double tanTheta = std::sin(TMath::Pi() - thetaCM)/(std::cos(TMath::Pi() - thetaCM) + tau);
  
  return std::atan(tanTheta);
  
}

double KE_LAB(double thetaCM, double Ep, double Ex=0.0) {

  double tau = (beam_mass/targ_mass)/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));

  double term1 = std::pow(targ_mass/(beam_mass + targ_mass),2);
  double term2 = 1 + tau*tau + 2*tau*std::cos(thetaCM);
  double term3 = Ep - Ex*(1 + beam_mass/targ_mass);
  
  return term1*term2*term3;
}

double Recoil_KE_LAB(double thetaCM, double Ep, double Ex=0.0) {

  double tau = 1.0/std::sqrt(1 - (Ex/Ep)*(1 + beam_mass/targ_mass));

  double term1 = beam_mass*targ_mass/std::pow(beam_mass + targ_mass,2);
  double term2 = 1 + tau*tau + 2*tau*std::cos(TMath::Pi() - thetaCM);
  double term3 = Ep - Ex*(1 + beam_mass/targ_mass);
  
  return term1*term2*term3;
}
//////////////////

////Positions////
//S3 segment position
TVector3 GetPos(const int det, const int ring, const int sec) {

   if(det < 0 || det > 1 || ring < 1 || ring > 24 || sec < 1 || sec > 32) {
     std::cout << "Bad det, ring, sec (" << det << "," << ring << "," << sec << ")"<< std::endl;
     return TVector3(std::sqrt(-1),std::sqrt(-1),std::sqrt(-1));
  }

  const double PI = TMath::Pi();
  
  double phi_offset = 0.5*PI; // Phi of sector 1 of downstream detector 
  bool clockwise; // Winding direction of sectors.
  if(det==0)
    clockwise = false;
  else
    clockwise = true;

  double janus_outer_radius = 3.5;
  double janus_inner_radius = 1.1;

  double rad_slope = (janus_outer_radius - janus_inner_radius)/24.;
  double rad_offset = janus_inner_radius;

  TVector3 pos(1.,0,0);
  pos.SetPerp((ring - 0.5)*rad_slope + rad_offset);
  
  double phi = phi_offset + (clockwise ? -1 : 1) * 2.*PI/32. * (sec - 1);
  pos.SetPhi(phi);

  double zoff;
  if(det == 1)
    zoff = DS_Offset;
  else
    zoff = US_Offset;
  
  pos.SetZ(zoff);
  if(det == 0)
    pos.RotateY(PI);

  return pos;
  
}

////////////Build data format////////////
struct S3 {

  //realistic info
  int det, ring, sector;
  double rEn, sEn;

  //perfect info
  TVector3 rPos, sPos;
  bool rP, rR, sP, sR;
  
};

struct GRETA {

  //realistic info
  int det, nsegs, segs[nSegs];
  double cEn, sEn[nSegs];
  
  //perfect info
  double x[nSegs], y[nSegs], z[nSegs];
  bool fep, pfep;

  int MainSeg() {

    if(!nsegs)
      return 0;
    
    int index=0;
    for(int i=1;i<nsegs;i++) {
      if(sEn[i] > sEn[index]) {
	index=i;
      }
    }
    
    return index;
    //return segs[index];
  }

  int LastSeg() {

     if(!nsegs)
      return 0;
    
    int index=0;
    for(int i=1;i<nsegs;i++) {
      if(sEn[i] < sEn[index]) {
	index=i;
      }
    }
    
    return index;
    //return segs[index];
  }

  TVector3 MainPos() {
    int index = MainSeg();
    return TVector3(x[index],y[index],z[index]);
    //return GetPos(det,MainSeg());
  }

  TVector3 LastPos() {
    int index = LastSeg();
    return TVector3(x[index],y[index],z[index]);
    //return GetPos(det,LastSeg());
  }

  TVector3 ExactMainPos() {

    if(!nsegs)
      return TVector3();
      //return GetPos(det,0);
    
    int index=0;
    for(int i=1;i<nsegs;i++) {
      if(sEn[i] > sEn[index]) {
	index=i;
      }
    }

    return TVector3(x[index],y[index],z[index]);
  }

  /*
  void AddHit(const GRETA& hit) {

    cEn += hit.cEn;
    if(!sup)
      sup = hit.sup;
    
    return;
  }
  */
  
  bool operator>(const GRETA& hit) const {
    return cEn > hit.cEn;
  }
  
};

/*
bool AddbackCriterion(GRETA& hit1,GRETA& hit2) {

  double res = (hit1.LastPos() - hit2.MainPos()).Mag();

  // In clover core separation 54.2564, 76.7367
  // Between clovers core separation 74.2400 91.9550 (high-eff mode)
  double seperation_limit = 93.0/10.0;

  int one_seg = hit1.LastSeg();
  int two_seg = hit2.MainSeg();

  // front segment to front segment OR back segment to back segment
  if((one_seg < 5 && two_seg < 5) || (one_seg > 4 && two_seg > 4))
    seperation_limit = 54.0/10.0;

  // front to back
  else if((one_seg < 5 && two_seg > 4) || (one_seg > 4 && two_seg < 5))
    seperation_limit = 105.0/10.0;
  
  if(res < seperation_limit)
    return true;

  return false;
}
*/

struct BuiltData {

  int evt;
  int nS3;
  int nGr;
  
  S3 s3[5];
  std::vector<GRETA> greta;
  std::vector<GRETA> gretaAB;

  void MakeS3Hit(const S3Data ringHit, const S3Data secHit) {

    s3[nS3].det = ringHit.det; //either hit works
    s3[nS3].ring = ringHit.ring;
    s3[nS3].sector = secHit.sector;
    
    s3[nS3].rEn = ringHit.en;
    s3[nS3].sEn = secHit.en;

    s3[nS3].rPos = TVector3(ringHit.x,ringHit.y,ringHit.z);
    s3[nS3].sPos = TVector3(secHit.x,secHit.y,secHit.z);
    
    s3[nS3].rP = ringHit.proj;
    s3[nS3].rR = ringHit.rec;

    s3[nS3].sP = secHit.proj;
    s3[nS3].sR = secHit.rec;

    nS3++;
  }

  /*
  void BuildAddbackHits() {
    
    if(nGr == 0)
      return;

    gretaAB.push_back(greta[0]);
    if(nGr == 1)
      return;
      
    int i;
    unsigned int j;
    for(i=1;i<nGr;i++) {
      
      for(j=0;j<gretaAB.size();j++) {
	if(AddbackCriterion(gretaAB[j],greta[i])) {

	  gretaAB[j].AddHit(greta[i]);
	  break;
	}
      }
      
      // if hit[i] was not added to a higher energy hit, create its own addback hit
      if(j == gretaAB.size())
	gretaAB.push_back(greta[i]);
      
    }

    return;
  }
  */
  
};
/////////////////////////////////////////

//Unpack raw data into correlated data
BuiltData BuildData(const int nE, const int nSch, const int nGch, const RawData& raw_dat) {
  
  BuiltData data;
  data.evt = nE;
  data.nS3 = 0;
  data.nGr = 0;

  //Correlate S3 Data
  std::vector<S3Data> rings;
  std::vector<S3Data> sectors;
  for(int i=0;i<nSch;i++) {

    S3Data sChan(raw_dat.sData[i]);
    
    if(sChan.IsRing())
      rings.push_back(sChan);
    else
      sectors.push_back(sChan);
    
  }

  std::sort(rings.begin(),rings.end(),std::greater<S3Data>());
  std::sort(sectors.begin(),sectors.end(),std::greater<S3Data>());

  std::vector<bool> used_rings;
  std::vector<bool> used_sectors;
  
  used_rings.resize(rings.size());
  std::fill(used_rings.begin(),used_rings.end(),false);

  used_sectors.resize(sectors.size());
  std::fill(used_sectors.begin(),used_sectors.end(),false);

  for(unsigned int i=0;i<sectors.size();i++) {
    if(used_sectors.at(i))
      continue;
    
    for(unsigned int j=0;j<rings.size();j++) {
      if(used_rings.at(j))
        continue;

      //Same detector
      if((sectors.at(i).det == rings.at(j).det) &&
	 
	 //Same energy
	 (TMath::Abs(sectors.at(i).en - rings.at(j).en) < 1.)) {
	
	data.MakeS3Hit(rings.at(j),sectors.at(i));
	
	used_sectors.at(i) = true;
	used_rings.at(j) = true;
	break;

      }
    }
  }

  bool broken = false;
  for(unsigned int i=0;i<sectors.size();i++) {
    broken=false;
    if(used_sectors.at(i))
      continue;
    
    for(unsigned int j=0;j<rings.size();j++) {
      if(used_rings.at(j))
        continue;

      for(unsigned int k=0;k<sectors.size();k++) {
	if(used_sectors.at(k))
	  continue;

	//Same detector
	if((sectors.at(i).det == rings.at(j).det) && (sectors.at(k).det == rings.at(j).det) &&

	   //Sector energies add to ring energy
	   (TMath::Abs(sectors.at(i).en + sectors.at(k).en - rings.at(j).en) < 1.)) {

	  data.MakeS3Hit(rings.at(j),sectors.at(i));
	  data.MakeS3Hit(rings.at(j),sectors.at(k));
	  
	  used_sectors.at(i) = true;
	  used_rings.at(j) = true;
	  used_sectors.at(k) = true;
	  broken = true;
	  break;

	}
      }
      if(broken)
	break;
    }
  }

  //Organize Greta data
  std::vector<bool> exists;
  exists.resize(nDets);
  std::fill(exists.begin(),exists.end(),false);
  
  int nG = 0;
  for(int i=0;i<nGch;i++) {

    int detect = raw_dat.gData[i].det;
    int segment = raw_dat.gData[i].seg;
    
    double energy = raw_dat.gData[i].en;
    double x = raw_dat.gData[i].x;
    double y = raw_dat.gData[i].y;
    double z = raw_dat.gData[i].z;
    
    bool FEP = raw_dat.gData[i].fep;
    bool PFEP = raw_dat.gData[i].pfep;
    
    if(!exists.at(detect-1)) {
      data.greta.emplace_back();
      data.greta[nG].det = detect;
      
      if((bool)segment) {
        data.greta[nG].nsegs = 1;
	data.greta[nG].segs[0] = segment;
	data.greta[nG].sEn[0] = energy;

	data.greta[nG].x[0] = x;
        data.greta[nG].y[0] = y;
        data.greta[nG].z[0] = z;
      }
      else {
	data.greta[nG].nsegs = 0;
	data.greta[nG].cEn = energy;
	data.greta[nG].fep = FEP;
	data.greta[nG].pfep = PFEP;
      }

      nG++;
      exists.at(detect-1) = true;
    }
    else {

      int index = 0;
      for(int j=0;j<nG;j++) {
	if(data.greta[j].det == detect) {
	  index = j;
	  break;
	}
      }
      
      int Nsegs = data.greta[index].nsegs;
      
      if((bool)segment) {
	data.greta[index].segs[Nsegs] = segment;
	data.greta[index].sEn[Nsegs] = energy;

	data.greta[index].x[Nsegs] = x;
        data.greta[index].y[Nsegs] = y;
        data.greta[index].z[Nsegs] = z;
	
	data.greta[index].nsegs++;
      }
      else {
	data.greta[index].cEn = energy;
	data.greta[index].fep = FEP;
	data.greta[index].pfep = PFEP;
      }
      
    }
    
  }
  data.nGr = nG;
  
  std::sort(data.greta.begin(),data.greta.end(),std::greater<GRETA>());
  //data.BuildAddbackHits();
  
  return data;
}

struct BuiltDataX {

  int evt;
  int nCh;
  int nGr;
  
  ChicoXData chico[2];
  std::vector<GRETA> greta;
  
};

BuiltDataX BuildDataX(const int nE, const int nCch, const int nGch, const RawDataX& raw_dat) {
  
  BuiltDataX data;
  data.evt = nE;
  data.nCh = 0;
  data.nGr = 0;

  //Just move raw ChicoX data into BuiltDataX container for now
  //Can implement something fancier later
  int nC = 0;
  for(int i=0;i<nCch;i++) {
    data.chico[i].id = raw_dat.cData[i].id;
    data.chico[i].en = raw_dat.cData[i].en;
    data.chico[i].tm = raw_dat.cData[i].tm;
    data.chico[i].x = raw_dat.cData[i].x;
    data.chico[i].y = raw_dat.cData[i].y;
    data.chico[i].z = raw_dat.cData[i].z;
    data.chico[i].proj = raw_dat.cData[i].proj;
    data.chico[i].rec = raw_dat.cData[i].rec;
    nC++;
  }
  data.nCh = nC;

  //Organize Greta data
  std::vector<bool> exists;
  exists.resize(nDets);
  std::fill(exists.begin(),exists.end(),false);
  
  int nG = 0;
  for(int i=0;i<nGch;i++) {

    int detect = raw_dat.gData[i].det;
    int segment = raw_dat.gData[i].seg;
    
    double energy = raw_dat.gData[i].en;
    double x = raw_dat.gData[i].x;
    double y = raw_dat.gData[i].y;
    double z = raw_dat.gData[i].z;
    
    bool FEP = raw_dat.gData[i].fep;
    bool PFEP = raw_dat.gData[i].pfep;
    
    if(!exists.at(detect-1)) {
      data.greta.emplace_back();
      data.greta[nG].det = detect;
      
      if((bool)segment) {
        data.greta[nG].nsegs = 1;
	data.greta[nG].segs[0] = segment;
	data.greta[nG].sEn[0] = energy;

	data.greta[nG].x[0] = x;
        data.greta[nG].y[0] = y;
        data.greta[nG].z[0] = z;
      }
      else {
	data.greta[nG].nsegs = 0;
	data.greta[nG].cEn = energy;
	data.greta[nG].fep = FEP;
	data.greta[nG].pfep = PFEP;
      }

      nG++;
      exists.at(detect-1) = true;
    }
    else {

      int index = 0;
      for(int j=0;j<nG;j++) {
	if(data.greta[j].det == detect) {
	  index = j;
	  break;
	}
      }
      
      int Nsegs = data.greta[index].nsegs;
      
      if((bool)segment) {
	data.greta[index].segs[Nsegs] = segment;
	data.greta[index].sEn[Nsegs] = energy;

	data.greta[index].x[Nsegs] = x;
        data.greta[index].y[Nsegs] = y;
        data.greta[index].z[Nsegs] = z;
	
	data.greta[index].nsegs++;
      }
      else {
	data.greta[index].cEn = energy;
	data.greta[index].fep = FEP;
	data.greta[index].pfep = PFEP;
      }
      
    }
    
  }
  data.nGr = nG;
  
  std::sort(data.greta.begin(),data.greta.end(),std::greater<GRETA>());
  //data.BuildAddbackHits();

  return data;
}
