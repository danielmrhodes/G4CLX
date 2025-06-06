#include "PPAC_Hit.hh"

G4ThreadLocal G4Allocator<PPAC_Hit>* PPAC_Hit_Allocator = 0;

PPAC_Hit::PPAC_Hit() {
  
  proj = false;
  rec = false;

}

PPAC_Hit::~PPAC_Hit() {}
