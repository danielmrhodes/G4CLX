#include "Gamma_Hit.hh"

G4ThreadLocal G4Allocator<Gamma_Hit>* Gamma_Hit_Allocator = 0;

Gamma_Hit::Gamma_Hit() : fep(false), pfep(false) {;}
Gamma_Hit::~Gamma_Hit() {;}
