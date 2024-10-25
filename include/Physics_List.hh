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


/*
#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"

class Physics_List_Constructor : public G4VPhysicsConstructor {

public:

  explicit Physics_List_Constructor(G4int ver=1, const G4String& name="");
  ~Physics_List_Constructor() override;
  
  void ConstructParticle() override;
  void ConstructProcess() override;

private:
  G4int verbose;

};

class Physics_List : public G4VModularPhysicsList {

public:

  Physics_List();
  ~Physics_List();
  
  void ConstructParticle();
  void ConstructProcess();

private:
  G4VPhysicsConstructor* fEmPhysicsList;

};
*/

#endif
