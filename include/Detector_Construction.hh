#ifndef Detector_Construction_h
#define Detector_Construction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "Detector_Construction_Messenger.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"

class Detector_Construction_Messenger;    
class Detector_Construction : public G4VUserDetectorConstruction {
  
public:
   
  Detector_Construction();
  ~Detector_Construction();
  
  G4VPhysicalVolume* Construct();

  void PlaceSeGA();
  void PlaceS3();
  void PlaceTarget();

  void CheckOverlaps() {check = true;}
  void SetPlaceSeGA() {place_sega = true;}
  void SetPlaceS3() {place_s3 = true;}
  void SetPlaceTarget() {place_target = true;} 
  
  void SetUS_Offset(G4double off) {US_Offset = off;}
  void SetDS_Offset(G4double off) {DS_Offset = off;}
  
  void SetTargetZ(G4int Z)  {target_Z = Z;}
  void SetTargetN(G4int N)  {target_N = N;}
  void SetTargetDensity(G4double dens)  {target_density = dens;}
  void SetTargetMass(G4double mass)  {target_mass = mass;}
  void SetTargetThickness(G4double thick)  {target_thickness = thick;}
  void SetTargetRadius(G4double r)  {target_radius = r;}
  void SetTargetStepSize(G4double step) {target_step = step;}

  void SetTarget(G4String target);
  void PrintTarget();
  
  G4Material* GetTargetMaterial() {return target_mat;}
  G4double GetTargetThickness() {return target_thickness;}

  G4double GetUS_Offset() {return US_Offset;}
  G4double GetDS_Offset() {return DS_Offset;}
  
private:

  void ConstructSDandField();
  
  void DefineSuppressedParameters();
  void DefineMaterials();

  Detector_Construction_Messenger* messenger;
  G4LogicalVolume* logic_world;
  G4VPhysicalVolume* world;

  //SeGA Z-offset
  G4double SeGA_Offset;
  
  //Bambino Z-offsest
  G4double US_Offset;
  G4double DS_Offset;
  
  //Target parameters
  G4int target_Z;
  G4int target_N;
  G4double target_density;
  G4double target_mass;
  G4double target_thickness;
  G4double target_radius;
  G4Material* target_mat;
  G4double target_step;

  G4bool place_sega;
  G4bool place_s3;
  G4bool place_target;

  G4bool check; //surface overlap check
  
};

#endif
