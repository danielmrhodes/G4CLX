#ifndef ChicoX_h
#define ChicoX_h 1

#include "G4LogicalVolume.hh"

class ChicoX {

public:

  ChicoX();
  ~ChicoX();

  //void MakeG10();
  void Placement(G4LogicalVolume* world,  G4bool check = false);
  //private:
};

#endif
