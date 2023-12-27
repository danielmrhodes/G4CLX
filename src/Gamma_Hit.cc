#include "Gamma_Hit.hh"

G4ThreadLocal G4Allocator<Gamma_Hit>* Gamma_Hit_Allocator = 0;

void Gamma_Hit::SetDetSeg(G4int id) {

  seg = id%100;
  det = (id-seg)/100;
  
  return;
  
}
