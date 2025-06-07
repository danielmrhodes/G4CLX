#include "Physics_List.hh"
//#include "EmStandardPhysics_option4_Atima.hh"
#include "Physics_Constructor.hh"

Physics_List::Physics_List() : G4VModularPhysicsList() {;}
Physics_List::~Physics_List() {;}

void Physics_List::ConstructProcess() {
  
  AddTransportation();
  //EmStandardPhysics_option4_Atima emPhysicsList;
  //emPhysicsList.ConstructProcess();  

  Physics_Constructor phys;
  phys.ConstructProcess();

  return;
}

void Physics_List::ConstructParticle() {
  
  //EmStandardPhysics_option4_Atima emPhysicsList;
  //emPhysicsList.ConstructParticle();

  Physics_Constructor phys;
  phys.ConstructParticle();
  
  return;
  
}
