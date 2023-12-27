#ifndef DATA_FORMAT_h
#define DATA_FORMAT_h 1

#include <cstddef>

struct INFO { //Diagnostic info
  
  int evtNum, indexP, indexR;
  double beamEn, thetaCM;
  bool projDS, projUS, rec;

  size_t bytes() {
    return 3*sizeof(int) + 2*sizeof(double) + 3*sizeof(bool);
  }
  
}__attribute__((__packed__));

struct Header {
  
  int evtNum, nSdata, nGdata;

  size_t bytes() {
    return 3*sizeof(int);
  }
  
}__attribute__((__packed__));

struct SeGAData {

  int det, seg;
  double en, x, y, z;
  bool fep, pfep;
  
};

struct S3Data {

  int det, ring, sector;
  double en, x, y, z;
  bool proj, rec;

  bool IsRing() const {return (bool)ring;}

  bool operator>(const S3Data& rhs) const { return en > rhs.en; }
  
};

struct RawData { //Simulated data

  S3Data sData[5];
  SeGAData gData[100];

}__attribute__((__packed__));

#endif
