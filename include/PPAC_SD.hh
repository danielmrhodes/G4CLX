#ifndef PPAC_SD_h
#define PPAC_SD_h 1

#include "G4VSensitiveDetector.hh"
#include "PPAC_Hit.hh"

class PPAC_SD : public G4VSensitiveDetector {
  
public:

  PPAC_SD(G4String name);
  ~PPAC_SD();

  G4bool ProcessHits(G4Step* step, G4TouchableHistory*);
  void Initialize(G4HCofThisEvent*);
  void EndOfEvent(G4HCofThisEvent*);

  void SetProjectileName(G4String name) {proj_name = name;}
  void SetRecoilName(G4String name) {recoil_name = name;}
  
private:

  G4String proj_name;
  G4String recoil_name;

  void ConsolidateHits();
  
  PPAC_Hit_Collection* HC;

};

#endif
