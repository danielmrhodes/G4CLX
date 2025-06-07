#ifndef Physics_Constructor_h
#define Physics_Constructor_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Physics_Constructor : public G4VPhysicsConstructor
{
public:

  explicit Physics_Constructor(G4int ver=1, const G4String& name = "");
  ~Physics_Constructor() override;

  void ConstructParticle() override;
  void ConstructProcess() override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
