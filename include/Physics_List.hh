#ifndef Physics_List_h
#define Physics_List_h 1

#include "G4VModularPhysicsList.hh"

class Physics_List : public G4VModularPhysicsList {

public:
   
  Physics_List();
  ~Physics_List();

  virtual void ConstructProcess();
  virtual void ConstructParticle();

};

#endif
