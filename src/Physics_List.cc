#include "Physics_List.hh"
#include "Physics_Constructor.hh"

Physics_List::Physics_List() : G4VModularPhysicsList() {;}
Physics_List::~Physics_List() {;}

void Physics_List::ConstructProcess() {
  
  AddTransportation();

  Physics_Constructor emPhysicsList;
  emPhysicsList.ConstructProcess();
  
  return;
}

void Physics_List::ConstructParticle() {
  
  Physics_Constructor emPhysicsList;
  emPhysicsList.ConstructParticle(); 
  
  return;
  
}
